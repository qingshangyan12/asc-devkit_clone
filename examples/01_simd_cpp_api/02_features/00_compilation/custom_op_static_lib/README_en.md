# Custom Operator Static Library

## Overview

This sample demonstrates how to build, package, and link a custom operator static library using `AddCustom` as an example, and execute the operator through the aclnn method.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── custom_op_static_lib
│   ├── CMakeLists.txt            // Top-level build project file
│   ├── app                       // Link static library and execute operator
│   └── op                        // Build and package to generate custom operator static library
```

## Code Implementation Introduction

In this sample, the `op` directory builds and packages to generate a custom operator static library. The generated static library path is `./build/customize-install/lib/lib${package_name}.a`. The `app` directory imports the static library through `find_package(${package_name})`, builds `main.cpp` to generate `execute_add_op`, calls `aclnnAddCustom`, and verifies the result.

For information about the AddCustom operator, see the [Operator Description](../custom_op/README_en.md).

## Build and Run

- Configure Environment Variables

  Select the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment. Ensure `ASCEND_HOME_PATH` points to the CANN installation root path so that header files and library paths take effect.
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

- Run Sample

  Execute the following steps in the sample root directory to run this sample.

  ```bash
  mkdir -p build; cd build
  cmake .. && make -j
  ./execute_add_op
  ```

  The following output indicates successful execution:

  ```log
  test pass
  ```