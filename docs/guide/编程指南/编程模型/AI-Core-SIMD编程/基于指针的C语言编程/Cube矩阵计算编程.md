# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**，该能力是Ascend C最基础、最通用的矩阵编程方式。

前面概述已经描述了AI Core中分别用Vector计算单元和Cube计算单元来进行矢量和矩阵类计算，Vector计算单元部署于AIV核，Cube计算单元部署于AIC核。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

## 矩阵计算数据流

Cube矩阵计算的主要数据流遵循`CopyGM2L1`->`CopyL12L0A/B`->`Mmad`->`CopyL0C2GM`的编程计算模型。
当我们描述一个常见的Cube矩阵计算时，需要执行以下三个步骤：
1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵的GM数据搬运到L1 Buffer中。
2. 通过`asc_copy_l12l0a`接口将A、B矩阵分别搬运到L0A Buffer和L0B Buffer上准备计算。
3. 通过`asc_mmad`接口对L0A Buffer、L0B Buffer（又称BT Buffer）上面的数据进行矩阵计算，并输出结果到L0C Buffer上。
4. 通过`asc_copy_l0c2gm`接口将L0C Buffer的数据进行处理并搬出到GM中，该接口可以利用Fixpipe Buffer数据进行随路量化、Relu等随路操作。

## 矩阵计算分形介绍

矩阵计算单元采用分块计算逻辑，例如可并行处理16×16的矩阵分片。在传统的线性存储布局（如行主序或列主序）中，数据按行或列连续存放。当需要读取一个16×16的矩形数据块时，若直接从内存中获取，则需访问不连续的多个内存地址以逐行“收集”数据，这种非连续访问会导致内存效率显著下降。
为解决上述问题，引入了矩阵分形存储格式。该格式通过软件层对数据进行重排，使每个16×16计算块在物理内存中连续存放。硬件仅需发起一次连续读取，即可将整个数据块加载至片上寄存器，避免了多地址寻址开销，从而大幅提升矩阵计算单元的数据吞吐率。
昇腾AI处理器的矩阵计算单元为追求极高的数据吞吐率与计算密度，采用了专有的硬件架构设计。与通用CPU常用的连续线性存储不同，该单元要求输入输出数据必须符合特定的分形存储格式。在使用Ascend C的Mmad类接口进行算子开发时，深入理解这些数据格式对于实现正确的数据搬运、分块及性能优化至关重要。
为便于理解与记忆，文档采用“大Y小x”的直观命名法：
 - 大Y（Z/N）：表示分形矩阵之间的排列顺序（Z为行主序，N为列主序）。
- 小x（z/n）：表示分形矩阵内部元素的排列顺序（z为行主序，n为列主序）。

### 关键分形格式详解
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

以上图为例，图中展示了一个具体的 M=40, K=56 的矩阵（数据类型为 half），在从 Global Memory (GM) 搬运至 L1 Buffer 并转换为 Nz 格式时的内存排布变化：
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

Ascend C编程中直接提供了`Global Memory`到`L1 Buffer`中的C语言编程接口，通过该接口可以直接将`Global Memory`数据搬入`L1 Buffer`并完成ND2NZ格式转换。

![GM2L1-ND2NZ搬运](image-23.png)

C语言编程提供了`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`接口来完成GM2L1的搬运，并随路完成ND2NZ的格式转换处理。另外在Ascend 950DT/Ascend 950RP上，新增了`asc_copy_gm2l1_dn2nz`的能力，支持从DN2NZ的格式转换的搬运。

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

### 矩阵搬入能力

矩阵计算的搬入是Ascend C编程框架中用于数据搬运的核心接口系列，主要实现`Global Memory`到`L0A Buffer/L0B Buffer`之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左右矩阵等数据的搬运需求，通常与Mmad接口配合使用。矩阵计算的搬入接口系列支持多种数据搬运场景，包括但不限于二维格式数据搬运、三维格式转换、压缩数据解压、矩阵转置搬运以及稀疏化数据处理等。通过灵活配置不同的参数结构体，开发者可以精确控制数据的搬运路径、起始位置、搬运长度等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。矩阵计算的搬入接口支持多种数据通路，每种通路对应特定的存储层级和访问特性。

结合上述Cube矩阵计算数据流和分形的基础介绍知识, C语言编程提供了如下的矩阵类的主要矩阵搬入能力(详细完备接口请查看API参考手册)：
| 数据通路 | 功能    | C语言编程接口  | 分行支持情况 |
|---------|---------|---------------|-------------|
| gm2l1   | 随路转换ND2NZ搬运  | `asc_copy_gm2l1_nd2nz` | ![ND->Nz](image-24.png) |
| l12l0a  | 高维切分数据搬运  | `asc_copy_l12l0a` | ![Nz->Zz](image-29.png) Ascend 950DT/Ascend 950RP上L0A分形为Nz, 搬运时为Nz-Nz |
| l12l0b  | 非对齐数据搬运  | `asc_copy_l12l0b` | ![Nz->Zn](image-28.png)|

**GM2L1 随路转换ND2NZ搬运**

该能力主要实现将矩阵从`Global Memory`搬运至`L1 Buffer`，并支持在数据搬运时进行ND到NZ格式的转换。
该能力主要通过`asc_copy_gm2l1_nd2nz`实现，并通过配置`src_nd_matrix_stride` & `src_d_value`等参数实现GM2L1并实现随路ND2NZ格式转换。

![alt text](image-39.png)
以上图为例, 实现A矩阵(Shape(16, 23), 数据类型为half)从GM搬运到L1的能力, 针对Atlas A2 训练系列产品/Atlas A2 推理系列产品 和 Atlas A3 训练系列产品/Atlas A3 推理系列产品, 调用接口如下：

```c
// 分配L1 Buffer空间, m方向为16， n方向 23向上对齐为32
constexpr uint64_t m = 16;
constexpr uint64_t k = 32;
__cbuf__ half dst[m * k];

// 通过asc_set_gm2l1_nz_para接口对dst矩阵(L1 Buffer)进行参数设置
constexpr uint64_t dst_nz_matrix_stride = 0, dst_nz_c0_stride = 16, dst_nz_n_stride = 1, dst_nd_num = 1;
// 目的矩阵中，相邻NZ矩阵起始地址的偏移
constexpr uint64_t nz_config = static_cast<uint64_t>(dst_nz_matrix_stride) << 48;
// ND转换为NZ格式后，目的NZ矩阵中相邻Z矩阵起始地址的偏移
nz_config |= static_cast<uint64_t>(dst_nz_c0_stride) << 32;
nz_config |= static_cast<uint64_t>(dst_nz_n_stride) << 16;
nz_config |= static_cast<uint64_t>(dst_nd_num);
asc_set_gm2l1_nz_para(nz_config);

// 源矩阵一行中包含的元素个数
constexpr uint64_t src_d_value = 32;
// 源矩阵相邻ND矩阵起始地址间的偏移
constexpr uint64_t src_nd_matrix_stride = 0;
// L2 Cache采用NORMAL模式
constexpr uint8_t l2_cache_ctrl = 1;
// 源矩阵中ND矩阵的行数
constexpr uint16_t n_value = 16;
// 源矩阵中ND矩阵的列数
constexpr uint32_t d_value = 23;
// 当dValue小于等于4的时候，可以使能SmallC0模式，C0_SIZE会补齐到4 * sizeof(T) Bytes。在该模式下，dst_n_stride参数和dst_c0_stride参数无效，无需配置。
constexpr bool enable_small_c0 = false;
asc_copy_gm2l1_nd2nz(dst, src, src_d_value, l2_cache_ctrl, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
```

**L12L0 数据加载搬运**

该能力主要实现将矩阵从`L1 Buffer`搬运至`L0A/L0B Buffer`，并支持从Nz分型转换到Zz或者Zn格式。
C语言编程根据通路区别分别提供了`asc_copy_l12l0a` 和`asc_copy_l12l0b`的能力，下面以`L1 Buffer`搬入到`L0A Buffer`为例, 来描述该数据搬运能力。

针对Ascend 950PR/Ascend 950DT, 提供`asc_copy_l12l0a`接口实现搬运存放在`L1 Buffer`里的512B大小的矩阵到`L0A Buffer`, 包含2D格式搬运、 2D格式转置搬运、3D格式搬运等。该能力负责完成普通矩阵计算所需的2D格式数据搬运, 以大小为512Byte的数据分形为单位进行搬运，实现示例如下:

![L12L0A非转置搬运](image-36.png)

通过如下方式调用可完成上述的`L1 Buffer`到`L0A Buffer`的搬入操作。
```c
__cbuf__ half l1_buffer[6400];
__ca__ half l0a_buffer[2304];
// m_step为2, k_step为3, 表示搬运6块数据块
uint8_t m_step = 2;
uint8_t k_step = 3;
// mStartPosition为2，说明M方向搬运起始位置是在距离源操作数起始位置M轴方向16*2=64个元素的位置
uint16_t m_start_position = 2;
// kStartPosition为2，说明K方向搬运起始位置是距离源操作数起始位置K轴方向32B*2=64B位置
uint16_t k_start_position = 2;
// srcStride为5，表示源操作数中，K方向前一个分形起始地址与后一个分形起始地址的间隔为5
int16_t src_stride = 5;
// dstStride为3：表示目的操作数中，K方向前一个分形起始地址与后一个分形起始地址的间隔为3
int16_t dst_stride = 3;
asc_copy_l12l0a(l0a_buffer, l1_buffer, m_start_postion, k_start_position, m_step, k_step, src_stride, dst_stride);
```

搬运到`L0B Buffer`采用`asc_copy_l12l0b`实现，接口参数及实现功能同L12L0A, 可参考[cube_datamove](../../../../../api/SIMD-API/C-API/矩阵数据搬运/)查看更多内容。

当输入A矩阵(`Global Memory`)上是转置排布(K * M)时，搬入到`L1 Buffer`的形状如下图最左边的表达形式, 要想`L0A Buffer`中存放的是(M * K)的Zz或者Nz(Ascend 950RP/Ascend 950DT), 在L12L0A的时候需要进行随路转置, 此时需要调用`asc_copy_l12l0a_transpose`完成处理。

![需要转置的处理场景](image-37.png)
```c
// 使用transpose接口完成处理
asc_copy_l12l0a_transpose(l0a_buffer, l1_buffer, m_start_postion, k_start_position, m_step, k_step, src_stride, dst_stride);
```

### 矩阵搬出能力

矩阵计算的搬出是AscendC编程框架中用于数据搬运的核心接口系列，主要实现L0C与`Global Memory`、`L1 Buffer`之间的数据高效传输。该接口系列提供了多种数据搬运模式，能够满足矩阵计算中左右矩阵等数据的搬运需求，通常在Mmad接口后配合使用。针对Ascend 950PR/Ascend 950DT，还支持`L0C Buffer`与`UB Buffer`之间的数据传输。

矩阵计算的搬出接口支持多种随路搬运场景，包括随路量化、随路Relu、随路格式转换(NZ2ND)、随路通道拆分(channel split)、随路通道合并(channel merge)。通过灵活配置不同的参数结构体，开发者可以精确控制数据的搬运路径、起始位置、搬运长度、随路搬运能力等关键信息，充分利用硬件的数据搬运能力，最大化算子的整体执行性能。针对Ascend 950PR/Ascend 950DT，还支持NZ2DN随路格式转换和UB双目标模式。

针对AtlasA3训练系列产品/AtlasA3推理系列产品和AtlasA2训练系列产品/AtlasA2推理系列产品：

![矩阵搬出流程](image-30.png)

针对Ascend 950PR/Ascend 950DT：

![矩阵搬出流程](image-31.png)

如上所述, 矩阵计算的搬出支持多种随路量化、Relu、格式转换的能力, 以`L0C Buffer`搬出到`Global Memory`为例,  矩阵计算的结果存放在L0C Buffer, 需要将结果搬运至`Global Memory(GM)`中, 并且在搬运过程中支持随路格式转换等操作。图中的F32->F16与F32->BF16为非量化模式，仅为cast，其余为随路saclar/vector量化模式。
随路量化、随路ReLU、随路格式转换、随路通道拆分以及随路通道合并的有效组合、中间数据类型和数据路径如下图所示。例如，L0C中F32类型的数据可以通过QF322B8_PRE随路量化得到B8数据，可以按需选择是否进一步使能随路Relu, 接下来B8数据经过NZ2NZ/NZ2ND操作可以输出到GM(OUT)。针对Ascend 950PR/Ascend 950DT，还支持NZ2DN随路格式转换。图中的F32->F16与F32->BF16为数据类型转换，其余为随路scalar/vector量化模式。
针对Atlas A3 训练系列产品/Atlas A3 推理系列产品和Atlas A2 训练系列产品/Atlas A2 推理系列产品：
![L0C2GM搬运](image-32.png)

针对Ascend 950PR/Ascend 950DT：
![L0C2GM搬运](image-33.png)

矩阵搬出接口支持多种随路能力的组合，需要设置不同的寄存器，配合数据搬运接口使能不同的数据搬运能力， 对应的接口和处理能力如下：
1. l0c2gm数据搬出接口，支持配置随路实现量化和Relu激活，NZ到ND格式的转换
```c
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctrl, uint64_t quant_pre, uint8_t relu_pre, 
                        bool enable_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, 
                        bool enable_clip_relu_post, uint8_t eltwise_op, bool enable_eltwise_antq, bool enable_c0_pad,
                        bool enable_broadcast, bool enable_nz2dn);
```  

2. 通过调用该接口设置随路量化，随路pRelu参数(AscendC待实现)，其中tensor的每个元素都代表一个量化参数，使用Vector量化/pRelu模式(AscendC待实现)时需要设置
```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag);
```

3. 通过调用该接口设置随路NZ2ND格式转换配置，使用随路NZ2ND需要设置
```c
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);
```

4. 寄存器设置接口，通过调用该接口设置随路Leaky Relu系数，使用随路Leaky Relu需要设置
```c
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config);
```

详细说明可参考接口说明文档[Cube接口说明](../../../../../api/SIMD-API/C-API/矩阵数据搬运/asc_copy_l0c2gm/)

## 矩阵计算能力说明

`asc_mmad`接口是AscendC C语言编程面向昇腾 AI 芯片的矩阵乘加核心计算接口，专为高性能算子开发设计，封装了昇腾 NPU 硬件的矩阵乘加计算能力，广泛用于神经网络层（如全连接层、卷积层）、数值计算类算子的开发。
如下图所示，`asc_mmad`接口实现昇腾NPU矩阵乘计算能力，其数学表达式为：C = A X B + C, 该接口调用入参为`L0A Buffer`和`L0B Buffer`, 输出为`L0C Buffer`。在Atlas A2 训练系列产品/Atlas A2 推理系列产品 和 Atlas A3 训练系列产品/Atlas A3 推理系列产品中 `L0A Buffer`分形为Zz，`L0B Buffer`分形为Nz, `L0C Buffer`分形为Nz。
Ascend 950PR/Ascend 950DT产品`L0A Buffer`的分形为Nz。

![Ascend 950PR/Ascend 950DT `asc_mmad`计算](image-38.png)

C语言编程针对AI Core Cube计算的能力, 通过重载实现提供了一系列名为`asc_mmad`, 支持不同数据类型计算的接口能力, 可通过下面的代码实现上图中的矩阵乘计算。

```c
__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

```c
// 1. 定义L0A/L0B上面矩阵的大小
constexpr uint32_t l0_m_size = 64;
constexpr uint32_t l0_k_size = 64;
constexpr uint32_t l0_n_size = 64;
__ca__ c_matrix[l0_m_size * l0_k_size];
__cb__ b_matrix[l0_k_size * l0_n_size];
__cc__ c_matrix[l0_m_size * l0_n_size];

// 2. 配置asc_mmad的参数
// 2.1. 配置实际参与计算的矩阵大小(m, n, k)
// 如上图，m 方向为三个分形，实际计算值为3 * 16 = 48
constexpr uint16_t m = 48;
// n 方向为三个分形，实际计算值为3 * 16 = 48
constexpr uint16_t n = 48;
// k 方向为4个分形大小，实际值为4 * 16 = 64
constexpr uint16_t k = 64;

// 2.2 配置相关特性参数
// unit_flag可以控制Mmad指令和Fixpipe指令细粒度的并行，使能该功能后，硬件每计算完一个分形，计算结果就会被搬出。
uint8_t unit_flag = 0;
// M=1时，该参数用来配置Mmad计算是否开启GEMV模式。
bool disable_gemv = false;
// 配置C矩阵初始值存在初始值。false时不对L0C进行初始化操作；true时使用BT Buffer中的数据对L0C进行初始化操作。
bool enable_cube_init = false;
// 是否使能C矩阵默认初始化清零操作。true时C矩阵默认初始化为0；false时C矩阵不进行默认操作，通过设置cmatrixSource参数进行初始化。
bool enable_cube_zero_init = true;

asc_mmad(c_matrix, a_matrix, b_matrix, m, n, k, unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
```

## Cube矩阵计算示例(Ascend 950PR/Ascend 950DT)

实现一个`C(M, N) = A(M, K) * B(K, N)`的矩阵乘计算, 其中 M = 64, K = 128, N = 64, 数据类型为half, 基于C语言编程的视实现如下。
```c
extern "C" __global__ __aicore__ void matmul_kernel(__gm__ half* a_gm, __gm__ half* b_gm, __gm__ float* c_gm)
{
    // ===== 第一步：内存规划与地址分配 =====
    constexpr uint32_t BLOCK_M = 64;  // 分块M维度
    constexpr uint32_t BLOCK_K = 128;   // 分块K维度
    constexpr uint32_t BLOCK_N = 64;   // 分块N维度
    
    __cbuf__ half a_l1_buf[BLOCK_M * BLOCK_K];   // L1中转区
    __cbuf__ half b_l1_buf[BLOCK_K * BLOCK_N];   // L1中转区
    __ca__ half l0a_buf[BLOCK_M * BLOCK_K];    // L0A输入
    __cb__ half l0b_buf[BLOCK_K * BLOCK_N];    // L0B输入
    __cc__ float l0c_buf[BLOCK_M * BLOCK_N];   // L0C输出
    
// 1. 将A矩阵从GM中搬运到L1中
// 1.1 设置L1 Buffer中的参数, 其中dst_nd_num为1, 只有一块ND块，所以dst_nz_matrix_stride为0, dst_nz_c0_stride为相邻Z矩阵的起始地址，偏移为16个C0_SIZE,
// dst_nz_n_stride 表示NZ矩阵中相邻行起始地址的偏移，为1个C0_SIZE。
    constexpr uint64_t dst_nz_matrix_stride = 0, dst_nz_c0_stride = 16, dst_nz_n_stride = 1, dst_nd_num = 1;
    constexpr uint64_t nz_config = static_cast<uint64_t>(dst_nz_matrix_stride) << 48;
    nz_config |= static_cast<uint64_t>(dst_nz_c0_stride) << 32;
    nz_config |= static_cast<uint64_t>(dst_nz_n_stride) << 16;
    nz_config |= static_cast<uint64_t>(dst_nd_num);
    asc_set_gm2l1_nz_para(nz_config);
    // 1.2 调用asc_copy_gm2l1_nd2nz拷贝接口实现GM2L1的搬运操作, 其中n_value为64, d_value为128
    // src_d_value表示源矩阵一行中的元素个数，值为128。只有一块ND矩阵，src_nd_matrix_stride为0。
    asc_copy_gm2l1_nd2nz(a_l1_buf, a_gm, 128, 1, BLOCK_M, BLOCK_K, 0, false);
    
// 2. 将B矩阵从GM中搬运到L1中
// 2.1 设置L1 Buffer中的参数, 参数同A矩阵一致
asc_set_gm2l1_nz_para(nz_config);
// 2.2 调用asc_copy_gm2l1_nd2nz拷贝接口实现GM2L1的搬运操作, 其中n_value为128, d_value为64
// src_d_value表示源矩阵一行中的元素个数，值为64。只有一块ND矩阵，src_nd_matrix_stride为0。
    asc_copy_gm2l1_nd2nz(b_l1_buf, a_gm, 64, 1, BLOCK_K, BLOCK_N, 0, false);

    // 3. 插入同步，搬入L0A/B Buffer前需要等待GM搬入到L1Buffer中
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    
    // 4. 将A, B矩阵搬入到L0A/B中
    // 4.1 A矩阵从L1 Buffer搬入到L0A Buffer
    /*
     * 参数说明：
     * m_step = 4，源矩阵M轴方向搬运长度，单位为16 element。所以值为 64 / 16
     * k_step = 8, 源矩阵K轴方向搬运长度，单位为32B。所以值为 128 * sizeof(half) / 32
     * m_start_postion = 0, 源矩阵M轴方向的起始位置，单位为16个元素。从起点开始，值为0
     * k_start_position = 0, 源矩阵K轴方向的起始位置，单位为32B。从起点开始，值为0
     * src_stride = 1, 源矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位：512B。源数据内连续排布，所以只为1个分形间隔
     * dst_stride = 1, 目标矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位：512B。目标数据内连续排布，所以只为1个分形间隔
     */ 
asc_copy_l12l0a(l0a_buf, a_l1_buf, 0, 0, 4, 8, 1, 1);
// 4.2 B矩阵从L1 Buffer搬入到L0B Buffer, 参数同A矩阵, GM中矩阵排布是(K, N), 搬入到L1中是(K, N)排布的Nz分形， N方向连续，但是L0B中是Zn排布，内部是沿着K方向连续的K，所以这边需要调用asc_copy_l12l0b_transpose实现
    asc_copy_l12l0b_transpose(l0b_buf, b_l1_buf, 0, 0, 4, 8, 1, 1);
    // 5. 插入同步，进行MM计算时需要等待搬入到L0 Buffer内
    asc_sync_notify(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    
    // 6. 执行Mmad, 计算大小为(BLOCK_M, BLOCK_K, BLOCK_N)
    asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
    asc_sync_notify(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    
    // 7. 将数据搬出到GM中, 需要配置随路的搬运信息
    asc_copy_l0c2gm(c_gm, l0c_buf, BLOCK_N, BLOCK_M, BLOCK_N, 16, 1, 0,
        0, QuantMode_t::NoQuant, 0, false, true, 0, 0, false, false, 0, false, false, false, false, false, false);
}
```