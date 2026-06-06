# Memory矢量计算编程<a name="ZH-CN_TOPIC_0000002600000002"></a>

本文介绍如何在Ascend C中使用语言扩展C API编写基于Unified Buffer（以下简称UB）的**矢量计算代码**，该能力是Ascend C最基础、最通用的矢量编程方式。这种基于UB内存的编程方式称为Memory矢量计算编程，该能力主要在Ascend 910B上提供。

AI Core包含Vector计算单元（部署于AIV核）和Cube计算单元（部署于AIC核），分别用于矢量和矩阵类计算。矢量计算通过Vector计算单元完成，源操作数和目的操作数均通过Unified Buffer（UB）存储。

![硬件架构图3510](硬件架构图3510.png)

## Memory矢量计算数据流

Memory矢量计算的数据流为：`CopyIn` → `Compute` → `CopyOut`。核内参与Memory矢量计算的硬件单元包括：

- **矢量计算执行单元**：从UB读取数据，完成计算后将结果写回UB。
- **数据搬运单元**：负责在Global Memory（GM）和UB之间搬运数据。
- **标量计算单元**：处理矢量计算所需的标量计算（例如地址计算、循环控制）。

当我们描述一个常见的Vector矢量计算时，需要执行以下三个步骤：

1. 通过DMA将数据从Global Memory搬入到Local Memory
2. 进行矢量计算
3. 通过DMA将数据从Local Memory搬出到Global Memory

## 矢量搬运能力说明

Global Memory（GM）与Unified Buffer（UB）之间的数据搬运提供了多样化的接口，支持丰富的数据搬运场景。为实现高效的Vector计算，系统支持将数据从GM搬运至UB，供Vector计算单元进行向量计算；计算完成后，再将结果从UB搬回GM，完成数据闭环。

开发者通过灵活配置结构体参数，精确控制数据块数量、地址长度及间隔等关键属性，实现连续和非连续数据搬运（高维切分数据搬运），充分释放硬件的数据搬运与计算协同能力，提升向量算子的执行效率。

| 数据通路 | 功能    | C语言编程接口  | 适用场景描述 |
|---------|---------|---------------|-------------|
| gm2ub   | 连续数据搬运  | `asc_copy_gm2ub` | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运 |
| gm2ub   | 高维切分数据搬运  | `asc_copy_gm2ub` | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运 |
| gm2ub   | 非对齐数据搬运  | `asc_copy_gm2ub_align` | 支持数据的非对齐搬运，搬运到UB时可以自行填充数据 |
| ub2gm   | 连续数据搬运  | `asc_copy_ub2gm` | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续数据搬运 |
| ub2gm   | 高维切分数据搬运  | `asc_copy_ub2gm` | 提供基础的数据搬运能力，数据在传输过程中保持原始格式和内容不变，支持连续和非连续的数据搬运 |
| ub2gm   | 非对齐数据搬运  | `asc_copy_ub2gm_align` | 支持数据的非对齐搬运。 |

根据上述矢量搬运能力的描述, 开发者可以根据不同的场景选择合适的接口进行处理。

**高维切分数据搬运**: 当用户需要将多块非连续的`Global Memory`搬入到一块`Unified Buffer`时，可通过高维切分模式配置搬运数据块的个数、单个数据块的长度、每个数据块之间的间隔。以src_gm数据(shape(16, 256)，数据类型为half)为例，隔行将数据搬运到`Unified Buffer`中，可通过如下方式实现：
```c
// 连续数据块的个数：16行数据，每隔一行搬入UB，需搬运8行
uint16_t burst_count = 8;
// 单个数据块的长度（以32B为单位）：值为256 * sizeof(half) / 32
uint16_t burst_len = 256 * sizeof(half) / 32;
// 源数据前一块到后一块的间隔（以32B为单位）：前后块间隔两行，值为256 * 2 * sizeof(half) / 32
uint16_t src_stride = 256 * 2 * sizeof(half) / 32;
// 目标数据前一块到后一块的间隔（以32B为单位）：数据块连续，值为256 * sizeof(half) / 32
uint16_t dst_stride = 256 * sizeof(half) / 32;

asc_copy_gm2ub(dst_ub, src_gm, burst_count, burst_len, src_stride, dst_stride);
```
> 📌 stride（前一块数据头到后一块数据头的间隔）为该接口从Ascend 950PR/Ascend 950DT引入，Ascend 910B使用gap（前一个数据块结束地址与后一个数据块起始地址的差值）。

**连续数据搬运**: 如果开发者需要将一块连续的`Global Memory`搬运到`Unified Buffer`时，只需指定搬运数据大小（字节）即可。例如src_gm数据(shape(256)，数据类型为half)，需搬入到`Unified Buffer`中，通过`asc_copy_gm2ub`直接调用：
```c
uint32_t size = 256 * sizeof(half);
asc_copy_gm2ub(dst_ub, src_gm, size);
```
> 📌 该接口主要为了提升用户数据搬运的易用性，由软件层面提供，底层通过高维切分接口实现。

**高维切分数据搬运**提供了用户进行非连续多个数据块搬入的能力，但该接口要求`Global Memory`中搬运的长度和间隔都必须保证是32B对齐的，因此对于一些非对齐场景无法支持。例如将上述示例中的src_gm数据修改为(shape(16, 200)，数据类型为half)，则无法通过`asc_copy_gm2ub`完成隔行数据的搬运，此时需要突破32B的约束限制。Ascend C根据芯片能力，提供了`asc_copy_gm2ub_align`接口来支持该搬运形式，该接口可以指定以字节为单位的burst_len和stride信息，完成搬运操作。

![非对齐搬入能力](非对齐搬入能力.png)
以上图为例, 将两个数据块从GM搬运至UB，每个blockLen为54B，源操作数相邻数据块之间的间隔为1B，目的操作数相邻数据块之间的间隔为32B。
在blockLen左侧和右侧分别填充2个和3个half类型元素，此时blockLen + leftPadding + rightPadding = 54B + 2 * 2B + 3 * 2B = 64B，满足32B对齐。
```c
// 连续数据块的个数：搬运两个连续数据块
uint16_t burst_count = 2;
// 单个数据块的长度（以字节为单位）：值为54
uint16_t burst_len = 54;
// 源数据前一块到后一块的间隔：源数据间隔单位是1B
uint16_t src_stride = 1;
// 目标数据前一块到后一块的间隔：UB操作约束地址必须32B对齐，值为32 / 32
uint16_t dst_stride = 32 / 32;
// 搬运结果左侧填充（元素个数）：值为4 / sizeof(half) = 2
uint8_t left_padding_num = 2;
// 搬运结果右侧填充（元素个数）：值为6 / sizeof(half) = 3
uint8_t right_padding_num = 3;

asc_copy_gm2ub_align(dst, src, burst_count, burst_len, left_padding_num, right_padding_num, false, 0, src_stride, dst_stride);
```

如上就完成了从`Global Memory`到`Unified Buffer`的搬入动作，反之从`Unified Buffer`搬出到`Global Memory`中也采用同样的能力进行处理。

## Memory矢量计算能力说明

Memory矢量计算提供连续计算和高维切分两种计算模式，并可通过掩码实现数据选择性参与计算。
以Add接口为例，两种计算模式的调用方式示意图如下。其中，连续计算简单直观，适合一维Tensor连续数据处理；高维切分计算灵活可控，支持迭代执行和地址间隔配置。开发者可根据实际业务需求选择合适的计算方式。

![Memory矢量计算模式示意图](Memory矢量计算模式示意图.png)

### 高维切分
高维切分能力主要是支持对计算接口设置`repeat_time`、`stride`和`mask`参数等, 能精确控制指令的迭代执行、操作数的地址间隔和迭代内的掩码。

- 迭代控制
Vector计算单元每个迭代会从UB中取出8个DataBlock（每个DataBlock数据块内部地址连续，长度为32Byte）进行计算，并写入对应的8个DataBlock中。

![DataBlock迭代示意图](DataBlock迭代示意图.png)

矢量计算API支持开发者通过repeatTime来配置迭代次数，从而控制指令的多次迭代执行。假设repeat_time设置为2，矢量计算单元会进行2个迭代的计算，可计算出2 × 8（每个迭代8个DataBlock） × 32Byte = 512Byte的结果。如果数据类型为half，则计算了256个元素。下图展示了2次迭代Exp计算的示意图。由于硬件限制，repeat_time不能超过255。

![2次迭代Exp计算示意图](2次迭代Exp计算示意图.png)

- 掩码设置

针对同一个迭代中的数据，可以通过mask参数进行掩码操作来控制实际参与计算的个数。下图为进行Abs计算时通过mask逐比特模式按位控制哪些元素参与计算的示意图，1表示参与计算，0表示不参与计算。关于掩码的详细介绍请参考掩码。

![掩码操作示意图](掩码操作示意图.png)

- 地址间隔配置

矢量计算单元还支持带间隔的向量计算，通过`data_block_stride`（单次迭代内不同`data_block`间地址步长）和`repeat_stride`（相邻迭代间相同`data_block`的地址步长）来进行配置。

**data_block_stride**是指单次迭代内不同`data_block`间地址步长。其取值不能超过UB大小限制。
1. 连续计算，`data_block_stride`设置为1，对同一迭代内的8个`data_block`数据连续进行处理。
2. 非连续计算，`data_block_stride`值大于1（如取2），同一迭代内不同`data_block`之间在读取数据时出现一个`data_block`的间隔，如下图所示。

![data_block_stride示例](data_block_stride示例.png)

> 📌 若dst_block_stride == 0, 等同于dst_block_stride = 1; 若src_block_stride = 0, 则源操作数将始终复用第一个data_block。

下图给出了单次迭代内源操作数与目的操作数在UB空间的读写示例。示例中源操作数的`data_block_stride`配置为2，表示单次迭代内不同DataBlock间地址步长为2个DataBlock；目的操作数的`data_block_stride`配置为1，表示单次迭代内地址连续。

![单次迭代内源和目的操作数读写示例](单次迭代内源和目的操作数读写示例.png)

**repeat_stride**是指相邻迭代间相同`data_block`的地址步长。
1. 连续计算场景：假设定义一个Unified Buffer供目的操作数和源操作数同时使用（即地址重叠），`repeat_stride`取值为8。此时，矢量计算单元第一次迭代读取连续8个`data_block`，第二轮迭代读取下一个连续的8个`data_block`，通过多次迭代即可完成所有输入数据的计算。
![repeat_stride连续计算场景](repeat_stride连续计算场景.png)

2. 非连续计算场景：`repeat_stride`取值大于8（如取10）时，则相邻迭代间矢量计算单元读取的数据在地址上不连续，出现2个`data_block`的间隔。
![repeat_stride非连续计算场景](repeat_stride非连续计算场景.png)

3. 反复计算场景：`repeat_stride`取值为0时，矢量计算单元会对首个连续的8个`data_block`进行反复读取和计算。
![repeat_stride反复计算场景](repeat_stride反复计算场景.png)

4. 部分重复计算：`repeat_stride`取值大于0且小于8时，相邻迭代间部分数据会被矢量计算单元重复读取和计算，此种情形一般场景不涉及。
![repeat_stride部分重复计算](repeat_stride部分重复计算.png)

当repeatTime大于1，需要多次迭代完成矢量计算时，开发者可以根据不同的使用场景合理设置repeatStride的值。下图给出了多次迭代间非连续场景的示意图，示例中源操作数和目的操作数的repeatStride均配置为9，表示相邻迭代间相同DataBlock起始地址之间的间隔为9个DataBlock。相同DataBlock是指DataBlock在迭代内的位置相同，比如下图中的src1和src9处于相邻迭代，在迭代内都是第一个DataBlock的位置，其间隔即为repeatStride的数值。

![多次迭代非连续场景示意图](多次迭代非连续场景示意图.png)

C语言编程提供C风格的编程接口, `repeat_stride`和`data_block_stride`都通过入参的形式传递给计算接口；
```c
/*
 * 接口参数: 输入都为常量, 可直接传入常量值传入
 * repeat_time = 1;
 * dst_data_block_stride = 1;
 * src0_data_block_stride = 1;
 * src1_data_block_stride = 1;
 * dst_repeat_stride = 8;
 * src0_repeat_stride = 8;
 * src1_repeat_stride = 8;
 */ 
asc_add(z, x, y, 32, 1, 1, 1, 16, 16, 16);
```

- 掩码设置

Mask(掩码)是Ascend C中用于控制数据元素是否参与计算的标识位序列。每一位掩码对应数据中的一个元素的位置，通过有效位和无效位标记，实现对数据操作的精细化开关控制。
掩码由固定位宽的数值表示：
- 有效位（通常为1）：对应元素正常参与计算。
- 无效位（通常为0）：对应元素被屏蔽，不执行操作。

掩码设置提供了两种模式：Counter模式和Normal模式。

在**Counter模式**连续计算中，Mask寄存器中的值只有低64bit有效，被视为参与计算的元素数量。高维切分矢量计算接口中的repeat_time参数会被忽略，由系统内部根据元素数量自动计算。

例如需要计算200个元素（half数据类型）的矢量计算，因Unified Buffer的32B对齐约束，空间申请需要为256个元素大小，此时可通过Counter模式处理。
```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];

// 1. 直接进行搬运处理(暂省略)
// 2. 使用高维切分api进行计算
// 2.1 设置mask counter模式
asc_set_mask_count();
// 2.2 设置Mask值为200, 表示本条计算200个元素
asc_set_vector_mask(0, static_cast<uint64_t>(200)); 
asc_add(dst_ub, src0_ub, src1_ub, 1, 1, 1, 1, 8, 8, 8);

// 3. 默认需要设置回Normal模式, 如果后续依然需要Counter模式则不用设置回Normal模式
asc_set_mask_normal();
```

在**Normal计算**模式中，MASK寄存器会作为每个Repeat的掩码使用。只有高维切分支持此模式，在接口上区分为逐bit模式和连续模式。
> 📌 Normal 为默认模式。若未发生模式切换，则无需重复配置该模式。

在Normal计算模式下，mask参数用来控制单次迭代内参与计算的元素个数，可通过`asc_set_vector_mask`来设置mask的值。

- 单次迭代内连续模式：表示前n个连续元素参与计算。取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，mask∈[0, 128]；当操作数为32位时，mask∈[0, 64]；当操作数为64位时，mask∈[0, 32]。连续模式下，mask超出范围按照对应数据类型下mask最大值计算；mask = 0表示指令不会执行计算操作，该接口将被视为NOP（空操作）；若设成了负值可能产生未定义行为。

该模式下，可以指定单次迭代内计算前n个连续元素，其他数据不参与计算。例如：对src数据(shape(256)，数据类型为half)，该数据需通过2次迭代计算完（每次迭代计算128个元素），可通过设置mask = 100，表示迭代1[0, 99]，迭代2[128, 227]对应的元素参与计算，其他元素不参与计算。
```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];

// 1. 数据搬运（暂省略）
// 2. 使用高维切分API进行计算
// 2.1 设置mask Normal模式
asc_set_mask_normal();
// 2.2 设置Mask值为100，表示单次迭代计算前100个元素
asc_set_vector_mask(0, static_cast<uint64_t>(100)); 
asc_add(dst_ub, src0_ub, src1_ub, 2, 1, 1, 1, 8, 8, 8);
```

- 单次迭代内逐比特模式: 可以按位控制哪些元素参与计算，bit位的值为1表示参与计算，0表示不参与。分为maskHigh（高位mask）和maskLow（低位mask）。参数取值范围和操作数的数据类型有关，数据类型不同，每次迭代内能够处理的元素个数最大值不同。当操作数为16位时，maskLow、maskHigh∈[0, 264-1]，并且不同时为0；当操作数为32位时，maskHigh为0，maskLow∈(0, 264-1]；当操作数为64位时，maskHigh为0，maskLow∈(0, 232-1]。

该模式下，可以指定maskHigh和maskLow的值，来处理各种掩码操作。如需进行交错计算，则mask设置成01010101...或者10101010...模式即可。

以src数据(shape(256)，数据类型为half)为例，需执行交错运算，则通过`asc_set_vector_mask`来设置掩码：
```c
__ubuf__ half dst_ub[256];
__ubuf__ half src0_ub[256];
__ubuf__ half src1_ub[256];

// 1. 数据搬运（暂省略）
// 2. 使用高维切分API进行计算
// 2.1 设置mask Normal模式
asc_set_mask_normal();
// 2.2 设置Mask为逐比特模式（0101...），实现交错计算
asc_set_vector_mask(0x5555555555555555, 0x5555555555555555); 
asc_add(dst_ub, src0_ub, src1_ub, 2, 1, 1, 1, 8, 8, 8);
```

### 连续计算

连续计算是C语言编程提供的简化计算接口，该能力由软件层面实现。开发者使用该接口无需配置mask掩码，连续计算针对源操作数的连续n个数据进行计算并连续写入目的操作数，解决一维数据的连续计算问题。

以Add接口为例，当开发者需要计算4096长度的向量加法，C语言编程通过如下形式调用：
```c
asc_add(z, x, y, 4096);
```

## Memory矢量计算示例

结合上述Memory矢量搬运和计算的能力，开发者可以基于C语言实现Memory矢量计算算子。下面是一个矢量加法示例，实现c = a + b功能：
```c
__vector__ __global__ __aicore__ void add_custom(__gm__ float* x, __gm__ float* y, __gm__ float* z)
{
    asc_init();

    uint32_t block_len = C_API_TOTAL_LENGTH / asc_get_block_num();
    uint32_t tile_len = block_len / C_API_TILE_NUM;

    __gm__ float* xGm = x + asc_get_block_idx() * block_len;
    __gm__ float* yGm = y + asc_get_block_idx() * block_len;
    __gm__ float* zGm = z + asc_get_block_idx() * block_len;

    __ubuf__ float xLocal[C_API_TILE_LENGTH];
    __ubuf__ float yLocal[C_API_TILE_LENGTH];
    __ubuf__ float zLocal[C_API_TILE_LENGTH];

    uint16_t burst_len = tile_len;
    for (uint32_t i = 0; i < C_API_TILE_NUM; i++) {
        if (i != 0) {
            asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID0);
        }

        burst_len = tile_len * sizeof(float) / C_API_ONE_BLOCK_SIZE;
        asc_copy_gm2ub(xLocal, xGm + i * tile_len, 1, burst_len, 0, 0);
        asc_copy_gm2ub(yLocal, yGm + i * tile_len, 1, burst_len, 0, 0);

        asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
        asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);

        if (i != 0) {
            asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID0);
        }

        asc_add(zLocal, xLocal, yLocal, tile_len * sizeof(float) / C_API_ONE_REPEAT_BYTE_SIZE, 1, 1, 1, 8, 8, 8);

        if (i != (C_API_TILE_NUM-1)) {
            asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0);
        }

        asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
        asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);

        asc_copy_ub2gm(zGm + i * tile_len, zLocal, 1, burst_len, 0, 0);

        if (i != (C_API_TILE_NUM-1)) {
            asc_sync_notify(PIPE_MTE3, PIPE_V, EVENT_ID0);
        }
    }
}
```




