# Logical Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates register-based logical compute with Tensor API. Input data is loaded as masked `reg_tensor<int32_t>` values. C++ operators perform logical not, bitwise AND, OR, XOR, and left/right shifts, and eight result rows are written back to Global Memory.

Both register and scalar operands are covered to show how a binary API inherits its active-lane mask from the `reg_tensor` operand.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
logical_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── logical_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## Sample Description

The input contains 64 `int32_t` elements. The output contains these eight rows:

| Operation | Operand Form |
| --- | --- |
| `!a` | Register logical not: zero becomes 1 and nonzero becomes 0 |
| `a & b` | Register and register |
| `a | 16` | Register and right scalar |
| `7 ^ a` | Left scalar and register |
| `a << shift` | Register shift count |
| `a << 2` | `int16_t` scalar shift count |
| `a >> shift` | Register shift count |
| `a >> 2` | `int16_t` scalar shift count |

The kernel copies data from GM to UB, loads Tensor values, executes register logical operations, and copies the output from UB to GM. The data generator performs the same operations with NumPy. Verification requires the output shape and every integer element to match exactly.

## Build and Run

Run the following steps in the sample directory.

- Configure the environment for the installed CANN toolkit.

  ```bash
  # Default path for the root user
  source /usr/local/Ascend/cann/set_env.sh

  # Default path for a non-root user
  source $HOME/Ascend/cann/set_env.sh

  # Custom installation path
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

A successful run prints:

```text
logical_compute_tensor_api executed successfully.
test pass!
```
