# gridDim 配置样例 msprof op 性能汇总（10 次平均）

采集工具：`msprof op --application=./grid_config`
采集时间：2026-05-27
目标平台：Ascend 950PR/950DT（AIV 物理核数 = 64）
采集场景：`SCENARIO_NUM = 1..6`，每场景 **10 次 trial**

## Task Duration 汇总（10 次平均）

数据来源：`scenario_<N>/trial_<1..10>/OPPROF_*/OpBasicInfo.csv`

| Scenario | Kernel              | Block Dim | Mean (μs)   | Min (μs) | Max (μs) | Std (μs) | 说明                                  |
| -------- | ------------------- | --------- | ----------- | -------- | -------- | -------- | ------------------------------------- |
| 1        | empty_kernel        | 36        | **2.022**   | 1.770    | 2.212    | 0.167    | 调度基线，核数 < 物理核 (36 < 64)     |
| 2        | empty_kernel        | 64        | **2.469**   | 2.169    | 2.693    | 0.200    | 调度基线，核数 = 物理核               |
| 3        | empty_kernel        | 128       | **3.370**   | 3.084    | 3.542    | 0.127    | 调度基线，核数 > 物理核 (2× 物理核)   |
| 4        | empty_kernel        | 1024      | **16.258**  | 16.067   | 16.686   | 0.186    | 调度基线，核数 ≫ 物理核 (16× 物理核)  |
| 5        | gather_adds_kernel  | 1024      | **35.081**  | 34.592   | 35.619   | 0.306    | 实际计算，核数 ≫ 物理核               |
| 6        | gather_adds_kernel  | 64        | **10.680**  | 10.262   | 11.174   | 0.230    | 实际计算，核数 = 物理核（最优配置）   |

变异系数（Std/Mean）整体 < 9%，数据稳定。

## 关键观察

### 调度开销随逻辑核数增长（Case 1 → 4，空壳核函数）

| 对比         | Block Dim 倍数  | Mean 耗时倍数        |
| ------------ | --------------- | -------------------- |
| Case 1 → 2   | 36 → 64 (1.78×) | 2.022 → 2.469 (1.22×) |
| Case 2 → 3   | 64 → 128 (2×)   | 2.469 → 3.370 (1.36×) |
| Case 3 → 4   | 128 → 1024 (8×) | 3.370 → 16.258 (4.83×) |

逻辑核数在物理核以内时 Task Duration 涨幅平缓；一旦显著超出物理核（Case 4），多余逻辑核必须串行排队，调度开销迅速放大。

### 核数匹配物理核的收益（Case 5 → 6，实际计算）

| 配置项               | Case 5             | Case 6             |
| -------------------- | ------------------ | ------------------ |
| 逻辑核数             | 1024（≫ 物理核）   | 64（= 物理核）     |
| 每核处理数据量       | 256                | 4096               |
| 每核 UB 占用         | 1 KB               | 16 KB              |
| **Mean 耗时**        | **35.081 μs**      | **10.680 μs**      |

Case 5 → Case 6 加速比 ≈ **3.28×**（10 次平均）。

将逻辑核数从 1024 降到 64 后：

1. 消除多余逻辑核的串行等待开销；
2. 每核工作集从 256 增到 4096，调度头开销摊薄到更多有效计算上；
3. 单核 UB 占用提升到 16 KB，仍远低于容量约束。

## 输出目录结构

```
perf_results_10/
├── summary.md                # 本文档
├── summary_10trials.csv      # 6 场景 × Mean/Min/Max/Std 横向汇总
├── raw_10trials.csv          # 60 条原始 Task Duration 记录
├── scenario_1/
│   ├── trial_1/OPPROF_*/     # 第 1 次 msprof op 完整产物
│   ├── trial_2/OPPROF_*/
│   └── ...trial_10
├── scenario_2/ ...
├── scenario_3/ ...
├── scenario_4/ ...
├── scenario_5/ ...
└── scenario_6/ ...
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
    for t in $(seq 1 10); do
        msprof op --output="../perf_results_10/scenario_${i}/trial_${t}" \
                  --application=./grid_config
    done
    cd ..
done
```
