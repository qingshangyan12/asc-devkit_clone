# Basic Compute Sample Implemented with Tensor API

## Overview

This sample uses Tensor API to run basic register compute on 64 `float` elements and writes 16 result rows to GM. Python scripts generate the input and NumPy golden data, then verify results element by element.

## Supported Products

- Ascend 950PR
- Ascend 950DT

## Directory Structure

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

## Sample Description

| Input/Output | Specification |
| :--- | :--- |
| `src_a`, `src_b`, `slope` | `[64]`, `float` |
| `output` | `[16, 64]`, `float` |

The sample covers `abs`, `exp`, `sqrt`, `log`, `log2`, `log10`, unary minus, `relu`, `prelu`, `leaky_relu`, `+`, binary minus, `*`, `/`, `max`, and `min`.

`sqrt`, `log`, `log2`, and `log10` use `abs(src_a) + 0.5` as input. Math functions use `rtol=5e-3`, `atol=5e-3`; other results use `rtol=1e-4`, `atol=1e-4`.

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
basic_arithmetic_tensor_api executed successfully.
test pass! max absolute error: ...
```
