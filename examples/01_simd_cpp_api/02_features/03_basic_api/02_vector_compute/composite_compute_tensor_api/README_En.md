# Composite Compute Sample Implemented with Tensor API

## Overview

This sample demonstrates composite register compute with Tensor API. Composite APIs express operations such as multiply-add, absolute difference, exponential difference, and multiply-cast directly, so callers do not have to manage intermediate registers for each elementary operation.

Two `float` vectors are loaded as `reg_tensor<float>` values. The sample calls six composite compute APIs and stores each result in a separate output row.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
composite_compute_tensor_api
├── CMakeLists.txt
├── README.md
├── README_En.md
├── composite_compute_tensor_api.asc
└── scripts
    ├── gen_data.py
    └── verify_result.py
```

## Sample Description

The input contains two vectors of 64 `float` elements. The sample produces six result rows:

| API | Computation in This Sample |
| --- | --- |
| `axpy` | `src_b + 0.25 * src_a` |
| `abs_sub` | `abs(src_a - src_b)` |
| `exp_sub` | `exp(src_a - src_b)` |
| `mul_dst_add` | `src_a * src_b + src_b` |
| `mul_add_dst` | `src_a + src_b * src_b` |
| `muls_cast` | Multiply `float` by 1.5, cast to `half`, and convert back to `float` for uniform storage and verification |

The kernel copies data from GM to UB, loads registers and calls the composite APIs in `compute`, stores the results, and copies the output back to GM. `gen_data.py` creates the inputs and NumPy golden data. `verify_result.py` performs element-wise accuracy checks.

`exp_sub` is checked with `rtol=5e-3` and `atol=5e-3`. Other results are checked with `rtol=1e-4` and `atol=1e-4`.

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

A successful run prints output similar to:

```text
composite_compute_tensor_api executed successfully.
test pass! max absolute error: ...
```
