# 基于Tensor API实现的复合计算样例

## 概述

本样例使用Tensor API对两组64元素`float`输入执行复合计算，并将5组结果写回GM。输入和真值由Python脚本生成，输出使用NumPy逐元素校验。

## 支持产品

- Ascend 950PR
- Ascend 950DT

## 目录结构

```text
composite_compute_tensor_api
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── composite_compute_tensor_api.asc
├── README.md
└── README_En.md
```

## 样例说明

| 输入/输出 | 规格 |
| :--- | :--- |
| `src_a`、`src_b` | `[64]`，`float` |
| `output` | `[5, 64]`，`float` |

| 输出行 | 接口 | 样例表达式 |
| :--- | :--- | :--- |
| 0 | `axpy` | `src_b + 0.25 * src_a` |
| 1 | `absdiff` | `abs(src_a - src_b)` |
| 2 | `exp_sub` | `exp(src_a - src_b)` |
| 3 | `fma` | `src_a * src_b + src_b` |
| 4 | `muls_cast` | `half(src_a * 1.5)` |

`muls_cast`的接口结果是`half`；样例仅为统一输出文件格式，将其转换为`float`后保存。`exp_sub`按`rtol=5e-3`、`atol=5e-3`校验，其他结果按`rtol=1e-4`、`atol=1e-4`校验。

## 编译运行

在本目录执行：

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

成功时输出：

```text
composite_compute_tensor_api executed successfully.
test pass! max absolute error: ...
```
