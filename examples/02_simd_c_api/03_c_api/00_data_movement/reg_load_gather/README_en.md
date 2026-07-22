# Reg Load Gather Sample

## Overview

This sample demonstrates Ascend C API interfaces that load non-contiguous UB data into vector registers. It uses `asc_gather` for element-indexed loads and `asc_gather_datablock` for DataBlock-indexed loads.

## Supported Products and CANN Version

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
reg_load_gather/
├── CMakeLists.txt                 # Build configuration
├── data_utils.h                   # Input and output file utilities
├── reg_load_gather.asc            # Ascend C API implementation
├── README.md                      # Chinese documentation
├── README_en.md                   # English documentation
└── scripts/
    ├── gen_data.py                # Generates input and golden data
    └── verify_result.py           # Verifies output data
```

## Scenarios

The input data type is `half`. The source length is 1024 and the output length is 128.

| Scenario | Interface | Index Type | Description |
| --- | --- | --- | --- |
| 1 | `asc_gather` | `uint16_t[128]` | Each index identifies an element in the source data. The operation gathers 128 elements. |
| 2 | `asc_gather_datablock` | `uint32_t[8]` | Each index identifies a 32B DataBlock in the source data. The operation gathers eight DataBlocks. |

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
