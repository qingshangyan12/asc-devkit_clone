# Shift Left Tensor API Sample

## Overview

This sample uses the Tensor API `operator<<` to shift 64 `uint32_t` elements by the corresponding `int32_t` values.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```plain
├── logical_compute
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data.
│   │   └── verify_result.py     // Verifies output data.
│   ├── CMakeLists.txt           // Build project file.
│   ├── data_utils.h             // Data input and output utilities.
│   ├── README.md
│   ├── README_En.md
│   └── logical_compute_tensor_api.asc  // Ascend C operator implementation and invocation sample.
```

## Sample Description

- Sample function:
  The sample shifts each input Tensor element by the corresponding register value:

  ```text
  output_i = input_i << shifts_i
  ```

- Sample specification:
  <table>
  <tr><td align="center">Sample Type (OpType)</td><td colspan="3" align="center">AIV Sample</td></tr>
  <tr><td rowspan="3" align="center">Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">input</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
  <tr><td align="center">shifts</td><td align="center">[64]</td><td align="center">int32_t</td></tr>
  <tr><td align="center">Output</td><td align="center">output</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
  <tr><td align="center">Kernel Function Name</td><td colspan="3" align="center">logical_compute_tensor_api</td></tr>
  </table>

- Sample implementation:

  1. Create ND Tensor views for GM/UB transfers and one-dimensional Tensor views for register computation in UB.
  2. Move the data and shifts from GM to UB.
  3. Load them as `reg_tensor<uint32_t>` and `reg_tensor<int32_t>` using `make_coord(offset)`.
  4. Import `operator<<`, evaluate `src << shift`, store the result in UB, and move it back to GM.
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
  logical_compute_tensor_api executed successfully.
  test pass! verified uint32 register shift-left.
  ```
