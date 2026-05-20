# MatrixTranspose Example

## Overview

This example uses matrix transpose to demonstrate memory access coalescing optimization in Ascend C SIMD and SIMT hybrid programming scenarios. The example contains 2 kernel versions, starting from direct index transpose, then adjusting the data write-back method through UB transfer to make GM read/write access closer to continuous access, demonstrating the global memory access optimization method for matrix transpose in hybrid programming.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── matrix_transpose
│   ├── CMakeLists.txt         // Build project file
│   ├── matrix_transpose.asc   // Matrix transpose example implementation
│   ├── figures                // Figures
│   └── README.md
```

## Example Description

- Computation Formula:

  $$
  output(x, y) = input(y, x)
  $$

  - input is the input matrix with shape [H,W] and data type float
  - output is the output matrix with shape [W,H] and data type float
- Example Specifications:
  <table>
  <tr><td rowspan="1" align="center">Example Type(OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">transpose_naive_kernel / transpose_coalesced_kernel</td></tr>
  </table>

## Example Implementation

### Case Implementation Description

This example implements different memory access strategies through two independent kernels, with each kernel corresponding to a specific Case version.

| Case   | Implementation Feature                                         | Kernel Function Used          | Optimization Feature              |
| ------ | ------------------------------------------------------------ | ----------------------------- | --------------------------------- |
| Case 0 | Calculate output coordinates directly according to transpose formula, GM continuous read, non-continuous write | transpose_naive_kernel     | Direct index transpose version      |
| Case 1 | Transfer through UB temporary storage and adjust read/write direction, GM read/write closer to continuous access | transpose_coalesced_kernel | UB transfer + global memory access coalescing |

#### Thread Block Layout

This example uses SIMD and SIMT hybrid programming, with kernel functions calling `__simt_vf__` functions through `asc_vf_call` internally. To facilitate comparison of performance differences between versions, this example only supports square matrices, and the matrix width and height must be multiples of 32. All Cases use the same thread block layout:

- This example flattens 2D tile coordinates to 1D `blockIdx.x`, and flattens 32×32 elements within a tile to 1D `threadIdx.x`.
- Each tile size is 32×32, and one block processes one tile. The number of blocks is `bn = grid_x * grid_y`, where `grid_x = matrix_width / TILE_DIM`, `grid_y = matrix_height / TILE_DIM`.

Based on the above partitioning method, `blockIdx.x` represents the flattened 1D tile number. By dividing and taking modulo of `grid_width`, you can restore the 2D tile coordinates `(block_row, block_col)` that the current block is responsible for. `threadIdx.x` represents the flattened 1D element number within the tile. By dividing and taking modulo of `TILE_DIM`, you can restore the local coordinates `(tile_row, tile_col)` of the element processed by the thread within the tile.

```cpp
int block_row = blockIdx.x / grid_width;
int block_col = blockIdx.x % grid_width;

int tile_row = threadIdx.x / TILE_DIM;
int tile_col = threadIdx.x % TILE_DIM;
```

The figure below shows a more intuitive mapping. The left side shows a 1024×1024 matrix divided into 32×32 tiles. The right side shows one tile that one block needs to process, with a size of 32×32.

<img src="./figures/blockMapping.png" width="60%">

Take the green element in the figure as an example. You can directly use built-in variables in the kernel to derive its input coordinates in GM.

- The tile coordinates are (2,1), corresponding to:
  - block_row = 2
  - block_col = 1
  - blockIdx.x = block_row × grid_width + block_col = 2 × 32 + 1 = 65
- The local coordinates of the element in the tile are (2,29), corresponding to:
  - threadIdx.x = tile_row × TILE_DIM + tile_col = 2 × 32 + 29 = 93

Combine with the index calculation in the code:

```cpp
int input_row = block_row * TILE_DIM + tile_row;
int input_col = block_col * TILE_DIM + tile_col;
int index_in = input_col + width * input_row;
```

Substitute the variable values in this example:

- block_row = 65 / 32 = 2
- block_col = 65 % 32 = 1
- tile_row = 93 / 32 = 2
- tile_col = 93 % 32 = 29
- input_row = block_row × TILE_DIM + tile_row = 2 × 32 + 2 = 66
- input_col = block_col × TILE_DIM + tile_col = 1 × 32 + 29 = 61

Therefore, the input element coordinates processed by this thread in GM are:

- input[input_row,input_col] = input[66,61]

If you continue to substitute into the 1D address formula, you get:

- index_in = input_col + width × input_row = 61 + 1024 × 66

### Performance Metrics Description

| Metric                | Description                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(us)   | Overall task time, including time scheduled to the accelerator, execution time on the accelerator, and response end time                  |
| aiv_time(us)        | Theoretical execution time of the Task on AI Vector Core, in us                                            |
| aiv_total_cycles    | After the Task is assigned to each AI Vector Core compute unit, the total execution cycles on each AI Vector Core compute unit |
| aiv_vec_time(us)    | vec type instruction (vector computation instruction) time, in us                                               |
| aiv_vec_ratio       | The ratio of vec type instruction (vector computation instruction) cycles in total cycles                           |
| aiv_scalar_time(us) | scalar type instruction (scalar computation instruction) time, in us                                            |
| aiv_scalar_ratio    | The ratio of scalar type instruction (scalar computation instruction) cycles in total cycles                        |

Except for Task Duration, all other metrics in this example show the average value of performance metrics across all blocks.

### Case 0: Direct Index Transpose Version

**Example Goal**: Implement basic matrix transpose function as the time-consuming baseline for subsequent optimized versions

**Core Implementation**:

- Each block processes one 32×32 tile
- Each SIMT thread processes 1 element in the tile
- The thread first reads the input element from GM according to the original coordinates, then calculates the output position after transpose of this element, and writes the data directly to the transposed GM address
- GM read direction is continuous, GM write-back direction is not continuous

The figure below shows the data flow of Case 0, with red marking showing the elements processed by one Warp when reading GM and writing GM. For threads in the same Warp, they will read one row of elements of the tile in the GM input, and write back to one column of the tile in the GM output. When reading GM input, the element addresses accessed by adjacent threads are continuous, which is continuous read. When writing back to output, adjacent threads are scattered to different rows of the output matrix, which is non-continuous write. Therefore, the core issue of this version is that the write-back address after transpose is no longer continuous, which usually significantly affects overall throughput.

<img src="./figures/case0.png" width="60%">

**Key Code**:

```cpp
int index_in = input_col + width * input_row;
int index_out = input_row + height * input_col;

output[index_out] = input[index_in];
```

**Performance Data**:

| Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      70.304      |    4.257    |     7024.463     |      4.115      |     0.967     |        0.131        |      0.030      |

**Analysis**:

- Case 0 has a Task Duration of 70.304us, as a direct index transpose version, which is the comparison baseline for subsequent optimized versions
- This version has continuous GM read, but GM write-back is cross-row, non-continuous write, and write requests from the same Warp are difficult to efficiently coalesce, so the overall time consumption is mainly limited by the write-back memory access pattern

---

### Case 1: UB Transfer + Global Memory Access Coalescing Transpose Version

**Optimization Goal**: Adjust transpose write-back method through UB transfer to make GM read/write closer to continuous access, reducing end-to-end time consumption

**Core Implementation**:

- Each thread reads 1 element from the tile in GM, and one Warp reads one row of elements in the tile
- Write the element to the tile in UB according to the original coordinates, and one Warp writes the read row to one row of the tile in UB
- After synchronization, each thread fetches data from UB, and one Warp reads one column of elements in the tile from UB
- Write the fetched value back to the transposed position in GM, and one Warp writes the read column of elements from UB to one row of the tile in the GM output

The figure below shows the data flow of Case 1, with red and yellow marked elements showing the elements processed by a Warp's threads when reading GM and writing GM. When reading GM input, the entire tile is transferred to UB according to GM layout. When writing GM output, a Warp's threads read one column of elements from UB and write back to their corresponding transposed positions.

<img src="./figures/case1.png" width="60%">

Different from Case 0, in Case 0, threads "directly write input elements to transposed GM positions", so adjacent threads are scattered to different rows of the output matrix. In Case 1, threads first put data into UB, transferring the originally non-continuous global write access to non-continuous read within UB. Therefore, the core benefit of this version is: although one UB read/write and one synchronization are added, it exchanges for a "read continuous, write also continuous" access pattern on the GM side, and the overall time consumption is usually significantly lower than Case 0.

**Key Code**:

```cpp
int tile_row = threadIdx.x / TILE_DIM;
int tile_col = threadIdx.x % TILE_DIM;

tile[tile_row][tile_col] = input[index_in];
asc_syncthreads();

int block_row = blockIdx.x / grid_width;
int block_col = blockIdx.x % grid_width;

int output_row = block_col * TILE_DIM + tile_row;
int output_col = block_row * TILE_DIM + tile_col;
int index_out = output_col + output_row * height;

output[index_out] = tile[tile_col][tile_row];
```

**Optimization Methods**:

- Use UB as tile transfer area to transfer the non-continuous GM write in Case 0 to UB side access
- Adjust the block coordinates of the output tile to make the same Warp closer to row-continuous write when writing back to GM
- Ensure the entire tile is written to UB before executing transpose direction read through `asc_syncthreads()`

**Performance Data**:

| Task Duration(us) | aiv_time(us) | aiv_total_cycles | aiv_vec_time(us) | aiv_vec_ratio | aiv_scalar_time(us) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      44.440      |    2.441    |     4027.577     |      2.295      |     0.941     |        0.134        |      0.054      |

**Analysis**:

- Compared with the direct index transpose version of Case 0, the Task Duration of Case 1 decreased from 70.304us to 44.440us, with a time consumption reduction of about 36.8%
- Calculated by Task Duration, Case 1 overall has about 1.58x performance of Case 0, indicating that after improving GM write-back continuity through UB transfer, the end-to-end time consumption has significant improvement
- Case 1 still requires additional UB read/write and synchronization, so the optimized Task Duration has not dropped to the ideal level of pure continuous memory access; this part of time consumption is the necessary overhead brought by UB transfer in exchange for GM memory access coalescing

---

## Performance Comparison Summary

### Ascend 950PR Performance Data

**Overall Optimization Effect**:

- Through memory access coalescing optimization from Case 0 to Case 1, the example Task Duration decreased from 70.304us to 44.440us, with a time consumption reduction of about 36.8%
- Case 1 has about 1.58x performance improvement relative to Case 0, indicating that after improving GM write-back continuity through UB transfer, the end-to-end time consumption has significant benefit

| Case version | Task Duration(us) | End-to-end time relative to Case 0 | Optimization Point                           |
| ------------ | ----------------- | -------------------- | -------------------------------- |
| Case 0       | 70.304            | **1x**         | Direct index transpose, GM continuous read, non-continuous write |
| Case 1       | 44.440            | **1.58x**      | UB transfer, global memory access coalescing             |

## Tuning Suggestions

1. **Prioritize GM Memory Access Continuity**: Matrix transpose has very small computation amount, and end-to-end time consumption is mainly affected by read/write memory access patterns.
2. **Use UB Transfer to Improve Write-back Pattern**: When direct transpose causes non-continuous GM write, you can transfer non-continuous access to the UB side in exchange for continuous GM read/write.
3. **Pay Attention to Synchronization Overhead**: UB transfer requires synchronization to ensure tile data integrity. When optimizing, you need to pay attention to the balance between memory access benefit and synchronization and UB read/write overhead.

## Build and Run

Execute the following steps in the root directory of this example to build and run the example.

- Configure Environment Variables
  Select the corresponding environment variable configuration command based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit package on the current environment.

  - Default path, root user installs CANN software package

    ```bash
    source /usr/local/Ascend/cann/set_env.sh
    ```

  - Default path, non-root user installs CANN software package

    ```bash
    source $HOME/Ascend/cann/set_env.sh
    ```

  - Specified path install_path, install CANN software package

    ```bash
    source ${install_path}/cann/set_env.sh
    ```

- Example Execution

  ```bash
  SCENARIO_NUM=0                       # Select execution scenario, available options 0-1
  mkdir -p build && cd build;          # Create and enter build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build project
  ./demo                               # Execute example
  ```

- Build Options

  | Option             | Available Values      | Description              |
  | ---------------- | ----------- | ----------------- |
  | `SCENARIO_NUM` | `0`-`1` | Example type, default is 0 |

  The execution result is shown below, indicating that the accuracy comparison passed.


  ```text
  [Success] Case accuracy is verification passed.
  ```

## Performance Analysis

Use the `msprof` tool to obtain detailed performance data:

```bash
msprof op ./demo   # Analyze case performance
```

A folder named "OPPROF_{timestamp}_XXX" will be generated in the default directory. The performance data folder structure example is as follows:

```text
├──dump                       # Raw performance data, users do not need to pay attention
├──ArithmeticUtilization.csv  # cube/vector instruction cycle ratio
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1 and main memory read/write bandwidth rate
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth rate
├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth rate
├──OpBasicInfo.csv            # Operator basic information
├──PipeUtilization.csv        # Collect compute unit and transfer unit time consumption and ratio
├──ResourceConflictRatio.csv  # The ratio of bank group, bank conflict and resource conflict on UB in all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```