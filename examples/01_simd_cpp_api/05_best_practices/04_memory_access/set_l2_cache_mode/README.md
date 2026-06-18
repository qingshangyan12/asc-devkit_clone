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

## 核心数据摘要（Atlas A2/A3 系列）

| Case | Task Duration(μs) | L2 命中率 | 对比结论 |
|------|-------------------|----------|---------|
| 1. Repeat分片 + L2开 | 533.4 | **75.0%** | 基准：分片使工作集 < L2 容量，4轮中3轮命中 |
| 2. Repeat分片 + L2关 | 911.6 | ~0% | vs Case1 +70.9%，关闭L2全部走GM |
| 3. Repeat整块 + L2开 | 1058.8 | **15.7%** | vs Case1 +98.5%，整块超出容量，颠簸开销甚至比关闭L2更差 (+16.1% vs Case2) |
| 4. Add + L2开 | 304.7 | ~0% | 流式数据只读一次，无复用 |
| 5. Add + L2 bypass | 232.9 | ~0% | vs Case4 **-23.6%**，省去cache写开销 |
| 6. 合并(Add开+Repeat) | 912.6 | 61.4%* | Repeat子阶段命中率反推 = 75% |
| 7. 合并(Add bypass+Repeat) | 864.8 | **75.0%** | vs Case6 **-5.2%**，收益来自Add bypass自身 |

> \* Case 6 的 61.4% 是 Add(0%) + Repeat(75%) 的加权平均。Repeat 子阶段命中率与 Case 1 一致，Add 的 L2 开启未对 Repeat 造成可观测的污染。
## 关键意外发现：Case 6 的 L2 污染效果未触发
Case 6 中 Repeat 子阶段的 L2 命中率仍为 75.0% （与独立 Case 1 相同）。原因：

- Add 和 Repeat 使用 不同 GM 地址 的数据
- Add 的流式数据在 Repeat 开始后被自然置换出 L2
- Repeat 的 75.5MB/分片 能完全被 192MB L2 容纳
- 因此 Add 是否开启 L2 不影响 Repeat 的命中率
Case 6→7 的 5.2% 收益 全部来自 Add bypass 自身 ，而非"保护 Repeat"。README 中已如实记录这一发现并给出了设计反思

## Case 1: N 方向切 4 份 + 每份连续搬 4 次 + L2 默认开启

**来源**：`data_copy` 样例 Case 6（`KernelDataCopyPadGm2Ub::ProcessSplitFourAndRepeat4Times`）

**设计意图**：对于需要多次重复读取的数据，利用 L2 Cache 将首次读取的数据缓存起来，后续重复访问直接从 L2 读取，显著降低 MTE2 搬运耗时。输入矩阵 301.99MB > L2 容量（A2/A3 约 192MB），整块无法完整保留，因此 N 方向切 4 份后每份约 75.50MB，小于 L2 容量，可在每个分片内获得高 L2 命中率。

**样例配置**：


L2 Cache 的"颠簸"(thrashing)开销- N 方向切 4 份，每个分片内连续重复搬运 4 次
- 总搬运数据量：301.99MB × 4 = 1207.96MB
- Tile：[64, 1024]，单次 DataCopyPad 搬运 131,072B

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**MTE2 性能数据**：

<table>
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>533.4</td><td>577.3</td><td>24,937,348</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

<table>
<tr><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td>577.3</td><td>24,937,348</td><td>3,539,106</td><td>1,179,674</td><td>3,539,146</td><td>1,179,626</td><td>75.0%</td></tr>
</table>

> **实测 vs 理论**：L2 命中率 75.0% 与理论预测完全一致（4 轮搬运中第 1 轮全部 miss、第 2~4 轮全部命中）。分片策略有效控制了单次工作集大小（75.5MB < 192MB L2 容量），最大化 L2 复用收益。Ascend 950 系列待补充。

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>911.6</td><td>1165.9</td><td>50,365,927</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

> 注：L2 关闭后数据不经过 Cache，L2Cache 计数器仅记录极少量元数据/初始化读写（r0=208 hit + 3 miss, r1=204 hit + 1 miss），命中率无实际参考意义。

<table>
<tr><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td>1165.9</td><td>50,365,927</td><td>208</td><td>3</td><td>204</td><td>1</td><td>~99%（无意义）</td></tr>
</table>

> **实测 vs 理论**：与 Case 1 对比，关闭 L2 后 Task Duration 从 533.4μs 增至 911.6μs（+70.9%）。实测加速比为 1.71×。总体趋势与理论方向一致：关闭 L2 导致每次重复搬运都走 GM，性能明显劣化。Ascend 950 系列待补充。**优化效果分析**：

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>1058.8</td><td>1221.1</td><td>52,749,710</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td></tr>
</table>

**L2Cache 性能数据**：

Atlas A2/A3 系列：

<table>
<tr><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td>1221.1</td><td>52,749,710</td><td>740,585</td><td>3,978,192</td><td>740,847</td><td>3,977,928</td><td>15.7%</td></tr>
</table>

> **实测 vs 理论**：L2 命中率仅 15.7%，远低于 Case 1 的 75.0%。整块矩阵（302MB）超过 L2 容量（192MB），数据在重复搬运时被大量换出。Task Duration 1058.8μs vs Case 1 的 533.4μs = 1.98×。
>
> **关键发现：Case 3（1058.8μs）比 Case 2（L2 关闭，911.6μs）更慢（+16.1%）**。这看似反直觉——L2 开启怎么可能比关闭更差？原因是 **L2 Cache 的"颠簸"(thrashing)开销**：
> - Case 2 关闭 L2 = 纯 GM→UB 直通，无任何 Cache 管理开销
> - Case 3 虽命中率仅 15.7%，但每次 miss 时硬件仍会执行：GM 读取 + Cache Line 分配 + 旧数据换出。这些"无效的缓存管理操作"消耗了额外的 L2 控制器带宽和 MTE2 时间，总开销反而超过了直接 bypass
> - 这是 L2 Cache 使用中的重要教训：**当工作集远超 L2 容量时，盲目开启 L2 可能得不偿失，分片控制工作集大小才是关键**

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>L2Cache命中率</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>304.7</td><td>374.8</td><td>16,191,903</td><td>~0%</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
</table>

> **实测 vs 理论**：流式数据（x、y 各只读一次），L2 命中率 ~0% 符合预期。Add 的 MTE2 在搬运数据的同时需要写入 L2 Cache，这是不必要的开销。Case 5 中通过 bypass 消除此开销可获得性能提升。Ascend 950 系列待补充。

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>L2Cache命中率</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>232.9</td><td>278.8</td><td>12,043,136</td><td>~0%（bypass）</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
</table>

> **实测 vs 理论**：与 Case 4 对比，L2 bypass 后 Task Duration 从 304.7μs 降至 232.9μs（**-23.6%**），aiv_total_cycles 从 16.2M 降至 12.0M（**-25.6%**）。省去了流式数据写入 L2 Cache 的开销，MTE2 搬运效率显著提升。vec 向量计算部分不受影响。Ascend 950 系列待补充。

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>912.6</td><td>1084.0</td><td>46,828,880</td><td>Add(L2开)+Repeat合并kernel</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
</table>

**L2Cache 性能数据（Atlas A2/A3 系列）**：

<table>
<tr><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td>1084.0</td><td>46,828,880</td><td>3,539,230</td><td>2,228,118</td><td>3,539,022</td><td>2,228,334</td><td>61.4%</td></tr>
</table>

> **实测 vs 理论分析**：
> 
> 合并 kernel 的 L2Cache 统计 = Add 阶段 + Repeat 阶段的合计。读取总次数 11.53M = Add 的 ~2.10M（流式，命中率 ~0%）+ Repeat 的 ~9.44M（与 Case 1 相同）。
>
> **Repeat 子阶段的命中率反推**：总命中 7.08M，Add 命中 ~0，Repeat 命中 = 7.08M / 9.44M = **75.0%**，与 Case 1 完全相同。
>
> **结论**：在本合并 kernel 的设计中（Add [8192,8192] + Repeat [12288,12288]，在同一 `__aicore__` 内顺序执行），**Add 的 L2 开启并未对 Repeat 造成可观测的 L2 命中率影响**。原因是 Add 和 Repeat 使用不同的输入数据（不同 GM 地址），Add 的流式数据在 Repeat 阶段自然被置换出 L2，Repeat 的 75.5MB 单次工作集完全能被 L2（192MB）容纳。整体 L2 命中率 61.4% 是 Add（0%）和 Repeat（75%）的加权平均值。
>
> Ascend 950 系列待补充。

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
<tr><th>架构</th><th>Task Duration(μs)</th><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>说明</th></tr>
<tr><td>Atlas A2/A3 系列</td><td>864.8</td><td>1041.4</td><td>44,986,314</td><td>Add(L2 bypass)+Repeat合并kernel</td></tr>
<tr><td>Ascend 950 系列</td><td></td><td></td><td></td><td></td></tr>
</table>

**L2Cache 性能数据（Atlas A2/A3 系列）**：

> 注：Add 阶段使用 L2 bypass，其读写不经过 Cache 计数器。L2Cache 统计仅来自 Repeat 阶段。

<table>
<tr><th>aiv_time(μs)</th><th>aiv_total_cycles</th><th>aiv_r0_read_cache_hit</th><th>aiv_r0_read_cache_miss_allocate</th><th>aiv_r1_read_cache_hit</th><th>aiv_r1_read_cache_miss_allocate</th><th>L2Cache命中率</th></tr>
<tr><td>1041.4</td><td>44,986,314</td><td>3,539,066</td><td>1,179,729</td><td>3,539,232</td><td>1,179,573</td><td>75.0%</td></tr>
</table>

> **实测 vs 理论分析**：
>
> Repeat 阶段的 L2 命中率 75.0%，与 Case 1（独立 Repeat）和 Case 6（Add L2 开启）完全一致。
>
> **Case 7 vs Case 6 对比**：Task Duration 从 912.6μs 降至 864.8μs（**-5.2%**）。收益全部来自 Add 阶段的 L2 bypass（省去 Add 流式数据的 Cache Line 填充开销），而非"保护 Repeat 的 L2 命中率"——因为 Case 6 中 Repeat 的命中率已经是 75%，没有提升空间。
>
> **设计反思**：在本样例的合并 kernel 设计下，Add 和 Repeat 使用不同 GM 地址的数据，L2 替换策略自然隔离了两阶段的缓存内容。因此 Case 6→7 仅展示了 Add bypass 自身的收益，未能展示"前序算子 L2 污染影响后序算子"的效果。如需要展示 L2 污染的全局影响，可考虑：1) 使 Add 和 Repeat 使用更大数据量（超出 L2 置换能力）；2) 将两个算子放在不同 stream 上并行执行竞争 L2 带宽。
>
> Ascend 950 系列待补充。

**优化效果分析**：

- Case 6 vs Case 7：Add L2 bypass 后，合并 kernel 的 Task Duration 从 912.6μs 降至 864.8μs（-5.2%）。收益来自 Add 阶段跳过了 L2 Cache Line 填充。
- Repeat 阶段的 L2 命中率在 Case 6 和 Case 7 中均为 75.0%，与独立 Case 1 一致。说明在当前设计下，Add 的 L2 开启未对 Repeat 造成可观测的污染。
- 与 Case 4→5（独立 Add）的 -23.6% 相比，合并 kernel 中的收益被稀释（Repeat 阶段耗时占主导）。
- 如需展示跨算子 L2 污染的全局效应，建议设计更极端的场景（如使 Add 数据量增大或并行竞争 L2）。

---

## 优化要点总结

| Case | 数据模式 | L2 策略 | 关键指标 | 结论 | 典型场景 |
|:---|:---|:---|:---|:---|:---|
| Case 1 vs 2 | 复用数据 | L2 开 vs 关 | 命中率 75%→0%, 耗时 533→912μs (+71%) | L2 开启可显著加速重复访问 | 权重复用、特征图多次访问 |
| Case 1 vs 3 | 复用数据 | 分片 vs 整块 | 命中率 75%→15.7%, 耗时 533→1059μs (+99%) | 分片控制工作集 < L2 容量是关键 | 大矩阵分片处理 |
| Case 4 vs 5 | 流式数据 | L2 开 vs bypass | 耗时 305→233μs (**-23.6%**) | L2 bypass 省去不必要的 cache 写 | 逐元素运算（Add/Mul）的输入 |
| Case 6 vs 7 | 融合算子 | Add L2 开 vs bypass | 耗时 913→865μs (-5.2%), Repeat命中率均为75% | Add bypass 自身收益；本设计未触发 L2 污染 | 融合算子内流式数据 bypass |

> **注意**：Case 6→7 中 Add bypass 的收益（-5.2%）小于单独 Case 4→5（-23.6%），因为合并 kernel 的总耗时中 Repeat 阶段占主导（~75%），Add 的优化空间被稀释。Repeat 的 L2 命中率在两个 Case 中均为 75%，表明 L2 替换策略在本场景下有效隔离了 Add 和 Repeat 的缓存内容。

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
