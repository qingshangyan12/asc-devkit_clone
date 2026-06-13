# Cube矩阵计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于`L1 Buffer`、`L0A Buffer`等片上存储的**矩阵计算代码**，该能力是Ascend C最基础、最通用的矩阵编程方式。
AI Core包含Vector计算单元（部署于AIV核）和Cube计算单元（部署于AIC核），分别用于矢量和矩阵类计算。Cube计算单元专用于执行矩阵运算，直接访问的专用缓存如下：L0A Buffer用于存储左矩阵，L0B Buffer用于存储右矩阵，L0C Buffer则用于存储初始化累加值和矩阵计算结果。

## 1. 矩阵编程的基本步骤

Cube矩阵计算的基本步骤为：数据搬入 → 数据加载 → 计算 → 数据搬出。当我们描述一个常见的Cube矩阵计算时，需要执行以下四个步骤：

1. 通过DMA将数据从`Global Memory`搬入到`Local Memory(L1 Buffer)`
2. 通过数据加载将`L1 Buffer`搬入到`L0A Buffer & L0B Buffer`
3. 进行矩阵计算, 计算结果存储在`L0C Buffer`中
4. 通过DMA将数据从`L0C Buffer`搬出到`Global Memory`

## 矩阵计算内存管理

### 矩阵计算内存申请

矩阵计算编程主要通过L0 Buffer进行矩阵计算。开发者需将输入数据从Global Memory搬入L1 Buffer，再从L1 Buffer搬入L0 Buffer，最终通过Cube计算指令完成计算。L1 Buffer与
L0 Buffer均通过数组定义的方式申请，开发者可采用静态或动态方式申请Local Memory资源

- 静态申请：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改。

```c
__global__ __cube__ void add_custom(...)
{
  constexpr uint32_t m_size = 128;
  constexpr uint32_t n_size = 128;
  constexpr uint32_t k_size = 256;
  // 申请L1A/B Buffer
  __cbuf__ half static_l1a_buffer[m_size * k_size];
  __cbuf__ half static_l1b_buffer[n_size * k_size];
  // 申请L0A/B/C Buffer
  __ca__ half static_l0a_buffer[m_size * k_size];
  __cb__ half static_l0b_buffer[n_size * k_size];
  __cb__ half static_l0c_buffer[m_size * n_size];
  
  // ... 使用Buffer进行矩阵计算
}
```

- 动态申请：同Unified Buffer，Cube侧的Local Memory也可以通过动态的方式进行内存的申请。

```c
// Device侧：声明动态共享内存
__global__ __vector__ void add_custom(...)
{
    extern __cbuf__ half dynamic_l1[];
    extern __ca__ half dynamic_l0a[];
    extern __cb__ half dynamic_l0b[];
    extern __cc__ half dynamic_l0c[];
    // ...
}
```

### 矩阵计算内存布局

矩阵计算单元采用分块计算逻辑，其中16×16是矩阵计算单元（fractal）的基本粒度，硬件可以并行处理多个fractal。在传统的线性存储布局（如行主序或列主序）中，数据按行或列连续存放。当需要读取一个16×16的矩形数据块时，若直接从内存中获取，则需访问不连续的多个内存地址以逐行“收集”数据，这种非连续访问会导致内存效率显著下降。
为解决上述问题，引入了矩阵分形存储格式。该格式通过软件层对数据进行重排，使每个16×16计算块在物理内存中连续存放。硬件仅需发起一次连续读取，即可将整个数据块加载至片上寄存器，避免了多地址寻址开销，从而大幅提升矩阵计算单元的数据吞吐率。**16×16是矩阵计算单元的基本分形粒度，适用于所有支持Cube计算的产品型号（Atlas A2/A3训练和推理系列产品、Ascend 950PR/Ascend 950DT）。**
昇腾AI处理器的矩阵计算单元为追求极高的数据吞吐率与计算密度，采用了专有的硬件架构设计。与通用CPU常用的连续线性存储不同，该单元要求输入输出数据必须符合特定的分形存储格式。在使用Ascend C的Mmad类接口进行算子开发时，深入理解这些数据格式对于实现正确的数据搬运、分块及性能优化至关重要。
为便于理解与记忆，文档采用“大Y小x”的直观命名法：
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

Zn格式主要用于L0B Buffer中存放数据, 如果离线处理好的右矩阵数据直接为Zn格式, 则在`Global Memory`和`L1 Buffer`上存储该格式。该格式采用大Z(外部行主序) + 小n(内部列主序)。

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
  Z 字流动： 每个 16 x 16 的小方块内部，以及方块之间，均按照特定的 Z 字形顺序存储（如图中折线箭头所示），完成了从“线性”到“分形”的物理映射，做好了进入 Cube 计算的准备。

Ascend C编程中直接提供了`Global Memory`到`L1 Buffer`中的C语言编程接口，通过该接口可以直接将`Global Memory`数据搬入`L1 Buffer`并完成ND2NZ格式转换。
C语言编程提供了`asc_set_gm2l1_nz_para`和`asc_copy_gm2l1_nd2nz`接口来完成GM2L1的搬运，并随路完成ND2NZ的格式转换处理。另外在Ascend 950PR/Ascend 950DT上，新增了`asc_copy_gm2l1_dn2nz`的能力，支持从DN到Nz的格式转换搬运。

## 矩阵搬运能力说明

### 矩阵数据搬入

矩阵搬入接口实现Global Memory到L0A/L0B Buffer的数据传输，通常与Mmad接口配合使用。该接口支持多种搬运模式：二维搬运、三维格式转换、压缩数据解压、矩阵转置、稀疏化数据处理等。

开发者通过配置参数控制搬运路径、起始位置、长度等信息，充分利用硬件能力，最大化算子性能。每种数据通路对应特定的存储层级和访问特性。

结合上述数据流和分形知识，C语言编程提供以下矩阵搬入能力（完整接口见API参考手册）：
| 数据通路 | 功能    | C语言编程接口  | 分行支持情况 |
|---------|---------|---------------|-------------|
| Global Memory → L1 Buffer   | 随路转换ND2NZ搬运  | `asc_copy_gm2l1_nd2nz` | ![ND->Nz](../../../../figures/ND-Nz.png) |
| L1 Buffer → L0A Buffer  | 将L1 Buffer数据搬运到L0A Buffer中  | `asc_copy_l12l0a` | ![Nz->Zz](../../../../figures/Nz-Zz.png) Ascend 950PR/Ascend 950DT上L0A分形为Nz, 搬运时为Nz-Nz |
| L1 Buffer → L0A Buffer  | 将L1 Buffer数据搬运到L0B Buffer中  | `asc_copy_l12l0b` | ![Nz->Zn](../../../../figures/Nz-Zn.png)|

#### GM2L1 随路转换ND2NZ搬运

该能力主要实现将矩阵从`Global Memory`搬运至`L1 Buffer`，并支持在数据搬运时进行ND到Nz格式的转换。

该能力通过`asc_copy_gm2l1_nd2nz`接口实现，配置`src_nd_matrix_stride`、`src_d_value`等参数完成GM2L1搬运和ND2NZ格式转换。

<img src="../../../../figures/GM到L1的ND2NZ搬运示例.png" alt="GM到L1的ND2NZ搬运示例" width="700" />


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

#### L12L0 数据搬运

该能力主要实现将矩阵从L1 Buffer搬运至L0A/L0B Buffer并支持从Nz分型转换到Zz或Zn格式，C语言编程提供了asc_copy_l12l0a和asc_copy_l12l0b接口，其中针对Ascend 950PR/Ascend 950DT的asc_copy_l12l0a接口可将L1 Buffer中512B大小的矩阵搬运到L0A Buffer，支持2D搬运、转置搬运、3D搬运等模式。

<img src="../../../../figures/L12L0A非转置搬运.png" alt="L12L0A非转置搬运" width="700" />

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

<img src="../../../../figures/需要转置的处理场景.png" alt="需要转置的处理场景" width="800" />

```c
// Use transpose interface to complete processing
asc_copy_l12l0a_transpose(l0a_buffer, l1_buffer, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```

### 矩阵数据搬出

矩阵搬出接口实现L0C到Global Memory或L1 Buffer的数据传输，支持随路量化、ReLU、格式转换(NZ2ND)、通道拆分合并等操作，开发者通过配置参数控制搬运路径、位置、长度等以充分利用硬件能力，通常在Mmad接口后使用，而针对Ascend 950PR/Ascend 950DT还支持L0C到UB Buffer的传输、NZ2DN格式转换以及UB双目标模式, 矩阵搬出能力如下图所示:

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/矩阵搬出流程-AtlasA2A3.png" alt="矩阵搬出流程" width="800" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/矩阵搬出流程-Ascend950PR.png" alt="矩阵搬出流程" width="800" />

矩阵搬出接口支持多种随路能力的灵活组合，以L0C到Global Memory的搬运场景为例：矩阵计算结果暂存于L0C Buffer后需搬运至GM，在此过程中可协同执行随路量化、ReLU激活、格式转换及通道拆分合并等操作。下图展示了这些随路能力的有效组合、中间数据类型及完整数据路径。例如，L0C中的F32数据可通过QF322B8_PRE随路量化得到B8数据，可选使能ReLU后经NZ2NZ/NZ2ND格式转换输出至GM。图中F32→F16和F32→BF16为数据类型转换，其余路径为随路scalar/vector量化模式；针对Ascend 950PR/Ascend 950DT，还额外支持NZ2DN格式转换。

针对Atlas A2/A3训练和推理系列产品：

<img src="../../../../figures/L0C2GM搬运-AtlasA2A3.png" alt="L0C2GM搬运" width="800" />

针对Ascend 950PR/Ascend 950DT：

<img src="../../../../figures/L0C2GM搬运-Ascend950PR.png" alt="L0C2GM搬运" width="800" />

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


## 矩阵计算能力说明

`asc_mmad`是Ascend C封装NPU硬件计算能力的矩阵乘加核心接口，用于全连接层、卷积层等算子开发，实现C = A × B + C的矩阵乘加计算（输入为L0A Buffer和L0B Buffer，输出为L0C Buffer），其中针对Atlas A2/A3训练和推理系列产品L0A Buffer分形为Zz、L0B Buffer分形为Zn、L0C Buffer分形为Nz，而Ascend 950PR/Ascend 950DT产品L0A Buffer分形为Nz。

![Ascend 950PR/Ascend 950DT `asc_mmad`计算](../../../../figures/Ascend950PR-Ascend950DT-asc_mmad计算.png)

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

## 矩阵计算的同步

AI Core内部执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行方式运行，读写同一存储资源时可能存在数据依赖关系。因此，Cube计算需通过流水同步接口确保正确执行。Cube矩阵计算主要包含四步：数据搬入（Global Memory → L1 Buffer）、数据搬入（L1 Buffer → L0A/B Buffer）、矩阵计算（L0A/B Buffer → L0C Buffer）、数据搬出（L0C Buffer → Global Memory）。四个步骤分别对应PIPE_MTE2、PIPE_MTE1、PIPE_M、PIPE_MTE3流水线，需通过核内同步接口协调执行顺序，确保各步骤按序完成。

当开发者进行一个矩阵乘法计算时(Ascend 950PR/Ascend 950DT产品), 开发者可以通过`asc_lock`&`asc_unlock`的方式完成对这四个步骤的同步。
```c
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint8_t mutex_id = 1;
    ...
    // 1. 步骤一: 数据搬入（Global Memory → L1 Buffer）, 执行流水为PIPE_MTE2
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2l1_nd2nz(a_l1_buf, a_gm, 128, 1, BLOCK_M, BLOCK_K, 0, false);
    asc_copy_gm2l1_nd2nz(b_l1_buf, b_gm, 64, 1, BLOCK_K, BLOCK_N, 0, false);
    asc_unlock(PIPE_MTE2, mutex_id);
    ...
    // 2. 步骤二: 数据搬入（L1 Buffer → L0A/B Buffer）, 执行流水为PIPE_MTE1
    asc_lock(PIPE_MTE1, mutex_id);
    asc_copy_l12l0a(l0a_buf, a_l1_buf, 0, 0, 4, 8, 1, 1);
    asc_copy_l12l0b_transpose(l0b_buf, b_l1_buf, 0, 0, 4, 8, 1, 1);
    asc_unlock(PIPE_MTE1, mutex_id);
    ...
    // 3. 步骤三: 矩阵计算（L0A/B Buffer → L0C Buffer）, 执行流水为PIPE_M
    asc_lock(PIPE_M, mutex_id);
    asc_mmad(l0c_buf, l0a_buf, l0b_buf, BLOCK_M, BLOCK_K, BLOCK_N, 0, false, false, false);
    asc_unlock(PIPE_M, mutex_id);
    ...
    // 4. 步骤四: 数据搬出（L0C Buffer → Global Memory）, 执行流水为PIPE_MTE3
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_l0c2gm(c_gm, l0c_buf, BLOCK_N, BLOCK_M, BLOCK_N, 16, 1, 0,
        0, QuantMode_t::NoQuant, 0, false, true, 0, 0, false, false, 0, false, false, false, false, false, false);
    asc_unlock(PIPE_MTE3, mutex_id);
}
```

## Cube矩阵计算示例

当前C API样例正在开发中，完整矩阵计算实现示例将在后续版本中补充。开发者可参考API文档了解[Cube接口详细说明](../../../../../api/SIMD-API/C-API/)。
