# Load and Store Example

## Overview

This example demonstrates the `Load` and `Store` convenience interfaces with `RegTraitNumOne` and `RegTraitNumTwo`. Select the scenario with the CMake build parameter `SCENARIO_NUM`. The scenarios cover both `Store` overloads, with and without `count`.

| SCENARIO_NUM | RegTensor Type | Store Interface | Transfer Size |
| --- | --- | --- | --- |
| 1 | `RegTensor<uint64_t, RegTraitNumOne>` | `Store(dstAddr, srcReg)` | 32 `uint64_t` elements (256 bytes) |
| 2 | `RegTensor<uint64_t, RegTraitNumOne>` | `Store(dstAddr, srcReg, count)`, with `count` set to 32 | 32 `uint64_t` elements (256 bytes) |
| 3 | `RegTensor<uint64_t, RegTraitNumTwo>` | `Store(dstAddr, srcReg)` | 64 `uint64_t` elements (512 bytes) |
| 4 | `RegTensor<uint64_t, RegTraitNumTwo>` | `Store(dstAddr, srcReg, count)`, with `count` set to 64 | 64 `uint64_t` elements (512 bytes) |

All four scenarios offset the source and destination addresses by one `uint64_t` element, so the UB start addresses are not 32-byte aligned. `Load` and `Store` perform the preprocessing and postprocessing required for the unaligned transfers.

## Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | Versions that support this feature |

## Directory Structure

```text
reg_load_store/
├── scripts/
│   └── gen_data.py        // Generates input and golden data
├── CMakeLists.txt         // Build configuration
├── data_utils.h           // File input and output helpers
├── load_store.asc         // Ascend C implementation and entry point
├── README.md              // Chinese documentation
└── README_en.md           // English documentation
```

## Example Description

The input and output each contain 68 `uint64_t` elements. The kernel first copies the data from GM to UB and offsets both UB addresses by 8 bytes. It then calls `Load` to move data into a RegTensor and calls the `Store` overload selected for the current scenario to write the output region.

- Scenario 1 uses `RegTraitNumOne` and the `Store` overload without `count` to transfer one VL, or 32 `uint64_t` elements.
- Scenario 2 uses `RegTraitNumOne` and the `Store` overload with `count` set to 32 to transfer one VL.
- Scenario 3 uses `RegTraitNumTwo` and the `Store` overload without `count` to transfer two VLs, or 64 `uint64_t` elements.
- Scenario 4 uses `RegTraitNumTwo` and the `Store` overload with `count` set to 64 to transfer two VLs.

## Build and Run

After configuring the CANN environment, run the following commands in the example directory:

```bash
source ${install_path}/cann/set_env.sh
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

Set `SCENARIO_NUM` to `1`, `2`, `3`, or `4` to run the corresponding scenario. Clear the CMake cache and rebuild before switching scenarios.

A successful run prints:

```text
test pass!
```
