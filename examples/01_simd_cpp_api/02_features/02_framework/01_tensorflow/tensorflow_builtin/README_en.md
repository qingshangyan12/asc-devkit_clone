# Custom Operator Project + TensorFlow Built-in Operator Sample

## Overview

This sample demonstrates how to map Ascend C custom operators to TensorFlow built-in operators based on Add computation, and invoke them through TensorFlow.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── tensorflow_builtin
│   └── run_add_custom.py       // TensorFlow invocation script (mapped to built-in AddV2 operator)
```

## Code Implementation Introduction

After completing the sample project creation, the framework/tf_plugin directory is generated under the sample project directory for storing TensorFlow framework adaptation plugin implementation files.

This sample demonstrates mapping Add computation to the TensorFlow built-in Add operator. The core process follows:
1. Use `np.random.uniform` to generate random input data, and define input nodes through `tf.compat.v1.placeholder`.
2. Build computation graph: use `tf.math.add` to implement tensor addition operation.
3. Create CPU and NPU sessions separately, execute computation graph and pass input data through `session.run`.
4. Use `np.allclose` to compare NPU and CPU computation results and verify computation correctness.

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.
- Compile, Package and Deploy Custom Operator Project

  Before running this sample, first enter the [Custom Operator Project Sample](../../../00_compilation/custom_op/) directory to complete compilation, packaging and deployment.

  > [!NOTE] Note
  > Adapt the plugin code at path: `examples/01_simd_cpp_api/02_features/00_compilation/custom_op/framework/tf_plugin/tensorflow_add_custom_plugin.cc`. Modify the TensorFlow invocation sample name OriginOpType to "AddV2" in the plugin code, as shown below:
  >
  > ```cc
  > REGISTER_CUSTOM_OP("AddCustom")
  >   .FrameworkType(TENSORFLOW)      // type: TENSORFLOW
  >   .OriginOpType("AddV2")      // name in tf module
  >   .ParseParamsByOperatorFn(AutoMappingByOpFn);
  > ```

- Install TensorFlow Plugin Package

  Refer to the "Install Framework Plugin Package" section in [TensorFlow 2.6.5 Model Migration Guide](https://www.hiascend.com/document/redirect/canncommercial-tfmigr26) for detailed installation instructions and steps.

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
  python3 run_add_custom.py
  ```

  The execution result shown below indicates successful execution.

  ```log
  test pass
  ```