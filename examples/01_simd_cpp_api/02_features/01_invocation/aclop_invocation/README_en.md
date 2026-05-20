# Custom Operator Project and aclop Single Operator Model Invocation Sample

## Overview

This sample introduces how to execute fixed-shape operators using the `aclopExecuteV2` single operator model method based on the example custom operator project.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products
- Atlas 200I/500 A2 Inference Products
- Atlas Inference Series Products

## Directory Structure

```
├── aclop_invocation
│   ├── add_custom.json         // Single operator description file (atc model conversion input)
│   ├── CMakeLists.txt          // Build project file
│   └── main.cpp                // Operator invocation main program
```

## Code Implementation Introduction

After completing the development and deployment of a custom operator, you can verify single operator functionality through single operator model invocation. For details, see the "Single Operator Model Execution" section in [Single Operator Invocation](https://hiascend.com/document/redirect/CannCommunityCppOpcall).

For single operator model invocation in offline mode, you need to generate a single operator offline model through `atc --singleop` in advance and configure the model directory in the application.

This sample executes the single operator model through the `aclopExecuteV2` interface. The core process is as follows:
1. Create input/output Tensor descriptions through `aclCreateTensorDesc` and prepare Device-side data Buffer using `aclCreateDataBuffer`.
2. Specify the model directory through `aclopSetModelDir`, then execute operator computation using `aclopExecuteV2`.
3. Synchronize stream `aclrtSynchronizeStream` and copy Device-side results back to Host side for result verification.

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
  # Use atc model conversion tool to generate single operator offline model
  atc --singleop=../add_custom.json --output=. --soc_version=${soc_version}
  cmake .. && make -j
  ./execute_add_op
  ```

  > Obtain the AI processor model <soc_version> as follows:
  > - For the following product models: Execute the `npu-smi info` command on the server with Ascend AI processor installed to query the **Name** information. The actual configuration value is AscendName. For example, if **Name** is xxxyy, the actual configuration value is Ascendxxxyy.
  >   - Atlas A2 Training Series Products / Atlas A2 Inference Series Products
  >   - Atlas 200I/500 A2 Inference Products
  >   - Atlas Inference Series Products
  >   - Atlas Training Series Products
  >
  > - For the following product models, execute the `npu-smi info -t board -i <id> -c <chip_id>` command on the server with Ascend AI processor installed to query **Chip Name** and **NPU Name** information. The actual configuration value is Chip Name_NPU Name. For example, if **Chip Name** is Ascendxxx and **NPU Name** is 1234, the actual configuration value is Ascendxxx_1234. Where:
  >
  >   id: Device ID, the NPU ID found through the `npu-smi info -l` command is the device ID
  >
  >   chip_id: Chip ID, the Chip ID found through the `npu-smi info -m` command is the chip ID
  >   - Ascend 950PR/Ascend 950DT
  >   - Atlas A3 Training Series Products / Atlas A3 Inference Series Products
  >
  >   Operator projects created based on the same series of AI processor models have common basic functionality (operator development, build, and deployment based on the project).

  The following output indicates successful execution:

  ```log
  test pass
  ```