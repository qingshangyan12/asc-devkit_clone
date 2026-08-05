# ELU 样例

## 概述

本样例使用 Ascend C 寄存器 C API 实现逐元素 ELU：

```text
y = scale * x                                      , x > 0
y = alpha * scale * (exp(x * input_scale) - 1)    , x <= 0
```

支持 `float32`、`float16` dense 连续输入。三个标量均为运行时 `float` 参数；half
输入先扩宽到 float 完成计算，再按 RN 转回 half。计算全程使用 Vector Core，Host
只负责数据搬运、分核和 kernel 生命周期。

## 支持产品

| 产品 | CANN 软件版本 |
| --- | --- |
| Ascend 950PR / Ascend 950DT | CANN 9.0.0～9.1.0 |

## 目录结构

```text
elu/
├── scripts/
│   ├── gen_data.py       # 生成单个输入与 golden
│   ├── gen_cases.py      # 生成 54 个精度和 14 个性能用例
│   ├── run_precision_cases.sh
│   ├── run_perf_cases.sh
│   └── summarize_perf.py
├── CMakeLists.txt
├── data_utils.h
├── elu.asc
├── README.md
└── README_en.md
```

## 实现说明

- dense ND Tensor 按 `numel` 扁平处理，元素数使用 `uint32_t`；空输入由 Host no-op。
- 每核使用 2048 元素输入/输出 UB tile，支持跨 tile 输入和非 32B 对齐尾部。
- `|x * input_scale| < 0.1` 时，float32 使用五阶、float16 使用四阶 Horner 多项式
  计算 `expm1`，其余位置使用 `asc_exp(z) - 1`。
- 先以 `asc_select` 选择正负分支，再统一乘 `scale`，避免未选分支污染结果。
- `src` 与 `dst` 为不重叠的连续地址；本样例不提供 in-place 或 stride 接口。

## 编译与运行

配置 CANN 环境后，在本目录执行：

```bash
source ${install_path}/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
python3 scripts/gen_data.py --dtype float32 --length 2048
./build/demo float32 2048 1.0 1.0 1.0 input/input_x.bin output/golden.bin
```

`gen_data.py` 的默认参数与 `./build/demo` 的无参数模式一致，因此最后一条也可简写为
`./build/demo`。

命令行格式：

```text
demo <float32|float16> <length> <alpha> <scale> <input_scale> <input.bin> <golden.bin>
```

golden 文件固定为 float32，程序按生态算子开源精度标准输出 `matched_ratio` 和
`max_abs_error`。生成并运行完整精度矩阵：

```bash
python3 scripts/gen_cases.py --output-root cases
bash scripts/run_precision_cases.sh ./build/demo cases/precision precision-results
```

用例生成需要带 NumPy 的 Python 3；在同一环境有多套 Python 时，应显式使用能成功
`import numpy` 的解释器。用例执行和结果汇总只使用 Bash 与 Python 标准库。

结果保存在 `precision-results/runner.log`、逐 case 日志和 `precision_summary.csv`；结果
目录必须不存在，避免覆盖既有证据。通过时末行是：

```text
PRECISION_SUMMARY passed=54 failed=0 total=54
```

矩阵覆盖任务要求的双 dtype 与 `1/32/1024/2048` shape，并扩展空输入、非对齐尾部、
公共系数组合、FLOAT 标量保留、未选分支隔离、属性 NaN/Inf 以及输入 NaN/±Inf。双 dtype
的 `131072` 与 `1000001` 分别使每个 block 执行 2 个和 9 个 UB tile，覆盖第二次及后续
GM→UB→Vector→GM 循环和跨迭代同步。

精度用例清单：

| 分组 | 用例数 | 覆盖点 |
| --- | ---: | --- |
| shape | 26 | 双 dtype × `1/31/32/33/255/1023/1024/2048/4096/5000/131072/1000001/0` |
| common coefficients | 16 | SELU、互异系数、近零、快速饱和、负 alpha/scale/input_scale、零 input_scale |
| dtype contracts | 8 | FLOAT 标量保留、Host/half 预乘隔离、属性 NaN/Inf 与指数上溢 |
| non-finite input | 4 | 双 dtype 分别隔离输入 NaN 与输入 ±Inf |

### 性能采集

任务书的性能要求为“无”，本样例不设置性能门槛。性能 runner 对两个 dtype 的
`32/256/1024/2048/4096/16384/65536` 共 14 个 case 先做正确性校验，再直接对正式
`demo` 二进制调用 msprof：

```bash
ELU_PERF_REPEATS=5 bash scripts/run_perf_cases.sh \
  ./build/demo cases/performance performance-results
```

`ELU_PERF_REPEATS` 默认为 1。runner 保留每次 profiling 日志和原始 `op_summary.csv`，
并生成 `performance_raw.csv` 与 `performance_summary.csv`；汇总同时记录
`Task Duration(us)`、`aiv_time(us)`、`aiv_total_cycles`、`Block Num` 和 `Task Type`，
核对纯 `AI_VECTOR_CORE`、无 Cube/AIC 活动以及预期分核。不同指标、代码、设备、CANN
版本或采集策略的数字不可直接当作性能提升比较。

切换到 NPU 仿真模式前应使用新的构建目录：

```bash
cmake -S . -B build-sim -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build-sim -j
```
