# Custom Operator Project + TensorFlow Custom Operator Sample

## Overview

This sample demonstrates how to map Ascend C custom operator Add computation to TensorFlow custom operators, and invoke Ascend C operators through TensorFlow.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── tensorflow_custom
│   ├── CMakeLists.txt                // Build project file
│   ├── custom_assign_add_custom.cc   // TensorFlow custom operator implementation
│   └── run_add_custom_tf.py          // TensorFlow invocation script (mapped to custom AddCustom operator)
```

## Code Implementation Introduction

After completing the operator project creation, the framework/tf_plugin directory is generated under the operator project directory for storing TensorFlow framework adaptation plugin implementation files.

This sample uses the custom operator Add as an example to map this operator to TensorFlow custom operators. The core process follows:
1. Load the custom operator library file through `tf.load_op_library` to obtain the custom operator interface `add_custom`.
2. Construct input data, use `tf.compat.v1.placeholder` to define input tensors, and compute results for `tf.math.add` and `add_custom` separately.
3. Configure `ConfigProto`, enable `NpuOptimizer`, and disable remapping and memory optimization to ensure operators execute as expected.
4. Use `np.allclose` to compare results from standard TensorFlow addition operator and custom operator to verify computation correctness.

## Build and Run

Execute the following steps in the sample root directory to build and run the operator.
- Compile, Package and Deploy Custom Operator Project

  Before running this sample, first enter the [Custom Operator Project Sample](../../../00_compilation/custom_op/) directory to complete compilation, packaging and deployment.

  > [!NOTE] Note
  > Adapt the plugin code at path: `examples/01_simd_cpp_api/02_features/00_compilation/custom_op/framework/tf_plugin/tensorflow_add_custom_plugin.cc`. Modify the TensorFlow invocation operator name OriginOpType to "AddCustom" in the plugin code, as shown below:
  >
  > ```cc
  > REGISTER_CUSTOM_OP("AddCustom")
  >   .FrameworkType(TENSORFLOW)      // type: TENSORFLOW
  >   .OriginOpType("AddCustom")      // name in tf module
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
  mkdir -p build; cd build
  cmake .. && make -j
  python3 ../run_add_custom_tf.py
  ```

  The execution result shown below indicates successful execution.

  ```log
  test pass
  ```