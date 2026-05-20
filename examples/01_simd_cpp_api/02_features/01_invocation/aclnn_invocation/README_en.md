# Custom Operator Project and aclnn Single Operator API Invocation Sample

## Overview

This sample introduces how to execute fixed-shape operators using the aclnn `OpType` single operator API method based on the example custom operator project.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas 200I/500 A2 Inference Products
- Atlas Inference Series Products

## Directory Structure

```
├── aclnn_invocation
│   ├── CMakeLists.txt          // Build project file
│   └── main.cpp                // Operator invocation main program
```

## Code Implementation Introduction

After completing the development and deployment of a custom operator, you can verify single operator functionality through single operator API invocation. For details, see the [Single Operator API Invocation](https://hiascend.com/document/redirect/CannCommunityAscendCInVorkSingleOp) section and the "Single Operator API Execution" section in [Single Operator Invocation](https://hiascend.com/document/redirect/CannCommunityCppOpcall).

Single operator API execution is a C-language API-based operator execution method. It does not require providing a single operator description file for offline model conversion and can directly call single operator API interfaces.

This sample executes operator computation through the two-stage interface of `aclnnAddCustomGetWorkspaceSize` and `aclnnAddCustom`. The core process is as follows:
1. Create input/output `aclTensor` and prepare Device-side data.
2. Call `aclnnAddCustomGetWorkspaceSize` to obtain the workspace size required for this computation, and allocate corresponding Device memory.
3. Call `aclnnAddCustom` to execute computation, synchronize stream `aclrtSynchronizeStream`, and copy results back to Host side for verification.

## Build and Run

- Build, Package, and Deploy Custom Operator Project

  Before running this sample, navigate to the [Custom Operator Project Sample](../../00_compilation/custom_op/) directory to complete build, packaging, and deployment.

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