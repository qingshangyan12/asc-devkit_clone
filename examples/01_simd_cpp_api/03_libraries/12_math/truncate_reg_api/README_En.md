# truncate_reg_api Sample

## Overview

This sample uses the experimental Reg Tensor API `asc::te::trunc` to truncate each floating-point register
element toward zero. The complete pipeline is:

```text
GM -> UB -> reg_tensor load -> asc::te::trunc -> reg_tensor store -> UB -> GM
```

The sample covers all supported element types:

| `SCENARIO_NUM` | Input/output type | Example |
| --- | --- | --- |
| 1 | `half` | `-3.9 -> -3.0` |
| 2 | `bfloat16_t` | `-3.9 -> -3.0` |
| 3 | `float` | `3.9 -> 3.0` |

`asc::te::trunc` processes only the elements enabled by the source `reg_tensor` mask and propagates that mask
to the returned register.

## Build and Run

Run the following commands in this sample directory:

```bash
SCENARIO_NUM=1
mkdir -p build && cd build
cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
make -j
python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
./demo
```

Set `SCENARIO_NUM` to `1`, `2`, and `3` in turn to verify every supported type. To use CPU debug mode or NPU
simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim`. Clear
`build/CMakeCache.txt` before switching modes.

Expected output:

```text
test pass!
```

> `asc::te::trunc` is currently experimental. This sample includes its public header,
> `tensor_api/arch/vector/experimental/type_conversion.h`.
