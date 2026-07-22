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
├── CMakeLists.txt                 # Build configuration
├── data_utils.h                   # Input and output file utilities
├── reg_load_store_mask.asc        # Ascend C API implementation
├── README.md                      # Chinese documentation
├── README_en.md                   # English documentation
├── figures/
│   └── reg_load_store_mask.png    # Scenario diagram
└── scripts/
    ├── gen_data.py                # Generates input and golden data
    └── verify_result.py           # Verifies output data
```

![MaskReg operation diagram](./figures/reg_load_store_mask.png)

## Scenarios

| Scenario | Data Type and Length | Description |
| --- | --- | --- |
| 1 | `uint8_t[1024]` | Loads MaskReg from the first 32B of input, writes the value 2 at masked positions, and stores MaskReg starting from byte 256 of the output. |
| 2 | `float[256]` with `uint8_t[128]` mask | Processes 64 `float` elements at a time, selecting from two inputs according to MaskReg bits. |

## Build and Run

Run the following commands in the sample directory. `SCENARIO_NUM` can be 1 or 2 and defaults to 1.

```bash
export SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
cd ..
python3 scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./build/demo
python3 scripts/verify_result.py output/output.bin output/golden.bin
```

The default mode is NPU execution. For NPU simulation, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command:

```bash
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCMAKE_ASC_RUN_MODE=sim ..
```

After changing the scenario or run mode, remove the `build` directory or rerun CMake to avoid reusing cached configuration.

## Build Options

| Option | Description | Default |
| --- | --- | --- |
| `SCENARIO_NUM` | Selects the scenario. Valid values are 1 and 2. | 1 |
| `CMAKE_ASC_ARCHITECTURES` | Specifies the NPU architecture. Only `dav-3510` is supported. | dav-3510 |
| `CMAKE_ASC_RUN_MODE` | Selects the run mode: `npu` or `sim`. | npu |

On success, the executable prints `test pass!` and the verification script prints `verification passed`.
