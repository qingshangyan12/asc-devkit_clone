# MatrixTranspose SIMD and SIMT Hybrid Memory Access Optimization Path Sample

## Overview

This sample uses matrix transpose to describe memory access optimization in Ascend C SIMD and SIMT hybrid programming. The sample starts from direct global memory (GM) transpose. It then introduces Memory Transfer Engine (MTE) movement, Unified Buffer (UB) staging, and 32x32 tiling. It compares two Thread Block mapping methods: launching Thread Blocks by tile groups and fixing the Thread Block count. It reduces bank conflicts by using UB padding. Finally, it uses double buffer to run the MTE2 movement pipeline and the SIMT Vector Function (VF) pipeline in parallel. This path shows the tuning process for SIMD and SIMT hybrid matrix transpose.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```text
├── simd_simt_matrix_transpose
│   ├── figures                     // Image resources for README
│   ├── CMakeLists.txt              // Build project file
│   ├── matrix_transpose.asc        // SIMD and SIMT hybrid matrix transpose optimization path implementation
│   └── README.md
```

## Sample Description

- Computation formula:

  ```text
  output(x, y) = input(y, x)
  ```

  - input is the input matrix. Its shape is [H, W], and its data type is float.
  - output is the output matrix. Its shape is [W, H], and its data type is float.

- Sample specifications:

  <table>
  <tr><td rowspan="1" align="center">Sample Type (OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">transpose_naive_kernel / transpose_ub_2tile_kernel / transpose_ub_pad_kernel</td></tr>
  </table>

## Sample Implementation

### Case Implementation Description

This sample constructs an optimization path through five cases. The kernel, Thread Block count, threads per Thread Block, and the main change introduced in each case are shown in the following table.

| Case | Kernel | Thread Block Count | Threads per Thread Block | Change Introduced in This Step |
| ---- | ------ | ------------------ | ------------------------ | ------------------------------- |
| 0 | `transpose_naive_kernel` | `core` | 2048 | Directly perform transpose in GM |
| 1 | `transpose_ub_2tile_kernel` | `ceil(tiles / 2)` | 2048 | Introduce MTE movement, UB staging, and 32x32 tiling. Set the Thread Block count by tile groups |
| 2 | `transpose_ub_2tile_kernel` | `core` | 2048 | Fix the Thread Block count to the hardware vector core count. One Thread Block loops over multiple tile groups |
| 3 | `transpose_ub_pad_kernel` | `core` | 2048 | Add 8 columns of padding to the UB tile to reduce bank conflicts during the transpose read phase |
| 4 | `transpose_ub_pad_kernel` | `core` | 2048 | Use double buffer rotation to run the MTE2 movement pipeline and the SIMT VF pipeline in parallel |

Here, `core` is the hardware vector core count. The runtime queries this value by using `aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`. The performance data in this sample was collected in a test environment where `core=64`. `tiles` is the total tile count `(W/32) x (H/32)`. When processing a 1024x1024 matrix, the matrix is divided into 32x32 tiles, so `tiles=1024`.

#### Thread Block to Tile Mapping

Starting from Case 1, the matrix is processed in 32x32 tiles. The left side of the following figure shows a 1024x1024 matrix divided into 32x32 tiles. The right side shows the thread mapping inside one 32x32 tile. The in-tile column index is denoted by `tx`, and the in-tile row index is denoted by `ty`.

**Figure 1: Thread Block to tile mapping**

<img src="./figures/block_mapping.png" width="60%">

Taking the green element in the figure as a reference, its input coordinates in GM can be derived from the tile coordinates and the local coordinates inside the tile.

- The tile coordinates of the green element are `(2, 1)`. For a 1024x1024 matrix, `tiles_x = 1024 / 32 = 32`. Therefore, `tile_id = 2 * 32 + 1 = 65`.
- The local coordinates of the green element inside the tile are `(2, 29)`, corresponding to `ty = 2` and `tx = 29`.

Substitute these values into the coordinate computation:

```cpp
uint32_t tile_y = tile_id / tiles_x;                     // 65 / 32 = 2
uint32_t tile_x = tile_id - tile_y * tiles_x;             // 65 - 2 * 32 = 1
uint32_t x_index = tile_x * TILE_DIM + tx;                // 1 * 32 + 29 = 61
uint32_t y_index = tile_y * TILE_DIM + ty;                // 2 * 32 + 2  = 66
uint32_t index = x_index + width * y_index;              // 61 + 1024 * 66
```

Therefore, the input element coordinates processed by this thread in GM are `input[66,61]`.

#### SIMD and SIMT Hybrid Data Flow

In Case 1 and later transpose paths, the Vector-side MTE moves data from GM to UB. Then `asc_vf_call` invokes a SIMT VF function to complete SIMT thread-level transpose reads and GM writes.

The core idea of UB staging is shown in the following figure. In the GM read phase, the kernel loads data into UB continuously along the row direction of the input matrix. In the GM write-back phase, the output tile coordinates are exchanged so that the same Warp writes one row of the output matrix. The original non-contiguous GM write is transferred to a transpose-direction read inside UB.

**Figure 2: UB staging transpose data flow**

<img src="./figures/ub_transpose_dataflow.png" width="60%">

### Performance Metrics

| Metric | Description |
| ------ | ----------- |
| Task Duration(μs) | Total task duration, including scheduling time to the accelerator, execution time on the accelerator, and response completion time |
| aiv_time(μs) | Theoretical execution time of the task on the AI Vector Core, in μs |
| aiv_total_cycles | Total execution cycles on each AI Vector Core compute unit after the task is assigned |
| aiv_vec_time(μs) | Duration of vec-type instructions, in μs |
| aiv_vec_ratio | Ratio of vec-type instruction cycles to total cycles |
| aiv_scalar_time(μs) | Duration of scalar-type instructions, in μs |
| aiv_scalar_ratio | Ratio of scalar-type instruction cycles to total cycles |

Except for Task Duration, all other metrics are averages across all Thread Blocks.

### Case 0: Direct Global Memory Transpose

**Optimization Goal**: Directly perform coordinate transformation in GM and observe the latency caused by writing to transposed addresses.

**Core Implementation**: This case does not use tiling or UB. SIMT threads read `input(row, col)` and directly write to the transposed position `output(col, row)`. The GM read is contiguous, while the write address strides across rows by column, making the write non-contiguous.

```cpp
uint32_t row = i / width;
uint32_t col = i - row * width;
// input(row,col) -> output(col,row)
output[col * height + row] = input[i];
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      36.140       |    33.793    |     55757.9      |      33.338      |     0.987     |        0.443        |      0.013       |

**Analysis**:

Case 0 keeps GM reads contiguous and writes data directly to the transposed output position. As a result, write addresses of adjacent threads in the same Warp are distributed across different rows of the output matrix. Matrix transpose involves little computation, so the main bottleneck of this version is non-contiguous GM writes. Later optimizations move the non-contiguous access from GM to UB, so that GM reads and writes become contiguous access patterns.

---

### Case 1: MTE Moves Data to UB Before Transpose Write-Back, and Each Thread Block Processes Two 32x32 Tiles

**Optimization Goal**: Introduce MTE movement, UB staging, and 32x32 tiling. This converts the non-contiguous global memory write in Case 0 into UB access and makes GM reads and writes as contiguous as possible. This case sets the Thread Block count by tile groups. Each Thread Block processes two 32x32 tiles.

**Core Implementation**: The matrix is divided into 32x32 tiles. The total tile count is `tiles=(W/32) x (H/32)`. In this case, each Thread Block launches 2048 threads and can process two tiles at the same time. Therefore, the Thread Block count is set to `ceil(tiles / 2)`. The Vector-side MTE continuously loads each tile along the row direction of the input matrix. The SIMT VF reads UB in the transpose direction and continuously writes back to GM along the row direction of the output matrix.

```cpp
__ubuf__ float tile[TILES_PER_BLOCK][TILE_DIM][TILE_DIM];

copy_gm_tile_to_ub(&tile[local_tile][0][0], input + tile_offset, width);
sync_mte2_to_v();
asc_vf_call<simt_store_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                              output, &tile[0][0][0], width, height, tiles_x,
                              tile_base, total_tiles);
sync_v_to_mte2();
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      23.215       |    2.281     |      3764.3      |      1.672       |     0.735     |        0.184        |      0.078       |

**Analysis**:

Case 1 uses UB staging to convert non-contiguous GM writes into transpose reads inside UB. Task Duration decreases from 36.140μs in Case 0 to 23.215μs, a latency reduction of about 35.8%. This version launches Thread Blocks by tile groups. A 1024x1024 matrix contains 1024 tiles, which correspond to 512 Thread Blocks. This count is much larger than the hardware vector core count. Extra Thread Block launch and scheduling overhead is reflected in Task Duration. Therefore, the next step optimizes the mapping from Thread Blocks to tiles.

---

### Case 2: MTE Moves Data to UB Before Transpose Write-Back, with a Fixed Thread Block Count

**Optimization Goal**: Change the Thread Block count in Case 1 from the tile group count to the hardware vector core count. Each Thread Block loops over multiple tile groups, which reduces Thread Block launch and scheduling overhead.

**Core Implementation**: Each Thread Block launches 2048 threads with the configuration `dim3(32,64,1)`. Each loop iteration processes two contiguous 32x32 tiles at the same time. Threads with `threadIdx.y=0..31` process the first tile, and threads with `threadIdx.y=32..63` process the second tile. The next iteration jumps to the subsequent tile group by `gridDim.x * 2`.

```cpp
uint32_t loop_start = block_idx * TILES_PER_BLOCK;
uint32_t loop_step = block_num * TILES_PER_BLOCK;

for (uint32_t tile_base = loop_start; tile_base < total_tiles; tile_base += loop_step) {
    copy_gm_tile_to_ub(&tile[local_tile][0][0], input + tile_offset, width);
    sync_mte2_to_v();
    asc_vf_call<simt_store_2tile>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                  output, &tile[0][0][0], width, height, tiles_x,
                                  tile_base, total_tiles);
    sync_v_to_mte2();
}
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      18.696       |    17.512    |     28894.9      |      13.509      |     0.772     |        0.701        |      0.040       |

**Analysis**:

Case 2 fixes the Thread Block count to the hardware vector core count. Compared with Case 1, which launches 512 Thread Blocks by tile groups, Case 2 launches and schedules fewer Thread Blocks. Task Duration decreases from 23.215μs to 18.696μs, a latency reduction of about 19.5%. Later versions continue to use 2048 threads and two tiles per iteration. The optimization focus then moves to UB access overhead.

---

### Case 3: MTE Moves Data to UB Before Transpose Write-Back with Padding

**Optimization Goal**: Reduce bank conflicts during the transpose read phase by adding UB padding on top of Case 2.

**Core Implementation**:

- The Thread Block layout and tile mapping are the same as those in Case 2.
- Each Thread Block still processes two 32x32 tiles per iteration.
- The physical UB tile layout changes from 32x32 to 32x40.
- The SIMT VF still reads only the valid 32 columns.

In SIMT scenarios, a common way to handle bank conflicts during transpose reads from UB is to add padding in the column direction of the tile. This padding makes elements in the same column staggered in the physical UB address space. For a 32x32 float tile, adding one subbank width, that is, 2 columns of padding, forms a 32x34 UB layout and avoids subbank conflicts.

In SIMD and SIMT hybrid programming, the Vector-side MTE moves data into UB. When MTE moves a padded two-dimensional array, the address stride between adjacent rows in UB also needs to meet the alignment requirement. The row stride of a 32x34 layout is `34 * sizeof(float) = 136B`, which does not meet the 32B alignment requirement. Therefore, this sample sets the padding column count to 8 and uses a 32x40 layout. The row stride is `40 * sizeof(float) = 160B`, which staggers UB bank accesses and meets the row stride alignment requirement for MTE movement.

The following uses the UB partitioning rules of Ascend 950PR/Ascend 950DT as a reference to describe how bank conflicts occur in this sample. It also compares the theoretical conflict intensity of the 32x32, 32x34, and 32x40 UB layouts.

The total UB size is 256 KB and is divided into 16 banks. These 16 banks are further organized into 8 bank groups. Each bank group contains 2 banks. In SIMT programming mode, each bank is divided into 4 subbanks, and each subbank has a width of 8B. The bank conflict in this sample refers to finer-grained subbank conflicts. Multiple threads in the same Warp may access UB at the same time in one UB access instruction. If these accesses belong to subbank resources with the same subbank ID in the same bank group, the hardware cannot process all requests in one cycle. The requests must wait in a queue, which forms a subbank conflict and increases access latency. If these accesses belong to the same 8B address range in the same subbank row, the hardware merges the requests and no subbank conflict occurs.

**Figure 3: UB bank structure**
<img src="./figures/bank结构示意图.png">

**Low-bit interleaving is used for addresses:** As shown in the following figure, contiguous addresses in UB are mapped to bank0 through bank15 at a granularity of 32B. The first 32B address segment is mapped to bank0, the second segment is mapped to bank1, and so on. The 16th segment is mapped to bank15, and the 17th segment is mapped back to the next row of bank0.

**Figure 4: UB address low-bit interleaving layout**
<img src="./figures/bank内存排布示意图.png">

Subbank conflicts in SIMT programming mainly include the following types:

- **Write-write conflict**: Multiple write operations try to access subbanks with the same subbank ID in the same bank group at the same time.
- **Read-read conflict**: Multiple read operations try to access subbanks with the same subbank ID in the same bank group at the same time.

In Case 2, the UB tile array is stored in row-major order. According to the low-bit interleaving rule, the first row of the tile array covers bank0 through bank3, the second row covers bank4 through bank7, the third row covers bank8 through bank11, and the remaining rows follow the same pattern. Each row has 32 float elements and exactly spans 4 banks. As shown in Figure 5, the figure shows how the first 12 rows of the tile array are placed in UB. The first element of each row is marked in blue. When data is written back to GM, threads in one Warp read one column of the tile. During the UB access, 32 threads are concentrated on subbank0 in two bank groups, which is a read-read conflict.

**Figure 5: Case 2 tile layout in UB**
<img src="./figures/case2_bank.png">

As shown in Figure 6, Case 3 adds 8 columns of padding to the UB tile array, changing each row from 32 elements to 40 elements. The row stride of 40 float elements is 160B, which corresponds to 20 subbanks. When the same column is read in the transpose direction, elements in adjacent rows are staggered by a stride of 20 subbanks in the physical UB address space. The accesses of 32 threads are no longer concentrated on subbanks with the same subbank ID in the same bank group, which reduces subbank conflicts during the transpose read phase.

**Figure 6: Case 3 tile layout in UB**
<img src="./figures/case3_bank.png">

```cpp
constexpr int TILE_PAD = 8;                          // The hybrid MTE movement requires 32B alignment, so a 32x40 UB layout is used.
constexpr int TILE_PAD_STRIDE = TILE_DIM + TILE_PAD;
__ubuf__ float tile[TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];

copy_gm_tile_to_ub(dst + local_tile * TILE_PAD_ELEMENTS, input + tile_offset, width,
                   TILE_PAD_STRIDE * sizeof(float));

output[x_index + y_index * height] =
    tile[local_tile * TILE_PAD_ELEMENTS + tx * TILE_PAD_STRIDE + ty];
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|      11.856       |    10.687    |     17633.7      |      6.785       |     0.635     |        0.702        |      0.065       |

**Analysis**:

Case 3 changes only the physical UB layout based on Case 2. It does not change GM read/write coordinates or the mapping from Thread Blocks to tiles. Compared with Case 2, Task Duration decreases from 18.696μs to 11.856μs, a latency reduction of about 36.6%. This result shows that after GM accesses become contiguous and the Thread Block count is fixed, bank conflicts during transpose reads from UB become one of the main bottlenecks.

---

### Case 4: MTE Moves Data to UB Before Transpose Write-Back with Padding and Double Buffer

**Optimization Goal**: Use two padded UB buffers and two event IDs on top of Case 3, so that the MTE2 movement pipeline and the SIMT VF pipeline run in parallel.

**Core Implementation**:

- Use two buffers. Each buffer contains two 32x40 padded tiles.
- Bind the two buffers to `EVENT_ID0` and `EVENT_ID1`.
- First move the first tile group into UB to fill the pipeline.
- In the loop, start MTE2 movement for the next tile group, and then call the SIMT VF to process the current buffer.
- After VF processing of the current buffer is complete, notify MTE2 that this buffer can be reused.

```cpp
__ubuf__ float tile[2][TILES_PER_BLOCK][TILE_DIM][TILE_PAD_STRIDE];

uint32_t curr_buffer = 0;
uint32_t curr_event = EVENT_ID0;

copy_gm_2tile_to_padded_ub(&tile[curr_buffer][0][0][0], input, width, tiles_x,
                           curr_tile_base, total_tiles);
asc_sync_notify(PIPE_MTE2, PIPE_V, curr_event);

for (uint32_t next_tile_base = curr_tile_base + loop_step; curr_tile_base < total_tiles;
     next_tile_base += loop_step) {
    asc_sync_wait(PIPE_MTE2, PIPE_V, curr_event);

    uint32_t next_buffer = curr_buffer ^ 1;
    uint32_t next_event = (next_buffer == 0) ? EVENT_ID0 : EVENT_ID1;
    if (next_tile_base < total_tiles) {
        asc_sync_wait(PIPE_V, PIPE_MTE2, next_event);
        copy_gm_2tile_to_padded_ub(&tile[next_buffer][0][0][0], input, width, tiles_x,
                                   next_tile_base, total_tiles);
        asc_sync_notify(PIPE_MTE2, PIPE_V, next_event);
    }

    asc_vf_call<simt_store_2tile_pad>(dim3(TILE_DIM, TILE_DIM * TILES_PER_BLOCK, 1),
                                      output, &tile[curr_buffer][0][0][0], width, height,
                                      tiles_x, curr_tile_base, total_tiles);
    asc_sync_notify(PIPE_V, PIPE_MTE2, curr_event);

    curr_buffer = next_buffer;
    curr_event = next_event;
    curr_tile_base = next_tile_base;
}
```

**Performance Data**:

| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
|       8.950       |    7.774     |     12826.7      |      6.728       |     0.866     |        0.794        |      0.102       |

**Analysis**:

Case 4 uses double buffer to provide physical isolation. This avoids MTE2 writes overwriting the UB region that the SIMT VF is reading. It also uses two event IDs to manage the lifecycle of the two buffers separately. The single-buffer serial barrier sequence, where VF starts after MTE2 completes and the next MTE2 movement starts after VF completes, is changed into pipeline overlap between adjacent iterations. Compared with Case 3, Task Duration decreases from 11.856μs to 8.950μs, a latency reduction of about 24.5%. Compared with direct GM transpose in Case 0, Task Duration decreases from 36.140μs to 8.950μs, improving overall performance by about 4.04x.

---

## Performance Comparison Summary

### Ascend 950PR Performance Data

The following table shows the performance data of each case when processing a 1024x1024 float matrix on Ascend 950PR.

| Case | Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio |
| ---- | :---------------: | :----------: | :--------------: | :--------------: | :-----------: | :-----------------: | :--------------: |
| 0    |      36.140       |    33.793    |     55757.9      |      33.338      |     0.987     |        0.443        |      0.013       |
| 1    |      23.215       |    2.281     |      3764.3      |      1.672       |     0.735     |        0.184        |      0.078       |
| 2    |      18.696       |    17.512    |     28894.9      |      13.509      |     0.772     |        0.701        |      0.040       |
| 3    |      11.856       |    10.687    |     17633.7      |      6.785       |     0.635     |        0.702        |      0.065       |
| 4    |       8.950       |    7.774     |     12826.7      |      6.728       |     0.866     |        0.794        |      0.102       |

The main transpose path is Case 0 -> Case 1 -> Case 2 -> Case 3 -> Case 4. Task Duration decreases from 36.140μs to 23.215μs, 18.696μs, 11.856μs, and 8.950μs. Compared with direct GM transpose in Case 0, Case 4 improves performance by about 4.04x.

Matrix transpose needs to read the complete input matrix from GM and write the complete output matrix back to GM. For a 1024x1024 float matrix, the ideal total GM read/write data volume is:

$$
D = 1024 \times 1024 \times 4B \times 2 = 8.39MB
$$

If the estimate uses the 1.6TB/s GM peak bandwidth of Ascend 950PR, the theoretical lower bound is:

$$
T_{theory} = \frac{8.39MB}{1.6TB/s} \approx 5.243μs
$$

Case 0 is far from the theoretical lower bound. The main reason is that transpose write-back makes write addresses of adjacent threads in the same Warp stride across rows, so GM writes cannot form ideal contiguous accesses. Case 1 uses UB staging to move non-contiguous access into UB, improving access continuity on the GM side. However, it launches 512 Thread Blocks by tile groups, which causes high scheduling overhead. Case 2 fixes the Thread Block count and reduces launch and scheduling overhead, but transpose reads from UB still have bank conflicts. Case 3 uses 32x40 UB padding to reduce bank conflicts, so latency decreases significantly. Case 4 further uses double buffer to run the MTE2 movement pipeline and the SIMT VF pipeline in parallel. Task Duration is close to the theoretical lower bound, but still higher than 5.243μs. The remaining gap mainly comes from non-GM-bandwidth overheads, including UB reads and writes, synchronization between MTE and V, `asc_vf_call` overhead, address calculation, and incomplete pipeline parallelism.

## Tuning Recommendations

When tuning memory access operators, first use the most direct implementation to expose the main gap. Then use the theoretical bandwidth limit to estimate the available optimization space. In this sample, the gap between direct transpose and the theoretical lower bound mainly comes from non-contiguous GM writes. Therefore, the optimization direction is not to increase compute parallelism, but to reorder memory accesses. Use 32x32 tiles as the granularity. Continuously move each tile into UB, read it in the transpose direction inside UB, and continuously write it back along the row direction of the output matrix.

After tiling, determine the mapping from Thread Blocks to tiles. Launching Thread Blocks by tile groups is simple. However, when the tile count is much larger than the hardware vector core count, this method introduces extra launch and scheduling overhead. Fixing the Thread Block count to the hardware vector core count and processing multiple tile groups in a loop inside the kernel can reduce this overhead. The thread count and the number of tiles processed by each Thread Block need to be evaluated together with register usage, stack usage, and measured latency.

After GM accesses become contiguous, the bottleneck moves to UB access and pipeline synchronization. When a 32x32 tile is accessed by column during transpose reads from UB, bank conflicts can occur. Padding can change the physical UB layout. In SIMD and SIMT hybrid programming, the layout also needs to meet the alignment requirements of MTE movement. Therefore, this sample uses a 32x40 padded tile. On this basis, double buffer rotates two UB buffers. When the current buffer enters the SIMT VF pipeline, the MTE2 pipeline can move the next batch of data into the other buffer. This runs the MTE2 movement pipeline and the SIMT VF pipeline in parallel and hides part of the movement overhead.

## Build and Run

In the sample root directory, perform the following steps to build and run the sample.

- Configure environment variables.

  Configure environment variables according to the [installation method](../../../../../docs/quick_start.md#prepare&install) of the CANN development kit package in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Description:** `${install_path}` is the CANN package installation directory. If no installation directory is specified, the default installation directory is `/usr/local/Ascend`.

- Run the sample.

  Run the following commands in the sample directory.

  ```bash
  SCENARIO_NUM=4                                                                     # Select a scenario. Valid values are 0-4.
  mkdir -p build && cd build;                                                        # Create and enter the build directory.
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project.
  ./matrix_transpose                                                                 # Run the sample.
  ```

- Build options

  | Option | Value | Description |
  | ------ | ----- | ----------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT). |
  | `SCENARIO_NUM` | `0`-`4` | Sample type. The default value is 4. |

  The following output indicates that the accuracy comparison is successful.

  ```text
  [Success] Case accuracy verification passed.
  ```

## Performance Analysis

Use the `msopprof` tool to collect detailed performance data:

```bash
msopprof ./matrix_transpose
```

After the command is complete, a folder named `OPPROF_{timestamp}_XXX` is generated in the default directory. The following is a reference performance data folder structure:

```text
├── dump                       // Raw performance data
├── ArithmeticUtilization.csv  // cube/vector instruction cycle ratio
├── L2Cache.csv                // L2 Cache hit rate
├── Memory.csv                 // UB, L1, and main memory read/write bandwidth
├── MemoryL0.csv               // L0A, L0B, and L0C read/write bandwidth
├── MemoryUB.csv               // Vector and Scalar read/write bandwidth to UB
├── OpBasicInfo.csv            // Basic operator information, including Task Duration(μs)
├── PipeUtilization.csv        // Compute unit and movement unit duration and ratio
├── ResourceConflictRatio.csv  // UB bank group, bank conflict, and resource conflict ratio
└── visualize_data.bin         // MindStudio Insight presentation file
```
