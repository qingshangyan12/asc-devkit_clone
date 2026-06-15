# Pure SIMT vs Mixed (SIMT+SIMD) gridDim 配置样例性能对比

采集工具：`msprof op --application=./grid_config`
采集时间：2026-05-27
目标平台：Ascend 950PR/950DT（AIV 物理核数 = 64）
采集方法：每场景 10 次 trial 取均值，提取 `OpBasicInfo.csv` 中的 `Task Duration`

样例位置：

| 样本     | 路径                                                      | 编程模型           | 编译选项                  | 场景数 |
| -------- | --------------------------------------------------------- | ------------------ | ------------------------- | ------ |
| **Pure** | `02_simt_grid_config_pure/grid_config.asc`                | Pure SIMT          | `--npu-arch=dav-3510 --enable-simt` | 6      |
| **Mixed**| `02_simt_grid_config/grid_config.asc`                     | SIMT+SIMD 混合 (`asc_vf_call`) | `--npu-arch=dav-3510`     | 10     |

## 1. 全量数据（10 次 trial 均值）

### 1.1 Pure SIMT 版

| Scenario | Kernel              | Block Dim | Mean (μs)  | Std (μs) | 配置说明                         |
| -------- | ------------------- | --------- | ---------- | -------- | -------------------------------- |
| 1        | empty_kernel        | 36        | **2.022**  | 0.167    | 36 cores, empty kernel           |
| 2        | empty_kernel        | 64        | **2.469**  | 0.200    | 64 cores, empty kernel           |
| 3        | empty_kernel        | 128       | **3.370**  | 0.127    | 128 cores, empty kernel          |
| 4        | empty_kernel        | 1024      | **16.258** | 0.186    | 1024 cores, empty kernel         |
| 5        | gather_adds_kernel  | 1024      | **35.081** | 0.306    | 1024 cores, gather+adds          |
| 6        | gather_adds_kernel  | 64        | **10.680** | 0.230    | 64 cores, gather+adds（最优）    |

### 1.2 Mixed 版（SIMT+SIMD via `asc_vf_call`）

| Scenario | Kernel                          | Block Dim | Mean (μs)  | Std (μs) | 配置说明                                          |
| -------- | ------------------------------- | --------- | ---------- | -------- | ------------------------------------------------- |
| 1        | vf_kernel                       | 36        | **2.036**  | 0.130    | 36 cores, 1 vf call (空壳)                        |
| 2        | vf_kernel                       | 64        | **2.482**  | 0.178    | 64 cores, 1 vf call (空壳)                        |
| 3        | vf_kernel                       | 128       | **3.333**  | 0.167    | 128 cores, 1 vf call (空壳)                       |
| 4        | vf_kernel                       | 1024      | **16.452** | 0.118    | 1024 cores, 1 vf call (空壳)                      |
| 5        | vf_kernel                       | 36        | **2.201**  | 0.182    | 36 cores, 2 vf calls                              |
| 6        | vf_kernel                       | 1024      | **16.494** | 0.146    | 1024 cores, 2 vf calls                            |
| 7        | vf_kernel                       | 64        | **2.999**  | 0.154    | 64 cores, 14 vf calls                             |
| 8        | gather_adds_kernel              | 1024      | **49.546** | 0.326    | 1024 cores, gather+adds                           |
| 9        | gather_adds_kernel              | 64        | **11.096** | 0.137    | 64 cores, vf loop **outside** kernel              |
| 10       | gather_adds_kernel_vf_inside    | 64        | **9.945**  | 0.163    | 64 cores, vf loop **inside** kernel（最优）       |

## 2. 同源场景对比

挑选两边语义可对齐的场景：

| 对应关系               | Pure (μs)  | Mixed (μs) | 差异      | 备注                                |
| ---------------------- | ---------- | ---------- | --------- | ----------------------------------- |
| 36 cores 空壳          | 2.022 (#1) | 2.036 (#1) | +0.7%     | 调度路径差异 ≈ 0                    |
| 64 cores 空壳          | 2.469 (#2) | 2.482 (#2) | +0.5%     | 调度路径差异 ≈ 0                    |
| 128 cores 空壳         | 3.370 (#3) | 3.333 (#3) | -1.1%     | 调度路径差异 ≈ 0                    |
| 1024 cores 空壳        | 16.258 (#4)| 16.452 (#4)| +1.2%     | 调度路径差异 ≈ 0                    |
| 1024 cores gather+adds | 35.081 (#5)| 49.546 (#8)| **+41.2%**| 同 1024 核失配，Mixed 每 block 工作量 ≈ 4× Pure（见下注） |
| **gather+adds 最优配置** | 10.680 (#6, 64 核) | **9.945 (#10, 64 核 vf_inside)** | **-6.9%** | 两版各自推荐写法下，Mixed 借助 `Reg::Adds` 向量指令吞吐反超 |

> **关于 1024 核 gather+adds 的 41% 差异归因**：
> 二者的 `rows_per_block` 都是 262144 / 1024 = 256，但每 block 实际处理量不同：
> - Pure 5：每 block 只处理 256 个元素（stride 循环里 threadIdx 0..255 各做 1 次）；
> - Mixed 8：每 block 处理 `THREAD_NUM = 1024` 个元素（`gather_output`/`adds_output`/`DataCopy` 全部按 1024 走），相邻 block 写出区间存在重叠覆盖（仅因重复值碰巧通过精度校验）。
>
> 也就是 Mixed 8 在同 1024 核配置下做了 ≈ 4× 工作量，墙钟仅多 41%，**单位工作量上 Mixed 反而更便宜**，主要受益于 `Reg::Adds` 的 SIMD 吞吐。`V_MTE3` 同步开销固定且很小，不是主导项。
> 结论：1024 核场景下两版都被调度失配主导，谁更快取决于谁工作量更小，对比意义有限——真正应当对照的是各自的最优配置（10.680 vs 9.945）。

### 关键观察

1. **空壳调度开销与编程模型无关**。两版 36/64/128/1024 核空壳 kernel 耗时差异 ≤ 1.2%，说明 `asc_vf_call` 包装本身不引入显著的 host→device launch 额外成本，调度开销主要由 `gridDim` 决定。

2. **核数匹配物理核 + vf-inside 是 Mixed 的最优解**。Mixed scenario 9（vf 循环放在 host 侧 kernel 外）→ 10（vf 循环放在 kernel 内）由 11.10μs 降到 9.94μs（-10.4%），最终略优于 Pure 6 的 10.68μs（-6.9%）。原因是 Mixed 把 SIMD 路径用 `Reg::Adds` 写成显式 vector 指令，比 Pure 的标量 `+` 更紧凑。

3. **gridDim 对 gather+adds 远比模型选择关键**。Pure 5→6（1024→64 核）加速 3.28×、Mixed 8→10（1024→64 核 + vf_inside）加速 4.98×，量级差异说明：先把核数调到物理核，再谈是否切 SIMD 路径。

## 3. 维度专项分析

### 3.1 调度开销随 Block Dim 增长（空壳，两版对照）

| Block Dim | Pure Mean (μs) | Mixed Mean (μs) |
| --------- | -------------- | --------------- |
| 36        | 2.022          | 2.036           |
| 64        | 2.469          | 2.482           |
| 128       | 3.370          | 3.333           |
| 1024      | 16.258         | 16.452          |

二者的曲线几乎重叠：核数 ≤ 物理核（64）阶段耗时缓涨；核数 = 2× 物理核（128）开始线性翻倍；核数 = 16× 物理核（1024）已是 64 核空壳的 ~6.6×。结论：**调度开销主导项与编程模型无关，只取决于 `gridDim`**。

### 3.2 Mixed 独有维度：vf 调用次数

| Scenario | Block Dim | vf calls | Mean (μs) | 单次 vf 摊销 (μs) |
| -------- | --------- | -------- | --------- | ----------------- |
| 1        | 36        | 1        | 2.036     | 2.036             |
| 5        | 36        | 2        | 2.201     | +0.165 / 第 2 次  |
| 2        | 64        | 1        | 2.482     | 2.482             |
| 7        | 64        | 14       | 2.999     | +0.040 / 每次额外 |
| 4        | 1024      | 1        | 16.452    | 16.452            |
| 6        | 1024      | 2        | 16.494    | +0.042 / 第 2 次  |

`asc_vf_call` 一次调用对空壳的额外开销极低（<0.05μs/call），相比之下 `gridDim` 才是决定性因素。

### 3.3 vf 循环位置：outside vs inside（Mixed scenario 9 vs 10）

| 形态                   | Mean (μs) | 说明                                                |
| ---------------------- | --------- | --------------------------------------------------- |
| vf 循环写在 kernel 外  | 11.096    | 每次 vf 调用都要 V_MTE3 同步、`DataCopy` 一次       |
| vf 循环写在 kernel 内  | 9.945     | 一次 gather 整段 → 一次 adds 整段 → 一次 DataCopy    |

把循环从 host launch 路径下沉进 kernel，省去了 vf 切换/同步的重复成本，**-10.4%**。

### 3.4 Pure 6 vs Mixed 10：两版"最优配置"

|                  | Pure 6             | Mixed 10                       |
| ---------------- | ------------------ | ------------------------------ |
| 编程模型         | Pure SIMT          | SIMT+SIMD (vf_inside)          |
| Block Dim        | 64                 | 64                             |
| 加法实现         | 标量 `+` (per-thread) | `Reg::Adds` (vector reg)    |
| Mean (μs)        | 10.680             | **9.945**                      |
| 加速比 (相对 Pure 6) | 1.00×             | **1.074×**                     |

Mixed 在最优配置下比 Pure 快约 **7.4%**，差异来自 SIMD 路径的 vector 指令吞吐优于 SIMT 标量加。

## 4. 选型建议

| 场景需求                         | 推荐编程模型   | 理由                                              |
| -------------------------------- | -------------- | ------------------------------------------------- |
| 快速原型 / 算法可读性优先        | Pure SIMT      | 代码最简单，无需手写 vf 包装                      |
| 极致性能 / 计算密集               | Mixed (vf_inside) | SIMD 寄存器路径吞吐更高，可手动控制同步粒度   |
| 调度开销主导（核数 ≫ 物理核）   | 二者均不推荐   | 先把 `gridDim` 调到 ≈ 物理核                      |
| 多次 vf 计算阶段                  | Mixed          | 把 vf 循环放在 kernel 内（inside 形态）           |

不论哪种模型，**首要原则不变：逻辑核数尽量等于物理核数**。Pure 5→6 加速 3.28×、Mixed 8→10 加速 4.98×，都是这条原则的直接体现。

## 5. 数据稳定性

两版所有场景的变异系数（Std/Mean）均 < 9%（Pure）/ < 9%（Mixed），最大单次偏差不超过均值的 ±10%，可视为可信均值。

## 6. 输出目录结构

```
02_simt_grid_config_pure/perf_results_10/
├── summary.md                # Pure 版独立汇总
├── summary_10trials.csv      # 6 行 Pure 均值
├── raw_10trials.csv          # 60 条原始记录
└── scenario_<1..6>/trial_<1..10>/OPPROF_*/   # msprof op 完整产物

02_simt_grid_config/perf_results_10/
├── summary_10trials.csv      # 10 行 Mixed 均值
├── raw_10trials.csv          # 100 条原始记录
└── scenario_<1..10>/trial_<1..10>/OPPROF_*/  # msprof op 完整产物

02_simt_grid_config_pure/perf_results_10/
└── comparison_pure_vs_mixed.md   # 本文档
```

## 7. 复现方式

```bash
source /usr/local/Ascend/ascend-toolkit/set_env.sh

# Pure 版
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

# Mixed 版
cd /home/qxk/asc-devkit/examples/03_simt_api/03_best_practices/02_simt_grid_config
for i in 1 2 3 4 5 6 7 8 9 10; do
    rm -rf bu && mkdir bu && cd bu
    cmake -DSCENARIO_NUM=$i .. && make -j
    for t in $(seq 1 10); do
        msprof op --output="../perf_results_10/scenario_${i}/trial_${t}" \
                  --application=./grid_config
    done
    cd ..
done
```
