# data_copy_gm2l1 Sample

## Overview

This sample introduces how to use DataCopy to move input data from GM (Global Memory) to L1 (L1 Buffer), supporting multiple input formats (Nz, ND, DN), Bias matrix movement, quantization matrix movement and other features.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── data_copy_gm2l1
│   ├── figures                        // Illustrations
│   ├── scripts
│   │   ├── gen_data.py                // Input data and golden data generation script
│   │   └── verify_result.py           // Verification script to check if output data matches golden data
│   ├── data_utils.h                   // Data read/write functions
│   └── data_copy_gm2l1.asc            // Ascend C sample implementation & invocation sample
```

## Scenario Detailed Description

This sample selects different input scenarios through build parameter `SCENARIO_NUM`. The meanings corresponding to different SCENARIO_NUM values are shown in the table below. All scenarios use the same matrix multiplication specification: [M, K, N] = [128, 128, 256], kernel function name is `data_copy_gm2l1`.

<table>
<caption style="font-weight: normal;">
   	     <span style="font-weight: bold; font-size: 1.2em;">📌 Table 1: Meanings of Different scenarioNum Values</span></caption>
<tr><td rowspan="1" align="center">scenarioNum</td><td align="center">Input Format</td><td align="center">Input Data Type</td><td align="center">Output Data Type</td><td align="center">Enable Bias</td><td align="center">Enable Vector Quantization</td></tr>
<tr><td align="center">1</td><td align="center">Nz</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">2</td><td align="center">ND</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">3</td><td align="center">DN</td><td align="center">half</td><td align="center">float</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">4</td><td align="center">ND</td><td align="center">half</td><td align="center">float</td><td align="center">Yes</td><td align="center">No</td></tr>
<tr><td align="center">5</td><td align="center">ND</td><td align="center">half</td><td align="center">int8_t</td><td align="center">No</td><td align="center">Yes</td></tr>
</table>

**Scenario 1: Input Format Nz, Input Data Type half**
- Input: A [128, 128] half type, Nz format; B [128, 256] half type, Nz format
- Output: C [128, 256] float type, ND format
- Implementation: Use `DataCopyParams` to move NZ format input data from GM to L1
- Description: Input data is Nz format, moved directly to L1 without format conversion
<p align="center">
  <img src="figures/data_copy_gm2l1_NZ2NZ.png" width="800">
</p>

**Scenario 2: Input Format ND, Input Data Type half**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] float type, ND format
- Implementation: Use `Nd2NzParams` to move ND format input data from GM to L1, automatically converting to Nz format
- Description: Input data is ND format, automatically converted to Nz format during movement process
<p align="center">
  <img src="figures/data_copy_gm2l1_ND2NZ.png" width="800">
</p>

**Scenario 3: Input Format DN, Input Data Type half (Only Ascend 950PR/Ascend 950DT supported)**
- Input: A [128, 128] half type, DN format; B [128, 256] half type, DN format
- Output: C [128, 256] float type, ND format
- Implementation: Use `Dn2NzParams` to move DN format input data from GM to L1, automatically converting to Nz format
- Description: Input data is DN format, automatically converted to Nz format during movement process, DN format only supported by Ascend 950PR/Ascend 950DT
<p align="center">
  <img src="figures/data_copy_gm2l1_DN2NZ.png" width="800">
</p>

**Scenario 4: Input Format ND, Input Data Type half, Enable Bias**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format; Bias [256] float type
- Output: C [128, 256] float type, ND format
- Implementation: Use `Nd2NzParams` to move input data and Bias from GM to L1, add Bias through `Mmad(c, a, b, bias, mmadParams)` during matrix multiplication computation
- Description: Add Bias during matrix multiplication computation process, Bias moved from C1 to C2, then participates in matrix multiplication computation

**Scenario 5: Input Format ND, Input Data Type half, Enable Vector Quantization**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format; Quantization parameters [256] uint64_t type
- Output: C [128, 256] int8_t type, ND format
- Implementation: Use `Nd2NzParams` to move input data from GM to L1, execute matrix multiplication computation, move quantization parameters from GM to L1 using `DataCopy`, then use `Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>(cGM, c, quantAlphaTensor, fixpipeParams)` for Vector quantization output
- Description: Matrix multiplication computation result is float type, converted to int8_t type through Vector quantization, each column of C matrix corresponds to one quantization parameter

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.
- Configure Environment Variables
  Select the appropriate environment variable configuration command based on the [installation method](../../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on your current environment.
  - Default path, root user installed CANN software package
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN software package
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, installed CANN software package
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Sample Execution
  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project, default npu mode
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input data
  ./demo                           # Execute compiled executable program, run sample
  python3 ../scripts/verify_result.py output/output.bin ./output/golden.bin  # Verify if output result is correct
  ```

  When using CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Example:
  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching build mode, clean cmake cache. Execute `rm CMakeCache.txt` in build directory then re-run cmake.

- Build Option Description
  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `cpu`, `sim` | Run mode: NPU run, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU Architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-5 | Scenario number, scenario 3 only supports dav-3510 architecture |

  The execution result shown below indicates successful precision comparison.
  ```bash
  test pass!
  ```