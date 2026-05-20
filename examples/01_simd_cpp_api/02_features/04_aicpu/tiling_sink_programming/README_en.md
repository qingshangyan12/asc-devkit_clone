# Custom Operator Tiling Sink Graph Mode Invocation Example

## Overview

This example demonstrates calling custom operators in PyTorch graph mode based on a sample custom operator project. It enables Tiling sink to device side execution to optimize scheduling performance.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── tiling_sink_programming
│   └── test_add_custom_tiling_sink.py   // PyTorch graph mode Tiling sink invocation test script
```

## Code Implementation Introduction

The example script `test_add_custom_tiling_sink.py` contains three key parts:
1. Register custom operator in PyTorch and provide placeholder implementations for Meta/CPU/PrivateUse1 to ensure graph entry capability.
2. Register FX to GE converter, mapping `add_custom_tiling_sink` to GE side custom operator `AddCustomTilingSink`.
3. Enable graph mode execution through `torch.compile` and enable `tiling_schedule_optimize` configuration.

## Build and Run

- Install PyTorch and Ascend Extension for PyTorch Plugin

  Refer to the installation instructions from [pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch) open source repository or [Ascend Extension for PyTorch Community](https://hiascend.com/document/redirect/Pytorch-index). Select a supported `Python` version and corresponding release version to complete `torch` and `torch-npu` installation.

- Build, Package, and Deploy Custom Operator Project

  Before running this example, first enter the [custom operator project example](../../00_compilation/custom_op/) directory to complete build, packaging, and deployment.

- Install Prerequisites

  ```bash
  pip3 install expecttest
  ```

- Configure Environment Variables

  Select the appropriate command based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on your current environment.
  - Default path, root user installs CANN package

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installs CANN package

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, install CANN package

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Example Execution

  Execute the following steps in the example root directory to run the example.

  ```bash
  python3 test_add_custom_tiling_sink.py
  ```

  The following output indicates successful accuracy verification.

  ```bash
  Ran 1 test in **s
  OK
  ```