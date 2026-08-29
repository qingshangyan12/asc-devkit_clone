# Copy Vector Tensor API Sample

## Overview

This sample demonstrates AIV-side Tensor API `GM->UB`, `UB->GM`, and `UB->UB` copies, including
a one-dimensional `GM->UB` copy and constant left and right padding. Scenarios 1 through 3 use a
`[32, 32]` `int8_t` tensor, scenario 4 uses a one-dimensional `[1024]` tensor, and scenario 5 pads
a `[32, 24]` input to a `[32, 32]` output. Every scenario validates the complete output.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Scenarios

| SCENARIO_NUM | Primary path | Layout | API form |
| --- | --- | --- | --- |
| 1 | GM->UB | ND | `copy(dst, src)` with automatic path inference |
| 2 | UB->GM | DN | `make_copy(..., trait)` followed by `atom.call(dst, src)` |
| 3 | UB->UB | NZ | Explicit atom with the coordinate/shape copy overload |
| 4 | GM->UB | One-dimensional | `make_shape(1024)` and `make_stride(1)` select the one-dim copy route |
| 5 | GM->UB | ND | Three left-padding and five right-padding elements, with `asc_set_copy_pad_val` configuring the value |

All five kernels are declared with `__global__ __vector__`. Auxiliary copies only prepare input or return
the result to GM. Scenario 4 uses a rank-1 layout so that the GM and UB tensors match the one-dimensional
layout pattern.

Scenario 5 uses `gm_to_ub_params{3, 5, true}` to configure three left-padding and five right-padding
elements per row. `asc_set_copy_pad_val(padding_value)` sets each padding element to `-9`.

## Build and Run

After setting up the CANN environment, run the following commands from the sample directory. This
example runs scenario 1 in simulator mode:

```bash
source ${install_path}/set_env.sh
cmake -S . -B build \
    -DCMAKE_ASC_RUN_MODE=sim \
    -DCMAKE_ASC_ARCHITECTURES=dav-3510 \
    -DSCENARIO_NUM=1
cmake --build build -j
cd build
python3 ../scripts/gen_data.py -scenarioNum=1
./demo
```

The data-generation `scenarioNum` must match the build-time `SCENARIO_NUM`.

A successful run prints:

```text
test pass!
```
