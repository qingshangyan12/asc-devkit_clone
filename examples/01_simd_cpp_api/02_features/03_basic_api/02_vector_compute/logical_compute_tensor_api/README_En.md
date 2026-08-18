# Logical Compute Sample Implemented with Tensor API

## Overview

This sample uses Tensor API to run logical and shift operations on 64 `int32_t` elements and writes eight result rows to GM. Python scripts generate golden data and require exact element-wise results.

## Supported Products

- Ascend 950PR
- Ascend 950DT

## Directory Structure

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

## Sample Description

| Input/Output | Specification |
| :--- | :--- |
| `src_a`, `src_b`, `shifts` | `[64]`, `int32_t` |
| `output` | `[8, 64]`, `int32_t` |

The sample evaluates `!a`, `a & b`, `a | 16`, `7 ^ a`, `a << shift`, `a << 2`, `a >> shift`, and `a >> 2`. `!a` is logical not: zero becomes 1 and nonzero becomes 0. Generated register shift counts range from 0 to 3.

## Build and Run

Run the following in this directory:

```bash
source /usr/local/Ascend/cann/set_env.sh
cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
cmake --build build -j
cd build
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

A successful run prints:

```text
logical_compute_tensor_api executed successfully.
test pass!
```
