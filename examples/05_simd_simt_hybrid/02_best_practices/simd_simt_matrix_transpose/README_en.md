# MatrixTranspose Example

## Overview

This example uses matrix transpose to demonstrate memory access optimization in Ascend C SIMD and SIMT hybrid programming. Starting from continuous copy and direct GM transpose baselines, it gradually introduces MTE movement, UB staging, tile mapping, UB padding, and a double-buffer-style layout. In the hybrid path, the Vector-side wrapper moves data from GM to UB through MTE, and the SIMT VF reads UB in the transposed direction before writing back to GM.

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
├── simd_simt_matrix_transpose
│   ├── CMakeLists.txt         // Build project file
│   ├── matrix_transpose.asc   // Matrix transpose implementation
│   ├── figures                // Image resources for README
│   └── README.md
```

## Example Description

- Computation formula:

  $$
  output(x, y) = input(y, x)
  $$

  - input is a float matrix with shape [H,W].
  - output is a float matrix with shape [W,H].
- Example specifications:
  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">MatrixTranspose</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[1024,1024]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">copy_kernel / transpose_naive_kernel / transpose_ub_*_kernel</td></tr>
  </table>

## Example Implementation

### Case Implementation

The example selects one path at compile time through `SCENARIO_NUM`. The host launches `__global__ __vector__` kernels with the three-slot launch form. The Vector-side kernel uses MTE APIs to move GM data into UB, then calls `__simt_vf__` functions through `asc_vf_call` for SIMT thread-level processing.

| Case | Kernel | Block count | SIMT threads | Change introduced |
| --- | --- | --- | --- | --- |
| 0 | `copy_kernel` | `core` | 2048 | Continuous copy baseline, using Vector-side MTE for GM->UB->GM movement |
| 1 | `copy_kernel` | `core` | 1024 | Continuous copy baseline aligned with later 1024-thread transpose cases |
| 2 | `transpose_naive_kernel` | `core` | 2048 | Direct SIMT GM transpose, with contiguous GM reads and non-contiguous GM writes |
| 3 | `transpose_ub_2tile_kernel` | `ceil(tiles / 2)` | 2048 | Move two 32x32 tiles to UB with MTE, then use SIMT transposed UB reads and GM writes |
| 4 | `transpose_ub_2tile_kernel` | `core` | 2048 | Fix the Thread Block count to the vector core count and process tile groups in a loop |
| 5 | `transpose_ub_kernel` | `core` | 1024 | Process one 32x32 tile per loop iteration |
| 6 | `transpose_ub_pad_kernel` | `core` | 1024 | Add two padding columns to the UB tile to reduce bank conflict risk |
| 7 | `transpose_ub_pad_kernel` | `core` | 1024 | Use two padded UB tiles as a double-buffer-style rotating layout |

`core` is the hardware vector core count queried at runtime through `aclrtGetDeviceInfo(ACL_DEV_ATTR_VECTOR_CORE_NUM)`. `tiles` is `(W/32) x (H/32)`, which is 1024 for a 1024x1024 matrix.

### Thread Block and Tile Mapping

Starting from Case 3, the matrix is processed as 32x32 tiles. The left side of the figure below shows a 1024x1024 matrix split into 32x32 tiles. The right side shows the thread mapping inside one 32x32 tile. `tx` is the tile-local column index, and `ty` is the tile-local row index.

<img src="./figures/blockMapping.png" width="60%">

For the green element in the figure:

- Its tile coordinate is `(2, 1)`. For a 1024x1024 matrix, `tiles_x = 1024 / 32 = 32`, so `tile_id = 2 * 32 + 1 = 65`.
- Its local coordinate inside the tile is `(2, 29)`, corresponding to `ty = 2` and `tx = 29`.

The input GM address is computed as:

```cpp
uint32_t tile_y = tile_id / tiles_x;
uint32_t tile_x = tile_id - tile_y * tiles_x;
uint32_t x_index = tile_x * TILE_DIM + tx;
uint32_t y_index = tile_y * TILE_DIM + ty;
uint32_t input_offset = x_index + y_index * width;
```

### Hybrid Data Flow

For Case 3 and later transpose paths, GM-to-UB movement is issued on the Vector side:

```cpp
asc_set_gm2ub_loop_size(TILE_DIM, 1);
asc_set_gm2ub_loop1_stride(width * sizeof(float), ub_row_bytes);
asc_copy_gm2ub_align(dst, src, 1, TILE_ROW_BYTES, 0, 0, false, 0, 0, 0);
asc_set_gm2ub_loop_size(1, 1);
```

After MTE movement completes, pipeline synchronization ensures that SIMT VF reads a complete tile:

```cpp
asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
asc_vf_call<simt_store_tile>(dim3(TILE_DIM, TILE_DIM, 1), output, tile, height, tiles_x, tile_id);
```

The core idea of UB staging is shown below: the GM read stage moves rows from the input matrix into UB contiguously. The GM write stage swaps output tile coordinates so that one warp writes one output row. The originally non-contiguous GM write is moved to a transposed UB read.

<img src="./figures/case3.png" width="60%">

### Case Details

#### Case 0 / Case 1: MTE Continuous Copy Baselines

Case 0 and Case 1 do not transpose the matrix. They are used as continuous GM movement baselines. The Vector kernel moves continuous data from GM to UB and then from UB back to GM. SIMT VF only touches the UB tile to keep the hybrid programming call path.

#### Case 2: Direct GM Transpose

Case 2 does not use UB staging. SIMT threads read `input(row, col)` and directly write `output(col, row)`. Neighboring threads in one warp read contiguous GM addresses, but their write-back addresses are distributed across different rows of the output matrix.

#### Case 3 / Case 4: MTE GM-to-UB Movement and Transposed Write-Back

Case 3 sets the Thread Block count according to the tile split and processes two tiles per Thread Block. Case 4 fixes the Thread Block count to the hardware vector core count and loops over tile groups inside the kernel. In both cases, the Vector side moves tiles from GM to UB with MTE, then SIMT VF reads UB in the transposed direction and writes GM contiguously.

#### Case 5: 1024-Thread UB Staging

Case 5 processes one 32x32 tile per Thread Block per loop iteration, with SIMT threads configured as `dim3(32, 32, 1)`. Compared with the 2048-thread two-tile version, this mapping has simpler index logic and a natural one-tile-per-Thread-Block shape.

#### Case 6: UB Staging with Padding

Case 6 changes the UB tile layout from 32x32 to 32x34. During MTE movement, `asc_set_gm2ub_loop1_stride` sets the UB destination row stride to `34 * sizeof(float)`, leaving two padding columns at the end of each row. SIMT VF still reads only the valid 32 columns.

The following figures use the Ascend 950PR/Ascend 950DT UB layout as an example.

<img src="./figures/bank结构示意图.png">

Continuous UB addresses are mapped to bank0 through bank15 at 32B granularity:

<img src="./figures/bank内存排布示意图.png">

In Case 5, a warp reads one UB column during the transposed read stage, which may concentrate accesses on the same subbank resources:

<img src="./figures/case5bank.png">

In Case 6, padding changes the row stride from 32 floats to 34 floats. Elements from the same column are staggered across subbanks, reducing the bank conflict risk:

<img src="./figures/case6bank.png">

#### Case 7: UB Staging with Padding and Double-Buffer-Style Layout

Case 7 uses two 32x34 UB tiles and rotates them with `cnt`. This demonstrates how the hybrid kernel can organize MTE input buffers on the Vector side and pass the current buffer to SIMT VF.

## Build and Run

Load the CANN environment before building:

```bash
source ${install_path}/cann/set_env.sh
```

Build and run:

```bash
mkdir -p build
cd build
SCENARIO_NUM=7
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
./matrix_transpose 1024
```

`SCENARIO_NUM` can be `0` through `7`, and the default is `7`. The matrix size can be specified as a runtime argument and must be a multiple of 32.

Expected successful output:

```text
Usage: ./matrix_transpose [matrix_size] (compile-time SCENARIO_NUM=7)
Running case 7: transpose_mte_ub_pad_db...
Threads/Thread Block: 1024
Matrix shape: 1024 x 1024
[Success] Case accuracy verification passed.
```

## Performance Analysis

Use `msprof op` to collect profiling data:

```bash
msprof op ./matrix_transpose 1024
```

This hybrid version has a different implementation path from the pure SIMT version, so performance numbers should be collected again before being added to the document. Start from `Task Duration` in `OpBasicInfo.csv`, then inspect MTE2 and Vector utilization in `PipeUtilization.csv` and UB bank conflict data in `ResourceConflictRatio.csv`.
