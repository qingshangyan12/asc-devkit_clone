# gridDim 配置样例 msprof op 性能汇总

采集工具：`msprof op --application=./grid_config`
采集时间：2026-05-27
目标平台：Ascend 950PR/950DT（AIV 物理核数 = 64）
采集场景：`SCENARIO_NUM = 1..6`

## Task Duration 汇总

数据来源：`scenario_<N>/OPPROF_*/OpBasicInfo.csv`

| Scenario | Kernel              | Block Dim | Task Duration (μs) | 说明                                  |
| -------- | ------------------- | --------- | ------------------ | ------------------------------------- |
| 1        | empty_kernel        | 36        | 2.019              | 调度基线，核数 < 物理核 (36 < 64)     |
| 2        | empty_kernel        | 64        | 2.505              | 调度基线，核数 = 物理核               |
| 3        | empty_kernel        | 128       | 3.561              | 调度基线，核数 > 物理核 (2× 物理核)   |
| 4        | empty_kernel        | 1024      | 16.575             | 调度基线，核数 ≫ 物理核 (16× 物理核)  |
| 5        | gather_adds_kernel  | 1024      | 34.778             | 实际计算，核数 ≫ 物理核               |
| 6        | gather_adds_kernel  | 64        | **10.943**         | 实际计算，核数 = 物理核（最优配置）   |

## 关键观察

### 调度开销随逻辑核数增长（Case 1 → 4，空壳核函数）

| 对比         | Block Dim 倍数  | Task Duration 倍数 |
| ------------ | --------------- | ------------------ |
| Case 1 → 2   | 36 → 64 (1.78×) | 2.019 → 2.505 (1.24×) |
| Case 2 → 3   | 64 → 128 (2×)   | 2.505 → 3.561 (1.42×) |
| Case 3 → 4   | 128 → 1024 (8×) | 3.561 → 16.575 (4.65×) |

逻辑核数在物理核以内时 Task Duration 涨幅平缓；一旦显著超出物理核（Case 4），多余逻辑核必须串行排队，调度开销迅速放大。

### 核数匹配物理核的收益（Case 5 → 6，实际计算）

| 配置项               | Case 5             | Case 6             |
| -------------------- | ------------------ | ------------------ |
| 逻辑核数             | 1024（≫ 物理核）   | 64（= 物理核）     |
| 每核处理数据量       | 256                | 4096               |
| 每核 UB 占用         | 1 KB               | 16 KB              |
| **Task Duration**    | **34.778 μs**      | **10.943 μs**      |

Case 5 → Case 6 加速比 ≈ **3.18×**。

将逻辑核数从 1024 降到 64 后：

1. 消除多余逻辑核的串行等待开销；
2. 每核工作集从 256 增到 4096，调度头开销摊薄到更多有效计算上；
3. 单核 UB 占用提升到 16 KB，仍远低于容量约束。

## 输出目录结构

```
perf_results/
├── summary.md                # 本文档
├── summary_op_basic.csv      # 6 场景 OpBasicInfo 横向汇总（机器可读）
├── scenario_1/OPPROF_*/      # SCENARIO_NUM=1 的 msprof op 完整产物
├── scenario_2/OPPROF_*/      # SCENARIO_NUM=2
├── scenario_3/OPPROF_*/      # SCENARIO_NUM=3
├── scenario_4/OPPROF_*/      # SCENARIO_NUM=4
├── scenario_5/OPPROF_*/      # SCENARIO_NUM=5
└── scenario_6/OPPROF_*/      # SCENARIO_NUM=6
```

每个 `OPPROF_*` 目录下包含的 CSV：

| CSV 文件                     | 内容                              |
| ---------------------------- | --------------------------------- |
| `OpBasicInfo.csv`            | 核函数名、Task Duration、Block Dim、频率等基础信息 |
| `ArithmeticUtilization.csv`  | 算术单元利用率                    |
| `PipeUtilization.csv`        | 流水线利用率                      |
| `MemoryUB.csv`               | UB 内存访问统计                   |
| `MemoryL0.csv`               | L0 Buffer 访问统计                |
| `Memory.csv`                 | 总内存访问统计                    |
| `L2Cache.csv`                | L2 Cache 命中/未命中              |
| `ResourceConflictRatio.csv`  | 资源冲突率                        |

## 复现方式

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh
cd /home/qxk/asc-devkit/examples/03_simt_api/03_best_practices/02_simt_grid_config_pure

for i in 1 2 3 4 5 6; do
    rm -rf bu && mkdir bu && cd bu
    cmake -DSCENARIO_NUM=$i .. && make -j
    msprof op --output="../perf_results/scenario_$i" --application=./grid_config
    cd ..
done
```
