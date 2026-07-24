# DataCopy ub2l1 Sample

## Overview

This sample demonstrates data movement from UB (Unified Buffer) to L1 (L1 Buffer) using DataCopy in the context of Mmad matrix multiplication. It covers two scenarios: contiguous data copy (aligned shape) and on-the-fly ND2NZ data copy with matrix alignment (non-aligned shape with padding).

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|---------|----------------------|
| Ascend 950PR / Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
├── data_copy_ub2l1
│   ├── scripts
│   │   ├── gen_data.py                   // Input data and ground truth data generation script
│   ├── CMakeLists.txt                    // Build configuration file
│   ├── data_utils.h                      // Data read/write utility functions
│   ├── data_copy_ub2l1.asc               // AscendC sample implementation & invocation
│   └── README.md                         // Sample documentation
```

## Sample Description

- Functionality:  
  Copy data from UB (Unified Buffer) to L1 (L1 Buffer), perform Mmad matrix multiplication, and finally move the result out to GM (Global Memory) using Fixpipe.

- Scenario Description:  
  Two copy scenarios are selected via the compile option `SCENARIO_NUM`:

  | Scenario | SCENARIO_NUM | M-N-K | Copy Interface | Input Data Format | Description |
  |----------|-------------|-------|----------------|-------------------|-------------|
  | Contiguous Copy | 1 | 128-128-128 (aligned) | `DataCopy(dst, src, count)` | NZ | Data content remains unchanged from UB to L1; input must be pre-converted to NZ format |
  | On-the-fly ND2NZ Copy | 2 | 120-120-120 (non-aligned, pads to 128-128-128) | `DataCopyPad` + `DataCopy(dst, src, DataCopyParams)` + `DataCopy(l1Dst, ubNZ, count)` | ND | Uses DataCopyPad to move ND data from GM to UB with width padding, then performs ND→NZ conversion in UB with height padding, finally copies contiguously to L1 |

  For API reference, see [UBToL1 Contiguous Data Copy](../../../../../docs/zh/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1连续数据搬运（DataCopy）.md) and [UBToL1 On-the-fly ND2NZ Copy](../../../../../docs/zh/api/SIMD-API/基础API/矩阵计算（ISASI）/矩阵计算的搬入/矩阵数据搬入至L1-Buffer/UBToL1随路转换-ND2NZ搬运（DataCopy）.md).

- Notes on Scenario 2 Implementation:

  Scenario 2 does not directly use the `DataCopy(dst, src, Nd2NzParams)` interface. Instead, it performs ND→NZ conversion inside UB first, then copies the result to L1 using the contiguous `DataCopy(l1Dst, ubNZ, count)`. This is because `DataCopy(dst, src, Nd2NzParams)` is implemented via software simulation — the hardware itself does not support this capability. If needed, users are recommended to design their own UB→L1 ND2NZ copy scheme based on their specific requirements.

  Scenario 2 uses a non-aligned shape (M=N=K=120, aligned to 128) and completes matrix alignment (padding) in UB during the ND2NZ conversion. The alignment is handled in two dimensions:

  1. **Width alignment**: The ND data in GM has a row width of 120 (not a multiple of 16), making strided 32-byte block copy in UB impossible. Therefore, the GM→UB stage uses the `DataCopyPad` interface to pad each row to 128 (16-aligned) during copy, ensuring that each row width in UB is 128 and the row stride is 32-byte aligned. The padding value is set to 0.

  2. **Height alignment**: The padded ND data in UB has only 120 rows, while the NZ format requires 128 rows (8 fractal blocks of 16 rows each). Before the ND2NZ conversion, the `Duplicate` interface is used to zero-fill the NZ destination area (128×128), then each column block copies only 120 valid rows, leaving the remaining 8 rows as zero padding.

  The data flow and pipeline synchronization for Scenario 2 are as follows:
  1. GM→UB (MTE2 pipeline): Use DataCopyPad to load ND data into the UB source region, completing width padding simultaneously.
  2. `MTE2_V` sync: Wait for GM→UB to complete.
  3. UB→UB zero-fill (V pipeline): Use Duplicate to zero-fill the NZ destination area, handling height padding.
  4. UB→UB per-column-block copy (V pipeline): Each C0 column block of the padded ND data is written into the UB temporary region in NZ layout, copying only valid height rows.
  5. `V_MTE3` sync: Wait for UB→UB conversion to complete.
  6. UB→L1 contiguous copy (MTE3 pipeline): NZ data in the UB temporary region is copied to L1.
  7. `MTE3_V` sync: Wait for matrix A copy to complete before processing matrix B, avoiding read/write conflicts on the shared temp area.

- Sample Specifications:
  <table>
  <tr><td rowspan="3" align="center">Input</td><td align="center">name</td><td align="center">shape (Scenario 1 / Scenario 2)</td><td align="center">data type</td><td align="center">format (Scenario 1 / Scenario 2)</td></tr>
  <tr><td align="center">x</td><td align="center">[128, 128] / [120, 120]</td><td align="center">half</td><td align="center">NZ / ND</td></tr>
  <tr><td align="center">y</td><td align="center">[128, 128] / [120, 120]</td><td align="center">half</td><td align="center">NZ / ND</td></tr>
  <tr><td rowspan="1" align="center">Output</td><td align="center">z</td><td align="center">[128, 128] / [120, 120]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">data_copy_ub2l1</td></tr>
  </table>

- Sample Implementation:
  1. AIV core: Copy data from GM (Global Memory) to UB (Unified Buffer). The data layout in GM differs between the two scenarios: Scenario 1 uses NZ format (column-major between fractals, row-major within fractal, pre-converted by `gen_data.py`); Scenario 2 uses ND format (original row-major, no pre-conversion needed), with width padding completed during copy via DataCopyPad.
  2. AIV core: Copy data from UB to L1 (L1 Buffer). Scenario 1 uses contiguous copy; Scenario 2 first performs ND→NZ conversion inside UB (including height padding), then copies to L1 using contiguous copy. After arriving in L1, the data layout is the same for both scenarios, and subsequent computation steps are identical.
  3. AIC core: Call the basic API LoadData to move data from L1 to L0A Buffer and L0B Buffer.
  4. AIC core: Call the basic API Mmad for matrix multiplication.
  5. AIC core: Call the basic API Fixpipe to move data from L0C Buffer to GM (Global Memory).

- Invocation  
  The kernel is invoked using the `<<<>>>` kernel call operator. The kernel is declared as `__mix__(1, 2)` (1 AIC + 2 AIV).

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.

- Set Environment Variables
  Configure environment variables according to the [installation method](../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit on your environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. If not specified, the default installation path is `/usr/local/Ascend`.

- Run the Sample

  Execute the following commands in the sample directory:
  ```bash
  mkdir -p build && cd build;                                               # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..;make -j;    # Build project, default npu mode, Scenario 1
  python3 ../scripts/gen_data.py --scenarioNum=1                            # Generate test input data (must match the build scenario)
  ./demo                                                                    # Run the compiled executable
  ```

  > **Note:** The `--scenarioNum` parameter of `gen_data.py` must match `-DSCENARIO_NUM` in cmake; otherwise, the input data format mismatch will cause accuracy check failure.

  To use CPU Debug or NPU Simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim`.

  Examples:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..;make -j; # CPU debug mode, Scenario 1
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=2 ..;make -j; # NPU simulation mode, Scenario 2
  ```

  > **Note:** Before switching build mode or scenario, clear the cmake cache by running `rm CMakeCache.txt` in the build directory, then re-run cmake.

- Build Options

  | Option | Values | Description |
  | ------ | ------ | ----------- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: dav-3510 corresponds to Ascend 950PR/950DT |
  | `SCENARIO_NUM` | `1` (default), `2` | Copy scenario: 1 = contiguous copy (NZ input, 128×128 aligned), 2 = on-the-fly ND2NZ copy (ND input, 120×120 non-aligned padded to 128×128) |

- Execution Result

  The expected output is as follows, indicating a successful accuracy comparison:
  ```bash
  test pass!
  ```
