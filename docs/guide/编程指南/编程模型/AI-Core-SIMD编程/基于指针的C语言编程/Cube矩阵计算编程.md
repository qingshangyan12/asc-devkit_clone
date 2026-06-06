# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**，该能力是Ascend C最基础、最通用的矩阵编程方式。

AI Core包含Vector计算单元（部署于AIV核）和Cube计算单元（部署于AIC核），分别用于矢量和矩阵类计算。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

## 矩阵计算数据流

Cube矩阵计算的数据流为：`CopyGM2L1` → `CopyL12L0A/B` → `Mmad` → `CopyL0C2GM`。
当我们描述一个常见的Cube矩阵计算时，需要执行以下四个步骤：
1. 通过`asc_copy_gm2l1_nd2nz`接口将A、B原始矩阵从GM搬运到L1 Buffer中。
2. 通过`asc_copy_l12l0a`和`asc_copy_l12l0b`接口分别将A、B矩阵搬运到L0A Buffer和L0B Buffer中。
3. 通过`asc_mmad`接口对L0A Buffer和L0B Buffer中的数据进行矩阵计算，结果输出到L0C Buffer。
4. 通过`asc_copy_l0c2gm`接口将L0C Buffer的结果搬运到GM中，支持随路量化、ReLU等操作。

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
针对Atlas A2/A3训练和推理系列产品，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Zz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式。如图1所示：
![矩阵乘法场景设计的数据格式](../../../../figures/矩阵乘法场景设计的数据格式.png)

针对Ascend 950PR/Ascend 950DT，矩阵乘法 C = A × B 要求：左矩阵 A 使用 Nz 格式，右矩阵 B 使用 Zn 格式，结果矩阵 C 使用 Nz 格式，如图2所示：
![矩阵乘法场景涉及的数据格式](../../../../figures/矩阵乘法场景涉及的数据格式.png)

**ND 格式(N-Dimension)**

ND格式是通用的N维张量格式，数据在内存中连续线性存放，通常存在于`Global Memory (GM)`中。在进行矩阵运算前，通常需要将其转换为NZ格式以适配Cube计算单元。

**Nz 格式**
NZ格式主要用于L1 Buffer和L0C Buffer中存放数据。针对Ascend 950PR/Ascend 950DT，L0A Buffer也采用Nz分形格式。该格式采用大N（外部列主序）+ 小z（内部行主序）的方式排布。

![Nz数据排布格式](../../../../figures/Nz数据排布格式.png)

Nz是Ascend架构的中间格式，介于线性数据(ND)和计算专用数据(Zz/Zn)之间，便于格式转换。大N排布（列主序Block）的设计通常是为了配合多核并行或后续算子（如Vector向量计算）在处理通道（Channel）维度数据时的便利性。

> 📌 Nz格式在不同Buffer中的应用场景：
> - **L0C Buffer**: 存储矩阵乘法结果。分形形状为16×16（256元素），适合Cube计算单元高效运算。
> - **L1 Buffer**: 存储中间数据。分形形状为16×(32B/sizeof(DataType))，大小512字节，便于转换为Zz和Zn格式。
> - **L0A Buffer（仅Ascend 950PR/950DT）**: 存储左矩阵。

**Zz 格式**
针对Atlas A2/A3训练和推理系列产品，L0A Buffer使用此格式（Ascend 950PR/950DT使用Nz格式）。该格式采用大Z（外部行主序）+ 小z（内部行主序）。

![Zz 格式(以half类型为例)](../../../../figures/Zz格式-以half类型为例.png)

在矩阵乘法中，A矩阵逐行读取数据。Zz格式的行主序特性使同一行元素物理地址连续，与计算访问模式匹配，便于Cube单元左侧通道通过DMA连续读取，避免跨步访问，提升数据搬运效率。

**Zn 格式**
Zn格式主要用于L0B Buffer中存放数据, 如果离线处理好的右矩阵数据直接为Zn格式, 则在`Global Memory`和`L1 Buffer`上存储该格式。该格式采用大Z(外部行主序) + 小n(内部列主序)。

![Zn 格式(以half类型为例)](../../../../figures/Zn格式-以half类型为例.png)

矩阵乘法需读取B矩阵列向量。普通行主序存储导致列数据地址跳跃。Zn格式在分形内部按列存储，使列数据在L0B中紧凑排列，便于Cube单元右侧通道线性读取完整列向量，避免跨步访问。

### 格式转换

根据上述信息，Global Memory采用ND格式，L1 Buffer采用Nz格式，因此需进行ND2NZ格式转换。

![ND2NZ格式转换](../../../../figures/ND2NZ格式转换.png)

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
C语言编程提供了`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`接口来完成GM2L1的搬运，并随路完成ND2NZ的格式转换处理。另外在Ascend 950DT/Ascend 950RP上，新增了`asc_copy_gm2l1_dn2nz`的能力，支持从DN到NZ的格式转换搬运。

## 矩阵搬运能力说明

### 矩阵搬入能力

矩阵搬入接口实现Global Memory到L0A/L0B Buffer的数据传输，通常与Mmad接口配合使用。该接口支持多种搬运模式：二维搬运、三维格式转换、压缩数据解压、矩阵转置、稀疏化数据处理等。

开发者通过配置参数控制搬运路径、起始位置、长度等信息，充分利用硬件能力，最大化算子性能。每种数据通路对应特定的存储层级和访问特性。

结合上述数据流和分形知识，C语言编程提供以下矩阵搬入能力（完整接口见API参考手册）：
| 数据通路 | 功能    | C语言编程接口  | 分行支持情况 |
|---------|---------|---------------|-------------|
| gm2l1   | 随路转换ND2NZ搬运  | `asc_copy_gm2l1_nd2nz` | ![ND->Nz](../../../../figures/ND-Nz.png) |
| l12l0a  | 高维切分数据搬运  | `asc_copy_l12l0a` | ![Nz->Zz](../../../../figures/Nz-Zz.png) Ascend 950DT/Ascend 950RP上L0A分形为Nz, 搬运时为Nz-Nz |
| l12l0b  | 非对齐数据搬运  | `asc_copy_l12l0b` | ![Nz->Zn](../../../../figures/Nz-Zn.png)|

**GM2L1 随路转换ND2NZ搬运**

该能力主要实现将矩阵从`Global Memory`搬运至`L1 Buffer`，并支持在数据搬运时进行ND到NZ格式的转换。

该能力通过`asc_copy_gm2l1_nd2nz`接口实现，配置`src_nd_matrix_stride`、`src_d_value`等参数完成GM2L1搬运和ND2NZ格式转换。

![GM到L1的ND2NZ搬运示例](../../../../figures/GM到L1的ND2NZ搬运示例.png)
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

该能力主要实现将矩阵从`L1 Buffer`搬运至`L0A/L0B Buffer`，并支持从Nz分型转换到Zz或Zn格式。

C语言编程提供了`asc_copy_l12l0a`和`asc_copy_l12l0b`接口。以下以L1到L0A搬运为例说明。

针对Ascend 950PR/Ascend 950DT，`asc_copy_l12l0a`接口将L1 Buffer中512B大小的矩阵搬运到L0A Buffer，支持2D搬运、转置搬运、3D搬运等模式。

![L12L0A非转置搬运](../../../../figures/L12L0A非转置搬运.png)

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
asc_copy_l12l0a(l0a_buffer, l1_buffer, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```

搬运到`L0B Buffer`采用`asc_copy_l12l0b`实现，接口参数及实现功能同L12L0A，可参考[cube_datamove](../../../../../api/SIMD-API/C-API/矩阵数据搬运/)查看更多内容。

当输入A矩阵（Global Memory）是转置排布(K × M)时，搬入到`L1 Buffer`为(K × N)排布的Nz分形。要使`L0A Buffer`存放(M × K)的Zz或Nz格式（Ascend 950RP/950DT），需要在L12L0A时进行转置，调用`asc_copy_l12l0a_transpose`接口。

![需要转置的处理场景](../../../../figures/需要转置的处理场景.png)
```c
// 使用transpose接口完成处理
asc_copy_l12l0a_transpose(l0a_buffer, l1_buffer, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```

### 矩阵搬出能力

矩阵搬出接口实现L0C到Global Memory或L1 Buffer的数据传输，支持随路量化、格式转换等操作，通常在Mmad接口后使用。针对Ascend 950PR/950DT，还支持L0C到UB Buffer的传输。

搬出接口支持随路量化、ReLU、格式转换(NZ2ND)、通道拆分合并等。开发者通过配置参数控制搬运路径、位置、长度等，充分利用硬件能力。针对Ascend 950PR/950DT，还支持NZ2DN格式转换和UB双目标模式。

针对AtlasA3训练系列产品/AtlasA3推理系列产品和AtlasA2训练系列产品/AtlasA2推理系列产品：

![矩阵搬出流程](../../../../figures/矩阵搬出流程-AtlasA2A3.png)

针对Ascend 950PR/Ascend 950DT：

![矩阵搬出流程](../../../../figures/矩阵搬出流程-Ascend950PR.png)

矩阵搬出接口支持多种随路能力组合。以L0C到Global Memory为例：矩阵计算结果存放在L0C Buffer，需搬运至GM，支持随路格式转换等操作。

随路量化、ReLU、格式转换、通道拆分合并的有效组合、中间数据类型和数据路径如下图所示。例如，L0C中F32数据通过QF322B8_PRE随路量化得到B8数据，可选使能ReLU，然后经过NZ2NZ/NZ2ND输出到GM。图中F32→F16和F32→BF16为数据类型转换，其他为随路scalar/vector量化模式。针对Ascend 950PR/950DT，还支持NZ2DN格式转换。
针对Atlas A2/A3训练和推理系列产品：
![L0C2GM搬运](../../../../figures/L0C2GM搬运-AtlasA2A3.png)

针对Ascend 950PR/Ascend 950DT：
![L0C2GM搬运](../../../../figures/L0C2GM搬运-Ascend950PR.png)

矩阵搬出接口支持多种随路能力组合，需配置相应寄存器参数以启用不同功能。对应的接口和处理能力如下：

1. L0C2GM数据搬出接口，支持配置随路量化、ReLU激活、NZ到ND格式转换：
```c
__aicore__ inline void asc_copy_l0c2gm(__gm__ bfloat16_t *dst_addr, __cc__ float *src_addr,
                        uint16_t n_size, uint16_t m_size, uint32_t loop_dst_stride, uint16_t loop_src_stride, uint8_t l2_cache_ctl,
                        uint8_t clip_relu_pre, uint8_t unit_flag_ctrl, uint64_t quant_pre, uint8_t relu_pre, 
                        bool enable_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, 
                        bool enable_clip_relu_post, uint8_t eltwise_op, bool enable_eltwise_antq, bool enable_c0_pad,
                        bool enable_broadcast, bool enable_nz2dn);
```

2. 设置随路量化参数和pReLU参数（待实现）。Tensor的每个元素代表一个量化参数：
```c
__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag);
```

3. 设置随路NZ2ND格式转换配置：
```c
__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride);
```

4. 设置随路Leaky ReLU系数：
```c
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config);
```

详细说明可参考接口说明文档[Cube接口说明](../../../../../api/SIMD-API/C-API/矩阵数据搬运/asc_copy_l0c2gm/)

## 矩阵计算能力说明

`asc_mmad`是Ascend C的矩阵乘加核心接口，封装NPU硬件计算能力，用于全连接层、卷积层等算子开发。

`asc_mmad`实现矩阵乘加计算：C = A × B + C。输入为L0A Buffer和L0B Buffer，输出为L0C Buffer。针对Atlas A2/A3训练和推理系列产品，L0A Buffer分形为Zz，L0B Buffer分形为Zn，L0C Buffer分形为Nz。Ascend 950PR/950DT产品L0A Buffer分形为Nz。

![Ascend 950PR/Ascend 950DT `asc_mmad`计算](../../../../figures/Ascend950PR-Ascend950DT-asc_mmad计算.png)

C语言编程提供了一系列`asc_mmad`重载接口，支持不同数据类型的矩阵计算。以下代码实现上图中的矩阵乘计算。

```c
__aicore__ inline void asc_mmad(__cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height, uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val);

```c
// 1. 定义L0A/L0B矩阵大小
constexpr uint32_t l0_m_size = 64;
constexpr uint32_t l0_k_size = 64;
constexpr uint32_t l0_n_size = 64;
__ca__ c_matrix[l0_m_size * l0_k_size];
__cb__ b_matrix[l0_k_size * l0_n_size];
__cc__ c_matrix[l0_m_size * l0_n_size];

// 2. 配置asc_mmad参数
// 2.1. 实际参与计算的矩阵大小(m, n, k)
// m方向：3个分形，实际值 = 3 × 16 = 48
constexpr uint16_t m = 48;
// n方向：3个分形，实际值 = 3 × 16 = 48
constexpr uint16_t n = 48;
// k方向：4个分形，实际值 = 4 × 16 = 64
constexpr uint16_t k = 64;

// 2.2 特性参数
// unit_flag：控制Mmad和Fixpipe指令细粒度并行。使能后，每计算完一个分形立即搬出结果。
uint8_t unit_flag = 0;
// disable_gemv：M=1时配置是否开启GEMV模式
bool disable_gemv = false;
// enable_cube_init：使能C矩阵初始值加载。false：不初始化L0C；true：从BT Buffer加载初始值。
bool enable_cube_init = false;
// enable_cube_zero_init：使能C矩阵默认清零。true：初始化为0；false：通过enable_cube_init参数控制初始化。
bool enable_cube_zero_init = true;

asc_mmad(c_matrix, a_matrix, b_matrix, m, n, k, unit_flag, disable_gemv, enable_cube_init, enable_cube_zero_init);
```

## Cube矩阵计算示例(Ascend 950PR/Ascend 950DT)

实现矩阵乘计算C(M, N) = A(M, K) × B(K, N)，其中M=64, K=128, N=64，数据类型为half。C语言编程实现如下：
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
    asc_copy_gm2l1_nd2nz(b_l1_buf, b_gm, 64, 1, BLOCK_K, BLOCK_N, 0, false);

    // 3. 插入同步，搬入L0A/B Buffer前需要等待GM搬入到L1 Buffer中
    asc_sync_notify(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_MTE1, EVENT_ID0);
    
// 4. 将A、B矩阵搬入到L0A/L0B
    // 4.1 A矩阵从L1搬入到L0A
    /*
     * 参数说明：
     * m_step=4：M轴搬运长度（单位：16元素），值为64/16
     * k_step=8：K轴搬运长度（单位：32B），值为128*sizeof(half)/32
     * m_start_position=0：M轴起始位置（单位：16元素）
     * k_start_position=0：K轴起始位置（单位：32B）
     * src_stride=1：源矩阵K方向分形间隔（单位：512B），连续排布为1
     * dst_stride=1：目标矩阵K方向分形间隔（单位：512B），连续排布为1
     */
    asc_copy_l12l0a(l0a_buf, a_l1_buf, 0, 0, 4, 8, 1, 1);
    
    // 4.2 B矩阵从L1搬入到L0B
    // GM中矩阵排布为(K, N)，L1中为(K, N)的Nz分形，N方向连续。
    // L0B要求Zn排布，内部沿K方向连续，需调用transpose接口转换。
    asc_copy_l12l0b_transpose(l0b_buf, b_l1_buf, 0, 0, 4, 8, 1, 1);
    // 5. 插入同步：进行MM计算前需等待数据搬入L0 Buffer
    asc_sync_notify(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    asc_sync_wait(PIPE_MTE1, PIPE_CUBE, EVENT_ID0);
    
    // 6. 执行Mmad，计算大小为(BLOCK_M, BLOCK_K, BLOCK_N)
    asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
    asc_sync_notify(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_CUBE, PIPE_MTE3, EVENT_ID0);
    
    // 7. 将结果搬出到GM，配置随路搬运信息
    asc_copy_l0c2gm(c_gm, l0c_buf, BLOCK_N, BLOCK_M, BLOCK_N, 16, 1, 0,
        0, QuantMode_t::NoQuant, 0, false, true, 0, 0, false, false, 0, false, false, false, false, false, false);
}
```