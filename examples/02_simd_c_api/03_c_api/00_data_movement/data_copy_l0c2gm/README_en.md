# data_copy_l0c2gm Example

## Overview

This example uses the Ascend C C API to move matrix-multiplication results from L0C Buffer to GM (Global Memory) through Fixpipe. It performs scalar or vector quantization, ReLU activation, and NZ2ND conversion during the movement. The ND-format input matrices A and B are converted to Nz while moved from GM to L1 Buffer, then processed through L0A Buffer and L0B Buffer by two K-axis matrix-multiplication chunks.

This example applies to Ascend 950PR/Ascend 950DT (`dav-3510`) and can run in NPU execution or NPU simulation mode. CPU debug mode is not provided.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
data_copy_l0c2gm
├── scripts
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output against golden data
├── data_utils.h                   // Data input/output helpers
├── CMakeLists.txt                 // CMake build file
├── data_copy_l0c2gm.asc           // C API implementation and launcher
├── README.md                      // Chinese example documentation
└── README_en.md                   // English example documentation
```

## Detailed Scenario Description

Use the `SCENARIO_NUM` build parameter to select a scenario. Every scenario uses the matrix-multiplication shape `[M, K, N] = [128, 128, 256]`; the K axis is split into two `[128, 64]` chunks. The kernel is named `data_copy_l0c2gm`.

| SCENARIO_NUM | A/B type | L0C type | Output type and format | Quantization mode | ReLU | NZ2ND |
|---|---|---|---|---|---|---|
| 1 | int8_t | int32_t | half, ND | Scalar `DEQF16` | Yes | Yes |
| 2 | int8_t | int32_t | half, Nz | Vector `VDEQF16` | No | No |
| 3 | half | float | int8_t, Nz | Scalar `QF322B8_PRE` | No | No |
| 4 | half | float | int8_t, ND | Vector `VQF322B8_PRE` | Yes | Yes |
| 5 | int8_t | int32_t | int8_t, ND | Scalar `REQ8` | No | Yes |
| 6 | int8_t | int32_t | int8_t, Nz | Vector `VREQ8` | Yes | No |
| 7 | int8_t | int32_t | int4b_t, Nz | Vector `VREQ4` | Yes | No |
| 8 | int8_t | int32_t | bfloat16_t, Nz | Vector `VQS322BF16_PRE` | No | No |
| 9 | half | float | half, Nz | Vector `VQF322F16_PRE` | No | No |
| 10 | half | float | bfloat16_t, Nz | Vector `VQF322BF16_PRE` | No | No |
| 11 | half | float | int4b_t, Nz | Vector `VQF322S4_PRE` | No | No |
| 12 | half | float | fp8_e4m3fn_t, Nz | Vector `VQF322FP8_PRE` | No | No |
| 13 | half | float | hifloat8_t, Nz | Vector `VQF322HIF8_PRE` | No | No |
| 14 | half | float | hifloat8_t, Nz | Vector `VQF322HIF8_PRE_HYBRID` | No | No |
| 15 | half | float | float, Nz | Vector `VQF322F32_PRE` | No | No |
| 16 | half | float | half, Nz | Cast `F322F16` | No | No |
| 17 | half | float | bfloat16_t, Nz | Cast `F322BF16` | No | No |

`SCENARIO_NUM` is passed by CMake as a compile-time macro. The kernel selects the corresponding scenario with `if constexpr`. After changing the scenario, recompile the project.

The device-side flow is as follows. Each K chunk follows `GM→L1 Buffer (MTE2)→L0A Buffer/L0B Buffer (MTE1)→Mmad (M)`. `MTE1→MTE2` and `M→MTE1` protect L1 Buffer, L0A Buffer, and L0B Buffer reuse for the next chunk. The final `M→FIX` dependency ensures that Fixpipe moves the L0C Buffer result to GM after Mmad completes. `asc_sync_pipe(PIPE_ALL)` is called at the end of the kernel to ensure that all pipelines complete.

**Scenario 1: int8 input, scalar dequantization, and ND half output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`
- Output: C `[128, 256]` in ND `half`
- Implementation: Configure scalar dequantization with `asc_set_l0c_copy_prequant`, use `DEQF16`, and enable ReLU and NZ2ND

**Scenario 2: int8 input, vector dequantization, and Nz half output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `half`
- Implementation: Use `VDEQF16`; each output column uses one vector quantization parameter

**Scenario 3: half input, scalar quantization, and Nz int8_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`
- Output: C `[128, 256]` in Nz `int8_t`
- Implementation: Configure scalar quantization with `asc_set_l0c_copy_prequant` and use `QF322B8_PRE`

**Scenario 4: half input, vector quantization, and ND int8_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in ND `int8_t`
- Implementation: Use `VQF322B8_PRE`, and enable ReLU and NZ2ND

**Scenario 5: int8 input, scalar quantization, and ND int8_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`
- Output: C `[128, 256]` in ND `int8_t`
- Implementation: Configure scalar quantization with `asc_set_l0c_copy_prequant`, use `REQ8`, and enable NZ2ND

**Scenario 6: int8 input, vector quantization, and Nz int8_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `int8_t`
- Implementation: Use `VREQ8` and enable ReLU

**Scenario 7: int8 input, vector quantization, and Nz int4b_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `int4b_t` (two int4 values packed per byte)
- Implementation: Use `VREQ4` (int32_t to int4b_t) and enable ReLU

**Scenario 8: int8 input, vector quantization, and Nz bfloat16_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `int8_t`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `bfloat16_t`
- Implementation: Use `VQS322BF16_PRE` (int32_t to bfloat16_t)

**Scenario 9: half input, vector quantization, and Nz half output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `half`
- Implementation: Use `VQF322F16_PRE` (float to half)

**Scenario 10: half input, vector quantization, and Nz bfloat16_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `bfloat16_t`
- Implementation: Use `VQF322BF16_PRE` (float to bfloat16_t)

**Scenario 11: half input, vector quantization, and Nz int4b_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `int4b_t` (two int4 values packed per byte)
- Implementation: Use `VQF322S4_PRE` (float to int4b_t)

**Scenario 12: half input, vector quantization, and Nz fp8_e4m3fn_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `fp8_e4m3fn_t`
- Implementation: Use `VQF322FP8_PRE` (float to fp8_e4m3fn_t)

**Scenario 13: half input, vector quantization, and Nz hifloat8_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `hifloat8_t`
- Implementation: Use `VQF322HIF8_PRE` (float to hifloat8_t, Half to Away Round)

**Scenario 14: half input, vector quantization, and Nz hifloat8_t output (Hybrid Round)**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `hifloat8_t`
- Implementation: Use `VQF322HIF8_PRE_HYBRID` (float to hifloat8_t, Hybrid Round)

**Scenario 15: half input, vector quantization, and Nz float output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`; `[256] uint64_t` quantization parameters
- Output: C `[128, 256]` in Nz `float`
- Implementation: Use `VQF322F32_PRE` (float to float). Precision reaches 2/1000 but not 1/10000. For 1/10000 precision requirements, use the AscendDeQuant high-level API

**Scenario 16: half input, cast conversion, and Nz half output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`
- Output: C `[128, 256]` in Nz `half`
- Implementation: Use `F322F16` (float cast to half), cast mode is CAST_RINT (round to nearest even)

**Scenario 17: half input, cast conversion, and Nz bfloat16_t output**

- Input: A `[128, 128]` and B `[128, 256]`, both ND `half`
- Output: C `[128, 256]` in Nz `bfloat16_t`
- Implementation: Use `F322BF16` (float cast to bfloat16_t), cast mode is CAST_RINT (round to nearest even)

For scenarios 2, 4, 6, 7, 8, 9, 10, 11, 12, 13, 14, and 15, the parameters are moved from GM to L1 Buffer with `asc_copy_gm2l1`, then from L1 Buffer to Fixpipe Buffer with `asc_copy_l12fb`. `asc_set_l0c2gm_config` configures the vector-quantization parameter address. Its parameter address is expressed in 128B units, so the parameter file must be rounded up to 128B and any padding must be zero-filled. The `[256] uint64_t` parameters in this example occupy 2048B and already meet this requirement. Moving these parameters from L1 Buffer to Fixpipe Buffer transfers 2048B of data in 64B data blocks, so `len_burst` in `asc_copy_l12fb` is set to 32. `asc_sync_pipe(PIPE_FIX)` completes the L1 Buffer-to-Fixpipe Buffer movement and parameter configuration before Fixpipe moves the L0C Buffer result to GM.

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
  | `SATURATION_MODE` | 0, 1 (default) | Floating-point saturation mode: 1=saturation (CTRL[48]=0, inf→±MAX, nan→0), 0=non-saturation (CTRL[48]=1, inf/nan preserved). Integer types always use saturation. |

- Expected result

  Successful output-to-golden verification prints:

  ```bash
  test pass!
  ```

- Boundary value testing (inf/-inf/nan)

  `gen_data.py` supports the `--boundary` parameter for generating input data containing `+inf`/`-inf`/`nan` special values to test the saturation/propagation behavior of each quantization mode under boundary values.

  - half input scenarios: inject `+inf`/`-inf`/`nan` into matrix A (the half type can represent these special values). The L0C result will contain the corresponding special values, which are saturated or propagated to the output during quantization.
  - int8 input scenarios: int8 cannot represent `inf`/`nan`, so extreme values (127/-128) are used to produce extreme int32 L0C values and test quantization saturation behavior.
  - Use the `--saturation` parameter and `-DSATURATION_MODE` build option to control the CTRL[48] bit, testing both saturation and non-saturation modes.

  Single scenario test (saturation mode):

  ```bash
  SCENARIO_NUM=3
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM --boundary --saturation=1
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DSATURATION_MODE=1 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin ./output/golden.bin
  ```

  Single scenario test (non-saturation mode):

  ```bash
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM --boundary --saturation=0
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DSATURATION_MODE=0 -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin ./output/golden.bin
  ```

  One-click test for all 17 scenarios × 2 modes:

  ```bash
  bash scripts/test_boundary.sh
  ```

  Expected behavior of each quantization mode for boundary values (saturation mode, CTRL[48]=0):

  | Output type | +inf | -inf | nan | Notes |
  |---------|------|------|-----|------|
  | int8 | Saturate to 127 | Saturate to -128 or 0 | Saturate to 0 | Integer types always saturate |
  | int4b | Saturate to 7 | Saturate to -8 or 0 | Saturate to 0 | Same as above |
  | half | Saturate to 65504 | Saturate to -65504 | Saturate to 0 | Controlled by CTRL[48]=0 |
  | bfloat16 | Saturate to max | Saturate to min | Saturate to 0 | Controlled by CTRL[48]=0 |
  | fp8_e4m3fn | Saturate to 448 | Saturate to -448 | Saturate to 0 (CTRL[50]=0) | CTRL[48]=0 + CTRL[50]=0 |
  | hifloat8 | 111 (HiF8+inf) | 239 (HiF8-inf) | Saturate to 0 | Controlled by CTRL[48]=0 |
  | float | Preserve inf | Preserve -inf | Preserve nan | CTRL[48] does not support float; always preserved |

  Non-saturation mode (CTRL[48]=1) expected behavior:

  | Output type | +inf | -inf | nan | Notes |
  |---------|------|------|-----|------|
  | int8 | Same as saturation | Same as saturation | Same as saturation | Integer types always saturate |
  | int4b | Same as saturation | Same as saturation | Same as saturation | Same as above |
  | half | Preserve inf | Preserve -inf | Preserve nan | float16 can represent |
  | bfloat16 | Preserve inf | Preserve -inf | Preserve nan | bfloat16 can represent |
  | fp8_e4m3fn | Output nan | Output nan | Output nan | inf or out-of-range → nan |
  | hifloat8 | 111 (HiF8+inf) | 239 (HiF8-inf) | 128 (HiF8NAN) | inf/nan preserved as encodings |
  | float | Preserve inf | Preserve -inf | Preserve nan | CTRL[48] does not support float; always preserved |
