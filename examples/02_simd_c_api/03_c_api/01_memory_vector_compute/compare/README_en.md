# compare Example

## Overview

This Memory vector compute sample uses [asc_lt](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt.md), [asc_lt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt_scalar.md), [asc_gt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_gt_scalar.md), and `asc_get_cmp_mask` to demonstrate little-endian packed comparison bits.

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Atlas A2 Training/Inference and Atlas A3 Training/Inference | >= CANN 9.2.0 |

## Directory Structure

```text
compare/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── compare.asc
├── README.md
└── README_en.md
```

## Scenarios

Each scenario takes 64 float values and writes its packed comparison result to the first 8 output bytes in little-endian bit order.

| `SCENARIO_NUM` | Comparison |
| --- | --- |
| 1 | `x[i] < y[i]` |
| 2 | `x[i] < y[i]`, then reads the CmpMask result |
| 3 | `x[i] < y[0]` |
| 4 | `x[i] > y[0]` |

## Build and Run

Run the following steps in the root directory of this example.

- Configure environment variables

  Configure environment variables according to the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install).

```bash
source ${install_path}/cann/set_env.sh
```

  > **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for a non-root user.

- NPU execution

```bash
SCENARIO_NUM=1                                                                                  # Run scenario 1
mkdir -p build && cd build;                                                                       # Create and enter the build directory
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Build the project (default NPU mode)
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # Generate test input and golden data
./demo                                                                                           # Run the compiled executable
```

- NPU simulation

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
```

Clear the CMake cache before changing a run mode or scenario. For example, run `rm CMakeCache.txt` in the `build` directory and configure again. The executable exits nonzero when a required ACL operation fails and never accepts output from an unexecuted kernel.

- Build options

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution or NPU simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU architecture: Atlas A2 Training/Inference and Atlas A3 Training/Inference |
| `SCENARIO_NUM` | `1`, `2`, `3`, `4` | Example scenario: vector comparison, CmpMask read, vector-scalar comparison, reverse vector-scalar comparison |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
