# data_copy_l0c2ub Example

## Overview

This example uses the Ascend C C API to move matrix-multiplication results from L0C Buffer to UB (Unified Buffer) through `asc_copy_l0c2ub`, followed by `asc_copy_ub2gm` to copy data from UB to GM (Global Memory). It supports NZ2NZ and NZ2ND format conversion, dual-target modes (split by M or N dimension), and inline scalar/vector quantization, ReLU activation, and Cast conversion. Scenarios 1-4 are NoQuant scenarios with half-type ND-format input matrices A and B. Scenarios 5-21 are quantization scenarios supporting int8/half input and covering output types including half/int8/int4b/bfloat16/fp8/hifloat8/float. The quantization parameters and scenario definitions are ported from the `data_copy_l0c2gm` example. The ND-format input matrices are converted to Nz while moved from GM to L1 Buffer, then processed through L0A Buffer and L0B Buffer by two K-axis matrix-multiplication chunks.

This example applies to Ascend 950PR/Ascend 950DT (`dav-3510`) and can run in NPU execution or NPU simulation mode. CPU debug mode is not provided.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
data_copy_l0c2ub
├── scripts
│   ├── gen_data.py                // Generates input and golden data
│   └── verify_result.py           // Verifies output against golden data
├── data_utils.h                   // Data input/output helpers
├── CMakeLists.txt                 // CMake build file
├── data_copy_l0c2ub.asc           // C API implementation and launcher
├── README.md                      // Chinese example documentation
└── README_en.md                   // English example documentation
```

## Detailed Scenario Description

The scenario is selected via the compile parameter `SCENARIO_NUM`. All scenarios use the same matrix-multiplication specification `[M, K, N] = [128, 128, 256]`, where K is split into two `[128, 64]` chunks. The kernel function name is `data_copy_l0c2ub`.

| SCENARIO_NUM | A/B type | L0C type | Output type | Output format | Quant mode | ReLU | Dual-target |
|---|---|---|---|---|---|---|---|
| 1 | half | float | float | Nz | NoQuant | No | No |
| 2 | half | float | float | ND | NoQuant | No | No |
| 3 | half | float | float | ND | NoQuant | No | Yes (M) |
| 4 | half | float | float | ND | NoQuant | No | Yes (N) |
| 5 | int8_t | int32_t | half | ND | Scalar `DEQF16` | Yes | No |
| 6 | int8_t | int32_t | half | Nz | Vector `VDEQF16` | No | No |
| 7 | half | float | int8_t | Nz | Scalar `QF322B8_PRE` | No | No |
| 8 | half | float | int8_t | ND | Vector `VQF322B8_PRE` | Yes | No |
| 9 | int8_t | int32_t | int8_t | ND | Scalar `REQ8` | No | No |
| 10 | int8_t | int32_t | int8_t | Nz | Vector `VREQ8` | Yes | No |
| 11 | int8_t | int32_t | int4b_t | Nz | Vector `VREQ4` | Yes | No |
| 12 | int8_t | int32_t | bfloat16_t | Nz | Vector `VQS322BF16_PRE` | No | No |
| 13 | half | float | half | Nz | Vector `VQF322F16_PRE` | No | No |
| 14 | half | float | bfloat16_t | Nz | Vector `VQF322BF16_PRE` | No | No |
| 15 | half | float | int4b_t | Nz | Vector `VQF322S4_PRE` | No | No |
| 16 | half | float | fp8_e4m3fn_t | Nz | Vector `VQF322FP8_PRE` | No | No |
| 17 | half | float | hifloat8_t | Nz | Vector `VQF322HIF8_PRE` | No | No |
| 18 | half | float | hifloat8_t | Nz | Vector `VQF322HIF8_PRE_HYBRID` | No | No |
| 19 | half | float | float | Nz | Vector `VQF322F32_PRE` | No | No |
| 20 | half | float | half | Nz | Cast `F322F16` | No | No |
| 21 | half | float | bfloat16_t | Nz | Cast `F322BF16` | No | No |

`SCENARIO_NUM` is passed as a compile-time macro by CMake, and the kernel selects the corresponding scenario via `if constexpr`. Recompilation is required after switching scenarios.

The kernel uses `__mix__(1, 2)` mode. The Cube Core (AIC) performs matrix multiplication and uses `asc_copy_l0c2ub` to move results from L0C to UB (quantization scenarios perform inline quantization during this step). The Vector Core (AIV) uses `asc_copy_ub2gm` to move data from UB to GM. Cross-core synchronization between AIC and AIV is performed via `asc_sync_block_arrive`/`asc_sync_block_wait`.

**Scenario 1: NZ output format**

- Input: A `[128, 128]`, B `[128, 256]`, both half type, ND format
- Output: C `[128, 256]`, float type, Nz format
- Implementation: `asc_copy_l0c2ub` with `enable_nz2nd=false`, `dual_dst_ctrl=0`
- Note: Only SUB BLOCK0 performs the UB-to-GM copy

**Scenario 2: ND output format**

- Input: A `[128, 128]`, B `[128, 256]`, both half type, ND format
- Output: C `[128, 256]`, float type, ND format
- Implementation: `asc_copy_l0c2ub` with `enable_nz2nd=true`, `dual_dst_ctrl=0`
- Note: Only SUB BLOCK0 performs the UB-to-GM copy

**Scenario 3: ND output, dual-target, split by M**

- Input: A `[128, 128]`, B `[128, 256]`, both half type, ND format
- Output: C `[64, 256]` per sub-block, float type, ND format
- Implementation: `asc_copy_l0c2ub` with `enable_nz2nd=true`, `dual_dst_ctrl=1` (M must be even)
- Note: Both SUB BLOCKs copy their half of data from UB to GM

**Scenario 4: ND output, dual-target, split by N**

- Input: A `[128, 128]`, B `[128, 256]`, both half type, ND format
- Output: C `[128, 128]` per sub-block, float type, ND format
- Implementation: `asc_copy_l0c2ub` with `enable_nz2nd=true`, `dual_dst_ctrl=2` (N must be a multiple of 32)
- Note: Both SUB BLOCKs use 2D `asc_copy_ub2gm` to copy data from UB to GM in a row-interleaved manner

**Scenario 5: int8 input, scalar dequantization, ND half output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`
- Output: C `[128, 256]`, ND format `half`
- Implementation: `asc_set_l0c_copy_prequant` configures scalar dequantization parameter, using `DEQF16` mode; ReLU and NZ2ND enabled

**Scenario 6: int8 input, Vector dequantization, Nz half output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `half`
- Implementation: `VDEQF16` mode, one Vector quant parameter per column

**Scenario 7: half input, scalar quantization, Nz int8_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`
- Output: C `[128, 256]`, Nz format `int8_t`
- Implementation: `asc_set_l0c_copy_prequant` configures scalar quantization parameter, using `QF322B8_PRE` mode

**Scenario 8: half input, Vector quantization, ND int8_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, ND format `int8_t`
- Implementation: `VQF322B8_PRE` mode; ReLU and NZ2ND enabled

**Scenario 9: int8 input, scalar quantization, ND int8_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`
- Output: C `[128, 256]`, ND format `int8_t`
- Implementation: `asc_set_l0c_copy_prequant` configures scalar quantization parameter, using `REQ8` mode; NZ2ND enabled

**Scenario 10: int8 input, Vector quantization, Nz int8_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `int8_t`
- Implementation: `VREQ8` mode; ReLU enabled

**Scenario 11: int8 input, Vector quantization, Nz int4b_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `int4b_t` (2 int4 values packed per byte)
- Implementation: `VREQ4` mode (int32_t to int4b_t); ReLU enabled

**Scenario 12: int8 input, Vector quantization, Nz bfloat16_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `int8_t`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `bfloat16_t`
- Implementation: `VQS322BF16_PRE` mode (int32_t to bfloat16_t)

**Scenario 13: half input, Vector quantization, Nz half output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `half`
- Implementation: `VQF322F16_PRE` mode (float to half)

**Scenario 14: half input, Vector quantization, Nz bfloat16_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `bfloat16_t`
- Implementation: `VQF322BF16_PRE` mode (float to bfloat16_t)

**Scenario 15: half input, Vector quantization, Nz int4b_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `int4b_t` (2 int4 values packed per byte)
- Implementation: `VQF322S4_PRE` mode (float to int4b_t)

**Scenario 16: half input, Vector quantization, Nz fp8_e4m3fn_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `fp8_e4m3fn_t`
- Implementation: `VQF322FP8_PRE` mode (float to fp8_e4m3fn_t)

**Scenario 17: half input, Vector quantization, Nz hifloat8_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `hifloat8_t`
- Implementation: `VQF322HIF8_PRE` mode (float to hifloat8_t, Half to Away Round)

**Scenario 18: half input, Vector quantization, Nz hifloat8_t output (Hybrid Round)**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `hifloat8_t`
- Implementation: `VQF322HIF8_PRE_HYBRID` mode (float to hifloat8_t, Hybrid Round)

**Scenario 19: half input, Vector quantization, Nz float output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`; quant params `[256] uint64_t`
- Output: C `[128, 256]`, Nz format `float`
- Implementation: `VQF322F32_PRE` mode (float to float); accuracy reaches 2 per mille but not 2 per cent mille. For 2 per cent mille accuracy requirements, use the AscendDeQuant high-level API

**Scenario 20: half input, Cast conversion, Nz half output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`
- Output: C `[128, 256]`, Nz format `half`
- Implementation: `F322F16` mode (float cast to half), CAST_RINT mode (round to nearest even)

**Scenario 21: half input, Cast conversion, Nz bfloat16_t output**

- Input: A `[128, 128]`, B `[128, 256]`, both ND format `half`
- Output: C `[128, 256]`, Nz format `bfloat16_t`
- Implementation: `F322BF16` mode (float cast to bfloat16_t), CAST_RINT mode (round to nearest even)

For scenarios 6, 8, 10, 11, 12, 13, 14, 15, 16, 17, 18, and 19, the quantization parameters are first moved from GM to L1 Buffer via `asc_copy_gm2l1`, then to Fixpipe Buffer via `asc_copy_l12fb`, and finally configured via `asc_set_l0c2gm_config`. The `asc_set_l0c2gm_config` parameter address is in units of 128B, so the parameter file must be 128B-aligned. The `[256] uint64_t` parameters total 2048B, which meets the requirement. When moving quantization parameters from L1 Buffer to Fixpipe Buffer, 2048B of data is transferred in 64B units, corresponding to `asc_copy_l12fb` `len_burst` of 32. After loading quantization parameters to Fixpipe Buffer, `asc_sync_pipe(PIPE_FIX)` ensures the parameters and configuration take effect. Then `asc_copy_l0c2ub` moves the computation results from L0C Buffer to UB (Unified Buffer), and finally the AIV core uses `asc_copy_ub2gm` to move data from UB to GM (Global Memory).

## Build and Run

Execute the following steps in the example root directory.

- Configure environment variables

  Configure environment variables according to the CANN toolkit [installation method](../../../../../docs/zh/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. For root users, the default is `/usr/local/Ascend`; for non-root users, the default is `${HOME}/Ascend`.

- NPU execution

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM
  ./demo
  python3 ../scripts/verify_result.py -scenarioNum=$SCENARIO_NUM output/output.bin ./output/golden.bin
  ```

- NPU simulation

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  Clean the CMake cache before switching modes, e.g. run `rm CMakeCache.txt` in the `build` directory.

- Build options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture, corresponding to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-21 | Scenario number |

- Expected output

  When the output matches the golden data, the following is printed:

  ```bash
  test pass!
  ```
