# AddBroadCast Operator Direct Invocation Example

## Overview

This example demonstrates the direct invocation method for Add operator kernel function. It adds input Broadcast for multi-core & tiling scenarios.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series/Atlas A3 Inference Series
- Atlas A2 Training Series/Atlas A2 Inference Series

## Directory Structure

```
├── add_broadcast
│   ├── scripts
│   │   ├── gen_data.py          // Script to generate input data and golden data
│   │   └── verify_result.py     // Script to verify output data matches golden data
│   ├── CMakeLists.txt           // Build configuration file
│   ├── data_utils.h             // Data read/write functions
│   └── add_broadcast.asc        // Ascend C operator implementation & invocation example
```

## Operator Description

- Operator Function:
  Add operator adds two inputs and returns the result. The mathematical expression is:
  ```
  z = x + y
  ```
- Operator Specification:
  <table>
  <tr><td rowspan="1" align="center">Operator Type(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">Operator Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">axis = 0: (8, 1024) / (8, 1022) / (17, 1024) / (17, 1022) axis = 1: (16, 1) / (16, 1) / (20, 1) / (20, 1)
  </td><td align="center">bfloat16_t/int8_t/float/half/int16_t/int32_t</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">axis = 0: (1, 1024) / (1, 1022) / (1, 1024) / (1, 1022) axis = 1: (16, 256) / (16, 255) / (20, 256) / (20, 255)</td><td align="center">bfloat16_t/int8_t/float/half/int16_t/int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Operator Output</td><td align="center">z</td><td align="center">axis = 0: (8, 1024) / (8, 1022) / (17, 1024) / (17, 1022) axis = 1: (16, 256) / (16, 255) / (20, 256) / (20, 255)</td><td align="center">bfloat16_t/int8_t/float/half/int16_t/int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">add_broadcast_custom</td></tr>
  </table>

- Operator Implementation:
  
  The computation logic is: Ascend C vector computation interfaces operate on LocalTensor elements. Input data must first be moved to on-chip storage, then use computation interface to add two input parameters, get the final result, and move it to external storage.

    Add operator implementation consists of 3 basic tasks: CopyIn, Compute, and CopyOut.
    CopyIn task moves input Tensor xGm and yGm from Global Memory to Local Memory, stored in xLocal and yLocal respectively.
    Compute task performs Broadcast on xLocal and yLocal and stores them in temporary memory, then executes addition operation. The result is stored in zLocal.
    CopyOut task moves output data from zLocal to output Tensor zGm on Global Memory.

  - Tiling Implementation
    TilingData parameter design: TilingData parameters are essentially parameters related to parallel data partitioning.

    This example attempts to evenly distribute input data across given number of cores. If even distribution is impossible, calculate the count of whole cores and tail cores. Whole cores and tail cores process different amounts of data. Define UB_BLOCK_NUM as maximum available UB data blocks, corresponding to UB size of UB_BLOCK_NUM * 32 Bytes. Perform tiling partitioning with UB_BLOCK_NUM granularity.

    This example uses 20 tiling parameters: xLen, yLen, dataType are total length and data type of x, y input data; coef is the expansion factor for Broadcast input; axis is the Broadcast axis for input data; blockLength, tileNum, tileLength, lastTileLength are computation amount per core, tiling partition count, whole block and tail block length in core-even-distribution scenario; former(tail)Num, former(tail)Length, former(tail)TileNum, former(tail)TileLength, former(tail)LastTileLength are partitioning data for whole cores and tail cores in core-uneven-distribution scenario, corresponding to core-even-distribution scenario; isEvenCore determines whether current scenario is core-even-distribution.

  - Invocation Implementation
    Use kernel invocation operator <<<>>> to call the kernel function.

## Build and Run

Execute the following steps in the example root directory to build and run the operator.
- Configure Environment Variables
  Choose the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
  - Default path, root user installation
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installation
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, CANN software package installation
    ```bash
    source ${install_path}/cann/set_env.sh
    ```
    
- Example Execution
  ```bash
  mkdir -p build && cd build;
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-2201;make -j; # Default npu mode
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output result correctness, confirm algorithm logic correctness
  ```

  For CPU debug or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Clear cmake cache before switching build modes. Execute `rm CMakeCache.txt` in build directory and re-run cmake.

- Build Option Description
  | Option | Available Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu`(default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2/A3 series, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |
  
- Execution Result
  The following result indicates successful precision comparison.
  ```bash
  test pass!
  ```