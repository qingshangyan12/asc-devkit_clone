# cast Example

## Overview

This example uses the Memory vector compute interfaces [asc_half2int4](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md) and [asc_half2int32](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md) to convert half data with round-up semantics.

This example supports two conversion scenarios, selected with the CMake build parameter `SCENARIO_NUM`.

| `SCENARIO_NUM` | Data type conversion |
| --- | --- |
| `0` | half to packed int4 |
| `1` | half to int32 |

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Atlas A2 Training/Inference and Atlas A3 Training/Inference | >= CANN 9.2.0 |

## Directory Structure

```text
cast/
├── scripts/gen_data.py       # Input and golden-data generator
├── CMakeLists.txt            # Build configuration
├── data_utils.h              # Binary-data reader
├── cast.asc                  # C API kernel and host invocation
├── README.md
└── README_en.md
```

## Example Description

This example loads 512 half input values from GM to UB, converts the data with round-up semantics, and stores the result back to GM. The host launches `cast_kernel` with `<<<>>>` and compares every output byte with `output/golden.bin`.

**Scenario 0: half to packed int4**

- Example functionality: Converts half data to round-up int4b_t data and packs it into uint8_t output.
- Example specifications:

  | Item | Specification |
  | --- | --- |
  | Example type (OpType) | AIV example |
  | Example input | `x`, [1, 512], half |
  | Example output | `y`, [1, 512], int4b_t, two values packed into one uint8_t |
  | Kernel function name | `cast_kernel` |

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load 512 half values from GM to UB.
  - Compute: Calls `asc_half2int4_ru` with CEIL rounding and saturates the results to [-8, 7].
  - Store: Calls `asc_copy_ub2gm_align` to store the 256-byte packed result from UB to GM.

**Scenario 1: half to int32**

- Example functionality: Converts half data to round-up int32_t data.
- Example specifications:

  | Item | Specification |
  | --- | --- |
  | Example type (OpType) | AIV example |
  | Example input | `x`, [1, 512], half |
  | Example output | `y`, [1, 512], int32_t |
  | Kernel function name | `cast_kernel` |

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load 512 half values from GM to UB.
  - Compute: Calls `asc_half2int32_ru` with CEIL rounding to produce 512 int32_t values.
  - Store: Calls `asc_copy_ub2gm_align` to store the 2048-byte result from UB to GM.

The generator uses deterministic half inputs that include positive, negative, and saturation-boundary values to generate golden data.

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
SCENARIO_NUM=0                                                                                  # Run scenario 0
mkdir -p build && cd build;                                                                       # Create and enter the build directory
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Build the project (default NPU mode)
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # Generate test input and golden data
./demo                                                                                           # Run the compiled executable
```

- Build options

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU architecture: Atlas A2 Training/Inference and Atlas A3 Training/Inference |
| `SCENARIO_NUM` | `0`, `1` | Example scenario: half to packed int4, half to int32 |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
