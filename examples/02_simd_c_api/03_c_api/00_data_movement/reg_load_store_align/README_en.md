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
├── CMakeLists.txt                 # Build configuration
├── data_utils.h                   # Input and output file utilities
├── reg_load_store_align.asc       # Ascend C API implementation
├── README.md                      # Chinese documentation
├── README_en.md                   # English documentation
└── scripts/
    ├── gen_data.py                # Generates input and golden data
    └── verify_result.py           # Verifies output data
```

## Scenarios

| Scenario | Register Load/Store Mode | Output Length | Description |
| --- | --- | --- | --- |
| 1 | Explicit offset | 1024 | Loads and stores 1021 elements with explicit offsets; the last three elements remain zero. |
| 2 | PostUpdate | 1024 | Uses PostUpdate mode to update addresses after each load and store. |
| 3 | AddrReg | 1024 | Uses an address register to configure the offset of each load and store. |
| 4 | DataBlock | 512 | Loads the first 16B of every 32B DataBlock and stores the data contiguously. |
| 5 | BRC B16 | 1024 | Broadcasts the first `half` element in each 256B segment to a vector. |
| 6 | Upsample B16 | 2048 | Repeats each input `half` element once before storing it. |

## Build and Run

Run the following commands in the sample directory. `SCENARIO_NUM` ranges from 1 to 6 and defaults to 1.

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
| `SCENARIO_NUM` | Selects the scenario. Valid values are 1 to 6. | 1 |
| `CMAKE_ASC_ARCHITECTURES` | Specifies the NPU architecture. Only `dav-3510` is supported. | dav-3510 |
| `CMAKE_ASC_RUN_MODE` | Selects the run mode: `npu` or `sim`. | npu |

On success, the executable prints `test pass!` and the verification script prints `verification passed`.
