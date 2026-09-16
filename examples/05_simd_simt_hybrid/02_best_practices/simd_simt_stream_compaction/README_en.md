# SIMD and SIMT Hybrid Stable Stream Compaction Performance Optimization Example

## Overview

This example uses the stable stream compaction operator to demonstrate how to use warp primitives to replace explicit loops and UB array accesses in SIMD-SIMT hybrid programming, so as to accelerate computation.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.2.0 |

## Directory Structure

```text
├── simd_simt_stream_compaction
│   ├── CMakeLists.txt              // Build project file
│   ├── stream_compaction.asc       // Example implementation
│   ├── README.md                   // Example description (Chinese)
│   └── README_en.md                // English example description
```

## Example Description

- Operator function:
  Stream compaction outputs all elements of an input sequence that satisfy a given predicate, preserving their relative order in the input. Its key property is that no thread knows its output position in advance; the position must be computed cooperatively by threads.

- Formula:

  Let the input be `input[0..N)` and the selection predicate be `P(x)`. The output contains all elements satisfying `P(input[i])` in their input order. The output position rank of each selected element is computed as follows:

  ```text
  rank(i) = count { j | 0 <= j < i, P(input[j]) }
  ```

  > The predicate used in this example selects the even numbers of the input, that is, x % 2 == 0.

- Example specification:

  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">Title</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">input</td><td align="center">[516096]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td><td align="center">output</td><td align="center">[516096]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">[1]</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

## Example Implementation

The example adopts block-partitioned multi-core parallel computing: 63 blocks each handle one 8192-element region (516096 int32 elements in total, about 50% of which are selected), and the 1024 threads within each block process the region in 8 tile rounds (8192/1024). Cross-block data interaction is involved: a later block must know how many elements the preceding blocks selected when computing its ranks. Therefore every kernel is implemented through two VF function calls: the first VF function counts `block_counts` and other statistics, and the second VF function produces `output`.

### Case overview:

This example builds an optimization path through three cases. The kernels, number of thread blocks, number of threads per block, and main changes introduced by each case are listed in the following table.

| Case | Kernel | Blocks | Threads per Block | Changes Introduced |
| ---- | ------ | ------ | ----------------- | ------------------ |
| 0 | `compact_thread_prefix_kernel` | 63 | 1024 | Thread hit flags cached in UB, prefixes accumulated serially per thread |
| 1 | `compact_warp_shared_flags_kernel` | 63 | 1024 | Warp/lane two-level reduction shrinks the serial accumulation range from the thread count to the lane count |
| 2 | `compact_prescanned_ballot_kernel` | 63 | 1024 | Warp primitives replace the explicit flag traversal of Case 1, further reducing UB accesses |

### Performance Metric Description

| Metric | Description |
| ------ | ----------- |
| Task Duration(μs) | Overall task duration, including the time scheduled to the accelerator, the execution time on the accelerator, and the response completion time |
| aiv_time(μs) | Theoretical execution time of the task on the AI Vector Core, in μs |
| aiv_total_cycles | Total execution cycles after the task is assigned to each AI Vector Core compute unit |
| aiv_vec_time(μs) | Duration of vec-type instructions (vector computation instructions); the thread computation in SIMT VF calls is also reflected in this statistic |
| aiv_vec_ratio | Proportion of vec-type instruction cycles in the total cycles |
| aiv_scalar_time(μs) | Duration of scalar-type instructions (scalar computation instructions), in μs |
| aiv_scalar_ratio | Proportion of scalar-type instruction cycles in the total cycles |
| aiv_mte2_time(μs) | Duration of MTE2 data-copy instructions, mainly corresponding to GM-to-UB transfers |
| aiv_mte3_time(μs) | Duration of MTE3 data-copy instructions, mainly corresponding to UB-to-GM transfers |

Except for Task Duration, all metrics are averaged over all thread blocks.

### Case 0: Caching Thread Hit States in UB

**Optimization objective**: Observe the cost of computing the rank through serial accumulation per thread, and establish the performance baseline.

**Core implementation**: The first VF function allocates an array (`thread_selected_flags`) in UB shared inside the kernel to cache the hit states of all threads (1 for a hit, 0 otherwise); a selected thread sums the hit flags of all preceding threads as its rank in the output. As shown in the following figure:
![](./figures/case_0.png)

**Key code**:

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t input_index = tile * THREAD_COUNT + thread_index;
    int32_t value = input[input_index];
    uint32_t hit = thread_selected_flags[input_index];

    uint32_t thread_prefix = 0U;
    for (uint32_t earlier = 0; earlier < thread_index; ++earlier) {
        thread_prefix += thread_selected_flags[tile * THREAD_COUNT + earlier];
    }

    if (hit == 1U) {
        output[block_base + tile_base[tile] + thread_prefix] = value;
    }
}
```

**Performance data**:
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 380.3 | 377.1 | 622290 | 373.6 | 0.9906 | 0.42 | 0.0011 | 1.34 | 0.0036 | 0.002 | 0.0000 |

**Performance data analysis**:
The duration of Case 0 is almost entirely consumed by the serial prefix accumulation in the second VF function: each thread must accumulate the hit flags of all preceding threads one by one when computing its rank. This is a pure serial dependency chain of "read flag, accumulate", so the threads cannot proceed in parallel. The measured aiv_vec_ratio is as high as 0.9906, indicating that the vec pipeline is fully occupied by this serial chain. The number of accesses to `thread_selected_flags` within a single tile round is about 1024×1023/2 ≈ 520 thousand, and the 8 tile rounds total about 4.2 million UB reads; the measured Vector-to-UB read bandwidth is about 91.7 GB/s. This implementation has the longest duration of the three cases and serves as the performance baseline for the subsequent optimizations.

### Case 1: Two-Level Prefix Reduction

**Optimization objective**: The problem of Case 0 is that threads near the end pay a high cost for serially accumulating `thread_selected_flags` when computing their ranks; hierarchical reduction reduces the computation and data-movement counts.

**Core implementation**: Reduction at the warp and lane levels: count the number of selected elements of each warp, as well as the position of a thread within its warp; the rank then equals the sum of the selected counts of all preceding warps plus the position within the current warp. As shown in the following figure:
![](./figures/case_1.png)
`thread_selected_flags` is the same as in Case 0; `warp_partials` (first storing the per-warp selected counts, then overwritten with the intra-tile exclusive prefixes) and `tile_base` are both located in UB, convenient for sharing among the threads of the kernel.

**Key code**:

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t input_index = tile * THREAD_COUNT + thread_index;
    int32_t value = input[input_index];
    uint32_t hit = thread_selected_flags[input_index];

    const uint32_t warp_base = warp_index * warpSize;
    const uint32_t flag_base = tile * THREAD_COUNT + warp_base;
    uint32_t lane_offset = 0U;
    for (uint32_t lane = 0U; lane < warpSize; ++lane) {
        if (lane < (uint32_t)laneid()) {
            lane_offset += thread_selected_flags[flag_base + lane];
        }
    }

    if (hit == 1U) {
        uint32_t stable_rank =
            block_base + tile_base[tile] + warp_bases[tile * WARP_COUNT + warp_index] + lane_offset;
        output[stable_rank] = value;
    }
}
```

**Performance data**:
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 25.0 | 23.8 | 39218 | 20.3 | 0.8538 | 0.46 | 0.0195 | 1.36 | 0.0573 | 0.002 | 0.0001 |

**Performance data analysis**:
Compared with Case 0, the serial scan range shrinks from 1024 threads to 32 lanes: on one hand the computation volume decreases, and on the other hand the number of accesses to `thread_selected_flags` within a single tile round drops from about 520 thousand (1024×1023/2) in Case 0 to about 33 thousand (1024×32), a roughly 16x reduction of UB accesses, with each access confined to the 32 consecutive flags of the thread's own warp; the measured Vector-to-UB read bandwidth increases from 91.7 GB/s to 180.7 GB/s. Combined, the two effects reduce the vec time from 373.6 μs to 20.3 μs, and the Task Duration from 380.3 μs to 25.0 μs, about 15.2x.

### Case 2: Replacing Explicit Traversal with Warp Primitives

**Optimization objective**: Further eliminate the UB accesses and loop traversal of `thread_selected_flags` in Case 1.

**Core implementation**: In the second VF function, use [asc_ballot](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_vote_functions/asc_ballot.md) to pack the hit states of the threads in a warp into the mask `selected_mask`, then use [__popc](../../../../docs/zh/api/SIMT-API/math_functions/integer_math_functions/__popc.md) together with [lanemask_lt](../../../../docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_lt.md) to compute the intra-warp offset `lane_offset`; in the first VF function, use `asc_reduce_add` to count the selected elements of each warp and precompute the output base address `tile_warp_base` of every [tile, warp], so the second VF function obtains the rank by a simple table lookup. As shown in the following figure:
![](./figures/case_2.png)

**Key code**:

```cpp
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t idx = tile * THREAD_COUNT + thread_index;
    uint32_t hit = is_selected_device(input[idx]) ? 1U : 0U;
    uint32_t warp_selected_count = (uint32_t)asc_reduce_add(hit);
    if (lane_index == 0U) {
        tile_warp_base[tile * WARP_COUNT + warp_index] = warp_selected_count;
    }
}

//...
for (uint32_t tile = 0U; tile < TILE_COUNT; ++tile) {
    uint32_t idx = tile * THREAD_COUNT + thread_index;
    int32_t value = input[idx];
    bool selected = is_selected_device(value);
    uint32_t selected_mask = asc_ballot(selected ? 1 : 0);
    if (selected) {
        uint32_t lane_offset = (uint32_t)__popc(selected_mask & (uint32_t)lanemask_lt());
        output[block_base + tile_warp_base[tile * WARP_COUNT + warp_index] + lane_offset] = value;
    }
}
```

**Performance data**:
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 9.48 | 8.2 | 13542 | 4.65 | 0.5667 | 0.42 | 0.0505 | 1.35 | 0.1639 | 0.002 | 0.0003 |

**Performance data analysis**:
Case 2 uses these warp interfaces to exchange data between threads directly at the register level, which is not only faster than accessing UB but also removes the need to maintain and access `thread_selected_flags`: within one tile round, the second VF function needs only one `asc_ballot` and one `__popc` to obtain the intra-warp offset, and in the first VF function the per-warp selected count is produced by the single instruction `asc_reduce_add`. Compared with Case 1, the Vector-to-UB read bandwidth drops from 180.7 GB/s to 31.5 GB/s (the flag-array traffic essentially disappears), the vec time drops from 20.3 μs to 4.65 μs, and the Task Duration drops from 25.0 μs to 9.48 μs, about 2.6x. At this point the mte2 proportion rises to 16.4%, and the GM-to-UB data transfer becomes one of the dominant remaining costs.

## Performance Comparison Summary

The following performance data was measured on Ascend 950PR (CANN 9.3.0, `dav-3510` architecture, 1650 MHz), with an input of 516096 int32 elements and about 50% selected.

The following table lists the Task Duration of each case at the same input size:

| Case | Implementation | Task Duration(μs) | Duration relative to Case 0 |
|:---|:---|---:|---:|
| 0 | Caching thread hit states in UB | 380.3 | 1X |
| 1 | Two-level prefix reduction | 25.0 | 15.2X |
| 2 | Warp primitives replace explicit flag traversal | 9.48 | 40.1X |

## Tuning Recommendations

- Prefer warp primitives (`asc_ballot`, `asc_reduce_add`, `asc_shfl_up`, and the like) to exchange data between threads at the register level, instead of using UB arrays plus explicit loops; in this example, that optimization path delivers an overall speedup of about 40x.
- When data must be exchanged through UB, shrink the serial accumulation range as much as possible (for example, from thread level down to warp/lane level), and keep each warp accessing its own consecutive data segment, to improve UB access efficiency.

## Build and Run

Run the following steps in the root directory of this example to build and run the example.

- Configure environment variables

  Configure environment variables based on the [installation method](../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in this example directory.

  ```bash
  SCENARIO_NUM=2                                                                     # Select the scenario to run (0-2)
  mkdir -p build && cd build;                                                        # Create and enter the build directory
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j;  # Build the project
  ./stream_compaction                                                                # Run the sample
  ```

  To use the NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Example:
  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..; make -j;   # NPU simulation mode
  ```

  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory and run cmake again. If an existing build directory is reused to switch back to NPU mode, explicitly pass `-DCMAKE_ASC_RUN_MODE=npu`, because CMake keeps the old cache.

- Build option description

  | Option | Values | Description |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `0`-`2` | Example type, specified at build time via `-DSCENARIO_NUM=` |

- Execution result

  The following execution result indicates that the accuracy comparison is successful.

  ```text
  [Success] Case accuracy verification passed.
  ```

## Performance Debugging

### Introduction to the msOpProf Tool

`msOpProf` is a single-operator performance analysis tool. It offers two usage methods: `msopprof` and `msopprof simulator`. The tool helps users identify anomalies in operator memory, operator code, and operator instructions, enabling comprehensive operator tuning. It currently supports performance data collection and automatic parsing for different run modes (on-device or simulation) and different file types (executables or operator binary `.o` files).

On-device performance collection directly measures the execution time of an operator on an Ascend AI Processor. This method is suitable for quickly locating operator performance issues in an on-device environment.

Use the `msOpProf` tool to obtain detailed performance data:

```bash
msopprof ./stream_compaction
```

After the command completes, a performance data folder named "OPPROF_{timestamp}_XXX" is generated in the default directory. The folder structure is as follows:

```bash
├──dump                       # Raw performance data; users do not need to inspect it
├──ArithmeticUtilization.csv  # Cube/Vector instruction cycle proportions
├──L2Cache.csv                # L2 Cache hit rate; affects MTE2. Plan data transfer logic properly to increase the hit rate
├──Memory.csv                 # Read/write bandwidth rates of UB, L1, and main memory
├──MemoryL0.csv               # Read/write bandwidth rates of L0A, L0B, and L0C
├──MemoryUB.csv               # Read/write bandwidth rates from Vector and Scalar to UB
├──OpBasicInfo.csv            # Basic operator information (including Task Duration)
├──PipeUtilization.csv        # Durations and proportions of computation and data transfer units
├──ResourceConflictRatio.csv  # Proportions of UB bank groups, bank conflicts, and resource conflicts among all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```

View specific performance analysis results:

```bash
# View Task Duration and various metrics
cat ./OPPROF_*/PipeUtilization.csv
```
