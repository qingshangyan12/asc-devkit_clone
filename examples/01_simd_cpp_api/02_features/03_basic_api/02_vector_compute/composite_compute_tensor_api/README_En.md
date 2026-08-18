# Composite Compute Sample Implemented with Tensor API

## Overview

This sample uses Tensor API to run composite compute on two 64-element `float` inputs and writes five result rows to GM. Python scripts generate NumPy golden data and verify the output element by element.

## Supported Products

- Ascend 950PR
- Ascend 950DT

## Directory Structure

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

## Sample Description

| Input/Output | Specification |
| :--- | :--- |
| `src_a`, `src_b` | `[64]`, `float` |
| `output` | `[5, 64]`, `float` |

| API | Expression |
| :--- | :--- |
| `axpy` | `src_b + 0.25 * src_a` |
| `absdiff` | `abs(src_a - src_b)` |
| `exp_sub` | `exp(src_a - src_b)` |
| `fma` | `src_a * src_b + src_b` |
| `muls_cast` | `half(src_a * 1.5)` |

`muls_cast` returns `half`; the sample converts it to `float` only for common output storage. `exp_sub` uses `rtol=5e-3`, `atol=5e-3`; other results use `rtol=1e-4`, `atol=1e-4`.

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
composite_compute_tensor_api executed successfully.
test pass! max absolute error: ...
```
