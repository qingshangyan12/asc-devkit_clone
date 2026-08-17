# Hierarchical Reduction Sample Using Cooperative Groups

## Overview

This sample is based on the Ascend C SIMT programming model. It demonstrates how the cooperative_groups library organizes threads into different hierarchy levels and performs efficient parallel reduction.

The sample defines one generic reduction interface, `reduce_sum`, whose parameter takes the `thread_group` base class. Three cases are built around this interface, using cross-block (`grid_group`), whole-block (`thread_block`), and sub-group (`thread_block_tile`) reduction domains respectively. The three call sites are written identically; the granularity difference is carried by the group object passed in. The project keeps a single `reduce_sum` implementation, shared by four call sites across the three cases. Its body contains no granularity-specific index arithmetic and no conditional branch on group type.

The programming model is pure SIMT. `grid_group` does not support mixed SIMD and SIMT programming scenarios, which this sample does not cover.

## Supported Products

Ascend 950PR, Ascend 950DT.

## Supported CANN Versions

CANN 9.2.0 and later.

`simt_api/cooperative_groups.h` is provided starting from 9.2.0. Earlier versions report `'simt_api/cooperative_groups.h' file not found` at compile time.

## Directory Structure

```text
├── reduce_sum
│   ├── figures                             // Image resources for the README
│   ├── CMakeLists.txt                      // Build project file
│   ├── reduce_sum.asc                      // Sample implementation, three kernels plus host-side invocation
│   ├── README.md                           
│   └── README_en.md                        
```

`CMakeLists.txt` is the build project file. `reduce_sum.asc` contains the generic reduction interface `reduce_sum`, the SIMT kernel implementations for the three cases, and the host-side argument parsing, case dispatch, and result verification.

## Sample Description

### Functionality

The input is an `int32` array with `input[i] = i + 1`, 4096 elements in ND format. The output is a single `int32`, the reduction total. Summing the arithmetic sequence gives the analytical result `4096 × 4097 / 2 = 8390656`; the expected value for verification is computed from this formula in the code.

The three cases partition the reduction domain differently, but they cover the same data without overlap, and all three totals equal that analytical result. The call chains and reduction levels are shown below:

**Figure 1**  Call chain and reduction levels of reduce_sum

![](./figures/reduce_sum_dispatch_en.png)

Each case has its own kernel, its own launch, and its own device memory. A single execution runs exactly one of them. The host-side case functions are grouped by the storage level of the workspace into two: the cross-block reduction places its workspace in global memory, while the whole-block and sub-group scenarios both place theirs in shared memory. The latter two share an identical host-side flow and are therefore combined into one function.

### Specifications

All three cases use the same execution configuration: a grid of 4 thread blocks, 1024 threads per block (32 warps), 4096 threads in total, with dynamic ubuf set to 0. The launch form is `<<<dim3(4), dim3(1024), DYN_UBUF_SIZE, stream>>>`. `THREADS_PER_BLOCK` is 1024, the number of threads per block under the default thread configuration. `TILE_SIZE` is 32 to match the warp width, placing each sub-group within a single warp.

```cpp
constexpr int32_t BLOCKS_PER_GRID   = 4;     // thread blocks per grid
constexpr int32_t THREADS_PER_BLOCK = 1024;  // threads per block, i.e. 32 warps
constexpr int32_t TILE_SIZE         = 32;    // case2 sub-group width, equals warpSize
constexpr int32_t TOTAL_THREADS     = 4096;  // BLOCKS_PER_GRID * THREADS_PER_BLOCK
constexpr int32_t WARPS_PER_BLOCK   = 32;    // THREADS_PER_BLOCK / TILE_SIZE
constexpr uint32_t DYN_UBUF_SIZE    = 0;     // pure SIMT, no dynamic ubuf
```

<table>
<tr><td rowspan="2" align="center">Sample input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
<tr><td align="center">input</td><td align="center">[4096]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="3" align="center">Sample output</td><td align="center">total_grid (CASE=0)</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td align="center">total_block (CASE=1)</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td align="center">total_tile (CASE=2)</td><td align="center">[1]</td><td align="center">int32</td><td align="center">ND</td></tr>
<tr><td rowspan="1" align="center">Kernel names</td><td colspan="4" align="center">reduce_grid_group / reduce_thread_block / reduce_block_tile</td></tr>
<tr><td rowspan="1" align="center">gridDim</td><td colspan="4" align="center">(4, 1, 1)</td></tr>
<tr><td rowspan="1" align="center">blockDim</td><td colspan="4" align="center">(1024, 1, 1)</td></tr>
</table>

#### CASE=0 (cross-block reduction)

| Item | Description |
|---|---|
| Group object | `this_grid()`, reduction domain of 4096 threads |
| Synchronization | Grid-level barrier, 24 times across 12 rounds |
| Workspace | 4096 slots in global memory, 16 KB |
| Aggregation | Direct assignment by rank 0 |

#### CASE=1 (whole-block reduction)

| Item | Description |
|---|---|
| Group object | `this_thread_block()`, reduction domain of 1024 threads |
| Synchronization | Block-level barrier `asc_syncthreads()`, 20 times across 10 rounds |
| Workspace | 1024 slots in shared memory, one copy per block |
| Aggregation | `asc_atomic_add` by rank 0 of each block |

#### CASE=2 (sub-group reduction)

| Item | Description |
|---|---|
| Group object | `tiled_partition<32>(block)`, reduction domain of 32 threads |
| Synchronization | Warp-internal, 10 times across two levels of 5 rounds, plus 1 block-level between levels |
| Workspace | Shared memory, 32 slots per warp at level one, plus `ub_partial[32]` to relay partial sums |
| Aggregation | `asc_atomic_add` by rank 0 of warp 0 in each block |

### Implementation

The three cases share one reduction implementation, differing only in the group object, the workspace, and the initial value passed in:

```cpp
reduce_sum(grid,  ws,                    input[global_idx]);          // case0
reduce_sum(block, ub_workspace,          input[global_idx]);          // case1
reduce_sum(tile,  ub_workspace + offset, input[global_idx]);          // case2 level one
reduce_sum(tile,  ub_workspace,          ub_partial[thread_rank()]);  // case2 level two
```

The parameter takes the `thread_group` base class, and the body uses only three base-class interfaces: `thread_rank()`, `size()`, and `sync()`. Whether `g.sync()` resolves to warp-internal synchronization, a block-level barrier, or a grid-level barrier is dispatched by the base class according to the actual group type; the interface itself is unaware of that dispatch:

```cpp
template <typename PtrT>
__aicore__ inline int32_t reduce_sum(const thread_group& g, PtrT x, int32_t val)
{
    const int32_t lane = static_cast<int32_t>(g.thread_rank());
    for (int32_t i = static_cast<int32_t>(g.size()) / 2; i > 0; i /= 2) {
        x[lane] = val;              // (1) write to this thread's slot
        g.sync();                   // (2) wait for the whole group to finish writing
        if (lane < i) {
            val += x[lane + i];     // (3) lower half reads its partner's slot
        }
        g.sync();                   // (4) wait for the whole group to finish reading
    }
    return lane == 0 ? val : -1;
}
```

Let the reduction domain size be `N = g.size()`. The stride `i` starts at `N / 2` and halves each round, for `log2(N)` rounds in total. The four steps per round and the final return are common to all three cases: all `N` threads execute `x[lane] = val`, writing the value they currently hold into slot `lane` of the workspace; `g.sync()` waits for the whole group to finish writing; threads with `lane < i` execute `val += x[lane + i]`, reading the partner slot at stride `i` and accumulating in a register, which reduces the count of live values from `i × 2` to `i`; `g.sync()` waits for the whole group to finish reading, so the next round's writes do not overwrite slots that have not yet been read. When `i` reaches 0 the loop ends, lane 0 holds the group total, and the remaining threads return −1 for the caller to handle according to its own aggregation method.

The two values added each round each stand for a batch of elements, the two batches do not overlap, and the sum stands for both batches together. So at the end of every round the live values still cover all elements between them, with nothing missed and nothing counted twice, only the number of threads holding a value is halved. This holds regardless of the reduction domain size `N`, so the totals are identical at all three granularities.

**Figure 2**  The halving reduction: stride and active thread count both halve each round

![](./figures/halving_reduction_en.png)

The workspace pointed to by parameter `x` carries the job of exchanging intermediate values among threads in the group. `val` lives in a register private to each thread, and cross-thread merging happens through this block of memory addressable by all participating threads. The workspace is sized to `g.size()` slots, indexed by rank within the group; what remains in it after execution is the last round's intermediate state, which is not read back to the host. Its storage level is determined by the span of the reduction domain: global memory when the domain crosses blocks, shared memory when it does not.

Template parameter `PtrT` carries the address space qualifier of the workspace pointer, either a raw global memory pointer or `__ubuf__ int32_t*`. Granularity dispatch is still performed by the group object `g`.

The last parameter of all three kernels is `uint32_t num_elements`, and each has one statement at the point of reading input: `global_idx < num_elements ? input[global_idx] : 0`. The thread count is fixed at 4096 by the execution configuration; threads whose index exceeds the element count take the additive identity 0 into the sum and write their slots and complete every round of halving and synchronization alongside the rest.

#### Cross-block Reduction

The reduction domain is all 4096 threads in the grid, and one `reduce_sum` call produces the total.

```cpp
__global__ void reduce_grid_group(
    const int32_t* input, int32_t* grid_workspace, int32_t* total_grid, uint32_t num_elements)
{
    grid_group grid = this_grid();
    const uint32_t global_idx = static_cast<uint32_t>(grid.thread_rank());
    volatile int32_t* ws = grid_workspace;
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(grid, ws, in_val);
    if (grid.thread_rank() == 0) {
        *total_grid = val;
    }
}
```

`thread_rank()` of `grid_group` is the global index 0 to 4095, used directly as the subscript into `input`.

The reduction domain crosses blocks, so the workspace must be visible at Grid scope, and of the four storage levels only global memory satisfies it. Global memory is allocated and initialized on the host side, and the device side obtains the pointer through a kernel parameter, so the workspace enters the parameter list as `int32_t* grid_workspace`. Among the three kernels, only this scenario carries a workspace parameter.

The reduction domain is 4096, giving 12 rounds. The reads in the first two rounds cross block boundaries: at `i = 2048`, lanes 0 to 2047 of blocks 0 and 1 read slots written by blocks 2 and 3; at `i = 1024`, lanes 0 to 1023 of block 0 read slots written by block 1. From the third round on the stride drops below 512 and the additions converge inside block 0.

The workspace pointer is converted to `volatile int32_t*` before being passed to `reduce_sum`. The Data Cache is private to each core, and `grid.sync()` only aligns the execution progress of the cores without invalidating existing cache copies; cross-block reads must bypass the cache through `volatile` to obtain the value the partner block wrote in the current round.

Although the additions in the last ten rounds have converged to block 0, the group object is still `grid_group`, so each `g.sync()` still requires all 4096 threads of the grid to arrive, and the 3072 threads of blocks 1 through 3 follow along through each round's write and two synchronizations. When the reduction domain is at grid granularity, the synchronization scope always covers all 4096 threads.

After 12 rounds only rank 0 of the grid holds the total, written out by `*total_grid = val`. With a single writer, no atomic addition is needed and `total_grid` does not need to be zeroed beforehand.

#### Whole-block Reduction

The reduction domain is the 1024 threads of one thread block. Each of the four blocks reduces to a partial sum, and the partial sums are aggregated by atomic addition.

```cpp
__global__ void reduce_thread_block(const int32_t* input, int32_t* total_block, uint32_t num_elements)
{
    __ubuf__ static int32_t ub_workspace[THREADS_PER_BLOCK];

    thread_block block = this_thread_block();
    const uint32_t global_idx = static_cast<uint32_t>(blockIdx.x) * static_cast<uint32_t>(blockDim.x)
                              + static_cast<uint32_t>(block.thread_rank());
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    const int32_t val = reduce_sum(block, ub_workspace, in_val);

    if (block.thread_rank() == 0) {
        asc_atomic_add(total_block, val);
    }
}
```

The reduction domain no longer crosses blocks, so the workspace visibility drops from Grid to Block and shared memory suffices. The on-chip offset of shared memory is laid out by the compiler at compile time, so the workspace is declared inside the kernel as `__ubuf__ static` and does not enter the parameter list, leaving the signature one parameter shorter than the cross-block scenario. A static declaration takes a compile-time constant length, so the capacity is `constexpr int32_t THREADS_PER_BLOCK`.

`block.thread_rank()` is the index within the block, 0 to 1023, and reading input converts it to a global subscript through `blockIdx.x * blockDim.x + block.thread_rank()`. The rank semantics narrowing from global to within-block is the main difference between this scenario and the cross-block one. Block numbering starts at 0 in each block, so every block has its own rank 0 and the four blocks each produce one partial sum.

The reduction domain is 1024, giving 10 rounds. In the first 5 rounds `i` decreases from 512 to 32, live threads span multiple warps, and synchronization is covered by the block-level barrier. After `i = 32` the live threads shrink to lanes 0 through 31, exactly one warp, while the group object remains `thread_block` and synchronization still dispatches to the block-level barrier. The sub-group scenario replaces the synchronization of these last 5 rounds with warp-internal synchronization.

Workspace reads and writes do not leave the core. `x[lane] = val` and `val += x[lane + i]` act directly on the Unified Buffer of the local core, participating threads share the same block of on-chip memory, and after `asc_syncthreads()` a write is visible to all threads in the block, so reads do not need `volatile`.

Partial sums of the four blocks and their aggregation:

| Block | Elements covered | Partial sum |
|---|---|---|
| 0 | 1 to 1024 | 524800 |
| 1 | 1025 to 2048 | 1573376 |
| 2 | 2049 to 3072 | 2621952 |
| 3 | 3073 to 4096 | 3670528 |

The general form is `524800 + 1048576k` (where `k` is the block index, ranging from 0 to 3). The four partial sums are accumulated by `asc_atomic_add`, and the result is independent of the order in which the blocks complete. The total consists of these four accumulations, and `total_block` is zeroed by `aclrtMemset` before launch.

#### Sub-group Reduction

The reduction is completed in two levels. `reduce_sum` is called twice within the same kernel, with a reduction domain of 32 both times.

```cpp
__global__ void reduce_block_tile(const int32_t* input, int32_t* total_tile, uint32_t num_elements)
{
    __ubuf__ static int32_t ub_workspace[THREADS_PER_BLOCK];   // level one, shared as 32 segments
    __ubuf__ static int32_t ub_partial[WARPS_PER_BLOCK];       // relays partial sums between levels

    thread_block block = this_thread_block();
    const uint32_t global_idx = static_cast<uint32_t>(blockIdx.x) * static_cast<uint32_t>(blockDim.x)
                              + static_cast<uint32_t>(block.thread_rank());

    auto tile = tiled_partition<TILE_SIZE>(block);
    const int32_t offset = static_cast<int32_t>(tile.meta_group_rank()) * TILE_SIZE;

    // level one: 32-way parallel, reduction domain 32
    const int32_t in_val = global_idx < num_elements ? input[global_idx] : 0;
    int32_t val = reduce_sum(tile, ub_workspace + offset, in_val);
    if (tile.thread_rank() == 0) {
        ub_partial[tile.meta_group_rank()] = val;
    }

    block.sync();

    // level two: warp 0 only, reduction domain still 32, input becomes the partial sums
    if (tile.meta_group_rank() == 0) {
        val = reduce_sum(tile, ub_workspace, ub_partial[tile.thread_rank()]);
        if (tile.thread_rank() == 0) {
            asc_atomic_add(total_tile, val);
        }
    }
}
```

`tiled_partition<32>(block)` takes the thread block as the parent group and splits it into `1024 / 32 = 32` sub-groups, 128 across the grid. The sub-group width equals the warp width, so `sync()` dispatches to the warp-internal synchronization path. The return type is `thread_block_tile<TILE_SIZE, thread_block>`, determined by two template parameters, the sub-group width and the parent group type; `auto` receives it at the call site, so the type never has to be spelled out.

After partitioning there are two ranks: `tile.thread_rank()` is the index within the warp, 0 to 31, which `lane` inside `reduce_sum` takes; `tile.meta_group_rank()` is the index of the sub-group within the parent group, 0 to 31, that is the warp number. `offset = tile.meta_group_rank() * TILE_SIZE` uses the warp number to give each of the 32 warps an exclusive 32-slot segment in the same `ub_workspace`.

Level one has a reduction domain of 32 and 5 rounds of halving, with all 128 warps executing in parallel. Warp w covers global subscripts `32w` through `32w+31`, that is element values `32w+1` through `32w+32`, giving a partial sum of `32 × 32w + 32 × 33 / 2 = 1024w + 528`:

| Warp | Elements covered | Partial sum |
|---|---|---|
| 0 | 1 to 32 | 528 |
| 1 | 33 to 64 | 1552 |
| … | … | … |
| 127 | 4065 to 4096 | 130576 |

Rank 0 of each segment then writes its partial sum into `ub_partial`, subscripted by warp number, one slot per sub-group.

The two levels are joined by `block.sync()`, at block granularity. The 32 writers of `ub_partial` belong to 32 different warps and the readers are the 32 lanes of warp 0, so the write and read cross warp boundaries; the block-level barrier ensures all 32 slots have landed before level two reads them.

Level two is confined to warp 0 by `if (tile.meta_group_rank() == 0)`, with the other 31 warps skipping it. Its 32 lanes each read one partial sum as the initial value and reduce once more to obtain the block total. `g.sync()` inside `reduce_sum` is warp-internal at this point, scoped to the single warp that entered the branch, and the 31 skipped warps do not participate in that synchronization. The level-two workspace is the first 32 slots of `ub_workspace`, the segment tile 0 used at level one; those partial sums have already been relayed to `ub_partial` after `block.sync()`, so the level-two writes overwrite the old contents without needing a separate allocation.

Both calls pass the same `tile` group object, with a reduction domain of 32 and 5 rounds each, and an identical halving process. Only the values of the three arguments differ:

| | Input | Workspace | Execution scope |
|---|---|---|---|
| Level one | `input[global_idx]` | `ub_workspace + offset` | All 32 warps |
| Level two | `ub_partial[thread_rank()]` | `ub_workspace` | Warp 0 only |

The same implementation is instantiated twice within one kernel, with no change to the function body. The two levels total 10 warp-internal synchronizations plus 1 block-level synchronization, level one runs 128-way parallel, and the extra on-chip space is `ub_partial[32]`, 128 B.

Summing the partial sums of the 32 warps within a block gives that block's partial sum, matching the whole-block scenario. After atomic addition across the four blocks the total is 8390656.

#### Host-side Implementation

`main` performs seven steps in order: parse arguments; build the input and the expected value; initialize the runtime through `aclInit`, `aclrtSetDevice`, and `aclrtCreateStream`; allocate input device memory and copy the data in; dispatch by case to one of the two case functions and retrieve the total; release memory and deinitialize; finally call `verify_result` and use its return value as the process exit code.

```cpp
// case0: workspace in global memory
int32_t run_global_workspace_case(int32_t* input_device, int32_t num_elements, aclrtStream stream);
// case1 and case2: workspace in shared memory, the kernel parameter selects which one to launch
int32_t run_shared_workspace_case(KernelType kernel, int32_t* input_device, int32_t num_elements, aclrtStream stream);
```

The two case functions do not call each other. Each allocates the device memory its scenario needs, launches the kernel, synchronizes the stream, reads the total back, releases its memory, and returns that total. Result comparison does not happen inside the case functions; it is centralized in `verify_result`.

The split follows the storage level of the workspace, since that level determines whether the host side needs an additional allocation. The cross-block reduction places its workspace in global memory, so the host must allocate a separate block of `TOTAL_THREADS` `int32` values and pass it in the kernel parameter list; its total is written solely by rank 0 of the grid, without atomic addition, and therefore needs no zeroing beforehand. The whole-block and sub-group scenarios declare their workspace inside the kernel as `__ubuf__ static`, so the host performs no corresponding allocation, and both need device memory only for the one `int32` total; aggregation in both uses `asc_atomic_add`, so that total is zeroed by `aclrtMemset` before launch.

The host-side flow of the latter two scenarios is thus identical, differing only in which kernel is launched. `run_shared_workspace_case` carries that difference in its `KernelType kernel` parameter:

```cpp
if (kernel == KernelType::THREAD_BLOCK) {
    reduce_thread_block<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK, DYN_UBUF_SIZE, stream>>>(
        input_device, total_device, num_elements);
} else {
    reduce_block_tile<<<BLOCKS_PER_GRID, THREADS_PER_BLOCK, DYN_UBUF_SIZE, stream>>>(
        input_device, total_device, num_elements);
}
```

Allocation, zeroing, stream synchronization, reading the total back, and releasing memory all sit outside that branch, shared by both scenarios. A single execution still launches exactly one kernel; case selection is performed by the dispatch in `main` and is unaffected by the merging of the case functions.

The expected value is computed in code from the element count, matching the analytical formula. `num_elements` takes `TOTAL_THREADS`, that is `BLOCKS_PER_GRID * THREADS_PER_BLOCK` or 4096, so golden is 8390656:

```cpp
const int32_t golden = static_cast<int32_t>(num_elements * (num_elements + 1) / 2);  // 4096 -> 8390656
```

After the case function returns, `main` finishes by releasing the input device memory, destroying the stream, resetting the device, and deinitializing, then returns the verification result:

```cpp
CHECK_ACL(aclrtFree(input_device));
CHECK_ACL(aclrtDestroyStream(stream));
CHECK_ACL(aclrtResetDevice(device_id));
CHECK_ACL(aclFinalize());

return verify_result(output, golden);
```

Every ACL call is wrapped in `CHECK_ACL`. When the return code is not `ACL_SUCCESS`, the macro prints its file and line, the error code, the runtime message obtained from `aclGetRecentErrMsg()`, and the literal text of the failing call, then terminates the process with `EXIT_FAILURE`. `aclGetRecentErrMsg()` may return `nullptr` when no detail is available, and passing that to `%s` is undefined behavior, so the macro guards against it:

```cpp
#define CHECK_ACL(expr)                                                                       \
    do {                                                                                      \
        aclError ret = (expr);                                                                \
        if (ret != ACL_SUCCESS) {                                                             \
            const char* msg = aclGetRecentErrMsg();                                           \
            fprintf(                                                                          \
                stderr, "Ascend Error: %s:%d code=%d %s \"%s\"\n", __FILE__, __LINE__,        \
                static_cast<int>(ret), (msg != nullptr) ? msg : "no detail", #expr);          \
            exit(EXIT_FAILURE);                                                               \
        }                                                                                     \
    } while (0)
```

Terminating on error keeps the subsequent launch and read-back from running, so results are never compared while the runtime is already in an abnormal state. The correctness of the reduction result is determined by `verify_result` comparing the total against golden, and its return value is the process exit code.

## Build and Run

Perform the following steps in the sample root directory to build and run.

Set up environment variables, where `${install_path}` is the CANN installation directory, defaulting to `/usr/local/Ascend` if not specified at install time:

```bash
source ${install_path}/cann/set_env.sh
```

`CMakeLists.txt` does not hardcode the CANN path. The toolchain, headers, and runtime libraries all come from environment variables, `find_package(ASC)` locates the version accordingly, and both the build and run stages draw on this same set of variables. When multiple CANN versions are installed, the following two items confirm which one is in effect:

```bash
which bisheng            # should point to ${install_path}/cann-9.2.0/bin/bisheng
echo $ASCEND_HOME_PATH   # should be ${install_path}/cann-9.2.0
```

CMake writes the compiler path into the cache of the `build` directory; after switching CANN versions, delete that directory and configure again.

Build and run:

```bash
mkdir build && cd build;
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;
./cooperative_groups 0
```

The only build option is `CMAKE_ASC_ARCHITECTURES`, whose value is `dav-3510`, corresponding to the NPU architecture of Ascend 950PR/950DT. The options passed to the compiler are `--npu-arch=${CMAKE_ASC_ARCHITECTURES} --enable-simt`.

At run time the case number is given as an argument, 0, 1, or 2, and a single execution runs that one case. If the argument is missing or invalid, the usage is printed and the process returns with exit code 1:

```bash
./cooperative_groups 0    # cross-block reduction
./cooperative_groups 1    # whole-block reduction
./cooperative_groups 2    # sub-group reduction
```

Taking the whole-block scenario as an example, a passing verification prints the following, with exit code 0:

```text
Case 1: whole-block reduction (thread_block), reduce domain = 1024
num_elements = 4096, threads = 4096
grid = 4 blocks x 1024 threads, reduce domain = 1024 per block, workspace in shared memory
Output: 8390656
Golden: 8390656
[Success] Case accuracy verification passed.
```

## Performance Analysis

Single-operator performance data is collected with `msopprof`; arguments for the command under test follow the executable directly:

```bash
msopprof ./cooperative_groups 1   # Analyze whole-block reduction case performance
msopprof ./cooperative_groups 2   # Analyze sub-group reduction case performance
```

The tool prints `Op Name`, `Task Duration(us)`, `Block Dim` and tuning suggestions straight to the terminal; full results land in an `OPPROF_*` directory under the current path. Three runs each, `Task Duration(us)`:

| Case | Reduction domain | Synchronization | Task Duration(us) |
|---|---|---|---|
| Whole-block | 1024 | 20 block-level | 7.01 / 7.12 / 7.22 |
| Sub-group | 32 | 10 warp-internal + 1 block-level | 6.11 / 6.29 / 6.58 |

Sub-group reduction is roughly 10% to 13% faster than whole-block reduction, and the ranges of the three runs do not overlap.

To narrow down bottlenecks further, split the collection dimensions with `--aic-metrics`, for example `msopprof --aic-metrics=PipeUtilization,Memory ./cooperative_groups 2`. Without NPU hardware, use simulation mode instead:

```bash
msopprof simulator --soc-version=<soc_version> ./cooperative_groups 2
```

The two scenarios have the same input size, the same memory traffic, and the same workspace storage level. They differ in synchronization scope: the 5 rounds of level one in sub-group reduction use warp-internal synchronization, with a single block-level barrier between the two levels, whereas all 10 rounds of whole-block reduction use block-level barriers. The smaller the synchronization scope, the fewer threads a single barrier must wait for.

The reduction domain of cross-block reduction spans blocks, with two full-grid barriers in each of its 12 rounds, and its workspace is in global memory so every round's reads and writes leave the core. It is not on the same scale as the two scenarios in the table above and is therefore not included in the comparison.
