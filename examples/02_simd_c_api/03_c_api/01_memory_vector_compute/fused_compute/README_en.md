# fused_compute Example

## Overview

This example uses the Memory vector compute interface [asc_leakyrelu](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_fused/asc_leakyrelu.md) to compute Leaky ReLU: `y = x` when `x > 0`, otherwise `y = 0.01 * x`.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.2.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.2.0 |

## Directory Structure

```text
fused_compute/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── element_wise_arithmetic.asc
├── README.md
└── README_en.md
```

## Example Description

This example contains one Leaky ReLU scenario. The kernel loads 512 half input values from GM to UB, computes each output value, and stores the results back to GM. The host launches `element_wise_arithmetic_kernel` with `<<<>>>` and compares every output element with `output/golden.bin`.

**Scenario: Leaky ReLU**

- Example functionality: Computes `y = x` when `x > 0`, or `y = 0.01 * x` when `x <= 0`.
- Example specifications:

  <table>
  <caption>Table 1: Example Specifications</caption>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">fused_compute</td></tr>

  <tr><td rowspan="3" align="center">Example Input</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 512]</td><td align="center">half</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 512]</td><td align="center">half</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">element_wise_arithmetic_kernel</td></tr>
  </table>

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load 512 half values from GM to UB.
  - Compute: Calls `asc_leakyrelu` with a half slope parameter of `0.01`.
  - Store: Calls `asc_copy_ub2gm_align` to store 512 half results from UB to GM.

The generator creates deterministic half input containing positive, negative, and zero values, and the corresponding golden data.

## Build and Run

Run the following steps in the root directory of this example.

- Configure environment variables

  Configure environment variables according to the CANN development kit [installation method](../../../../../docs/en/quick_start.md#prepare&install).

```bash
source ${install_path}/cann/set_env.sh
```

  > **Note:** `${install_path}` is the CANN package installation directory. The default is `/usr/local/Ascend` for the root user and `${HOME}/Ascend` for a non-root user.

- NPU execution

```bash
mkdir -p build && cd build;                                                                       # Create and enter the build directory
cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Build the project (default NPU mode)
python3 ../scripts/gen_data.py                                                                    # Generate test input and golden data
./demo                                                                                           # Run the compiled executable
```

- Build options

| Option | Values | Description |
|--------|--------|-------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default) | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
