# Same-Address Conflict Operator Direct Call Example

## Overview

This example introduces the impact of same-address conflicts and two solutions, and provides kernel direct call methods.

## Supported Products

- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── gm_address_conflict
│   ├── scripts
│   │   ├── gen_data.py                    // Input data and ground truth data generation script
│   │   └── verify_result.py               // Verification script for checking output data against ground truth
│   ├── CMakeLists.txt                     // Build project file
│   ├── data_utils.h                       // Data read and write functions
│   └── gm_address_conflict.asc            // Ascend C operator implementation and call example
```

## Operator Description

- Operator Function:
  The Adds operator implements the function of adding a Tensor with a scalar value 2.0 and returning the addition result.

  The corresponding mathematical expression is:

  ```python
  z = x + 2.0
  ```

  - x: Input, shape [8192, 128], data type float.
  - z: Output, shape [8192, 128], data type float.

- Operator Specifications:

<table>
<tr><td rowspan="1" align="center">Operator Type(OpType)</td><td colspan="4" align="center">Adds</td></tr>
<tr><td rowspan="2" align="center">Operator Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">x</td><td align="center">8192 * 128</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Operator Output</td><td align="center">z</td><td align="center">8192 * 128</td><td align="center">float</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">adds_custom_v1/adds_custom_v2/adds_custom_v3</td></tr>
</table>


- Operator Implementation:

  - Kernel Implementation

    Computation logic: This example mainly introduces the impact of same-address conflicts in data transfers on transfer efficiency. In Global Memory data access, data access requests (read/write) are address-translated to 512 Bytes alignment within the AI processor. At the same time, if multiple cores' data access requests fall within consecutive 512 Bytes after translation, the AI processor will serially process requests falling within the same 512 Bytes range for data consistency requirements, resulting in reduced transfer efficiency, that is, same-address access phenomenon occurs.

    This example has 3 implementation versions:
    adds_custom_v1: Basic implementation version, each core has consistent computation order, same-address conflicts exist, bandwidth efficiency is poor.
    adds_custom_v2: Avoids same-address conflicts by adjusting each core's computation order.
    adds_custom_v3: Avoids same-address conflicts by adjusting the tiling order.

    The current operator execution mechanism guarantees that user kernel input parameters (including workspace/tiling) addresses are 512 Bytes aligned. Therefore, users only need to determine based on the address offset whether two addresses will fall within consecutive 512 Bytes range.


## Build and Run

Execute the following steps in the root directory of this example to build and run the operator.

- Configure Environment Variables

  Select the appropriate command to configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.

  - Default path, root user installed CANN package

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN package

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, installed CANN package

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Example Execution

  ```bash
  mkdir -p build && cd build;   # Create and enter build directory
  cmake ..;make -j;             # Build project
  python3 ../scripts/gen_data.py   # Generate test input data
  ./demo                        # Execute the compiled executable program, run the example
  python3 ../scripts/verify_result.py output/output_z_1.bin output/golden.bin   # Verify output results are correct, confirm algorithm logic is correct
  python3 ../scripts/verify_result.py output/output_z_2.bin output/golden.bin   # Verify output results are correct, confirm algorithm logic is correct
  python3 ../scripts/verify_result.py output/output_z_3.bin output/golden.bin   # Verify output results are correct, confirm algorithm logic is correct
  ```

  The following result indicates successful precision comparison:

  ```bash
  test pass!
  ```