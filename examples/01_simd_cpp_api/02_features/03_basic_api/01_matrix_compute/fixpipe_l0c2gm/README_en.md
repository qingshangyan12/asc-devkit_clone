# fixpipe_l0c2gm Sample

## Overview

This sample introduces how to use Fixpipe to move matrix multiplication results from CO1 (L0C Buffer) to GM (Global Memory), supporting multiple output formats (Nz, ND, DN), data type conversion, inline quantization, ReLU and ChannelSplit features. These interfaces efficiently transmit matrix multiplication computation results in L0C to global memory, supporting various data format conversions and preprocessing capabilities.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── fixpipe_l0c2gm
│   ├── figures                        // Illustrations
│   ├── scripts
│   │   ├── gen_data.py                // Input data and golden data generation script
│   │   └── verify_result.py           // Verification script to check if output data matches golden data
│   ├── CMakeLists.txt                 // Build project file
│   ├── data_utils.h                   // Data read/write functions
│   └── fixpipe_l0c2gm.asc             // Ascend C sample implementation & invocation sample
```

## FixpipeParamsV220 vs FixpipeParamsArch3510 Structure Comparison

Different products support different parameter structures:
- **Ascend 950PR/Ascend 950DT**: Supports both `FixpipeParamsV220` and `FixpipeParamsArch3510` parameter structures, recommend using `FixpipeParamsArch3510`
- **Atlas A3 Training/Inference Series, Atlas A2 Training/Inference Series**: Only supports `FixpipeParamsV220`

This sample selects different architectures through build parameter `CMAKE_ASC_ARCHITECTURES`, automatically selecting corresponding parameter structure based on architecture:
- `dav-2201` architecture: Uses `FixpipeParamsV220`
- `dav-3510` architecture: Uses `FixpipeParamsArch3510`

<table>
<caption style="font-weight: normal;">
  	     <span style="font-weight: bold; font-size: 1.2em;">📌 Table 1: Parameter Structure Comparison</span>
<tr><td rowspan="1" align="center">Member Name</td><td align="center">FixpipeParamsV220</td><td align="center">FixpipeParamsArch3510</td><td align="center">Description</td></tr>
<tr><td align="center">`nSize`</td><td align="center">✅</td><td align="center">✅</td><td>Size of output matrix in N direction</td></tr>
<tr><td align="center">`mSize`</td><td align="center">✅</td><td align="center">✅</td><td>Size of output matrix in M direction</td></tr>
<tr><td align="center">`srcStride`</td><td align="center">✅</td><td align="center">✅</td><td>Starting address offset of adjacent Z layout in source Nz matrix</td></tr>
<tr><td align="center">`dstStride`</td><td align="center">✅</td><td align="center">✅</td><td>Starting address offset of adjacent Z layout in destination matrix (Nz format) or number of elements per row (ND/DN format)</td></tr>
<tr><td align="center">`quantPre`</td><td align="center">✅</td><td align="center">✅</td><td>Quantization mode control</td></tr>
<tr><td align="center">`deqScalar`</td><td align="center">✅</td><td align="center">✅</td><td>Scalar quantization parameter</td></tr>
<tr><td align="center">`reluEn`</td><td align="center">✅</td><td align="center">✅</td><td>ReLU switch</td></tr>
<tr><td align="center">`unitFlag`</td><td align="center">✅</td><td align="center">✅</td><td>Mmad and Fixpipe fine-grained parallel control</td></tr>
<tr><td align="center">`isChannelSplit`</td><td align="center">✅</td><td align="center">✅</td><td>Channel split switch</td></tr>
<tr><td align="center">`ndNum` / `srcNdStride` / `dstNdStride`</td><td align="center">✅</td><td align="center">✅ (in TransformParams)</td><td>Parameters controlling multi-matrix transmission in NZ2ND scenario, independent members in V220, integrated into `TransformParams` structure in Arch3510</td></tr>
<tr><td align="center">`dnNum` / `srcNzMatrixStride` / `dstDnMatrixStride` / `srcNzC0Stride`</td><td align="center">❌</td><td align="center">✅ (in TransformParams)</td><td>Parameters controlling multi-matrix transmission in NZ2DN scenario, only Arch3510 supported</td></tr>
<tr><td align="center">`TransformParams`</td><td align="center">❌</td><td align="center">✅</td><td>Type selector based on template parameter, automatically selects parameter type according to CO2Layout</td></tr>
<tr><td align="center">`dualDstCtrl`</td><td align="center">❌</td><td align="center">✅</td><td>Dual destination mode control, supports M dimension split or N dimension split</td></tr>
<tr><td align="center">`subBlockId`</td><td align="center">❌</td><td align="center">✅</td><td>Indicates destination UB number in single destination mode</td></tr>
</table>

## Scenario Detailed Description

This sample selects different output scenarios through build parameter `SCENARIO_NUM`. The meanings corresponding to different SCENARIO_NUM values are shown in the table below. All scenarios use the same matrix multiplication specification: [M, N, K] = [128, 256, 128], kernel function name is `fixpipe_l0c2gm`.

<table>
<caption style="font-weight: normal;">
  	     <span style="font-weight: bold; font-size: 1.2em;">📌 Table 2: Meanings of Different scenarioNum Values</span>
<tr><td rowspan="1" align="center">scenarioNum</td><td align="center">L0C Data Type</td><td align="center">Output Data Type</td><td align="center">Output Format</td><td align="center">Enable Quantization</td><td align="center">Enable ReLU</td><td align="center">Enable ChannelSplit</td></tr>
<tr><td align="center">1</td><td align="center">float</td><td align="center">float</td><td align="center">Nz</td><td align="center">No</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">2</td><td align="center">float</td><td align="center">float</td><td align="center">ND</td><td align="center">No</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">3</td><td align="center">float</td><td align="center">float</td><td align="center">DN</td><td align="center">No</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">4</td><td align="center">float</td><td align="center">int8_t</td><td align="center">ND</td><td align="center">Yes</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">5</td><td align="center">float</td><td align="center">int8_t</td><td align="center">ND</td><td align="center">Yes</td><td align="center">No</td><td align="center">No</td></tr>
<tr><td align="center">6</td><td align="center">float</td><td align="center">float</td><td align="center">ND</td><td align="center">No</td><td align="center">Yes</td><td align="center">No</td></tr>
<tr><td align="center">7</td><td align="center">float</td><td align="center">float</td><td align="center">Nz</td><td align="center">No</td><td align="center">No</td><td align="center">Yes</td></tr>
</table>

**Scenario 1: Output Format Nz, Output Data Type float**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] float type, Nz format
- Implementation: Use `Fixpipe<outputType, l0cType, AscendC::CFG_NZ>` to move data from CO1 to GM, output as Nz format
- Description: CO1 data is Nz format directly output to GM as Nz format, data maintains original format unchanged
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2NZ.png" width="800">
</p>

**Scenario 2: Output Format ND, Output Data Type float**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] float type, ND format
- Implementation: Use `Fixpipe<outputType, l0cType, AscendC::CFG_ROW_MAJOR>` to specify ROW_MAJOR format conversion
- Description: Convert Nz format data in CO1 to ND format and output to GM, ND format has no alignment requirement like Nz format, configure parameters according to actual size when outputting
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2ND.png" width="800">
</p>

**Scenario 3: Output Format DN, Output Data Type float (Only Ascend 950PR/Ascend 950DT supported)**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [256, 128] float type, DN format
- Implementation: Use `Fixpipe<outputType, l0cType, AscendC::CFG_COLUMN_MAJOR>` to specify COLUMN_MAJOR format conversion
- Description: Convert Nz format data in CO1 to DN format and output to GM
<p align="center">
  <img src="figures/fixpipe_l0c2gm_NZ2DN.png" width="800">
</p>

**Scenario 4: Output Format ND, Output Data Type int8_t, Enable Scalar Quantization**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] int8_t type, ND format
- Implementation: Set `fixpipeParams.quantPre = QuantMode_t::QF322B8_PRE`, use Scalar quantization mode
- Description: Quantize float type data to int8_t type, entire C matrix uses one quantization parameter

**Scenario 5: Output Format ND, Output Data Type int8_t, Enable Vector Quantization**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] int8_t type, ND format
- Implementation: Set `fixpipeParams.quantPre = QuantMode_t::VQF322B8_PRE`, use Vector quantization mode, pass quantization parameters for each column through quantAlphaTensor
- Description: Quantize float type data to int8_t type, each column of C matrix corresponds to one quantization parameter, quantization parameters need to be copied from GM to L1

**Scenario 6: Output Format ND, Output Data Type float, Enable ReLU**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 256] float type, ND format
- Implementation: Set `fixpipeParams.reluEn = true` to enable ReLU functionality
- Description: Execute ReLU operation during data movement from CO1 to GM, setting negative values to 0

**Scenario 7: Output Format Nz, Output Data Type float, Enable ChannelSplit**
- Input: A [128, 128] half type, ND format; B [128, 256] half type, ND format
- Output: C [128, 512] float type, Nz format (with channel split enabled)
- Implementation: Set `fixpipeParams.isChannelSplit = true` to enable ChannelSplit functionality
- Description: Enable channel split functionality during data movement from CO1 to GM, splitting 16x16 small z fractal matrix into two independent 16X8 small z fractal matrices output to GM. Fixpipe interface input and output must both be float type, only supports Nz format

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
    When using NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` parameter.
  
  Example:
  ```bash
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching build mode, clean cmake cache. Execute `rm CMakeCache.txt` in build directory then re-run cmake.

- Build Option Description
  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `sim` | Run mode: NPU run, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU Architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | 1-7 | Scenario number |

  The execution result shown below indicates successful precision comparison.
  ```bash
  test pass!
  ```