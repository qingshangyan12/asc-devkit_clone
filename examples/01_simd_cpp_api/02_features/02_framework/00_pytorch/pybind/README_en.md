# pybind11 Custom Operator Direct Invocation Sample

## Overview

This sample demonstrates how to register a custom operator using pybind11 based on the Add operator.

## Supported Products

- Ascend 950PR/Ascend 950DT
- Atlas A3 Training Series Products/Atlas A3 Inference Series Products
- Atlas A2 Training Series Products/Atlas A2 Inference Series Products

## Directory Structure

```
├── pybind
│   ├── CMakeLists.txt        // Build project file
│   ├── add_custom.asc        // Ascend C operator implementation and pybind11 registration
│   └── add_custom_test.py    // PyTorch invocation script
```

## Sample Description

- Sample Functionality:

  Add calculation formula:

  ```
  z = x + y
  ```

- Sample Specifications:
  <table border="2" align="center">
  <caption>Table 1: AddCustom Sample Specifications</caption>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">AddCustom</td></tr>
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

- Custom Sample Registration:

  This sample defines a namespace named `ascendc_ops` in `add_custom.asc` and registers the `ascendc_add` function within it.

  pybind11 enables PyTorch framework to invoke sample Kernel programs, achieving integration and deployment of Ascend C samples in the PyTorch framework.

  `add_custom.asc` uses the `pybind11` library to wrap C++ code into a Python module. The implementation defines a pybind11 module named `m` containing a function named `ascendc_add`. This function is the same as `ascendc_ops::ascendc_add` and converts the C++ function to a Python function. For example:

  ```c++
  PYBIND11_MODULE(ascendc_ops, m)
  {
      m.doc() = "add_custom pybind11 interfaces";
      m.def("ascendc_add", &ascendc_ops::ascendc_add, "");
  }
  ```

  In the `ascendc_add` function, the current NPU stream is obtained through the `c10_npu::getCurrentNPUStream()` function, and the custom Kernel function `add_custom` is called through the kernel launcher `<<<>>>` to execute the sample on the NPU.

- Python Test Script

  In the `add_custom_test.py` invocation script, import the custom module `import ascendc_ops`, call the registered `ascendc_add` function, and verify the numerical correctness of the custom sample by comparing NPU output with CPU standard addition results.

## Build and Run

- Install PyTorch and Ascend Extension for PyTorch Plugin

  Refer to the [pytorch: Ascend Extension for PyTorch](https://gitcode.com/Ascend/pytorch) open source repository or [Ascend Extension for PyTorch Community](https://hiascend.com/document/redirect/Pytorch-index) installation instructions to select a supported `Python` version release and complete the installation of `torch` and `torch-npu`.

- Install Prerequisites

  ```bash
  pip3 install pybind11 expecttest
  ```

- Configure Environment Variables

  Select the appropriate command based on the CANN development kit [installation method](../../../../../../docs/en/quick_start.md#prepare&install) on your current environment.
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
  mkdir -p build && cd build;      # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;    # Build project
  python3 ../add_custom_test.py    # Execute sample
  ```

- Build Options

  | Option | Available Values | Description |
  | ----------------| -----------------------------| --------------------------------------------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products, dav-3510 corresponds to Ascend 950PR/Ascend 950DT |

- Execution Result

  The following output indicates successful accuracy comparison:

  ```bash
  Ran 1 test in **s
  OK
  ```