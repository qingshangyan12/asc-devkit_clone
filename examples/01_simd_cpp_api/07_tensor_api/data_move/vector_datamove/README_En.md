# Vector Datamove Sample

## Overview

This sample demonstrates the four AIV-side Tensor API vector copy paths: `GM->UB`, `UB->GM`,
`UB->UB`, and `UB->L1`. It uses a `[32, 32]` `int8_t` tensor with 32-byte rows and validates the
complete output in every scenario.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Scenarios

| SCENARIO_NUM | Primary path | Layout | API form |
| --- | --- | --- | --- |
| 1 | GM->UB | ND | `copy(dst, src)` with automatic path inference |
| 2 | UB->GM | DN | `make_copy(..., trait)` followed by `atom.call(dst, src)` |
| 3 | UB->UB | NZ | Explicit atom with the coordinate/shape copy overload |
| 4 | UB->L1 | ZN | Explicit `copy(atom, dst, src)` with AIV execution completion validation |

Every kernel is declared with `__global__ __vector__`. Auxiliary copies only prepare input or return
the result to GM. On dav-3510, L1->UB is effective only on AIC, so a pure AIV kernel cannot read L1
back. Scenario 4 verifies that UB->L1 compiles, is issued, and completes synchronization, then returns
the source UB to GM as a kernel completion check.

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
python3 ../scripts/gen_data.py
./demo
```

A successful run prints:

```text
test pass!
```
