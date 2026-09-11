# reduce Example

## Overview

This example uses the Memory vector compute interfaces [asc_repeat_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_repeat_reduce_sum.md) and [asc_datablock_reduce_sum](../../../../../docs/zh/api/SIMD-API/c_api/vector_compute/vector_reduce/asc_datablock_reduce_sum.md) to sum float data.

This example supports two sum-reduction scenarios, selected with the CMake build parameter `SCENARIO_NUM`.

<table>
<caption>Table 1: SCENARIO_NUM Parameter Description</caption>
<tr><td align="center">SCENARIO_NUM</td><td align="center">Sum-Reduction Scenario</td></tr>
<tr><td align="center">1</td><td align="center">Repeat reduction</td></tr>
<tr><td align="center">2</td><td align="center">DataBlock reduction</td></tr>
</table>

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.2.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.2.0 |

## Directory Structure

```text
reduce/
├── scripts/gen_data.py
├── CMakeLists.txt
├── data_utils.h
├── reduce.asc
├── README.md
└── README_en.md
```

## Example Description

This example loads 64 float input values from GM to UB, performs sum reduction, and stores the result back to GM. The host launches `reduce_kernel` with `<<<>>>` and compares all valid output values with `output/golden.bin` using an absolute tolerance of `1e-4`.

**Scenario 1: repeat reduction**

- Example functionality: Sums 64 float values within one repeat.
- Example specifications:

  <table>
  <caption>Table 2: Scenario 1 Example Specifications</caption>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">reduce</td></tr>

  <tr><td rowspan="3" align="center">Example Input</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 1]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">reduce_kernel</td></tr>
  </table>

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load 64 float values from GM to UB.
  - Compute: Calls `asc_repeat_reduce_sum` to sum the 64 values within one repeat.
  - Store: Calls `asc_copy_ub2gm_align` to store one float result from UB to GM.

**Scenario 2: DataBlock reduction**

- Example functionality: Separately sums the eight float values in each of eight 32-byte DataBlocks.
- Example specifications:

  <table>
  <caption>Table 3: Scenario 2 Example Specifications</caption>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">reduce</td></tr>

  <tr><td rowspan="3" align="center">Example Input</td></tr>
  <tr><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">x</td><td align="center">[1, 64]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td></tr>
  <tr><td align="center">y</td><td align="center">[1, 8]</td><td align="center">float</td><td align="center">ND</td></tr>

  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">reduce_kernel</td></tr>
  </table>

- Example implementation:
  - Load: Calls `asc_copy_gm2ub_align` to load 64 float values from GM to UB.
  - Compute: Calls `asc_datablock_reduce_sum` to sum the eight float values in each 32-byte DataBlock.
  - Store: Calls `asc_copy_ub2gm_align` to store eight float results from UB to GM.

The generator creates deterministic float input and the corresponding golden data for each scenario.

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
SCENARIO_NUM=1                                                                                  # Run scenario 1
mkdir -p build && cd build;                                                                       # Create and enter the build directory
cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # Build the project (default NPU mode)
python3 ../scripts/gen_data.py -scenario_num=$SCENARIO_NUM                                       # Generate test input and golden data
./demo                                                                                           # Run the compiled executable
```

- Build options

| Option | Values | Description |
|--------|--------|-------------|
| `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default) | NPU architecture: dav-2201 corresponds to Atlas A2 Training Series Products/Atlas A2 Inference Series Products and Atlas A3 Training Series Products/Atlas A3 Inference Series Products |
| `SCENARIO_NUM` | `1` (default), `2` | Example scenario: repeat reduction, DataBlock reduction |

- Execution result

The following output indicates that the golden-data comparison passed:

```text
test pass!
```
