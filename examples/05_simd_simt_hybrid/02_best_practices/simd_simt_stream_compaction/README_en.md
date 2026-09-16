# SIMD and SIMT Hybrid Stable Stream Compaction Performance Optimization Example

## Overview

This example uses the stable stream compaction operator to demonstrate how to use Warp functions to replace explicit UB traversal in SIMD-SIMT hybrid programming, so as to accelerate computation.

## Supported Products and CANN Versions

| Product | CANN version |
|------|-------------|
| Ascend 950PR&950DT series products | >= CANN 9.2.0 |

## Directory Structure

```text
├── simd_simt_stream_compaction
│   ├── CMakeLists.txt              // Build project file
│   ├── stream_compaction.asc       // Example implementation
│   ├── figures                     // Image resources used in the README
│   ├── README.md                   // Example description (Chinese)
│   └── README_en.md                // English example description
```

## Example Description

- Operator function:

  Stream Compaction refers to: given an input sequence and a condition that target elements must satisfy, output the set of elements satisfying the condition, with their relative order consistent with that in the original sequence. The key difficulty of this operator is that no thread knows the position of a selected element in the output in advance; the position must be computed cooperatively by threads.

- Formula:

  Let the input be `input[0..N)` and the selection predicate be `P(x)`. The output contains all elements satisfying `P(input[i])` in their input order. The output position rank of each selected element is computed as follows:

  ```text
  rank(i) = count { j | 0 <= j < i, P(input[j]) }
  ```

  > The predicate used in this example selects the even numbers of the input, that is, x % 2 = 0.

- Example specification:

  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">Title</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">input</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td><td align="center">output</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

## Example Implementation

The example adopts block-partitioned multi-core parallel computing: 64 thread blocks each handle one 8192-element region (524288 int32 elements in total, about 50% of which are selected), and the 1024 threads within each thread block process the region in 8 tile rounds (8192/1024). Because a later thread block must know how many elements the preceding thread blocks selected when computing its ranks, the kernels of all cases are implemented through two VF function calls: the first VF function counts `block_counts` (located in Global Memory, convenient for cross-kernel sharing) and other statistics, and the second VF function produces `output`, with cross-kernel synchronization in between.

### Case Implementation Description

This example builds an optimization path through three cases. The kernels, number of thread blocks, number of threads per block, and main changes introduced by each case are listed in the following table.

| Case | Kernel | Blocks | Threads per Block | Changes Introduced |
| ---- | ------ | ------ | ----------------- | ------------------ |
| 0 | compact_thread_prefix_kernel | 64 | 1024 | Thread hit flags cached in UB, prefixes accumulated serially per thread |
| 1 | compact_warp_shared_flags_kernel | 64 | 1024 | Warp/lane two-level reduction shrinks the serial accumulation range from the thread count to warpSize |
| 2 | compact_prescanned_ballot_kernel | 64 | 1024 | Warp functions replace the explicit traversal of Case 1, further reducing UB accesses |

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
| aiv_mte2_ratio | Proportion of MTE2 data-copy instruction cycles in the total cycles |
| aiv_ub_read_bw_vector(GB/s) | Bandwidth of Vector instructions reading UB, reflecting the throughput of the vec pipeline fetching data from UB |

Except for Task Duration, all metrics are averaged over all thread blocks.

### Case 0: Caching Thread Hit States in UB

**Optimization objective**: Observe the cost of computing the rank through serial accumulation per thread, and establish the performance baseline.

**Core implementation**: The first VF function allocates an array (`thread_selected_flags`) in UB shared inside the kernel to cache the hit states of all threads (1 for a hit, 0 otherwise); in the second VF function, a selected thread sums the hit flags of all preceding threads as its rank in the output. As shown in the following figure:

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
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 380.3 | 377.1 | 622290 | 373.6 | 0.9907 | 0.44 | 0.0012 | 1.37 | 0.0036 | 91.7 |

**Performance data analysis**:
The duration of Case 0 is almost entirely consumed by the serial prefix accumulation in the second VF function: each thread must accumulate the hit flags of all preceding threads one by one when computing its rank. This is a pure serial dependency chain of "read flag, accumulate". The measured aiv_vec_ratio is as high as 0.9906, indicating that the vec pipeline is fully occupied by this serial chain. The number of accesses to `thread_selected_flags` within a single tile round is about 1024×1023/2 ≈ 520 thousand, and the 8 tile rounds total about 4.2 million UB reads; the measured Vector-to-UB read bandwidth is about 91.7GB/s. This implementation has the longest duration of the three cases and serves as the performance baseline for the subsequent optimizations.

### Case 1: Two-Level Prefix Reduction

**Optimization objective**: The problem of Case 0 is that threads near the end pay a high cost for serially accumulating `thread_selected_flags` when computing their ranks; hierarchical reduction reduces the computation and data-movement counts.

**Core implementation**: The previous serial accumulation of `thread_selected_flags` is split into warp and lane two-level reduction: count the number of selected elements of each warp, as well as the position of a thread within its warp; when computing the rank, it equals the sum of the selected counts of all preceding warps plus the position within the current warp. As shown in the following figure:

![](./figures/case_1.png)

`thread_selected_flags` is the same as in Case 0; `warp_selected_counts` and `tile_base` are both located in UB, convenient for sharing among the threads of the kernel.

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
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 25.0 | 24.0 | 39553 | 20.5 | 0.8538 | 0.45 | 0.0187 | 1.38 | 0.0574 | 179.2 |

**Performance data analysis**:
Compared with Case 0, the serial scan range shrinks from 1024 threads to 32 lanes: on one hand the computation volume decreases, and on the other hand the number of accesses to `thread_selected_flags` within a single tile round drops from about 520 thousand (1024×1023/2) in Case 0 to about 33 thousand (1024×32), a roughly 16x reduction of UB accesses, with each access confined to the 32 consecutive flags of the thread's own warp; the measured Vector-to-UB read bandwidth increases from 91.7GB/s to 179.2GB/s. Combined, the two effects reduce the vec time from 373.6 μs to 20.5 μs, and the Task Duration from 380.3 μs to 25.0 μs, about 15.2x.

### Case 2: Replacing Explicit Traversal with Warp Primitives

**Optimization objective**: Further eliminate the UB accesses and loop traversal of `thread_selected_flags` in Case 1.

**Core implementation**: In Case 1, `warp_selected_count` (VF1) and `lane_offset` (VF2) are obtained by traversing UB arrays; in fact, high-performance interfaces are available for intra-warp computation. In this case, [asc_ballot](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_vote_functions/asc_ballot.md), [lanemask_lt](../../../../docs/zh/api/SIMT-API/Warp_functions/lane_id_functions/lanemask_lt.md), and [asc_reduce_add](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_reduce_functions/asc_reduce_add.md) can be used to efficiently compute `warp_selected_count` and `lane_offset`, which, combined with the precomputed output base address `tile_warp_base` of each [tile, warp], further reduces the time of computing the rank. As shown in the following figure:

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
| Task Duration(μs) | aiv_time(μs) | aiv_total_cycles | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_ub_read_bw_vector(GB/s) |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| 9.48 | 8.2 | 13547 | 4.70 | 0.5724 | 0.43 | 0.0529 | 1.37 | 0.1670 | 31.5 |

**Performance data analysis**:
Case 2 uses these warp interfaces to exchange data between threads directly at the register level, which is not only faster than accessing UB but also removes the need to maintain and access `thread_selected_flags`: within one tile round, the second VF function needs only one `asc_ballot` and one `__popc` to obtain the intra-warp offset, and in the first VF function the per-warp selected count is produced by the single instruction `asc_reduce_add`. Compared with Case 1, the Vector-to-UB read bandwidth drops from 179.2GB/s to 31.5GB/s (the flag-array traffic essentially disappears), the vec time drops from 20.5 μs to 4.70 μs, and the Task Duration drops from 25.0 μs to 9.48 μs, about 2.6x.

## Performance Comparison Summary

The following table lists the Task Duration of each case at the same input size:

| Case | Implementation | Task Duration(μs) | Performance relative to Case 0 |
|:---|:---|---:|---:|
| 0 | Caching thread hit states in UB | 380.3 | 1X |
| 1 | Two-level prefix reduction | 25.0 | 15.2X |
| 2 | Warp functions replace explicit traversal | 9.48 | 40.1X |

The three cases are progressively optimized: Case 1 converges the serial accumulation from thread block level to warp level, mainly reducing the total number of UB reads; Case 2 further moves the intra-warp cooperation to the register level with warp functions, mainly reducing the vec instruction time. In terms of the metric changes, the source of the performance improvement is clearly separable: aiv_vec_time drops from 373.6 μs (Case 0) to 20.5 μs (Case 1) and further to 4.70 μs (Case 2), a total drop of about 80x, which is the main body of the total duration reduction; aiv_mte2_time remains basically unchanged across the three cases (about 1.4 μs), so its proportion instead rises from 0.4% to 16.7%. Overall, in the final Task Duration of 9.48 μs, vec computation accounts for about 4.7 μs and mte2 transfer for about 1.4 μs, with the rest being scheduling and cross-kernel synchronization overhead.

## Tuning Recommendations

- In the SIMT programming model, to fully exploit the advantage of multi-threaded parallelism, avoid or minimize executing large loops within a single thread.
- Prefer [Warp functions](../../../../docs/zh/api/SIMT-API/Warp_functions/Warp_functions.md) to exchange data between threads at the register level, instead of using UB arrays plus explicit loops.

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

  > **Note:** Clear the cmake cache before switching build modes. Execute `rm CMakeCache.txt` in the build directory and run cmake again.

- Build option description

  | Option | Values | Description |
  |:---|:---|:---|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `sim` | Run mode: NPU execution, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example supports only dav-3510 (Ascend 950PR&950DT series products) |
  | `SCENARIO_NUM` | `0`-`2` | Example type, default is 2 |

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
