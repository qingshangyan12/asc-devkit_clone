# MatrixTranspose Example

## Overview

This example uses matrix transpose to introduce memory access coalescing optimization in the Ascend C SIMD and SIMT hybrid programming scenario. The example includes 2 Case versions, starting from direct index transpose, then adjusting the data write-back method through UB relay to make GM reads and writes closer to sequential access, demonstrating global memory access optimization methods for matrix transpose under hybrid programming.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── simt_and_simd_matrix_transpose
│   ├── CMakeLists.txt         // Build project file
│   ├── matrix_transpose.asc   // Matrix transpose example implementation
│   ├── figures                // Image resources for README
│   └── README.md
```

## Example Description

- Computation formula:

  $$
  output(x, y) = input(y, x)
  $$

  - input is the input matrix, shape [H,W], data type float
  - output is the output matrix, shape [W,H], data type float
- Example specifications:
  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">transpose_kernel</td></tr>
  </table>

## Example Implementation

### Case Implementation Description

This example implements different memory access strategies by calling different SIMT VF functions, with each SIMT VF function corresponding to a specific Case version.

| Case   | Implementation Characteristics                                           | Called SIMT VF Function               | Optimization Feature              |
| ------ | -------------------------------------------------- | -------------------------- | --------------------- |
| Case 0 | Directly computes output coordinates according to the transpose formula, GM sequential read, non-sequential write   | simt_transpose_naive     | Direct index transpose version      |
| Case 1 | Relays through UB to swap read/write directions, GM reads and writes are closer to sequential access | simt_transpose_coalesced | UB relay + global memory access coalescing |

#### Thread Block Layout

This example uses SIMD and SIMT hybrid programming, where the kernel function internally calls `__simt_vf__` functions through `asc_vf_call`. To facilitate comparison of performance differences across versions, this example only supports square matrices, and the matrix width and height must be integer multiples of 32. All Cases use an identical thread block layout:

- This example flattens 2D tile coordinates to 1D `blockIdx.x`, and flattens the 32×32 elements within a tile to 1D `threadIdx.x`.
- Each tile is 32×32 in size, with one block processing one tile. Block count is `bn = grid_x * grid_y`, where `grid_x = matrix_width / TILE_DIM`, `grid_y = matrix_height / TILE_DIM`.

Based on the above splitting method, `blockIdx.x` represents the flattened 1D tile index. Through integer division and modulo of `grid_width`, the 2D tile coordinates `(block_row, block_col)` responsible for the current block can be recovered. `threadIdx.x` represents the flattened 1D element index within the tile. Through integer division and modulo of `TILE_DIM`, the local coordinates `(tile_row, tile_col)` of the element processed by the thread within the tile can be recovered.

```cpp
int block_row = blockIdx.x / grid_width;
int block_col = blockIdx.x % grid_width;

int tile_row = threadIdx.x / TILE_DIM;
int tile_col = threadIdx.x % TILE_DIM;
```

The figure below provides a more intuitive mapping diagram. The left side shows the 1024×1024 matrix divided into 32×32 tiles. The right side shows one tile that a block needs to process, with size 32×32.

<img src="./figures/blockMapping.png" width="60%">

Taking the green element in the figure as an example, its input coordinates in GM can be directly derived using built-in kernel variables.

- Tile coordinates are (2,1), corresponding to:
  - block_row = 2
  - block_col = 1
  - blockIdx.x = block_row × grid_width + block_col = 2 × 32 + 1 = 65
- Element local coordinates within the tile are (2,29), corresponding to:
  - threadIdx.x = tile_row × TILE_DIM + tile_col = 2 × 32 + 29 = 93

Combined with the index computation in the code:

```cpp
int input_row = block_row * TILE_DIM + tile_row;
int input_col = block_col * TILE_DIM + tile_col;
int index_in = input_col + width * input_row;
```

Substituting the variable values in this example:

- block_row = 65 / 32 = 2
- block_col = 65 % 32 = 1
- tile_row = 93 / 32 = 2
- tile_col = 93 % 32 = 29
- input_row = block_row × TILE_DIM + tile_row = 2 × 32 + 2 = 66
- input_col = block_col × TILE_DIM + tile_col = 1 × 32 + 29 = 61

Therefore, the input element coordinate processed by this thread in GM is:

- input[input_row,input_col] = input[66,61]

Substituting into the 1D address formula:

- index_in = input_col + width × input_row = 61 + 1024 × 66

### Performance Metric Description

| Metric                | Description                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(μs)|Total Task duration, including scheduling time to the accelerator, execution time on the accelerator, and completion response time.|
| aiv_time|Theoretical execution time of the Task on AI Vector Core, in μs.|
| aiv_vec_time(μs) | vec type instruction (vector computation instructions) duration, in μs. |
| aiv_vec_ratio | Ratio of vec type instruction cycle count to total cycle count. |
| aiv_scalar_time(μs) | scalar type instruction (scalar computation instructions) duration, in μs. |
| aiv_scalar_ratio | Ratio of scalar type instruction cycle count to total cycle count. |
| aiv_mte2_time(μs) | mte2 type instruction (GM->UB transfer instructions) duration, in μs. |
| aiv_mte2_ratio | Ratio of mte2 type instruction cycle count to total cycle count. |
| aiv_mte3_time(μs) | mte3 type instruction (UB->GM transfer instructions) duration, in μs. |
| aiv_mte3_ratio | Ratio of mte3 type instruction cycle count to total cycle count. |

### Case 0: Direct Index Transpose Version

**Example Objective**: Implement basic matrix transpose functionality as the duration comparison baseline for subsequent optimization versions

**Core Implementation**:

- Each block processes one 32×32 tile.
- Each SIMT thread processes 1 element within the tile.
- The thread first reads the input element from GM using original coordinates, then computes the transposed output position for that element, and writes the data directly to the transposed GM address.
- GM read direction is sequential, GM write-back direction is non-sequential.

The figure below shows the data flow for Case 0, with red highlighting the elements processed by one Warp when reading GM and writing GM. Threads in the same Warp read one row of elements from the tile in GM input and write back one column of elements to the tile in GM output. When reading GM input, adjacent threads access sequential element addresses, which is sequential read. When writing back to output, adjacent threads are scattered across different rows of the output matrix, which is non-sequential write. Therefore, the core issue of this version is that the transposed write-back addresses are no longer sequential, which typically significantly affects overall throughput.

<img src="./figures/case0.png" width="60%">

**Key Code**:

```cpp
int index_in = input_col + width * input_row;
int index_out = input_row + height * input_col;

output[index_out] = input[index_in];
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 63.731 | 62.810 | 58.143 | 0.926 | 2.439 | 0.039 | 0.059 | 0.001 | 0.02 | 0.0 |

**Analysis**:

- Case 0 Task Duration is 63.731μs, as the direct index transpose version, serving as the comparison baseline for subsequent optimization versions.
- This version still has sequential GM reads, but GM write-back is cross-row, non-sequential. Write requests from the same Warp are difficult to merge efficiently, so overall duration is primarily limited by the write-back memory access pattern.

---

### Case 1: UB Relay + Global Memory Access Coalescing Transpose Version

**Optimization Objective**: Adjust the transpose write-back method through UB relay to make GM reads and writes closer to sequential access, reducing end-to-end duration

**Core Implementation**:

- Transfer input data tiles from GM to UB.
- Each thread fetches data from UB, with one Warp reading one column of elements from the tile in UB.
- Write the fetched values back to the transposed positions in GM, with one Warp writing one column of elements read from UB to one row of the tile in GM output.

The figure below shows the data flow for Case 1, with red and yellow highlighted elements showing the elements processed by one Warp's threads when reading GM and writing GM. When reading GM input, the entire tile is transferred to UB following the GM layout. When writing GM output, one Warp's threads read one column of elements from UB and write to their corresponding transposed positions.

<img src="./figures/case1.png" width="60%">

Unlike Case 0, where threads "directly write input elements to the transposed GM position" causing adjacent threads to scatter across different rows of the output matrix, Case 1 first transfers the input tile to UB following the GM layout through `asc_copy_gm2ub_align`, then SIMT threads read from UB in the transposed direction and write back to GM. Therefore, the core benefit of this version is: transferring the originally non-sequential GM write-back to UB-side transpose reading, exchanging for a "sequential read, sequential write" access pattern on the GM side, with overall duration significantly lower than Case 0.

**Key Code**:

```cpp
uint32_t tile_row = threadIdx.x / TILE_DIM;
uint32_t tile_col = threadIdx.x % TILE_DIM;

output_tile[tile_col + tile_row * height] = input_tile[tile_col * TILE_DIM + tile_row];
```

**Optimization Methods**:

- Use UB as a tile relay area, transferring the non-sequential GM writes from Case 0 to UB-side transpose reads.
- Adjust the output tile block coordinates so that when writing back to GM, the same Warp is closer to sequential row writes.
- Ensure GM to UB transfer completion before launching SIMT VF processing through `asc_sync_notify()` / `asc_sync_wait()`.

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| 30.602 | 29.6 | 16.942 | 0.572 | 3.404 | 0.115 | 6.912 | 0.234 | 0.022 | 0.001 |

**Analysis**:

- Compared to Case 0 direct index transpose version, Case 1 Task Duration decreases from 63.731μs to 30.602μs, a duration reduction of approximately 52.0%.
- Based on Task Duration, Case 1 overall is approximately 2.08x the performance of Case 0, indicating that improving GM write-back sequentiality through UB relay results in significant end-to-end duration reduction.
- Case 1 reduces the costly non-sequential GM write-back overhead from Case 0, but the code still includes tile transfer from GM to UB, pipeline synchronization, and SIMT thread transpose-direction reads from UB, so the optimized Task Duration will not equal the ideal duration of pure sequential GM reads and writes.

---

## Performance Comparison Summary

### Ascend 950PR Performance Data

**Comprehensive Optimization Effect**:

- Through memory access coalescing optimization from Case 0 to Case 1, example Task Duration decreases from 63.731μs to 30.602μs, a duration reduction of approximately 52.0%.
- Case 1 relative to Case 0 performance improvement is approximately 2.08x, indicating that improving GM write-back sequentiality through UB relay provides significant end-to-end duration benefits.

| Case version | Task Duration(μs) | End-to-End Duration vs Case 0 | Optimization Point                           |
| ------------ | ----------------- | -------------------- | -------------------------------- |
| Case 0       | 63.731            | **1x**         | Direct index transpose, GM sequential read, non-sequential write |
| Case 1       | 30.602            | **2.08x**      | UB relay, global memory access coalescing             |

## Tuning Recommendations

1. **Prioritize GM memory access sequentiality**: Matrix transpose has very small computation volume. End-to-end duration is primarily affected by read/write memory access patterns.
2. **Use UB relay to improve write-back patterns**: When direct transpose causes non-sequential GM writes, transfer non-sequential access to the UB side in exchange for sequential GM reads and writes.
3. **Pay attention to transfer and pipeline synchronization overhead**: UB relay requires transferring from GM to UB and ensuring tile data is readable by SIMT threads through pipeline synchronization. During optimization, balance GM memory access benefits, UB transpose reads, and pipeline synchronization overhead.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit in the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in this example directory.
  ```bash
  SCENARIO_NUM=0                       # Select execution scenario, options 0-1
  mkdir -p build && cd build;          # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project
  ./demo                               # Run the example
  ```

  Build option description

  | Option             | Values      | Description              |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `0`-`1` | Example type, default is 0 |

  The following execution result indicates that the accuracy comparison succeeded.


  ```text
  [Success] Case accuracy is verification passed.
  ```

## Performance Analysis

Use the `msprof` tool to obtain detailed performance data:

```bash
msprof ./demo   # Analyze case performance
```

A PROF_ prefixed folder is generated in the current directory. The `mindstudio_profiler_output` directory stores Host and Device performance data summaries. Performance data analysis is recommended to view files in this directory.

```bash
PROF_xxxx_XXXXXX
├── device_{id}
└── host
└── mindstudio_profiler_log
└── mindstudio_profiler_output    # Stores Host and Device performance data summaries
    ├── msprof_*.json
    ├── xx_*.csv
    └── README.txt
```

View the specific performance analysis results:

```bash
# View Task Duration and various data
cat ./PROF_*/mindstudio_profiler_output/op_summary_*.csv
```
