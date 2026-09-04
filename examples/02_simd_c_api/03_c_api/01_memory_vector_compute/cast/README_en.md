# cast Example

## Overview

This Memory vector compute sample uses [asc_half2int4](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int4.md) and [asc_half2int32](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_convert/asc_half2int32.md) to convert half data with round-up semantics.

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

## Scenarios

| `SCENARIO_NUM` | Function | Input | Output |
| --- | --- | --- | --- |
| 0 | half to packed int4, CEIL rounding and saturation to [-8, 7] | 512 half values | 256 bytes |
| 1 | half to int32, CEIL rounding | 512 half values | 512 int32 values |

The generator creates deterministic boundary inputs and golden data. The executable reads `input/input_x.bin`, runs the kernel, and compares every output byte with `output/golden.bin`.

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

- NPU simulation

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
```

Clear the CMake cache before changing a run mode or scenario. For example, run `rm CMakeCache.txt` in the `build` directory and configure again.

- Build options

| Option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution or NPU simulation |
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` | NPU architecture: Atlas A2 Training/Inference and Atlas A3 Training/Inference |
| `SCENARIO_NUM` | `0`, `1` | Example scenario: half to packed int4, half to int32 |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
