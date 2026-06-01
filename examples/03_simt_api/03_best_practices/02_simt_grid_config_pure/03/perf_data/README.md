# perf_data —— 实测性能数据归档

本目录保存中-shape 核数扫描的全部实测数据，用于支撑 `../README` 中"中-shape 不必打满物理核"的结论。

## 内容

- **measurements.md** —— 数据汇总表与结论（人读首选）。所有数字为 msprof Task Duration(us)，每点多次取平均。
- **raw/** —— msprof op 落盘的原始 CSV（已剔除 `dump/` 与 `visualize_data.bin` 等二进制中间产物，仅保留可读 CSV）。

## raw/ 目录结构

```text
raw/
├── shape16384_fullsweep_1to64/   # [128,128]=16384，1~64核全量扫描（含谷底，最优≈32核）
│   └── nb<核数>/t<第几次>/*.csv   #   每核数采集5次（t1..t5）
├── shape16384/                   # 16384 的小核数补点（1/2/4/8/16/32），探针 8x2048 写法
├── shape65536/                   # [32,2048]=65536，核数16/32/64（单调降，64核最快）
└── shape131072/                  # [64,2048]=131072，核数16/32/64（单调降，64核最快）
```

每个叶子目录是一次 `msprof op` 的输出，关键文件：

| 文件 | 用途 |
| ---- | ---- |
| `OpBasicInfo.csv` | 含 **Task Duration(us)**，本例核心指标 |
| `PipeUtilization.csv` | 各 block 的 aiv_time / cycles / vec_ratio 等 |
| 其余 CSV | L2Cache / Memory / 算术利用率等辅助指标 |

## 复现方法

探针程序见 `../probe/`（`probe_gather.asc` + `CMakeLists.txt`，参数化 `TOTAL`/`NB`/`TPB`）：

```bash
source /usr/local/Ascend/cann/set_env.sh
cd ../probe && mkdir build && cd build
# 对每个 (元素总数 TOTAL, 核数 NB) 组合：
cmake -DTOTAL=16384 -DNB=32 .. && make -j
msprof op --application=./probe_gather --output=<输出目录>
# 从 <输出目录>/OPPROF_*/OpBasicInfo.csv 读取 Task Duration，多次取平均
```

> 每 block 线程数固定 2048（`TPB`，gather 为 memory-bound，取硬件上限以并发掩盖访存延迟）。
> 环境：Ascend 950PR，物理 AIV 核数 64。数据存在 run-to-run 波动（见各表 std 列）。
