# MatMul C API样例

## 功能说明

本样例使用 Ascend C C API 和单个 Cube Core 实现固定规格的 FP32 矩阵乘加：

```text
C[30, 70] = A[30, 40] × B[40, 70] + Bias[1, 70]
```

| 参数 | 类型 | Shape | GM排布 | 是否转置 |
| --- | --- | --- | --- | --- |
| `src0` / A | float32 | `[30,40]` | ND | 否 |
| `src1` / B | float32 | `[40,70]` | ND | 否 |
| `weight` / Bias | float32 | `[1,70]` | ND | 不适用 |
| `dst` / C | float32 | `[30,70]` | ND | 不适用 |

核函数固定启动 1 个 Cube Core。Host 仅负责文件读写、Device 内存管理、核函数启动和结果回传，不在 Host 侧计算矩阵乘。

## 环境要求

| 项目 | 要求 |
| --- | --- |
| 硬件 | Ascend 950PR / Ascend 950DT |
| NPU架构 | `dav-3510` |
| CANN | 9.1.0 |
| Python | 3.8+ |
| Python依赖 | NumPy |

尚未加载 CANN 环境时，先执行：

```bash
source ${install_path}/cann/set_env.sh
```

## 一键编译和测试

进入算子目录：

```bash
cd examples/02_simd_c_api/03_c_api/03_matrix_compute/matmul
```

在正式 CANN 9.1.0 验收环境中执行：

```bash
USE_REPO_HEADERS=0 ./run.sh
```

`run.sh` 自动完成以下操作：

1. 使用 CMake 编译 `matmul.asc`；
2. 生成测试输入和 FP64 CPU Golden；
3. 在单个 Cube Core 上执行 MatMul；
4. 校验输出精度；
5. 依次执行全部 6 个精度 case；
6. 输出汇总结果。

任意步骤失败时脚本以非零状态退出。全部通过时终端末尾显示：

```text
Cases: 6/6 passed
Overall result: PASS
```

如需在当前 asc-devkit 源码树中使用仓库头文件进行开发验证，可执行：

```bash
./run.sh
```

## 手工编译和测试

```bash
cmake -S . -B build \
  -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
  -DCMAKE_ASC_RUN_MODE=npu \
  -DCMAKE_ASC_FLAGS=

cmake --build build -j2

python3 scripts/run_cases.py \
  --demo build/demo \
  --root . \
  --seed 20260803
```

## 自测用例

### 精度测试

| Case | 测试目的 | 输入范围 |
| --- | --- | --- |
| `uniform` | 均匀随机输入 | `[-100,100]` |
| `normal` | 正态分布及边界裁剪 | `[-100,100]` |
| `boundary` | ±100、±1、±0.5、0及抵消组合 | `[-100,100]` |
| `bias_only` | A/B全零，验证Bias广播 | `[-100,100]` |
| `non_aligned_tail` | 验证最后有效M/K/N坐标和非对齐尾部 | `[-2,3.25]` |
| `zeros` | 全零边界输入 | `0` |

精度判定标准：

- `rtol = 2^-10`；
- `atol = 2^-16`；
- `matched_ratio >= 0.99`；
- `max_abs_error <= max(1e-2, 32 ULP)`；
- 输出不得包含 NaN 或 Inf。

### 性能测试

任务书明确性能要求为“无”，因此不设置性能测试 case，性能数据记为不适用。

## 测试产物

执行测试后生成：

```text
build/demo                       # 可执行文件
input/                           # 当前case输入
output/output.bin                # NPU输出
output/golden.bin                # CPU Golden
output/cases/*_manifest.json     # case参数
output/cases/*_report.json       # 精度结果
```

`build/`、`input/` 和 `output/` 均为运行产物，不提交到 Git。

## 目录说明

```text
matmul/
├── CMakeLists.txt
├── matmul.asc
├── data_utils.h
├── run.sh
├── scripts/
│   ├── gen_data.py
│   ├── run_cases.py
│   └── verify_result.py
├── figures/
│   └── validation_summary.png   # 验收截图
├── SELF_TEST_REPORT.md
├── README.md
└── README_en.md
```

完整测试参数和实测结果见 [SELF_TEST_REPORT.md](./SELF_TEST_REPORT.md)。开发中发现的 CANN 版本范围歧义见 [asc-devkit #1404](https://gitcode.com/cann/asc-devkit/issues/1404)。
