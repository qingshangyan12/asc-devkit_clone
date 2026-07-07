# asc_atomic_add Atomic-Add Performance Sample

## Overview

Taking the [asc_atomic_add()](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/原子操作/asc_atomic_add.md) atomic add as an example, this sample uses five groups of controlled experiments to show how the memory tier (Global Memory / Unified Buffer), the organization of the atomic operation (direct atomic add / two-level reduction), whether the return value is used, and the data type (int32 / int64) affect the performance of atomic add, and provides corresponding optimization guidance.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Versions

- \>= CANN 9.1.0

## Directory Structure

```text
├── atomic_add_perf
│   ├── figures                // Image resources used in the README
│   ├── CMakeLists.txt         // Sample build script
│   ├── atomic_add_perf.asc    // Sample source code
│   ├── README.md
│   └── README_en.md
```

## Sample Description

This sample selects one scenario to test via `SCENARIO_NUM`; each run launches only one target kernel. The 12 scenarios are grouped into 5 controlled experiments (Case 1~5). Each group changes only one attribute while keeping the others identical, so that the impact of that single attribute on atomic-add performance can be observed in isolation.

Each thread performs one `asc_atomic_add()`, so the total number of atomic adds equals the total thread count:

```text
TOTAL_ATOMIC_ADDS = BLOCK_NUM * THREADS_PER_BLOCK
```

**Scenario list**:

| Scenario | Case | Memory | Data Type | Return Value | Scale (Block×Thread) |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | Case 1 | Global Memory | int32_t | No | 1×2048 |
| 2 | Case 1 | Unified Buffer | int32_t | No | 1×2048 |
| 3 | Case 2 | Global Memory | int32_t | No | 8×256 |
| 4 | Case 2 | Unified Buffer | int32_t | No | 8×256 |
| 5 | Case 3 | Global Memory | int32_t | Yes | 1×2048 |
| 6 | Case 3 | Global Memory | int32_t | No | 1×2048 |
| 7 | Case 3 | Unified Buffer | int32_t | Yes | 1×2048 |
| 8 | Case 3 | Unified Buffer | int32_t | No | 1×2048 |
| 9 | Case 4 | Global Memory | int32_t | No | 48×256 |
| 10 | Case 4 | Global Memory | int64_t | No | 48×256 |
| 11 | Case 5 | Global Memory | int32_t | Yes | 48×256 |
| 12 | Case 5 | Global Memory | int64_t | Yes | 48×256 |

**Five controlled groups**:

| Dimension | Scenarios | Single variable | Section |
|---|---|---|:---:|
| Single-block contention | Scenario 1 ↔ Scenario 2 | Global Memory → Unified Buffer | Case 1 |
| Multi-block contention | Scenario 3 ↔ Scenario 4 | Global Memory → Unified Buffer | Case 2 |
| Return value usage | Scenario 5 ↔ Scenario 6 (GM)<br>Scenario 7 ↔ Scenario 8 (UB) | With return value → Without return value | Case 3 |
| Data type (no return value) | Scenario 9 ↔ Scenario 10 | int32_t → int64_t | Case 4 |
| Data type (with return value) | Scenario 11 ↔ Scenario 12 | int32_t → int64_t | Case 5 |

## Sample Implementation

This section first explains the performance metric, then analyzes the impact of memory tier, atomic organization, return value, and data type on atomic-add performance group by group (Case 1~5). Each group changes only one attribute while keeping the others identical, and provides the corresponding `msopprof` measured data and optimization advice.

### Performance Metric

| Metric | Description |
| --- | --- |
| Task Duration (μs) | Total task duration, including the time scheduled onto the accelerator, the execution time on the accelerator, and the response/completion time |

---

### Case 1: GM atomic add vs UB two-level reduction under single-block contention

**Goal**: Compare the performance of an atomic add landing on Global Memory versus Unified Buffer, when all threads contend for the same address.

**Core implementation**: A single thread block launches 2048 threads. In the GM scenario, the 2048 threads perform an atomic add on the same GM address; in the UB scenario, the 2048 threads first perform atomic adds on a single UB accumulator within the block, and after the intra-block accumulation completes, one thread writes the result back to GM.

```cpp
// Scenario 1: GM single address, all threads contend for the same GM counter
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);  // 2048 threads access the same address
}

// Scenario 2: UB two-level reduction, contention collapses onto intra-block UB
__global__ void atomic_add_ub_reduce_no_return(int32_t* gm_result)
{
    __ubuf__ int32_t local;
    if (threadIdx.x == 0) {
        local = 0;
    }
    asc_syncthreads();

    asc_atomic_add(&local, 1);   // contention happens on UB
    asc_syncthreads();

    if (threadIdx.x == 0) {
        asc_atomic_add(gm_result, local);  // only 1 GM atomic add per block
    }
}
```

**Performance data**:

| Scenario | Memory | Task Duration (μs) |
|:---:|:---:|---:|
| 1 | Global Memory | 144.36 |
| 2 | Unified Buffer | 3.13 |

**Analysis**:

- With the same number of contending threads, the GM scenario takes about 144 μs and the UB scenario about 3 μs, a difference of roughly **46×. Performing an atomic operation on Unified Buffer is far cheaper than on Global Memory.

**Conclusion**: Atomic accumulations that can be done on UB should not be done on GM.

---

### Case 2: GM direct atomic add vs UB two-level reduction under multi-block contention

**Goal**: Compare two organizations when multiple thread blocks contend for the same address simultaneously: "all threads directly atomic-add to the same GM address" versus "first reduce within each block's UB, then write back to GM".

**Core implementation**: Launch 8 thread blocks of 256 threads each (2048 threads in total). In the GM scenario, the 2048 threads directly perform an atomic add on the same GM address; in the UB scenario, each block first reduces on its own UB accumulator, then each block performs one GM atomic-add writeback, so the number of GM atomic operations drops from 2048 to 8. The two scenarios reuse the two kernels from Case 1, changing only the number of blocks and the number of threads per block.

**Performance data**:

| Scenario | Organization | Task Duration (μs) |
|:---:|:---:|---:|
| 3 | GM direct atomic add | 87.59 |
| 4 | UB two-level reduction | 1.47 |

**Analysis**:

- Under cross-block contention for the same GM address, all atomic operations queue up on GM, taking about 88 μs; switching to two-level reduction collapses the contention onto UB and leaves only a few GM atomic operations, taking about 1.5 μs, a difference of roughly **60×.
- Two-level reduction drastically cuts the number of atomic operations landing on GM (2048 → 8).

**Conclusion**: When multiple thread blocks accumulate to the same GM address, prefer the two-level reduction "intra-block UB reduction + one GM writeback per block", to avoid the queuing latency of all threads contending for the same GM address.

---

### Case 3: Return-value overhead

**Goal**: Compare the performance of `asc_atomic_add()` with and without using the return value.

**Core implementation**: A single thread block launches 2048 threads, reusing Case 1 and changing only "whether the return value is used". Both GM and UB are tested in with-return-value and without-return-value variants. When the return value is used, the old value is written to a thread-local `volatile` variable, preventing the return-value path from being optimized away by the compiler.

**Performance data**:

| Memory | With return value (μs) | Without return value (μs) | With / Without ratio |
|:---:|---:|---:|:---:|
| GM int32 (Scenario 5 vs 6) | 380.50 | 148.06 | 2.57× |
| UB int32 (Scenario 7 vs 8) | 3.41 | 3.11 | 1.10× |

**Analysis**:

- **GM int32**: On GM, the hardware applies an instruction-level optimization when the return value is unused, making it about 2.6× faster than when the return value is used.
- **UB int32**: On UB, the difference between the optimized and unoptimized atomic instructions is small; without the return value is only 1.1× of with the return value.

**Conclusion**: Unless the business logic requires it, the return value of the API should not be used.

---

### Case 4: Data type (no return value)

**Goal**: Compare int32 and int64 atomic-add performance when multiple threads map via modulo onto a set of densely packed addresses (real same-address contention) and the return value is not used.

**Core implementation**: Launch 48 thread blocks of 256 threads each (12288 threads in total). Threads map to a set of densely packed addresses via `address = base + (global_thread_id % TARGET_COUNT)`, with `TARGET_COUNT=1024`, i.e. about 12 threads contend for the same address, producing same-address contention. The GM scenario uses int32 and int64 respectively, neither using the return value.

```cpp
// Multiple threads land on the same address via modulo, producing same-address contention
__global__ void atomic_add_gm_dense_i32_no_return(int32_t* counters)
{
    // return value not used
    asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % TARGET_COUNT), 1);
}
```

**Performance data**:

| Data type | Task Duration (μs) |
|:---:|---:|
| int32 (Scenario 9) | 1.89 |
| int64 (Scenario 10) | 58.46 |

**Analysis**:

- int32 triggers a hardware optimization when the return value is unused; even with about 12 threads contending for the same address, the time is as low as 1.89 μs.
- int64 does not have this optimization; under the same contention it takes as much as 58.46 μs.
- Under same-address contention, int32 is about **31× faster than int64.

**Conclusion**: For pure counting scenarios that do not need to read back the old value, prefer int32 over int64.

---

### Case 5: Data type (with return value)

**Goal**: Using the same compact address layout as Case 4 but with the return value used, illustrate that atomic operations on the same CacheLine incur queuing overhead.

**Core implementation**: The same mapping layout as Case 4 (48 thread blocks × 256 threads, `address = base + (global_thread_id % TARGET_COUNT)`, `TARGET_COUNT=1024`), but the return value is used. The GM scenario compares int32 and int64 data types.

The following diagram illustrates the queuing of atomic operations on the same CacheLine:

<img src="figures/case5.png" alt="local_his" style="width: 70%; height: auto;">

**Performance data**:

| Data type | Task Duration (μs) | Addresses per 128B CacheLine |
|:---:|---:|:---:|
| int32 (Scenario 11) | 110.95 | 128B/4B = 32 |
| int64 (Scenario 12) | 54.28 | 128B/8B = 16 |

**Analysis**:

- Counter-intuitive: the narrower type int32 is about **2× slower** than the wider type int64 (110.95 vs 54.28).
- The root cause is that **atomic operations on the same CacheLine must queue**: a GM atomic operation serializes at the granularity of a **128B CacheLine**; atomic operations landing on the same CacheLine must execute one after another, even if they modify independent, non-conflicting addresses. With the same total number of atomic adds, 1024 int32 addresses occupy only 32 CacheLines, while 1024 int64 addresses occupy 64. Each CacheLine queues about 2× as many atomic operations for int32 as for int64, and the time roughly doubles accordingly.
- This phenomenon is unrelated to the int32 type itself; it stems solely from each CacheLine holding more addresses under a dense layout.

**Conclusion**: Do not choose int32 for densely packed GM addresses based on the intuition that "narrower is faster". Even when the addresses each thread accesses do not conflict, a narrower type packs more atomic-add addresses per CacheLine, so more atomic operations queue on each CacheLine and it ends up slower. You can add padding to the data so that each CacheLine holds fewer atomic-add addresses, reducing the queuing overhead.

---

## Performance Comparison Summary

**All-scenario Task Duration summary**:

| Scenario | Case | Memory | Data Type | Return Value | Scale | Task Duration (μs) |
|:---:|:---:|:---:|:---:|:---:|:---:|---:|
| 1 | Case 1 | GM | int32 | No | 1×2048 | 144.36 |
| 2 | Case 1 | UB | int32 | No | 1×2048 | 3.13 |
| 3 | Case 2 | GM | int32 | No | 8×256 | 87.59 |
| 4 | Case 2 | UB | int32 | No | 8×256 | 1.47 |
| 5 | Case 3 | GM | int32 | Yes | 1×2048 | 380.50 |
| 6 | Case 3 | GM | int32 | No | 1×2048 | 148.06 |
| 7 | Case 3 | UB | int32 | Yes | 1×2048 | 3.41 |
| 8 | Case 3 | UB | int32 | No | 1×2048 | 3.11 |
| 9 | Case 4 | GM | int32 | No | 48×256 | 1.89 |
| 10 | Case 4 | GM | int64 | No | 48×256 | 58.46 |
| 11 | Case 5 | GM | int32 | Yes | 48×256 | 110.95 |
| 12 | Case 5 | GM | int64 | Yes | 48×256 | 54.28 |

**Gains of the five controlled groups**:

| Dimension | Key comparison | Gain | Recommendation |
|---|---|---|---|
| Memory tier | UB (3.13 μs) vs GM (144.36 μs) | ~46× | Perform atomic accumulation on UB |
| Atomic organization | UB two-level reduction (1.47 μs) vs GM direct atomic add (87.59 μs) | ~60× | Use two-level reduction when multiple blocks accumulate to the same address |
| Return value | GM without return value (148.06 μs) vs with return value (380.50 μs) | ~2.6× | Do not use the return value unless the business logic requires it |
| Data type (optimized) | int32 (1.89 μs) vs int64 (58.46 μs) | ~31× | Prefer int32 when the return value is unused; its optimization is robust under same-address contention |
| Data type (unoptimized) | int32 (110.95 μs) vs int64 (54.28 μs) | ~0.5× | Under dense layout the narrower type queues more on each CacheLine; do not infer performance from type width alone |

## Tuning Advice

1. **Lower the memory tier of atomic operations first**: atomic accumulations that can be done in UB should not be done in GM.
2. **Use two-level reduction when multiple thread blocks accumulate to the same address**: first reduce within each block's UB, then have each block perform one GM atomic writeback, avoiding all threads queuing on GM and reducing the number of atomic operations landing on GM.
3. **Do not use the return value for pure counting**: unless the business logic requires it, the return value of the API should not be used; and prefer int32 over int64, because int32 triggers a hardware optimization when the return value is unused, while int64 does not.
4. **Do not infer atomic performance from data-type width**: performance depends on the number of addresses per CacheLine, not on the type itself. Even when the addresses each thread accesses do not conflict, a narrower type packs more atomic-add addresses per CacheLine under a dense layout, so more atomic operations queue on each CacheLine and it ends up slower. In high-contention or dense-layout scenarios, add padding to the data so that each CacheLine holds only a few atomic-add addresses.

## Build and Run

Execute the following steps in the sample root directory to build and run the sample.

- Configure environment variables

  Configure the environment variables according to the [installation method](../../../../../../docs/quick_start.md#prepare&install) of the CANN development kit on your current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. When no installation directory is specified, it is installed under `/usr/local/Ascend` by default.

- Run the sample

  Build and run scenario 1:

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=1 ..
  make -j
  ./atomic_add_perf
  ```

  Build options:

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this sample supports only dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SCENARIO_NUM` | `1`-`12` | Scenario number to test. The scale (Block×Thread), data type, and whether the return value is used for each scenario are configured centrally by `get_scenario_config()` in the source |
  | `TARGET_COUNT` | Positive integer | Effective only for Case 4/5 (scenarios 9~12): the size of the address array; threads map to addresses via `global_thread_id % TARGET_COUNT`. When left empty, the default `1024` is used (about 12 threads contend for one address); decrease it to intensify contention, increase it to relax contention |
  | `SKIP_VALIDATION` | `ON`/`OFF` | Whether to skip result validation; default `OFF`. Recommended to set to `ON` when profiling GM scenarios with `msopprof` |

  The following output indicates that correctness validation passed.

  ```text
  Scenario 1: Case1 GM int32 single-address, no return value
  Workload: blocks=1 threads_per_block=2048 total_atomic_adds=2048
  Validation passed
  ```

## Performance Analysis

Use the `msopprof` tool to obtain detailed performance data:

```bash
msprof op ./atomic_add_perf
```

After the command completes, a folder named `OPPROF_{timestamp}_XXX` is generated in the default directory. An example of the performance-data folder structure is as follows:

```text
├──dump                       # Raw performance data; users can ignore this
├──ArithmeticUtilization.csv  # cube/vector instruction cycle share
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1, and main memory read/write bandwidth
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth
├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth
├──OpBasicInfo.csv            # Operator basic information
├──PipeUtilization.csv        # Compute and move (DMA) unit time and share
├──ResourceConflictRatio.csv  # UB bank group, bank conflict, and resource conflict ratio across all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```

> [!NOTE]
> **About Validation failed during profiling**: In all scenarios of this sample, the final accumulation result is written back to a GM counter, which is zeroed only once when allocated on the host side; the kernel only accumulates and does not zero it on entry. `msopprof`'s warmup+replay executes the kernel repeatedly on the same GM memory, so the counter is accumulated multiple times. Under strict validation this reports `Validation failed` (the count is an integer multiple of the expected value).
>
> In the UB two-level-reduction scenario, although the intermediate UB accumulator is zeroed on kernel entry and is therefore idempotent under replay, the block subtotal is still written back to the GM counter via `asc_atomic_add`. That GM counter is likewise accumulated multiple times under replay, so validation fails for it too.
>
> This is an inherent conflict between the replay mechanism and the validation logic. Although the log prints `[ERROR] Get profiling data failed.`, the profiling data (the CSVs and metrics such as `Task Duration`) still lands on disk normally and that message can be ignored. When profiling any scenario, it is recommended to skip validation with `-DSKIP_VALIDATION=ON`.
