# torch.library Custom Operator Registration Direct Call Sample

## Overview

This sample demonstrates how to register custom operators using PyTorch's torch.library mechanism based on the Add operator.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── torch_library
│   ├── CMakeLists.txt          // Build project file
│   ├── add_custom.asc          // Ascend C operator implementation & torch.library registration
│   └── add_custom_test.py      // PyTorch invocation script
```

## Sample Description

- Sample Function:

  The Add computation formula is:

  ```
  z = x + y
  ```

- Sample Specifications:
  <table border="2" align="center">
  <caption>Table 1: AddCustom Sample Specification Description</caption>
  <tr><td rowspan="1" align="center">Sample Type(OpType)</td><td colspan="4" align="center">AddCustom</td></tr>
  </tr>
  <tr><td rowspan="3" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td align="center">y</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  </tr>
  </tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">z</td><td align="center">[8, 2048]</td><td align="center">half</td><td align="center">ND</td></tr>
  </tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">add_custom</td></tr>
  </table>

- Custom Operator Registration:

  This sample defines a namespace named `ascendc_ops` in `add_custom.asc` and registers the `ascendc_add` function within it.

  PyTorch provides the `TORCH_LIBRARY` macro as the core interface for custom sample registration. Use it to create and initialize custom operator libraries. After registration, invoke from Python side using `torch.ops.namespace.op_name`. For example:

  ```c++
  TORCH_LIBRARY(ascendc_ops, m) {
      m.def(ascendc_add"(Tensor x, Tensor y) -> Tensor");
  }
  ```

  Use `TORCH_LIBRARY_IMPL` to bind operators to specific `DispatchKey` (PyTorch device dispatch identifier). For NPU devices, register operator implementations to the `PrivateUse1` dispatch key. For example:

  ```c++
  TORCH_LIBRARY_IMPL(ascendc_ops, PrivateUse1, m)
  {
      m.impl("ascendc_add", TORCH_FN(ascendc_ops::ascendc_add));
  }
  ```

  In the `ascendc_add` function, obtain the current NPU stream using `c10_npu::getCurrentNPUStream()`, then invoke the custom Kernel function `add_custom` through the kernel call operator `<<<>>>` to execute the operator on NPU.

- Python Test Script

  In `add_custom_test.py`, load the generated custom operator library using `torch.ops.load_library`, invoke the registered `ascendc_add` function, and verify numerical correctness by comparing NPU output with CPU standard addition results.

## Build and Run

- Install PyTorch and Ascend Extension for PyTorch Plugin

  Refer to the installation instructions from [pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch) open source repository or [Ascend Extension for PyTorch Community](https://hiascend.com/document/redirect/Pytorch-index). Select the supported `Python` version compatible release, and complete installation of `torch` and `torch-npu`.

- Install Prerequisites

  ```bash
  pip3 install expecttest
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
  Execute the following steps in the sample root directory to run this sample.

  ```bash
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project
  python3 ../add_custom_test.py    # Execute sample
  ```

- Build Option Description

  | Option | Available Values | Description |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201`(default), `dav-3510` | NPU Architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution Result

  The execution result shown below indicates successful precision comparison.

  ```bash
  Ran 1 test in **s
  OK
  ```