# asc_copy_l0c2ub Example

## Overview

This example demonstrates the Ascend C C API `asc_copy_l0c2ub`. After matrix multiplication produces FP32 data in L0C Buffer, Fixpipe applies scalar quantization and writes packed signed int4 data to Unified Buffer (UB). An AIV then copies the UB output to GM.

The example supports Ascend 950PR and Ascend 950DT only (`dav-3510`) and requires CANN 9.1.0 or later.

## Directory Structure

```text
asc_copy_l0c2ub
├── scripts
│   ├── gen_data.py          # Generates inputs and golden output
│   └── verify_result.py     # Compares output with golden data
├── asc_copy_l0c2ub.asc      # C API implementation and host launch
├── CMakeLists.txt           # CMake build file
├── data_utils.h             # Binary input/output helpers
├── README.md                # Chinese documentation
└── README_en.md             # English documentation
```

## Data Description

The input matrices are `A[16, 16]` and `B[16, 64]`, both using `half`. The generator creates an identity matrix for A and fills B with 3, so every matrix multiplication result is 3.

`asc_set_l0c_copy_prequant` configures scalar pre-quantization. `asc_copy_l0c2ub` uses `QF322S4_PRE` to quantize results to signed int4. Two int4 elements are packed into one byte, so the output is `16 * 64 / 2 = 512` bytes. Every golden byte is `0x33`.

## Build and Run

Run the following commands from this example directory.

1. Configure the CANN environment.

   ```bash
   source ${install_path}/cann/set_env.sh
   ```

2. Build, generate data, run, and validate.

   ```bash
   mkdir -p build && cd build
   cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
   make -j
   python3 ../scripts/gen_data.py
   ./demo
   python3 ../scripts/verify_result.py output/output.bin output/golden.bin
   ```

3. Build for NPU simulation.

   ```bash
   cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
   make -j
   ```

   Remove `build/CMakeCache.txt` before switching run modes.

## Build Options

| Option | Values | Description |
|---|---|---|
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | NPU or NPU simulation mode |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Ascend 950PR / Ascend 950DT architecture |

The verifier prints `test pass!` when the output matches the golden data.
