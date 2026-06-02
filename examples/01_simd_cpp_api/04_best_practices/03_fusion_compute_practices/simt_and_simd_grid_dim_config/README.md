# gridDim配置与vf调用优化样例

## 概述

本样例以Gather计算为例，介绍Ascend C SIMD与SIMT混合编程场景下的gridDim配置和vf函数调用优化思路。样例包含5个Case（23个场景），从空壳核函数调度基线到不同shape下的gridDim/blockDim配置优化。

## 支持的产品

- Ascend 950PR/Ascend 950DT

## 支持的CANN软件版本

- \> CANN 9.0.0

## 目录结构介绍

```text
├── simt_and_simd_grid_dim_config
│   ├── figures                // README中的图片资源
│   ├── CMakeLists.txt         // 编译工程文件
│   ├── grid_config.asc        // Ascend C算子实现 & 调用样例
│   └── README.md
```

## 样例描述

- 计算公式：

  $$output[i][j] = input[index[i][j]]$$

  - input为输入张量，数据类型为float，形状因Case不同而异：Case 0/1/2为[2200000]，Case 3为[200000]，Case 4为[20000]
  - index为索引张量，数据类型为uint32，形状因Case不同而异：Case 0/1/2为[1024, 2048]，Case 3为[64, 2048]，Case 4为[8, 2048]，索引范围为[0, input_total_length)
  - output为输出张量，数据类型为float，形状与index相同
  - 计算过程：根据index[i][j]索引从input中采集对应位置的数据，写入output[i][j]位置

- 样例规格：

  <table>
  <tr><td rowspan="1" align="center">样例类型(OpType)</td><td colspan="4" align="center">GatherAdds</td></tr>
  <tr><td rowspan="3" align="center">样例输入</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2200000]/[200000]/[20000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">[1024,2048]/[64,2048]/[8,2048]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">样例输出</td><td align="center">output</td><td align="center">[1024,2048]/[64,2048]/[8,2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">核函数名</td><td colspan="4" align="center">empty_kernel / gather_adds_kernel</td></tr>
  </table>

## 样例实现

### Case实现说明

本样例将23个`SCENARIO_NUM`归为5类Case，先建立调度基线，再逐步观察vf调用、大shape、中shape和小shape下的配置选择。

| Case   | SCENARIO_NUM           | Shape                      | 核函数                                  | 对比目的                                  |
| ------ | ---------------------- | --------------------------------- | --------------------------------------- | ----------------------------------------- |
| Case 0 | 1-5     | [1024, 2048]           | empty_kernel                            | 测1 / 32 / 64 / 128 / 1024核的空核调度开销 |
| Case 1 | 6-8              | [1024, 2048]              | empty_kernel                            | 对比1次vf调用和2次vf调用的头开销差异        |
| Case 2 | 9-11            | [1024, 2048]      | gather_adds_kernel                      | 大shape下，gridDim.x设置1024 / 128 / 64，对比性能差异 |
| Case 3 | 12-15      | [64, 2048]        | gather_adds_kernel                      | 中shape下，gridDim.x设置16 / 32 / 64 / 128，对比性能差异|
| Case 4 | 16-23 | [8, 2048] | gather_adds_kernel | 在小shape下：①gridDim.x设置4 / 8 / 16 / 32 / 64对比性能差异；②固定核数16 / 32 / 64，把blockDim.x从2048调整到每核实际元素数(1024 / 512 / 256)对比性能差异 |

### 性能指标说明

| 指标                | 说明                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Task整体耗时，包含调度到加速器的时间、加速器上的执行时间以及响应结束时间                  |

### Case 0: 空核函数调度基线（SCENARIO_NUM=1-5）

**样例目标**：用空壳核函数建立不同gridDim下的Task调度耗时基线，展示逻辑核数和调度头开销的关系。

**核心实现**：

- 核函数`empty_kernel`仅调用空壳vf函数`simt_empty`，函数体内无实际计算逻辑，用于隔离测试gridDim配置带来的开销
- 通过`SCENARIO_NUM=1~5`控制五种gridDim配置：1 / 32 / 64 / 128 / 1024，blockDim.x固定2048，每次调用1次vf

**关键代码**：

```cpp
__simt_vf__ __launch_bounds__(THREAD_NUM) inline void simt_empty(...) {}

__global__ __vector__ void empty_kernel(...)
{
    for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
        asc_vf_call<simt_empty>(dim3(THREAD_NUM), input, index);
    }
}
```

**性能数据**：

| SCENARIO_NUM | Block Dim | Task Duration(us) |
| :----------: | :-------: | :---------------: |
| 1 | 1 | 3.933 |
| 2 | 32 | 4.413 |
| 3 | 64 | 4.602 |
| 4 | 128 | 5.269 |
| 5 | 1024 | 13.687 |

#### 从gridDim到逻辑核/物理核

在SIMT kernel启动时，`gridDim.x`决定本次任务要启动多少个block。对于本样例的一维grid配置，可以直接把`gridDim.x`理解为要启动的**逻辑核数量**。

这些逻辑核最终要落到硬件实际存在的Vector Core上执行，因此需要引入两个概念：

- **逻辑核**：由`gridDim.x`配置出来的block数量，是软件侧希望启动的并行任务数
- **物理核**：硬件实际拥有的Vector Core数量，需要通过`aclrtGetDeviceInfo`在运行期查询获取（本样例环境为64），查询方法参考[运行期如何查询物理核数](#运行期查询物理核数)

两者的关系会影响调度开销：当`gridDim.x ≤ 64`时，所有逻辑核可以一次占满物理核并行执行；当`gridDim.x > 64`时，超出64的逻辑核必须等待前面的逻辑核执行完、让出物理核后才能被调度，等待开销会随超出的逻辑核数量增加而上升。

#### 调度头开销总结

| 对比 | Block Dim 变化 | Task Duration 变化 | 现象 |
| ---- | -------------- | ------------------ | ---- |
| 1 → 32 | 32× | 3.933 → 4.413（1.12×） | 逻辑核数 ≪ 物理核，均在物理核内，仅启动更多核的微小固定开销 |
| 32 → 64 | 2× | 4.413 → 4.602（1.04×） | 逻辑核数 ≤ 物理核，一次占满，涨幅平缓 |
| 64 → 128 | 2× | 4.602 → 5.269（1.15×） | 逻辑核数超物理核，出现等待，开始上升 |
| 128 → 1024 | 8× | 5.269 → 13.687（2.59×） | 逻辑核数 ≫ 物理核，等待开销陡增 |

可以总结出两条规律：

1. **逻辑核数 ≤ 物理核数时调度开销增长较缓**：1核、32核、64核都在物理核数以内、无需等待，Task Duration从3.933us缓慢升到4.602us
2. **逻辑核数超过物理核数后调度开销随等待量增长**：128核有64个、1024核有960个逻辑核必须等待物理核空闲后才能被调度，调度耗时随需要等待的逻辑核数量近似线性增加

#### 运行期查询物理核数

通过`aclrtGetDeviceInfo`在运行期查询设备的物理Vector Core数量，并将查询到的物理核数传入核函数。本样例运行环境的物理核数为64。关键代码如下：

```cpp
int32_t device_id = 0;
int64_t physical_aiv_core_num = 0;
aclError ret = aclrtGetDeviceInfo(
    static_cast<uint32_t>(device_id),
    ACL_DEV_ATTR_VECTOR_CORE_NUM,
    &physical_aiv_core_num);
if (ret == ACL_SUCCESS) {
    printf("Physical AIV (Vector Core) num: %ld\n", physical_aiv_core_num);
}
```

调用前需先完成`aclInit`与`aclrtSetDevice`。

`aclrtDevAttr`枚举值说明：

| 枚举值 | 含义 |
| ------ | ---- |
| `ACL_DEV_ATTR_VECTOR_CORE_NUM` | Vector Core (AIV) 数量 |

---

### Case 1: vf调用次数对头开销的影响（SCENARIO_NUM=6-8）

**样例目标**：在Case 0的基础上，对比1次和2次vf调用对Task Duration的影响，展示vf调用次数和调度头开销的关系。

**核心实现**：

- 与Case 0使用相同的空壳核函数`empty_kernel`，唯一变化的变量是将vf调用次数从1次增加到2次
- 通过`SCENARIO_NUM=6-8`分别启动32、64、1024个逻辑核，每次调用2次vf

**关键代码**：

```cpp
// Case 1: vf_call_times=2，核函数内循环调用2次vf
for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
    asc_vf_call<simt_empty>(dim3(THREAD_NUM), input, index);
}
```

**性能数据**：

| SCENARIO_NUM | Block Dim | vf调用次数 | Task Duration(us) |
| :----------: | :-------: | :--------: | :---------------: |
| 6 | 32 | 2 | 4.503 |
| 7 | 64 | 2 | 4.869 |
| 8 | 1024 | 2 | 14.982 |

**与Case 0对比**：

| 对比 | Block Dim | vf次数变化 | Task Duration变化 | 增幅 |
| ---- | :-------: | :--------: | :---------------: | :--: |
| SCENARIO_NUM=2→6 | 32 | 1→2 | 4.413 → 4.503 | +2.0% |
| SCENARIO_NUM=3→7 | 64 | 1→2 | 4.602 → 4.869 | +5.8% |
| SCENARIO_NUM=5→8 | 1024 | 1→2 | 13.687 → 14.982 | +9.5% |

**分析**：

- 32核和64核下，1次→2次vf调用耗时增长约2.0%和5.8%，涨幅较小
- 1024核下增幅较大（9.5%），但绝对增量仅1.295us，远小于核数变化带来的调度开销差异（Case 0中64核→1024核增长约9.085us）
- **结论**：vf调用次数对Task Duration影响相对较小，核数配置是影响调度开销的主要因素

<img src="figures/fig_case01_scheduling.png" alt="Case 0/1 调度开销：逻辑核数与vf调用次数" width="600" />

---

### Case 2: 大shape Gather配置不同gridDim性能对比（SCENARIO_NUM=9-11）

**样例目标**：对比同一大shape下执行Gather计算，配置不同gridDim的性能差异。

**核心实现**：

- 三个场景均使用核函数`gather_adds_kernel`，差异在于逻辑核个数和每个线程处理的数据量
- input形状为[2200000]，index和output形状为[1024, 2048]，blockDim.x固定2048
- `vf_call_times = rows_per_block / thread_num`：每个线程以for循环方式处理本block负责的全部元素

**关键代码**：

```cpp
// simt_gather: SIMT vf函数 —— 以thread_num为步长做for循环
for (uint32_t vf_idx = 0; vf_idx < vf_loop_times; vf_idx++) {
    uint32_t idx = threadIdx.x + index_start + vf_idx * thread_num;
    uint32_t gather_idx = index[idx];
    output[threadIdx.x + vf_idx * thread_num] = input[gather_idx];
}

// gather_adds_kernel: 核函数通过thread_num适配不同场景
asc_vf_call<simt_gather>(dim3(thread_num), ..., vf_loop_times, thread_num);
AscendC::DataCopy(output_global, gather_output, rows_per_block);
```

**性能数据**：

| SCENARIO_NUM | Block Dim | 每核处理元素数 | 每线程处理数据量(vf_call_times) | Task Duration(us) | 说明 |
| :----------: | :-------: | :-------: | :-------: | :---------------: | ---- |
| 9 | 1024 | 2048 | 1 | 78.02 | 逻辑核 ≫ 物理核，排队等待 |
| 10 | 128 | 16384 | 8 | 63.161 | 逻辑核 > 物理核，部分等待 |
| **11** | **64** | **32768** | **16** | **62.474** | **逻辑核 = 物理核，最优** |

<img src="figures/fig_case2_large.png" alt="Case 2 大shape[1024, 2048] Task Duration 随核数变化" width="600" />

**分析**：

- 1024→128：Task Duration从78.02us降到63.161us（↓19.1%），逻辑核数从远超物理核降到2倍物理核，等待开销减少
- 128→64：Task Duration从63.161us降到62.474us（↓1.1%），逻辑核数降到物理核数，无需等待，进一步小幅优化
- **结论**：大shape下逻辑核数应等于物理核数（64），核数超过物理核数排队等待开销会显著影响性能

---

### Case 3: 中shape Gather配置不同gridDim性能对比（SCENARIO_NUM=12-15）

**样例目标**：对比同一中shape下执行Gather计算，配置不同gridDim的性能差异。

**核心实现**：

- 四个场景均使用核函数`gather_adds_kernel`，差异在于逻辑核个数和每个线程处理的数据量
- input形状为[200000]，index和output形状为[64, 2048]，blockDim.x固定2048
- 主样例通过`SCENARIO_NUM=12-15`设置16 / 32 / 64 / 128核

**性能数据**：

| SCENARIO_NUM | Block Dim | 每核处理元素数 | 每线程处理数据量(vf_call_times) | Task Duration(us) | 说明 |
| :----------: | :-------: | :-------: | :-------: | :---------------: | ---- |
| 12 | 16 | 8192 | 4 | 82.379 | 核数偏少，单核工作量重 |
| 13 | 32 | 4096 | 2 | 53.865 | 继续加核仍明显收益 |
| **14** | **64** | **2048** | **1** | **40.398** | **最优，逻辑核数=物理核** |
| 15 | 128 | 1024 | 1 | 54.785 | 越过物理核，耗时增加 |

<img src="figures/fig_case3_medium.png" alt="Case 3 中shape[64, 2048] Task Duration 随核数变化" width="600" />

**分析**：

- 16 → 32 → 64：核数翻倍时Task Duration持续下降（82.379 → 53.865 → 40.398），说明数据量较大、每核处理元素数（≥2048）足够，加到物理核上限前增加并行核数的收益始终明显
- 64 → 128：核数翻倍越过64物理核，Task Duration从40.398us升到54.785us，**耗时增加约35.6%**。多出的64个逻辑核必须排队等物理核空闲
- **结论**：中shape应打满物理核（64），但**打满≠越多越好**——核数超过物理核数，超出的逻辑核只能排队等待，必须以物理核数为上限

---

### Case 4: 小shape Gather配置不同gridDim与线程数性能对比（SCENARIO_NUM=16~23）

**样例目标**：小shape下每核工作量较小，启动更多核带来的调度头开销会反超有效计算的收益，必须根据shape实测选择核数；并进一步说明线程数也应按实际工作量设置。

#### 逻辑核数和性能的关系（SCENARIO_NUM=16-20）

**核心实现**：

- 五个场景均使用核函数`gather_adds_kernel`，blockDim.x固定2048
- input形状为[20000]，index和output形状为[8, 2048]
- 主样例通过`SCENARIO_NUM=16~20`设置4 / 8 / 16 / 32 / 64核

**性能数据**：

| SCENARIO_NUM | Block Dim | 每核处理元素数 | 每线程处理数据量(vf_call_times) | Task Duration(us) | 说明 |
| :----------: | :-------: | :-------: | :-------: | :---------------: | ---- |
| 16 | 4 | 4096 | 2 | 9.989 | 核数太少，单核工作量重 |
| 17 | 8 | 2048 | 1 | 8.755 | 继续加核仍明显收益 |
| **18** | **16** | **1024** | **1** | **8.533** | **最低点，最优核数** |
| 19 | 32 | 512 | 1 | 9.004 | 核数增加，耗时增加 |
| 20 | 64 | 256 | 1 | 10.744 | 打满物理核，头开销反超 |

<img src="figures/fig_case4_small_griddim.png" alt="Case 4 小shape[8, 2048] Task Duration 随核数变化" width="600" />

**分析**：

- 折线呈"先降后升"的谷形，**最优核数 = 16（物理核数的1/4）**，并非打满64核
- 16 → 32：核数翻倍，Task Duration从8.533us升到9.004us，**耗时增加约5.5%**
- 32 → 64：核数继续翻倍，Task Duration从9.004us升到10.744us，**耗时增加约19.3%**。小shape下每核任务量较小（256/512元素），增加核数带来的调度开销反超并行收益
- 与Case 3对照：同样的kernel与线程数，shape小到一定程度后最优核数从64降到16，说明"最优核数"随shape缩小而变化，必须实测
- **结论**：小shape无法照搬"打满物理核"的经验，必须像本Case一样设置核数实测。更小的数据量最优核数要小于最大物理核数

#### 线程数调优：把blockDim.x调整到每核实际处理的元素数（SCENARIO_NUM=21-23）

**样例目标**：在Case 4固定核数的基础上，进一步说明小shape下每block的线程数也应按实际工作量设置——把线程数从硬件上限2048调整到每核实际处理的元素数，可消除空转线程、性能更优。

**核心实现**：

- 核函数仍是`gather_adds_kernel`，逻辑完全不变；唯一变化的变量是**blockDim.x = 每核实际处理元素数**（`8 * 2048 / 核数`）：16核→1024线程、32核→512线程、64核→256线程
- 此时`rows_per_block == thread_num`，每个线程只需处理一个数据（vf_call_times=1），没有任何线程空转

**关键代码**：

```cpp
// 把blockDim.x设为每核实际处理的元素数，而不是固定2048
uint32_t thread_num = SMALL_SHAPE_TOTAL_LENGTH / num_blocks;  // 16核→1024 / 32核→512 / 64核→256
asc_vf_call<simt_gather>(dim3(thread_num), ..., vf_loop_times, thread_num);
```

**性能数据**：

| 核数 | 每核元素数 | 2048线程 Task Duration(us) | 线程数=元素/核 Task Duration(us) | 性能变化 |
| :--: | :--------: | :------------------------: | :------------------------------: | :--: |
| 16 | 1024 | 8.533（SCENARIO_NUM=18） | 7.832（SCENARIO_NUM=21，1024线程） | ↓8.3% |
| 32 | 512 | 9.004（SCENARIO_NUM=19） | 7.959（SCENARIO_NUM=22，512线程） | ↓11.6% |
| 64 | 256 | 10.744（SCENARIO_NUM=20） | 8.820（SCENARIO_NUM=23，256线程） | ↓17.6% |

下图为同核数下2048线程与"线程数=每核实际元素数"的Task Duration对比：

<img src="figures/fig_case4_thread_tuning.png" alt="Case 4 小shape[8, 2048] 线程数调优：2048线程 vs 线程数=每核实际元素数" width="600" />

**分析**：

- 固定核数下，线程数调到每核实际处理的元素数后Task Duration均下降：16核↓8.3%、32核↓11.6%、64核↓17.6%
- 64核下降幅尤为明显（约17.6%），说明线程空转浪费对性能影响较大
- **结论**：小shape下不仅核数要按shape实测，blockDim.x也应按每核实际元素数设置。当每核元素数小于硬件线程上限2048时，把线程数从2048调到实际处理的元素数即可去掉空转线程，性能更优；只有当每核元素数 ≥ 2048（如大shape/中shape）时才取满2048线程

---

## 性能对比总结

### Ascend 950PR性能数据

**调度开销随核数变化（Case 0）**：

| SCENARIO_NUM | Block Dim | Task Duration(us) | 说明 |
| ------------ | --------- | ----------------- | ---- |
| 1 | 1 | 3.933 | 调度基线，单核启动开销 |
| 2 | 32 | 4.413 | 核数 < 物理核 |
| 3 | 64 | 4.602 | 核数 = 物理核，调度耗时增长较缓 |
| 4 | 128 | 5.269 | 核数 > 物理核（2×），调度开始上升 |
| 5 | 1024 | 13.687 | 核数 ≫ 物理核（16×），调度开销主导 |

**vf调用次数对头开销的影响（Case 1）**：

| 对比 | Block Dim | vf次数 | Task Duration(us) | 增幅 |
| ---- | --------- | ------ | ----------------- | ---- |
| SCENARIO_NUM=2→6 | 32 | 1→2 | 4.413 → 4.503 | +2.0% |
| SCENARIO_NUM=3→7 | 64 | 1→2 | 4.602 → 4.869 | +5.8% |
| SCENARIO_NUM=5→8 | 1024 | 1→2 | 13.687 → 14.982 | +9.5% |

- vf调用次数对Task Duration影响相对较小，核数配置是影响调度开销的主要因素

**大shape的最优核数（Case 2）**：

| SCENARIO_NUM | 核数 | 每核处理元素数 | Task Duration(us) |
| :----------: | :--: | :-------: | :---------------: |
| 9 | 1024 | 2048 | 78.02 |
| 10 | 128 | 16384 | 63.161 |
| **11** | **64** | **32768** | **62.474** |

- 最优核数为 **64核**；1024→128→64 耗时持续下降，核数降到物理核数后无需排队等待，性能最优

**中shape的最优核数（Case 3）**：

| SCENARIO_NUM | 核数 | 每核处理元素数 | Task Duration(us) |
| :----------: | :--: | :-------: | :---------------: |
| 12 | 16 | 8192 | 82.379 |
| 13 | 32 | 4096 | 53.865 |
| **14** | **64** | **2048** | **40.398** |
| 15 | 128 | 1024 | 54.785 |

- 最优核数在 **64核**；16→32→64 耗时持续下降，gridDim在增加到物理核上限过程中持续受益
- 64→128：核数越过物理核上限，耗时增长约35.6%

**小shape的最优核数（Case 4）**：

| SCENARIO_NUM | 核数 | 每核处理元素数 | Task Duration(us) |
| :----------: | :--: | :-------: | :---------------: |
| 16 | 4 | 4096 | 9.989 |
| 17 | 8 | 2048 | 8.755 |
| **18** | **16** | **1024** | **8.533** |
| 19 | 32 | 512 | 9.004 |
| 20 | 64 | 256 | 10.744 |

- 最优核数在 **16核**（物理核数的1/4），不是打满的64核；16→32 耗时增加约5.5%，32→64 耗时增加约19.3%
- 与Case 3对照：同样的kernel与线程数，shape小到一定程度后最优核数从64降到16

**小shape的线程数调优（Case 4）**：

| 核数 | 每核元素数 | 2048线程 Task Duration(us) | 线程数=元素/核 Task Duration(us) | 提升 |
| :--: | :--------: | :------------------------: | :------------------------------: | :--: |
| 16 | 1024 | 8.533（SCENARIO_NUM=18） | 7.832（SCENARIO_NUM=21） | ↓8.3% |
| 32 | 512 | 9.004（SCENARIO_NUM=19） | 7.959（SCENARIO_NUM=22） | ↓11.6% |
| 64 | 256 | 10.744（SCENARIO_NUM=20） | 8.820（SCENARIO_NUM=23） | ↓17.6% |

- 固定核数下，线程数调到每核实际处理元素数后Task Duration均下降

## 调优建议

1. **大shape / 中shape，优先匹配物理核数**：逻辑核数应尽量等于物理核数，避免逻辑核不足（工作量重）或超过物理核（排队等待）。
2. **小shape需实测核数性能**：当数据量较小时，启动更多核带来的调度头开销会反超收益，最优核数不一定是最大物理核数。可参考Case 4通过分核测试寻找最优核数。
3. **小shape下线程数按每核实际处理元素数设置**：blockDim.x不必固定在硬件上限2048。当每核处理的元素数小于2048时，vf函数上多出的线程会完全空转，把线程数调到每核实际元素数即可去掉空转、性能更优（见Case 4 SCENARIO_NUM=21-23）。

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量
  请根据当前环境上CANN开发套件包的[安装方式](../../../../../docs/quick_start.md#prepare&install)，选择对应配置环境变量的命令。

  - 默认路径，root用户安装CANN软件包

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - 默认路径，非root用户安装CANN软件包

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - 指定路径install_path，安装CANN软件包

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- 样例执行

  ```bash
  SCENARIO_NUM=1                       # 选择执行场景，可选1-23
  mkdir -p build && cd build;          # 创建并进入build目录
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # 编译工程
  ./grid_config                        # 执行样例
  ```

- 编译选项说明

  | 选项             | 可选值      | 说明              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `1`-`23` | 样例类型，默认为1 |

  执行结果如下，说明精度对比成功。

  ```text
  [Success] Case accuracy is verification passed.
  ```

## 性能分析

使用 `msprof` 工具获取详细性能数据：

```bash
msprof ./grid_config   # 分析性能
```

当前目录下会生成PROF_前缀的文件夹，`mindstudio_profiler_output`目录保存Host和各个Device的性能数据汇总，性能数据分析推荐查看该目录下文件

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # 保存Host和各个Device的性能数据汇总
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```
查看具体的性能分析结果：
```
# 查看Task Duration 以及各项数据
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
