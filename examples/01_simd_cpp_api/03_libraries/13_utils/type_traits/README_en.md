# type_traits Type Traits Direct Invocation Example

## Overview

This example implements type_traits type traits use cases using Ascend C programming language. Type traits interfaces include is_void/remove_const/add_const and so on. Use <<<>>> kernel invocation operator to complete basic flow of operator kernel function execution verification on NPU side, providing corresponding end-to-end implementation.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series/Atlas A3 Inference Series
- Atlas A2 Training Series/Atlas A2 Inference Series

## Directory Structure

```
├── type_traits
│   ├── CMakeLists.txt          // Build configuration file
│   ├── data_utils.h            // Data read/write functions
│   ├── type_traits.asc         // Ascend C operator implementation & invocation example
│   └── README.md               // Example introduction
```

## Function Description

- type_traits type traits interfaces include is_void/is_integral/is_floating_point/is_array/is_pointer/is_reference/is_const/remove_const/remove_volatile/remove_cv/remove_reference/remove_pointer/add_const/add_volatile/add_cv/add_pointer/add_lvalue_reference/add_rvalue_reference. Use these interfaces for type judgment, addition, deletion, conversion and other operations during program compilation.

- Operator Implementation:
  - Kernel Implementation
    Ascend C calls each interface and prints interface return values and ascendc_assert judgment.

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
  The following result indicates successful execution.
  ```bash
  test pass!
  ```