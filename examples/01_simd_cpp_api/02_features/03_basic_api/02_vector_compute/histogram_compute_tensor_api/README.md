# 基于Tensor API实现的直方图计算样例

## 概述

本样例使用Tensor API对256个`uint8_t`元素执行直方图计算，并将4组统计结果写回GM。输入和真值由Python脚本生成，输出逐元素精确校验。

## 支持产品

- Ascend 950PR
- Ascend 950DT

## 目录结构

```text
histogram_compute_tensor_api
├── scripts
│   ├── gen_data.py
│   └── verify_result.py
├── CMakeLists.txt
├── histogram_compute_tensor_api.asc
├── README.md
└── README_En.md
```

## 样例说明

| 输入/输出 | 规格 |
| :--- | :--- |
| `input` | `[256]`，`uint8_t` |
| `output` | `[4, 128]`，`uint16_t` |

目的寄存器初值为3，统计结果会累加到该初值上。

| 输出行 | 配置 | 统计范围 |
| :--- | :--- | :--- |
| 0 | `low + frequency` | 0到127的频次 |
| 1 | `high + frequency` | 128到255的频次 |
| 2 | `low + cumulative` | 0到127的累计频次 |
| 3 | `high + cumulative` | 128到255的累计频次 |

所有`uint16_t`结果均要求与真值完全一致。

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
histogram_compute_tensor_api executed successfully.
test pass!
```
