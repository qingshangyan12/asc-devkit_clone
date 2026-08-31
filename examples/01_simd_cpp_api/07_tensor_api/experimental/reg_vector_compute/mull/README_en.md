# Mull Tensor API Sample

## Overview

This sample uses the Tensor API `mull` to multiply two 64-element `uint32_t` inputs and produce the low and high 32-bit halves of each product.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```plain
├── mull
│   ├── scripts
│   │   ├── gen_data.py          // Generates input and golden data.
│   │   └── verify_result.py     // Verifies output data.
│   ├── CMakeLists.txt           // Build project file.
│   ├── data_utils.h             // Data input and output utilities.
│   ├── README.md
│   ├── README_en.md
│   └── mull.asc  // Ascend C operator implementation and invocation sample.
```

## Sample Description

- Sample function:

  ```text
  product_i = uint64(src0_i) * uint64(src1_i)
  low_i = product_i & 0xffffffff
  high_i = product_i >> 32
  ```

- Sample specification:

  <table>
    <tr><td align="center">Sample Type (OpType)</td><td colspan="3" align="center">AIV Sample</td></tr>
    <tr><td rowspan="3" align="center">Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td></tr>
    <tr><td align="center">src0</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">src1</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td rowspan="2" align="center">Output</td><td align="center">low</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">high</td><td align="center">[64]</td><td align="center">uint32_t</td></tr>
    <tr><td align="center">Kernel Function Name</td><td colspan="3" align="center">mull</td></tr>
  </table>

- Sample implementation:

  1. Create ND Tensor views for GM/UB transfers and one-dimensional Tensor views for register computation in UB.
  2. Move `src0` and `src1` from GM to UB and load them using the one-dimensional coordinate `make_coord(offset)`.
  3. Call `mull` with a 64-element mask. The function returns the low 32 bits and writes the high 32 bits to `high`.
  4. Store both results in UB and move them back to GM.
  5. Save the output files on the host and verify both 32-bit halves against the golden data.

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
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Operator execution mode: NPU execution or NPU simulation. |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. `dav-3510` corresponds to Ascend 950PR/Ascend 950DT. |
  | `CANN_ASC_USE_EXPERIMENTAL` | `ON` (required), `OFF` (default) | Enables experimental ASC APIs. |

- Expected Result

  The following output indicates that the result matches the golden data:

  ```text
  mull executed successfully.
  test pass! verified low and high uint32 mull results.
  ```
