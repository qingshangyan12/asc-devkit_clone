# gridDim 配置样例 msprof op 性能汇总

- 采集命令：`msprof op --application=./grid_config`
- 采集时间：2026-05-28
- 目标平台：Ascend 950PR / 950DT（AIV 物理核数 = 64）
- 采集场景：`SCENARIO_NUM = 1..6`
- 数据规模（已更新）：`input[1100000]`, `index/output[1024,1024]`
  - `INDEX_WIDTH = 1024`
  - `OUTPUT_TOTAL_LENGTH = 1024 * 1024 = 1,048,576`
  - `input_total_length = 1,100,000`

## Task Duration 汇总

数据来源：`scenario_<N>/OPPROF_*/OpBasicInfo.csv`

| Scenario | Kernel              | Block Dim | Task Duration (μs) | 说明                                  |
| -------- | ------------------- | --------- | ------------------ | ------------------------------------- |
| 1        | empty_kernel        | 36        | 1.667              | 调度基线，核数 < 物理核 (36 < 64)     |
| 2        | empty_kernel        | 64        | 2.257              | 调度基线，核数 = 物理核               |
| 3        | empty_kernel        | 128       | 3.408              | 调度基线，核数 > 物理核 (2× 物理核)   |
| 4        | empty_kernel        | 1024      | 16.391             | 调度基线，核数 ≫ 物理核 (16× 物理核)  |
| 5        | gather_adds_kernel  | 1024      | 54.087             | 实际计算，核数 ≫ 物理核               |
| 6        | gather_adds_kernel  | 64        | **34.357**         | 实际计算，核数 = 物理核（最优配置）   |

## Pipe / 内存指标汇总（按核取平均）

数据来源：`scenario_<N>/OPPROF_*/PipeUtilization.csv`

| Scenario | aiv_time 平均 (μs) | vec_ratio | scalar_ratio | mte2_ratio | mte2_bw (GB/s) | mte3_ratio |
| -------- | ------------------ | --------- | ------------ | ---------- | -------------- | ---------- |
| 1        | 0.391              | 0.0000    | 0.9352       | 0.0104     | 32.42          | 0.003401   |
| 2        | 0.463              | 0.0000    | 0.9418       | 0.0093     | 32.58          | 0.003058   |
| 3        | 0.309              | 0.0000    | 0.8992       | 0.0159     | 32.78          | 0.005303   |
| 4        | 0.167              | 0.0000    | 0.8456       | 0.0244     | 32.77          | 0.008128   |
| 5        | 2.697              | 0.9423    | 0.0532       | 0.0014     | 33.81          | 0.000457   |
| 6        | 32.852             | 0.9861    | 0.0135       | 0.0001     | 218.21         | 0.000037   |

> 说明：Case 1-4 的 kernel 体为空，aiv 时间几乎全部花在 scalar / 启动开销上；Case 5/6 的 vector pipe 占比 ≥94%，已经是计算密集场景。

## 关键观察

### 1. 调度开销随逻辑核数增长（Case 1 → 4，空壳核函数）

| 对比         | Block Dim 倍数  | Task Duration 倍数        |
| ------------ | --------------- | ------------------------- |
| Case 1 → 2   | 36 → 64 (1.78×) | 1.667 → 2.257 (1.35×)     |
| Case 2 → 3   | 64 → 128 (2×)   | 2.257 → 3.408 (1.51×)     |
| Case 3 → 4   | 128 → 1024 (8×) | 3.408 → 16.391 (4.81×)    |

逻辑核数在物理核以内时 Task Duration 涨幅平缓（线性以下）；一旦显著超出物理核（Case 4），多余逻辑核必须串行排队，调度开销迅速放大。

### 2. 核数匹配物理核的收益（Case 5 → 6，实际计算）

| 配置项               | Case 5             | Case 6             |
| -------------------- | ------------------ | ------------------ |
| 逻辑核数             | 1024（≫ 物理核）   | 64（= 物理核）     |
| 每核处理数据量       | 1024               | 16384              |
| 每核 UB 占用         | 4 KB               | 64 KB              |
| **Task Duration**    | **54.087 μs**      | **34.357 μs**      |
| 单核 aiv_time 平均   | 2.697 μs           | 32.852 μs          |
| MTE2 实际带宽        | 33.81 GB/s         | **218.21 GB/s**    |
| Vector 占比          | 0.9423             | 0.9861             |

Case 5 → Case 6 端到端加速比 ≈ **1.57×**。

收益来自：

1. 消除多余逻辑核的串行等待开销（1024 逻辑核 → 64 物理核 = 16× 排队）；
2. 每核工作集从 1024 增到 16384，调度头开销摊薄到更多有效计算上；
3. MTE2 实际带宽从 33.8 GB/s 提升到 218.2 GB/s（≈6.5×），更接近硬件峰值，说明 Case 6 的访存模式被充分流水起来。

### 3. INDEX_WIDTH 256 → 1024 后的变化

相比此前 `INDEX_WIDTH=256` 的版本：

- Case 1-4 的 Task Duration 与之前数量级一致（空壳核数据规模无关）。
- Case 5 从 ≈34.8 μs 升到 54.1 μs：每核数据量 256 → 1024，工作量翻 4 倍，本来就该更慢。
- Case 6 从 ≈10.9 μs 升到 34.4 μs：每核数据量 4096 → 16384，工作量翻 4 倍，但带宽利用率显著上来（218 GB/s），所以耗时不到线性 4×。
- 优化结论不变：核数匹配物理核仍优于 1024 逻辑核，加速比从原来的 ≈3.18× 收窄到 ≈1.57×（数据量更大时调度开销占比下降，纯计算占比上升，gap 自然缩小）。

## 输出目录结构

```
perf_results_new/
├── summary.md                # 本文档
├── summary_op_basic.csv      # 6 场景 OpBasicInfo + Pipe 横向汇总
├── scenario_1/
│   ├── msprof_op.log
│   └── OPPROF_*/             # SCENARIO_NUM=1 的 msprof op 完整产物
├── scenario_2/...
├── scenario_3/...
├── scenario_4/...
├── scenario_5/...
└── scenario_6/...
```

每个 `OPPROF_*` 目录下含：`OpBasicInfo.csv`、`PipeUtilization.csv`、`MemoryUB.csv`、`L2Cache.csv`、`ArithmeticUtilization.csv`、`ResourceConflictRatio.csv`、`Memory.csv`、`MemoryL0.csv`、`visualize_data.bin`、`dump/`。
