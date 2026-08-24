# Hierarchical Reduction Sample Using Cooperative Groups

## Overview

This sample demonstrates the basic usage of the [cooperative groups API](../../../../../../docs/zh/api/SIMT-API/cooperative_groups/cooperative_groups.md) through reduction sum operations, highlighting the code abstraction capabilities provided by cooperative groups.


## Supported Products

Ascend 950PR/Ascend 950DT

## Supported CANN Versions

- \>= CANN 9.2.0


## Directory Structure

```text
├── reduce_sum
│   ├── figures                             // Image resources for the README
│   ├── CMakeLists.txt                      // Build project file
│   ├── reduce_sum.asc                      // Sample implementation, three kernels plus host-side invocation
│   ├── README.md
│   └── README_en.md
```


## Sample Description

### Functionality

This sample provides three implementations of reduction sum operations, demonstrating how cooperative groups control thread collaboration at `grid`, `thread block`, and `Warp` granularities. The reduction sum operation is abstracted into a common function `reduce_sum`, which achieves reductions at different granularities by accepting different cooperative group objects, showcasing the code abstraction capabilities of cooperative groups.

### Specifications

All three cases use the same execution configuration: a grid of 4 thread blocks, 1024 threads per block (the default thread configuration), 4096 threads in total, with dynamic ubuf set to 0. The launch form is `<<<dim3(4), dim3(1024), DYN_UBUF_SIZE, stream>>>`.


<table>
<tr><td rowspan="1" align="center">OpType</td><td colspan="4" align="center">Cooperative Groups</td></tr>
<tr><td rowspan="2" align="center">Sample input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">input</td><td align="center">[4096]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Sample output</td><td align="center">output</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
</table>


### Implementation

**Figure 1**  The halving reduction process

![](./figures/halving_reduction_en.png)

The figure takes 16 threads as an example. In each round, every active thread adds the value it holds to the value held `i` threads away, after which the number of active threads is halved and the stride `i` halves along with it.

```cpp
template <typename PtrT>
__aicore__ inline int32_t reduce_sum(const thread_group& g, PtrT x, int32_t val)
{
    const int32_t lane = static_cast<int32_t>(g.thread_rank());
    for (int32_t i = static_cast<int32_t>(g.size()) / 2; i > 0; i /= 2) {
        x[lane] = val;              // (1) write to the slot of this thread
        g.sync();                   // (2) wait for the whole group to finish writing
        if (lane < i) {
            val += x[lane + i];     // (3) read the slot i threads away
        }
        g.sync();                   // (4) wait for the whole group to finish reading
    }
    return lane == 0 ? val : -1;
}
```

The number of threads participating in the reduction is `N = g.size()`. The distance `i` between adjacent threads involved in the computation starts at `N/2` and halves each round. Since `val` is stored in registers private to each thread and is not accessible to other threads, the parameter `x` is provided to point at a workspace, which is used to store intermediate values of the computation.

> [!NOTE]Note
> When a case calls the `reduce_sum` interface, `global_idx < num_elements ? input[global_idx] : 0` is required to guard against reading past the end of the input.

#### Performance Metric


| Metric | Description |
| --- | --- |
| Task Duration (μs) | Total task duration, including the time scheduled onto the accelerator, the execution time on the accelerator, and the response/completion time |

#### Case0: Cross-block Reduction

All threads in the grid are joined into a single group participating in the reduction, and one call to `reduce_sum` produces the total. The reduction takes 12 rounds of halving, each synchronized by a grid-level barrier: the additions in the first two rounds cross block boundaries, after which the reduction converges inside block 0, but the group object passed to `reduce_sum` is still `grid_group`, so each round of synchronization continues to require all 4096 threads of the grid to arrive, and the thread with rank 0 finally writes out the result directly.

**Figure 2**  Case0 cross-block reduction

![](./figures/reduce_sum_case0_en.png)

```cpp
__global__ void reduce_grid_group(
    const int32_t* input, int32_t* grid_workspace, int32_t* total_grid, int32_t num_elements)
{
    grid_group grid = this_grid();
    const int32_t global_idx = (int32_t)grid.thread_rank();
    volatile int32_t* ws = grid_workspace;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(grid, ws, in_val);
    if (grid.thread_rank() == 0) {
        *total_grid = val;
    }
}
```

The threads participating in the reduction are all 4096 threads of the grid. The reduction has to cross blocks, and GM is the memory shared by all blocks in the grid, so the workspace `grid_workspace` must be allocated in GM, and `ws` must be qualified with `volatile` so that every read lands in GM rather than in the Data Cache private to each core, which keeps the data consistent from round to round.

**Performance data**:

| Task Duration(us) |
| :---------------: |
|      16.101       |

#### Case1: Whole-block Reduction

The threads participating in the reduction are the 1024 threads of one block, and the 4 blocks reduce independently. The reduction takes 10 rounds of halving, all synchronized by block-level barriers. Rank 0 of each block accumulates the sum of that block into the same output through `asc_atomic_add`, which aggregates the total.

**Figure 3**  Case1 whole-block reduction

![](./figures/reduce_sum_case1_en.png)

```cpp
__global__ void reduce_thread_block(const int32_t* input, int32_t* total_block, int32_t num_elements)
{
    __ubuf__ int32_t ub_workspace[THREADS_PER_BLOCK];
    thread_block block = this_thread_block();
    const int32_t global_idx = (int32_t)(blockIdx.x * blockDim.x + block.thread_rank());
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(block, ub_workspace, in_val);
    if (block.thread_rank() == 0) {
        asc_atomic_add(total_block, val);
    }
}
```

The threads participating in this reduction are inside a block, so the workspace resides in UB and is defined statically inside the kernel with `THREADS_PER_BLOCK`, without being passed as a parameter.

**Performance data**:

| Task Duration(us) |
| :---------------: |
|       3.514       |

#### Case2: Warp-internal Reduction

The threads participating in the reduction are the 32 threads of one warp, and the reduction is done in two levels. At the first level, the 32 warps of a block each reduce independently: every group of 32 threads performs 5 rounds of halving, each round synchronized by a warp-level barrier, and rank 0 of each warp writes its partial sum into `ub_partial`. The two levels are joined by one block-level barrier, which ensures that all 32 partial sums have been written. At the second level, warp 0 reads these 32 partial sums and performs another 5 rounds of halving, again synchronizing only within the warp, after which its rank 0 accumulates the block sum into the same output through `asc_atomic_add`, and the 4 blocks aggregate the total.

**Figure 4**  Case2 warp-internal reduction

![](./figures/reduce_sum_case2_en.png)

```cpp
__global__ void reduce_block_tile(const int32_t* input, int32_t* total_tile, int32_t num_elements)
{
    __ubuf__ int32_t ub_workspace[THREADS_PER_BLOCK];
    __ubuf__ int32_t ub_partial[WARPS_PER_BLOCK];
    thread_block block = this_thread_block();
    const int32_t global_idx = (int32_t)(blockIdx.x * blockDim.x + block.thread_rank());
    auto tile = tiled_partition<TILE_SIZE>(block);
    const int32_t offset = (int32_t)tile.meta_group_rank() * TILE_SIZE;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    int32_t val = reduce_sum(tile, ub_workspace + offset, in_val);

    if (tile.thread_rank() == 0) {
        ub_partial[tile.meta_group_rank()] = val;
    }
    block.sync();

    if (tile.meta_group_rank() == 0) {
        val = reduce_sum(tile, ub_workspace, ub_partial[tile.thread_rank()]);

        if (tile.thread_rank() == 0) {
            asc_atomic_add(total_tile, val);
        }
    }
}
```

`TILE_SIZE` is 32, the width of one warp, so that each sub-group produced by `tiled_partition<TILE_SIZE>(block)` falls exactly within one warp. Since the 32 threads within a warp are inherently synchronized under the hardware SIMT execution model, `sync()` calls in the code require no actual waiting, costing far less than a block-level barrier that waits for 1024 threads. A value greater than 32 makes a sub-group span warps, its `sync()` degrades to a block-level barrier, and this advantage no longer holds.

**Performance data**:

| Task Duration(us) |
| :---------------: |
|       2.397       |

## Performance Comparison

The performance data of the three cases is listed below:

| Case | Threads in the reduction | Halving rounds | Synchronization count and scope | Task Duration(us) |
| :--: | :----: | :--: | :--: | :-----------------: |
| Cross-block reduction | 4096 | 12 | 24 grid-level | 16.101 |
| Whole-block reduction | 1024 | 10 | 20 block-level | 3.514 |
| Warp-internal reduction | 32 | 5 + 5 | 20 warp-internal (no overhead) + 1 block-level | 2.397 |

The three cases have the same input size and the same memory traffic, so the performance difference comes mainly from the scope of synchronization: the more threads a single synchronization has to wait for, the longer the threads that arrive early sit idle at the barrier.

- Warp-internal reduction is roughly 31.8% faster than whole-block reduction. The two have the same number of halving rounds and both keep the workspace in UB; they differ only in the scope of synchronization. The 20 `sync()` calls in warp-internal reduction incur no actual overhead due to inherent thread synchronization, with only one block-level barrier between the two levels that generates actual waiting, whereas each of the 20 synchronizations in whole-block reduction waits for all 1024 threads of the block.
- Cross-block reduction takes roughly 4.6 times as long as whole-block reduction, for two reasons. First, every synchronization has to wait for all 4096 threads of the grid, the widest scope of the three. Second, the workspace can only be placed in GM and the pointer additionally needs `volatile`, so every read and write of each round accesses GM directly, and the access latency of GM is far higher than that of UB.

## Build and Run

Run the following steps in the root directory of this example to build and execute the example.

- Configure Environment Variables

  Configure environment variables based on the [installation method](../../../../../../docs/zh/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the Example

  Run the following commands in this example directory. The case number is given as an argument at run time, and a single execution runs only that one case.

  ```bash
  mkdir -p build && cd build;                             # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;    # Build the project
  ./cooperative_groups 0                                  # Run the cross-block reduction case
  ./cooperative_groups 1                                  # Run the whole-block reduction case
  ./cooperative_groups 2                                  # Run the warp-internal reduction case
  ```

- Build Options Description

  | Option | Values | Description |
  |---------------------------|------------|---------------------------------------------------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |


  The output is as follows, which indicates that the accuracy verification passed.

  ```text
  [Success] Case accuracy verification passed.
  ```

## Performance Analysis

Use the `msOpProf` tool to collect detailed performance data:

```bash
msopprof ./cooperative_groups 0   # Analyze the performance of the cross-block reduction case
msopprof ./cooperative_groups 1   # Analyze the performance of the whole-block reduction case
msopprof ./cooperative_groups 2   # Analyze the performance of the warp-internal reduction case
```

After the command completes, a folder named "OPPROF_{timestamp}_XXX" is generated in the default directory. The performance data folder structure example is as follows:

```text
├──dump                       # Raw performance data, no user attention needed
├──ArithmeticUtilization.csv  # cube/vector instruction cycle ratio
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1, and main memory read/write bandwidth
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth
├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth
├──OpBasicInfo.csv            # Operator basic information
├──PipeUtilization.csv        # Compute unit and transfer unit duration and ratio
├──ResourceConflictRatio.csv  # UB bank group, bank conflict, and resource conflict ratio across all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```
