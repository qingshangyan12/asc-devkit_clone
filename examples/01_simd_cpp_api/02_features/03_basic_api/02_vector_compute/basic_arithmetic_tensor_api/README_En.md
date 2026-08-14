# Basic Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates register-based basic compute with Tensor API. It copies two `float` input vectors and one PReLU slope vector from Global Memory to Unified Buffer, loads them as masked `reg_tensor<float>` values, applies unary math, activation, and binary arithmetic operations, and writes 16 result rows back to Global Memory.

The sample focuses on connecting Tensor `load/store` operations with value-returning register compute APIs. The mask is stored in each `reg_tensor` and propagated through the compute chain.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
basic_arithmetic_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── basic_arithmetic_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## Sample Description

The input contains 64 `float` elements. The `compute` function evaluates 16 element-wise operations: `abs`, `exp`, `sqrt`, `log`, `log2`, `log10`, unary minus, `relu`, `prelu`, `leaky_relu`, addition, subtraction, multiplication, division, `max`, and `min`.

The sample performs these steps:

1. Copy `src_a`, `src_b`, and `slope` from Global Memory to Unified Buffer.
2. Load register values from Tensor objects and attach an all-active mask.
3. Call each basic compute API and store its result in one output row.
4. Copy all 16 rows back to Global Memory and compare them with NumPy golden data.

Math functions are checked with `rtol=5e-3` and `atol=5e-3`. Other results are checked with `rtol=1e-4` and `atol=1e-4`.

## Build and Run

Run the following steps in the sample directory.

- Configure the environment for the installed CANN toolkit.

  Default path for the root user:

  ```bash
  source /usr/local/Ascend/cann/set_env.sh
  ```

  Default path for a non-root user:

  ```bash
  source $HOME/Ascend/cann/set_env.sh
  ```

  Custom installation path:

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Build and run the sample.

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510
  cmake --build build -j
  cd build
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin
  ```

A successful run prints output similar to:

```text
basic_arithmetic_tensor_api executed successfully.
test pass! max absolute error: ...
```
