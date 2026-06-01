# grid_config 17 场景性能复测报告

## 复测信息

| 项目 | 内容 |
| ---- | ---- |
| 采集工具 | `msprof op --application=./grid_config` |
| 采集日期 | 2026-05-30 |
| 目标平台 | Ascend 950PR/950DT（AIV 物理核数 = 64） |
| CANN 版本 | cann-9.0.0 |
| 编译命令 | `cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$i ..; make -j` |
| 数据指标 | Task Duration(us)，来源 `scenario_<N>/OPPROF_*/OpBasicInfo.csv` |
| 采集方式 | 每场景单次采集（**单次结果含运行间抖动，下方偏差需据此理解**） |
| 对比基准 | `../Readme_new.md` 各性能表 |

## 实测原始数据（17 场景）

| SCENARIO | Kernel | Block Dim | Task Duration(us) |
| :------: | ------ | :-------: | :---------------: |
| 1  | empty_kernel          | 1    | 1.329   |
| 2  | empty_kernel          | 32   | 1.450   |
| 3  | empty_kernel          | 64   | 2.103   |
| 4  | empty_kernel          | 128  | 3.052   |
| 5  | empty_kernel          | 1024 | 16.226  |
| 6  | gather_naive_kernel   | 1024 | 77.734  |
| 7  | gather_strided_kernel | 64   | 58.956  |
| 8  | gather_strided_kernel | 32   | 110.514 |
| 9  | gather_strided_kernel | 16   | 18.003  |
| 10 | gather_strided_kernel | 32   | 11.268  |
| 11 | gather_strided_kernel | 64   | 8.972   |
| 12 | gather_strided_kernel | 128  | 11.241  |
| 13 | gather_strided_kernel | 4    | 11.164  |
| 14 | gather_strided_kernel | 8    | 6.698   |
| 15 | gather_strided_kernel | 16   | 4.746   |
| 16 | gather_strided_kernel | 32   | 3.922   |
| 17 | gather_strided_kernel | 64   | 4.546   |

## 逐表对比

### Case 0：空核调度基线（场景 1–5）

| SCENARIO | Block Dim | 文档(us) | 实测(us) | 偏差 |
| :------: | :-------: | :------: | :------: | :--: |
| 1 | 1    | 1.356  | 1.329  | −2.0%  |
| 2 | 32   | 1.643  | 1.450  | −11.7% |
| 3 | 64   | 2.218  | 2.103  | −5.2%  |
| 4 | 128  | 2.927  | 3.052  | +4.3%  |
| 5 | 1024 | 16.104 | 16.226 | +0.8%  |

**结论**：趋势完全一致——逻辑核 ≤ 物理核(64) 时缓涨、>64 后陡增。文档结论成立。

### Case 1：大shape Gather（场景 6–8）

| SCENARIO | Block Dim | 文档(us) | 实测(us) | 偏差 |
| :------: | :-------: | :------: | :------: | :--: |
| 6 | 1024   | 75.834  | 77.734  | +2.5% |
| 7 | **64** | **56.902** | **58.956** | +3.6% |
| 8 | 32     | 110.419 | 110.514 | +0.1% |

**结论**：三个值都很贴近。64核最优、32核（半数物理核）反而比1024核还慢——结论成立。6→7 加速比实测 1.32×（文档 1.33×）。

### Case 2：中shape 131072（场景 9–12）

| SCENARIO | Block Dim | 文档(us) | 实测(us) | 偏差 |
| :------: | :-------: | :------: | :------: | :--: |
| 9  | 16     | 16.753 | 18.003 | +7.5%  |
| 10 | 32     | 10.127 | 11.268 | +11.3% |
| 11 | **64** | **7.753** | **8.972** | +15.7% |
| 12 | 128    | 9.857  | 11.241 | +14.0% |

**结论**：整段系统性偏高约 7–16%，但形状不变——64核最优、128核越过物理核反弹（实测 +25.3%，文档 +27.1%）。结论成立。**偏差偏大，建议多采几次取中位数再决定是否回写文档。**

### Case 3：小shape 16384（场景 13–17）

| SCENARIO | Block Dim | 文档(us) | 实测(us) | 偏差 |
| :------: | :-------: | :------: | :------: | :--: |
| 13 | 4      | 10.792 | 11.164 | +3.4% |
| 14 | 8      | 6.676  | 6.698  | +0.3% |
| 15 | 16     | 4.693  | 4.746  | +1.1% |
| 16 | **32** | **3.834** | **3.922** | +2.3% |
| 17 | 64     | 4.212  | 4.546  | +7.9% |

**结论**：谷形成立——32核最低点、64核反弹。但 32→64 实测反弹 +15.9%，文档写 +9.9%，幅度差一截。

## 总体结论

文档 `Readme_new.md` 中 **17 个场景的全部定性结论与趋势均复现成立**：

1. Case 0 调度开销随核数台阶式上涨，≤64核平缓、>64核陡增；
2. Case 1 大shape 下 64核（=物理核）最优，且 32核劣于 1024核；
3. Case 2 中shape 下 64核最优、128核越过物理核反弹；
4. Case 3 小shape 下 32核（半数物理核）是谷底，64核反弹。

绝对值多数偏差在个位数百分比内，属正常运行间抖动。**两处偏差稍大、不影响结论但值得留意**：

- Case 2 整段系统性偏高 7–16%（尤其 64核 7.753→8.972）；
- Case 3 的 32→64 反弹幅度（文档 +9.9% / 实测 +15.9%）。

如需让文档数值更贴近本机当前实测，建议对 Case 2、Case 3 各多采几次取中位数再回写。

## 复现方式

```bash
source /usr/local/Ascend/cann/set_env.sh
cd /home/qxk/asc-devkit/examples/03_simt_api/03_best_practices/02_simt_grid_config_pure
for i in $(seq 1 17); do
    rm -rf build && mkdir build && cd build
    cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$i .. && make -j
    msprof op --output="../perf_results_recheck/scenario_$i" --application=./grid_config
    cd ..
done
```

## 目录结构

```
perf_results_recheck/
├── recheck_report.md         # 本报告
├── recheck_progress.log      # 采集进度日志（含每场景 OpBasicInfo 行）
└── scenario_1 .. scenario_17/OPPROF_*/   # 各场景 msprof op 完整产物
```
