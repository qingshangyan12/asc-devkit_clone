# MatMul Example

## Overview

This example demonstrates how to implement a fixed-shape FP32 matrix multiply-add on one Cube Core through the Ascend C C API:

```text
C[30, 70] = A[30, 40] x B[40, 70] + bias[1, 70]
```

Matrices A and B are not transposed. The Host handles data I/O, memory management, kernel launch, and synchronization. Matrix multiplication and bias addition are performed entirely on the Device.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|--------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── matmul
│   ├── scripts
│   │   ├── gen_data.py             // Input and golden data generation script
│   │   └── verify_result.py        // Accuracy verification script
│   ├── CMakeLists.txt              // Build project file
│   ├── data_utils.h                // Data read/write functions
│   ├── matmul.asc                  // Kernel implementation and Host-side main function
│   ├── README_en.md                // English example documentation
│   └── README.md                   // Chinese example documentation
```

## Example Description

The input and output specifications are shown below.

| Parameter | Type | Data Type | Shape | Transposed |
|-----------|------|-----------|-------|------------|
| `src0` | Input | float | `[30, 40]` | false |
| `src1` | Input | float | `[40, 70]` | false |
| `weight` | Input | float | `[1, 70]` | - |
| `dst` | Output | float | `[30, 70]` | - |

The complete data processing flow is as follows:

1. Use `asc_copy_gm2l1_nd2nz` and `asc_copy_gm2l1_dn2nz` to transfer matrices A and B from Global Memory to the L1 Buffer and convert the layout from ND to Nz.
2. Use `asc_copy_l12l0a` and `asc_copy_l12l0b` to transfer matrices A and B to the L0A Buffer and L0B Buffer.
3. Use `asc_copy_l12bt` to transfer bias from the L1 Buffer to the BiasTable Buffer.
4. Use `asc_mmad` to perform matrix multiply-add and write the result to the L0C Buffer.
5. Use `asc_copy_l0c2gm` to transfer the result from the L0C Buffer to Global Memory and convert the layout from Nz to ND.

The kernel launches one Cube Core. Local Memory for matrix A, matrix B, bias, and the output matrix is aligned to fractal requirements. The source uses `static_assert` to check the capacity of each Buffer.

## Accuracy Test Cases

All cases use the fixed shapes above and input values in the range `[-100, 100]`.

| Case | Input Construction | Verification Target |
|------|--------------------|---------------------|
| P01 | A and B are zero; bias is a linear sequence | Bias loading and broadcasting |
| P02 | A, B, and bias are zero | Zero values and output initialization |
| P03 | Periodic `-100`, `100`, and `0` values | Input boundaries and signed multiply-add |
| P04 | Paired positive and negative values | Cancellation behavior |
| P05 | Sparse pattern in B | Matrix B layout and direction |
| P06 | Fixed random seed 20260804 | Random accuracy |
| P07 | Fixed random seed 20260805 | Random accuracy |
| P08 | Fixed random seed 20260806 | Random accuracy |

Golden data is calculated with NumPy float64. According to the FP32 accuracy criteria, the verification script requires a matching ratio of at least 0.99 and requires each element to have either an absolute error no greater than `1e-2` or a ULP error no greater than 32.

## Build and Run

Run the following steps in the root directory of this example.

- Configure the environment and Python dependency

  ```bash
  source ${install_path}/cann/set_env.sh
  python3 -m pip install numpy
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. The default path is `/usr/local/Ascend` for the root user and `$HOME/Ascend` for a non-root user.

- Build the example

  ```bash
  cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=npu
  cmake --build build -j
  ```

  For NPU simulation mode, set `CMAKE_ASC_RUN_MODE` to `sim` and use a separate build directory:

  ```bash
  cmake -S . -B build_sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim
  cmake --build build_sim -j
  ```

- Run one accuracy case

  ```bash
  python3 scripts/gen_data.py --case P01 --output-dir input
  ./build/demo
  python3 scripts/verify_result.py output/output.bin input/golden.bin
  ```

- Run all accuracy cases

  ```bash
  for case_name in P01 P02 P03 P04 P05 P06 P07 P08; do
      python3 scripts/gen_data.py --case ${case_name} --output-dir input || exit 1
      ./build/demo || exit 1
      python3 scripts/verify_result.py output/output.bin input/golden.bin || exit 1
  done
  ```

- Build options

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | NPU execution mode or NPU simulation mode |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture for Ascend 950PR/Ascend 950DT |

- Execution result

  Successful accuracy verification includes the following output:

  ```text
  matched_ratio=1.000000
  status=PASSED
  ```
