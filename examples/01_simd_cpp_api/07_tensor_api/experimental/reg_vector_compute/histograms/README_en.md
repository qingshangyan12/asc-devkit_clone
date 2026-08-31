# Histograms Tensor API Sample

## Overview

This sample uses the Tensor API `histograms` in high-half cumulative mode to process 256 `uint8_t` inputs and produce 128 `uint16_t` results.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```plain
├── histograms
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data.
│   │   └── verify_result.py     // Verifies output data.
│   ├── CMakeLists.txt           // Build project file.
│   ├── data_utils.h             // Data input and output utilities.
│   ├── README.md
│   ├── README_en.md
│   └── histograms.asc  // Ascend C operator implementation and invocation sample.
```

## Sample Description

- Sample function:
  The sample computes the cumulative histogram for the high-half range `[128, 255]`. The output initial value is 3:

  ```text
  output_i = 3 + count(input_j <= 128 + i), 0 <= i < 128
  ```

- Sample specification:
  <table>
  <tr><td align="center">Sample Type (OpType)</td><td colspan="3" align="center">AIV Sample</td></tr>
  <tr><td rowspan="2" align="center">Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
  <tr><td align="center">input</td><td align="center">[256]</td><td align="center">uint8_t</td></tr>
  <tr><td align="center">Output</td><td align="center">output</td><td align="center">[128]</td><td align="center">uint16_t</td></tr>
  <tr><td align="center">Kernel Function Name</td><td colspan="3" align="center">histograms</td></tr>
  </table>

- Sample implementation:

  1. Create ND Tensor views for GM/UB transfers and one-dimensional Tensor views for register computation in UB.
  2. Move the input from GM to UB and load it as `reg_tensor<uint8_t>` using `make_coord(offset)`.
  3. Initialize the `reg_tensor<uint16_t>` destination to 3 and configure `histogram_half::high` and `histogram_mode::cumulative`.
  4. Call `histograms<histogram_config>`, store the result in UB, and move it back to GM.
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
  histograms executed successfully.
  test pass! verified cumulative histogram for high half bins.
  ```
