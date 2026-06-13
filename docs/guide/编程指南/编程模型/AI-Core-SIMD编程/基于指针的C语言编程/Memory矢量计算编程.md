# Memory矢量计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用C语言编程API编写基于Unified Buffer（以下简称UB）的矢量计算代码，该能力是Ascend C最基础、最通用的矢量编程方式。这种基于UB内存的编程方式称为Memory矢量计算编程。Memory矢量计算在Atlas A2/A3训练和推理系列产品上提供完整的UB直接编程能力；Ascend 950PR/Ascend 950DT虽然支持UB操作，但推荐使用Reg矢量计算编程以获得更高性能。

## Memory矢量编程的基本步骤

Memory矢量计算的数据流为：数据搬入 → 计算 → 数据搬出。当我们描述一个常见的Vector矢量计算时，需要执行以下三个步骤：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

## Memory矢量计算内存管理

Memory矢量编程主要是通过Local Memory(Unified Buffer)进行矢量计算, 开发者必须将输入搬入到Unified Buffer并通过Vector计算指令完成计算。其中Unified Buffer通过数组定义的方式进行申请, 开发者可以通过动态或者静态的方式申请Unified Buffer进行矢量计算。

- 静态申请：分配一段指定大小的内存空间，空间大小在编译时确定，不可动态修改。

```c
__global__ __vector__ void add_custom(...)
{
    constexpr uint32_t ub_size = 256;
    __ubuf__ half static_buffer[ub_size];  // Compiler automatically aligns to 32 bytes
}
```

- 动态申请：用户需要通过<<<>>>中参数dynamic_ub_size指定动态内存的空间大小，其大小在运行期确定。

```c
// Device-side: declare dynamic shared memory
__global__ __vector__ void add_custom(...)
{
    extern __ubuf__ half dynamic_buf[];
    // ...
}

// Host-side: specify dynamic shared memory size via third parameter in <<<>>>
int32_t main(int argc, char const *argv[])
{
    // ...
    uint32_t dyn_ubuf_size = 1024 * sizeof(char);
    add_custom<<<blocks_per_grid, threads_per_block, dyn_ubuf_size, stream>>>(...);
    // ...
}
```

## Memory数据搬运

Global Memory（GM）与Unified Buffer（UB）之间的数据搬运提供了多样化的接口，支持丰富的数据搬运场景。为实现高效的Vector计算，系统支持将数据从GM搬运至UB，供Vector计算单元进行向量计算；计算完成后，再将结果从UB搬回GM，完成数据闭环。

开发者通过灵活配置结构体参数，精确控制数据块数量、地址长度及间隔等关键属性，实现连续和非连续数据搬运（高维切分数据搬运），充分释放硬件的数据搬运与计算协同能力，提升向量算子的执行效率。

<table>
<thead>
<tr><th>数据通路</th><th>搬运模式</th><th>接口</th><th>适用场景</th></tr>
</thead>
<tbody>
<tr>
<td>高维切分</td>
<td><code>asc_copy_gm2ub</code></td>
<td>非连续多块数据搬运，可配置块数和间隔</td>
</tr>
<tr>
<td rowspan="3">Global M</td>
<td>连续搬运</td>
<td><code>asc_copy_gm2ub</code></td>
<td>连续地址的一维数据搬运</td>
</tr>
<tr>
<td>非对齐</td>
<td><code>asc_copy_gm2ub_align</code></td>
<td>地址非32B对齐，支持自定义填充</td>
</tr>
<tr>
<td>高维切分</td>
<td><code>asc_copy_ub2gm</code></td>
<td>非连续多块数据搬运，可配置块数和间隔</td>
</tr>
<tr>
<td rowspan="3">ub2gm</td>
<td>连续搬运</td>
<td><code>asc_copy_ub2gm</code></td>
<td>连续地址的一维数据搬运</td>
</tr>
<tr>
<td>非对齐</td>
<td><code>asc_copy_ub2gm_align</code></td>
<td>地址非32B对齐</td>
</tr>
</tbody>
</table>

根据上述矢量搬运能力的描述, 开发者可以根据不同的场景选择合适的接口进行处理。

**高维切分搬运**: 当用户需要将多块非连续的`Global Memory`搬入到一块`Unified Buffer`时，可通过高维切分模式配置搬运数据块的个数、单个数据块的长度、每个数据块之间的间隔。以src_gm数据(shape(16, 256)，数据类型为half)为例，隔行将数据搬运到`Unified Buffer`中，可通过如下方式实现：
```c
// Number of consecutive data blocks: 16 rows of data, transfer every alternate row to UB, need to transfer 8 rows
uint16_t burst_count = 8;
// Length of single data block (in 32B units): value is 256 * sizeof(half) / 32
uint16_t burst_len = 256 * sizeof(half) / 32;
// Stride from previous block to next block in source data (in 32B units): interval of two rows between blocks, value is 256 * 2 * sizeof(half) / 32
uint16_t src_stride = 256 * 2 * sizeof(half) / 32;
// Stride from previous block to next block in destination data (in 32B units): data blocks are consecutive, value is 256 * sizeof(half) / 32
uint16_t dst_stride = 256 * sizeof(half) / 32;

asc_copy_gm2ub(dst_ub, src_gm, burst_count, burst_len, src_stride, dst_stride);
```
> 📌 stride（前一块数据头到后一块数据头的间隔）为该接口从Ascend 950PR/Ascend 950DT引入，Atlas A2/A3训练和推理系列产品使用gap（前一个数据块结束地址与后一个数据块起始地址的差值）。

**连续搬运**: 如果开发者需要将一块连续的`Global Memory`搬运到`Unified Buffer`时，只需指定搬运数据大小（字节）即可。例如src_gm数据(shape(256)，数据类型为half)，需搬入到`Unified Buffer`中，通过`asc_copy_gm2ub`直接调用：
```c
uint32_t size = 256 * sizeof(half);
asc_copy_gm2ub(dst_ub, src_gm, size);
```
> 📌 该接口主要为了提升用户数据搬运的易用性，由软件层面提供，底层通过高维切分接口实现。

**非对齐搬运**: 高维切分数据搬运提供了用户进行非连续多个数据块搬入的能力，但该接口要求`Global Memory`中搬运的长度和间隔都必须保证是32B对齐的，因此对于一些非对齐场景无法支持。例如将上述示例中的src_gm数据修改为(shape(16, 200)，数据类型为half)，则无法通过`asc_copy_gm2ub`完成隔行数据的搬运，此时需要突破32B的约束限制。Ascend C根据芯片能力，提供了`asc_copy_gm2ub_align`接口来支持该搬运形式，该接口可以指定以字节为单位的burst_len和stride信息，完成搬运操作。

![非对齐搬入能力](../../../../figures/非对齐搬入能力.png)

以上图为例, 将两个数据块从GM搬运至UB，每个blockLen为54B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。
在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 54B + 2 * 2B + 3 * 2B = 64B，满足32B对齐。
```c
// Number of consecutive data blocks: transfer two consecutive data blocks
uint16_t burst_count = 2;
// Length of single data block (in bytes): value is 54
uint16_t burst_len = 54;
// Stride from previous block to next block in source data: source data stride unit is 1B
uint16_t src_stride = 1;
// Stride from previous block to next block in destination data: UB operation constraint address must be 32B aligned, value is 32 / 32
uint16_t dst_stride = 32 / 32;
// Left padding of transfer result (element count): value is 4 / sizeof(half) = 2
uint8_t left_padding_num = 2;
// Right padding of transfer result (element count): value is 6 / sizeof(half) = 3
uint8_t right_padding_num = 3;

asc_copy_gm2ub_align(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, false, 0, src_stride, dst_stride);
```

如上就完成了从`Global Memory`到`Unified Buffer`的搬入动作，反之从`Unified Buffer`搬出到`Global Memory`中也采用同样的能力进行处理。

更多的搬运能力可参考API手册。

## 3. Memory数据计算

Memory矢量计算提供连续计算和高维切分两种计算模式，并可通过掩码实现数据选择性参与计算。
以Add接口为例，两种计算模式的调用方式示意图如下。其中，连续计算简单直观，适合一维Tensor连续数据处理；高维切分计算灵活可控，支持迭代执行和地址间隔配置。开发者可根据实际业务需求选择合适的计算方式。

<img src="../../../../figures/Memory矢量计算模式示意图.png" alt="Memory矢量计算模式示意图" width="800" />

### 3.1 高维切分计算

高维切分能力主要是支持对计算接口设置`repeat_time`、`stride`和`mask`参数等, 能精确控制指令的迭代执行、操作数的地址间隔和迭代内的掩码。

#### 3.1.1 迭代控制
Vector计算单元每个迭代会从UB中取出8个DataBlock（每个DataBlock数据块内部地址连续，长度为32Byte）进行计算，并写入对应的8个DataBlock中。

<img src="../../../../figures/DataBlock迭代示意图.png" alt="DataBlock迭代示意图" width="800" />

矢量计算API支持开发者通过repeatTime来配置迭代次数，从而控制指令的多次迭代执行。假设repeat_time设置为2，矢量计算单元会进行2个迭代的计算，可计算出2 × 8（每个迭代8个DataBlock） × 32Byte = 512Byte的结果。如果数据类型为half，则计算了256个元素。下图展示了2次迭代Exp计算的示意图。**由于硬件限制，repeat_time取值范围为[1, 255]，该限制适用于所有支持Memory矢量计算的产品型号（Atlas A2/A3训练和推理系列产品、Ascend 950PR/Ascend 950DT）。**

<img src="../../../../figures/2次迭代Exp计算.png" alt="2次迭代Exp计算" width="800" />

#### 3.1.2 地址间隔配置

矢量计算单元还支持带间隔的向量计算，通过`data_block_stride`（单次迭代内不同`data_block`间地址步长）和`repeat_stride`（相邻迭代间相同`data_block`的地址步长）来进行配置。`data_block_stride`是指单次迭代内不同`data_block`间地址步长。其取值不能超过UB大小限制。
连续计算，`data_block_stride`设置为1，对同一迭代内的8个`data_block`数据连续进行处理。
非连续计算，`data_block_stride`值大于1（如取2），同一迭代内不同`data_block`之间在读取数据时出现一个`data_block`的间隔，如下图所示。

<img src="../../../../figures/data_block_stride示例.png" alt="data_block_stride示例" width="800" />

> 📌 若dst_block_stride == 0, 等同于dst_block_stride = 1; 若src_block_stride = 0, 则源操作数将始终复用第一个data_block。

下图给出了单次迭代内源操作数与目的操作数在UB空间的读写示例。示例中源操作数的`data_block_stride`配置为2，表示单次迭代内不同DataBlock间地址步长为2个DataBlock；目的操作数的`data_block_stride`配置为1，表示单次迭代内地址连续。

<img src="../../../../figures/单次迭代内源和目的操作数读写示例.png" alt="单次迭代内源和目的操作数读写示例" width="800" />

其中**repeat_stride**是指相邻迭代间相同`data_block`的地址步长, 可通过设置`repeat_stride`来灵活控制不同的场景。

| 场景 | repeat_stride取值 | 数据读取特点 | 适用场景 |
|------|------------------|-------------|--------|
| 连续计算 | 8 | 每轮迭代读取连续8个data_block，完成所有输入数据的计算 | <img src="../../../../figures/repeat_stride连续计算场景.png" alt="repeat_stride连续计算场景" width="600" /> |
| 非连续计算 | >8 (如10) | 相邻迭代间存在data_block间隔，地址不连续 | <img src="../../../../figures/repeat_stride非连续计算场景.png" alt="repeat_stride非连续计算场景" width="600" /> |
| 反复计算 | 0 | 对首个连续8个data_block反复读取和计算 | <img src="../../../../figures/repeat_stride反复计算场景.png" alt="repeat_stride反复计算场景" width="600" /> |
| 部分重复计算 | (0, 8) | 相邻迭代间部分数据重复读取，一般场景不涉及 | <img src="../../../../figures/repeat_stride部分重复计算.png" alt="repeat_stride部分重复计算" width="600" /> |

当 `repeat_time`>1 时，通过多次迭代完成计算。`repeat_stride` 表示相邻迭代间相同位置`data_block`的起始地址间隔（以 `data_block` 为单位）。例如 `repeat_stride` = 9 时，第一迭代的第1个 `data_block` 与第二迭代的第1个 `data_block` 间隔9个 `data_block`。

![多次迭代非连续场景示意图](../../../../figures/多次迭代非连续场景示意图.png)

通过 C 风格接口配置 `repeat_stride` 和 `data_block_stride` 实现上述多次迭代功能。
```c
// 上述示例repeat间隔为9, 则通过如下配置可完成对应的操作
uint8_t repeat_time = 1;
uint8_t dst_block_stride = 1;
uint8_t src0_block_stride = 1;
uint8_t src1_block_stride = 1;
uint8_t dst_repeat_stride = 9;
uint8_t src0_repeat_stride = 9;
uint8_t src1_repeat_stride = 9;

asc_add(z, x, y, repeat_time, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
```
#### 3.1.3 掩码设置

针对同一个迭代中的数据，可以通过mask参数进行掩码操作来控制实际参与计算的个数。下图为进行Abs计算时通过mask逐比特模式按位控制哪些元素参与计算的示意图，1表示参与计算，0表示不参与计算。

<img src="../../../../figures/掩码操作示意图.png" alt="掩码操作示意图" width="800" />

每一位掩码对应数据中的一个元素的位置，通过有效位和无效位标记，实现对数据操作的精细化开关控制。
掩码由固定位宽的数值表示：有效位（通常为1）：对应元素正常参与计算。无效位（通常为0）：对应元素被屏蔽，不执行操作。

掩码设置提供了两种模式：**Counter模式**和**Normal模式**。

Counter模式用于计算连续N个元素。该模式下，Mask寄存器仅低64bit有效，表示参与计算的元素数量；repeat_time参数被忽略，系统根据元素数量自动计算迭代次数。
例如计算200个half类型元素时，受Unified Buffer的32B对齐约束，需申请256个元素空间。此时使用Counter模式，可精确处理实际元素数量，避免多余计算。

<img src="../../../../figures/Count模式计算.png" alt="Count模式计算" width="600" />

```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];
half scalar_value = 1.0;
// 1. Direct transfer processing (temporarily omitted)
// 2. Use high-dimensional segmentation API for computation
// 2.1 Set mask counter mode
asc_set_mask_count();
// 2.2 Set Mask value to 200, indicating this computation processes 200 elements
asc_set_vector_mask(0, static_cast<uint64_t>(200)); 
asc_add_scalar(dst_ub, src0_ub, scalar_value, 1, 1, 1, 1, 8, 8, 8);

// 3. Default needs to reset to Normal mode, if subsequent still requires Counter mode then no need to reset to Normal mode
asc_set_mask_normal();
```

在**Normal计算**模式中，MASK寄存器会作为每个Repeat的掩码使用。只有高维切分支持此模式，在接口上区分为逐bit模式和连续模式。
> 📌 Normal 为默认模式。若未发生模式切换，则无需重复配置该模式。

在Normal计算模式下，mask参数用来控制单次迭代内参与计算的元素个数，可通过`asc_set_vector_mask`来设置mask的值。

- 单次迭代内连续模式：表示前n个连续元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。

> **mask范围推导**：Vector计算单元单次迭代固定处理32B × 8 block = 256B数据，因此mask最大值 = 256B / sizeof(DataType)。当操作数为16位（sizeof=2B）时，mask∈[0, 128]（256/2=128）；当操作数为32位（sizeof=4B）时，mask∈[0, 64]（256/4=64）；当操作数为64位（sizeof=8B）时，mask∈[0, 32]（256/8=32）。此推导规则可推广至其他数据类型（如bfloat16=16bit→mask上限128，fp32=32bit→mask上限64）。

连续模式下，mask超出范围按照对应数据类型下mask最大值计算；mask = 0表示指令不会执行计算操作，该接口将被视为NOP（空操作）；若设置为负值可能产生未定义行为。

该模式下，可以指定单次迭代内计算前n个连续元素，其他数据不参与计算。例如：对src数据(shape(256)，数据类型为half)，该数据需通过2次迭代计算完（每次迭代计算128个元素），可通过设置mask = 100，表示第1次迭代计算[0, 99]范围的元素，第2次迭代计算[128, 227]范围的元素参与计算，其他元素不参与计算。

<img src="../../../../figures/单次迭代内连续计算.png" alt="单次迭代内连续计算" width="600" />

```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];
half scalar_value = 1.0;

// 1. Data transfer (temporarily omitted)
// 2. Use high-dimensional segmentation API for computation
// 2.1 Set mask Normal mode
asc_set_mask_normal();
// 2.2 Set Mask value to 100, indicating single iteration computes first 100 elements
asc_set_vector_mask(0, static_cast<uint64_t>(100)); 
asc_adds(dst_ub, src0_ub, scalar_value, 2, 1, 1, 1, 8, 8, 8);
```

- 单次迭代内逐比特模式: 可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。分为maskHigh（高位mask）和maskLow（低位mask）。参数取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，maskLow、maskHigh∈[0, 264-1]，并且不同时为0；当操作数为32位时，maskHigh为0，maskLow∈(0, 264-1]；当操作数为64位时，maskHigh为0，maskLow∈(0, 232-1]。

该模式下，可以指定maskHigh和maskLow的值，来处理各种掩码操作。如需进行交错计算，则mask设置成01010101...或者10101010...模式即可。
以src数据(shape(256)，数据类型为half)为例，通过设置逐bit的掩码，可在单迭代中选取特定bit位的数值参与计算：

<img src="../../../../figures/单次迭代内逐bit计算.png" alt="单次迭代内逐bit计算" width="600" />

```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];

// 1. Data transfer (temporarily omitted)
// 2. Use high-dimensional segmentation API for computation
// 2.1 Set mask Normal mode
asc_set_mask_normal();
// 2.2 Set Mask to bit-wise mode (0101...), implement interleaved computation
asc_set_vector_mask(0x000000000000AAAA, 0x0000000000000000); 
asc_add(dst_ub, src0_ub, src1_ub, 2, 1, 1, 1, 8, 8, 8);
```

### 3.2 连续计算

连续计算是C语言编程提供的简化计算接口，该能力由软件层面实现。开发者使用该接口无需配置mask掩码，连续计算针对源操作数的连续n个数据进行计算并连续写入目的操作数，解决一维数据的连续计算问题。

以Add接口为例，当开发者需要计算4096长度的向量加法，C语言编程通过如下形式调用：
```c
asc_add(z, x, y, 4096);
```

## Memory矢量计算同步控制

AI Core内部执行单元（如MTE2搬运单元、Vector计算单元等）以异步并行方式运行，读写同一存储资源时可能存在数据依赖关系。因此，Memory矢量计算需通过流水同步接口确保正确执行。Memory矢量计算流程较Cube矩阵计算更为简洁，主要包含三步：数据搬入（Global Memory → Unified Buffer）、计算（Unified Buffer）、数据搬出（Unified Buffer → Global Memory）。三个步骤分别对应PIPE_MTE2、PIPE_V、PIPE_MTE3流水线，需通过核内同步接口协调执行顺序，确保各步骤按序完成。

当开发者进行一个矢量加法计算时(Ascend 950PR/Ascend 950DT产品), 开发者可以通过`asc_lock`&`asc_unlock`的方式完成对这三个步骤的同步。
```c
__global__ __vector__ void add_kernel(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    uint8_t mutex_id = 1;
    // ...
    // 1. Step 1: Data transfer in, execution pipeline is PIPE_MTE2
    asc_lock(PIPE_MTE2, mutex_id);
    asc_copy_gm2ub_align(x_local, x_gm, 1, 48 * sizeof(half), 0, 0, false, 0, 0, 0);
    asc_copy_gm2ub_align(y_local, y_gm, 1, 48 * sizeof(half), 0, 0, false, 0, 0, 0);
    asc_unlock(PIPE_MTE2, mutex_id);

    // 2. Step 2: Compute, execution pipeline is PIPE_V
    asc_lock(PIPE_V, mutex_id);
    asc_add(z, x, y, 4096);
    asc_unlock(PIPE_V, mutex_id);
    
    // 3. Step 3: Data transfer out, execution pipeline is PIPE_MTE3
    asc_lock(PIPE_MTE3, mutex_id);
    asc_copy_ub2gm_align(z_gm, z_local, 1, 48 * sizeof(int8_t), 0, 0, 0);
    asc_unlock(PIPE_MTE3, mutex_id);
}
```

## Memory矢量计算示例

结合上述Memory矢量搬运与计算能力，开发者可基于C语言实现Memory矢量计算算子。具体开发实践可参考[c_api_add样例](../../../../../../examples/02_simd_c_api/00_introduction/01_add/c_api_delicacy_async_add/)，使用C语言编程接口完成Memory矢量算子的开发。
