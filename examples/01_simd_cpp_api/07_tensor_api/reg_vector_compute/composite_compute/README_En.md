# Fma Tensor API Sample

## Overview

This sample uses the Tensor API `fma` to perform fused multiply-add on three 64-element `float` inputs.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```plain
├── composite_compute
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data.
│   │   └── verify_result.py     // Verifies output data.
│   ├── CMakeLists.txt           // Build project file.
│   ├── data_utils.h             // Data input and output utilities.
│   ├── README.md
│   ├── README_En.md
│   └── composite_compute_tensor_api.asc  // Ascend C operator implementation and invocation sample.
```

## Sample Description

- Sample function:
  The sample performs fused multiply-add on three input Tensors:

  ```text
  output_i = input_a_i * input_b_i + input_c_i
  ```

- Sample specification:
  <table>
  <tr><td align="center">Sample Type (OpType)</td><td colspan="3" align="center">AIV Sample</td></tr>
  <tr><td rowspan="4" align="center">Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">input_a</td><td align="center">[64]</td><td align="center">float</td></tr>
  <tr><td align="center">input_b</td><td align="center">[64]</td><td align="center">float</td></tr>
  <tr><td align="center">input_c</td><td align="center">[64]</td><td align="center">float</td></tr>
  <tr><td align="center">Output</td><td align="center">output</td><td align="center">[64]</td><td align="center">float</td></tr>
  <tr><td align="center">Kernel Function Name</td><td colspan="3" align="center">composite_compute_tensor_api</td></tr>
  </table>

- Sample implementation:

  1. Create ND Tensor views for GM/UB transfers and one-dimensional Tensor views for register computation in UB.
  2. Move the three inputs from GM to UB by using `asc::te::copy`.
  3. Load the inputs as `reg_tensor<float>` using the one-dimensional coordinate `make_coord(offset)` and select 64 elements.
  4. Call `fma(input_a, input_b, input_c)`, store the result in UB, and move it back to GM.
  5. Invoke the kernel with `<<<>>>` and verify the output against the golden data.

## Build and Run

Run the following steps in the sample root directory to build and execute the sample.

- Configure Environment Variables

  Configure the environment variables according to the installation method of the CANN development toolkit. For details, see the [Quick Start Guide](../../../../../docs/zh/quick_start.md).

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Run the Sample

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DCANN_ASC_USE_EXPERIMENTAL=ON ..
  make -j
  python3 ../scripts/gen_data.py
  ./demo
  python3 ../scripts/verify_result.py
  ```

  To run the sample in NPU simulation mode, add `-DCMAKE_ASC_RUN_MODE=sim` to the CMake command.

  When switching the run mode, clear the CMake cache in the `build` directory before reconfiguring the project.

- Build Options

  | Option | Available Values | Description |
  | --- | --- | --- |
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution or NPU simulation. |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. `dav-3510` corresponds to Ascend 950PR/Ascend 950DT. |
  | `CANN_ASC_USE_EXPERIMENTAL` | `ON` (required), `OFF` (default) | Enables experimental ASC APIs. |

- Expected Result

  The following output indicates that the result matches the golden data:

  ```text
  composite_compute_tensor_api executed successfully.
  test pass! verified float fma.
  ```
