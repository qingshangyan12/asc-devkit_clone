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

- Example specification:

  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">StreamCompaction</td></tr>
  <tr><td align="center">Title</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td rowspan="2" align="center">Example Input</td><td align="center">input</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">input_count</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
  <tr><td rowspan="2" align="center">Example Output</td><td align="center">output</td><td align="center">[524288]</td><td align="center">int32</td><td align="center">ND</td></tr>
  <tr><td align="center">selected_count</td><td align="center">scalar</td><td align="center">uint32</td><td align="center">-</td></tr>
  </table>

- Case overview:

  This example builds an optimization path through five cases. The kernels, number of thread blocks, number of threads per block, and main changes introduced by each case are listed in the following table.

  | Case | Kernel | Blocks | Threads per Block | Changes Introduced |
  | ---- | ------ | ------ | ----------------- | ------------------ |
  | 0 | `compact_atomic_rank_kernel` | 8 | 1024 | Each thread computes independently and obtains its rank by rescanning the prefix |
  | 1 | `compact_thread_prefix_kernel` | 64 | 1024 | Thread hit flags cached in UB and prefixes accumulated serially per thread, eliminating repeated scans and atomic contention |
  | 2 | `compact_warp_shared_flags_kernel` | 64 | 1024 | Warp/lane two-level reduction shrinks the serial accumulation range from the thread count to the warp count |
  | 3 | `compact_warp_ballot_kernel` | 64 | 1024 | Warp primitives replace the explicit flag traversal of Case 2, further reducing UB accesses |
  | 4 | `compact_warp_ballot_batched_kernel` | 64 | 1024 | Each thread batches 4 elements, reducing the loop count and thus the total intra-kernel synchronization overhead |

## Example Implementation

The cases process different data volumes: Case 0 processes only 8192 elements, while the other cases process the full 524288 elements (64 blocks, each responsible for one 8192-element region). The reason is that Case 0 computes the rank by rescanning all elements before the current position, so the computation grows quadratically with the input size and the duration increases drastically on the full input; processing all elements would take too long. Even so, Case 0 still has the longest duration of all the scenarios.

Each block of Cases 1-4 launches 1024 threads but must process 8192 elements, so chunked loops are required: in Cases 1-3, each block divides its 8192-element region into 8 rounds of 1024 elements per chunk; in each round, the 1024 threads each handle 1 element and cooperatively compute the in-chunk rank, and at the end of each round the hit count of the round is accumulated into `running_offset` as the base for subsequent chunks, with `asc_syncthreads` between rounds keeping the threads of the block in step; in Case 4, each thread processes 4 consecutive elements at a time, each loop iteration covers 4096 elements, and each block needs only 2 iterations.

### Case 0: Atomic Count and GM Prefix Rescan

**Optimization objective**: Observe the cost when all threads compute fully independently in parallel, and establish the performance baseline.

**Core implementation**: 8 blocks cooperatively traverse the first 8192 elements in a grid-stride loop; a selected thread atomically updates the total count `selected_count` and calls the `count_selected_before` method to rescan the elements before the current position to obtain the output rank.

**Key code**:

```cpp
for (uint32_t input_index = grid_offset; input_index < input_count; input_index += thread_stride) {
    int32_t value = input[input_index];
    if (!is_selected_device(value)) {
        continue;
    }

    (void)asc_atomic_add(selected_count, 1U);
    uint32_t stable_rank = count_selected_before(input, input_index);
    output[stable_rank] = value;
}
```

**Performance data**:

| Implementation | Task Duration (μs) |
|:---|---:|
| Case 0 | 892.728 |

**Performance data analysis**:

The duration comes from two aspects:

- Repeated scanning: when a selected thread computes the output rank of element i, it rescans input[0..i). Among the first 8192 elements, 1487 are selected, giving a rescan volume of about 1487×8192/2 ≈ 6.1 million reads.
- Global atomic contention: multiple threads issue asc_atomic_add on the same GM address selected_count, which serializes them in a queue.

Case 0 appears to use multiple threads, but it is actually fake parallelism; its duration is even higher than running the computation serially with a single thread.

### Case 1: Caching Thread Hit States in UB

**Optimization objective**: Eliminate the repeated computation and atomic contention of Case 0.

**Core implementation**: First, each thread writes its hit flag into `thread_selected_flags`: 1 for a hit and 0 otherwise. Then, each selected thread serially accumulates the flags of all preceding threads to obtain `thread_prefix`, that is, its rank in the output.

**Key code**:

```cpp
__ubuf__ uint32_t thread_selected_flags[THREAD_COUNT];
thread_selected_flags[thread_index] = selected ? 1U : 0U;
asc_syncthreads();

uint32_t thread_prefix = 0U;
for (uint32_t earlier = 0; earlier < thread_index; ++earlier) {
    thread_prefix += thread_selected_flags[earlier];
}
```

**Performance data**:
| Implementation | Task Duration (μs) |
|:---|---:|
| Case 1 | 353.022 |

**Performance data analysis**:
Compared with Case 0, the duration drops from 892.728 μs to 353.022 μs (about 2.5x): the repeated prefix scans of about 6.1 million reads and the same-address atomic contention are eliminated, each element is read only once, and the stable rank is incrementally maintained from the hit flags in UB. The remaining costs are the thread-level serial accumulation (up to 1024 UB reads per thread per chunk) and 4 block-level synchronizations per chunk, which Case 2 continues to optimize.

### Case 2: Two-Level Prefix Reduction

**Optimization objective**: Turn the thread-level serial accumulation of Case 1 into hierarchical reduction, reducing the total number of UB accesses.

**Core implementation**:
As in Case 1, the thread hit states `thread_selected_flags` are obtained first, and then:
1. Each selected thread computes its offset within its warp, `lane_offset`, and the sum of the hit counts of all threads in the warp, `warp_selected_count`.
2. From `warp_selected_counts[]`, the offset of each warp among all warps, `warp_selected_offsets[]`, is obtained.

The in-chunk rank of a selected element equals the sum of `warp_selected_offset` and `lane_offset`.

**Key code**:

```cpp
uint32_t warp_base = warp_index * WARP_SIZE;
uint32_t lane_offset = 0U;
uint32_t warp_selected_count = 0U;
for (uint32_t lane = 0U; lane < WARP_SIZE; ++lane) {
    uint32_t flag = thread_selected_flags[warp_base + lane];
    warp_selected_count += flag;
    if (lane < lane_index) {
        lane_offset += flag;
    }
}
if (lane_index == 0) {
    warp_selected_counts[warp_index] = warp_selected_count;
}

if (thread_index < WARP_COUNT) {
    const uint32_t lane_index = (uint32_t)laneid();
    uint32_t own_count = warp_selected_counts[thread_index];
    uint32_t inclusive = own_count;
    for (uint32_t d = 1; d < WARP_SIZE; d <<= 1) {
        uint32_t n = (uint32_t)asc_shfl_up((int32_t)inclusive, d);
        if (lane_index >= d) {
            inclusive += n;
        }
    }
    warp_selected_offsets[thread_index] = inclusive - own_count;
}
```

**Performance data**:
| Implementation | Task Duration (μs) |
|:---|---:|
| Case 2 | 83.921 |

**Performance data analysis**:
In Case 1, the number of accesses to `thread_selected_flags` within a single chunk is about 1024\*(1024-1)/2 ≈ 520 thousand; in Case 2 it is about 1024\*32 + 2\*(32-1)/2 ≈ 33 thousand, so the UB access volume is greatly reduced, which brings the performance improvement.

### Case 3: Replacing Explicit Flag Traversal with Warp Primitives

**Optimization objective**: Eliminate the UB accesses and loop traversal of `thread_selected_flags` in Case 2.

**Core implementation**: Use `asc_ballot` to pack the hit states of the threads in a warp into the mask `selected_mask`, then use `__popc` together with `lanemask_lt` to compute `warp_selected_count` and `lane_offset`.

**Key code**:

```cpp
uint32_t selected_mask = asc_ballot(selected ? 1 : 0);
uint32_t warp_selected_count = (uint32_t)__popc(selected_mask);
uint32_t lane_offset = (uint32_t)__popc(selected_mask & (uint32_t)lanemask_lt());
```

**Performance data**:
| Implementation | Task Duration (μs) |
|:---|---:|
| Case 3 | 20.087 |

**Performance data analysis**:
Case 3 uses these warp interfaces to exchange data between threads directly at the register level, which is not only faster than accessing UB but also removes the need to maintain and access `thread_selected_flags`, further reducing the computation time compared with Case 2.

### Case 4: Batched Compaction, 4 Elements per Thread at a Time

**Optimization objective**: Reduce the intra-kernel synchronization overhead that grows linearly with the chunk loop count in Case 3.

**Core implementation**: Each thread reads 4 consecutive elements at once and packs the hit states into the 4-bit bitmap `valid_bits`. One batch covers 4096 inputs, so the whole region needs only 2 batches (Case 3 uses 8 chunks). The in-batch rank is assembled from four parts: `running_offset` (accumulated across chunks) + `warp_selected_offset` + the intra-warp lane prefix + `sub_offset` (the intra-thread sub-offset), which adds a layer of intra-thread reduction on top of the inter-warp and intra-warp reductions.

**Key code**:

```cpp
// Each thread processes ITEMS_PER_THREAD elements in one chunk loop iteration
for (uint32_t k = 0; k < ITEMS_PER_THREAD; ++k) {
    uint32_t idx = my_base + k;
    bool valid = idx < region_count;
    int32_t value = valid ? input[idx] : 0;
    values[k] = value;
    valid_bits |= (valid && is_selected_device(value)) ? (1U << k) : 0U;
}
uint32_t thread_hits = (uint32_t)__popc(valid_bits);

// When computing the rank, the intra-thread offset sub_offset of the selected element is also needed
uint32_t inclusive_prefix = warp_inclusive_scan(thread_hits);

uint32_t my_base_rank = running_offsets[0] + warp_selected_offsets[warp_index] + (inclusive_prefix - (uint32_t)__popc(valid_bits));
for (uint32_t k = 0; k < ITEMS_PER_THREAD; ++k) {
    if ((valid_bits & (1U << k)) != 0U) {
        uint32_t sub_offset = (uint32_t)__popc(valid_bits & ((1U << k) - 1U));
        output[my_base_rank + sub_offset] = values[k];
    }
}
```

**Performance data**:
| Implementation | Task Duration (μs) |
|:---|---:|
| Case 4 | 16.026 |

**Performance data analysis**:
In Case 3, each chunk loop iteration requires 5 intra-kernel synchronizations; when the total duration is low, the synchronization overhead takes a larger share. Case 4 reduces this from 5×8 (about 40) synchronizations in Case 3 to 5×2 (about 10), removing a large number of synchronization calls. Although batching brings the cost that the start addresses of adjacent threads within a warp are 16 bytes apart and memory access is no longer contiguous, the gain from reducing the synchronization count is higher.

## Performance Comparison Summary

The following table lists the performance of each case processing 524288 int32 elements (Case 0 compacts only the first 8192), collected with `msOpProf` on Ascend 950PR (`dav-3510`, 1650 MHz, CANN 9.3.0). Actual performance is affected by the chip frequency, CANN version, and input size. Performance data should be compared on the same device, frequency, CANN version, and input size.

| Case | Implementation | Task Duration (μs) | Duration relative to Case 0 |
|:---|:---|---:|---:|
| 0 | Atomic count and GM prefix rescan | 892.728 | 1X |
| 1 | Caching thread hit states in UB | 353.022 | 0.395X |
| 2 | Two-level prefix reduction | 83.921 | 0.094x |
| 3 | Warp primitives replace explicit flag traversal | 20.087 | 0.023X |
| 4 | Batched compaction | 16.026 | 0.018X |

## Tuning Recommendations

- Prefer warp primitives to exchange data between threads at the register level, instead of using UB arrays plus explicit loops.
- When the total duration is low, the intra-kernel synchronization overhead takes a larger share; at that point, consider reducing the synchronization count.

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
  SCENARIO_NUM=4                                                                     # Select the scenario to run (0-4)
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
  | `SCENARIO_NUM` | `0`-`4` | Example type, default is 4 |

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
