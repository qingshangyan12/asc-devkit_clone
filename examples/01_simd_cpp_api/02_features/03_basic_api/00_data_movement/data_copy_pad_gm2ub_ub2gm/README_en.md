# DataCopyPad Sample

## Overview

This sample implements non-32-byte aligned data movement and padding functionality based on the DataCopyPad API in data movement scenarios. The DataCopyPad API supports non-aligned data movement from Global Memory to Local Memory, and can pad specified values on the left or right side of data.

The data movement process includes: Global Memory (GM) → Unified Buffer (UB) (using DataCopyPad for non-aligned movement with padding) → Global Memory (GM). This sample uses static Tensor method to allocate UB memory, supports switching different scenarios through build parameters, and demonstrates different usage methods of DataCopyPad.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── data_copy_pad_gm2ub_ub2gm
│   ├── scripts
│   │   ├── gen_data.py         // Input data and golden data generation script
│   │   └── verify_result.py    // Verification script to check if output data matches golden data
│   ├── CMakeLists.txt          // Build project file
│   ├── data_utils.h            // Data read/write functions
│   ├── figures                 // Illustrations
│   └── data_copy_pad.asc       // Ascend C sample implementation & invocation sample
```

## Scenario Description

This sample selects different scenarios through build parameter `SCENARIO_NUM`. All scenarios use ND data format with kernel function name `data_copy_pad_custom`.

<table border="2">
<caption>Table 1: Scenario Configuration Reference Table</caption>
<tr><th>scenarioNum</th><th>Input Shape</th><th>Output Shape</th><th>Data Type</th><th>Padding/Movement Mode</th><th>Description</th></tr>
<tr><td>1</td><td>[1, 20]</td><td>[1, 32]</td><td>half</td><td>SetPadValue padding</td><td>Right padding 12 elements, requires SetPadValue to set padding value to 1</td></tr>
<tr><td>2</td><td>[32, 59]</td><td>[32, 64]</td><td>float</td><td>rightPadding padding</td><td>Right padding 5 elements, padding value defaults to 0, no SetPadValue required</td></tr>
<tr><td>3</td><td>[3, 24]</td><td>[1, 80]</td><td>half</td><td>Compact padding</td><td>Compact mode, right padding 16 bytes on last data block (dav-3510 only)</td></tr>
<tr><td>4</td><td>[1, 320]</td><td>[1, 576]</td><td>int8</td><td>LoopMode movement (Compact)</td><td>SetLoopModePara enables loop mode, Compact mode, implements GM→UB non-continuous stride movement (dav-3510 only)</td></tr>
<tr><td>5</td><td>[1, 320]</td><td>[1, 576]</td><td>int8</td><td>LoopMode movement (Normal)</td><td>SetLoopModePara enables loop mode, Normal mode, implements GM→UB non-continuous stride movement (dav-3510 only)</td></tr>
<tr><td>6</td><td>[2, 4, 3, 128, 126]</td><td>[512, 128]</td><td>int8</td><td>LoopMode movement (Normal)</td><td>Five-dimensional data movement, moving [2, 2, 2, 64, 126], each row padded 2 bytes to 128 bytes (dav-3510 only)</td></tr>
</table>

### Scenario Detailed Description

**Scenario 1: Custom Padding Using SetPadValue**
- Input shape: [1, 20]
- Output shape: [1, 32]
- Data type: half
- Parameter configuration: isPad=false, leftPadding=0, rightPadding=12
- Description: Use SetPadValue to set padding value to 1, right padding 12 elements. **SetPadValue requires additional call by user to set padding value**, used with isPad=false.

**Scenario 2: Default Padding Using rightPadding**
- Input shape: [32, 59]
- Output shape: [32, 64]
- Data type: float
- Parameter configuration: isPad=true, leftPadding=0, rightPadding=5
- Description: **No SetPadValue required**, when isPad=true padding value defaults to 0, right padding 5 elements.

**Scenario 3: Data Movement Using Compact Mode ----This scenario only supports Ascend 950PR/Ascend 950DT products**
- Input shape: [3, 24]
- Output shape: [1, 80]
- Data type: half
- Parameter configuration: blockLen=48, blockCount=3, leftPadding=0, rightPadding=16, isPad=false
- Description: Compact mode allows single movement without alignment, unified padding at the end of entire data block to 32-byte alignment. In this example, leftPadding is 0, rightPadding is 16, padding 16 bytes on the right side of the last data block. Destination operand data amount is 160 bytes.

**Scenario 4: Enable loop mode using SetLoopModePara (Compact mode) ----This scenario only supports Ascend 950PR/Ascend 950DT products**
- Input shape: [1, 320], as shown in Figure 1
- Output shape: [1, 576], as shown in Figure 2
- Data type: int8
- Parameter configuration:
  - GM→UB: LoopModeParams{loop1Size=2, loop2Size=2, loop1SrcStride=80, loop1DstStride=128, loop2SrcStride=160, loop2DstStride=288}, DataCopyMVType::OUT_TO_UB
  - DataCopyExtParams: BLOCK_COUNT=2, BLOCK_LEN=40 (using constexpr constant)
  - DataCopyPadExtParams: isPad=true, leftPadding=0, rightPadding=0, padValue=-1
- Description: Enable loop mode through SetLoopModePara, use Compact mode to implement GM→UB non-continuous stride data movement. In Compact mode, after each inner loop movement of 80B, pad 16B to achieve 96B alignment, padding value set to -1.

**Figure 1** Source Operand Movement Scenario Example

<img src="figures/datacopypad1.png" width="80%">

**Figure 2** Destination Operand Compact Mode Movement Scenario Example

<img src="figures/datacopypad2.png">

**Scenario 5: Enable loop mode using SetLoopModePara (Normal mode) ----This scenario only supports Ascend 950PR/Ascend 950DT products**
- Input shape: [1, 320], as shown in Figure 1
- Output shape: [1, 576], as shown in Figure 3
- Data type: int8
- Parameter configuration:
  - GM→UB: LoopModeParams{loop1Size=2, loop2Size=2, loop1SrcStride=80, loop1DstStride=128, loop2SrcStride=160, loop2DstStride=288}, DataCopyMVType::OUT_TO_UB
  - DataCopyExtParams: BLOCK_COUNT=2, BLOCK_LEN=40 (using constexpr constant)
  - DataCopyPadExtParams: isPad=true, leftPadding=0, rightPadding=0, padValue=-1
- Description: Enable loop mode through SetLoopModePara, use Normal mode to implement GM→UB non-continuous stride data movement. In Normal mode, after each block movement, pad 24B to achieve 64B alignment, padding value set to -1.

**Figure 3** Destination Operand Normal Mode Movement Scenario Example

<img src="figures/datacopypad3.png">

**Scenario 6: Enable loop mode using SetLoopModePara (Normal mode) for five-dimensional data movement ----This scenario only supports Ascend 950PR/Ascend 950DT products**
- Input shape: [2, 4, 3, 128, 126], five-dimensional data
- Output shape: [512, 128], continuously stored in UB
- Data type: int8
- Movement specification: [2, 2, 2, 64, 126], each 126 bytes padded 2 bytes to 128 bytes
- Parameter configuration:
  - GM→UB: LoopModeParams{loop1Size=2, loop2Size=2, loop1SrcStride=128*126, loop1DstStride=64*128, loop2SrcStride=3*128*126, loop2DstStride=2*64*128}, DataCopyMVType::OUT_TO_UB
  - DataCopyExtParams: blockCount=64, blockLen=126, srcStride=0, dstStride=0
  - DataCopyPadExtParams: isPad=true, leftPadding=0, rightPadding=0, padValue=0
  - Dimension 0 uses for loop to move 2 times

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
  SCENARIO_NUM=1  # Set scenario number
  mkdir -p build && cd build;      # Create and enter build directory
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # Build project
  python3 ../scripts/gen_data.py -scenarioNum=$SCENARIO_NUM   # Generate test input data
  ./demo                           # Execute compiled executable program, run sample
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin -scenarioNum=$SCENARIO_NUM  # Verify if output result is correct
  ```

  When using NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:

  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching build mode, clean cmake cache. Execute `rm CMakeCache.txt` in build directory then re-run cmake.

- Build Option Description

  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `sim` | Run mode: NPU run, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  | `SCENARIO_NUM` | `1`(default), `2`, `3`, `4`, `5`, `6` | Scenario number: 1 (SetPadValue padding), 2 (rightPadding), 3 (Compact mode), 4 (SetLoopModePara loop mode Compact), 5 (SetLoopModePara loop mode Normal), 6 (five-dimensional LoopMode Normal) |

- Execution Result

  The execution result shown below indicates successful precision comparison.

  ```bash
  test pass!
  ```