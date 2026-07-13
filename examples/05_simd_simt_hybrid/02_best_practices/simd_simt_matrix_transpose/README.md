# MatrixTranspose SIMD与SIMT混合访存优化路径样例

## 概述

本样例以矩阵转置为例，介绍Ascend C SIMD与SIMT混合编程方式下的访存优化思路。样例从直接全局内存转置出发，引入MTE搬运、UB中转与32×32分块，对比按tile分组启动Thread Block与固定Thread Block数两种映射方式，通过UB padding降低bank冲突，最后借助双缓冲（Double Buffer）使MTE2搬运流水和SIMT VF流水并行，呈现SIMD与SIMT混合矩阵转置的调优路径。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN版本

- CANN >= 9.1.0

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

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transpose_naive_kernel / transpose_ub_2tile_kernel / transpose_ub_pad_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过5个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

| Case | 核函数                         | 线程块数量        | 每个线程块的线程数 | 本步引入的变化                                                            |
| ---- | ------------------------------ | ----------------- | ------------------ | ------------------------------------------------------------------------- |
| 0    | `transpose_naive_kernel`       | `core`            | 2048               | 直接在GM中完成转置                                                        |
| 1    | `transpose_ub_2tile_kernel`    | `ceil(tiles / 2)` | 2048               | 引入MTE搬运、UB中转和32×32分块，按tile分组数量设置Thread Block数          |
| 2    | `transpose_ub_2tile_kernel`    | `core`            | 2048               | Thread Block数固定为硬件vector core数，单个Thread Block循环处理多个tile组 |
| 3    | `transpose_ub_pad_kernel`      | `core`            | 2048               | UB tile增加8列padding，降低转置读阶段的bank冲突                           |
| 4    | `transpose_ub_pad_kernel`      | `core`            | 2048               | 双缓冲轮换，使MTE2搬运流水和SIMT VF流水并行                               |

其中 `core`为硬件vector core数，通过 `aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`在运行时查询获得。本样例中的性能数据基于 `core=64` 的测试环境采集得到。`tiles`为总tile数 `(W/32) × (H/32)`；处理1024×1024矩阵时，矩阵会被划分为32×32个tile，因此 `tiles=1024`。

#### 线程块与tile映射

从Case 1开始，矩阵按32×32的tile处理。下图左侧表示1024×1024矩阵被划分为32×32个tile，右侧表示其中一个32×32的tile内的线程映射关系。tile内列索引记为 `tx`，tile内行索引记为 `ty`。

**图1　Thread Block与tile的映射示意图**

<img src="./figures/block_mapping.png" width="60%">

以图中绿色元素为例，可以根据tile坐标和tile内局部坐标推导它在GM中的input坐标。

- 图中绿色元素所在的tile坐标是 `(2, 1)`。对于1024×1024矩阵，`tiles_x = 1024 / 32 = 32`，因此对应 `tile_id = 2 * 32 + 1 = 65`。
- 图中绿色元素在tile内的局部坐标是 `(2, 29)`，对应 `ty = 2`、`tx = 29`。

代入坐标计算：

```cpp
uint32_t tile_y = tile_id / tiles_x;                     // 65 / 32 = 2
uint32_t tile_x = tile_id - tile_y * tiles_x;             // 65 - 2 * 32 = 1
uint32_t x_index = tile_x * TILE_DIM + tx;                // 1 * 32 + 29 = 61
uint32_t y_index = tile_y * TILE_DIM + ty;                // 2 * 32 + 2  = 66
uint32_t index = x_index + width * y_index;              // 61 + 1024 * 66
```

因此该线程在GM中处理的输入元素坐标为 `input[66,61]`。

#### SIMD与SIMT混合数据流

Case 1及后续转置路径中，GM到UB的数据搬运由Vector侧MTE完成，再通过 `asc_vf_call`调用SIMT VF函数完成SIMT线程级转置读取与GM写回。

UB中转的核心思想如下图所示：GM读取阶段按输入矩阵行方向连续搬入UB；GM写回阶段交换输出tile坐标，使同一个Warp写出输出矩阵的一行。原本非连续的GM写被转移为UB内转置方向读取。

**图2　UB中转转置数据流向示意图**

<img src="./figures/ub_transpose_dataflow.png" width="60%">

### 性能指标说明

| 指标                 | 说明                                                                                      |
| -------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(μs)    | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |
| aiv_time(μs)         | Task在AI Vector Core上的理论执行时间，单位为μs                                            |
| aiv_total_cycles     | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行cycle总数 |
| aiv_vec_time(μs)     | vec类型指令（向量类运算指令）耗时，单位为μs                                               |
| aiv_vec_ratio        | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比                           |
| aiv_scalar_time(μs)  | scalar类型指令（标量类运算指令）耗时，单位为μs                                            |
| aiv_scalar_ratio     | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比                        |

除Task Duration外，其余指标均为所有Thread Block上的平均值。

### Case 0：直接全局内存转置

**优化目标**：在GM上直接进行坐标变换，观察直接写回转置地址带来的耗时变化。

**核心实现**：不分块、不使用UB。SIMT线程读取 `input(row, col)`，直接写入转置位置 `output(col, row)`。全局内存读取连续，写入地址按列跨行，为非连续写。

```cpp
uint32_t row = i / width;
uint32_t col = i - row * width;
// input(row,col) -> output(col,row)
output[col * height + row] = input[i];
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      36.140       |    33.793    |     55757.9      |      33.338      |     0.987     |        0.443        |      0.013       |

**分析**：

Case 0在保持GM连续读取的同时，直接把数据写入转置后的输出位置，导致同一Warp内相邻线程的写地址分布到输出矩阵的不同行。由于矩阵转置本身计算量很小，该版本的主要瓶颈来自GM非连续写。后续优化需要把非连续访问从GM转移到UB，使GM读写转变为连续访问模式。

---

### Case 1：MTE搬入UB后转置写回，每个Thread Block处理两个32×32的tile

**优化目标**：引入MTE搬运、UB中转和32×32分块，将Case 0的非连续全局内存写转换为UB内访问，使全局内存读写尽量连续。本Case按切分后的tile分组数量设置Thread Block数，每个Thread Block处理两个32×32的tile。

**核心实现**：将矩阵划分为32×32的tile，总tile数为 `tiles=(W/32) × (H/32)`。本Case中每个Thread Block启动2048个线程，可同时处理两个tile，因此Thread Block数量设置为 `ceil(tiles / 2)`。Vector侧MTE按输入矩阵行方向连续搬入tile，SIMT VF按转置方向读取UB，并按输出矩阵行方向连续写回GM。

```cpp
__ubuf__ float tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];

copy_gm_tile_to_ub(&tile[local_tile][0][0], input + tile_offset, width);
sync_mte2_to_v();
asc_vf_call<simt_store_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                              output, &tile[0][0][0], width, height, tiles_x,
                              tile_base, total_tiles);
sync_v_to_mte2();
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      23.215       |    2.281     |      3764.3      |      1.672       |     0.735     |        0.184        |      0.078       |

**分析**：

Case 1通过UB中转把非连续GM写转换为UB内转置读取，Task Duration从Case 0的36.140μs降至23.215μs，耗时下降约35.8%。该版本按tile分组数量启动Thread Block，1024×1024矩阵共有1024个tile，对应512个Thread Block，明显超过硬件vector core数。额外的线程块启动和调度开销会反映到Task Duration中，因此下一步优先优化Thread Block到tile的映射。

---

### Case 2：MTE搬入UB后转置写回，固定Thread Block数

**优化目标**：将Case 1的线程块数量从切分后的tile分组数改为硬件vector core数，每个Thread Block通过循环处理多个tile分组，减少Thread Block启动和调度开销。

**核心实现**：每个Thread Block启动2048个线程，配置参数为 `dim3(32,64,1)`。循环每轮会同时处理两个连续的32×32的tile：`threadIdx.y=0..31`的线程处理第一个tile，`threadIdx.y=32..63`的线程处理第二个tile；下一轮按 `gridDim.x * 2`跳到后续tile分组。

```cpp
uint32_t loop_start = block_idx * TILES_PER_BLOCK;
uint32_t loop_step = block_num * TILES_PER_BLOCK;

for (uint32_t tile_base = loop_start; tile_base < total_tiles; tile_base += loop_step) {
    copy_gm_tile_to_ub(&tile[local_tile][0][0], input + tile_offset, width);
    sync_mte2_to_v();
    asc_vf_call<simt_store_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                  output, &tile[0][0][0], width, height, tiles_x,
                                  tile_base, total_tiles);
    sync_v_to_mte2();
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      18.696       |    17.512    |     28894.9      |      13.509      |     0.772     |        0.701        |      0.040       |

**分析**：

Case 2将Thread Block数固定为硬件vector core数。相比Case 1按tile分组启动512个Thread Block的写法，Case 2需要启动和调度的线程块数量更少，Task Duration从23.215μs降低到18.696μs，耗时下降约19.5%。后续版本继续沿用2048线程、每轮两个tile的路径，把优化重点转向UB访问开销。

---

### Case 3：MTE搬入UB后转置写回+padding

**优化目标**：在Case 2的基础上，通过UB padding降低转置读阶段的bank冲突。

**核心实现**：

- 线程块布局、tile映射和Case 2保持一致。
- 每个Thread Block仍然每轮处理两个32×32 tile。
- UB中的tile物理布局从32×32改为32×40。
- SIMT VF仍只读取有效的32列。

SIMT场景下，处理转置读UB的bank冲突通常会在tile的列方向增加padding，使同一列元素在UB物理地址上错开分布。对于32×32的float tile，增加一个subbank宽度即2列padding即可形成32×34的UB布局，从而避免subbank冲突。

SIMD与SIMT混合编程场景下，UB数据由Vector侧MTE搬入。MTE搬运带padding的二维数组时，还需要考虑UB中相邻行的地址跨度对齐。32×34布局的行跨度为 `34 * sizeof(float) = 136B`，不满足32B对齐；因此本样例将padding列数设为8，使用32×40布局，行跨度为 `40 * sizeof(float) = 160B`，既能错开UB bank访问，也满足MTE搬运对UB行跨度的对齐要求。

下面以Ascend 950PR/Ascend 950DT的UB划分规则为例，说明本样例中bank冲突是如何产生的，以及32×32、32×34和32×40三种UB布局理论上的冲突强度差异。

UB总大小256KB，划分为16个bank；这16个bank进一步组织为8个bank group，每个bank group包含2个bank。在SIMT编程模式下，每个bank又会被划分为4个subbank，每个subbank的宽度为8B。本样例所说的bank冲突指的是更细粒度的subbank冲突：同一个Warp内的多个线程可能在同一条UB访问指令中同时访问UB；当这些访问数据属于同一个bank group的相同编号subbank资源时，硬件无法在一个周期内处理全部请求，需要排队等待，从而形成subbank冲突并增加访问延迟；如果这些访问数据属于同一个subbank的同一行内8B地址范围，硬件会将这些请求合并处理，不会形成subbank冲突。

**图3　UB bank结构示意图**
<img src="./figures/bank结构示意图.png">

**地址采用低位交织：** 如下图所示，UB内连续地址按32B粒度映射到bank0～bank15：第1个32B地址段映射到bank0，第2个映射到bank1，依次类推；第16个映射到bank15，第17个映射回bank0的下一行。

**图4　UB地址低位交织排布示意图**
<img src="./figures/bank内存排布示意图.png">

SIMT编程方式下subbank冲突主要有以下几种：

- **写写冲突**：多个写操作同时尝试访问同一个bank group的相同编号subbank。
- **读读冲突**：多个读操作同时尝试访问同一个bank group的相同编号subbank。

Case 2中UB的tile数组按照行优先存储。按照地址低位交织规则，tile数组的第一行刚好覆盖bank0～bank3，第二行覆盖bank4～bank7，第三行覆盖bank8～bank11，其余行依次类推。每行32个float数据会恰好跨越4个bank存储。如图5所示，图中展示了tile数组前12行元素在UB上的排布，每行第一个元素用蓝色进行标记。写回GM时，一个Warp的线程会读取tile的一列元素；访问UB时，32个线程会集中访问两个bank group的subbank0，属于读读冲突场景。

**图5　Case2 tile在UB上的排布示意图**
<img src="./figures/case2_bank.png">

如图6所示，Case 3中对UB中的tile数组增加8列padding，由每行32个元素改为40个元素。每行40个float的行跨度为160B，对应20个subbank；转置读同一列时，相邻行元素在UB物理地址上按20个subbank的跨度错开，32个线程的访问不再集中到相同bank group的同一编号subbank，从而降低转置读阶段的subbank冲突。

需要注意的是，32×40布局是SIMD与SIMT混合编程场景下结合MTE搬运对齐要求后的折中选择。与纯SIMT场景下增加2列padding形成32×34布局不同，32×40布局仍可能存在少量subbank冲突，但冲突强度已经明显低于未padding的32×32布局。

**图6　Case3 tile在UB上的排布示意图**
<img src="./figures/case3_bank.png">

```cpp
constexpr int TILE_PAD = 8;                          // 混合MTE搬运要求32B对齐，使用32×40的UB布局
constexpr int TILE_PAD_STRIDE = TILE_DIM + TILE_PAD;
__ubuf__ float tile[TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];

copy_gm_tile_to_ub(dst + local_tile * TILE_PAD_ELEMENTS, input + tile_offset, width,
                   TILE_PAD_STRIDE * sizeof(float));

output[x_index + y_index * height] =
    tile[local_tile * TILE_PAD_ELEMENTS + tx * TILE_PAD_STRIDE + ty];
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      11.856       |    10.687    |     17633.7      |      6.785       |     0.635     |        0.702        |      0.065       |

**分析**：

Case 3只在Case 2基础上改变UB物理布局，不改变GM读写坐标和Thread Block到tile的映射方式。相比Case 2，Task Duration从18.696μs降低到11.856μs，耗时下降约36.6%。该结果说明GM访存连续化和固定Thread Block数之后，转置读UB阶段的bank冲突已经成为主要瓶颈之一。

---

### Case 4：MTE搬入UB后转置写回+padding+双缓冲

**优化目标**：在Case 3的基础上，通过两份padded UB缓冲区和双event id，使MTE2搬运流水和SIMT VF流水并行。

**核心实现**：

- 使用两份缓冲区，每份缓冲区包含两个32×40 padded tile。
- 两份缓冲区分别绑定 `EVENT_ID0`和 `EVENT_ID1`。
- 先搬入第一组tile填充流水。
- 循环中先为下一组tile发起MTE2搬入，再对当前缓冲区调用SIMT VF处理。
- 当前缓冲区的VF处理完成后通知MTE2该缓冲区可复用。

```cpp
__ubuf__ float tile[2][TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];

uint32_t curr_buffer = 0;
uint32_t curr_event = EVENT_ID0;

copy_gm_2tile_to_padded_ub(&tile[curr_buffer][0][0][0], input, width, tiles_x,
                           curr_tile_base, total_tiles);
asc_sync_notify(PIPE_MTE2, PIPE_V, curr_event);

for (uint32_t next_tile_base = curr_tile_base + loop_step; curr_tile_base < total_tiles;
     next_tile_base += loop_step) {
    asc_sync_wait(PIPE_MTE2, PIPE_V, curr_event);

    uint32_t next_buffer = curr_buffer ^ 1;
    uint32_t next_event = (next_buffer == 0) ? EVENT_ID0 : EVENT_ID1;
    if (next_tile_base < total_tiles) {
        asc_sync_wait(PIPE_V, PIPE_MTE2, next_event);
        copy_gm_2tile_to_padded_ub(&tile[next_buffer][0][0][0], input, width, tiles_x,
                                   next_tile_base, total_tiles);
        asc_sync_notify(PIPE_MTE2, PIPE_V, next_event);
    }

    asc_vf_call<simt_store_2tile_pad>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                      output, &tile[curr_buffer][0][0][0], width, height,
                                      tiles_x, curr_tile_base, total_tiles);
    asc_sync_notify(PIPE_V, PIPE_MTE2, curr_event);

    curr_buffer = next_buffer;
    curr_event = next_event;
    curr_tile_base = next_tile_base;
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|       8.950       |    7.774     |     12826.7      |      6.728       |     0.866     |        0.794        |      0.102       |

**分析**：

Case 4通过双缓冲提供物理隔离，避免MTE2写入覆盖SIMT VF正在读取的UB区域；通过双event id分别管理两份缓冲区的生命周期，将单缓冲中的“MTE2搬完再VF、VF完再下一轮MTE2”的串行屏障，调整为相邻轮次之间的流水重叠。相比Case 3，Task Duration从11.856μs降低到8.950μs，耗时下降约24.5%。相比Case 0直接GM转置，Task Duration从36.140μs降低到8.950μs，整体性能提升约4.04倍。

---

## 性能对比总结

### Ascend 950PR性能数据

下面给出各个Case处理1024×1024 float矩阵时，在Ascend 950PR上的性能数据。

| Case | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| ---- | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
| 0    |      36.140       |    33.793    |     55757.9      |      33.338      |     0.987     |        0.443        |      0.013       |
| 1    |      23.215       |    2.281     |      3764.3      |      1.672       |     0.735     |        0.184        |      0.078       |
| 2    |      18.696       |    17.512    |     28894.9      |      13.509      |     0.772     |        0.701        |      0.040       |
| 3    |      11.856       |    10.687    |     17633.7      |      6.785       |     0.635     |        0.702        |      0.065       |
| 4    |       8.950       |    7.774     |     12826.7      |      6.728       |     0.866     |        0.794        |      0.102       |

转置主路径Case 0 -> Case 1 -> Case 2 -> Case 3 -> Case 4，Task Duration由36.140μs依次降至23.215μs、18.696μs、11.856μs、8.950μs。Case 4相对Case 0直接GM转置提升约4.04倍。

矩阵转置需要从GM读取完整输入矩阵，并向GM写回完整输出矩阵。对于1024×1024的float矩阵，理想情况下GM读写总数据量为：

$$
D = 1024 \times 1024 \times 4B \times 2 = 8.39MB
$$

若按Ascend 950PR的GM峰值带宽1.6TB/s估算，完全连续读写且不考虑调度、同步、UB访问和地址计算等额外开销时，理论耗时下限为：

$$
T_{theory} = \frac{8.39MB}{1.6TB/s} \approx 5.243μs
$$

Case 0距离理论下限较远，主要原因是转置写回导致同一Warp内相邻线程写地址跨行分散，GM写无法形成理想连续访问。Case 1通过UB中转把非连续访问转移到UB内部，GM侧访问连续性改善，但按tile分组启动512个Thread Block，调度开销较高。Case 2固定Thread Block数后减少了启动和调度开销，但转置读UB仍存在bank冲突。Case 3通过32×40的UB padding降低bank冲突后，耗时明显下降。Case 4进一步使用双缓冲，使MTE2搬运流水和SIMT VF流水并行，Task Duration接近理论下限，但仍高于5.243μs，主要来自UB读写、MTE与V之间的同步、`asc_vf_call`调用开销、地址计算以及流水无法完全并行等非GM带宽开销。

## 调优建议

访存类算子调优时，应先用最直接的实现暴露主要差距，再结合理论带宽上限判断优化空间。本样例中，直接转置与理论下限的差距主要来自非连续GM写，因此优化方向不是增加计算并行度，而是重排访存：以32×32的tile为单位连续搬入UB，在UB内完成转置方向读取，再按输出矩阵行方向连续写回GM。

分块后需要先确定Thread Block到tile的映射方式。按切分后的tile分组启动Thread Block实现简单，但当tile数远大于硬件vector core数时，会带来额外的启动和调度开销；将Thread Block数固定为硬件vector core数，并在kernel内循环处理多个tile组，可以降低这部分开销。线程数和单个Thread Block处理的tile数量需要结合寄存器、栈和实测耗时判断。

当GM访问已经转为连续后，瓶颈会转移到UB访问和流水同步。转置读UB时，如果按列访问32×32的tile，容易出现bank冲突，可以通过padding改变UB物理布局；在SIMD与SIMT混合编程中还需要满足MTE搬运的对齐要求，因此本样例采用32×40的padded tile。在此基础上，双缓冲通过两份UB缓冲区轮换，使当前缓冲区进入SIMT VF流水处理的同时，另一份缓冲区可由MTE2流水搬入下一批数据，从而实现MTE2搬运流水和SIMT VF流水并行，隐藏部分搬运开销。

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
  SCENARIO_NUM=4                                                                     # 选择执行场景，可选0-4
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./matrix_transpose                                                            # 执行样例
  ```
- 编译选项说明

  | 选项                       | 可选值     | 说明                                                       |
  | -------------------------- | ---------- | ---------------------------------------------------------- |
  | `CMAKE_ASC_ARCHITECTURES`  | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`             | `0`-`4`    | 样例类型，默认为4                                          |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能分析

使用 `msopprof`工具获取详细性能数据：

```bash
msopprof ./matrix_transpose
```

命令执行完成后，会在默认目录生成以 `OPPROF_{timestamp}_XXX`命名的文件夹，性能数据文件夹结构示例如下：

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
