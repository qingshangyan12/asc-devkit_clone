# MatrixTranspose SIMD与SIMT混合访存优化路径样例

## 概述

本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程方式下的访存优化思路。样例包含一维连续复制基线，以及一系列逐步优化的转置核函数：从直接全局内存转置出发，引入MTE搬运与UB中转、对比按切分后的tile分组启动Thread Block与固定Thread Block数两种映射方式、通过UB padding消除bank冲突，最后借助双缓冲形式预留轮换buffer，完整呈现SIMD与SIMT混合编程场景下矩阵转置的调优路径。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 目录结构介绍

```text
├── simd_simt_matrix_transpose
│   ├── figures                     // README中的图片资源
│   ├── CMakeLists.txt              // 编译工程文件
│   ├── matrix_transpose.asc        // SIMD与SIMT混合矩阵转置优化路径实现
│   └── README.md
```

## 样例描述

- 计算公式：

  ```text
  output(x, y) = input(y, x)
  ```

  - input为输入矩阵，形状为 [H, W]，数据类型为float
  - output为输出矩阵，形状为 [W, H]，数据类型为float
  - 连续复制基线（Case 0/1）不执行转置，校验output与input相等

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">copy_kernel / transpose_naive_kernel / transpose_ub_2tile_kernel / transpose_ub_kernel / transpose_ub_pad_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过8个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

| Case | 核函数 | 线程块数量 | 每个线程块的线程数 | 本步引入的变化 |
| --- | --- | --- | --- | --- |
| 0 | `copy_kernel` | `core` | 2048 | 建立连续读写的耗时基线（高线程数），Vector侧通过MTE执行GM->UB->GM搬运 |
| 1 | `copy_kernel` | `core` | 1024 | 仅降低线程数，对照无同步场景下的线程数影响 |
| 2 | `transpose_naive_kernel` | `core` | 2048 | 写地址变为非连续，隔离非连续写的代价 |
| 3 | `transpose_ub_2tile_kernel` | `ceil(tiles / 2)` | 2048 | 引入MTE搬入UB与32×32分块，按切分后的tile分组启动Thread Block |
| 4 | `transpose_ub_2tile_kernel` | `core` | 2048 | Thread Block数固定为硬件vector core数，单个Thread Block循环处理多个tile分组 |
| 5 | `transpose_ub_kernel` | `core` | 1024 | 改为1024线程，单个Thread Block循环处理多个32×32的tile |
| 6 | `transpose_ub_pad_kernel` | `core` | 1024 | UB tile增加2列padding，消除转置读阶段的bank冲突 |
| 7 | `transpose_ub_pad_kernel` | `core` | 1024 | 双缓冲轮换形式，预留独立轮换buffer |

其中`core`为硬件vector core数，通过`aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`在运行时查询获得。`tiles`为总tile数`(W/32) × (H/32)`，1024×1024时为1024。

#### 线程块与tile映射

从Case 3开始，矩阵按32×32的tile处理。下图左侧表示1024×1024矩阵被划分为32×32个tile，右侧表示其中一个32×32的tile内的线程映射关系。tile内列索引记为`tx`，tile内行索引记为`ty`。

<img src="./figures/blockMapping.png" width="60%">

以图中绿色元素为例，可以根据tile坐标和tile内局部坐标推导它在GM中的input坐标。

- 图中绿色元素所在的tile坐标是`(2, 1)`。对于1024×1024矩阵，`tiles_x = 1024 / 32 = 32`，因此对应`tile_id = 2 * 32 + 1 = 65`。
- 图中绿色元素在tile内的局部坐标是`(2, 29)`，对应`ty = 2`、`tx = 29`。

代入坐标计算：

```cpp
uint32_t tile_y = tile_id / tiles_x;                     // 65 / 32 = 2
uint32_t tile_x = tile_id - tile_y * tiles_x;             // 65 - 2 * 32 = 1
uint32_t x_index = tile_x * TILE_DIM + tx;                // 1 * 32 + 29 = 61
uint32_t y_index = tile_y * TILE_DIM + ty;                // 2 * 32 + 2  = 66
uint32_t index = x_index + width * y_index;              // 61 + 1024 * 66
```

因此该线程在GM中处理的输入元素坐标为`input[66,61]`。

#### 混合编程数据流

Case 3及之后的转置主路径把GM到UB的数据搬运放在Vector侧MTE执行，再通过`asc_vf_call`调用`__simt_vf__`函数完成SIMT线程级转置读取与GM写回。MTE搬入一个32×32的tile到UB的核心代码如下：

```cpp
asc_set_gm2ub_loop_size(TILE_DIM, 1);
asc_set_gm2ub_loop1_stride(width * sizeof(float), ub_row_bytes);
asc_copy_gm2ub_align(dst, src, 1, TILE_ROW_BYTES, 0, 0, false, 0, 0, 0);
asc_set_gm2ub_loop_size(1, 1);
```

MTE搬运完成后，通过流水同步保证SIMT VF读取到完整tile，再由SIMT VF按转置方向读取UB并连续写回GM：

```cpp
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_vf_call<simt_store_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, tile, height, tiles_x, tile_id);
asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0);
asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID0);
```

UB中转的核心思想如下图所示：GM读取阶段按输入矩阵行方向连续搬入UB；GM写回阶段交换输出tile坐标，使同一个warp写出输出矩阵的一行。原本非连续的GM写被转移为UB内转置方向读取。

<img src="./figures/case3.png" width="60%">

### 性能指标说明

| 指标 | 说明 |
| --- | --- |
| Task Duration(μs) | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间 |
| aiv_time(μs) | Task在AI Vector Core上的理论执行时间，单位为μs |
| aiv_total_cycles | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行cycle总数 |
| aiv_vec_time(μs) | vec类型指令（向量类运算指令）耗时，单位为μs |
| aiv_vec_ratio | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比 |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位为μs |
| aiv_scalar_ratio | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比 |

除Task Duration外，其余指标均为所有Thread Block上的平均值。

### Case 0 / Case 1：MTE连续复制基线

**优化目标**：实测当前硬件上纯连续GM读写的数据搬运能力，作为后续转置实现可逼近的性能上限。

**核心实现**：将矩阵视为一维数组，Vector核函数通过MTE将连续数据从GM搬到UB，再从UB写回GM；SIMT VF只参与对UB tile的简单触达，用于保持混合编程调用路径。Case 0采用2048线程/Thread Block，Case 1采用1024线程/Thread Block。

```cpp
__global__ __vector__ void copy_kernel(__gm__ float *output, __gm__ float *input, uint32_t elements)
{
    __ubuf__ float tile[COPY_ELEMENTS_PER_BLOCK];
    ...
    asc_copy_gm2ub_align(tile, input + offset, 1, copy_bytes, 0, 0, false, 0, 0, 0);
    sync_mte2_to_v();
    asc_vf_call<simt_fill_copy_tile>(dim3(THREADS_PER_BLOCK), tile, copy_elements);
    sync_v_to_mte3();
    asc_copy_ub2gm_align(output + offset, tile, 1, copy_bytes, 0, 0, 0);
    sync_mte3_to_mte2();
}
```

**性能数据**：

| Case | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| --- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0 | 7.158 | 5.811 | 9587.5 | 1.799 | 0.312 | 0.468 | 0.080 |
| 1 | 6.721 | 5.493 | 9063.8 | 1.442 | 0.265 | 0.458 | 0.083 |

**分析**：

Case 0/1只执行一维连续读写，不涉及坐标交换、UB中转和Thread Block内同步，主要用于实测当前硬件在该数据规模下的GM连续搬运能力。1024×1024的float矩阵复制需要从GM读取4MB数据并写回4MB数据，读写总数据量为：

$$D = 1024 \times 1024 \times 4B \times 2 = 8.39MB$$

由于后续转置Case在完成坐标交换的同时还会引入UB访问和流水同步开销，因此Case 0的连续复制耗时可作为矩阵转置优化的性能上限参照。

---

### Case 2：直接全局内存转置

**优化目标**：实现最直接的转置，隔离非连续全局内存写相对复制基线的代价。

**核心实现**：不分块、不使用UB。SIMT线程读取`input(row, col)`，直接写入转置位置`output(col, row)`。全局内存读取连续，写入地址按列跨行，为非连续写。

```cpp
uint32_t row = i / width;
uint32_t col = i - row * width;
// input(row,col) -> output(col,row)
output[col * height + row] = input[i];
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 36.464 | 33.516 | 55301.3 | 33.096 | 0.988 | 0.409 | 0.013 |

**分析**：

Case 2在保持GM连续读取的同时，直接把数据写入转置后的输出位置，导致同一warp内相邻线程的写地址分布到输出矩阵的不同行。相比Case 0的连续复制基线，非连续GM写会显著增加耗时。后续优化需要把非连续访问从GM转移到UB，使GM读写转变为连续访问模式。

---

### Case 3：MTE搬入UB后转置写回，每个Thread Block处理两个32×32的tile

**优化目标**：引入MTE搬入UB与分块，将Case 2的非连续全局内存写转换为UB内访问，使全局内存读写均恢复连续；本Case按切分后的tile分组启动Thread Block，每个Thread Block覆盖两个32×32的tile。

**核心实现**：将矩阵划分为32×32的tile，每个Thread Block使用2048线程处理两个tile，因此`blocks_per_grid=ceil(tiles / 2)`。Vector侧MTE将GM中的tile按行方向连续搬入UB，搬运完成后通过流水同步通知SIMT VF；SIMT VF配置参数为`dim3(32,64,1)`，其中`threadIdx.y=0..31`的线程处理第一个tile，`threadIdx.y=32..63`的线程处理第二个tile，代码通过`local_tile=threadIdx.y>>5`得到当前线程所属的tile编号，按转置方向读取UB并连续写回GM。

UB中转的核心思想如下图所示：GM读取阶段按输入矩阵行方向连续搬入UB；GM写回阶段交换输出tile坐标，使同一个warp写出输出矩阵的一行。原本非连续的GM写被转移为UB内转置方向读取。

<img src="./figures/case3.png" width="60%">

转置操作需要交换行列坐标，如果直接逐元素写回，输出地址会沿列方向跨行分散。分块后的处理粒度从单个元素变为局部tile：MTE先按输入矩阵的行方向把tile连续读入UB，SIMT VF再从UB中按转置方向取数，最后按输出矩阵的行方向连续写回GM。这样，转置带来的不连续访问被限制在UB内部，GM读写都保持连续访问模式。

tile大小选为32×32，是因为`threadIdx.x`方向的32个线程可以对应tile内一行元素，使一个warp的GM读写访问覆盖32个连续float；同时32×32的tile含1024个元素，便于在1024线程配置下由一个Thread Block处理一个tile，或在2048线程配置下由一个Thread Block处理两个tile。

```cpp
__ubuf__ float tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];
uint32_t local_tile = threadIdx.y >> 5;                // 0或1，区分Thread Block内两个tile
uint32_t ty = threadIdx.y & (TILE_DIM - 1);
uint32_t tx = threadIdx.x;

uint32_t tile_id = tile_base + local_tile;
uint32_t tile_y = tile_id / tiles_x;
uint32_t tile_x = tile_id - tile_y * tiles_x;
uint32_t x_index = tile_y * TILE_DIM + tx;
uint32_t y_index = tile_x * TILE_DIM + ty;
output[x_index + y_index * height] = tile[local_tile * TILE_ELEMENTS + tx * TILE_DIM + ty];
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 23.417 | 2.272 | 3749.0 | 1.674 | 0.738 | 0.179 | 0.077 |

**分析**：

Case 3通过MTE搬入UB中转把非连续GM写转换为UB内转置读取，GM读写连续化已经带来收益。该实现按切分后的tile分组设置线程块数量，1024个tile对应512个Thread Block，线程块数量明显超过硬件vector core数，额外的线程块启动和调度开销会反映到Task Duration中。为了减少这部分开销，Case 4将Thread Block数固定为硬件vector core数，每个Thread Block通过循环处理多个tile分组。

---

### Case 4：MTE搬入UB后转置写回，固定Thread Block数，每轮处理两个32×32的tile

**优化目标**：将Case 3的线程块数量从切分后的tile分组数改为硬件vector core数，每个Thread Block通过循环处理多个tile分组。

**核心实现**：每个Thread Block启动2048个线程，配置参数为`dim3(32,64,1)`。循环每轮会同时处理两个连续的32×32的tile：`threadIdx.y=0..31`的线程处理第一个tile，`threadIdx.y=32..63`的线程处理第二个tile；下一轮按`gridDim.x*2`跳到后续tile分组。

```cpp
for (uint32_t tile_base = loop_start; tile_base < loop_end; tile_base += loop_step) {
    // MTE搬入两个tile到UB
    copy_gm_tile_to_ub(&tile[local_tile][0][0], input + tile_offset, width);
    sync_mte2_to_v();
    // SIMT VF按转置方向读取UB并写回GM
    asc_vf_call<simt_store_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1), output, ...);
    sync_v_to_mte2();
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 18.617 | 17.463 | 28813.8 | 13.505 | 0.774 | 0.677 | 0.039 |

**分析**：

Case 4将Thread Block数固定为硬件vector core数。相比Case 3启动512个Thread Block的写法，Case 4需要启动和调度的线程块数量更少。增加`--cce-res-usage`编译选项后，可以在编译日志中看到寄存器和栈的使用情况。2048线程配置下`__launch_bounds__(2048)`限制了每个线程最多可使用的寄存器数量，索引逻辑变复杂后可能出现寄存器溢出，抵消固定Thread Block数带来的收益。Case 5改用1024线程配置，让一个Thread Block每轮只处理一个tile，先消除寄存器溢出。

---

### Case 5：MTE搬入UB后转置写回，固定Thread Block数（1024线程）

**优化目标**：改为1024线程，将单轮处理粒度调整为一个32×32的tile，确定后续优化的基础配置。

**核心实现**：在Case 4的基础上改为每个Thread Block启动1024个线程，配置参数为`dim3(32,32,1)`。一个32×32的tile包含1024个元素，因此一个Thread Block在循环每轮正好处理一个tile；下一轮按`gridDim.x`跳到后续tile。

```cpp
__ubuf__ float tile[TILE_DIM][TILE_DIM];
...
copy_gm_tile_to_ub(&tile[0][0], input + tile_offset, width);
sync_mte2_to_v();
asc_vf_call<simt_store_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, &tile[0][0], height, tiles_x, tile_id);
sync_v_to_mte2();
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 22.709 | 21.512 | 35494.4 | 15.115 | 0.703 | 0.714 | 0.033 |

**分析**：

改为1024线程后，索引逻辑更简单，也更接近一个tile对应一个Thread Block的自然映射。相对Case 2的直接转置，UB中转将非连续全局内存写转换为UB内访问，配合固定Thread Block数的循环处理方式可显著降低耗时。

Case 5仍保留了一个新的瓶颈：UB转置读阶段需要按列读取`tile[tx][ty]`，会产生bank冲突。线程组织已经回到1024线程常驻配置，Case 6沿用该配置，只调整UB布局来处理转置读阶段的bank冲突。

---

### Case 6：MTE搬入UB后转置写回+padding

**优化目标**：在Case 5的基础上，通过UB padding缓解转置读阶段的bank冲突。

**核心实现**：

- GM到UB阶段与Case 5一致，仅UB目的行跨度由`32 * sizeof(float)`改为`40 * sizeof(float)`。
- 差异只在UB布局，从32×32改成32×40。
- MTE搬入时通过`asc_set_gm2ub_loop1_stride`将UB目的行跨度设置为`40 * sizeof(float)`，使每行末尾保留8列padding，后续SIMT VF仍只读取有效的32列。
- 该版本没有改变算法路径，也没有改变Thread Block和线程切分，只是调整了UB中的物理布局。

> **说明：** 在纯SIMT版本中，padding列数取2即可让行跨度（34个float=17个subbank）与32个subbank互质，从而完全消除读读冲突。但在SIMD与SIMT混合版本中，UB数据由Vector侧MTE搬入，MTE搬运要求UB目的地址按32B对齐，而行跨度`34 * sizeof(float) = 136B`不是32B的整数倍，会触发"MTE non-atomic instruction address is not aligned"硬件错误。因此混合版本将padding列数取8，使行跨度`40 * sizeof(float) = 160B`满足32B对齐。该约束下行跨度为20个subbank，与32个subbank的最大公约数为4，无法完全消除冲突，但可将Case 5的16路读读冲突缓解为4路。

下面以Ascend 950PR/Ascend 950DT的UB划分规则为例，说明本样例中bank冲突是如何产生的，以及Case 5和Case 6理论上的冲突强度差异。

UB中的bank划分如下图所示。UB总大小256KB，划分为16个bank（每个bank512行×32B=16KB）；这16个bank进一步组织为8个bank group，每个bank group包含2个bank（bank i与bank i+8同属bank group i，即`bank_group_id=bank_id%8`）。在SIMT编程模式下，每个bank又会被划分为4个subbank，每个subbank的宽度为8B。

SIMT场景下，同一个warp内的多个线程可能在同一条UB访问指令中同时访问UB；当这些访问数据属于同一个bank group的相同编号的subbank资源时，硬件无法在一个周期内处理全部请求，需要排队等待，从而形成subbank冲突并增加访问延迟；如果这些访问数据属于同一个subbank的同一行内的8B地址范围，硬件会将这些请求合并处理，不会形成subbank冲突。

<img src="./figures/bank结构示意图.png">

**地址采用低位交织：** 如下图所示，UB内连续地址按32B粒度映射到bank0～bank15：第1个32B地址段映射到bank0，第2个映射到bank1，依次类推；第16个映射到bank15，第17个映射回bank0的下一行。

<img src="./figures/bank内存排布示意图.png">

SIMT编程方式下subbank冲突主要有以下几种：

- **写写冲突**：多个写操作同时尝试访问同一个bank group的相同编号的subbank。
- **读读冲突**：多个读操作同时尝试访问同一个bank group的相同编号的subbank。

Case 5中UB的tile数组按照行优先存储。按照地址低位交织规则，tile数组的第一行刚好覆盖bank0～bank3，第二行覆盖bank4～bank7，第三行覆盖bank8～bank11，其余行依次类推。每行32个float数据会恰好跨越4个bank存储。Case 5中，一个warp的线程会读取tile的一列元素写回GM输出；访问UB时，32个线程会集中访问两个bank group的subbank0，属于读读冲突场景。

<img src="./figures/case5bank.png">

Case 6中对UB中的tile数组增加8列padding，由每行32个元素改为40个元素，其在UB上的排列如下图所示。由于每行有40个元素，行跨度变为20个subbank，与32个subbank的最大公约数为4。Case 6访问UB时，32个线程的访问会从Case 5的2个subbank扩展到8个subbank，每个subbank仅4个线程访问，将16路读读冲突缓解为4路（下图按纯SIMT版本的34列padding绘制，混合版本因MTE 32B对齐约束改用40列，缓解原理一致，冲突路数由0路变为4路）。

<img src="./figures/case6bank.png">

```cpp
constexpr int TILE_PAD = 8;                          // Case6/7的UB padding列数（32x40，满足MTE 32B对齐）
constexpr int TILE_PAD_STRIDE = TILE_DIM + TILE_PAD;
__ubuf__ float tile[TILE_DIM][TILE_PAD_STRIDE];
...
copy_gm_tile_to_ub(&tile[0][0], input + tile_offset, width, TILE_PAD_STRIDE * sizeof(float));
sync_mte2_to_v();
asc_vf_call<simt_store_pad_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, &tile[0][0], height, tiles_x, tile_id);
sync_v_to_mte2();
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 15.640 | 14.466 | 23868.2 | 7.967 | 0.551 | 0.742 | 0.051 |

**分析**：

相对Case 5的22.709μs，padding将Task Duration降至15.640μs，下降约31%。该优化仅调整UB物理布局，不改变GM读写坐标和算法路径，说明转置读阶段的bank冲突是Case 5的主要瓶颈。受MTE 32B对齐约束，混合版本只能将16路冲突缓解为4路，但收益已经比较显著。消除大部分bank冲突后，每轮循环仍保留MTE搬入后和SIMT VF写回后两次流水同步，同步开销成为后续可压缩的部分。

---

### Case 7：MTE搬入UB后转置写回+padding+double buffer形式（1024线程）

**优化目标**：使用两份带padding的UB tile按`cnt`轮换，并通过双event id让MTE2搬入与SIMT VF转置处理真正重叠并行，隐藏MTE2搬运耗时。

**核心实现**：Case 6每轮循环包含两次流水同步（`sync_mte2_to_v`和`sync_v_to_mte2`），这两次同步都是"notify后立即wait"的屏障形式，导致MTE2搬入与SIMT VF处理严格串行：MTE2搬完才允许VF读，VF读完才允许下一轮MTE2写。本版本在Case 6的padding基础上做两点改造：

- **双buffer物理隔离**：使用两份`32×40`的UB tile，按`cnt`轮换，下一轮MTE2写入另一份buffer，不会覆盖当前轮正在被SIMT VF读取的buffer。
- **双event id重叠并行**：为两份buffer各绑定一个event id（偶数轮`EVENT_ID0`、奇数轮`EVENT_ID1`），并用`SetFlag`（非阻塞发信号）与`WaitFlag`（阻塞等信号）分离的写法替代屏障式同步。循环前预置两份buffer的`V_MTE2` flag，使前两轮MTE2搬入无需等待；循环内MTE2搬入只需等"两轮前同一buffer的VF处理完成"，从而让第N轮的MTE2搬入与第N-1轮的SIMT VF处理在不同流水线上同时执行。

```cpp
__ubuf__ float tile[2][TILE_DIM][TILE_PAD_STRIDE]; // 双缓冲：两份带padding的UB tile
// 循环前预置信号，使前两轮MTE2搬入不等VF
if ASC_IS_AIV {
    asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0);
    asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID1);
}
for (uint32_t tile_id = block_idx; tile_id < total_tiles; tile_id += block_num) {
    uint32_t eventId = (cnt == 0) ? EVENT_ID0 : EVENT_ID1;
    // MTE2：等VF释放该buffer，再搬入
    if ASC_IS_AIV { asc_sync_wait(PIPE_V, PIPE_MTE2, eventId); }
    copy_gm_tile_to_ub(&tile[cnt][0][0], input + tile_offset, width, TILE_PAD_STRIDE * sizeof(float));
    if ASC_IS_AIV { asc_sync_notify(PIPE_MTE2, PIPE_V, eventId); }
    // V：等MTE2搬完该buffer，再转置读UB并写GM
    if ASC_IS_AIV { asc_sync_wait(PIPE_MTE2, PIPE_V, eventId); }
    asc_vf_call<simt_store_pad_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, &tile[cnt][0][0], height, tiles_x, tile_id);
    if ASC_IS_AIV { asc_sync_notify(PIPE_V, PIPE_MTE2, eventId); } // 通知MTE2：该buffer可复用
    cnt ^= 1;
}
// 循环后等最后两个buffer的VF处理完成
if ASC_IS_AIV {
    asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE2, EVENT_ID1);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 9.855 | 8.558 | 14119.5 | 7.720 | 0.903 | 0.725 | 0.085 |

**分析**：

相对Case 6的15.640μs，双缓冲重叠并行将Task Duration降至9.855μs，下降约37%。在该数据规模（1024×1024、64个vector core、每核16个tile）下，每个Thread Block的16轮tile处理中，除首尾各一轮无法重叠外，中间14轮的VF处理均与相邻轮的MTE2搬入并行执行。Case 7是本样例中耗时最低的版本，相对Case 0复制基线（7.158μs）约为1.38倍，差距主要来自MTE2搬入本身的开销及首尾两轮不可重叠的部分。

---

## 性能对比总结

### Ascend 950PR性能数据

| Case | 核函数 | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| --- | --- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 0 | `copy_kernel` | 7.158 | 5.811 | 9587.5 | 1.799 | 0.312 | 0.468 | 0.080 |
| 1 | `copy_kernel` | 6.721 | 5.493 | 9063.8 | 1.442 | 0.265 | 0.458 | 0.083 |
| 2 | `transpose_naive_kernel` | 36.464 | 33.516 | 55301.3 | 33.096 | 0.988 | 0.409 | 0.013 |
| 3 | `transpose_ub_2tile_kernel` | 23.417 | 2.272 | 3749.0 | 1.674 | 0.738 | 0.179 | 0.077 |
| 4 | `transpose_ub_2tile_kernel` | 18.617 | 17.463 | 28813.8 | 13.505 | 0.774 | 0.677 | 0.039 |
| 5 | `transpose_ub_kernel` | 22.709 | 21.512 | 35494.4 | 15.115 | 0.703 | 0.714 | 0.033 |
| 6 | `transpose_ub_pad_kernel` | 15.640 | 14.466 | 23868.2 | 7.967 | 0.551 | 0.742 | 0.051 |
| 7 | `transpose_ub_pad_kernel` | 9.855 | 8.558 | 14119.5 | 7.720 | 0.903 | 0.725 | 0.085 |

**综合优化效果**：

- 转置主路径Case 2 -> Case 5 -> Case 6 -> Case 7，Task Duration由36.464μs依次降至22.709μs、15.640μs、9.855μs，相对Case 2提升约3.70倍。
- 耗时最低的Case 7相对Case 0复制基线约为1.38倍，差距主要来自MTE2搬入本身的开销及双缓冲首尾两轮不可重叠的部分。

## 调优建议

访存类算子调优时，建议先用连续复制确认当前规模下的GM搬运上限，再用最直接的实现暴露主要差距。本样例中，直接转置与复制基线的差距主要来自非连续GM写，因此优化方向不是增加计算并行度，而是重排访存：以32×32的tile为单位由MTE连续读入UB，在UB内完成转置方向读取，再按输出矩阵行方向连续写回GM。

分块后需要先确定Thread Block到tile的映射方式。按切分后的tile分组启动Thread Block实现简单，但当tile数远大于硬件vector core数时，会带来额外的启动和调度开销；将Thread Block数固定为硬件vector core数，并在kernel内循环处理多个tile，可以降低这部分开销。线程数也不能只参考连续搬运场景，高线程数会限制每个线程可使用的寄存器数量，索引逻辑变复杂后可能出现寄存器溢出，需要结合`--cce-res-usage`和实测耗时选择。

当GM访问已经转为连续后，瓶颈会转移到UB访问和流水同步。转置读UB时，如果按列访问32×32的tile，容易出现bank冲突，可以通过padding改变UB物理布局；混合编程场景下MTE搬入与SIMT VF处理之间需要流水同步，单buffer时同步是"MTE2搬完才VF、VF完才下一轮搬入"的串行屏障。双缓冲要真正发挥作用，仅声明两份buffer还不够，必须同时用双event id（`EVENT_ID0`/`EVENT_ID1`）分别绑定两份buffer，并用`SetFlag`（非阻塞发信号）与`WaitFlag`（阻塞等信号）分离的写法替代屏障式同步，循环前预置flag使前两轮不等、循环后等末轮完成，从而让第N轮的MTE2搬入与第N-1轮的SIMT VF处理在不同流水线上并行执行，将VF计算完全隐藏在MTE2搬运背后。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  
  请根据当前环境上CANN开发套件包的[安装方式](../../../../docs/quick_start.md#prepare&install)，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=7                                                                     # 选择执行场景，可选0-7
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./matrix_transpose                                                        # 执行样例
  ```

  程序第一个参数为方阵边长，必须为32的整数倍。

- 编译选项说明

  | 选项                       | 可选值      | 说明                                                       |
  | -------------------------- | ----------- | ---------------------------------------------------------- |
  | `CMAKE_ASC_ARCHITECTURES`  | `dav-3510`  | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`             | `0`-`7`     | 样例类型，默认为7                                          |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能分析

使用`msprof`工具获取详细性能数据：

```bash
msprof op ./build/matrix_transpose
```

命令执行完成后，会在输出目录生成以`OPPROF_{timestamp}_XXX`命名的文件夹，性能数据文件夹结构示例如下：

```text
├── dump                       // 原始性能数据
├── ArithmeticUtilization.csv  // cube/vector指令cycle占比
├── L2Cache.csv                // L2 Cache命中率
├── Memory.csv                 // UB、L1和主存储器读写带宽
├── MemoryL0.csv               // L0A、L0B、L0C读写带宽
├── MemoryUB.csv               // Vector和Scalar到UB的读写带宽
├── OpBasicInfo.csv            // 算子基础信息，包含Task Duration(μs)
├── PipeUtilization.csv        // 计算单元和搬运单元耗时及占比
├── ResourceConflictRatio.csv  // UB bank group、bank conflict和资源冲突率
└── visualize_data.bin         // MindStudio Insight呈现文件
```
