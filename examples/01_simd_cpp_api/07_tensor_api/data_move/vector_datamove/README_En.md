# Vector Datamove Sample

## Overview

This sample demonstrates four AIV-side Tensor API vector data movement paths: `GM->UB`, `UB->GM`,
`UB->UB`, and `UB->L1`. It also demonstrates constant left and right padding for `GM->UB` data
movement. Scenarios 1 through 4 use `[32, 32]` `int8_t` tensors. Scenario 5 pads a `[32, 24]` input
to produce a `[32, 32]` output. Each scenario validates the complete output.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Sample Scenarios

| SCENARIO_NUM | Primary Path | Layout | Main API Usage |
| --- | --- | --- | --- |
| 1 | GM->UB | ND | `copy(dst, src)` with automatic path inference |
| 2 | UB->GM | DN | `make_copy(..., trait)` followed by `atom.call(dst, src)` |
| 3 | UB->UB | NZ | An explicit atom with the coordinate/shape region copy overload |
| 4 | UB->L1 | ZN | Explicit `copy(atom, dst, src)` with AIV data movement completion validation |
| 5 | GM->UB | ND | Three left-padding and five right-padding elements; `asc_set_copy_pad_val(PADDING_VALUE)` sets each padding element to `-9` |

This sample demonstrates only vector-side data movement. The kernels in all five scenarios are declared
with `__global__ __vector__`. Auxiliary copies in the scenarios are used only to prepare input or return
results to GM. Because the `L1->UB` path on dav-3510 takes effect only on AIC, Scenario 4 cannot read L1
back in a pure AIV kernel. Instead, it verifies that `UB->L1` can be compiled, issued, and synchronized,
and then returns the source UB data to GM to confirm that the kernel completes successfully.

## Directory Structure

```text
|-- CMakeLists.txt
|-- README.md
|-- README_En.md
|-- vector_datamove.asc
|-- data_utils.h
`-- scripts
    `-- gen_data.py
```

## Build and Run

Perform the following steps in the sample root directory to build and run the operator.

- Configure environment variables

  Configure the environment variables according to the [CANN Development Kit installation
  method](../../../../../docs/quick_start.md#prepare&install) used in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Run the sample

  Run the following commands in the sample directory. Set `SCENARIO_NUM` to a value from 1 to 5 for
  the scenario to run.

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=${SCENARIO_NUM} -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  python3 ../scripts/gen_data.py -scenarioNum=${SCENARIO_NUM}
  ./demo
  ```

  To use CPU debug mode or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or
  `-DCMAKE_ASC_RUN_MODE=sim`, respectively. For example:

  ```bash
  cmake -DSCENARIO_NUM=1 -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  cmake -DSCENARIO_NUM=4 -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 .. && make -j
  ```

  > **Note:** Before switching the build mode, clear the CMake cache by running
  > `rm CMakeCache.txt` in the `build` directory, and then run CMake again.

- Build options

  | Option | Value | Description |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debugging, or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT, Atlas A3 training products, and Atlas A3 inference products |

- Execution result

  The following output indicates that the accuracy comparison is successful:

  ```text
  test pass!
  ```
