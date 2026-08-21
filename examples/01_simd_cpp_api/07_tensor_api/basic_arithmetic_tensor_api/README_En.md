# Basic Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates register-based basic compute with Tensor API. It copies two `float` input vectors, one PReLU slope vector, and two `uint32_t` input vectors from Global Memory to Unified Buffer and loads them as masked `reg_tensor` values. It produces 28 floating-point results covering unary, activation, and register/scalar binary forms, plus 10 integer results covering carry, borrow, and wide multiplication.

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

Each input contains 64 elements. The floating-point section evaluates 28 results: 10 unary or activation results and 18 binary results covering the `reg-reg`, `reg-scalar`, and `scalar-reg` operand forms. The integer section evaluates 10 `uint32_t` results from `add`, `add_c`, `sub`, `sub_c`, and `mull`.

The sample performs these steps:

1. Copy the three floating-point inputs and two `uint32_t` inputs from Global Memory to Unified Buffer.
2. Load register values from Tensor objects and attach an all-active mask.
3. Call each basic compute API form and store its result in one output row.
4. Copy 28 float rows and 10 `uint32_t` rows back to Global Memory and compare them with NumPy golden data.

The result rows are organized as follows:

| Rows | APIs | Operand forms |
| :--- | :--- | :------------ |
| 0-9 | `abs`, `exp`, `sqrt`, `log`, `log2`, `log10`, unary `-`, `relu`, `prelu`, `leaky_relu` | Unary input, register slope, or scalar slope as required |
| 10-12 | `operator+` | `reg-reg`, `reg-scalar`, `scalar-reg` |
| 13-15 | Binary `operator-` | `reg-reg`, `reg-scalar`, `scalar-reg` |
| 16-18 | `operator*` | `reg-reg`, `reg-scalar`, `scalar-reg` |
| 19-21 | `operator/` | `reg-reg`, `reg-scalar`, `scalar-reg` |
| 22-24 | `max` | `reg-reg`, `reg-scalar`, `scalar-reg` |
| 25-27 | `min` | `reg-reg`, `reg-scalar`, `scalar-reg` |

Inputs to `sqrt`, `log`, `log2`, and `log10` are transformed to `abs(src_a) + 0.5` to stay in their mathematical domains. All divisors are positive and nonzero.

The `uint32_t` rows are organized as follows. Carry predicates are converted to `0` or `1` so they can be saved and checked exactly.

| Rows | API | Results |
| :--- | :-- | :------ |
| 0-1 | `add` | Low 32 bits and carry-out |
| 2-3 | `add_c` | Low 32 bits and carry-out, using the preceding `add` carry as input |
| 4-5 | `sub` | Low 32 bits and carry-out; `1` means no borrow |
| 6-7 | `sub_c` | Low 32 bits and carry-out, using the preceding `sub` carry as input |
| 8-9 | `mull` | Low and high 32 bits of the 64-bit product |

Even and odd lanes are initialized with different integer patterns so that both carry/no-carry and borrow/no-borrow cases are exercised.

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
  python3 ../scripts/verify_result.py \
    output/output.bin output/golden.bin \
    output/uint32_output.bin output/uint32_golden.bin
  ```

The verification script checks file sizes and reports the API expression, row, lane, actual value, and golden value for the first mismatch. A successful run prints output similar to:

```text
basic_arithmetic_tensor_api executed successfully.
test pass! verified 38 result rows (2432 elements).
max float absolute error: ...; uint32 results matched exactly.
```
