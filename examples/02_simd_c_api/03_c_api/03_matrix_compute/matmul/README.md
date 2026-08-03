# MatMul C API样例

## 概述

本样例使用 Ascend C C API 在单个 Cube Core 上实现固定规格的 FP32 矩阵乘：

$$
C_{i,j} = \sum_{k=0}^{39} A_{i,k} B_{k,j} + Bias_j
$$

输入输出规格如下：

| 参数 | 类型 | Shape | GM排布 | 是否转置 |
|---|---|---:|---|---|
| `src0` / A | float | [30, 40] | ND | 否 |
| `src1` / B | float | [40, 70] | ND | 否 |
| `weight` / Bias | float | [1, 70] | ND | - |
| `dst` / C | float | [30, 70] | ND | - |

核心约束：

- 核函数声明为 `__global__ __cube__`，矩阵乘加由 `asc_mmad` 在 Cube 单元执行；
- 启动核数固定为 1，不进行 Host 侧分块或逐元素计算；
- Host 仅负责文件 I/O、Device 内存管理、核函数启动和结果回传；
- 未调用 `asc_enable_hf32`，避免 HF32 舍入带来的额外精度损失；
- 支持 Ascend 950PR/Ascend 950DT（`dav-3510`）。

> `asc_mmad` 标准矩阵乘 C API 在 CANN 9.1.0 发布说明中标注为新增接口，因此本样例的可复现基线为 CANN 9.1.0。任务书中的“CANN 9.0.0～9.1.0”版本歧义已记录在 [asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404)。

## 实现流程

1. `asc_copy_gm2l1_nd2nz`：A、B、Bias 从 GM 搬入 L1，并完成 ND 到 Nz 转换；
2. `asc_copy_l12l0a`：A 从 L1 搬入 L0A；
3. `asc_copy_l12l0b_transpose`：B 从 L1 搬入 L0B，在搬运中转置分形并生成 Cube 所需的 Zn 排布；
4. `asc_copy_l12bt`：Bias 从 L1 搬入 BiasTable Buffer；
5. `asc_mmad`：按对齐后的 `right_width=80` 计算 `A × B + Bias`，C 的初值来自 BiasTable Buffer；
6. `asc_copy_l0c2gm`：仅将有效 `[30,70]` 区域由 L0C 搬回 GM，并完成 Nz 到 ND 转换。

非对齐尾部由样例显式管理：M 从 30 扩大到 32，B 的 K/N 从 `[40,70]` 扩大到 `[48,80]`，L0C 使用 `[32,80]`；Cube 读取完整对齐分形，最终由 Fixpipe 裁掉无效行列。CANN C API 不会替调用方完成通用动态尾块治理。

详细对齐和内存计算见 [DESIGN.md](./DESIGN.md)。

## 目录结构

```text
matmul
├── figures
│   └── validation_summary.png   # CANN 9.1.0真机验证截图
├── scripts
│   ├── gen_data.py              # 生成输入、FP64参考值和case清单
│   ├── run_cases.py             # 在Ascend 950环境连续执行6个精度case
│   └── verify_result.py         # 按opbase FLOAT32标准校验
├── test_results
│   └── precision_summary.json   # 2026-08-03 Ascend950PR实测精度摘要
├── CMakeLists.txt
├── data_utils.h
├── matmul.asc
├── run.sh                       # 编译并执行全部精度case
├── DESIGN.md
├── SELF_TEST_REPORT.md
├── README.md
└── README_en.md
```

## 环境要求

| 项目 | 要求 |
|---|---|
| 硬件 | Ascend 950PR / Ascend 950DT |
| NPU架构 | `dav-3510` |
| CANN | 9.1.0 |
| Python | 3.8+ |
| Python依赖 | NumPy |

如未安装 NumPy：

```bash
python3 -m pip install numpy
```

## 一键编译与验证

在本样例目录执行以下命令，即可完成编译并运行全部 6 个精度 case：

```bash
./run.sh
```

`run.sh` 按顺序完成以下操作：

1. 使用 CMake 配置并编译 `matmul.asc`，生成 `build/demo`；
2. 调用 `scripts/gen_data.py` 生成输入和 FP64 CPU Golden；
3. 启动单个 Cube Core 执行 MatMul；
4. 调用 `scripts/verify_result.py` 校验逐元素混合容差和最大绝对误差；
5. 对 `uniform`、`normal`、`boundary`、`bias_only`、`non_aligned_tail` 和 `zeros` 六个 case 重复上述流程。

任意编译、运行或精度检查失败时，脚本立即以非零状态退出。全部通过时，终端最后输出：

```text
Cases: 6/6 passed
Overall result: PASS
```

最终汇总表同时列出每个 case 的 matched ratio、最大绝对误差、有效阈值和 PASS/FAIL，并打印架构、运行模式、头文件来源、构建目录及报告目录。

默认构建目录为 `build/`，验证产物位于 `output/cases/`：

| 文件 | 说明 |
|---|---|
| `*_manifest.json` | case 名称、Seed、Shape 和实际输入范围 |
| `*_output.bin` | Ascend 950 实际输出 |
| `*_golden.bin` | FP64 CPU 计算后舍入为 FP32 的参考输出 |
| `*_report.json` | matched ratio、最大绝对误差、阈值和最终结果 |

正式 CANN 9.1.0 验收环境应使用安装包头文件：

```bash
USE_REPO_HEADERS=0 ./run.sh
```

当前 CANN 9.0.0-beta.2 环境缺少标准矩阵 C API 声明，默认 `USE_REPO_HEADERS=1`，使用当前 asc-devkit 仓库头文件完成开发验证。

### 可选参数

| 环境变量 | 默认值 | 说明 |
|---|---|---|
| `BUILD_DIR` | `./build` | CMake 构建目录 |
| `RUN_DIR` | 当前样例目录 | 输入、输出和报告根目录 |
| `JOBS` | `2` | 并行编译任务数 |
| `SEED` | `20260803` | 首个 case 的随机种子，后续 case 依次加 1 |
| `ARCH` | `dav-3510` | NPU 架构 |
| `ASC_RUN_MODE` | `npu` | ASC 运行模式 |
| `USE_REPO_HEADERS` | `1` | 是否额外使用 asc-devkit 仓库头文件 |
| `CANN_ENV` | 未设置 | 可选的 CANN 环境脚本路径 |

如果尚未加载 CANN 环境，可执行：

```bash
CANN_ENV=${install_path}/cann/set_env.sh ./run.sh
```

### 手工执行单个 case

调试单个 case 时，可以使用：

```bash
source ${install_path}/cann/set_env.sh

mkdir -p build
cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..

python3 scripts/gen_data.py --case uniform --seed 20260803
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

使用独立构建目录切换到 NPU 仿真模式：

```bash
ASC_RUN_MODE=sim BUILD_DIR=build_sim ./run.sh
```

## 精度 case

| case | 说明 | 范围 |
|---|---|---|
| `uniform` | 均匀随机分布 | [-100, 100] |
| `normal` | 正态分布并裁剪 | [-100, 100] |
| `boundary` | ±100、±1、±0.5、0等边界/抵消组合 | [-100, 100] |
| `bias_only` | A/B全零、Bias为非零边界序列，专门验证Bias广播 | [-100, 100] |
| `non_aligned_tail` | 直接命中 A[29,39]、B[39,69] 和 Bias[69]，验证最后有效分形 | [-2, 3.25] |
| `zeros` | 全零输入和Bias | 0 |

每个 case 的输入清单、输出、golden 和 JSON 精度报告会归档到 `output/cases/`。

## 精度判定

`verify_result.py` 对齐生态算子开源 FLOAT32 精度标准：

- `rtol = 2^-10`
- `atol = 2^-16`
- `matched_ratio >= 0.99`
- `max_abs_error <= max(1e-2, 32 ULP)`

只有匹配率和最大绝对误差两个条件同时满足时才返回成功。

校验器会拒绝长度错误、NaN 和 Inf。非有限输出的失败报告使用 JSON `null` 表示不可用的数值字段，保证报告仍可被严格 JSON 解析器读取。

## 输入输出文件

| 文件 | 数据类型 | 元素数 |
|---|---|---:|
| `input/src0.bin` | float32 | 30 × 40 |
| `input/src1.bin` | float32 | 40 × 70 |
| `input/weight.bin` | float32 | 1 × 70 |
| `output/output.bin` | float32 | 30 × 70 |
| `output/golden.bin` | float32 | 30 × 70 |

主程序使用相对路径访问上述文件，因此应从 `matmul` 样例根目录启动 `./build/demo`。

## 当前验证状态

已于 2026-08-03 在 Ascend950PR 实机执行全部 6 个 case，全部通过，逐元素匹配率均为 100%。其中最大绝对误差为 `0.01171875`，对应有效硬上限为 `0.25`（32 ULP 路径）。Bias-only 和 non-aligned-tail 用例误差均为 0，确认 BiasTable 广播和非对齐尾部分形处理正确。

本次环境的编译器/运行时为 CANN 9.0.0-beta.2，但安装包头文件不包含标准 `asc_mmad` 等接口；测试时使用当前 `asc-devkit` master 头文件（commit `fb822b36`）完成编译。因此正式验收前必须在纯 CANN 9.1.0 环境重新编译并执行全部 case，同时补充实际终端截图。详细数据见 [SELF_TEST_REPORT.md](./SELF_TEST_REPORT.md)。

开发过程中发现的 CANN 版本范围歧义已提交易用性 Issue：[asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404)。

机器可读摘要见 [precision_summary.json](./test_results/precision_summary.json)。
