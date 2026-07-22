# Reg Load Store Align Sample

## Overview

This sample demonstrates aligned UB-to-vector-register load and store interfaces in Ascend C API. The input contains two `half` arrays of length 1024. Vector computation adds the corresponding elements, while each scenario uses a different register load/store addressing or data-distribution mode.

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
reg_load_store_align/
├── scripts/
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output data
├── CMakeLists.txt                 // Build configuration
├── data_utils.h                   // Input and output file utilities
├── reg_load_store_align.asc       // Ascend C API implementation and entry
├── README.md                      // Chinese documentation
└── README_en.md                   // English documentation
```

## Detailed Scenario Description

All scenarios run on one AIV core. MTE2 first copies the input from GM to UB. The entry function invokes SIMD VF functions through `asc_vf_call` to process register-width chunks for loading, addition, and storage. Finally, MTE3 copies the result from UB to GM, and `asc_sync_pipe(PIPE_ALL)` at the end of the kernel completes all pipeline operations.

| Scenario | Register Load/Store Mode | Output Length | Description |
| --- | --- | --- | --- |
| 1 | Explicit offset | 1024 | Loads and stores 1021 elements with explicit offsets; the last three elements remain zero. |
| 2 | PostUpdate | 1024 | Uses PostUpdate mode to update addresses after each load and store. |
| 3 | AddrReg | 1024 | Uses an address register to configure the offset of each load and store. |
| 4 | DataBlock | 512 | Loads the first 16B of every 32B DataBlock and stores the data contiguously. |
| 5 | BRC B16 | 1024 | Broadcasts the first `half` element in each 256B segment to a vector. |
| 6 | Upsample B16 | 2048 | Repeats each input `half` element once before storing it. |

## Build and Run

### Configure Environment Variables

Configure environment variables according to the CANN toolkit installation in the current environment:

```bash
source ${install_path}/cann/set_env.sh
```

> **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for non-root users.

### Run the Sample

Run the following commands in the sample root directory. `SCENARIO_NUM` can be 1 through 6 and defaults to 1.

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
| `SCENARIO_NUM` | `1`-`6` | Selects the aligned register load/store scenario. |
| `CMAKE_ASC_RUN_MODE` | `npu`, `sim` | Selects NPU execution or NPU simulation mode. |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Specifies the NPU architecture for Ascend 950PR/Ascend 950DT. |

## Expected Result

When the output matches the golden data, the verification script prints:

```bash
test pass!
```
