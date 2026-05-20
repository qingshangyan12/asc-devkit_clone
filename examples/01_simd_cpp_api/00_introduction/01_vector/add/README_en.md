# Add Example

## Overview

This example demonstrates the basic usage of Ascend C vector addition.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── add
│   ├── CMakeLists.txt      // Build project file
│   ├── add.asc             // Ascend C example implementation & invocation example
```

## Example Description

- Example Function:
  The Add example implements the function of adding two data values and returning the result. The corresponding mathematical expression is:
  ```
  z = x + y
  ```

- Example Specifications:
  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">Add</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

- Example Implementation:
  - Kernel Implementation
    This example uses 8 cores to complete the computation, with each core processing 2048 elements. The offset calculation is:
    ```
    block_idx * blockLength
    ```

    The Add example implementation process consists of 3 steps:

    **Step 1: Copy Data to UB (Unified Buffer)**

    Copy the inputs x and y from GM (Global Memory) to xLocal and yLocal on UB (Unified Buffer).

    **Step 2: Execute Vector Addition**

    Execute the addition operation on xLocal and yLocal, and store the result in zLocal on UB (Unified Buffer).

    **Step 3: Copy Result to GM (Global Memory)**

    Copy the output data from zLocal to the output z on GM (Global Memory).

  - Invocation Implementation
    Use the kernel caller <<<>>> to invoke the kernel function.

## Build and Run

Execute the following steps in the root directory of this example to build and run the example.

- Configure Environment Variables
  Select the corresponding environment variable configuration command based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on the current environment.

  - Default path, root user installs CANN software package
    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installs CANN software package
    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, install CANN software package
    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Example Execution
  ```bash
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project, default npu mode
  ./demo                           # Execute example
  ```

  When using NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Before switching build modes, clean the cmake cache by executing `rm CMakeCache.txt` in the build directory and then re-run cmake.

- Build Options

  | Option | Available Values | Description |
  |--------|------------------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU run, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 for Atlas A2/A3 series, dav-3510 for Ascend 950PR/950DT |

- Execution Result
  The execution result is shown below, indicating that the accuracy comparison passed.
  ```bash
  test pass!
  ```