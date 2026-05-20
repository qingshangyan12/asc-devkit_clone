# Custom Operator Project Multi-Vendor Parallel Build, Package, and Deployment Sample

## Overview

This sample demonstrates how to use `ExternalProject_Add` to parallel build two independent custom operator projects in a top-level CMake project:
- `add_custom` (AddCustom)
- `leaky_relu_custom` (LeakyReluCustom)

The `add_custom` project uses flat directory organization (host/kernel/tiling source code at the same level), while `leaky_relu_custom` maintains hierarchical directory structure.

Each sub-project separately completes the build and packaging of custom operators and generates independent `custom_opp_*.run` installation packages.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas 200I/500 A2 Inference Products
- Atlas Inference Series Products

> Note: This sample involves multiple operator examples. Please refer to the actual supported product models for each operator example.

## Directory Structure

```
├── parallel_ops_package
│   ├── CMakeLists.txt              // Top-level build project file (parallel build multiple operator packages)
│   ├── add_custom                  // Add operator project directory
│   └── leaky_relu_custom           // LeakyRelu operator project directory
│       ├── CMakeLists.txt          // LeakyRelu operator build project file
│       ├── framework               // Framework adapter plugin directory
│       ├── op_host                 // Host-side operator implementation directory
│       └── op_kernel               // Kernel-side operator implementation directory
```

## Sample Description

`parallel_ops_package` uses the same Add/LeakyRelu sample description as `custom_op`. Please refer to:

- [custom_op/README_en.md "Sample Description" section](../custom_op/README_en.md#sample-description)

## Sample Specifications

`parallel_ops_package` uses the same Add/LeakyRelu specifications as `custom_op`. Please refer to:

- [custom_op/README_en.md "Sample Specifications" section](../custom_op/README_en.md#sample-specifications)

## Code Implementation Introduction

For Add/LeakyRelu code implementation in `parallel_ops_package`, refer to the `custom_op` documentation.

- [custom_op/README_en.md "Code Implementation Introduction" section](../custom_op/README_en.md#code-implementation-introduction)

## Build and Run

Execute the following steps in the sample root directory to build, package, and deploy the custom sample packages.

- Configure Environment Variables

  Select the appropriate command based on the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
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

- Build, Package, and Deploy Two Operator Packages

  ```bash
  cmake -S . -B build
  cmake --build build -j
  # add_custom package
  ./build/add_custom/custom_opp_*.run

  # leaky_relu_custom package
  ./build/leaky_relu_custom/custom_opp_*.run
  ```

  The following output indicates successful execution:

  ```log
  SUCCESS
  ```

## Build Result Description

The top-level project generates two subdirectories under `build/`:
- `build/add_custom/`: AddCustom intermediate artifacts and installation package
- `build/leaky_relu_custom/`: LeakyReluCustom intermediate artifacts and installation package

These two directories are independent of each other, facilitating parallel development and release in multi-Vendor scenarios.