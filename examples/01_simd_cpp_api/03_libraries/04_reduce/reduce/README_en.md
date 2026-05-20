# Reduce Example

## Overview

This example demonstrates how to implement a reduce operator using the WholeReduceSum and BlockReduceSum high-level APIs. It accumulates data elements on continuous memory and returns the result.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series/Atlas A3 Inference Series
- Atlas A2 Training Series/Atlas A2 Inference Series

## Directory Structure

```
├── reduce
│   ├── scripts
│   │   ├── gen_data.py         // Script to generate input data and golden data
│   ├── CMakeLists.txt          // Build configuration file
│   ├── data_utils.h            // Data read/write functions
│   └── reduce.asc              // Ascend C operator implementation & invocation example
```

## Operator Description

- Operator Function:
  ReduceCustom accumulates data elements on continuous memory and returns the result. The mathematical expression is:

  ```
  z = sum(x)
  ```

- Operator Specification:
  <table>
  <tr><td rowspan="1" align="center">Operator Type(OpType)</td><td colspan="4" align="center"> reduce </td></tr>

  <tr><td rowspan="3" align="center">Operator Input</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">4096</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">Operator Output</td></tr>
  <tr><td align="center">y</td><td align="center">32</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">reduce_custom</td></tr>
  </table>

- Operator Implementation:
  This example implements a ReduceCustom operator with fixed shape: input x[4096], output y[32]. The x length range is (0,4096] for float and (0,16384] for half.

  The example accumulates continuous input data to get the sum of elements in this continuous buffer.

    - 1. When length is less than 256B, ComputeKey = 1. Use WholeReduceSum to get the result in one operation.

    - 2. When length is in (256B,2KB] for float input or (256B,4KB] for half input, ComputeKey = 2. BlockReduceSum executes faster than WholeReduceSum for the same input length. Use one BlockReduceSum plus one WholeReduceSum to achieve higher computational efficiency.

    - 3. When length is in (2KB,16KB] for float input or (4KB,32KB] for half input, ComputeKey = 3. One WholeReduceSum accumulates more efficiently than two BlockReduceSum operations. Use two WholeReduceSum (instead of two BlockReduceSum + one WholeReduceSum) to get the accumulated sum of this buffer.

    - 4. When length is 10000 for float input, ComputeKey = 4. This corresponds to the WholeReduceSumImpl processing method. In Counter mode, use WholeReduceSum instruction to loop through each row of 2D data and get the reduction result for each row.

    - 5. When length is 20000 for float input, ComputeKey = 5. This corresponds to the BinaryReduceSumImpl processing method. In Counter mode, first divide the data into two parts, use Add instruction to add them, loop repeatedly, and finally use one WholeReduceSum instruction to get the reduction result. This approach performs better than single WholeReduceSum instruction in scenarios with large data volume and many loops.
    Note: The code uses Counter mode.

  - Kernel Implementation
    The computation logic is: Ascend C vector computation interfaces operate on LocalTensor elements. Input data must first be moved to on-chip storage, then use Reduce high-level API to complete reduce computation, get the final result, and move it to external storage.

    ReduceCustom operator implementation consists of 3 basic tasks: CopyIn, Compute, and CopyOut. CopyIn task moves input Tensor xGm from Global Memory to xLocal. Compute task performs reduce computation on xLocal. The computation method is determined by ComputeKey parameter, which depends on input length. The result is stored in zLocal. CopyOut task moves output data from zLocal to output Tensor zGm on Global Memory.

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