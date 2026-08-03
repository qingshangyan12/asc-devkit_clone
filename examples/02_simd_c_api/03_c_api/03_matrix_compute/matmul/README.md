# MatMul C API 样例

## 概述

本样例使用 Ascend C C API 在单个 Cube Core 上完成带偏置的 FP32 矩阵乘法：

```text
dst[i, j] = sum(src0[i, k] * src1[k, j]) + weight[j]
```

| 参数 | 数据类型 | Shape | 格式 | 转置 |
|------|----------|-------|------|------|
| `src0` | float32 | `[30, 40]` | ND | 否 |
| `src1` | float32 | `[40, 70]` | ND | 否 |
| `weight` | float32 | `[1, 70]` | ND | - |
| `dst` | float32 | `[30, 70]` | ND | - |

核函数仅启动一个 Cube Core，不使用 Host 侧逐元素计算，也不需要 Tiling 分块。

## 实现说明

数据流程为：

```text
GM(ND) -> L1(Nz) -> L0A/L0B -> Cube Mmad(+BiasTable) -> L0C -> GM(ND)
```

- `src0` 通过 `asc_copy_gm2l1_nd2nz` 和 `asc_copy_l12l0a` 进入 L0A。
- `src1` 先转为 L1 Nz 排布，再通过 `asc_copy_l12l0b_transpose` 得到 L0B 所需排布；GM 中输入保持题目规定的 `[40,70]` ND。
- `weight` 补齐到 320 字节后通过 `asc_copy_l12bt` 进入 BiasTable。
- `asc_set_fp32_mode` 启用 FP32 计算模式，`asc_mmad` 完成 Cube 矩阵乘加。
- `asc_copy_l0c2gm` 将 L0C 结果转回 ND 并仅写出有效的 `30 * 70` 个元素。

本规格的 Local Memory 使用量为：L1 23872 字节，L0A 5120 字节，L0B 15360 字节，L0C 10240 字节，均在硬件容量范围内。

## 支持的产品及版本

| 产品 | CANN 版本 |
|------|-----------|
| Ascend 950PR / Ascend 950DT | >= 9.1.0 |

## 目录结构

```text
matmul/
├── CMakeLists.txt
├── README.md
├── README_en.md
├── TEST_REPORT.md
├── data_utils.h
├── matmul.asc
└── scripts/
    ├── gen_data.py
    └── verify_result.py
```

## 编译运行

1. 配置 CANN 环境：

   ```bash
   source ${install_path}/cann/set_env.sh
   ```

2. 在本目录下编译、生成数据并运行：

   ```bash
   mkdir -p build && cd build
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
   make -j
   python3 ../scripts/gen_data.py
   ./demo
   python3 ../scripts/verify_result.py output/output.bin output/golden.bin
   ```

使用 NPU 仿真模式时，在 CMake 命令中增加 `-DCMAKE_ASC_RUN_MODE=sim`。精度验证通过时输出 `test pass!`。

## 测试用例

| 用例 | 输入范围 | 预期结果 |
|------|----------|----------|
| `random` | `src0/src1/weight` 均为 `[-100, 100]` | 固定种子随机精度、正负值及尾块 |
| `ones` | 所有输入均为 1 | 结果所有元素均为 41，检查 Bias 广播 |
| `zero_bias` | A/B 为固定种子随机值，Bias 为 0 | 检查纯矩阵乘法路径 |

验证脚本按生态算子 FLOAT32 精度标准检查输出元素数、NaN/Inf，并以 `rtol=2^-10`、`atol=2^-16` 判定单点精度；要求匹配率不低于 99%，且最大绝对误差满足 `1e-2` 或 32 ULP 硬上限。
数据生成和验证脚本仅使用 Python 标准库，无需安装 NumPy。

可通过 `python3 ../scripts/gen_data.py --case ones` 或 `--case zero_bias` 生成其他用例；每次生成后依次运行 `./demo` 和验证脚本。

完整的 Ascend 950 实机测试环境、精度数据、耗时说明和复现命令见 [TEST_REPORT.md](TEST_REPORT.md)。
