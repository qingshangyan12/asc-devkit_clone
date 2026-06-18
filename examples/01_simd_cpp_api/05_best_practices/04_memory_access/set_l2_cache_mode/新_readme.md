# SetL2CacheHint 最佳实践样例

## 概述

本样例聚焦于 `SetL2CacheHint` 接口的使用，展示 L2 Cache mode 配置在不同数据访问模式下的性能影响。样例覆盖**复用数据**、**流式数据**和**融合算子**三大类场景，通过 7 个 Case 帮助开发者理解何时应开启/关闭 L2 Cache，以及如何通过分片策略最大化 L2 Cache 的利用率。

- **Case 1-3** 抽取自 `data_copy` 样例（GM→UB 重复搬运，shape=[12288,12288]）
- **Case 4-5** 抽取自 `add_high_performance` 样例（矩阵加法 + 双缓冲，shape=[8192,8192]）
- **Case 6-7** 新增：合并 Repeat+Add 到一个 kernel 中顺序执行

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
  Case 6: Add(L2开启) + Repeat(L2开启) 在同一process
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

本组场景输入为 half 类型二维矩阵，shape 为 [12288, 12288]，单矩阵数据量：

$$单矩阵数据量 = 12288 \times 12288 \times 2B = 301,989,888 B \approx 301.99 MB$$

使用 `DataCopyPad` 将数据从 GM 搬运到 UB，通过对比不同的搬运策略和 L2 Cache 配置，观察 L2 命中和 MTE2 性能的差异。本组场景不包含计算逻辑，仅执行数据搬运操作。

单次 `DataCopyPad` 搬运的源数据量按 `tileM × tileN × sizeof(half)` 计算：

$$单次搬运数据量 = 64 \times 1024 \times 2B = 131,072 B$$

### 流式数据场景（Case 4-5）

本组场景实现两个 shape 为 [8192, 8192] 的 half 类型矩阵相加（z = x + y），采用双缓冲（Ping-Pong）技术实现数据搬运与向量计算的流水线并行。

### 融合算子场景（Case 6-7）

将 Add（[8192, 8192]）和 DataCopyRepeat（[12288, 12288]）合并到同一个 kernel 的 Process 函数中，先执行 Add 再执行 Repeat。通过对比 Add 的 L2 开启 vs bypass，观察融合算子内 L2 配置的影响。

## 核心特性总览

<table>
<tr><th>分组</th><th>Case</th><th>来源</th><th>主要观察对象</th><th>对比方法</th></tr>
<tr><td rowspan="3">复用数据</td><td>Case 1</td><td>data_copy Case 6</td><td>分片重复 + L2开启（基线）</td><td>N方向切4份，L2默认开启</td></tr>
<tr><td>Case 2</td><td>data_copy Case 6 变体</td><td>L2关闭对复用数据的劣化</td><td>与Case 1对比：SetL2CacheHint(DISABLE)</td></tr>
<tr><td>Case 3</td><td>data_copy Case 5</td><td>L2容量约束：整块 vs 分片</td><td>与Case 1对比：整块重复 vs 分片重复</td></tr>
<tr><td rowspan="2">流式数据</td><td>Case 4</td><td>add_high_perf Case 4</td><td>流式数据 + L2开启（基线）</td><td>Add双缓冲，L2默认开启</td></tr>
<tr><td>Case 5</td><td>add_high_perf Case 5</td><td>L2 bypass对流式数据的收益</td><td>与Case 4对比：SetL2CacheHint(DISABLE)</td></tr>
<tr><td rowspan="2">融合算子</td><td>Case 6</td><td>新增</td><td>Add(L2开)+Repeat在同一kernel内</td><td>展示融合算子内L2配置的影响</td></tr>
<tr><td>Case 7</td><td>新增</td><td>Add(L2 bypass)+Repeat在同一kernel内</td><td>与Case 6对比：Add改为L2 bypass</td></tr>
</table>

---

## 性能指标说明

### 关于两轮 Profiling

性能数据通过两轮独立采集获得：

| 轮次 | 命令 | 用途 | 数据来源 |
|------|------|------|---------|
| 第一轮 | `msprof --ai-core=on --aic-metrics=L2Cache ./demo` | 采集 L2 Cache 命中率 | `profiling_result_L2Cache/` |
| 第二轮 | `msprof --ai-core=on ./demo` | 采集准确的执行耗时和流水占比 | `profiling_result_without_L2Cache/` |

> **注意**：`--aic-metrics=L2Cache` 参数在某些场景下（如频繁 Cache 换出）会引入显著的 profiler 开销，导致执行时间偏高。因此本文中的 **Task Duration、MTE2/MTE3/vec 时间均来自第二轮采集**，**L2 命中率来自第一轮采集**。

### PipeUtilization 性能指标

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>Task Duration(μs)</td><td>整个任务执行的总时间，算子执行时间以该参数为准。</td></tr>
<tr><td>aiv_time(μs)</td><td>Task 在 Vector AI Core 上的执行时间。</td></tr>
<tr><td>aiv_mte2_time(μs)</td><td>MTE2 类型指令（GM → AI Core 搬运类指令）耗时。</td></tr>
<tr><td>aiv_mte2_ratio</td><td>MTE2 类型指令的 cycle 数在 total cycle 数中的占比。</td></tr>
<tr><td>aiv_mte3_time(μs)</td><td>MTE3 类型指令（UB → GM 搬运类指令）耗时。</td></tr>
<tr><td>aiv_mte3_ratio</td><td>MTE3 类型指令的 cycle 数在 total cycle 数中的占比。</td></tr>
<tr><td>aiv_vec_time(μs)</td><td>vec 类型指令（向量类运算指令）耗时。</td></tr>
<tr><td>aiv_vec_ratio</td><td>vec 类型指令的 cycle 数在 total cycle 数中的占比。</td></tr>
</table>

### L2Cache 性能指标

<table>
<tr><th>字段名</th><th>字段含义</th></tr>
<tr><td>aiv_r0_read_cache_hit</td><td>读 r0 通道 Cache 命中次数。</td></tr>
<tr><td>aiv_r0_read_cache_miss_allocate</td><td>读 r0 通道 Cache 缺失后重新分配的次数。</td></tr>
<tr><td>aiv_r1_read_cache_hit</td><td>读 r1 通道 Cache 命中次数。</td></tr>
<tr><td>aiv_r1_read_cache_miss_allocate</td><td>读 r1 通道 Cache 缺失后重新分配的次数。</td></tr>
</table>

> **L2Cache 命中率公式**（Atlas A2/A3 系列）：
> `l2cache_hit_ratio = (r0_hit + r1_hit) / (r0_hit + r1_hit + r0_miss_allocate + r1_miss_allocate)`

---

## 核心数据摘要（Atlas A2/A3 系列）

| Case | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | aiv_vec_time(μs) | aiv_vec_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|-------------------|----------------|-------------------|---------------|----------|
| 1. Repeat分片 + L2开 | 538.2 | 575.7 | 98.0% | — | — | — | — | **75.0%** |
| 2. Repeat分片 + L2关 | 1056.4 | 1208.5 | 99.2% | — | — | — | — | ~0% |
| 3. Repeat整块 + L2开 | 805.3 | 956.5 | 99.1% | — | — | — | — | **15.7%** |
| 4. Add + L2开 | 304.7 | 363.8 | 97.1% | 117.4 | 31.3% | 26.4 | 7.0% | ~0% |
| 5. Add + L2 bypass | 229.3 | 261.2 | 95.4% | 169.6 | 61.9% | 26.4 | 9.6% | ~0% |
| 6. 合并(Add开+Repeat) | 909.3 | 1072.5 | 99.1% | 104.0 | 9.6% | 26.4 | 2.4% | **61.4%*** |
| 7. 合并(Add bypass+Repeat) | 850.1 | 991.8 | 98.9% | 174.9 | 17.4% | 26.4 | 2.6% | **75.0%** |

> \* Case 6 的 61.4% 是 Add 阶段（~0%）+ Repeat 阶段（75%）的加权平均。Repeat 子阶段命中率与 Case 1 一致。

---

## Case 1: N 方向切 4 份 + 每份连续搬 4 次 + L2 默认开启

**来源**：`data_copy` 样例 Case 6

**设计意图**：对于需要多次重复读取的数据，利用 L2 Cache 加速第 2~4 轮读取。N 方向切 4 份后每份约 75.5MB，小于 A2/A3 L2 容量（192MB）。

**样例配置**：

- 矩阵：[12288, 12288]，half 类型，单矩阵 301.99MB
- N 方向切 4 份，每个分片内连续重复搬运 4 次
- 总搬运数据量：301.99MB × 4 = 1207.96MB
- Tile：[64, 1024]

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|----------|
| Atlas A2/A3 系列 | 538.2 | 575.7 | 98.0% | 75.0% |
| Ascend 950 系列 | — | — | — | — |

> **实测 vs 理论**：L2 命中率 75.0% 与理论预测完全一致（4 轮中第 1 轮全部 miss、第 2~4 轮全部命中）。分片策略有效控制了工作集大小。纯搬运场景 MTE2 占比 98.0% 符合预期。

---

## Case 2: N 方向切 4 份 + 每份连续搬 4 次 + L2 Cache 关闭

**来源**：`data_copy` 样例 Case 6 变体

**设计意图**：与 Case 1 搬运模式完全相同，唯一区别是 `SetL2CacheHint(CACHE_MODE_DISABLE)`。证明对于复用数据，关闭 L2 后每次都要从 GM 重新读取。

**L2 策略**：`SetL2CacheHint(CACHE_MODE_DISABLE)`

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|----------|
| Atlas A2/A3 系列 | 1056.4 | 1208.5 | 99.2% | ~0% |
| Ascend 950 系列 | — | — | — | — |

> **实测 vs 理论**：关闭 L2 后 Task Duration 从 538.2μs 增至 1056.4μs（**+96.3%**）。实测加速比 1.96×，理论预期为 4/1.75 = 2.29×，略低于理论值因 GM 带宽在实际搬运中更稳定。总体上**明确证明复用数据必须保持 L2 开启**。

> **性能优化建议**：对于需要重复访问的数据，保持 L2 Cache 默认开启（`CACHE_MODE_NORMAL`）。

---

## Case 3: 整块连续重复搬运 4 次 + L2 默认开启

**来源**：`data_copy` 样例 Case 5

**设计意图**：与 Case 1 搬运总量相同，但整块矩阵（301.99MB）超过 L2 容量（192MB），缓存换出严重。证明即使 L2 开启，也必须通过分片控制工作集大小。

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|----------|
| Atlas A2/A3 系列 | 805.3 | 956.5 | 99.1% | 15.7% |
| Ascend 950 系列 | — | — | — | — |

> **实测 vs 理论**：L2 命中率仅 15.7%，远低于 Case 1 的 75.0%。整块矩阵超出容量，数据被大量换出。
>
> **三项策略性能排序**：
>
> | 策略 | TD(μs) | 排名 |
> |------|---------|------|
> | Case 1: 分片 + L2 开启 | 538.2 | 🥇 最优 |
> | Case 3: 整块 + L2 开启 | 805.3 (+49.6%) | 🥈 |
> | Case 2: 分片 + L2 关闭 | 1056.4 (+96.3%) | 🥉 最差 |
>
> **核心结论**：即使命中率仅 15.7%，L2 开启（Case 3, 805μs）仍显著优于完全关闭（Case 2, 1056μs），但远不如分片策略（Case 1, 538μs）。**分片 > 整块 > 关闭**。

> **性能优化建议**：分片是最大化 L2 命中率的关键手段。当无法分片时，保持 L2 开启仍优于完全关闭。

---

## Case 4: Add 双缓冲 + L2 Cache 默认开启

**来源**：`add_high_performance` 样例 Case 4

**设计意图**：Add 输入 x、y 为流式数据（只读一次）。L2 默认开启时，MTE2 搬运同时写入 L2 Cache，但数据不会被再次访问，这是不必要的开销。

**样例配置**：

- 矩阵：[8192, 8192]，half 类型（128MB 每矩阵）
- M 方向均匀切分到 AIV_CORE_NUM 个核

**L2 策略**：默认开启 `CACHE_MODE_NORMAL`

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | aiv_vec_time(μs) | aiv_vec_ratio |
|------|-------------------|-------------------|----------------|-------------------|----------------|-------------------|---------------|
| Atlas A2/A3 系列 | 304.7 | 363.8 | 97.1% | 117.4 | 31.3% | 26.4 | 7.0% |
| Ascend 950 系列 | — | — | — | — | — | — | — |

> **实测 vs 理论**：流式数据命中率 ~0% 符合预期。MTE2+MTE3 占比 > 100%，说明 Ping-Pong 双缓冲有效实现了搬运与写回的流水重叠。vec 计算时间 26.4μs 稳定。

---

## Case 5: Add 双缓冲 + L2 Cache Bypass

**来源**：`add_high_performance` 样例 Case 5

**设计意图**：对输入 x、y 调用 `SetL2CacheHint(CACHE_MODE_DISABLE)`，省去流式数据的 L2 写开销。

**L2 策略**：x、y 输入 `SetL2CacheHint(CACHE_MODE_DISABLE)`

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | aiv_vec_time(μs) | aiv_vec_ratio |
|------|-------------------|-------------------|----------------|-------------------|----------------|-------------------|---------------|
| Atlas A2/A3 系列 | 229.3 | 261.2 | 95.4% | 169.6 | 61.9% | 26.4 | 9.6% |
| Ascend 950 系列 | — | — | — | — | — | — | — |

> **实测 vs 理论**：与 Case 4 对比：
> - Task Duration: 304.7 → 229.3μs（**-24.8%**）
> - MTE2 耗时: 363.8 → 261.2μs（**-28.2%**）
> - vec 时间不变（~26.4μs）：向量计算与 L2 无关
>
> L2 bypass 省去了流式数据写入 L2 Cache 的开销，效果显著。

> **性能优化建议**：对只读一次的流式数据，使用 `SetL2CacheHint(CACHE_MODE_DISABLE)` 省去 cache 写开销。

---

## Case 6: 合并 Repeat + Add Kernel（Add L2 开启）

**来源**：新增

**设计意图**：将 Case 1 的分片重复搬运和 Case 4 的双缓冲 Add 合并到同一个 kernel 中。Add 矩阵 [8192, 8192]，Repeat 矩阵 [12288, 12288]。Phase 1 执行 Add（L2 默认开启），Phase 2 执行 Repeat（分片重复 4 次）。

**L2 策略**：Add 默认 L2 开启 + Repeat 默认 L2 开启

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | aiv_vec_time(μs) | aiv_vec_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|-------------------|----------------|-------------------|---------------|----------|
| Atlas A2/A3 系列 | 909.3 | 1072.5 | 99.1% | 104.0 | 9.6% | 26.4 | 2.4% | 61.4%* |
| Ascend 950 系列 | — | — | — | — | — | — | — | — |

> \* 整体 L2 命中率 61.4% = Add 阶段（~0%）+ Repeat 阶段（~75%）的加权平均。
>
> **实测 vs 理论**：Repeat 子阶段命中率与独立 Case 1 的 75% 一致。Add 和 Repeat 使用不同 GM 地址的数据，L2 替换策略自然隔离了两阶段的缓存内容，**Add 的 L2 开启未对 Repeat 造成可观测的污染**。

---

## Case 7: 合并 Repeat + Add Kernel（Add L2 Bypass）

**来源**：新增

**设计意图**：与 Case 6 结构相同，唯一区别是 Add 使用 L2 bypass。展示 bypass 在融合算子中的收益。

**L2 策略**：Add L2 bypass + Repeat L2 开启

**性能数据**：

| 架构 | Task Duration(μs) | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio | aiv_vec_time(μs) | aiv_vec_ratio | L2 命中率 |
|------|-------------------|-------------------|----------------|-------------------|----------------|-------------------|---------------|----------|
| Atlas A2/A3 系列 | 850.1 | 991.8 | 98.9% | 174.9 | 17.4% | 26.4 | 2.6% | 75.0%* |
| Ascend 950 系列 | — | — | — | — | — | — | — | — |

> \* Repeat 阶段 L2 命中率 75.0%，与 Case 1 和 Case 6 完全一致。
>
> **实测 vs 理论**：与 Case 6 对比：
> - Task Duration: 909.3 → 850.1μs（**-6.5%**）
> - 收益来自 Add bypass 省去的 L2 Cache Line 填充开销
> - Repeat 的 L2 命中率未受影响（Case 6 已是 75%，无提升空间）
> - 合并 kernel 中 bypass 收益（-6.5%）小于独立 Add 场景（Case 4→5 的 -24.8%），因 Repeat 阶段耗时占主导（~75%），稀释了 Add 优化的贡献
>
> **设计反思**：在本样例的合并 kernel 设计下，Add 和 Repeat 使用不同 GM 地址，L2 替换策略自然隔离了两阶段缓存。如需展示跨算子 L2 污染的全局效应，建议设计更极端的场景（如使 Add 数据量增大或两个算子并行竞争 L2）。

---

## 优化要点总结

| 对比 | 数据模式 | L2 策略 | 关键指标 | 结论 | 典型场景 |
|:---|:---|:---|:---|:---|:---|
| Case 1 vs 2 | 复用数据 | L2 开 vs 关 | 538→1056μs (+96%) | L2 开启可显著加速重复访问 | 权重复用、特征图多次访问 |
| Case 1 vs 3 | 复用数据 | 分片 vs 整块 | 538→805μs (+50%); 命中率 75%→16% | 分片控制工作集 < L2 容量是关键 | 大矩阵分片处理 |
| Case 2 vs 3 | 复用数据 | L2 关 vs 整块L2开 | 1056 vs 805μs | L2 开启即使命中率低，仍优于关闭 | 无法分片时的次优选择 |
| Case 4 vs 5 | 流式数据 | L2 开 vs bypass | 305→229μs (**-24.8%**) | L2 bypass 省去不必要的 cache 写 | 逐元素运算的输入 |
| Case 6 vs 7 | 融合算子 | Add L2 开 vs bypass | 909→850μs (-6.5%) | Add bypass 自身收益；Repeat 命中率不受影响 | 融合算子内流式数据 bypass |

**L2 Cache Mode 决策树**：

```text
该数据会被多次读取吗？
  ├── 是 → 使用 CACHE_MODE_NORMAL（默认）
  │         单次工作集 > L2 容量？
  │         ├── 是 → 先分片，在每个分片内使用 CACHE_MODE_NORMAL
  │         └── 否 → 直接使用 CACHE_MODE_NORMAL
  └── 否 → 使用 CACHE_MODE_DISABLE（流式数据 bypass）
```

## 编译运行

在本样例根目录下执行如下步骤，编译并执行样例。

- 配置环境变量

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

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

- 性能采集

  ```bash
  # 第一轮：采集 L2 Cache 命中率
  msprof --ai-core=on --aic-metrics=L2Cache ./demo

  # 第二轮：采集准确执行时间
  msprof --ai-core=on ./demo
  ```

- 编译选项说明

  | 参数 | 说明 | 可选值 | 默认值 |
  |------|------|--------|--------|
  | `CMAKE_ASC_ARCHITECTURES` | NPU 架构 | `dav-2201`、`dav-3510` | `dav-2201` |
  | `SCENARIO_NUM` | Case 编号 | 1-7 | 1 |
