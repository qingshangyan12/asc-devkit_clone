# 基于Tensor API实现的逻辑计算样例

## 概述

本样例使用Tensor API对64个`int32_t`元素执行逻辑和移位计算，并将8组结果写回GM。输入和真值由Python脚本生成，输出逐元素精确校验。

## 支持产品

- Ascend 950PR
- Ascend 950DT

## 目录结构

```text
logical_compute_tensor_api
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── logical_compute_tensor_api.asc
├── README.md
└── README_En.md
```

## 样例说明

| 输入/输出 | 规格 |
| :--- | :--- |
| `src_a`、`src_b`、`shifts` | `[64]`，`int32_t` |
| `output` | `[8, 64]`，`int32_t` |

| 输出行 | 样例表达式 |
| :--- | :--- |
| 0-3 | `!a`、`a & b`、`a | 16`、`7 ^ a` |
| 4-7 | `a << shift`、`a << 2`、`a >> shift`、`a >> 2` |

`!a`表示逻辑非：输入为0时结果为1，否则结果为0。脚本将寄存器移位量限制在0到3，所有结果均要求与真值完全一致。

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
logical_compute_tensor_api executed successfully.
test pass!
```
