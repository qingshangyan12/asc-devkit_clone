# compare Example

## Overview

This example uses the Memory vector compute interfaces [asc_lt](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_lt.md), [asc_gt_scalar](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_compare/asc_gt_scalar.md), and `asc_get_cmp_mask` to demonstrate little-endian packed comparison bits.

This example supports three comparison scenarios, selected with the CMake build parameter `SCENARIO_NUM`.

| `SCENARIO_NUM` | Comparison scenario |
| --- | --- |
| `1` | Vector comparison |
| `2` | CmpMask read |
| `3` | Vector-scalar comparison |

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

## Example Description

This example loads two input vectors of 64 float values from GM to UB and then compares them. Result bit `i` is stored at output bit `i`; the valid 8-byte result uses little-endian bit order. The kernel stores a 32-byte comparison-result buffer. The host launches `compare_kernel` with `<<<>>>` and validates the first 8 valid bytes one by one.

**Scenario 1: vector comparison**

- Example functionality: Compares `x[i] < y[i]` element by element and directly outputs the packed comparison result.
- Example specifications:

  | Item | Specification |
  | --- | --- |
  | Example type (OpType) | AIV example |
  | Example input | `x`, `y`, [1, 64], float |
  | Example output | `output`, [1, 8], uint8_t, 64 comparison results packed in little-endian bit order |
  | Kernel function name | `compare_kernel` |

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load `x` and `y` from GM to UB.
  - Compute: Calls the output-parameter overload of `asc_lt` to compare the vectors and write the packed result to the UB output buffer.
  - Store: Calls `asc_copy_ub2gm_align` to store the 32-byte output buffer to GM; the host validates the first 8 valid bytes.

**Scenario 2: CmpMask read**

- Example functionality: Compares `x[i] < y[i]` element by element and obtains the comparison result through CmpMask.
- Example specifications:

  | Item | Specification |
  | --- | --- |
  | Example type (OpType) | AIV example |
  | Example input | `x`, `y`, [1, 64], float |
  | Example output | `output`, [1, 8], uint8_t, 64 comparison results packed in little-endian bit order |
  | Kernel function name | `compare_kernel` |

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load `x` and `y` from GM to UB.
  - Compute: Calls the output-less `asc_lt` to generate CmpMask, then calls `asc_get_cmp_mask` to write CmpMask to the UB output buffer.
  - Store: Calls `asc_copy_ub2gm_align` to store the 32-byte output buffer to GM; the host validates the first 8 valid bytes.

**Scenario 3: vector-scalar comparison**

- Example functionality: Compares `x[i] > y[0]` element by element, with `y[0]` used as a scalar.
- Example specifications:

  | Item | Specification |
  | --- | --- |
  | Example type (OpType) | AIV example |
  | Example input | `x`, `y`, [1, 64], float |
  | Example output | `output`, [1, 8], uint8_t, 64 comparison results packed in little-endian bit order |
  | Kernel function name | `compare_kernel` |

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load `x` and `y` from GM to UB.
  - Compute: Calls `asc_gt_scalar` to compare `x[i]` with scalar `y[0]` and write the packed result to the UB output buffer.
  - Store: Calls `asc_copy_ub2gm_align` to store the 32-byte output buffer to GM; the host validates the first 8 valid bytes.

The generator creates deterministic input and the corresponding 8-byte golden data for each scenario.

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

The executable exits nonzero when a required ACL operation fails and never accepts output from an unexecuted kernel.

- Build options

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU architecture: Atlas A2 Training/Inference and Atlas A3 Training/Inference |
| `SCENARIO_NUM` | `1`, `2`, `3` | Example scenario: vector comparison, CmpMask read, vector-scalar comparison |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
