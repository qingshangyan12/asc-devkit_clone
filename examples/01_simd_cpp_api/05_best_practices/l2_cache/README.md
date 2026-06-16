# L2 Cache Mode 最佳实践样例

## 概述

本样例系统性地展示 L2 Cache Mode 配置对不同数据访问模式下的性能影响。样例覆盖**数据复用场景**和**流式数据场景**两大类，通过 7 个 Case 逐步揭示 L2 Cache 的收益、局限以及全局效应，帮助开发者理解何时应开启/关闭 L2 Cache，以及如何通过分片策略最大化 L2 Cache 的利用率。

**优化路径**：

```
复用数据场景（数据需多次读取）
  Case 1: N方向切4份，每份重复搬4次，L2默认开启（基准）
  Case 2: 同Case 1，但关闭L2 Cache → 与Case 1对比，展示关闭L2对复用场景的劣化
  Case 3: 整块重复搬4次，L2默认开启 → 与Case 1对比，展示L2容量约束下分片的必要性

流式数据场景（数据只读一次）
  Case 4: Add算子(双缓冲)，L2默认开启（基准）
  Case 5: 同Case 4，但流式数据关闭L2 Cache → 与Case 4对比，展示bypass对单算子的收益

全局视角（多算子流水线）
  Case 6: Add(L2开启，污染L2) + DataCopyRepeat(分片重复) → 展示L2污染影响后续算子
  Case 7: Add(L2 bypass) + DataCopyRepeat(分片重复) → 与Case 6对比，展示bypass保后续算子性能
```

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── l2_cache
│   ├── scripts
│   │   └── gen_data.py         // 输入数据和真值数据生成脚本文件
│   ├── CMakeLists.txt          // 编译工程文件
│   ├── l2_cache.asc            // Ascend C 样例入口（kernel 调用 + main 函数）
│   ├── l2_cache.h              // Kernel 实现（DataCopyRepeat + Add）
│   ├── data_utils.h            // 数据读入写出函数
│   └── README.md               // 样例说明文档
```

## 样例描述

### 数据复用场景（Case 1-3）

本组场景输入为 half 类型二维矩阵，shape 为 [12288, 12288]，单矩阵数据量为：

$$单矩阵数据量 = 12288 \times 12288 \times 2B = 301,989,888 B \approx 301.99 MB$$

使用 `DataCopyPad` 将数据从 GM 搬运到 UB，通过对比不同的搬运策略和 L2 Cache 配置，观察 L2 命中和 MTE2 性能的差异。本组场景不包含计算逻辑，仅执行数据搬运操作。

单次 `DataCopyPad` 搬运的源数据量按 `tileM × tileN × sizeof(half)` 计算：

$$单次搬运数据量 = 64 \times 1024 \times 2B = 131,072 B$$

### 流式数据场景（Case 4-5）

本组场景实现的是固定 shape 为 [12288, 12288] 的两个矩阵相加：

$$z = x + y$$

- x：输入，形状为 [12288, 12288]，数据类型为 half
- y：输入，形状为 [12288, 12288]，数据类型为 half
- z：输出，形状为 [12288, 12288]，数据类型为 half

采用双缓冲（Ping-Pong）技术实现数据搬运与向量计算的流水线并行，通过对比 L2 Cache 开启和 bypass 的差异，观察流式数据对 MTE2 搬运效率的影响。

### 流水线场景（Case 6-7）

本组场景将上述两类算子组合为流水线：先执行 Add 算子（Case 4/5），再执行 DataCopyRepeat 算子（Case 1）。通过对比 Add 流式数据的 L2 开启 vs bypass，观察前序算子对后序算子 L2 命中率的全局影响。

- Add 算子读取 `add_x.bin`、`add_y.bin`（流式数据，只读一次）
- DataCopyRepeat 算子读取 `repeat_input.bin`（复用数据，分片重复 4 次）

两个算子读取**不同的数据**，以此展示：Add 的流式数据写入 L2 Cache 会占据宝贵的缓存空间，将后续 DataCopyRepeat 需要用到的数据挤出 L2，导致其 L2 命中率下降。

## 核心特性总览

<table>
<tr><th>分组</th><th>Case</th><th>主要观察对象</th><th>对比方法</th></tr>
<tr><td rowspan="3">复用数据</td><td>Case 1</td><td>分片重复搬运 + L2开启（基线）</td><td>分片重复搬运，L2默认开启</td></tr>
<tr><td>Case 2</td><td>L2 关闭对复用数据的劣化</td><td>与Case 1对比：仅关闭L2，其余相同</td></tr>
<tr><td>Case 3</td><td>L2 容量约束：整块 vs 分片</td><td>与Case 1对比：整块重复 vs 分片重复</td></tr>
<tr><td rowspan="2">流式数据</td><td>Case 4</td><td>流式数据 + L2开启（基线）</td><td>Add双缓冲，L2默认开启</td></tr>
<tr><td>Case 5</td><td>L2 bypass对流式数据的收益</td><td>与Case 4对比：仅关闭流式数据L2</td></tr>
<tr><td rowspan="2">流水线</td><td>Case 6</td><td>流式数据L2污染 + 复用数据受损</td><td>Add(L2开)→Repeat(L2开)</td></tr>
<tr><td>Case 7</td><td>流式数据L2 bypass + 复用数据收益</td><td>与Case 6对比：Add改为L2 bypass</td></tr>
</table>

## 性能指标说明

### MTE2 性能表

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准。</td></tr>
<tr><td>*_total_cycles</td><td>Task 执行总 cycle 数。</td></tr>
<tr><td>*_time(μs)</td><td>Task 在对应 AI Core 上的理论执行时间，单位为 μs。</td></tr>
<tr><td>*_mte2_time(μs)</td><td>MTE2 类型指令（GM → AI Core 搬运类指令）耗时，单位为 μs。</td></tr>
<tr><td>*_mte2_ratio</td><td>MTE2 类型指令的 cycle 数在 total cycle 数中的占比。</td></tr>
<tr><td>*_mte3_time(μs)</td><td>MTE3 类型指令（UB → GM 搬运类指令）耗时，单位为 μs。</td></tr>
<tr><td>*_mte3_ratio</td><td>MTE3 类型指令的 cycle 数在 total cycle 数中的占比。</td></tr>
<tr><td>*_vec_time(μs)</td><td>vec 类型指令（向量类运算指令）耗时，单位 μs。</td></tr>
<tr><td>*_vec_ratio</td><td>vec 类型指令的 cycle 数在 total cycle 数中的占用比。</td></tr>
</table>

### L2Cache 性能表

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准。</td></tr>
<tr><td>*_total_cycles</td><td>Task 执行总 cycle 数。</td></tr>
<tr><td>*_time(μs)</td><td>Task 在对应 AI Core 上的理论执行时间，单位为 μs。</td></tr>
<tr><td>*_write_cache_hit</td><td>写 Cache 命中的次数。</td></tr>
<tr><td>*_write_cache_miss_allocate</td><td>写 Cache 缺失后重新分配缓存的次数。</td></tr>
<tr><td>*_r*_read_cache_hit</td><td>读 r* 通道 Cache 命中次数，r0/r1 为硬件读写的两个通道，分析总读命中次数时需将两个通道累加。</td></tr>
<tr><td>*_r*_read_cache_miss_allocate</td><td>读 r* 通道 Cache 缺失后重新分配的次数，r0/r1 为硬件读写的两个通道，分析总读缺失次数时需将两个通道累加。</td></tr>
<tr><td>*_read_local_l2_hit</td><td>读 Cache 命中的次数。</td></tr>
<tr><td>*_read_local_l2_miss</td><td>读 Cache 缺失次数。</td></tr>
<tr><td>*_read_local_l2_victim</td><td>读 Cache 未命中并触发 Cache 中数据被换出的次数。</td></tr>
</table>

> **说明**：Atlas A2/A3 系列和 Ascend 950 系列的 profiler 输出字段不同，命中率计算方式也不同。
> Atlas A2/A3 系列使用 `l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)`；
> Ascend 950 系列使用 `l2cache_hit_ratio = hit / (hit + miss + victim)`。

## 样例实现与性能分析

为便于表格展示，下文将 Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品简称为 Atlas A2/A3 系列，将 Ascend 950PR/Ascend 950DT 简称为 Ascend 950 系列。

### Case 1: 分片重复搬运 + L2 Cache 默认开启

**实现方式**：参考 `KernelDataCopyRepeat::ProcessSplitFourAndRepeat4Times()` 函数实现

将 N 方向切为 4 份，在每个分片内连续重复搬运 4 次，利用 L2 Cache 加速第 2~4 轮的读取。

**关键代码**：

```cpp
// N 方向切 4 份，每份连续搬运 4 次
constexpr uint32_t quarterN = n / 4;
for (uint32_t splitIdx = 0; splitIdx < 4; splitIdx++) {
    RepeatCopy(splitIdx * quarterN, quarterN, 4);
}
// RepeatCopy 内部：按 tile 粒度遍历分片，外层 repeatTimes 次
```

**样例配置**：

- 矩阵 shape：[12288, 12288]，half 类型
- 单矩阵数据量：301.99 MB，N 方向切 4 份后每份约 75.50 MB
- A2/A3 L2 Cache 容量：约 192 MB，950 系列 L2 Cache 容量：约 128 MB
- 单份数据（75.50 MB）可完整保留在 L2 Cache 中
- Tile 大小：[64, 1024]，单次 `DataCopyPad` 搬运 131,072 B
- 总搬运数据量：301.99 MB × 4 = 1207.96 MB

**数据搬运模式**：

```text
GM 矩阵: [12288, 12288]
┌────── N/4 ──────┬────── N/4 ──────┬────── N/4 ──────┬────── N/4 ──────┐
│     分片0        │     分片1        │     分片2        │     分片3        │
└─────────────────┴─────────────────┴─────────────────┴─────────────────┘

每个分片内连续搬运 4 轮：
  分片0: 第1轮从GM读取(写入L2)，第2~4轮优先从L2读取
  分片1: 第1轮从GM读取(写入L2)，第2~4轮优先从L2读取
  分片2: 第1轮从GM读取(写入L2)，第2~4轮优先从L2读取
  分片3: 第1轮从GM读取(写入L2)，第2~4轮优先从L2读取
```

**L2 策略**：默认开启 `CacheMode::CACHE_MODE_NORMAL`（不调用 `SetL2CacheHint`）

**预期**：L2 命中率约 75%（第 1 轮 miss，第 2~4 轮命中）

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>N切4份，每份重复4次，L2默认开启</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>N切4份，每份重复4次，L2默认开启</td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

Ascend 950 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

---

### Case 2: 分片重复搬运 + L2 Cache 关闭

**实现方式**：参考 `KernelDataCopyRepeat::ProcessSplitFourAndRepeat4TimesL2Disable()` 函数实现

搬运模式与 Case 1 完全相同，唯一区别是在搬运前调用 `SetL2CacheHint(CACHE_MODE_DISABLE)` 关闭 L2 Cache。

**关键代码**：

```cpp
// 关闭 L2 Cache，DataCopy 直接从 GM 搬运到 UB，不写入 L2
srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
ProcessSplitFourAndRepeat4Times(); // 搬运逻辑与 Case 1 完全相同
```

**L2 策略**：`SetL2CacheHint(CACHE_MODE_DISABLE)`，所有 4 轮搬运都从 GM 直接读取

**对比方式**：与 Case 1 对比，仅改变 L2 Cache 配置（开 → 关），其余配置完全相同

**预期**：L2 命中率约 0%（数据不写入 L2，无法命中），MTE2 耗时约等于 Case 1 的 4 倍（4 轮全部走 GM），端到端性能明显劣化

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>N切4份，每份重复4次，L2关闭</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>N切4份，每份重复4次，L2关闭</td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

Ascend 950 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

**优化效果分析**：

- 与 Case 1 对比，Case 2 关闭 L2 Cache 后，4 轮搬运都需要从 GM 读取数据，没有 L2 命中收益
- 说明对于需要多次重复读取的数据，保持 L2 Cache 开启可以获得显著的搬运加速

> **性能优化建议**：对于需要重复访问的数据，应保持 L2 Cache 默认开启（`CACHE_MODE_NORMAL`）。L2 Cache 可以缓存首次读取的数据，后续重复访问时直接从 L2 读取，速度远快于从 GM 重新读取。

---

### Case 3: 整块重复搬运 + L2 Cache 默认开启

**实现方式**：参考 `KernelDataCopyRepeat::ProcessRepeatWhole4Times()` 函数实现

将整个矩阵作为一整块，连续重复搬运 4 次。

**关键代码**：

```cpp
// 整块矩阵连续搬运 4 次（不切分 N 方向）
RepeatCopy(0, n, 4);
```

**L2 策略**：默认开启 `CacheMode::CACHE_MODE_NORMAL`

**对比方式**：与 Case 1 对比，改变搬运策略（整块 vs 分片），总搬运数据量相同（均为 1207.96 MB）

**L2 容量分析**：

- 单矩阵数据量：301.99 MB
- A2/A3 L2 Cache 容量：约 192 MB
- 950 系列 L2 Cache 容量：约 128 MB
- 整块矩阵（301.99 MB）超出两款产品的 L2 Cache 容量，第 1 轮写入 L2 的数据会在第 2 轮开始前被逐出
- 因此第 2~4 轮的大部分数据需要重新从 GM 读取，命中率极低

**预期**：L2 命中率接近 0%，MTE2 耗时接近 Case 2（L2 关闭）的水平，端到端性能远差于 Case 1

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>整块矩阵，连续重复4次，L2默认开启</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>整块矩阵，连续重复4次，L2默认开启</td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

Ascend 950 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

**优化效果分析**：

- 与 Case 1 对比，Case 3 虽然 L2 Cache 同样开启，但由于单次工作集（301.99 MB）超出 L2 Cache 容量，缓存换出严重，命中率极低
- 说明即使 L2 Cache 开启，也需要通过分片策略控制单次工作集大小，使其落在 L2 Cache 可容纳范围内

> **性能优化建议**：即使 L2 Cache 开启，如果单次工作集超出 L2 容量，也无法获得复用收益。应通过分片策略，将重复访问限制在更小的数据范围内，使单次工作集可以完整保留在 L2 Cache 中。

---

### Case 4: Add 算子 + 双缓冲 + L2 Cache 默认开启

**实现方式**：参考 `KernelAdd::ProcessDoubleBuffer()` 函数实现

实现两个 [12288, 12288] 矩阵的加法运算，采用双缓冲流水线并行，L2 Cache 默认开启。

**关键代码**：

```cpp
// 使用 Ping-Pong 双缓冲区，交替搬运和计算
for (uint32_t loopIdx = 0; loopIdx < totalBlocks; loopIdx++) {
    int32_t eventID = ((loopIdx & 1) == 0 ? EVENT_ID0 : EVENT_ID1);
    // 等待上一轮搬运完成 → 搬运当前block → 计算 → 写出结果
    AscendC::DataCopy(xLocal, xGm[startElement], curLen);
    AscendC::DataCopy(yLocal, yGm[startElement], curLen);
    AscendC::Add(zLocal, xLocal, yLocal, curLen);
    AscendC::DataCopy(zGm[startElement], zLocal, curLen);
}
```

**样例配置**：

- 矩阵 shape：[12288, 12288]，half 类型
- M 方向均匀切分到 AIV_CORE_NUM 个核（A2/A3: 48 核，950: 64 核）
- 单核数据量：256 × 12288（A2/A3）或 192 × 12288（950）
- dataCopyLen：A2/A3 为 16384，950 为 21840，单次 DataCopy 搬运 32768 B
- 双缓冲（Ping-Pong）流水线并行

**L2 策略**：默认开启 `CacheMode::CACHE_MODE_NORMAL`

**Add 数据流分析**：

- x、y 输入数据每个元素只读取一次（流式数据），不会发生 L2 Cache Hit
- 默认 L2 开启时，MTE2 在搬运数据到 UB 的同时，还会将数据写入 L2 Cache（Cache Line 填充）
- 这个额外的写操作消耗了 L2 Cache 控制器的内部带宽

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>aiv_mte3_time(μs)</th><th>aiv_mte3_ratio</th><th>aiv_vec_time(μs)</th><th>aiv_vec_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>Add + 双缓冲，L2默认开启</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>Add + 双缓冲，L2默认开启</td></tr>
</table>

**优化效果分析**：

- MTE2 搬运耗时占比较大（> 70%），向量计算占比较小
- 流式数据的 L2 Cache 写操作产生不必要的带宽开销
- 此外，流式数据写入 L2 Cache 还会占据缓存空间，产生 cache 污染

> **性能优化建议**：对于只读取一次的流式数据，考虑关闭 L2 Cache 以避免不必要的 cache 写开销和污染。

---

### Case 5: Add 算子 + 双缓冲 + L2 Cache Bypass

**实现方式**：参考 `KernelAdd::ProcessDoubleBufferL2Bypass()` 函数实现

在 Case 4 基础上，对输入 x、y 调用 `SetL2CacheHint(CACHE_MODE_DISABLE)` 禁用 L2 Cache。

**关键代码**：

```cpp
// 对只读取一次的流式数据设置 L2 Cache bypass
// 数据直接从 GM 搬运到 UB，不经过 L2 Cache
xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
ProcessDoubleBuffer(); // 其余逻辑与 Case 4 完全相同
```

**L2 策略**：x、y 输入 `SetL2CacheHint(CACHE_MODE_DISABLE)`，数据直接从 GM 到 UB，不经过 L2 Cache

**对比方式**：与 Case 4 对比，仅关闭流式数据的 L2 Cache

**预期**：MTE2 耗时显著下降（约 30%），端到端性能明显提升

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>aiv_mte3_time(μs)</th><th>aiv_mte3_ratio</th><th>aiv_vec_time(μs)</th><th>aiv_vec_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>Add + 双缓冲，L2 bypass</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td>Add + 双缓冲，L2 bypass</td></tr>
</table>

**优化效果分析**：

- 与 Case 4 对比，关闭流式数据的 L2 Cache 后，MTE2 省去了不必要的 cache 写操作
- MTE2 耗时下降约 30%，端到端性能相应提升
- 同时避免了流式数据对 L2 Cache 的空间污染

> **性能优化建议**：对于数据量较大且只读取一次的流式数据（如 Add 算子的输入、Element-wise 操作的输入等），建议设置 `SetL2CacheHint(CACHE_MODE_DISABLE)` 禁用 L2 Cache，以减少不必要的 cache 写开销。

---

## 全局视角：多算子流水线场景

### 背景

在真实推理/训练场景中，多个算子按流水线方式串行执行（如 `LayerNorm → MatMul → Add → Activation`）。前序算子的 L2 Cache 配置不仅影响自身性能，还会影响后续算子的 L2 Cache 命中率，形成全局效应。

Case 6 和 Case 7 将流式数据场景的 Add 算子（Case 4/5）与复用数据的 DataCopyRepeat 算子（Case 1）组合为流水线，展示前序算子对后序算子的影响。

### 流水线结构

```text
Case 6：流式数据污染 L2 → 复用数据 L2 命中受损
  ┌─────────────────────┐   ┌──────────────────────────┐
  │  Add（Case 4 配置）  │   │  DataCopyRepeat（Case 1）  │
  │  x,y: L2 默认开启    │──▶│  repeat_input: L2 默认开启 │
  │  GM→UB 时写入 L2     │   │  分片重复4次，期望L2命中   │
  └─────────────────────┘   └──────────────────────────┘
    流式数据占用 L2 空间       →  复用数据命中率下降

Case 7：流式数据 L2 bypass → 复用数据 L2 命中正常
  ┌─────────────────────┐   ┌──────────────────────────┐
  │  Add（Case 5 配置）  │   │  DataCopyRepeat（Case 1）  │
  │  x,y: L2 Bypass     │──▶│  repeat_input: L2 默认开启 │
  │  GM→UB 不经过 L2    │   │  分片重复4次，期望L2命中   │
  └─────────────────────┘   └──────────────────────────┘
    L2 空间保持干净           →  复用数据命中率正常
```

---

### Case 6: Add(L2 开启) + DataCopyRepeat(分片重复)

**实现方式**：在 main 函数中先后启动两个 kernel：

1. `kernel_add<false>` — Add 算子，L2 默认开启（流式数据写入 L2）
2. `kernel_repeat<REPEAT_SPLIT_L2_ON>` — DataCopyRepeat 算子，分片重复 4 次

两个 kernel 读取不同的输入矩阵（Add 读取 add_x.bin、add_y.bin，Repeat 读取 repeat_input.bin）。

**L2 策略**：

- Add: x、y 默认 L2 开启 → MTE2 搬运同时写入 L2 Cache，占据缓存空间
- Repeat: 默认 L2 开启 → 期望利用 L2 复用，但 L2 已被 Add 的流式数据污染

**关键代码**（main 函数中）：

```cpp
// 第一个 kernel：Add（L2 开启）
kernel_add<false><<<AIV_CORE_NUM, nullptr, stream>>>(addInputDevice[0], addInputDevice[1], addOutputDevice);
aclrtSynchronizeStream(stream);

// 第二个 kernel：DataCopyRepeat（读取不同的输入矩阵）
kernel_repeat<REPEAT_SPLIT_L2_ON><<<AIV_CORE_NUM, nullptr, stream>>>(repeatInputDevice);
aclrtSynchronizeStream(stream);
```

**预期**：DataCopyRepeat 的 L2 命中率低于 Case 1（独立运行时），因为 Add 的流式数据写入了 L2 Cache，挤占了原本可用于缓存 repeat_input 的空间。

**MTE2 性能数据**（仅统计 DataCopyRepeat kernel 的耗时）：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>Add(L2开)→Repeat(L2开)</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>Add(L2开)→Repeat(L2开)</td></tr>
</table>

**L2Cache 性能数据**（仅统计 DataCopyRepeat kernel）：

Atlas A2/A3 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

Ascend 950 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

---

### Case 7: Add(L2 bypass) + DataCopyRepeat(分片重复)

**实现方式**：在 main 函数中先后启动两个 kernel：

1. `kernel_add<true>` — Add 算子，L2 bypass（流式数据不写入 L2）
2. `kernel_repeat<REPEAT_SPLIT_L2_ON>` — DataCopyRepeat 算子，分片重复 4 次

**L2 策略**：

- Add: x、y `SetL2CacheHint(CACHE_MODE_DISABLE)` → 数据不经过 L2，L2 空间保持干净
- Repeat: 默认 L2 开启 → L2 中有充足空间缓存 repeat_input

**关键代码**（main 函数中）：

```cpp
// 第一个 kernel：Add（L2 bypass，不污染 L2）
kernel_add<true><<<AIV_CORE_NUM, nullptr, stream>>>(addInputDevice[0], addInputDevice[1], addOutputDevice);
aclrtSynchronizeStream(stream);

// 第二个 kernel：DataCopyRepeat（L2 空间干净，命中率高）
kernel_repeat<REPEAT_SPLIT_L2_ON><<<AIV_CORE_NUM, nullptr, stream>>>(repeatInputDevice);
aclrtSynchronizeStream(stream);
```

**对比方式**：与 Case 6 对比，唯一的变量是 Add 的 L2 配置（开启 vs bypass）

**预期**：DataCopyRepeat 的 L2 命中率与 Case 1（独立运行）接近，Add 的 L2 bypass 保护了后续算子的 L2 复用

**MTE2 性能数据**（仅统计 DataCopyRepeat kernel 的耗时）：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>Add(L2 bypass)→Repeat(L2开)</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>Add(L2 bypass)→Repeat(L2开)</td></tr>
</table>

**L2Cache 性能数据**（仅统计 DataCopyRepeat kernel）：

Atlas A2/A3 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

Ascend 950 系列：

<table>
<tr><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_read_local_l2_hit</th><th>aiv_read_local_l2_miss</th><th>aiv_read_local_l2_victim</th><th>L2Cache命中率</th></tr>
<tr><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

**优化效果分析**：

- 对比 Case 6 vs Case 7，Add 的 L2 bypass 可以保护后续算子的 L2 Cache 命中率，避免流式数据污染 L2
- 说明在多算子流水线场景中，L2 Cache 的配置需要从全局视角考虑，前序算子的 L2 策略会影响后序算子的性能

> **性能优化建议**：在多算子流水线场景中，前序算子中的流式数据应设置 L2 bypass，以避免其占据 L2 Cache 空间挤出后续算子的热点数据。这是一个从全局视角出发的优化策略，需要结合上下游算子的数据访问模式通盘考虑。

---

## 优化要点总结

| 优化场景 | 数据模式 | L2 策略 | 收益来源 | 典型案例 |
|:---|:---|:---|:---|:---|
| 复用数据 + L2 开 | 同一数据被多次读取 | `CACHE_MODE_NORMAL`（默认） | 第一次从 GM 读取写入 L2，后续从 L2 读取 | 权重复用、特征图多次访问 |
| 复用数据 + 分片 | 单次工作集 < L2 容量 | `CACHE_MODE_NORMAL` + 分片策略 | 分片后工作集可完整保留在 L2 中，最大化命中率 | 大矩阵分片重复处理 |
| 流式数据 + L2 bypass | 数据只读取一次 | `CACHE_MODE_DISABLE` | 省去 L2 写操作开销，同时避免污染 | Add、Mul 等逐元素运算的输入 |
| 流水线 + L2 bypass | 前序算子流式，后序算子复用 | 前序 bypass，后序 NORMAL | 保护 L2 空间，让后序算子享受高 L2 命中率 | 推理管线：LayerNorm → Attention → FFN |

**L2 Cache Mode 决策树**：

```text
该数据会被多次读取吗？
  ├── 是 → 单次工作集是否小于 L2 容量？
  │        ├── 是 → 使用 CACHE_MODE_NORMAL（默认），配合分片策略
  │        └── 否 → 先分片，再在每个分片内使用 CACHE_MODE_NORMAL
  └── 否 → 是否为多算子流水线中的前序算子？
           ├── 是 → CACHE_MODE_DISABLE（保护 L2 给后序算子）
           └── 否 → CACHE_MODE_DISABLE（省去不必要的 cache 写开销）
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  请根据当前环境上 CANN 开发套件包的安装方式，配置环境变量。

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录，未指定安装目录时默认安装至 `/usr/local/Ascend` 下。

- 样例执行

  在本样例目录下执行如下命令。

  ```bash
  SCENARIO_NUM=1 ASC_ARCH=dav-2201
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=$ASC_ARCH ..
  make -j
  cd ..
  python3 scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  cd build
  ./demo
  ```

  使用 NPU 仿真模式时，添加 `-DCMAKE_ASC_RUN_MODE=sim` 参数即可。

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=$ASC_ARCH ..
  make -j
  ```

  > **注意：** 切换编译模式前需清理 cmake 缓存，可在 build 目录下执行 `rm CMakeCache.txt` 后重新 cmake。

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | 场景编号 | 1-7 | 1 |
  | `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

- 性能采集

  使用 `msprof` 工具获取详细性能数据：

  ```bash
  msprof ./demo
  ```

  采集 Case 1-3 及 Case 6-7 中 DataCopyRepeat kernel 的 L2Cache 数据时，使用 L2Cache metric：

  ```bash
  msprof --ai-core=on --aic-metrics=L2Cache ./demo
  ```

  采集后会在当前目录生成 `PROF_` 前缀目录，性能汇总文件位于 `mindstudio_profiler_output` 目录下。

  ```txt
  PROF_xxxx_XXXXXX
  ├── device_{id}
  ├── host
  ├── mindstudio_profiler_log
  └── mindstudio_profiler_output
      ├── msprof_*.json
      ├── op_summary_*.csv
      └── README.txt
  ```

  查看具体的性能分析结果：

  ```bash
  # 查看 Task Duration 以及各项数据
  cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
  ```

## 参考文档

- [SetL2CacheHint API 文档](../../../../../../docs/api/SIMD-API/基础数据结构/GlobalTensor/SetL2CacheHint.md)
- [设置合理的L2 CacheMode](../../../../../../docs/guide/算子实践参考/SIMD算子性能优化/内存访问/设置合理的L2-CacheMode.md)
- [系统缓存概述](../../../../../../docs/api/SIMD-API/基础API/缓存控制/系统缓存概述.md)
- [流式数据开启L2Cache时MTE2性能劣化根因分析](../00_vector_compute/add_high_performance/流式数据开启L2Cache时MTE2性能劣化根因分析.md)
