# data_copy_ub2l1 Example

## Overview

This example uses the Ascend C C API to move matrix data from UB (Unified Buffer) to L1 (L1 Buffer), then performs L1-to-L0A and L1-to-L0B movement, Mmad matrix multiplication, and Fixpipe result writeback. It supports Ascend 950PR/Ascend 950DT (`dav-3510`) only and provides NPU execution and NPU simulation modes.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── data_copy_ub2l1
│   ├── scripts
│   │   ├── gen_data.py                // Generates input and golden data
│   │   └── verify_result.py           // Compares output with golden data
│   ├── CMakeLists.txt                 // CMake build file
│   ├── data_utils.h                   // Data read/write helper functions
│   ├── data_copy_ub2l1.asc            // C API implementation and launcher
│   ├── README.md                      // Chinese example documentation
│   └── README_en.md                   // English example documentation
```

## Scenario Description

Use the `SCENARIO_NUM` build parameter to select an input scenario. Both scenarios use the matrix multiplication shape [M, K, N] = [32, 32, 32], and the kernel name is `data_copy_ub2l1`.

<table>
<caption style="font-weight: normal;">
         <span style="font-weight: bold; font-size: 1.2em;">Table 1: SCENARIO_NUM values</span></caption>
<tr><td align="center">SCENARIO_NUM</td><td align="center">Input format</td><td align="center">Input type</td><td align="center">Output type</td><td align="center">UB-to-L1 processing</td></tr>
<tr><td align="center">1</td><td align="center">Nz</td><td align="center">half</td><td align="center">float</td><td align="center">Contiguous copy</td></tr>
<tr><td align="center">2</td><td align="center">ND</td><td align="center">half</td><td align="center">float</td><td align="center">Convert to Nz in UB, then copy contiguously</td></tr>
</table>

`SCENARIO_NUM` is passed by CMake as a compile-time macro. The kernel selects the scenario with `if constexpr`; rerun CMake and rebuild after changing it.

**Scenario 1: Nz input, half input type**

- Input: A [32, 32] in `half` Nz format; B [32, 32] in `half` Nz format
- Output: C [32, 32] in `float` ND format
- Implementation: AIV subcore 0 first moves A and B from GM to UB with `asc_copy_gm2ub`. After `asc_sync_notify` and `asc_sync_wait` establish the MTE2-to-MTE3 dependency, `asc_copy_ub2l1` copies the Nz data contiguously to L1

**Scenario 2: ND input, half input type**

- Input: A [32, 32] in `half` ND format; B [32, 32] in `half` ND format
- Output: C [32, 32] in `float` ND format
- Implementation: AIV subcore 0 first moves A and B from GM to UB. After MTE2-to-V synchronization, `CopyUbNdToNz` calls `asc_copy_ub2ub` for each C0 column block to rearrange ND data into Nz data. A and B use separate UB temporary buffers, avoiding read/write overlap between the two rearrangements and the following UB-to-L1 transfers. After V-to-MTE3 synchronization, `asc_copy_ub2l1` copies the Nz data contiguously to L1

### Data Flow and Synchronization

1. AIV subcore 0 uses MTE2 to move data from GM to UB.
2. Scenario 1 establishes MTE2-to-MTE3 synchronization and performs the contiguous UB-to-L1 copy. Scenario 2 establishes MTE2-to-V synchronization, rearranges ND data into Nz in UB, then establishes V-to-MTE3 synchronization before the contiguous UB-to-L1 copy.
3. `asc_sync_pipe(PIPE_MTE3)` ensures that both UB-to-L1 transfers finish. `asc_sync_block_arrive` then notifies AIC, and AIC waits for L1 data with `asc_sync_block_wait`. The host obtains the synchronization address with `aclrtGetHardwareSyncAddr`; the kernel configures it with `asc_set_ffts_base_addr`.
4. AIC calls `asc_copy_l12l0a` and `asc_copy_l12l0b_transpose` to move Nz data from L1 to L0A and L0B. After MTE1-to-M synchronization, it calls `asc_mmad` for matrix multiplication.
5. After M-to-FIX synchronization, `asc_set_l0c2gm_nz2nd` configures the Nz-to-ND conversion and `asc_copy_l0c2gm` writes the L0C result back to GM.

Only required pipeline dependencies are established; `PIPE_ALL` synchronization is not used.

## Build and Run

Run the following steps in the example root directory.

- Configure environment variables

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:**`${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for non-root users.

- Run the example

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project; NPU mode by default
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate input and golden data
  ./demo                           # Run the example
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # Verify the result
  ```

  Set `SCENARIO_NUM` and `-scenarioNum` to the same value: 1 or 2.

  To use NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command:

  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
  ```

  > **Notice:**Clear the CMake cache before switching run modes or scenarios. Run `rm CMakeCache.txt` in the build directory and rerun CMake.

- Build options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture for Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`, `2` | Scenario number |

- Expected result

  A successful accuracy comparison prints:

  ```bash
  test pass!
  ```
