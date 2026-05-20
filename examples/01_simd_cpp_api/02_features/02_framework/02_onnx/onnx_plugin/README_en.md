# Custom Operator Project + ONNX Model Invocation Sample

## Overview

This sample introduces how to invoke custom operators through ONNX network invocation using LeakyRelu computation as an example.

## Supported Products

This sample supports the following product models:
- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── leaky_relu_onnx_invocation
│   ├── CMakeLists.txt           // Build project file
│   ├── leaky_relu.py            // Script to generate LeakyRelu ONNX model
│   └── main.cpp                 // Operator invocation main program (load om model and execute LeakyRelu inference)
```

## Sample Implementation

After completing custom operator development and deployment, verify sample functionality through ONNX model invocation. Convert the LeakyRelu ONNX model to an om model, then load the model in the application and execute inference.

This sample executes model inference through the `aclmdlExecute` interface. The core process follows:
1. Load the om model through `aclmdlLoadFromFile` and obtain model ID, then call `aclmdlGetDesc` to get model description information.
2. Call `aclmdlCreateDataset` to create input/output datasets, and allocate Device memory and create DataBuffer based on `aclmdlGetInputSizeByIndex`/`aclmdlGetOutputSizeByIndex`.
3. Construct Host-side input data, copy to Device side using `aclrtMemcpy`, call `aclmdlExecute` to execute inference, and copy results back to Host side for result verification.

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.
- Compile, Package and Deploy Custom Operator Project

  Before running this sample, first enter the [Custom Operator Project Sample](../../../00_compilation/custom_op/) directory to complete compilation, packaging and deployment.

- Install ONNX

  ```bash
  pip3 install onnx==1.12.0
  ```

- Configure Environment Variables

  Select the appropriate environment variable configuration command based on the [installation method](../../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on your current environment.
  - Default path, root user installed CANN software package

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installed CANN software package

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, installed CANN software package

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Sample Execution

  ```bash
  mkdir -p build; cd build
  python3 ../leaky_relu.py
  # Use atc model conversion tool to convert *.onnx format model to *.om format model
  atc --model=./leaky_relu.onnx --framework=5 --soc_version=${soc_version} --output=./leaky_relu_custom --input_shape="X:8,16,1024" --input_format=ND
  cmake .. && make -j
  ./execute_leaky_relu_op
  ```

  > Obtain the AI processor model <soc_version> through the following methods:
  > - For the following product models: Execute the `npu-smi info` command on the server with Ascend AI processor installed to query and obtain **Name** information. The actual configuration value is AscendName. For example, if **Name** value is xxxyy, the actual configuration value is Ascendxxxyy.
  >   - Atlas A2 Training Series Products / Atlas A2 Inference Series Products
  >   - Atlas 200I/500 A2 Inference Products
  >   - Atlas Inference Series Products
  >   - Atlas Training Series Products
  >
  > - For the following product models, execute the `npu-smi info -t board -i <id> -c <chip_id>` command on the server with Ascend AI processor installed to query and obtain **Chip Name** and **NPU Name** information. The actual configuration value is Chip Name_NPU Name. For example, if **Chip Name** value is Ascendxxx and **NPU Name** value is 1234, the actual configuration value is Ascendxxx_1234. Where:
  >
  >   id: Device ID, the NPU ID obtained through `npu-smi info -l` command is the device ID
  >
  >   chip_id: Chip ID, the Chip ID obtained through `npu-smi info -m` command is the chip ID
  >   - Ascend 950PR/Ascend 950DT
  >   - Atlas A3 Training Series Products / Atlas A3 Inference Series Products
  >
  >   Sample projects created based on AI processor models of the same series share common basic functions (sample development, compilation and deployment based on the project).

  The execution result shown below indicates successful execution.

  ```log
  test pass
  ```