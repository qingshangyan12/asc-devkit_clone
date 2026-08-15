# data_copy_l0c2l1 Example

## Overview

This example uses the Ascend C C API to move matrix-multiplication results from L0C Buffer to L1 Buffer through `asc_copy_l0c2l1`. It performs scalar or vector quantization and Cast conversion during the movement. The ND-format input matrices A and B are converted to Nz while moved from GM to L1 Buffer, then processed through L0A Buffer and L0B Buffer by two K-axis matrix-multiplication chunks.

Since Ascend 950PR/Ascend 950DT (dav-3510) does not support direct L1-to-GM data movement, this example follows the pattern from the basic API `fixpipe_l0c2l1` example: a second Mmad (C * identity matrix D) moves the quantized result from L1 Buffer back through L0A Buffer to L0C Buffer, and then `asc_copy_l0c2gm` transfers it to GM (Global Memory) for host-side verification.

This example applies to Ascend 950PR/Ascend 950DT (`dav-3510`) and can run in NPU execution or NPU simulation mode. CPU debug mode is not provided.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
data_copy_l0c2l1
├── scripts
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output against golden data
├── data_utils.h                   // Data input/output helpers
├── CMakeLists.txt                 // CMake build file
├── data_copy_l0c2l1.asc           // C API implementation and launcher
├── README.md                      // Chinese example documentation
└── README_en.md                   // English example documentation
```

## Detailed Scenario Description

Use the `SCENARIO_NUM` build parameter to select a scenario. Every scenario uses the matrix-multiplication shape `[M, K, N] = [128, 128, 128]`; the K axis is split into two `[128, 64]` chunks. The kernel is named `data_copy_l0c2l1`.

| SCENARIO_NUM | A/B type | L0C type | Output type and format | Quantization mode | 2nd Mmad type |
|---|---|---|---|---|---|
| 1 | half | float | int8_t, Nz | Scalar `QF322B8_PRE` | int8×int8→int32 |
| 2 | half | float | int8_t, Nz | Vector `VQF322B8_PRE` | int8×int8→int32 |
| 3 | int8_t | int32_t | int8_t, Nz | Scalar `REQ8` | int8×int8→int32 |
| 4 | int8_t | int32_t | int8_t, Nz | Vector `VREQ8` | int8×int8→int32 |
| 5 | half | float | half, Nz | Cast `F322F16` | half×half→float |
| 6 | half | float | half, Nz | Vector `VQF322F16_PRE` | half×half→float |
| 7 | int8_t | int32_t | half, Nz | Scalar `DEQF16` | half×half→float |
| 8 | int8_t | int32_t | half, Nz | Vector `VDEQF16` | half×half→float |
| 9 | half | float | bfloat16_t, Nz | Cast `F322BF16` | bf16×bf16→float |
| 10 | half | float | bfloat16_t, Nz | Vector `VQF322BF16_PRE` | bf16×bf16→float |
| 11 | int8_t | int32_t | int4b_t, Nz | Vector `VREQ4` | int4b×int4b→int32 |
| 12 | int8_t | int32_t | bfloat16_t, Nz | Vector `VQS322BF16_PRE` | bf16×bf16→float |
| 13 | half | float | int4b_t, Nz | Vector `VQF322S4_PRE` | int4b×int4b→int32 |
| 14 | half | float | fp8_e4m3fn_t, Nz | Vector `VQF322FP8_PRE` | fp8×fp8→float |
| 15 | half | float | hifloat8_t, Nz | Vector `VQF322HIF8_PRE` | hif8×hif8→float |
| 16 | half | float | hifloat8_t, Nz | Vector `VQF322HIF8_PRE_HYBRID` | hif8×hif8→float |
| 17 | half | float | float, Nz | Vector `VQF322F32_PRE` | float×float→float |

> **Note:** Scenarios 9-17 require CANN support for the corresponding `asc_copy_l0c2l1`, `asc_copy_l12l0a`, `asc_copy_l12l0b_trans`, and `asc_mmad` overloads (int4b_t, bfloat16_t, fp8_e4m3fn_t, hifloat8_t, float types).

`SCENARIO_NUM` is passed by CMake as a compile-time macro. The kernel selects the corresponding scenario with `if constexpr`. After changing the scenario, recompile the project.

The device-side data flow is as follows:

1. **First Mmad**: Each K chunk follows `GM -> L1 Buffer (MTE2) -> L0A Buffer/L0B Buffer (MTE1) -> Mmad (M)`. The result is stored in L0C Buffer.
2. **L0C-to-L1 movement** (`asc_copy_l0c2l1`, PIPE_FIX): After Mmad completes, the L0C Buffer result is moved to L1 Buffer with inline quantization/Cast conversion. For vector quantization scenarios, quantization parameters are first moved from GM to L1 Buffer, then to Fixpipe Buffer, and the parameter address is configured.
3. **Second Mmad** (verification path): The quantized result in L1 Buffer is loaded back to L0A Buffer (PIPE_MTE1). Simultaneously, an identity matrix D is loaded from GM to L1 Buffer and then to L0B Buffer. A second `Mmad` (C * D = C) stores the result in L0C Buffer.
4. **L0C-to-GM movement** (`asc_copy_l0c2gm`, PIPE_FIX): The second Mmad result is moved to GM with NoQuant mode and NZ2ND format conversion.

`asc_sync_pipe(PIPE_ALL)` is called at the end of the kernel to ensure that all pipelines complete.

## Build and Run

Run the following steps in the example root directory.

- Configure environment variables

  Configure the environment according to the CANN development kit [installation instructions](../../../../../docs/en/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for non-root users.

- NPU execution

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project; NPU mode by default
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input and golden data
  ./demo                           # Run the example executable
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin ./output/golden.bin  # Verify the result
  ```

- NPU simulation

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
  ```

  Clear the CMake cache before switching run modes. For example, run `rm CMakeCache.txt` in the `build` directory before reconfiguring.

- Build options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture for Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-17 | Scenario number |

- Expected result

  Successful output-to-golden verification prints:

  ```bash
  test pass!
  ```
