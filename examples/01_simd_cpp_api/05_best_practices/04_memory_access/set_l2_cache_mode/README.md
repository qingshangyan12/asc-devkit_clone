# SetL2CacheHint 最佳实践样例

## 概述

本样例聚焦于 `SetL2CacheHint` 接口的使用，展示 L2 Cache mode 配置在不同数据访问模式下的性能影响。样例覆盖**复用数据**和**流式数据**两大类场景，通过 5 个 Case 帮助开发者理解何时应开启/关闭 L2 Cache，以及如何通过分片策略最大化 L2 Cache 的利用率。

- **Case 1-3** 抽取自 `data_copy` 样例（GM→UB 重复搬运，shape=[12288,12288]）
- **Case 4-5** 抽取自 `add_high_performance` 样例（矩阵加法 + 双缓冲，shape=[8192,8192]）
- **Case 6-7** 新增：合并 Repeat+Add 到一个 kernel 中顺序执行（shape=[12288,12288]）

**优化路径**：

```
复用数据场景（数据需多次读取）
  Case 1: N方向切4份，每份连续搬4次，L2默认开启（基准）
  Case 2: 同Case 1，但通过SetL2CacheHint关闭L2 Cache → 与Case 1对比
  Case 3: 整块重复搬4次，L2默认开启 → 与Case 1对比，展示L2容量约束

流式数据场景（数据只读一次）
  Case 4: Add + 双缓冲，L2默认开启（基准）
  Case 5: 同Case 4，但流式数据SetL2CacheHint关闭L2 Cache → 与Case 4对比

融合算子（Add + Repeat 合并到一个 kernel）
  Case 6: Add(L2开启) + Repeat(L2开启) 在同一process → 展示融合算子内L2污染
  Case 7: Add(L2 bypass) + Repeat(L2开启) 在同一process → 与Case 6对比
```

## 本样例支持的产品及CANN软件版本

| 产品 | CANN软件版本 |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 训练系列产品/Atlas A3 推理系列产品 | >= CANN 9.0.0 |
| Atlas A2 训练系列产品/Atlas A2 推理系列产品 | >= CANN 9.0.0 |

## 目录结构介绍

```
├── set_l2_cache_mode
│   ├── scripts
│   │   └── gen_data.py              // 输入数据和真值数据生成脚本文件
│   ├── CMakeLists.txt               // 编译工程文件
│   ├── set_l2_cache_mode.asc        // Ascend C 样例入口（kernel 调用 + main 函数）
│   ├── set_l2_cache_mode.h          // Kernel 实现（DataCopyRepeat + Add）
│   ├── data_utils.h                 // 数据读入写出函数
│   └── README.md                    // 样例说明文档
```

## 样例描述

### 数据复用场景（Case 1-3）

本组场景输入为 half 类型二维矩阵，shape 为 [12288, 12288]。使用 `DataCopyPad` 将数据从 GM 搬运到 UB，通过对比不同的搬运策略和 L2 Cache 配置，观察 L2 命中和 MTE2 性能的差异。本组场景不包含计算逻辑，仅执行数据搬运操作。

### 流式数据场景（Case 4-5）

本组场景实现两个 shape 为 [8192, 8192] 的 half 类型矩阵相加（z = x + y），采用双缓冲（Ping-Pong）技术实现数据搬运与向量计算的流水线并行。

## 核心特性总览

<table>
<tr><th>分组</th><th>Case</th><th>来源</th><th>主要观察对象</th><th>对比方法</th></tr>
<tr><td rowspan="3">复用数据</td><td>Case 1</td><td>data_copy Case 6</td><td>分片重复 + L2开启（基线）</td><td>N方向切4份，L2默认开启</td></tr>
<tr><td>Case 2</td><td>data_copy Case 6 变体</td><td>L2关闭对复用数据的劣化</td><td>与Case 1对比：SetL2CacheHint(DISABLE)</td></tr>
<tr><td>Case 3</td><td>data_copy Case 5</td><td>L2容量约束：整块 vs 分片</td><td>与Case 1对比：整块重复 vs 分片重复</td></tr>
<tr><td rowspan="2">流式数据</td><td>Case 4</td><td>add_high_perf Case 4</td><td>流式数据 + L2开启（基线）</td><td>Add双缓冲，L2默认开启</td></tr>
<tr><td>Case 5</td><td>add_high_perf Case 5</td><td>L2 bypass对流式数据的收益</td><td>与Case 4对比：SetL2CacheHint(DISABLE)</td></tr>
<tr><td rowspan="2">融合算子</td><td>Case 6</td><td>新增</td><td>Add(L2开)+Repeat在同一kernel内</td><td>展示融合算子内L2污染</td></tr>
<tr><td>Case 7</td><td>新增</td><td>Add(L2 bypass)+Repeat在同一kernel内</td><td>与Case 6对比：Add改为L2 bypass</td></tr>
</table>

## 性能指标说明

本章节性能数据在 Atlas A2/A3 系列和 Ascend 950 系列产品上采集。为便于表格展示，下文将 Atlas A2 训练系列产品/Atlas A2 推理系列产品、Atlas A3 训练系列产品/Atlas A3 推理系列产品简称为 Atlas A2/A3 系列，将 Ascend 950PR/Ascend 950DT 简称为 Ascend 950 系列。

### MTE2 性能表

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准。</td></tr>
<tr><td>*_total_cycles</td><td>Task 执行总 cycle 数。</td></tr>
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
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间。</td></tr>
<tr><td>*_total_cycles</td><td>Task 执行总 cycle 数。</td></tr>
<tr><td>*_write_cache_hit</td><td>写 Cache 命中的次数。</td></tr>
<tr><td>*_write_cache_miss_allocate</td><td>写 Cache 缺失后重新分配缓存的次数。</td></tr>
<tr><td>*_r*_read_cache_hit</td><td>读 r* 通道 Cache 命中次数。分析总读命中次数时需将 r0/r1 两个通道累加。</td></tr>
<tr><td>*_r*_read_cache_miss_allocate</td><td>读 r* 通道 Cache 缺失后重新分配的次数。</td></tr>
<tr><td>*_read_local_l2_hit</td><td>读 Cache 命中的次数（Ascend 950 系列）。</td></tr>
<tr><td>*_read_local_l2_miss</td><td>读 Cache 缺失次数（Ascend 950 系列）。</td></tr>
<tr><td>*_read_local_l2_victim</td><td>读 Cache 未命中并触发 Cache 中数据被换出的次数（Ascend 950 系列）。</td></tr>
</table>

> **说明**：Atlas A2/A3 系列和 Ascend 950 系列的 profiler 输出字段不同：
> Atlas A2/A3: `l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)`
> Ascend 950: `l2cache_hit_ratio = hit / (hit + miss + victim)`

---

## Case 1: N 方向切 4 份 + 每份连续搬 4 次 + L2 默认开启

**来源**：`data_copy` 样例 Case 6（`KernelDataCopyPadGm2Ub::ProcessSplitFourAndRepeat4Times`）

**设计意图**：对于需要多次重复读取的数据，利用 L2 Cache 将首次读取的数据缓存起来，后续重复访问直接从 L2 读取，显著降低 MTE2 搬运耗时。输入矩阵 301.99MB > L2 容量（A2/A3 约 192MB），整块无法完整保留，因此 N 方向切 4 份后每份约 75.50MB，小于 L2 容量，可在每个分片内获得高 L2 命中率。

**样例配置**：

- 矩阵：[12288, 12288]，half 类型
- N 方向切 4 份，每个分片内连续重复搬运 4 次
- 总搬运数据量：301.99MB × 4 = 1207.96MB
- Tile：[64, 1024]，单次 DataCopyPad 搬运 131,072B

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
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

## Case 2: N 方向切 4 份 + 每份连续搬 4 次 + L2 Cache 关闭

**来源**：`data_copy` 样例 Case 6 变体（`KernelDataCopyPadGm2Ub::ProcessSplitFourAndRepeat4TimesL2Disable`）

**设计意图**：与 Case 1 搬运模式完全相同，唯一区别是调用 `SetL2CacheHint(CACHE_MODE_DISABLE)` 关闭 L2 Cache。证明对于需要复用的数据，关闭 L2 Cache 会导致数据无法被缓存，后续重复访问每次都需要从 GM 重新读取，搬运耗时显著增加。

**关键代码**：

```cpp
// 关闭 L2 Cache，DataCopy 直接从 GM 搬运到 UB，不写入 L2
srcGlobal.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
ProcessSplitFourAndRepeat4Times(); // 搬运逻辑与 Case 1 完全相同
```

**L2 策略**：`SetL2CacheHint(CACHE_MODE_DISABLE)`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
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
- 说明对于需要多次重复读取的数据，应保持 L2 Cache 默认开启（`CACHE_MODE_NORMAL`）

> **性能优化建议**：对于需要重复访问的数据，应保持 L2 Cache 默认开启。L2 Cache 可以缓存首次读取的数据，后续重复访问时直接从 L2 读取。

---

## Case 3: 整块连续重复搬运 4 次 + L2 默认开启

**来源**：`data_copy` 样例 Case 5（`KernelDataCopyPadGm2Ub::ProcessRepeatWhole4Times`）

**设计意图**：与 Case 1 搬运总数据量相同（均为 1207.96MB），但整块矩阵（301.99MB）大于 L2 Cache 容量，每次完整矩阵搬运都会导致旧数据被换出，L2 命中率极低。证明即使 L2 Cache 开启，如果单次工作集超出 L2 容量，也必须通过分片策略来控制工作集大小。

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
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

- 与 Case 1 对比，Case 3 虽然 L2 Cache 同样开启，但整块工作集超出 L2 容量，缓存换出严重
- 说明 L2 Cache 的分片策略至关重要：应通过分片将单次工作集控制在 L2 容量以内

> **性能优化建议**：即使 L2 Cache 开启，如果单次工作集超出 L2 容量，也无法获得复用收益。应通过分片策略，将重复访问限制在更小的数据范围内。

---

## Case 4: Add 双缓冲 + L2 Cache 默认开启

**来源**：`add_high_performance` 样例 Case 4（`KernelAdd::ProcessDoubleBuffer`）

**设计意图**：Add 算子的输入 x、y 均为流式数据（每个元素只读一次），采用双缓冲流水线并行。L2 Cache 默认开启时，MTE2 将数据搬运到 UB 的同时会写入 L2 Cache，但数据不会被再次访问，写入 L2 的操作是纯浪费的，额外消耗了 L2 控制器带宽。

**样例配置**：

- 矩阵：[8192, 8192]，half 类型（128MB 每矩阵）
- M 方向均匀切分到 AIV_CORE_NUM 个核
- dataCopyLen：A2/A3 为 16384，950 为 21760

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>aiv_mte3_time(μs)</th><th>aiv_mte3_ratio</th><th>aiv_vec_time(μs)</th><th>aiv_vec_ratio</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

---

## Case 5: Add 双缓冲 + L2 Cache Bypass

**来源**：`add_high_performance` 样例 Case 5（`KernelAdd::ProcessDoubleBufferL2Bypass`）

**设计意图**：与 Case 4 双缓冲流水线完全相同，唯一区别是对输入 x、y 调用 `SetL2CacheHint(CACHE_MODE_DISABLE)`。对于只读一次的流式数据，禁用 L2 Cache 后 MTE2 直接将数据从 GM 搬运到 UB，省去了不必要的 cache 写开销，并可避免流式数据占据 L2 空间。

**关键代码**：

```cpp
// 对只读取一次的流式数据设置 L2 Cache bypass
xGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
yGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
ProcessDoubleBuffer(); // 其余逻辑与 Case 4 完全相同
```

**L2 策略**：x、y 输入 `SetL2CacheHint(CACHE_MODE_DISABLE)`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>aiv_mte3_time(μs)</th><th>aiv_mte3_ratio</th><th>aiv_vec_time(μs)</th><th>aiv_vec_ratio</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td></td><td></td><td></td><td></td></tr>
</table>

**优化效果分析**：

- 与 Case 4 对比，关闭流式数据的 L2 Cache 后，MTE2 省去了不必要的 cache 写操作

> **性能优化建议**：对于数据量较大且只读取一次的流式数据（如 Add 算子的输入），建议设置 `SetL2CacheHint(CACHE_MODE_DISABLE)`，省去不必要的 cache 写开销和 L2 空间占用。

---

## Case 6: 合并 Repeat + Add Kernel（Add L2 开启）

**来源**：新增

**设计意图**：将 Case 1 的分片重复搬运和 Case 4 的双缓冲 Add 合并到同一个 `__aicore__` 的 Process 函数中。Add 矩阵为 [8192, 8192]，Repeat 矩阵为 [12288, 12288]。Phase 1 执行 Add（L2 默认开启），流式数据写入 L2 Cache；Phase 2 执行 Repeat（分片重复 4 次）。由于 Add 的流式数据占据了 L2 空间，Repeat 阶段的 L2 命中率预期低于 Case 1（单独运行时）。

**样例配置**：

- Add 矩阵：[8192, 8192]，half 类型（与 Case 4/5 一致）
- Repeat 矩阵：[12288, 12288]，half 类型（与 Case 1 一致）
- M 方向均匀切分到 AIV_CORE_NUM 个核（两个阶段独立切分）
- dataCopyLen：A2/A3 为 16384，950 为 21840
- Repeat tile：[64, 1024]

**L2 策略**：

- Phase 1 (Add): L2 默认开启 → 流式数据写入 L2
- Phase 2 (Repeat): L2 默认开启 → 受 Phase 1 的流式数据污染

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>Add(L2开)+Repeat合并</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>Add(L2开)+Repeat合并</td></tr>
</table>

---

## Case 7: 合并 Repeat + Add Kernel（Add L2 Bypass）

**来源**：新增

**设计意图**：与 Case 6 结构相同，唯一区别是 Phase 1 的 Add 使用 L2 bypass（`SetL2CacheHint(CACHE_MODE_DISABLE)`）。流式数据不写入 L2 Cache，Phase 2 的 Repeat 阶段 L2 空间干净，命中率与 Case 1（单独运行时）接近。

**关键代码**：

```cpp
// Phase 1: Add（L2 bypass）
if constexpr (addL2Bypass) {
    addXGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
    addYGm.SetL2CacheHint(AscendC::CacheMode::CACHE_MODE_DISABLE);
}
// ... 双缓冲 Add ...
// Phase 2: Repeat（分片重复 4 次，L2 默认开启）
```

**L2 策略**：

- Phase 1 (Add): L2 bypass → 不污染 L2
- Phase 2 (Repeat): L2 默认开启 → L2 空间干净，命中率理想

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_total_cycles</th><th>aiv_mte2_time(μs)</th><th>aiv_mte2_ratio</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td></td><td></td><td></td><td></td><td>Add(L2 bypass)+Repeat合并</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td><td>Add(L2 bypass)+Repeat合并</td></tr>
</table>

**优化效果分析**：

- 与 Case 6 对比，Case 7 的 Add 使用 L2 bypass，避免了流式数据对 L2 的污染
- Phase 2（Repeat）的 L2 命中率与 Case 1（独立运行）接近
- 说明在融合算子中，前序流式计算阶段的 L2 bypass 同样能保护后续内存密集阶段的 L2 命中率
- Case 6-7 与 Case 4-5 形成了"单算子独立运行 → 融合算子连续执行"的完整认知链条

---

## 优化要点总结

| Case | 数据模式 | L2 策略 | 结论 | 典型场景 |
|:---|:---|:---|:---|:---|
| Case 1 vs 2 | 复用数据 | L2 开 vs 关 | L2 开启可显著加速重复访问 | 权重复用、特征图多次访问 |
| Case 1 vs 3 | 复用数据 | 分片 vs 整块 | 分片使工作集小于 L2 容量，最大化命中率 | 大矩阵分片处理 |
| Case 4 vs 5 | 流式数据 | L2 开 vs bypass | Bypass 省去不必要的 cache 写开销 | 逐元素运算（Add/Mul）的输入 |
| Case 6 vs 7 | 融合算子 | Add L2 开 vs bypass | 融合算子内 L2 bypass 保护后续内存密集阶段 | 多头注意力、融合算子 |

**L2 Cache Mode 决策树**：

```text
该数据会被多次读取吗？
  ├── 是 → 使用 CACHE_MODE_NORMAL（默认）
  │         单次工作集 > L2 容量？
  │         └── 是 → 先分片，再在每个分片内使用 CACHE_MODE_NORMAL
  └── 否 → 使用 CACHE_MODE_DISABLE（流式数据 bypass）
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **说明：** `${install_path}` 为 CANN 包安装目录。

- 样例执行

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

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | 场景编号 | 1-7 | 1 |
  | `CMAKE_ASC_RUN_MODE` | 运行模式 | `npu`、`sim` | `npu` |
  | `CMAKE_ASC_ARCHITECTURES` | NPU 硬件架构 | `dav-2201`、`dav-3510` | `dav-2201` |

- 性能采集

  ```bash
  # 普通性能采集
  msprof ./demo

  # Case 1-3 的 L2Cache 数据采集
  msprof --ai-core=on --aic-metrics=L2Cache ./demo

  # 查看性能汇总
  cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
  ```

## 参考文档

- [SetL2CacheHint API 文档](../../../../../docs/api/SIMD-API/基础数据结构/GlobalTensor/SetL2CacheHint.md)
- [DataCopy 最佳实践样例](../data_copy/README.md)
- [Add 性能调优样例](../../00_vector_compute/add_high_performance/README.md)
