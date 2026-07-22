# Reg Load Gather Sample

## Overview

This sample uses Ascend C API register data-movement interfaces to load non-contiguous UB data into vector registers, store the results in UB, and copy them to GM. Select a scenario with the CMake parameter `SCENARIO_NUM`:

| `SCENARIO_NUM` | Load Scenario |
| --- | --- |
| 1 | Uses `asc_gather` for element-indexed loads. |
| 2 | Uses `asc_gather_datablock` for 32B DataBlock-indexed loads. |

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
reg_load_gather/
├── scripts/
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output data
├── CMakeLists.txt                 // Build configuration
├── data_utils.h                   // Input and output file utilities
├── reg_load_gather.asc            // Ascend C API implementation and entry
├── README.md                      // Chinese documentation
└── README_en.md                   // English documentation
```

## Detailed Scenario Description

Both scenarios run on one AIV core. MTE2 first copies the input from GM to UB. The entry function invokes the SIMD VF function through `asc_vf_call` to perform the register Gather operation. The VF function processes the data in chunks of one vector-register width. After Vector processing, MTE3 copies the result from UB to GM, and `asc_sync_pipe(PIPE_ALL)` at the end of the kernel completes all pipeline operations.

### Scenario 1: Element-Indexed Gather

- `asc_loadalign` loads 128 `uint16_t` indexes into `vector_uint16_t`.
- `asc_gather` performs element-indexed loads, producing `dst[i] = src[index[i]]`.
- `asc_storealign` stores the `vector_half` result in UB.

| Item | Name | Shape | Data Type |
| --- | --- | --- | --- |
| Sample type | AIV sample | - | - |
| Input | `src` | `[1, 1024]` | `half` |
| Input | `index` | `[1, 128]` | `uint16_t` |
| Output | `dst` | `[1, 128]` | `half` |
| Kernel function | `reg_load_gather` | - | - |

Each index is an element offset relative to the start of `src`, with a valid range of `[0, 1023]`.

### Scenario 2: DataBlock Gather

- One DataBlock is 32B, which contains 16 `half` elements.
- `asc_loadalign` loads eight `uint32_t` indexes into `vector_uint32_t`.
- `asc_gather_datablock` loads eight DataBlocks, and `asc_storealign` stores 128 contiguous `half` results.

| Item | Name | Shape | Data Type |
| --- | --- | --- | --- |
| Sample type | AIV sample | - | - |
| Input | `src` | `[1, 1024]` | `half` |
| Input | `index` | `[1, 8]` | `uint32_t` |
| Output | `dst` | `[1, 128]` | `half` |
| Kernel function | `reg_load_gather` | - | - |

Each `index` value is a byte offset relative to the start of `src` and must be a multiple of 32B. The data generation script writes indexes in `[0, 2016]`, selecting from the 64 DataBlocks that represent 1024 `half` elements.

## Build and Run

### Configure Environment Variables

Configure environment variables according to the CANN toolkit installation in the current environment:

```bash
source ${install_path}/cann/set_env.sh
```

> **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for non-root users.

### Run the Sample

Run the following commands in the sample root directory. `SCENARIO_NUM` can be 1 or 2 and defaults to 1.

```bash
SCENARIO_NUM=1                                                                    # Select scenario 1
mkdir -p build && cd build;                                                        # Create and enter the build directory
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # Build the project; NPU mode by default
python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM                         # Generate test input and golden data
./demo                                                                             # Run the example executable
python3 ../scripts/verify_result.py output/output.bin output/golden.bin           # Verify the result
```

The default mode is NPU execution. To use NPU simulation, add `-DCMAKE_ASC_RUN_MODE=sim`:

```bash
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
```

Before changing the scenario or run mode, delete `CMakeCache.txt` in the `build` directory and rerun CMake to avoid using cached configuration.

### Build Options

| Option | Valid Values | Description |
| --- | --- | --- |
| `SCENARIO_NUM` | `1`, `2` | Selects element-indexed or DataBlock-indexed load. |
| `CMAKE_ASC_RUN_MODE` | `npu`, `sim` | Selects NPU execution or NPU simulation mode. |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Specifies the NPU architecture for Ascend 950PR/Ascend 950DT. |

## Expected Result

When the output matches the golden data, the verification script prints:

```bash
test pass!
```
