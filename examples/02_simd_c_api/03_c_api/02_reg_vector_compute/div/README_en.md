# Div Example

## Overview

This example is based on the C_API programming interface to implement Div operation, mainly calling the asc_div interface.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```
div
│   ├── scripts/             // Test script directory
│   │   └── gen_data.py      // Generate test input and golden data
│   ├── CMakeLists.txt       // Build configuration file
│   ├── data_utils.h         // Data read/write utility functions
│   ├── div.asc              // Ascend C operator implementation & invocation example
│   └── README.md            // Example description document
```

## Example Description

- Example functionality:

  The example computes float type data, using the asc_div interface to execute division. The computation formula is as follows:
  $$z_i = \frac{x_i}{y_i}$$

- Example specifications
  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">AIV Example</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center" colspan="2">data type</td></tr>
  <tr><td align="center">x</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td align="center">y</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">z</td><td align="center">[1024]</td><td align="center" colspan="2">float</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">div</td></tr>
  </table>

- Example implementation
    - Implementation flow:
        1. Allocate memory on the Host side and initialize input data
        2. Transfer data from Host memory to Device global memory
        3. Call the kernel function to execute computation on the Vector core
        4. Inside the kernel function: transfer input data from Global Memory to Unified Buffer via DataCopy
        5. Call the VF (Vector Function) function on the Unified Buffer
        6. Inside the div_vf function, perform asc_load, asc_div, and asc_store operations
        7. Transfer computation results from Unified Buffer back to Global Memory
        8. Perform precision verification and golden data comparison on the Host side (allowing 1 ULP error)

  - Constraints:
      - Input data length must be a multiple of GetVecLen()
      - The divisor cannot be 0. In practical applications, a check for zero divisors is required
      - The example currently only supports the float data type

## Build and Run

- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  mkdir -p build && cd build;                                               # Create and enter the build directory
  cmake ..;make -j;                                                         # Build the project
  python3 ../scripts/gen_data.py;                                           # Generate test input data
  ./demo                                                                    # Run the example
  ```

  To use NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j; # NPU simulation mode
  ```

  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and re-run cmake.
- Build option description

  | Option | Values | Description |
  |:--------------------------|:----------------------|:---------------------------------------------|
  | `CMAKE_ASC_RUN_MODE`      | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation                      |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510`            | NPU architecture: dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution result
  The following execution result indicates that the precision comparison is successful (allowing 1 ULP error).
  ```bash
  [Success] Case accuracy is verification passed (within 1 ULP).
  ```
