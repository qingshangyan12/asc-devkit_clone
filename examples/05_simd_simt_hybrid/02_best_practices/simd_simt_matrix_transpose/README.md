# SIMD与SIMT混合编程Matrix Transpose性能调优样例

## 概述

本样例基于SIMT场景下的[matrix_transpose_practice样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/03_simt_api/03_best_practices/00_memory_optimizations/matrix_transpose_practice)，采用SIMD与SIMT混合编程对自定义transpose算子进行进一步优化，通过混合编程中的MTE搬运与SIMT计算并行，提高矩阵转置性能。为了展示逐步优化过程，本样例从直接全局内存转置出发，引入MTE搬运、UB中转与32×32分块，对比按tile分组启动Thread Block与固定Thread Block数两种映射方式，通过UB padding降低bank冲突，最后借助双缓冲使MTE2搬入、SIMT VF转置和MTE3搬出流水并行，呈现SIMD与SIMT混合矩阵转置的调优路径。

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
│   ├── README.md                   // 样例说明文档
│   └── README_en.md                // 英文样例说明文档
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
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">transpose_naive_kernel / transpose_ub_2tile_static_kernel / transpose_ub_2tile_loop_kernel / transpose_ub_pad_loop_kernel / transpose_ub_pad_db_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例通过5个Case构成一条优化路径，各Case的核函数、线程块数量、每个线程块的线程数和主要变化如下表所示。

| Case | 核函数                               | 线程块数量          | 每个线程块的线程数 | 本步引入的变化                                                            |
| ---- | ------------------------------------ | ------------------- | ------------------ | ------------------------------------------------------------------------- |
| 0    | `transpose_naive_kernel`           | `core`            | 2048               | 直接在GM中完成转置                                                        |
| 1    | `transpose_ub_2tile_static_kernel` | `ceil(tiles / 2)` | 2048               | 引入MTE搬运、UB中转和32×32分块，按tile分组数量设置Thread Block数         |
| 2    | `transpose_ub_2tile_loop_kernel`   | `core`            | 2048               | Thread Block数固定为硬件vector core数，单个Thread Block循环处理多个tile组 |
| 3    | `transpose_ub_pad_loop_kernel`     | `core`            | 2048               | UB tile增加8列padding，降低SIMT VF转置访问阶段的bank冲突                  |
| 4    | `transpose_ub_pad_db_kernel`       | `core`            | 2048               | 双缓冲轮换，使MTE2搬入、SIMT VF转置和MTE3搬出流水并行                     |

其中 `core`为硬件vector core数，通过 `aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`在运行时查询获得。本样例中的性能数据基于 `core=64` 的测试环境采集得到。`tiles`为总tile数 `(W/32) × (H/32)`；处理1024×1024矩阵时，矩阵会被划分为32×32个tile，因此 `tiles=1024`。

#### 性能指标说明

| 指标                 | 说明                                                                                      |
| -------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(μs)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |
| aiv_time(μs)        | Task在AI Vector Core上的理论执行时间，单位为μs                                           |
| aiv_total_cycles     | 该Task被分配到每个AI Vector Core计算单元上后，每个AI Vector Core计算单元上的执行cycle总数 |
| aiv_vec_time(μs)    | vec类型指令（向量类运算指令）耗时，单位为μs                                              |
| aiv_vec_ratio        | vec类型指令（向量类运算指令）的cycle数在total cycle数中的占用比                           |
| aiv_scalar_time(μs) | scalar类型指令（标量类运算指令）耗时，单位为μs                                           |
| aiv_scalar_ratio     | scalar类型指令（标量类运算指令）的cycle数在total cycle数中的占用比                        |
| aiv_mte2_time(μs)   | MTE2搬入指令耗时，单位为μs                                                               |
| aiv_mte3_time(μs)   | MTE3搬出指令耗时，单位为μs                                                               |

除Task Duration外，其余指标均为所有Thread Block上的平均值。

### Case 0：直接全局内存转置

**优化目标**：在GM上直接进行坐标变换。

**核心实现**：SIMT VF内读取 `input(row, col)`，直接写入转置位置 `output(col, row)`。全局内存读取连续，写入地址按列跨行，为非连续写。

```cpp
uint32_t row = i / width;
uint32_t col = i - row * width;
// 直接写转置后的GM地址，写入方向跨行不连续。
output[col * height + row] = input[i];
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
|       36.215       |    33.627    |     55484.7     |      33.181      |     0.987     |        0.434        |      0.013      |

**分析**：

Case 0在保持GM连续读取的同时，直接把数据写入转置后的输出位置，导致同一Warp内相邻线程的写地址分布到输出矩阵的不同行。由于矩阵转置本身计算量很小，该版本的主要瓶颈来自GM非连续写。后续优化需要把非连续访问从GM转移到UB，使GM读写转变为连续访问模式。

---

### Case 1：MTE搬入UB后转置写回，每个Thread Block处理两个32×32的tile

**优化目标**：引入MTE搬运、UB中转和32×32分块，将Case 0的非连续全局内存写转换为UB内访问，使全局内存读写尽量连续。本Case按切分后的tile分组数量设置Thread Block数，每个Thread Block处理两个32×32的tile。

**核心实现**：将矩阵划分为32×32的tile，总tile数为 `tiles=(W/32) × (H/32)`。本Case中每个Thread Block启动2048个线程，可同时处理两个tile，因此Thread Block数量设置为 `ceil(tiles / 2)`。本Case的数据路径为：MTE2先将GM数据搬入UB输入缓冲区，SIMT VF在UB上完成转置并写入UB输出缓冲区，最后由MTE3将UB输出缓冲区连续搬回GM。

```cpp
__ubuf__ float in_tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];
__ubuf__ float out_tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];
uint32_t loop_start = block_idx * TILES_PER_BLOCK;
uint32_t loop_step = block_num * TILES_PER_BLOCK;
uint32_t loop_end = loop_start + TILES_PER_BLOCK;

// Case 1按tile分组启动Thread Block，每个Thread Block只处理当前分配到的两个tile。
for (uint32_t tile_base = loop_start; tile_base < loop_end && tile_base < total_tiles; tile_base += loop_step) {
    copy_gm_2tile_to_ub(&in_tile[0][0][0], input, width, tiles_x, tile_base, total_tiles);
    // MTE2搬入完成后，SIMT VF读取UB输入buffer。
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<simt_transpose_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                      &out_tile[0][0][0], &in_tile[0][0][0],
                                      tile_base, total_tiles);
    // SIMT VF写完输出buffer后，MTE3将结果搬回GM。
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    copy_ub_2tile_to_gm(output, &out_tile[0][0][0], height, tiles_x, tile_base, total_tiles);
    // MTE3搬出完成后，本轮UB buffer可进入下一轮MTE2搬入。
    asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
|       23.374       |     2.292     |      3781.8      |       1.445       |     0.633     |        0.207        |      0.088      |

**分析**：

Case 1通过UB中转把非连续GM写转换为UB内转置访问，Task Duration从Case 0的36.215μs降至23.374μs，耗时下降约35.5%。该版本按tile分组数量启动Thread Block，1024×1024矩阵共有1024个tile，对应512个Thread Block，明显超过硬件vector core数。额外的线程块启动和调度开销会反映到Task Duration中，因此下一步优先优化Thread Block到tile的映射。

---

### Case 2：MTE搬入UB后转置写回，固定Thread Block数

**优化目标**：将Case 1的线程块数量从切分后的tile分组数改为硬件vector core数，每个Thread Block通过循环处理多个tile分组，减少Thread Block启动和调度开销。

**核心实现**：每个Thread Block启动2048个线程，配置参数为 `dim3(32,64,1)`。循环每轮会同时处理两个连续的32×32的tile：`threadIdx.y=0..31`的线程处理第一个tile，`threadIdx.y=32..63`的线程处理第二个tile；下一轮按 `gridDim.x * 2`跳到后续tile分组。

```cpp
uint32_t loop_start = block_idx * TILES_PER_BLOCK;
uint32_t loop_step = block_num * TILES_PER_BLOCK;

// Case 2固定Thread Block数，每个Thread Block按gridDim.x步长循环处理多个tile组。
for (uint32_t tile_base = loop_start; tile_base < total_tiles; tile_base += loop_step) {
    copy_gm_2tile_to_ub(&in_tile[0][0][0], input, width, tiles_x, tile_base, total_tiles);
    // MTE2搬入完成后，SIMT VF读取UB输入buffer。
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<simt_transpose_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                      &out_tile[0][0][0], &in_tile[0][0][0],
                                      tile_base, total_tiles);
    // SIMT VF写完输出buffer后，MTE3将结果搬回GM。
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    copy_ub_2tile_to_gm(output, &out_tile[0][0][0], height, tiles_x, tile_base, total_tiles);
    // MTE3搬出完成后，本轮UB buffer可进入下一轮MTE2搬入。
    asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
|       18.734       |    17.553    |     28961.7     |      11.602      |     0.661     |        0.880        |      0.050      |

**分析**：

Case 2将Thread Block数固定为硬件vector core数。相比Case 1按tile分组启动512个Thread Block的写法，Case 2需要启动和调度的线程块数量更少，Task Duration从23.374μs降低到18.734μs，耗时下降约19.9%。在SIMD与SIMT混合编程场景下，降低Thread Block数不会改变SIMT VF内部的转置逻辑，也不会引入额外的SIMT线程侧计算复杂度；与SIMT场景不同，本Case未观察到寄存器溢出。后续版本继续沿用2048线程、每轮两个tile的路径，把优化重点转向UB访问开销。

---

### Case 3：MTE搬入UB后转置写回+padding

**优化目标**：在Case 2的基础上，通过UB padding降低SIMT VF转置访问阶段的bank冲突。

**核心实现**：

- 线程块布局、tile映射和Case 2保持一致。
- 每个Thread Block仍然每轮处理两个32×32 tile。
- UB中的 `in_tile`物理布局从32×32改为32×40。
- `out_tile`仍采用32×32连续布局，SIMT VF按输出tile行方向连续写入，MTE3再连续搬出到GM。
- SIMT VF仍只处理有效的32×32数据。

以Ascend 950PR/Ascend 950DT为例，UB划分为16个bank，并组织为8个bank group，每个bank又划分为4个subbank。SIMT VF内若同一个Warp内多个线程在同一条UB访问指令中访问同一个bank group的相同编号subbank，硬件需要排队处理，从而形成subbank冲突并增加访问延迟。

**图1　UB bank结构示意图**

<img src="./figures/bank结构示意图.png">

SIMT VF内访问UB时的bank冲突为更细粒度的subbank冲突，主要有以下两类subbank冲突：

- **写写冲突**：多个写操作同时访问同一个bank group的相同编号subbank。
- **读读冲突**：多个读操作同时访问同一个bank group的相同编号subbank。

详细的地址低位交织规则以及冲突场景可参考[bank_conflict样例](https://gitcode.com/cann/asc-devkit/blob/master/examples/03_simt_api/02_features/01_api_features/00_memory_access/bank_conflict)。

Case 2中UB的 `in_tile`数组按照行优先存储。按照地址低位交织规则，`in_tile`数组的第一行刚好覆盖bank0～bank3，第二行覆盖bank4～bank7，第三行覆盖bank8～bank11，其余行依次类推。每行32个float数据会恰好跨越4个bank存储。如图3所示，图中展示了tile数组前12行元素在UB上的排布，每行第一个元素用蓝色进行标记。SIMT VF转置时，一个Warp的线程会读取 `in_tile`的一列元素；访问UB时，32个线程会集中访问两个bank group的subbank0，属于读读冲突场景。`out_tile`按输出tile行方向连续写入，可通过访存合并降低写入开销，因此不需要增加padding。

**图3　Case2 tile在UB上的排布示意图**

<img src="./figures/case2_bank.png">

SIMT场景下，处理转置访问UB的bank冲突通常会在tile的列方向增加padding，使同一列元素在UB物理地址上错开分布。对于32×32的float tile，增加一个subbank宽度即2列padding即可形成32×34的UB布局，从而避免subbank冲突。

SIMD与SIMT混合编程场景下，UB数据由MTE搬入。MTE搬运带padding的二维数组时，还需要考虑UB中相邻行的地址跨度对齐。32×34布局的行跨度为 `34 * sizeof(float) = 136B`，不满足32B对齐；因此本样例将padding列数设为8，使用32×40布局，行跨度为 `40 * sizeof(float) = 160B`，既能错开UB bank访问，也满足MTE搬运对UB行跨度的对齐要求。

如图4所示，Case 3中对UB中的tile数组增加8列padding，由每行32个元素改为40个元素。每行40个float的行跨度为160B，对应20个subbank；转置访问同一列时，相邻行元素在UB物理地址上按20个subbank的跨度错开，32个线程的访问不再集中到相同bank group的同一编号subbank，从而降低SIMT VF转置访问阶段的subbank冲突。

需要注意的是，32×40布局是SIMD与SIMT混合编程场景下结合MTE搬运对齐要求后的折中选择。与SIMT场景下增加2列padding形成32×34布局不同，32×40布局仍可能存在少量subbank冲突，但冲突强度已经明显低于未padding的32×32布局。

**图4　Case3 tile在UB上的排布示意图**

<img src="./figures/case3_bank.png">

```cpp
constexpr int TILE_PAD = 8;                          // MTE搬运要求32B对齐，使用32×40的UB布局
constexpr int TILE_PAD_STRIDE = TILE_DIM + TILE_PAD;
__ubuf__ float in_tile[TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];
__ubuf__ float out_tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];
uint32_t loop_start = block_idx * TILES_PER_BLOCK;
uint32_t loop_step = block_num * TILES_PER_BLOCK;

// Case 3沿用固定Thread Block数和循环处理方式，仅将输入tile换成32×40 padded布局。
for (uint32_t tile_base = loop_start; tile_base < total_tiles; tile_base += loop_step) {
    copy_gm_2tile_to_padded_ub(&in_tile[0][0][0], input, width, tiles_x, tile_base, total_tiles);
    // MTE2搬入完成后，SIMT VF按32×40 padded布局读取UB输入buffer。
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_vf_call<simt_transpose_2tile_pad>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                          &out_tile[0][0][0], &in_tile[0][0][0],
                                          tile_base, total_tiles);
    // simt_transpose_2tile_pad内部按32×40物理布局读取输入tile，按32×32连续布局写入输出tile。
    ...
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
    copy_ub_2tile_to_gm(output, &out_tile[0][0][0], height, tiles_x, tile_base, total_tiles);
    // 单buffer复用前，等待MTE3搬出完成。
    asc_sync_notify(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_MTE2, EVENT_ID0);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
|       11.617       |    10.420    |     17192.8     |       4.465       |     0.429     |        0.877        |      0.084      |

**分析**：

Case 3只在Case 2基础上改变UB物理布局，不改变GM读写坐标和Thread Block到tile的映射方式。相比Case 2，Task Duration从18.734μs降低到11.617μs，耗时下降约38.0%。该结果说明GM访存连续化和固定Thread Block数之后，SIMT VF转置访问UB阶段的bank冲突已经成为主要瓶颈之一。

---

### Case 4：MTE搬入UB后转置写回+padding+双缓冲

**优化目标**：在Case 3的基础上，通过两份UB输入/输出缓冲区和双event id，使MTE2搬入、SIMT VF转置和MTE3搬出流水并行。

**核心实现**：

- 使用两份输入缓冲区和两份输出缓冲区；输入缓冲区使用32×40 padded tile，输出缓冲区使用32×32连续tile。
- 两份缓冲区分别使用 `EVENT_ID0`和 `EVENT_ID1`管理生命周期。
- 先搬入第一组tile填充流水。
- 循环中先为下一组tile发起MTE2搬入，再对当前输入/输出缓冲区调用SIMT VF处理。
- SIMT VF完成后释放当前输入缓冲区，并通知MTE3搬出当前输出缓冲区。

```cpp
__ubuf__ float in_tile[2][TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];
__ubuf__ float out_tile[2][TILES_PER_BLOCK][TILE_DIM][TILE_DIM];
uint32_t curr_buffer = 0;
uint32_t curr_event = EVENT_ID0;
uint32_t loop_count = 0;

// 初始化两个event，允许MTE2写入两组输入buffer。
asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID0);
asc_sync_notify(PIPE_V, PIPE_MTE2, EVENT_ID1);

// 先搬入第一组tile填充流水线。
asc_sync_wait(PIPE_V, PIPE_MTE2, curr_event);
copy_gm_2tile_to_padded_ub(&in_tile[curr_buffer][0][0][0], input, width, tiles_x,
                           curr_tile_base, total_tiles);
asc_sync_notify(PIPE_MTE2, PIPE_V, curr_event);

for (uint32_t next_tile_base = curr_tile_base + loop_step; curr_tile_base < total_tiles;
     next_tile_base += loop_step) {
    // curr_buffer用于SIMT VF转置和MTE3搬出，next_buffer用于下一轮MTE2搬入。
    uint32_t next_buffer = curr_buffer ^ 1;
    uint32_t next_event = (next_buffer == 0) ? EVENT_ID0 : EVENT_ID1;

    if (next_tile_base < total_tiles) {
        asc_sync_wait(PIPE_V, PIPE_MTE2, next_event);
        copy_gm_2tile_to_padded_ub(&in_tile[next_buffer][0][0][0], input, width, tiles_x,
                                   next_tile_base, total_tiles);
        asc_sync_notify(PIPE_MTE2, PIPE_V, next_event);
    }

    asc_sync_wait(PIPE_MTE2, PIPE_V, curr_event);
    // 第三轮开始复用同一个输出buffer，需要先等待该buffer上一次MTE3搬出完成。
    if (loop_count >= 2) {
        asc_sync_wait(PIPE_MTE3, PIPE_V, curr_event);
    }
    // 当前输入buffer准备好后，在当前输出buffer中生成转置后的连续tile。
    asc_vf_call<simt_transpose_2tile_pad>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                          &out_tile[curr_buffer][0][0][0],
                                          &in_tile[curr_buffer][0][0][0],
                                          curr_tile_base, total_tiles);
    asc_sync_notify(PIPE_V, PIPE_MTE2, curr_event);
    asc_sync_notify(PIPE_V, PIPE_MTE3, curr_event);
    asc_sync_wait(PIPE_V, PIPE_MTE3, curr_event);
    copy_ub_2tile_to_gm(output, &out_tile[curr_buffer][0][0][0], height, tiles_x,
                        curr_tile_base, total_tiles);
    asc_sync_notify(PIPE_MTE3, PIPE_V, curr_event);
    ++loop_count;

    curr_buffer = next_buffer;
    curr_event = next_event;
    curr_tile_base = next_tile_base;
}

// 只等待已经发起过的MTE3，避免等待没有notify过的event。
if (loop_count > 0) {
    asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID0);
}
if (loop_count > 1) {
    asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID1);
}
```

**性能数据**：

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
|       6.824       |     5.621     |      9275.0      |       4.319       |     0.769     |        0.965        |      0.171      |

**分析**：

Case 4通过双缓冲避免MTE2写入覆盖SIMT VF正在读取的输入缓冲区，也避免SIMT VF写入覆盖MTE3正在搬出的输出缓冲区；通过双event id分别管理两组输入/输出缓冲区的生命周期，将单缓冲中的“MTE2搬入、SIMT VF转置、MTE3搬出”串行屏障，调整为相邻轮次之间的流水重叠。相比Case 3，Task Duration从11.617μs降低到6.824μs，耗时下降约41.3%。相比Case 0直接GM转置，Task Duration从36.215μs降低到6.824μs，整体性能提升约5.31倍。

Case 3与Case 4的仿真指令流水图分别如图5、图6所示。从图5可以看到，Case 3使用单buffer串行处理每组tile。MTE2搬入完成后需要通过 `WAIT_FLAG` 等待SIMT VF读取，SIMT VF完成后再等待MTE3搬出，MTE3搬出结束后才能复用同一组UB buffer进入下一轮MTE2搬入。因此MTE2、SIMT VF和MTE3三个流水之间存在明显的串行等待，SIMT VF执行间隔中会出现较多由搬运和同步带来的空隙。

从图6可以看到，Case 4采用双缓冲后，当前buffer进行SIMT VF转置和MTE3搬出的同时，另一组buffer可以提前发起下一组tile的MTE2搬入。MTE2、SIMT VF和MTE3在相邻轮次之间形成重叠。因此Case 4能够隐藏部分MTE搬运和同步开销，Task Duration进一步降低。图中仍然存在少量等待，主要来自首个tile组预搬入时没有前一轮SIMT VF/MTE3可重叠、最后一组tile的MTE3搬出需要等待完成，以及同一输出buffer再次被SIMT VF写入前必须等待前序MTE3搬出完成。

**图5　Case 3仿真指令流水图**

<img src="./figures/case3_trace.png">

**图6　Case 4仿真指令流水图**

<img src="./figures/case4_trace.png">

## 性能对比总结

### Ascend 950PR性能数据

下面给出各个Case处理1024×1024 float矩阵时，在Ascend 950PR上的性能数据。

| Case | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| ---- | :----------------: | :-----------: | :--------------: | :---------------: | :-----------: | :------------------: | :--------------: |
| 0    |       36.215       |    33.627    |     55484.7     |      33.181      |     0.987     |        0.434        |      0.013      |
| 1    |       23.374       |     2.292     |      3781.8      |       1.445       |     0.633     |        0.207        |      0.088      |
| 2    |       18.734       |    17.553    |     28961.7     |      11.602      |     0.661     |        0.880        |      0.050      |
| 3    |       11.617       |    10.420    |     17192.8     |       4.465       |     0.429     |        0.877        |      0.084      |
| 4    |       6.824       |     5.621     |      9275.0      |       4.319       |     0.769     |        0.965        |      0.171      |

转置主路径Case 0 -> Case 1 -> Case 2 -> Case 3 -> Case 4，Task Duration由36.215μs依次降至23.374μs、18.734μs、11.617μs、6.824μs。Case 4相对Case 0直接GM转置提升约5.31倍。

矩阵转置需要从GM读取完整输入矩阵，并向GM写回完整输出矩阵。对于1024×1024的float矩阵，理想情况下GM读写总数据量为：

$$
D = 1024 \times 1024 \times 4B \times 2 = 8.39MB
$$

若按Ascend 950PR的GM峰值带宽1.6TB/s估算，完全连续读写且不考虑调度、同步、UB访问和地址计算等额外开销时，理论耗时下限为：

$$
T_{theory} = \frac{8.39MB}{1.6TB/s} \approx 5.243μs
$$

Case 0距离理论下限较远，主要原因是转置写回导致同一Warp内相邻线程写地址跨行分散，GM写无法形成理想连续访问。Case 1通过UB中转把非连续访问转移到UB内部，GM侧访问连续性改善，但按tile分组启动512个Thread Block，调度开销较高。Case 2固定Thread Block数后减少了启动和调度开销，但SIMT VF转置访问UB仍存在bank冲突。Case 3通过32×40的UB padding降低bank冲突后，耗时明显下降。Case 4进一步使用双缓冲，使MTE2搬入、SIMT VF转置和MTE3搬出流水并行，Task Duration降低到6.824μs，对应等效GM读写带宽约1.23TB/s，约达到Ascend 950PR理论GM峰值带宽1.6TB/s的76.8%。该结果已经接近理论带宽上限，但仍高于5.243μs的理论耗时下限，主要来自UB读写、MTE与V之间的同步、`asc_vf_call`调用开销、地址计算以及流水无法完全并行等非GM带宽开销。这些开销属于当前实现路径下暂无明显进一步优化手段的部分。

## 调优建议

访存类算子调优时，应先用最直接的实现暴露主要差距，再结合理论带宽上限判断优化空间。本样例中，直接转置与理论下限的差距主要来自非连续GM写，因此优化方向不是增加计算并行度，而是重排访存：在SIMD与SIMT混合场景下，以32×32的tile为单位使用MTE连续搬入UB输入缓冲区，在UB内完成转置方向访问并写入UB输出缓冲区，再使用MTE3按输出矩阵行方向连续搬出到GM。

分块后需要先确定Thread Block到tile的映射方式。按切分后的tile分组启动Thread Block实现简单，但当tile数远大于硬件vector core数时，会带来额外的启动和调度开销；将Thread Block数固定为硬件vector core数，并在kernel内循环处理多个tile组，可以降低这部分开销。线程数和单个Thread Block处理的tile数量需要结合寄存器、栈和实测耗时判断。

当GM访问已经转为连续后，瓶颈会转移到UB访问和流水同步。转置访问UB时，如果按列访问32×32的输入tile，容易出现bank冲突，可以通过padding改变UB物理布局；在SIMD与SIMT混合编程中还需要满足MTE搬运的对齐要求，因此本样例的输入缓冲区采用32×40的padded tile。输出缓冲区按输出tile行方向连续写入，采用32×32连续布局即可。在此基础上，双缓冲通过两组UB输入/输出缓冲区轮换，使当前缓冲区进入SIMT VF转置和MTE3搬出流水处理的同时，另一组缓冲区可由MTE2流水搬入下一批数据，从而实现MTE2搬入、SIMT VF转置和MTE3搬出流水并行，隐藏部分搬运开销。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请参考[环境变量配置](../../../../docs/zh/quick_start.md#cann-env-setup)，配置当前环境上的CANN开发套件包环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为CANN包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。
  >
- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=4                                                                     # 选择执行场景，可选0-4
  mkdir -p build && cd build;                                                        # 创建并进入build目录
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./matrix_transpose                                                            # 执行样例
  ```
- 编译选项说明

  | 选项                        | 可选值       | 说明                                                       |
  | --------------------------- | ------------ | ---------------------------------------------------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU架构：本样例仅支持dav-3510（Ascend 950PR/Ascend 950DT） |
  | `SCENARIO_NUM`            | `0`-`4`  | 样例类型，默认为4                                          |

  执行结果如下，说明精度对比成功。


  ```text
  [Success] Case accuracy verification passed.
  ```

## 性能分析

### msOpProf工具介绍

msOpProf工具是单算子性能分析工具。包含msopprof和msopprof simulator两种使用方式。该工具协助用户定位算子内存、算子代码以及算子指令的异常，实现全方位的算子调优。当前支持基于不同运行模式（上板或仿真）和不同文件形式（可执行文件或算子二进制.o文件）进行性能数据的采集和自动解析。

- 上板性能采集

  通过上板性能采集，可以直接测定算子昇腾AI处理器上的运行时间。该方式适合在板环境中快速定位算子性能问题。

  基于可执行文件demo通过msopprof执行算子调优：

  ```
  msopprof ./demo
  ```

  - 性能数据说明
    命令完成后，会在默认目录下生成以“OPPROF_{timestamp}_XXX”命名的文件夹,性能数据文件夹结构示例如下：

    ```bash
    ├──dump                       # 原始的性能数据，用户无需关注
    ├──ArithmeticUtilization.csv  # cube/vector指令cycle占比
    ├──L2Cache.csv                # L2 Cache命中率，影响MTE2，建议合理规划数据搬运逻辑，增加命中率
    ├──Memory.csv                 # UB，L1和主存储器读写带宽速率
    ├──MemoryL0.csv               # L0A，L0B，和L0C读写带宽速率
    ├──MemoryUB.csv               # Vector和Scalar到UB的读写带宽速率
    ├──OpBasicInfo.csv            # 算子基础信息
    ├──PipeUtilization.csv        # 采集计算单元和搬运单元耗时和占比
    ├──ResourceConflictRatio.csv  # UB上的bank group、bank conflict和资源冲突率在所有指令中的占比
    └──visualize_data.bin         # MindStudio Insight呈现文件
    ```

查看具体的性能分析结果：

```bash
# 查看Task Duration 以及各项数据
cat ./OPPROF_*/PipeUtilization.csv
```

## 仿真调优

可以使用 `msopprof simulator` 进行仿真性能分析，生成可视化的指令流水图等信息。命令如下：

```bash
SCENARIO_NUM=4                                                                     # 选择执行场景，可选0-4
mkdir -p build && cd build;                                                        # 创建并进入build目录
cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
msopprof simulator --soc-version=<soc_version> ./matrix_transpose
```

> 使用仿真调优功能前，需要在 `CMakeLists.txt` 中添加 `-g` 编译选项，用于生成调试信息，使仿真器能够采集指令流水图。`soc_version` 获取方式及仿真调优其他说明可参考[仿真调优样例](https://gitcode.com/cann/asc-devkit/tree/master/examples/01_simd_cpp_api/01_utilities/08_simulator)。

命令完成后，会在当前目录下生成以 `OPPROF_{timestamp}_XXX` 命名的文件夹，产物结构如下：

```text
OPPROF_{timestamp}_XXX/
├── dump                    // 原始性能数据，用户无需关注
└── simulator
    ├── core*.veccore*/     // 各向量核的仿真指令流水图文件
    └── visualize_data.bin  // MindStudio Insight呈现文件
```

执行后，可以在 **MindStudio Insight** 中打开 `visualize_data.bin` 查看可视化指令流水图。

更多 `msOpProf` 工具使用方法，请参见[MindStudio工具调优（msOpProf）快速入门](https://www.hiascend.com/document/detail/zh/canncommercial/900/devaids/optool/docs/zh/quick_start/msopprof_quick_start.md)。
