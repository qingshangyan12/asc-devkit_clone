# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**，该能力是Ascend C最基础、最通用的矢量编程方式。

前面概述已经描述了AI Core中分别用Vector计算单元和Cube计算单元来进行矢量和矩阵类计算, 计算单元分别部署于AIV核和AIC核。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

## 矩阵计算数据流

Cube矩阵计算的主要数据流为: `CopyGM2L1`->`CopyL12L0A/B`->`Mmad`->`CopyL0C2GM`的编程计算模型。
当我们描述一个常见的Cube矩阵计算时，需要执行以下三个步骤：
1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵的GM数据搬运到L1 Buffer中。
2. 通过`asc_copy_l12l0a`接口将A、B矩阵分别搬运到L0A Buffer和L0B Buffer上准备计算。
3. 通过`asc_mmad`接口对L0A Buffer、L0B Buffer(BT Buffer)上面的数据进行矩阵计算，并输出结果到L0C Buffer上。
4. 通过`asc_copy_l0c2gm`接口将L0C Buffer的数据进行处理并搬出到GM中，该接口可以利用Fixpipe Buffer数据进行随路量化、Relu等随路操作。

## 矩阵计算分形介绍

矩阵计算单元采用分块计算逻辑，例如可并行处理16×16的矩阵分片。在传统的线性存储布局（如行主序或列主序）中，数据按行或列连续存放。当需要读取一个16×16的矩形数据块时，若直接从内存中获取，则需访问不连续的多个内存地址以逐行“收集”数据，这种非连续访问会导致内存效率显著下降。
为解决上述问题，引入了矩阵分形存储格式。该格式通过软件层对数据进行重排，使每个16×16计算块在物理内存中连续存放。硬件仅需发起一次连续读取，即可将整个数据块加载至片上寄存器，避免了多地址寻址开销，从而大幅提升矩阵计算单元的数据吞吐率。
昇腾AI处理器的矩阵计算单元为追求极高的数据吞吐率与计算密度，采用了专有的硬件架构设计。与通用CPU常用的连续线性存储不同，该单元要求输入输出数据必须符合特定的分形存储格式。在使用Ascend C的Mmad类接口进行算子开发时，深入理解这些数据格式对于实现正确的数据搬运、分块及性能优化至关重要。
为便于理解与记忆，文档采用“大Y小x”的直观命名法：
 - 大Y（Z/N）：表示分形矩阵之间的排列顺序（Z为行主序，N为列主序）。
 - 小x（z/n）：表示分形矩阵内部元素的排列顺序（z为行主序，n为列主序）。

### 关键分型格式详解
矩阵分形存储格式是对多维Tensor在内存中数据排布格式的描述。
为了适配矩阵计算单元的物理读取路径，不同的产品形态有不同的格式要求：
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品 和 Atlas A3 训练系列产品/Atlas A3 推理系列产品，矩阵乘法 C = A x B 要求：左矩阵 A 使用 Zz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式。如图1所示：
![矩阵乘法场景设计的数据格式](image-17.png)

针对Ascend 950PR/Ascend 950DT，矩阵乘法 C = A x B 要求：左矩阵 A 使用 Nz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式，如图2所示：
![矩阵乘法场景涉及的数据格式](image-18.png)

**ND 格式(N-Dimension)**

ND格式是通用的N维张量格式，即"线性格式"，数据在内存中是连续线性的，通常存在于`Global Memory(GM)`中。 在进行矩阵运算前，通常需要将其转换为NZ格式以适配Cube计算单元。

**Nz 格式**
NZ格式主要用于L1 Buffer & L0C Buffer中存放数据。针对Ascend 950PR/Ascend 950DT, L0A Buffer的存储数据分形也为Nz分形格式。该格式采用大N(外部列主序) + 小z(内部行主序)的方式排布。

![Nz数据排布格式](image-19.png)

Nz 是 Ascend 架构内部的“中间标准格式”。它是对“线性数据 (ND)”和“计算专用数据 (Zz/Zn)”的一种折中。大 N 排布（列主序 Block）的设计通常是为了配合多核并行或后续算子（如 Vector 向量计算）在处理通道（Channel）维度数据时的便利性

> 📌 通常情况下，Nz格式在L0C Buffer和L1 Buffer中分别用于不同的场景：在L0C Buffer中，Nz格式用于存储矩阵乘法的结果。其分形形状为16x16，包含256个元素，这种结构非常适合Cube计算单元进行高效的矩阵乘法运算。在L1 Buffer中，Nz格式被采用以便于将数据搬运到L0A Buffer和L0B Buffer时，能够方便地转换为对应的Zz和Zn格式。此时，分形形状为16 x (32B / sizeof(Datatype))，大小为512字节。因此，当数据从L0C Buffer搬运到L1 Buffer时，其分形大小可能会发生变化。针对Ascend 950PR/Ascend 950DT，Nz格式在L0A Buffer中用于存储矩左矩阵。

**Zz 格式**
针对Atlas A2 训练系列产品/Atlas A2 推理系列产品 和 Atlas A3 训练系列产品/Atlas A3 推理系列产品, L0A Buffer使用此格式(Ascend 950PR/Ascend 950DT L0A Buffer使用Nz数据排布格式)。该格式采用大Z（外部行主序）+ 小z（内部行主序）。

![Zz 格式(以half类型为例)](image-20.png)

在 A x B 矩阵乘法中，A 矩阵采用横向扫描模式逐行读取数据，得益于 Zz 格式的微观行存储特性，A 矩阵同一行的相邻元素在 L0A Buffer 中物理地址连续排列，这种内存布局与计算访问模式天然契合，使得 Cube 单元左侧通道可直接通过 DMA 进行高效连续读取，避免了跨步访问带来的地址跳转开销，显著提升了数据搬运效率。

**Zn 格式**
Zn格式主要用于L0B Buffer中存放数据, 如果离线处理好的右矩阵数据直接为Zn格式, 则在`Global Memory`和`L1 Buffer`上存储该格式。该格式采用大Z(外部行主序) + 小n(内部列主序)。

![Zn 格式(以half类型为例)](image-21.png)

矩阵乘法计算需要读取B矩阵的列向量，面对普通行主序存储导致的同一列数据间隔整行而产生的地址跳跃问题，Zn格式通过在分形内部强制按列存储的巧妙设计，将B矩阵逻辑列数据在L0B物理内存中紧凑排列，使Cube单元右侧通道能够通过线性读取高效获取完整列向量，彻底消除了跨步访问的效率瓶颈，实现了内存布局与计算模式的完美适配。

### 格式转换

从上面的信息可以获得, `Global Memory`的存储格式为`ND`格式, `L1 Buffer`的存储格式为`Nz`格式, 这边在搬运的时候就需要进行ND2NZ的格式转换搬运。

![ND2NZ格式转换](image-22.png)

已上图为例，图中展示了一个具体的 M=40, K=56 的矩阵（数据类型为 half），在从 Global Memory (GM) 搬运至 L1 Buffer 并转换为 Nz 格式时的内存排布变化：
- 原始数据 (ND 格式)：
  左侧 GM 中存储的是原始矩阵，shape为 40 x 56。
  数据呈线性连续排布（图中长箭头所示），遵循行主序（Row Major）规则。
- 对齐与填充 (Padding)：
  由于 Cube 单元的分形（Fractal）要求，目标矩阵必须能够被 16 x 16 的 Block 整除（假设数据类型为 half，即 32/sizeof(half)=16）。
  M 轴填充： 原始 M=40 不能被 16 整除，需向上补齐至 48（16 x 3）。
  K 轴填充： 原始 K=56 不能被 16 整除，需向上补齐至 64（16 x 4）。
  图中灰色区域即为补齐产生的无效数据（Padding），白色区域为原始有效数据。
- 重排布 (NZ 格式)：
  右侧 L1 Buffer 中，数据被重组为 3 x 4 个分形块。
  Z 字流动： 每个 16 x 16 的小方块内部，以及方块之间，均按照特定的 Z 字形顺序存储（如图中折线箭头所示），完成了从“线性”到“分形”的物理映射，做好了进入 Cube 计算的准备。

Ascend C编程中直接提供了`Global Memory`到`L1 Buffer`中的C语言编程接口，通过该接口可以直接将`Global Memory`数据搬如`L1 Buffer`并完成ND2NZ格式转换。

![GM2L1-ND2NZ搬运](image-23.png)

C语言编程提供了`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`接口来完成GM2L1的搬运，并随路完成ND2NZ的格式转换处理, 另外在Ascend 950DT/Ascend 950RP上, 新增了`asc_copy_gm2l1_dn2nz`的能力, 支持从DN2NZ的格式转换的搬运。

```c
// 搬运关于最内层循环的步长为4个字节
constexpr uint64_t loop1_src_stride = 4;
//L2 Cache采用NORMAL模式
constexpr uint8_t l2_cache_ctl = 1;
// N值为3
constexpr uint16_t n_value = 3;
// D值为3
constexpr uint32_t d_value = 3;
// 搬运关于最外层循环的步长为2个字节
constexpr uint64_t loop4_src_stride = 2;
// 每一个C0数据块都会被pad至32个字节
constexpr bool smallc0_en = true;
__gm__ half src[256];
__cbuf__ half dst[256];
asc_copy_gm2l1_nd2nz(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
```

## 矩阵搬运能力说明

矩阵计算的搬入是Ascend C编程框架中用于数据搬运的核心接口系列，主要实现`Global Memory`到`L0A Buffer/L0B Buffer`之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左右矩阵等数据的搬运需求，通常与Mmad接口配合使用。矩阵计算的搬入接口系列支持多种数据搬运场景，包括但不限于二维格式数据搬运、三维格式转换、压缩数据解压、矩阵转置搬运以及稀疏化数据处理等。通过灵活配置不同的参数结构体，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

结合上述Cube矩阵计算数据流和分形的基础介绍知识, C语言编程提供了如下的矩阵类的主要搬运能力(详细完备接口请查看API参考手册)：
| 数据通路 | 功能    | C语言编程接口  | 分行支持情况 |
|---------|---------|---------------|-------------|
| gm2l1   | 连续数据搬运  | `asc_copy_gm2l1_nd2nz` | ![ND->Nz](image-24.png) |
| l12l0a  | 高维切分数据搬运  | `asc_copy_l12l0a` | ![Nz->Zz](image-29.png) Ascend 950DT/Ascend 950RP上L0A分形为Nz, 搬运时为Nz-Nz |
| l12l0b  | 非对齐数据搬运  | `asc_copy_l12l0b` | ![Nz->Zn](image-28.png)|
| l0c2gm  | 连续数据搬运  | `asc_copy_l0c2gm` | [Nz->ND] |


## 矩阵计算能力说明
