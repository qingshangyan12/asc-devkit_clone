# int4 Data Type Matmul Compatibility Example

## Overview

This example demonstrates the end-to-end implementation of int4 matrix multiplication computation. It isolates different hardware implementations through compile-time macros.

- Atlas A2/A3 Training/Inference Series Products support int4 data type and can directly perform Matmul computation.
- Ascend 950PR/950DT Cube computation unit does not support int4 data type. You need to unzip int4x2 to int8 before performing Matmul computation.

## Supported Products

  - Ascend 950PR/Ascend 950DT
  - Atlas A3 Training Series Products/Atlas A3 Inference Series Products
  - Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── matmul_s4
│   ├── scripts                   // Data generation script directory
│   │   ├── gen_data.py           // Input data and ground truth data generation script
│   │   └── verify_result.py      // Verification script for checking output data against ground truth
│   ├── CMakeLists.txt            // Build project file
│   ├── data_utils.h              // Data read and write functions
│   └── matmul_s4.asc             // AscendC example implementation and call example
```

## Example Specifications:

  <table>
  <caption>Table 1 Example Specifications Description</caption>
  <tr><td rowspan="1" align="center">Category</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="4" align="center">Example Input</td></tr>
  <tr><td align="center">x4</td><td align="center">[256, 256]</td><td align="center">int4b_t</td><td align="center">ND</td></tr>
  <tr><td align="center">y4</td><td align="center">[256, 256]</td><td align="center">int4b_t</td><td align="center">ND</td></tr>
  <tr><td align="center">tiling</td><td align="center">[1, 64]</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[256, 256]</td><td align="center">int32_t</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">matmul_s4</td></tr>
  </table>

## Example Implementation:

  - **Atlas A2/A3 Training/Inference Series Products**: Cube computation unit supports int4 data type and can directly execute Matmul computation.
  - **Ascend 950PR/950DT**: Cube computation unit does not support int4 data type. You need to execute Unzip operation on Vector Core through mix mode to unzip int4x2 to int8 before performing Matmul computation.

## Build and Run

Execute the following steps in the root directory of this example to build and run the example.

- Configure Environment Variables

  Select the appropriate command to configure environment variables based on the [installation method](../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.

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
  mkdir -p build && cd build;
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Default npu mode
  python3 ../scripts/gen_data.py                                            # Generate test input data
  ./demo                                                                    # Execute the compiled executable program
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify output results
  ```

  For CPU debugging or NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debugging mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching build mode, clean cmake cache. Execute `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build Options Description

  | Option | Options | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU run, CPU debugging, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2/A3 series, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution Result

  The following result indicates successful precision comparison:

  ```bash
  test pass!
  ```