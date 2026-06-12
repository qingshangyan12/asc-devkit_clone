# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**，该能力是Ascend C最基础、最通用的矩阵编程方式。

AI Core包含Vector计算单元（部署于AIV核）和Cube计算单元（部署于AIC核），分别用于矢量和矩阵类计算。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

## 1. 矩阵计算数据流

Cube矩阵计算的数据流为：`CopyGM2L1` → `CopyL12L0A/B` → `Mmad` → `CopyL0C2GM`。
当我们描述一个常见的Cube矩阵计算时，需要执行以下四个步骤：
1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵从GM搬运到L1 Buffer中。
2. 通过`asc_copy_l12l0a`和`asc_copy_l12l0b`接口分别将A、B矩阵搬运到L0A Buffer和L0B Buffer中。
3. 通过`asc_mmad`接口对L0A Buffer和L0B Buffer中的数据进行矩阵计算，结果输出到L0C Buffer。
4. 通过`asc_copy_l0c2gm`接口将L0C Buffer的结果搬运到GM中，支持随路量化、ReLU等操作。

## 2. 矩阵计算分形介绍

矩阵计算单元采用分块计算逻辑，其中16×16是矩阵计算单元（fractal）的基本粒度，硬件可以并行处理多个fractal。在传统的线性存储布局（如行主序或列主序）中，数据按行或列连续存放。当需要读取一个16×16的矩形数据块时，若直接从内存中获取，则需访问不连续的多个内存地址以逐行“收集”数据，这种非连续访问会导致内存效率显著下降。
为解决上述问题，引入了矩阵分形存储格式。该格式通过软件层对数据进行重排，使每个16×16计算块在物理内存中连续存放。硬件仅需发起一次连续读取，即可将整个数据块加载至片上寄存器，避免了多地址寻址开销，从而大幅提升矩阵计算单元的数据吞吐率。**16×16是矩阵计算单元的基本分形粒度，适用于所有支持Cube计算的产品型号（Atlas A2/A3训练和推理系列产品、Ascend 950PR/Ascend 950DT）。**
昇腾AI处理器的矩阵计算单元为追求极高的数据吞吐率与计算密度，采用了专有的硬件架构设计。与通用CPU常用的连续线性存储不同，该单元要求输入输出数据必须符合特定的分形存储格式。在使用Ascend C的Mmad类接口进行算子开发时，深入理解这些数据格式对于实现正确的数据搬运、分块及性能优化至关重要。
为便于理解与记忆，文档采用“大Y小x”的直观命名法：
 - 大Y（Z/N）：表示分形矩阵之间的排列顺序（Z为行主序，N为列主序）。
 - 小x（z/n）：表示分形矩阵内部元素的排列顺序（z为行主序，n为列主序）。

### 2.1 关键分形格式详解
矩阵分形存储格式是对多维Tensor在内存中数据排布格式的描述。
为了适配矩阵计算单元的物理读取路径，不同的产品形态有不同的格式要求：
针对Atlas A2/A3训练和推理系列产品，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Zz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式。如图1所示：
<img src="../../../../figures/矩阵乘法场景设计的数据格式.png" alt="矩阵乘法场景设计的数据格式" width="600" />

针对Ascend 950PR/Ascend 950DT，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Nz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式，如图2所示：
<img src="../../../../figures/矩阵乘法场景涉及的数据格式.png" alt="矩阵乘法场景涉及的数据格式" width="600" />

#### 2.1.1 ND 格式(N-Dimension)

ND格式是通用的N维张量格式，数据在内存中连续线性存放，通常存在于`Global Memory (GM)`中。在进行矩阵运算前，通常需要将其转换为Nz格式以适配Cube计算单元。

#### 2.1.2 Nz 格式
Nz格式主要用于L1 Buffer和L0C Buffer中存放数据。针对Ascend 950PR/Ascend 950DT，L0A Buffer也采用Nz分形格式。该格式采用大N（外部列主序）+ 小z（内部行主序）的方式排布。

<img src="../../../../figures/Nz数据排布格式.png" alt="Nz数据排布格式" width="600" />


Nz是Ascend架构的中间格式，介于线性数据(ND)和计算专用数据(Zz/Zn)之间，便于格式转换。大N排布（列主序Block）的设计通常是为了配合多核并行或后续算子（如Vector向量计算）在处理通道（Channel）维度数据时的便利性。

> 📌 Nz格式在不同Buffer中的应用场景：
> - **L0C Buffer**: 存储矩阵乘法结果。分形形状为16×16（256元素），适合Cube计算单元高效运算。
> - **L1 Buffer**: 存储中间数据。分形形状为16×(32B/sizeof(DataType))，大小512字节，便于转换为Zz和Zn格式。
> - **L0A Buffer（仅Ascend 950PR/Ascend 950DT）**: 存储左矩阵。

#### 2.1.3 Zz 格式

针对Atlas A2/A3训练和推理系列产品，L0A Buffer使用此格式（Ascend 950PR/Ascend 950DT使用Nz格式）。该格式采用大Z（外部行主序）+ 小z（内部行主序）。

<img src="../../../../figures/Zz格式-以half类型为例.png" alt="Zz 格式(以half类型为例)" width="600" />


在矩阵乘法中，A矩阵逐行读取数据。Zz格式的行主序特性使同一行元素物理地址连续，与计算访问模式匹配，便于Cube单元左侧通道通过DMA连续读取，避免跨步访问，提升数据搬运效率。

#### 2.1.4 Zn 格式

Zn格式主要用于L0B Buffer中存放数据, 如果离线处理好的右矩阵数据直接为Zn格式, 则在`Global Memory`和`L1 Buffer`上存储该格式。该格式采用大Z(外部行主序) + 小n(内部列主序)。

<img src="../../../../figures/Zn格式-以half类型为例.png" alt="Zn 格式(以half类型为例)" width="600" />


矩阵乘法需读取B矩阵列向量。普通行主序存储导致列数据地址跳跃。Zn格式在分形内部按列存储，使列数据在L0B中紧凑排列，便于Cube单元右侧通道线性读取完整列向量，避免跨步访问。

### 2.2 格式转换

根据上述信息，Global Memory采用ND格式，L1 Buffer采用Nz格式，因此需进行ND2NZ格式转换。

<img src="../../../../figures/ND2NZ格式转换.png" alt="ND2NZ格式转换" width="600" />

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
  Z 字流动： 每个 16 x 16 的小方块内部，以及方块之间，均按照特定的 Z 字形顺序存储（如图中折线箭头所示），完成了从“线性”到“分形”的物理映射，做好了进入 Cube 计算的准备。

Ascend C编程中直接提供了`Global Memory`到`L1 Buffer`中的C语言编程接口，通过该接口可以直接将`Global Memory`数据搬入`L1 Buffer`并完成ND2NZ格式转换。
C语言编程提供了`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`接口来完成GM2L1的搬运，并随路完成ND2NZ的格式转换处理。另外在Ascend 950PR/Ascend 950DT上，新增了`asc_copy_gm2l1_dn2nz`的能力，支持从DN到Nz的格式转换搬运。

## 3. 矩阵搬运能力说明

### 3.1 矩阵搬入能力

矩阵搬入接口实现Global Memory到L0A/L0B Buffer的数据传输，通常与Mmad接口配合使用。该接口支持多种搬运模式：二维搬运、三维格式转换、压缩数据解压、矩阵转置、稀疏化数据处理等。

开发者通过配置参数控制搬运路径、起始位置、长度等信息，充分利用硬件能力，最大化算子性能。每种数据通路对应特定的存储层级和访问特性。

结合上述数据流和分形知识，C语言编程提供以下矩阵搬入能力（完整接口见API参考手册）：
| 数据通路 | 功能    | C语言编程接口  | 分行支持情况 |
|---------|---------|---------------|-------------|
| gm2l1   | 随路转换ND2NZ搬运  | `asc_copy_gm2l1_nd2nz` | ![ND->Nz](../../../../figures/ND-Nz.png) |
| l12l0a  | 高维切分数据搬运  | `asc_copy_l12l0a` | ![Nz->Zz](../../../../figures/Nz-Zz.png) Ascend 950PR/Ascend 950DT上L0A分形为Nz, 搬运时为Nz-Nz |
| l12l0b  | 非对齐数据搬运  | `asc_copy_l12l0b` | ![Nz->Zn](../../../../figures/Nz-Zn.png)|

#### 3.1.1 GM2L1 随路转换ND2NZ搬运

该能力主要实现将矩阵从`Global Memory`搬运至`L1 Buffer`，并支持在数据搬运时进行ND到Nz格式的转换。

该能力通过`asc_copy_gm2l1_nd2nz`接口实现，配置`src_nd_matrix_stride`、`src_d_value`等参数完成GM2L1搬运和ND2NZ格式转换。

<img src="../../../../figures/GM到L1的ND2NZ搬运示例.png" alt="GM到L1的ND2NZ搬运示例" width="600" />


以上图为例, 实现A矩阵(Shape(16, 23), 数据类型为half)从GM搬运到L1的能力, 针对Atlas A2/A3训练和推理系列产品, 调用接口如下：

```c
// Allocate L1 Buffer space, m direction is 16, k direction 23 aligned up to 32
constexpr uint64_t m = 16;
constexpr uint64_t k = 32;
__cbuf__ half dst[m * k];

// Set parameters for dst matrix (L1 Buffer) via asc_set_gm2l1_nz_para interface
constexpr uint64_t dst_nz_matrix_stride = 0, dst_nz_c0_stride = 16, dst_nz_n_stride = 1, dst_nd_num = 1;
// Offset between starting addresses of adjacent Nz matrices in destination matrix
constexpr uint64_t nz_config = static_cast<uint64_t>(dst_nz_matrix_stride) << 48;
// After ND conversion to Nz format, offset between starting addresses of adjacent Z matrices in destination Nz matrix
nz_config |= static_cast<uint64_t>(dst_nz_c0_stride) << 32;
nz_config |= static_cast<uint64_t>(dst_nz_n_stride) << 16;
nz_config |= static_cast<uint64_t>(dst_nd_num);
asc_set_gm2l1_nz_para(nz_config);

// Number of elements contained in one row of source matrix
constexpr uint64_t src_d_value = 32;
// Offset between starting addresses of adjacent ND matrices in source matrix
constexpr uint64_t src_nd_matrix_stride = 0;
// L2 Cache uses NORMAL mode
constexpr uint8_t l2_cache_ctrl = 1;
// Number of rows in ND matrix of source matrix
constexpr uint16_t n_value = 16;
// Number of columns in ND matrix of source matrix
constexpr uint32_t d_value = 23;
// When dValue is less than or equal to 4, can enable SmallC0 mode, C0_SIZE will pad to 4 * sizeof(T) Bytes. In this mode, dst_n_stride parameter and dst_c0_stride parameter are invalid, no need to configure.
constexpr bool enable_small_c0 = false;
asc_copy_gm2l1_nd2nz(dst, src, src_d_value, l2_cache_ctrl, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
```

#### 3.1.2 L12L0 数据搬运

该能力主要实现将矩阵从L1 Buffer搬运至L0A/L0B Buffer并支持从Nz分型转换到Zz或Zn格式，C语言编程提供了asc_copy_l12l0a和asc_copy_l12l0b接口，其中针对Ascend 950PR/Ascend 950DT的asc_copy_l12l0a接口可将L1 Buffer中512B大小的矩阵搬运到L0A Buffer，支持2D搬运、转置搬运、3D搬运等模式。

<img src="../../../../figures/L12L0A非转置搬运.png" alt="L12L0A非转置搬运" width="600" />

通过如下方式调用可完成上述的`L1 Buffer`到`L0A Buffer`的搬入操作。
```c
__cbuf__ half l1_buffer[6400];
__ca__ half l0a_buffer[2304];
// m_step is 2, k_step is 3, indicating transfer of 6 data blocks
uint8_t m_step = 2;
uint8_t k_step = 3;
// mStartPosition is 2, indicates M direction transfer starting position is at M axis direction 16*2=64 elements away from source operand starting position
uint16_t m_start_position = 2;
// kStartPosition is 2, indicates K direction transfer starting position is at K axis direction 32B*2=64B position away from source operand starting position
uint16_t k_start_position = 2;
// srcStride is 5, indicates in source operand, interval between starting addresses of previous fractal and next fractal in K direction is 5
int16_t src_stride = 5;
// dstStride is 3: indicates in destination operand, interval between starting addresses of previous fractal and next fractal in K direction is 3
int16_t dst_stride = 3;
asc_copy_l12l0a(l0a_buffer, l1_buffer, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```

搬运到`L0B Buffer`采用`asc_copy_l12l0b`实现，接口参数及实现功能同L12L0A，可参考[cube_datamove](../../../../../api/SIMD-API/C-API/矩阵数据搬运/)查看更多内容。

当输入A矩阵（Global Memory）是转置排布(K × M)时，搬入到`L1 Buffer`为(K × N)排布的Nz分形。要使`L0A Buffer`存放(M × K)的Zz或Nz格式（Ascend 950PR/Ascend 950DT），需要在L12L0A时进行转置，调用`asc_copy_l12l0a_transpose`接口。

<img src="../../../../figures/需要转置的处理场景.png" alt="需要转置的处理场景" width="600" />

```c
// Use transpose interface to complete processing
asc_copy_l12l0a_transpose(l0a_buffer, l1_buffer, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```

### 3.2 矩阵搬出能力

矩阵搬出接口实现L0C到Global Memory或L1 Buffer的数据传输，支持随路量化、ReLU、格式转换(NZ2ND)、通道拆分合并等操作，开发者通过配置参数控制搬运路径、位置、长度等以充分利用硬件能力，通常在Mmad接口后使用，而针对Ascend 950PR/Ascend 950DT还支持L0C到UB Buffer的传输、NZ2DN格式转换以及UB双目标模式, 矩阵搬出能力如下图所示:

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/矩阵搬出流程-AtlasA2A3.png" alt="矩阵搬出流程" width="600" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/矩阵搬出流程-Ascend950PR.png" alt="矩阵搬出流程" width="600" />

矩阵搬出接口支持多种随路能力的灵活组合，以L0C到Global Memory的搬运场景为例：矩阵计算结果暂存于L0C Buffer后需搬运至GM，在此过程中可协同执行随路量化、ReLU激活、格式转换及通道拆分合并等操作。下图展示了这些随路能力的有效组合、中间数据类型及完整数据路径。例如，L0C中的F32数据可通过QF322B8_PRE随路量化得到B8数据，可选使能ReLU后经NZ2NZ/NZ2ND格式转换输出至GM。图中F32→F16和F32→BF16为数据类型转换，其余路径为随路scalar/vector量化模式；针对Ascend 950PR/Ascend 950DT，还额外支持NZ2DN格式转换。

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/L0C2GM搬运-AtlasA2A3.png" alt="L0C2GM搬运" width="600" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/L0C2GM搬运-Ascend950PR.png" alt="L0C2GM搬运" width="600" />

C语言编程提供了`asc_copy_l0c2gm`来使能发挥芯片的各种随路能力, 通过直接传入配置参数可完成随路搬运的操作.
```c
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctrl, uint64_t quant_pre, uint8_t relu_pre, 
                        bool enable_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, 
                        bool enable_clip_relu_post, uint8_t eltwise_op, bool enable_eltwise_antq, bool enable_c0_pad,
                        bool enable_broadcast, bool enable_nz2dn);

```

另外，对于是否开启unit_flag, nz2nd信息的设置，需配置相应寄存器参数以启用不同功能。对应的接口和处理能力如下：
```c
// 1.0 数据搬运过程中进行随路量化时，通过调用该接口设置量化流程中的矢量量化参数。
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool enable_unit_flag);

// 2.0 数据搬运过程中进行随路格式转换（NZ格式转换为ND格式）时，通过调用该接口设置格式转换的相关配置。
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);

// 3.0 对RELU_ALPHA寄存器中的值进行设置。这是一个64bit的寄存器，存储在fixpipe或cube指令中进行Scalar ReLU时使用的alpha值。
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config);
```

详细说明可参考接口说明文档[Cube接口说明](../../../../../api/SIMD-API/C-API/矩阵数据搬运/asc_copy_l0c2gm/)

## 4. 矩阵计算能力说明

`asc_mmad`是Ascend C封装NPU硬件计算能力的矩阵乘加核心接口，用于全连接层、卷积层等算子开发，实现C = A × B + C的矩阵乘加计算（输入为L0A Buffer和L0B Buffer，输出为L0C Buffer），其中针对Atlas A2/A3训练和推理系列产品L0A Buffer分形为Zz、L0B Buffer分形为Zn、L0C Buffer分形为Nz，而Ascend 950PR/Ascend 950DT产品L0A Buffer分形为Nz。

![Ascend 950PR/Ascend 950DT `asc_mmad`计算](../../../../figures/Ascend950PR-Ascend950DT-asc_mmad计算.png)

C语言编程提供了一系列`asc_mmad`重载接口，支持不同数据类型的矩阵计算。以下代码实现上图中的矩阵乘计算。
```c
// 1. Define L0A/L0B matrix sizes
constexpr uint32_t l0_m_size = 64;
constexpr uint32_t l0_k_size = 64;
constexpr uint32_t l0_n_size = 64;
__ca__ c_matrix[l0_m_size * l0_k_size];
__cb__ b_matrix[l0_k_size * l0_n_size];
__cc__ c_matrix[l0_m_size * l0_n_size];

// 2. Configure asc_mmad parameters
// 2.1. Actual matrix sizes participating in computation (m, n, k)
// m direction: 3 fractals, actual value = 3 × 16 = 48
constexpr uint16_t m = 48;
// n direction: 3 fractals, actual value = 3 × 16 = 48
constexpr uint16_t n = 48;
// k direction: 4 fractals, actual value = 4 × 16 = 64
constexpr uint16_t k = 64;

// 2.2 Feature parameters
// unit_flag: Controls fine-grained parallelism of Mmad and Fixpipe instructions. When enabled, results are transferred out immediately after computing each fractal.
uint8_t unit_flag = 0;
// disable_gemv: Configures whether to enable GEMV mode when M=1
bool disable_gemv = false;
// enable_cube_init: Enables C matrix initial value loading. false: do not initialize L0C; true: load initial value from BT Buffer.
bool enable_cube_init = false;
// enable_cube_zero_init: Enables C matrix default zero initialization. true: initialize to 0; false: control initialization via enable_cube_init parameter.
bool enable_cube_zero_init = true;

asc_mmad(c_matrix, a_matrix, b_matrix, m, n, k, unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
```

## Cube矩阵计算示例(Ascend 950PR/Ascend 950DT)

实现矩阵乘计算C(M, N) = A(M, K) × B(K, N)，其中M=64, K=128, N=64，数据类型为half。Ascend C语言实现如下：
```c
extern "C" __global__ __aicore__ void matmul_kernel(__gm__ half* a_gm, __gm__ half* b_gm, __gm__ float* c_gm)
{
    // ===== Step 1: Memory planning and address allocation =====
    constexpr uint32_t BLOCK_M = 64;  // Block M dimension
    constexpr uint32_t BLOCK_K = 128;   // Block K dimension
    constexpr uint32_t BLOCK_N = 64;   // Block N dimension
    
    __cbuf__ half a_l1_buf[BLOCK_M * BLOCK_K];   // L1 staging area
    __cbuf__ half b_l1_buf[BLOCK_K * BLOCK_N];   // L1 staging area
    __ca__ half l0a_buf[BLOCK_M * BLOCK_K];    // L0A input
    __cb__ half l0b_buf[BLOCK_K * BLOCK_N];    // L0B input
    __cc__ float l0c_buf[BLOCK_M * BLOCK_N];   // L0C output
    
    // 1. Transfer A matrix from GM to L1
    // 1.1 Set parameters in L1 Buffer, where dst_nd_num is 1, only one ND block, so dst_nz_matrix_stride is 0, dst_nz_c0_stride is offset of starting address of adjacent Z matrices, offset is 16 C0_SIZE,
    // dst_nz_n_stride indicates offset of starting addresses of adjacent rows in Nz matrix, is 1 C0_SIZE.
    constexpr uint64_t dst_nz_matrix_stride = 0, dst_nz_c0_stride = 16, dst_nz_n_stride = 1, dst_nd_num = 1;
    constexpr uint64_t nz_config = static_cast<uint64_t>(dst_nz_matrix_stride) << 48;
    nz_config |= static_cast<uint64_t>(dst_nz_c0_stride) << 32;
    nz_config |= static_cast<uint64_t>(dst_nz_n_stride) << 16;
    nz_config |= static_cast<uint64_t>(dst_nd_num);
    asc_set_gm2l1_nz_para(nz_config);
    // 1.2 Call asc_copy_gm2l1_nd2nz copy interface to implement GM2L1 transfer operation, where n_value is 64, d_value is 128
    // src_d_value indicates number of elements in one row of source matrix, value is 128. Only one ND matrix, src_nd_matrix_stride is 0.
    asc_copy_gm2l1_nd2nz(a_l1_buf, a_gm, 128, 1, BLOCK_M, BLOCK_K, 0, false);
    
    // 2. Transfer B matrix from GM to L1
    // 2.1 Set parameters in L1 Buffer, parameters are same as A matrix
    asc_set_gm2l1_nz_para(nz_config);
    // 2.2 Call asc_copy_gm2l1_nd2nz copy interface to implement GM2L1 transfer operation, where n_value is 128, d_value is 64
    // src_d_value indicates number of elements in one row of source matrix, value is 64. Only one ND matrix, src_nd_matrix_stride is 0.
    asc_copy_gm2l1_nd2nz(b_l1_buf, b_gm, 64, 1, BLOCK_K, BLOCK_N, 0, false);

    // 3. Insert synchronization, need to wait for GM transfer to L1 Buffer before transferring to L0A/B Buffer
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    
    // 4. Transfer A, B matrices to L0A/L0B
    // 4.1 Transfer A matrix from L1 to L0A
    /*
     * Parameter description:
     * m_step=4: M axis transfer length (unit: 16 elements), value is 64/16
     * k_step=8: K axis transfer length (unit: 32B), value is 128*sizeof(half)/32
     * m_start_position=0: M axis starting position (unit: 16 elements)
     * k_start_position=0: K axis starting position (unit: 32B)
     * src_stride=1: Source matrix K direction fractal interval (unit: 512B), consecutive arrangement is 1
     * dst_stride=1: Destination matrix K direction fractal interval (unit: 512B), consecutive arrangement is 1
     */
    asc_copy_l12l0a(l0a_buf, a_l1_buf, 0, 0, 4, 8, 1, 1);
    
    // 4.2 Transfer B matrix from L1 to L0B
    // Matrix arrangement in GM is (K, N), in L1 is (K, N) Nz fractal, N direction is consecutive.
    // L0B requires Zn arrangement, internally consecutive along K direction, need to call transpose interface for conversion.
    asc_copy_l12l0b_transpose(l0b_buf, b_l1_buf, 0, 0, 4, 8, 1, 1);
    // 5. Insert synchronization: need to wait for data transfer to L0 Buffer before MM computation
    asc_sync_notify(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    
    // 6. Execute Mmad, computation size is (BLOCK_M, BLOCK_K, BLOCK_N)
    asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
    asc_sync_notify(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    
    // 7. Transfer results to GM, configure inline transfer information
    // Parameter mapping for asc_copy_l0c2gm:
    // dst_addr=c_gm, src_addr=l0c_buf
    // n_size=BLOCK_N (64), m_size=BLOCK_M (64)
    // loop_dst_stride=BLOCK_N (destination ND stride)
    // loop_src_stride=16 (source Nz stride, in C0_SIZE units)
    // l2_cache_ctl=1 (NORMAL mode)
    // clip_relu_pre=0, unit_flag_ctrl=0, quant_pre=0
    // relu_pre=0, enable_split=false, enable_nz2nd=true
    // quant_post=0, relu_post=0, enable_clip_relu_post=false
    // eltwise_op=0, enable_eltwise_antq=false, enable_c0_pad=false
    // enable_broadcast=false, enable_nz2dn=false (950PR specific)
    asc_copy_l0c2gm(c_gm, l0c_buf, BLOCK_N, BLOCK_M, BLOCK_N, 16, 1, 0,
        0, QuantMode_t::NoQuant, 0, false, true, 0, 0, false, false, 0, false, false, false, false, false, false);
}
```
