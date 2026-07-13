# asc_copy_gm2l1 Sample

## Overview

This sample shows how to use the Ascend C API to move input data from GM (Global Memory) to the L1 Buffer. It supports NZ, ND, and DN input formats, Bias matrix movement, and vector-quantization parameter movement. After the input is moved, the sample performs L1-to-L0 movement, matrix multiplication, and L0C-to-GM result writeback.

This sample supports Ascend 950PR/Ascend 950DT (`dav-3510`) only. It runs in NPU mode only; CPU Debug and NPU simulation modes are not provided.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── asc_copy_gm2l1
│   ├── scripts
│   │   ├── gen_data.py                // Generates input and golden data
│   │   └── verify_result.py           // Verifies that output matches the golden data
│   ├── data_utils.h                   // Data input/output helpers
│   ├── CMakeLists.txt                 // CMake build file
│   ├── asc_copy_gm2l1.asc             // C API implementation and launcher
│   ├── README.md                       // Chinese sample documentation
│   └── README_en.md                    // English sample documentation
```

## Detailed Scenario Description

Use the `SCENARIO_NUM` build parameter to select an input scenario. The meaning of each value is shown below. All scenarios use the same matrix multiplication shape: [M, K, N] = [128, 128, 256]. The kernel is named `asc_copy_gm2l1`.

<table>
<caption style="font-weight: normal;">
         <span style="font-weight: bold; font-size: 1.2em;">📌 Table 1: SCENARIO_NUM values</span></caption>
<tr><td rowspan="1" align="center">SCENARIO_NUM</td><td align="center">Input format</td><td align="center">Input type</td><td align="center">Output type</td><td align="center">Bias enabled</td><td align="center">Vector quantization enabled</td></tr>
<tr><td align="center">1</td><td align="center">NZ</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">2</td><td align="center">ND</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">3</td><td align="center">DN</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">4</td><td align="center">ND</td><td align="center">half</td><td align="center">float</td><td align="center">Yes</td><td align="center">No</td></tr>
<tr><td align="center">5</td><td align="center">ND</td><td align="center">half</td><td align="center">int8_t</td><td align="center">No</td><td align="center">Yes</td></tr>
</table>

`SCENARIO_NUM` is passed by CMake as a compile-time macro. The kernel selects the corresponding scenario with `if constexpr`; rerun CMake and rebuild after changing the scenario.

**Scenario 1: NZ input, half input type**

- Input: A [128, 128] in `half` NZ format; B [128, 256] in `half` NZ format
- Output: C [128, 256] in `float` ND format
- Implementation: Use `asc_copy_gm2l1_sync` to move the NZ input data from GM to L1
- Description: The inputs are already in NZ format, so no format conversion is required

**Scenario 2: ND input, half input type**

- Input: A [128, 128] in `half` ND format; B [128, 256] in `half` ND format
- Output: C [128, 256] in `float` ND format
- Implementation: Use `asc_copy_gm2l1_nd2nz_sync` to move ND input data from GM to L1 and convert it to NZ
- Description: The ND inputs are converted to NZ during the movement

**Scenario 3: DN input, half input type (Ascend 950PR/Ascend 950DT only)**

- Input: A [128, 128] in `half` DN format; B [128, 256] in `half` DN format
- Output: C [128, 256] in `float` ND format
- Implementation: Use `asc_copy_gm2l1_dn2nz_sync` to move DN input data from GM to L1 and convert it to NZ
- Description: The DN inputs are converted to NZ during the movement

**Scenario 4: ND input, half input type, Bias enabled**

- Input: A [128, 128] in `half` ND format; B [128, 256] in `half` ND format; Bias [256] in `float`
- Output: C [128, 256] in `float` ND format
- Implementation: Use `asc_copy_gm2l1_nd2nz_sync` to move input data and Bias from GM to L1; use `asc_copy_l12bt_sync` to move Bias from L1 to BiasTable (C2); use `asc_mmad_sync` with C2 as the initial C matrix value to add Bias
- Description: The same Bias vector is added to every output row

**Scenario 5: ND input, half input type, vector quantization enabled**

- Input: A [128, 128] in `half` ND format; B [128, 256] in `half` ND format; quantization parameters [256] in `uint64_t`
- Output: C [128, 256] in `int8_t` ND format
- Implementation: Use `asc_copy_gm2l1_nd2nz_sync` to move input data from GM to L1; use `asc_copy_gm2l1_sync` and `asc_copy_l12fb_sync` to move quantization parameters from GM through L1 to Fixpipe Buffer; configure their address with `asc_set_l0c2gm_config`, then use `asc_copy_l0c2gm_sync` with `VQF322B8_PRE` for vector-quantized output
- Description: The matrix multiplication result is `float` and is converted to `int8_t`; each C matrix column uses one quantization parameter

## Build and Run

Run the following steps in the sample root directory to build and execute the sample.

- Configure environment variables

  Configure environment variables according to the CANN development kit [installation instructions](../../../../../docs/quick_start.md#prepare&install).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. If it is not specified, the default is `/usr/local/Ascend`.

- Run the sample

  Run the following commands in the sample directory.

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project; NPU mode only
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input data
  ./demo                           # Run the sample executable
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # Verify the result
  ```

  Set both `SCENARIO_NUM` and `-scenarioNum` to a value from 1 through 5 to run the corresponding scenario.

- Build options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture for Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-5 | Scenario number |

- Expected result

  For scenarios 1-4, the floating-point result must satisfy the accepted tolerance. For scenario 5, the int8 result must exactly match the golden data. In either case, successful verification prints:

  ```bash
  test pass!
  ```
