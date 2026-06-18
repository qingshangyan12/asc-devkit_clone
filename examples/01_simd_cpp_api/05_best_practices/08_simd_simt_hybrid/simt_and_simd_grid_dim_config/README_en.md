# Thread Block Count Configuration and VF Call Optimization Example

## Overview

This example uses Gather computation to introduce thread block count configuration and vf function call optimization in the Ascend C SIMD and SIMT hybrid programming scenario. The example includes 4 Cases (16 scenarios), ranging from empty kernel function scheduling overhead to thread block count configuration optimization under different data sizes.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Versions

- \> CANN 9.0.0

## Directory Structure

```text
├── simt_and_simd_grid_dim_config
│   ├── figures                // Image resources for README
│   ├── CMakeLists.txt         // Build project file
│   ├── grid_config.asc        // Ascend C operator implementation & invocation example
│   └── README.md
```

## Example Description

- Computation formula:

  $$output[i][j] = input[index[i][j]]$$

  - input is the input tensor, data type float, shape [2200000]
  - index is the index tensor, data type uint32, shape varies by Case: Case 0/1/2 is [1024, 2048], Case 3 is [8, 2048], index range is [0, input_total_length)
  - output is the output tensor, data type float, shape is the same as index
  - Computation process: Collect data from the corresponding position in input based on index[i][j], and write to the output[i][j] position

- Example specifications:

  <table>
  <tr><td rowspan="1" align="center">Example Type (OpType)</td><td colspan="4" align="center">Gather</td></tr>
  <tr><td rowspan="3" align="center">Example Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">input</td><td align="center">[2200000]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">index</td><td align="center">[1024,2048]/[8,2048]</td><td align="center">uint32</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Example Output</td><td align="center">output</td><td align="center">[1024,2048]/[8,2048]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Function Name</td><td colspan="4" align="center">empty_kernel / gather_kernel</td></tr>
  </table>

## Example Implementation

### Case Implementation Description

This example controls 16 test scenarios through the build macro `SCENARIO_NUM`, grouped into 4 Case categories, from empty kernel function scheduling overhead to vf call count, and configuration choices for large and small shapes.

| Case   | SCENARIO_NUM           | Shape                      | Kernel Function                                  | Comparison Purpose                                  |
| ------ | ---------------------- | --------------------------------- | --------------------------------------- | ----------------------------------------- |
| Case 0 | 1-5     | [1024, 2048]           | empty_kernel                            | Thread block count set to 1/32/64/128/1024, comparing scheduling overhead for different thread block counts |
| Case 1 | 6-8              | [1024, 2048]              | empty_kernel                            | Thread block count set to 32/64/1024, comparing scheduling overhead of 1 vf call vs 2 vf calls        |
| Case 2 | 9-11            | [1024, 2048]      | gather_kernel                      | For large shape, thread block count set to 1024/64/32, comparing performance differences |
| Case 3 | 12-16 | [8, 2048] | gather_kernel | For small shape, thread block count set to 4/8/16/32/64, thread count allocated by per-core processing count, comparing performance differences |

### Performance Metric Description

| Metric                | Description                                                                                      |
| ------------------- | ----------------------------------------------------------------------------------------- |
| Task Duration(μs)   | Total Task duration, including scheduling time to the accelerator, execution time on the accelerator, and completion response time                  |

### Case 0: Empty Kernel Scheduling Overhead (SCENARIO_NUM=1-5)

**Example Objective**:

By calling an empty kernel function, demonstrate the fixed Task startup overhead under different thread block count configurations.

**Core Implementation**:

- The kernel function `empty_kernel` only calls the empty vf function `simt_empty`, with no actual computation logic in the function body, used to isolate testing the overhead from different thread block count configurations
- Controls five thread block count configurations through `SCENARIO_NUM=1~5`: 1/32/64/128/1024, thread count fixed at 2048, 1 vf call per invocation

**Key Code**:

```cpp
__simt_vf__ __launch_bounds__(MAX_THREAD_NUM) inline void simt_empty(...)
{}

__global__ __vector__ void empty_kernel(...)
{
    for (uint32_t vf_idx = 0; vf_idx < vf_call_times; vf_idx++) {
        asc_vf_call<simt_empty>(dim3(MAX_THREAD_NUM), ...);
    }
}
```

**Performance Data**:

| SCENARIO_NUM | Thread Block Count | Task Duration(μs) |
| :----------: | :-------: | :---------------: |
| 1 | 1 | 3.933 |
| 2 | 32 | 4.413 |
| 3 | 64 | 4.602 |
| 4 | 128 | 5.269 |
| 5 | 1024 | 13.687 |

**Analysis**:

- 1→32→64→128: Task Duration increases from 3.933μs to 5.269μs, scheduling overhead grows progressively
- 128→1024: Task Duration jumps from 5.269μs to 13.687μs, scheduling overhead increases significantly with thread block count

**Conclusion**:

Scheduling overhead grows as thread block count increases. The more thread blocks, the larger the fixed Task startup overhead.

---

### Case 1: Impact of VF Call Count on Header Overhead (SCENARIO_NUM=6-8)

**Example Objective**:

Building on Case 0, compare the impact of 1 vs 2 vf calls on Task Duration, demonstrating the relationship between vf call count and scheduling header overhead.

**Core Implementation**:

- Uses the same empty kernel function `empty_kernel` as Case 0, with the only change being increasing vf call count from 1 to 2, controlled by configuring vf_call_times
- Launches 32, 64, 1024 thread blocks through `SCENARIO_NUM=6-8`, with 2 vf calls per invocation

**Performance Data**:

| SCENARIO_NUM | Thread Block Count | VF Call Count | Task Duration(μs) |
| :----------: | :-------: | :--------: | :---------------: |
| 6 | 32 | 2 | 4.503 |
| 7 | 64 | 2 | 4.869 |
| 8 | 1024 | 2 | 14.982 |

**Comparison with Case 0**:

| Comparison | Thread Block Count | VF Count Change | Task Duration Change | Increase |
| ---- | :-------: | :--------: | :---------------: | :--: |
| SCENARIO_NUM=2→6 | 32 | 1→2 | 4.413 → 4.503 | +2.0% |
| SCENARIO_NUM=3→7 | 64 | 1→2 | 4.602 → 4.869 | +5.8% |
| SCENARIO_NUM=5→8 | 1024 | 1→2 | 13.687 → 14.982 | +9.5% |

Performance data is shown in the following figure:

<img src="figures/fig_case01_scheduling.png" alt="Case 0/1 Scheduling Overhead: Thread Block Count and VF Call Count" width="600" />

**Analysis**:

- At 32 and 64 cores, 1→2 vf call duration increase is approximately 2.0% and 5.8%, a small increase
- At 1024 cores, 1→2 vf call duration increase is approximately 9.5%, a relatively large increase

**Conclusion**:

VF calls also have scheduling overhead. Try to use 1 vf call to process all data.

---

### Case 2: Large Shape Gather Performance Comparison with Different Thread Block Counts (SCENARIO_NUM=9-11)

**Example Objective**:

Compare performance differences when executing Gather computation with the same large shape but different thread block count configurations.

**Core Implementation**:

- All three scenarios use the kernel function `gather_kernel`, differing in thread block count and data volume processed per thread
- input shape is [2200000], index and output shape is [1024, 2048]
- Gather operator computation logic is relatively simple, mainly affected by memory access bandwidth, belonging to the Memory Bound type. The overall computation volume is small with few registers used, so the thread count per block can be configured as 2048
- The vf function `simt_gather` uses a grid-stride loop: starting with `blockIdx.x * blockDim.x + threadIdx.x` as the initial index, stepping by `gridDim.x * blockDim.x` to traverse the index array, with each thread processing all elements assigned to the block via a for loop
- Thread count is allocated through `resolve_thread_num` based on actual elements processed per core: when elements per core ≥ 2048, use 2048; otherwise use the actual element count

**Key Code**:

```cpp
// simt_gather: SIMT vf function — grid-stride loop, traverses the entire index array with total thread count as stride
__simt_vf__ __launch_bounds__(MAX_THREAD_NUM) inline void simt_gather(..., uint32_t index_total_length)
{
    uint32_t idx = blockIdx.x * blockDim.x + threadIdx.x;
    uint32_t stride = gridDim.x * blockDim.x;
    for (uint32_t i = idx; i < index_total_length; i += stride) {
        uint32_t gather_idx = index[i];
        if (gather_idx >= input_total_length) { gather_idx = 0; }
        output[i] = input[gather_idx];
    }
}

// gather_kernel: Kernel function calls simt_gather, thread count adaptively allocated by resolve_thread_num
__global__ __vector__ void gather_kernel(..., uint32_t thread_num)
{
    asc_vf_call<simt_gather>(dim3(thread_num), ..., index_total_length);
}
```

#### Logical Cores and Physical Cores

When launching SIMT kernel functions, the first parameter in the kernel invocation <<<>>> determines the number of thread blocks to launch. For this example, the thread block count can be understood as the **logical core count** to launch.

These logical cores must ultimately be mapped to the actual Vector Cores available in hardware, requiring introduction of two concepts:

- **Logical cores**: The number of thread blocks launched, representing the number of parallel tasks the software wishes to launch
- **Physical cores**: The actual number of Vector Cores available in hardware, which must be queried at runtime through [aclrtGetDeviceInfo](https://www.hiascend.com/document/detail/zh/CANNCommunityEdition/910beta1/API/runtimeapi/aclcppdevg_03_1867.html) (64 in this example environment). Code example:
  ```cpp
  // Runtime query of physical Vector Core (AIV) count; returns AIV_CORE_NUM on query failure
  uint32_t query_aiv_core_num(int32_t device_id)
  {
      int64_t core_num = 0;
      aclError ret = aclrtGetDeviceInfo(
          static_cast<uint32_t>(device_id), ACL_DEV_ATTR_VECTOR_CORE_NUM, &core_num);
      return (ret == ACL_SUCCESS && core_num > 0) ? static_cast<uint32_t>(core_num) : AIV_CORE_NUM;
  }
  ```

The relationship between the two affects scheduling overhead: when logical core count ≤ physical core count, all logical cores can occupy physical cores at once for parallel execution; when logical core count > physical core count, excess logical cores must wait for preceding logical cores to finish and release physical cores before being scheduled, with waiting overhead increasing as the number of excess logical cores grows.

**Performance Data**:

| SCENARIO_NUM | Thread Block Count | Elements Per Core | Elements Per Thread | Task Duration(μs) |
| :----------: | :-------: | :-------: | :-------: | :---------------: |
| 9 | 1024 | 2048 | 1 | 78.533 |
| **10** | **64** | **32768** | **16** | **62.649** |
| 11 | 32 | 65536 | 32 | 113.609 |

Performance data is shown in the following figure:

<img src="figures/fig_case2_large.png" alt="Case 2 Task Duration vs Thread Block Count" width="600" />

**Analysis**:

- **`Thread block count=64` compared to `thread block count=1024`**, Task Duration decreases from 78.533μs to 62.649μs, a duration reduction of approximately 20.2%, performance improvement of approximately 26%**. Thread block count decreases from 1024 to 64, significantly reducing the number of thread blocks launched and scheduled, greatly reducing scheduling overhead
- **`Thread block count=64` compared to `thread block count=32`**, Task Duration decreases from 113.609μs to 62.649μs, a duration reduction of approximately 44.8%, performance improvement of approximately 82%**. With the same grid-stride loop pattern, `thread block count=64` provides higher parallelism; although increased thread block count brings more launch and scheduling overhead, this overhead is less than the benefit from improved parallelism, resulting in lower overall duration
- Limited by hardware resources, the number of physical cores that can execute in parallel has an upper bound. Since one physical core can only host and execute one thread block at a time, when `thread block count=1024`, thread blocks exceeding the physical core count must wait for preceding thread blocks to complete before being scheduled, with additional launch and scheduling fixed overhead increasing significantly; when `thread block count=32`, only half the physical cores are used, with many idle physical cores, insufficient parallelism, and doubled workload per thread block

**Conclusion**:

For large shapes, prioritize matching thread block count to actual physical core count. In this example, `thread block count=64` both avoids the large thread block scheduling overhead from `thread block count=1024` and avoids the insufficient parallelism from `thread block count=32`, making it the optimal configuration in Case 2.

---

### Case 3: Small Shape Gather Performance Comparison with Different Thread Block and Thread Counts (SCENARIO_NUM=12-16)

**Example Objective**:

Compare performance differences when executing Gather computation with the same small shape but different thread block count and thread count configurations.

**Core Implementation**:

- All five scenarios use the kernel function `gather_kernel`
- input shape is [2200000], index and output shape is [8, 2048]
- Thread count is allocated through `resolve_thread_num` based on actual elements processed per core: when elements per core is less than 2048, reduce thread count to eliminate idle threads

**Performance Data**:

| SCENARIO_NUM | Thread Block Count | Thread Count | Elements Per Core | Elements Per Thread | Task Duration(μs) |
| :----------: | :-------: | :-------: | :-------: | :-------: | :---------------: |
| 12 | 4 | 2048 | 4096 | 2 | 9.989 |
| 13 | 8 | 2048 | 2048 | 1 | 8.755 |
| **14** | **16** | **1024** | **1024** | **1** | **7.832** |
| 15 | 32 | 512 | 512 | 1 | 7.959 |
| 16 | 64 | 256 | 256 | 1 | 8.820 |

Performance data is shown in the following figure:

<img src="figures/fig_case3_small_griddim.png" alt="Case 3 Task Duration vs Thread Block Count" width="600" />

**Analysis**:

- The curve shows a "first decrease then increase" valley shape, **optimal thread block count = 16 (1/4 of physical core count)**, not filling all 64 cores
- As thread block count increases from 4 to 8 to 16, Task Duration continuously decreases (9.989 → 8.755 → 7.832), with parallelism benefits continuously improving
- 16→32: Thread block count doubles, Task Duration increases from 7.832μs to 7.959μs, **duration increase of approximately 1.6%**. At this point, per-core workload is already small, and scheduling overhead from adding cores begins to approach parallelism benefits
- 32→64 (physical cores): Thread block count continues to increase, Task Duration increases from 7.959μs to 8.820μs, **duration increase of approximately 10.8%**. For small shapes, per-core task volume is small (256/512 elements), and scheduling overhead from increasing thread block count exceeds parallelism benefits
- Compared with Case 2: with the same kernel, when shape decreases to a certain point, the optimal thread block count drops from 64 to 16, indicating that "optimal thread block count" changes as shape decreases and must be measured

**Conclusion**:

For small shapes, the experience of "using all physical cores" cannot be directly applied. Multiple thread block count settings must be tested as demonstrated in this Case.

---

## Performance Comparison Summary

### Ascend 950PR Performance Data

**Scheduling overhead vs thread block count (Case 0)**:

| SCENARIO_NUM | Thread Block Count | Task Duration(μs) |
| ------------ | --------- | ----------------- |
| 1 | 1 | 3.933 |
| 2 | 32 | 4.413 |
| 3 | 64 | 4.602 |
| 4 | 128 | 5.269 |
| 5 | 1024 | 13.687 |

- Scheduling overhead grows as thread block count increases

**Impact of vf call count on header overhead (Case 1)**:

| Comparison | Thread Block Count | VF Count | Task Duration(μs) | Increase |
| ---- | --------- | ------ | ----------------- | ---- |
| SCENARIO_NUM=2→6 | 32 | 1→2 | 4.413 → 4.503 | +2.0% |
| SCENARIO_NUM=3→7 | 64 | 1→2 | 4.602 → 4.869 | +5.8% |
| SCENARIO_NUM=5→8 | 1024 | 1→2 | 13.687 → 14.982 | +9.5% |

- VF calls also have scheduling overhead

**Optimal thread block count for large shape (Case 2)**:

| SCENARIO_NUM | Thread Block Count | Elements Per Core | Task Duration(μs) |
| :----------: | :-------: | :-------: | :---------------: |
| 9 | 1024 | 2048 | 78.533 |
| **10** | **64** | **32768** | **62.649** |
| 11 | 32 | 65536 | 113.609 |

- Optimal thread block count is **64** (equal to physical core count); `thread block count=64` both avoids the large thread block scheduling overhead from `thread block count=1024` and avoids the insufficient parallelism from `thread block count=32`

**Optimal thread block count for small shape (Case 3)**:

| SCENARIO_NUM | Thread Block Count | Thread Count | Elements Per Core | Task Duration(μs) |
| :----------: | :-------: | :-------: | :-------: | :---------------: |
| 12 | 4 | 2048 | 4096 | 9.989 |
| 13 | 8 | 2048 | 2048 | 8.755 |
| **14** | **16** | **1024** | **1024** | **7.832** |
| 15 | 32 | 512 | 512 | 7.959 |
| 16 | 64 | 256 | 256 | 8.820 |

- Optimal thread block count is **16** (1/4 of physical core count), not the full 64 cores; 14→15→16 duration continuously increases

## Tuning Recommendations

1. **Large shapes need to match physical core count first**: Limited by hardware resources, the number of physical cores that can execute in parallel has an upper bound. Thread block count should be as close to physical core count as possible, avoiding too many thread blocks (queuing waits, accumulated scheduling overhead) or too few (insufficient parallelism, heavy per-block workload).
2. **Small shapes require measured thread block count performance**: For small shapes, per-core workload is small, and scheduling header overhead from launching more thread blocks exceeds benefits. The optimal thread block count may not be the maximum physical core count. Refer to Case 3 for tiered testing to find the optimal thread block count.

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
  SCENARIO_NUM=1                       # Select execution scenario, options 1-16
  mkdir -p build && cd build;          # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM ..;make -j;  # Build the project
  ./grid_config                        # Run the example
  ```

  Build option description

  | Option             | Values      | Description              |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this example only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `1`-`16` | Example type, default is 1 |

  The following execution result indicates that the accuracy comparison succeeded.

  ```text
  [Success] Case accuracy is verification passed.
  ```

## Performance Analysis

Use the `msprof` tool to obtain detailed performance data:

```bash
msprof ./grid_config   # Analyze performance
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
