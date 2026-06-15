# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用C++ Tensor编程API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**。

AI Core包含Vector计算单元（部署于AIV核）和Cube计算单元（部署于AIC核）。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer存储左矩阵，L0B Buffer存储右矩阵，L0C Buffer存储初始化累加值和矩阵计算结果。

## 矩阵编程的基本步骤

Cube矩阵计算的基本步骤为：数据搬入 → 数据加载 → 计算 → 数据搬出。当我们描述一个常见的Cube矩阵计算时，需要执行以下四个步骤：

1. 通过DMA将数据从`Global Memory`搬入到`Local Memory(L1 Buffer)`
2. 通过数据加载将`L1 Buffer`搬入到`L0A Buffer & L0B Buffer`
3. 进行矩阵计算，计算结果存储在`L0C Buffer`中
4. 通过DMA将数据从`L0C Buffer`搬出到`Global Memory`

## 矩阵计算内存管理

### 矩阵计算内存申请

矩阵计算编程主要通过L0 Buffer进行矩阵计算。开发者需将输入数据从Global Memory搬入L1 Buffer，再从L1 Buffer搬入L0 Buffer，最终通过Cube计算指令完成计算。L1 Buffer与L0 Buffer均通过LocalTensor对象进行管理，开发者可通过LocalMemAllocator分配器申请矩阵计算所需的内存空间。

```cpp
__global__ __cube__ void add_custom(...)
{
  constexpr uint32_t m_size = 128;
  constexpr uint32_t n_size = 128;
  constexpr uint32_t k_size = 256;
  
  AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
  AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
  AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0bAllocator;
  AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0cAllocator;
  
  // Allocate L1A/B Buffer
  LocalTensor<half> l1a_buffer = l1Allocator.Alloc<half, m_size * k_size>();
  LocalTensor<half> l1b_buffer = l1Allocator.Alloc<half, n_size * k_size>();
  // Allocate L0A/B/C Buffer
  LocalTensor<half> l0a_buffer = l0aAllocator.Alloc<half, m_size * k_size>();
  LocalTensor<half> l0b_buffer = l0bAllocator.Alloc<half, n_size * k_size>();
  LocalTensor<float> l0c_buffer = l0cAllocator.Alloc<float, m_size * n_size>();
  
  // ... Use Buffer for matrix computation
}
```

### 矩阵计算内存布局

矩阵计算单元采用分块计算逻辑，其中16×16是矩阵计算单元（fractal）的基本粒度，硬件可以并行处理多个fractal。在传统的线性存储布局（如行主序或列主序）中，数据按行或列连续存放。当需要读取一个16×16的矩形数据块时，若直接从内存中获取，则需访问不连续的多个内存地址以逐行"收集"数据，这种非连续访问会导致内存效率显著下降。
为解决上述问题，引入了矩阵分形存储格式。该格式通过软件层对数据进行重排，使每个16×16计算块在物理内存中连续存放。硬件仅需发起一次连续读取，即可将整个数据块加载至片上寄存器，避免了多地址寻址开销，从而大幅提升矩阵计算单元的数据吞吐率。**16×16是矩阵计算单元的基本分形粒度，适用于所有支持Cube计算的产品型号（Atlas A2/A3训练和推理系列产品、Ascend 950PR/Ascend 950DT）。**
昇腾AI处理器的矩阵计算单元为追求极高的数据吞吐率与计算密度，采用了专有的硬件架构设计。与通用CPU常用的连续线性存储不同，该单元要求输入输出数据必须符合特定的分形存储格式。在使用Ascend C的Mmad类接口进行算子开发时，深入理解这些数据格式对于实现正确的数据搬运、分块及性能优化至关重要。
为便于理解与记忆，文档采用"大Y小x"的直观命名法：
 - 大Y（Z/N）：表示分形矩阵之间的排列顺序（Z为行主序，N为列主序）。
 - 小x（z/n）：表示分形矩阵内部元素的排列顺序（z为行主序，n为列主序）。

### 关键分形格式详解

矩阵分形存储格式是对多维Tensor在内存中数据排布格式的描述。
为了适配矩阵计算单元的物理读取路径，不同的产品形态有不同的格式要求：
针对Atlas A2/A3训练和推理系列产品，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Zz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式。如图1所示：

<img src="../../../../figures/矩阵乘法场景设计的数据格式.png" alt="矩阵乘法场景设计的数据格式" width="800" />

针对Ascend 950PR/Ascend 950DT，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Nz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式，如图2所示：

<img src="../../../../figures/矩阵乘法场景涉及的数据格式.png" alt="矩阵乘法场景涉及的数据格式" width="800" />

#### ND 格式(N-Dimension)

ND格式是通用的N维张量格式，数据在内存中连续线性存放，通常存在于`Global Memory (GM)`中。在进行矩阵运算前，通常需要将其转换为Nz格式以适配Cube计算单元。

#### Nz 格式

Nz格式主要用于L1 Buffer和L0C Buffer中存放数据。针对Ascend 950PR/Ascend 950DT，L0A Buffer也采用Nz分形格式。该格式采用大N（外部列主序）+ 小z（内部行主序）的方式排布。

<img src="../../../../figures/Nz数据排布格式.png" alt="Nz数据排布格式" width="400" />

Nz是Ascend架构的中间格式，介于线性数据(ND)和计算专用数据(Zz/Zn)之间，便于格式转换。大N排布（列主序Block）的设计通常是为了配合多核并行或后续算子（如Vector向量计算）在处理通道（Channel）维度数据时的便利性。

> 📌 Nz格式在不同Buffer中的应用场景：
> - **L0C Buffer**: 存储矩阵乘法结果。分形形状为16×16（256元素），适合Cube计算单元高效运算。
> - **L1 Buffer**: 存储中间数据。分形形状为16×(32B/sizeof(DataType))，大小512字节，便于转换为Zz和Zn格式。
> - **L0A Buffer（仅Ascend 950PR/Ascend 950DT）**: 存储左矩阵。

#### Zz 格式

针对Atlas A2/A3训练和推理系列产品，L0A Buffer使用此格式（Ascend 950PR/Ascend 950DT使用Nz格式）。该格式采用大Z（外部行主序）+ 小z（内部行主序）。

<img src="../../../../figures/Zz格式-以half类型为例.png" alt="Zz 格式(以half类型为例)" width="400" />

在矩阵乘法中，A矩阵逐行读取数据。Zz格式的行主序特性使同一行元素物理地址连续，与计算访问模式匹配，便于Cube单元左侧通道通过DMA连续读取，避免跨步访问，提升数据搬运效率。

#### Zn 格式

Zn格式主要用于L0B Buffer中存放数据，如果离线处理好的右矩阵数据直接为Zn格式，则在`Global Memory`和`L1 Buffer`上存储该格式。该格式采用大Z(外部行主序) + 小n(内部列主序)。

<img src="../../../../figures/Zn格式-以half类型为例.png" alt="Zn 格式(以half类型为例)" width="400" />

矩阵乘法需读取B矩阵列向量。普通行主序存储导致列数据地址跳跃。Zn格式在分形内部按列存储，使列数据在L0B中紧凑排列，便于Cube单元右侧通道线性读取完整列向量，避免跨步访问。

### 格式转换

根据上述信息，Global Memory采用ND格式，L1 Buffer采用Nz格式，因此需进行ND2NZ格式转换。

<img src="../../../../figures/ND2NZ格式转换.png" alt="ND2NZ格式转换" width="800" />

以上图为例，图中展示了一个具体的 M=40, K=56 的矩阵（数据类型为 half），在从 Global Memory (GM) 搬运至 L1 Buffer 并转换为 Nz 格式时的内存排布变化：
- 原始数据 (ND 格式)：
  左侧 GM 中存储的是原始矩阵，shape为 40 x 56。
  数据呈线性连续排布（图中长箭头所示），遵循行主序（Row Major）规则。
- 对齐与填充 (Padding)：
  由于 Cube 单元的分形（Fractal）要求，目标矩阵必须能够被 16 x 16 的 Block 整除（假设数据类型为 half，即 32/sizeof(half)=16）。
  M 轴填充： 原始 M=40 不能被 16 整除，需向上补齐至 48（16 x 3）。
  K 轴填充： 原始 K=56 不能被 16 整除，需向上补齐至 64（16 x 4）。
  图中灰色区域即为补齐产生的无效数据（Padding），白色区域为原始有效数据。
- 重排布 (Nz 格式)：
  右侧 L1 Buffer 中，数据被重组为 3 x 4 个分形块。
  Z 字流动： 每个 16 x 16 的小方块内部，以及方块之间，均按照特定的 Z 字形顺序存储（如图中折线箭头所示），完成了从"线性"到"分形"的物理映射，做好了进入 Cube 计算的准备。

## 基础Tensor编程范式

基础Tensor仅封装数据指针与大小信息，开发者需手动配置搬运参数、计算分形步长、管理内存布局。适用于需要精细控制硬件行为的场景。

### 矩阵数据搬入

矩阵搬入接口实现Global Memory到L0A/L0B Buffer的数据传输，通常与Mmad接口配合使用。该接口支持多种搬运模式：二维搬运、三维格式转换、压缩数据解压、矩阵转置、稀疏化数据处理等。

开发者通过配置参数控制搬运路径、起始位置、长度等信息，充分利用硬件能力，最大化算子性能。每种数据通路对应特定的存储层级和访问特性。

结合上述数据流和分形知识，C++ Tensor编程提供以下矩阵搬入能力（完整接口见API参考手册）：

| 数据通路 | 功能    | C++ Tensor编程接口  | 分形支持情况 |
|---------|---------|---------------|-------------|
| Global Memory → L1 Buffer   | 随路转换ND2NZ搬运  | `DataCopy(dst, src, nd2nzParams)` | ![ND->Nz](../../../../figures/ND-Nz.png) |
| L1 Buffer → L0A Buffer  | 将L1 Buffer数据搬运到L0A Buffer中  | `DataCopy(dst, src, l12l0Params)` | ![Nz->Zz](../../../../figures/Nz-Zz.png) Ascend 950PR/Ascend 950DT上L0A分形为Nz, 搬运时为Nz-Nz |
| L1 Buffer → L0B Buffer  | 将L1 Buffer数据搬运到L0B Buffer中  | `DataCopy(dst, src, l12l0Params)` | ![Nz->Zn](../../../../figures/Nz-Zn.png) |

#### GM2L1 随路转换ND2NZ搬运

该能力主要实现将矩阵从`Global Memory`搬运至`L1 Buffer`，并支持在数据搬运时进行ND到Nz格式的转换。

该能力通过`DataCopy`接口配合`Nd2NzParams`参数实现，配置`nd_num`、`src_d_value`等参数完成GM2L1搬运和ND2NZ格式转换。

<img src="../../../../figures/GM到L1的ND2NZ搬运示例.png" alt="GM到L1的ND2NZ搬运示例" width="700" />

以上图为例，实现A矩阵(Shape(16, 23)，数据类型为half)从GM搬运到L1的能力，针对Atlas A2/A3训练和推理系列产品，调用接口如下：

```cpp
// Allocate L1 Buffer space
constexpr uint64_t m = 16;
constexpr uint64_t k = 32;
AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
AscendC::LocalTensor<half> dst = l1Allocator.Alloc<half, m * k>();

AscendC::GlobalTensor<half> srcGlobal;
srcGlobal.SetGlobalBuffer((__gm__ half*)src);

// Set parameters for Nd2NzParams
AscendC::Nd2NzParams intriParams{1, 16, 23, 0, 32, 32, 1, 0};

// Global Memory -> Local Memory with ND2NZ conversion
AscendC::DataCopy(dst, srcGlobal, intriParams);
```

#### L12L0 数据搬运

该能力主要实现将矩阵从L1 Buffer搬运至L0A/L0B Buffer并支持从Nz分形转换到Zz或Zn格式，C++ Tensor编程提供了`DataCopy`接口配合`L12L0Params`参数，其中针对Ascend 950PR/Ascend 950DT的L12L0A搬运可将L1 Buffer中512B大小的矩阵搬运到L0A Buffer，支持2D搬运、转置搬运、3D搬运等模式。

<img src="../../../../figures/L12L0A非转置搬运.png" alt="L12L0A非转置搬运" width="700" />

通过如下方式调用可完成上述的`L1 Buffer`到`L0A Buffer`的搬入操作。

```cpp
AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
AscendC::LocalTensor<half> l1_buffer = l1Allocator.Alloc<half, 6400>();
AscendC::LocalTensor<half> l0a_buffer = l0aAllocator.Alloc<half, 2304>();

// m_step is 2, k_step is 3, indicating transfer of 6 data blocks
uint8_t m_step = 2;
uint8_t k_step = 3;
// mStartPosition is 2, indicates M direction transfer starting position
uint16_t m_start_position = 2;
// kStartPosition is 2, indicates K direction transfer starting position
uint16_t k_start_position = 2;
// srcStride is 5, indicates interval between fractal starting addresses in K direction
int16_t src_stride = 5;
// dstStride is 3: indicates interval between fractal starting addresses in K direction
int16_t dst_stride = 3;

AscendC::L12L0Params l12l0Params{m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride};
AscendC::DataCopy(l0a_buffer, l1_buffer, l12l0Params);
```

搬运到`L0B Buffer`采用同样的`DataCopy`接口实现，接口参数及实现功能同L12L0A，可参考[cube_datamove](../../../../../api/SIMD-API/C++-API/矩阵数据搬运/)查看更多内容。

当输入A矩阵（Global Memory）是转置排布(K × M)时，搬入到`L1 Buffer`为(K × N)排布的Nz分形。要使`L0A Buffer`存放(M × K)的Zz或Nz格式（Ascend 950PR/Ascend 950DT），需要在L12L0A时进行转置，调用带transpose参数的接口。

<img src="../../../../figures/需要转置的处理场景.png" alt="需要转置的处理场景" width="800" />

```cpp
AscendC::L12L0TransposeParams l12l0TransposeParams{m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride};
AscendC::DataCopy(l0a_buffer, l1_buffer, l12l0TransposeParams);
```

### 矩阵数据搬出

矩阵搬出接口实现L0C到Global Memory或L1 Buffer的数据传输，支持随路量化、ReLU、格式转换(NZ2ND)、通道拆分合并等操作，开发者通过配置参数控制搬运路径、位置、长度等以充分利用硬件能力，通常在Mmad接口后使用，而针对Ascend 950PR/Ascend 950DT还支持L0C到UB Buffer的传输、NZ2DN格式转换以及UB双目标模式，矩阵搬出能力如下图所示：

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/矩阵搬出流程-AtlasA2A3.png" alt="矩阵搬出流程" width="800" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/矩阵搬出流程-Ascend950PR.png" alt="矩阵搬出流程" width="800" />

矩阵搬出接口支持多种随路能力的灵活组合，以L0C到Global Memory的搬运场景为例：矩阵计算结果暂存于L0C Buffer后需搬运至GM，在此过程中可协同执行随路量化、ReLU激活、格式转换及通道拆分合并等操作。下图展示了这些随路能力的有效组合、中间数据类型及完整数据路径。例如，L0C中的F32数据可通过QF322B8_PRE随路量化得到B8数据，可选使能ReLU后经NZ2NZ/NZ2ND格式转换输出至GM。图中F32→F16和F32→BF16为数据类型转换，其余路径为随路scalar/vector量化模式；针对Ascend 950PR/Ascend 950DT，还额外支持NZ2DN格式转换。

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/L0C2GM搬运-AtlasA2A3.png" alt="L0C2GM搬运" width="800" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/L0C2GM搬运-Ascend950PR.png" alt="L0C2GM搬运" width="800" />

C++ Tensor编程提供了`DataCopy`接口配合`L0C2GMParams`参数来使能发挥芯片的各种随路能力，通过直接传入配置参数可完成随路搬运的操作。

```cpp
AscendC::L0C2GMParams l0c2gmParams{n_size, m_size, loop_dst_stride, loop_src_stride, 
    l2_cache_ctl, clip_relu_pre, unit_flag_ctrl, quant_pre, relu_pre, 
    enable_split, enable_nz2nd, quant_post, relu_post, 
    enable_clip_relu_post, eltwise_op, enable_eltwise_antq, enable_c0_pad,
    enable_broadcast, enable_nz2dn};

AscendC::DataCopy(dst_addr, src_addr, l0c2gmParams);
```

另外，对于是否开启unit_flag，nz2nd信息的设置，需配置相应参数以启用不同功能。对应的接口和处理能力如下：

```cpp
// 数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的矢量量化参数。
AscendC::SetL0C2GMConfig(relu_pre_addr, quant_pre_addr, enable_unit_flag);

// 数据搬运过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。
AscendC::SetL0C2GMNZ2ND(nd_num, src_nd_stride, dst_nd_stride);

// 对RELU_ALPHA寄存器中的值进行设置。这是一个64bit的寄存器，存储在fixpipe或cube指令中进行Scalar ReLU时使用的alpha值。
AscendC::SetL0C2GMLeakyReluAlpha(config);
```

详细说明可参考接口说明文档[Cube接口说明](../../../../../api/SIMD-API/C++-API/矩阵数据搬运/asc_copy_l0c2gm/)

### 矩阵计算

`Mmad`是Ascend C封装NPU硬件计算能力的矩阵乘加核心接口，用于全连接层、卷积层等算子开发，实现C = A × B + C的矩阵乘加计算（输入为L0A Buffer和L0B Buffer，输出为L0C Buffer），其中针对Atlas A2/A3训练和推理系列产品L0A Buffer分形为Zz、L0B Buffer分形为Zn、L0C Buffer分形为Nz，而Ascend 950PR/Ascend 950DT产品L0A Buffer分形为Nz。

![Ascend 950PR/Ascend 950DT `Mmad`计算](../../../../figures/Ascend950PR-Ascend950DT-asc_mmad计算.png)

表 矩阵计算矩阵A、B、C解释说明(Ascend 950PR/Ascend 950DT)
<table>
<thead>
<tr>
<th>矩阵计算逻辑</th>
<th>矩阵计算物理位置</th>
<th>维度</th>
<th>输入/输出数据格式</th>
<th>数据类型</th>
</tr>
</thead>
<tbody>
<tr>
<td>A</td>
<td>L0A Buffer</td>
<td>M × K</td>
<td>Nz</td>
<td rowspan="3">数据类型</td>
</tr>
<tr>
<td>B</td>
<td>L0B Buffer</td>
<td>K × N</td>
<td>Zn</td>
</tr>
<tr>
<td>C</td>
<td>L0C Buffer</td>
<td>M × N（可支持使用偏置矩阵Bias进行初始化，维度为1 × N）</td>
<td>Nz</td>
</tr>
</tbody>
</table>

C++ Tensor编程提供了一系列`Mmad`重载接口，支持不同数据类型的矩阵计算。以下代码实现上图中的矩阵乘计算。

```cpp
// Define L0A/L0B matrix sizes
constexpr uint32_t l0_m_size = 64;
constexpr uint32_t l0_k_size = 64;
constexpr uint32_t l0_n_size = 64;

AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0bAllocator;
AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0cAllocator;

AscendC::LocalTensor<half> a_matrix = l0aAllocator.Alloc<half, l0_m_size * l0_k_size>();
AscendC::LocalTensor<half> b_matrix = l0bAllocator.Alloc<half, l0_k_size * l0_n_size>();
AscendC::LocalTensor<float> c_matrix = l0cAllocator.Alloc<float, l0_m_size * l0_n_size>();

// Configure Mmad parameters
// m direction: 3 fractals, actual value = 3 × 16 = 48
constexpr uint16_t m = 48;
// n direction: 3 fractals, actual value = 3 × 16 = 48
constexpr uint16_t n = 48;
// k direction: 4 fractals, actual value = 4 × 16 = 64
constexpr uint16_t k = 64;

// Feature parameters
// unit_flag: Controls fine-grained parallelism of Mmad and Fixpipe instructions
uint8_t unit_flag = 0;
// disable_gemv: Configures whether to enable GEMV mode when M=1
bool disable_gemv = false;
// enable_cube_init: Enables C matrix initial value loading
bool enable_cube_init = false;
// enable_cube_zero_init: Enables C matrix default zero initialization
bool enable_cube_zero_init = true;

AscendC::MmadParams mmadParams{m, n, k, unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init};
AscendC::Mmad(c_matrix, a_matrix, b_matrix, mmadParams);
```

### 同步机制

AI Core内部执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行方式运行，读写同一存储资源时可能存在数据依赖关系。因此，Cube计算需通过流水同步接口确保正确执行。Cube矩阵计算主要包含四步：数据搬入（Global Memory → L1 Buffer）、数据搬入（L1 Buffer → L0A/B Buffer）、矩阵计算（L0A/B Buffer → L0C Buffer）、数据搬出（L0C Buffer → Global Memory）。四个步骤分别对应PIPE_MTE2、PIPE_MTE1、PIPE_M、PIPE_FIX流水线，需通过核内同步接口协调执行顺序，确保各步骤按序完成。

当开发者进行一个矩阵乘法计算时（Ascend 950PR/Ascend 950DT产品），开发者可以通过`Mutex::Lock`&`Mutex::Unlock`的方式完成对这四个步骤的同步。

```cpp
__global__ __cube__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint8_t mutex_id = 1;
    
    AscendC::LocalMemAllocator<AscendC::Hardware::L1> l1Allocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0A> l0aAllocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0B> l0bAllocator;
    AscendC::LocalMemAllocator<AscendC::Hardware::L0C> l0cAllocator;
    
    AscendC::LocalTensor<half> a_l1_buf = l1Allocator.Alloc<half, BLOCK_M * BLOCK_K>();
    AscendC::LocalTensor<half> b_l1_buf = l1Allocator.Alloc<half, BLOCK_K * BLOCK_N>();
    AscendC::LocalTensor<half> l0a_buf = l0aAllocator.Alloc<half, BLOCK_M * BLOCK_K>();
    AscendC::LocalTensor<half> l0b_buf = l0bAllocator.Alloc<half, BLOCK_K * BLOCK_N>();
    AscendC::LocalTensor<float> l0c_buf = l0cAllocator.Alloc<float, BLOCK_M * BLOCK_N>();
    
    AscendC::GlobalTensor<half> a_gm, b_gm;
    AscendC::GlobalTensor<float> c_gm;
    a_gm.SetGlobalBuffer((__gm__ half*)x);
    b_gm.SetGlobalBuffer((__gm__ half*)y);
    c_gm.SetGlobalBuffer((__gm__ float*)z);
    
    // Step 1: Data transfer in (Global Memory → L1 Buffer), pipeline is PIPE_MTE2
    AscendC::Mutex::Lock<PIPE_MTE2>(mutex_id);
    AscendC::Nd2NzParams nd2nzParams{1, BLOCK_M, BLOCK_K, 0, BLOCK_K, BLOCK_K, 1, 0};
    AscendC::DataCopy(a_l1_buf, a_gm, nd2nzParams);
    AscendC::Nd2NzParams nd2nzParamsB{1, BLOCK_K, BLOCK_N, 0, BLOCK_N, BLOCK_N, 1, 0};
    AscendC::DataCopy(b_l1_buf, b_gm, nd2nzParamsB);
    AscendC::Mutex::Unlock<PIPE_MTE2>(mutex_id);
    
    // Step 2: Data transfer in (L1 Buffer → L0A/B Buffer), pipeline is PIPE_MTE1
    AscendC::Mutex::Lock<PIPE_MTE1>(mutex_id);
    AscendC::L12L0Params l12l0aParams{0, 0, 4, 8, 1, 1};
    AscendC::DataCopy(l0a_buf, a_l1_buf, l12l0aParams);
    AscendC::L12L0TransposeParams l12l0bParams{0, 0, 4, 8, 1, 1};
    AscendC::DataCopy(l0b_buf, b_l1_buf, l12l0bParams);
    AscendC::Mutex::Unlock<PIPE_MTE1>(mutex_id);
    
    // Step 3: Matrix computation (L0A/B Buffer → L0C Buffer), pipeline is PIPE_M
    AscendC::Mutex::Lock<PIPE_M>(mutex_id);
    AscendC::MmadParams mmadParams{BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false};
    AscendC::Mmad(l0c_buf, l0a_buf, l0b_buf, mmadParams);
    AscendC::Mutex::Unlock<PIPE_M>(mutex_id);
    
    // Step 4: Data transfer out (L0C Buffer → Global Memory), pipeline is PIPE_FIX
    AscendC::Mutex::Lock<PIPE_FIX>(mutex_id);
    AscendC::L0C2GMParams l0c2gmParams{BLOCK_N, BLOCK_M, BLOCK_N, 16, 1, 0, 0, 0, false, true, 0, 0, false, false, 0, false, false, false, false, false};
    AscendC::DataCopy(c_gm, l0c_buf, l0c2gmParams);
    AscendC::Mutex::Unlock<PIPE_FIX>(mutex_id);
}
```

### 基于基础Tensor的矩阵计算编程示例

结合上述矩阵计算的搬运与计算能力，开发者可基于C++ Tensor编程实现矩阵类算子。具体开发实践可参考[matmul实现样例](../../../../../../examples/01_simd_cpp_api/00_introduction/02_matrix/matmul_basic_api/)。

## 扩展Tensor编程范式

Ascend 950PR/950DT在Tensor基础上引入Layout布局概念，将Shape和Stride移到编译期确定，实现类型安全和分形布局自动生成。开发者只需定义Tensor的Layout，搬运和计算接口自动推断底层参数，大幅降低矩阵算子开发难度。

> 📌 扩展Tensor(带有layout)能力从**Ascend 950PR/Ascend 950DT**开始支持。

#### Layout的层次化表述

我们使用Shape和Stride来表达Layout排布格式，Shape用于表达Layout形状，Stride则用于区分不同的排布。比如下图中的行优先和列优先排布：

- 行优先:Shape (2, 4) Stride (4, 1)
- 列优先:Shape (2, 4) Stride (1, 2)

图中每个方格中的数字表示该位置元素在内存中按顺序排列时的下标。对于相同的矩阵位置，排布方式不同时，其在内存中的顺序可能是不同的，例如，矩阵坐标 (1, 0) 在行优先和列优先的情况下，对应元素在内存中的顺序分别是4和1。

![行优先排布](../../../../figures/行优先排布.png)

![列优先排布](../../../../figures/列优先排布.png)

通常Shape或者Stride中的元素是一个单独的正整数（如上文的示例），但是遇到一些复杂的内存排布情况可能无法表达，所以采用层次化表述法，该方式中，元素也可以是一个元组。元组的第一个元素用于描述不同层次中矩阵的行数，元组的第二个元素用于描述不同层次中矩阵的列数。
例如在下图中，Layout的Shape和Stride分别为：

- Shape((2, 3), (2, 4))
- Stride((1, 4), (2, 12))

![层次化Layout表达](../../../../figures/层次化Layout表达.png)

图中展示了两层矩阵：内层矩阵为内部用灰色线包裹的矩阵，外层矩阵为将内层矩阵视为一个元素时黑色线包裹的矩阵。Shape的第一个元素描述行方向的形状，(2, 3)表示内层矩阵和外层矩阵的行数分别为2和3；Shape的第二个元素描述的是列方向上的形状，(2，4)表示内存矩阵和外层矩阵的列数分别为2和4。Stride中的每个元素与Shape中的元素对应，表示该对应维度下，相邻元素首地址在内存地址上的间隔，图片中用箭头表示了每个维度相邻元素的首地址间隔。

#### 矩阵计算中的分形表达

在基于Ascend C进行矩阵编程场景下，会用到以下几种常用的Layout，这些格式都采用如上文介绍的层次化表述法来表达，要求有内外层两层矩阵。其中行排布可以看作Z字形排布，列排布可以看作N字形排布，Layout的分形名称从前到后依次描述从外层到内层的排布顺序，比如NZLayout格式表示外层为N字形排布，内层为Z字形排布。四维Layout的具体表达方式如下：

```cpp
Layout = ((Shape): (Stride))
Shape = ((ShapeRow0, ShapeRow1), (ShapeColumn0, ShapeColumn1))
Stride = ((StrideRow0, StrideRow1), (StrideColumn0, StrideColumn1))
```

以Nz分形为例，Nz分形格式的内层分形Shape中，ShapeRow0固定为16行，ShapeColumn0固定为32Byte / sizeof(T)，内层分形的Stride中，StrideRow0固定为32Byte / sizeof(T)，StrideColumn0也固定为1，即内层分形按z字形组织，外层分形按N字形组织。ShapeRow1需满足row方向16对齐，ShapeColumn1需满足column方向32B对齐。

<img src="../../../../figures/Nz数据排布格式.png" alt="Nz数据排布格式" width="400" />

下面是一个连续的NZ Layout示例，其中C0_ELEMENT = Std::Int<32>{} / sizeof(T)。

```cpp
Layout = ((Shape) : (Stride))
Shape = ((Std::Int<16>{}, ceil_div(row, Std::Int<16>{})), (C0_ELEMENT, ceil_div(column, C0_ELEMENT)))
Stride = ((C0_ELEMENT, C0_ELEMENT * Std::Int<16>{}), (Std::Int<1>{}, C0_ELEMENT * ceil_align(row, Std::Int<16>{})))
```

#### Layout的构造

结合如上分形表达的描述, 基于Layout的能力, C++ Tensor编程提供了原始的Layout构造接口, 例如, 需要创建一个多维的内存排布的形状(128, 256, 128)时, 可通过`MakeLayout`接口直接构造。

```cpp
auto threeDimLayout = MakeLayout(MakeShape(128, 256, 128));
```

而对于一个Nz分形的排布, 也可以通过`MakeLayout`来构造一个层次化的Layout表达, 譬如, 对于外层大小为(128, 256), 数据类型为half的Nz数据而言，其可以通过`MakeLayout`构造一个四维的layout排布:
```cpp
// 定义(128, 256)的Nz分型, 其四维表达为: ((16, 128 / 16), (16, 256 / 16))
auto NzLayout = MakeLayout(MakeShape((16, 8), (16, 16)));
```

Cube矩阵计算中对于不同的内部存储都有着自己独特的分形结构，为了简化用户的编写,  C++ Tensor编程提供了`MakeFrameLayout`来简化用户编写生成如上的四维Layout定义：
```cpp
// 通过MakeFrameLayout来简化用户对于Nz分形的定义, 用户只需要感知对应存储的分形结构, 无需直接表达四维结构
auto NzLayout = MakeFrameLayout<NZLayoutPtn>(128, 256);
```

### 基于Layout的Tensor表达

扩展的Tensor即是将上述的Layout信息内置到Tensor结构中, 该Tensor是对基础Tensor的扩展实现, 其由数据指针和数据布局组成。一个Tensor对象同时描述"数据放在哪里"和"数据长什么样"。
C++ Tensor提供了`MakeTensor`函数让开发者能够简便的构造出带有Layout的扩展Tensor结构, 定义一个(shape = (128, 256), type = half)的Global Memory和一个同等大小的L1 Buffer数据块，通过结合Layout构造函数可以表达为:

```cpp
// 构造Global Memory & L1 Buffer的Layout形状 
auto globalLayout = MakeLayout(MakeShape(128, 256));
auto l1Layout = MakeFrameLayout<NZLayoutPtn>(128, 256);

// 通过指针 + Layout构造GlobalTensor & L1 LocalTensor
auto globalTensor = MakeTensor(gmPtr, globalLayout);
auto l1Tensor = MakeTensor(l1Ptr, l1Layout);
```

### 矩阵搬运和计算

基础搬运接口需手动传入`count`或`DataCopyParams`参数指定搬运长度和搬运模式。扩展Tensor携带Shape、Stride和存储位置信息，编译器自动推断搬运参数和物理通路，简化接口调用。同时提供**Atom定制接口**，开发者可基于CopyTraits自定义搬运行为（如自定义搬运模式、随路处理等），满足特殊场景定制需求。

- **GM2L1搬运**: `Copy(dst, src)`搬运矩阵数据到L1 Buffer，支持随路格式转换（ND2NZ、DN2NZ、NZ2NZ等），开发者定义源Tensor和目的Tensor的Layout，接口自动处理格式转换。
```cpp
// Directly specify Global Tensor and Local Tensor for equal size copy, interface automatically infers copy parameters
Copy(l1Tensor, globalBias);

// Specify Global Tensor offset to copy a slice from Global Tensor
Copy(l1Tensor, globalBias.Slice(MakeCoord(0, j * baseN), MakeShape(1, curN)));

// Create custom CopyAtom based on OP, CopyGM2L1 implements custom GM->L1 copy capability
auto atomCopy = MakeCopy(CopyGM2L1{}, DataCopyTraitDefault{});
Copy(atomCopy, dstTensor, srcTensor);

// Fixpipe inline quantization example
Copy(CopyAtom<CopyTraits<CopyL0C2GM, FixpipeTraitDefault>>{}, globalC, l0CTensor, quantScalar);
```

同样，对于`L1 Buffer`->`L0A Buffer`/`L0B Buffer`的搬运接口以及矩阵计算接口也只需要指定对应的操作Tensor, 开发者无需感知具体的操作细节参数, 对于矩阵计算编程, 极大的简化了开发者编写算子的难度。

### 基于扩展Tensor的矩阵计算示例

结合上述矩阵计算的搬运与计算能力，开发者可基于C++ Tensor编程实现矩阵类算子。具体开发实践可参考[matmul实现样例](../../../../../../examples/01_simd_cpp_api/00_introduction/02_matrix/matmul_tensor_api/)。
