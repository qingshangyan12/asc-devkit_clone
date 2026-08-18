# 基于Tensor API实现的基础计算样例

## 概述

本样例使用Tensor API对64个`float`元素执行基础寄存器计算，并将16组结果写回GM。输入和真值由Python脚本生成，输出使用NumPy逐元素校验。

## 支持产品

- Ascend 950PR
- Ascend 950DT

## 目录结构

```text
basic_arithmetic_tensor_api
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── basic_arithmetic_tensor_api.asc
├── README.md
└── README_En.md
```

## 样例说明

| 输入/输出 | 规格 |
| :--- | :--- |
| `src_a`、`src_b`、`slope` | `[64]`，`float` |
| `output` | `[16, 64]`，`float` |

样例覆盖以下接口：

| 输出行 | 接口 |
| :--- | :--- |
| 0-5 | `abs`、`exp`、`sqrt`、`log`、`log2`、`log10` |
| 6-9 | 一元`-`、`relu`、`prelu`、`leaky_relu` |
| 10-15 | `+`、二元`-`、`*`、`/`、`max`、`min` |

为保证数学函数输入有效，`sqrt`、`log`、`log2`和`log10`使用`abs(src_a) + 0.5`作为输入。数学函数按`rtol=5e-3`、`atol=5e-3`校验，其余结果按`rtol=1e-4`、`atol=1e-4`校验。

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
basic_arithmetic_tensor_api executed successfully.
test pass! max absolute error: ...
```
