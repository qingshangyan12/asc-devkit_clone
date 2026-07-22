# Reg Load Store Mask Sample

## Overview

This sample demonstrates loading, storing, and using MaskReg with Ascend C API. Scenario 1 loads MaskReg from UB, performs masked duplication, and stores MaskReg. Scenario 2 uses MaskReg bits to select elements from two `float` inputs.

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
reg_load_store_mask/
├── scripts/
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output data
├── CMakeLists.txt                 // Build configuration
├── data_utils.h                   // Input and output file utilities
├── reg_load_store_mask.asc        // Ascend C API implementation and entry
├── README.md                      // Chinese documentation
├── README_en.md                   // English documentation
└── figures/
    └── reg_load_store_mask.png    // Scenario diagram
```

![MaskReg operation diagram](./figures/reg_load_store_mask.png)

## Detailed Scenario Description

Both scenarios run on one AIV core. MTE2 first copies input data from GM to UB. The entry function invokes SIMD VF functions through `asc_vf_call` to load or generate MaskReg and complete the register operation. Finally, MTE3 copies the result from UB to GM, and `asc_sync_pipe(PIPE_ALL)` at the end of the kernel completes all pipeline operations.

| Scenario | Data Type and Length | Description |
| --- | --- | --- |
| 1 | `uint8_t[1024]` | Loads MaskReg from the first 32B of input, writes the value 2 at masked positions, and stores MaskReg starting from byte 256 of the output. |
| 2 | `float[256]` with `uint8_t[128]` mask | Processes 64 `float` elements at a time, selecting from two inputs according to MaskReg bits. |

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
| `SCENARIO_NUM` | `1`, `2` | Selects the MaskReg load, store, or selection scenario. |
| `CMAKE_ASC_RUN_MODE` | `npu`, `sim` | Selects NPU execution or NPU simulation mode. |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | Specifies the NPU architecture for Ascend 950PR/Ascend 950DT. |

## Expected Result

When the output matches the golden data, the verification script prints:

```bash
test pass!
```
