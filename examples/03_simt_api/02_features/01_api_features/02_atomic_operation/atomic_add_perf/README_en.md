# asc_atomic_add Atomic-Add Performance Sample

## Overview

Taking the [asc_atomic_add()](https://gitcode.com/cann/asc-devkit/blob/master/docs/api/SIMT-API/原子操作/asc_atomic_add.md) atomic add as an example, this sample uses five groups of controlled experiments to show how the memory tier (Global Memory / Unified Buffer), the organization of the atomic operation (direct atomic add / intra-block accumulation followed by GM writeback), whether the return value is used, and the data type (int32 / int64) affect the performance of atomic add, and provides corresponding optimization guidance.

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

Each thread performs one `asc_atomic_add()`, so the total accumulated value equals the total thread count:

```text
TOTAL_ACCUMULATED_VALUE = BLOCK_NUM * THREADS_PER_BLOCK
```

**Scenario list**:

| Scenario | Case | Memory | Data Type | Return Value | Scale (Block×Thread) |
|:---:|:---:|:---:|:---:|:---:|:---:|
| 1 | Case 1 | Global Memory | int32_t | No | 1×1024 |
| 2 | Case 1 | Unified Buffer | int32_t | No | 1×1024 |
| 3 | Case 2 | Global Memory | int32_t | No | 8×1024 |
| 4 | Case 2 | Unified Buffer | int32_t | No | 8×1024 |
| 5 | Case 3 | Global Memory | int32_t | Yes | 1×2048 |
| 6 | Case 3 | Global Memory | int32_t | No | 1×2048 |
| 7 | Case 3 | Unified Buffer | int32_t | Yes | 1×2048 |
| 8 | Case 3 | Unified Buffer | int32_t | No | 1×2048 |
| 9 | Case 4 | Global Memory | int32_t | No | 48×256 |
| 10 | Case 4 | Global Memory | int64_t | No | 48×256 |
| 11 | Case 5 | Global Memory | int32_t | Yes | 48×256 |
| 12 | Case 5 | Global Memory | int64_t | Yes | 48×256 |

**Five controlled groups**:

| Dimension | Scenarios | Single variable | Case |
|---|---|---|:---:|
| Memory tier under single block | Scenario 1 ↔ Scenario 2 | Global Memory → Unified Buffer | Case 1 |
| Memory tier under multi-block | Scenario 3 ↔ Scenario 4 | Global Memory → Unified Buffer | Case 2 |
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

### Case 1: Memory-tier performance difference (single-block scenario)

**Goal**: Compare the performance of an atomic add landing on Global Memory versus Unified Buffer, when all threads contend for the same address.

**Core implementation**: A single thread block launches 1024 threads. In the GM scenario, the 1024 threads perform an atomic add on the same GM address; in the UB scenario, the 1024 threads perform an atomic add on the same UB address. Case 1 only compares the overhead of the interface accessing GM versus UB; the UB scenario does not perform correctness validation.

```cpp
// Scenario 1: GM single address, all threads contend for the same GM counter
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);  // 1024 threads access the same GM address
}

// Scenario 2: UB single address, all threads call the atomic-add interface on the same UB address
__global__ void atomic_add_ub_local_no_return()
{
    __ubuf__ int32_t local[1];
    asc_atomic_add(&local[0], 1);  // 1024 threads access the same UB address
}
```

**Performance data**:

| Scenario | Memory | Task Duration (μs) |
|:---:|:---:|---:|
| 1 | Global Memory | 87.60 |
| 2 | Unified Buffer | 1.84 |

**Analysis**:

- With the same number of contending threads, the GM scenario takes about 87.60 μs and the UB scenario about 1.84 μs, a difference of roughly**48×**. The atomic operation on UB is significantly faster than on Global Memory.

**Conclusion**: Atomic accumulations that can be done on UB should not be done on GM.

---

### Case 2: Memory-tier performance difference (multi-block scenario)

**Goal**: When multiple thread blocks contend for the same GM address simultaneously, compare two organizations: "all threads directly atomic-add to the same GM address" versus "first accumulate within each block's own UB, then have each block write back to GM".

**Core implementation**: Launch 8 thread blocks of 1024 threads each (8192 threads in total). In the GM scenario, the 8192 threads directly perform an atomic add on the same GM address; in the UB scenario, each block first accumulates on its own UB array `local[1]`, then each block performs one GM atomic-add writeback, so the number of GM atomic operations drops from 8192 to 8.

```cpp
// Scenario 3: GM single address, 8192 threads all contend for the same GM counter
__global__ void atomic_add_gm_single_no_return(int32_t* counter)
{
    asc_atomic_add(counter, 1);
}

// Scenario 4: Each block first accumulates in its own UB, then thread 0 writes back to GM
__global__ void atomic_add_ub_block_accum_no_return(int32_t* gm_result)
{
    __ubuf__ int32_t local[1];
    if (threadIdx.x == 0) {
        local[0] = 0;
    }
    asc_syncthreads();

    asc_atomic_add(&local[0], 1);  // 1024 threads access this block's UB address
    asc_syncthreads();

    if (threadIdx.x == 0) {
        asc_atomic_add(gm_result, local[0]);  // only 1 GM atomic add per block
    }
}
```

**Performance data**:

| Scenario | Organization | Task Duration (μs) |
|:---:|:---:|---:|
| 3 | GM direct atomic add | 351.47 |
| 4 | UB intra-block accumulation + GM writeback | 2.25 |

**Analysis**:

- GM is accessible by threads of all thread blocks. In scenario 3, 8192 threads across 8 thread blocks all directly perform an atomic add on the same GM address, taking about 351.47 μs. Compared to Case 1, Case 2 increases the number of threads doing direct GM atomic adds from 1024 to 8192, proportionally increasing the number of atomic operations landing on the same GM address, and the time rises significantly.
- UB is accessible only by threads within the current thread block; different thread blocks use their own UB. In scenario 4, each block first obtains an intra-block accumulation result in its own UB, and finally writes back to GM only once. This reduces the number of atomic operations landing on GM from 8192 to 8, taking about 2.25 μs.
- The UB intra-block accumulation + writeback approach retains only 8 GM atomic operations; the two methods differ by roughly**156×**.

**Conclusion**: When multiple thread blocks need to perform same-address atomic accumulation, it is not recommended to have every thread directly perform an atomic add on GM. It is preferable to first obtain an intra-block accumulation result in each block's UB, then accumulate that result onto GM.

---

### Case 3: Return-value overhead

**Goal**: Compare the performance of `asc_atomic_add()` with and without using the return value, on both GM and UB.

**Core implementation**: A single thread block launches 2048 threads, changing only "whether the return value is used". Both GM and UB are tested in with-return-value and without-return-value variants. The UB scenario only calls the atomic-add interface on a UB address, with no initialization, no GM writeback, and no result validation. When the return value is used, the old value is written to a thread-local `volatile` variable, preventing the return-value path from being optimized away by the compiler.

**Performance data**:

| Memory | With return value (μs) | Without return value (μs) | Ratio (With / Without) |
|:---:|---:|---:|:---:|
| GM int32 (Scenario 5 vs 6) | 421.59 | 159.88 | 2.64× |
| UB int32 (Scenario 7 vs 8) | 3.17 | 2.76 | 1.15× |

**Analysis**:

- **GM int32**: On GM, the hardware executes an optimized instruction when the return value is unused, making it about 2.6× faster than when the return value is used.
- **UB int32**: On UB, the difference between the optimized and unoptimized atomic instructions is small; with the return value is about 1.15× of without the return value.

**Conclusion**: Unless the business logic requires it, the return value of the API should not be used.

---

### Case 4: Data type (no return value)

**Goal**: Compare int32 and int64 atomic-add performance on GM, where multiple threads map via modulo onto a set of densely packed addresses (real same-address contention) and the return value is not used.

> [!NOTE]
> Since int64 atomic add only supports GM, the data-type comparison in this sample is conducted only in the GM scenario.

**Core implementation**: Launch 48 thread blocks of 256 threads each (12288 threads in total). Threads map to a set of densely packed addresses via `address = base + (global_thread_id % TARGET_COUNT)`, with `TARGET_COUNT=1024`, i.e. about 12 threads contend for the same address, producing same-address contention. The GM scenario uses int32 and int64 respectively, neither using the return value.

```cpp
// Multiple threads land on the same address via modulo, producing same-address contention
__global__ void atomic_add_gm_dense_i32_no_return(int32_t* counters, uint64_t target_count)
{
    // return value not used
    asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % target_count), 1);
}
```

**Performance data**:

| Data type | Task Duration (μs) |
|:---:|---:|
| int32 (Scenario 9) | 1.89 |
| int64 (Scenario 10) | 58.46 |

**Analysis**:

- int32 triggers a hardware instruction optimization when the return value is unused; even with about 12 threads contending for the same address, the time is as low as 1.89 μs.
- int64 does not have this optimization; under the same contention it takes as much as 58.46 μs.
- Under same-address contention, int32 is about**31×** faster than int64.

**Conclusion**: For pure counting scenarios that do not use the return value, prefer int32 over int64.

---

### Case 5: Data type (with return value)

**Goal**: On GM, using the same compact address layout as Case 4 but with the return value used, compare int32 and int64 atomic-add performance, illustrating that one should not choose the atomic-add data type based solely on the intuition that "narrower types are faster".

> [!NOTE]
> Since int64 atomic add only supports GM, the data-type comparison in this sample is conducted only in the GM scenario.

**Core implementation**: Uses the same thread scale and address mapping as Case 4 (48 thread blocks × 256 threads, `address = base + (global_thread_id % TARGET_COUNT)`, `TARGET_COUNT=1024`), but Case 5 uses the return value of `asc_atomic_add()`. The GM scenario compares int32 and int64 data types.

```cpp
// Same address mapping as Case 4, but using the asc_atomic_add() return value
__global__ void atomic_add_gm_dense_i32_return(int32_t* counters, uint64_t target_count)
{
    volatile int32_t old_value_sink =
        asc_atomic_add(counters + ((blockIdx.x * blockDim.x + threadIdx.x) % target_count), 1);
    (void)old_value_sink;
}
```

**Performance data**:

| Data type | Task Duration (μs) |
|:---:|---:|
| int32 (Scenario 11) | 110.95 |
| int64 (Scenario 12) | 54.28 |

**Analysis**:

- Counter-intuitive: the narrower type int32 is about**2×** slower than the wider type int64 (110.95 vs 54.28).
- When an atomic operation accesses a GM address, it passes through the L2 cache hierarchy and is processed by the atomic processing unit on the L2 side. The L2 cache does not handle individual int32 or int64 values in isolation; instead, it caches and accesses memory in units of a contiguous segment called a CacheLine. When the return value is needed, if multiple threads update the same address simultaneously, ordinary reads and writes can cause multiple threads to read the same old value and later writebacks to overwrite earlier ones, resulting in lost additions; atomic operations prevent this, so "read old value, compute new value, write back new value" must be completed as an indivisible whole; the L2 side queues related atomic operations by CacheLine. Therefore, when analyzing GM atomic-operation queuing, one cannot look only at individual variables but must also consider which operations fall on the same CacheLine.

  | Data type | Per-address size | Addresses per 128B CacheLine |
  |:---:|:---:|:---:|
  | int32 | 4B | 128B/4B = 32 |
  | int64 | 8B | 128B/8B = 16 |

- The root cause is that **atomic operations on the same CacheLine must queue**: the CacheLine size is 128B; even if threads modify independent, non-conflicting addresses, as long as those addresses belong to the same CacheLine, they share the same queuing channel. With the same total number of atomic adds, 1024 int32 addresses occupy only 32 CacheLines, while 1024 int64 addresses occupy 64. Each CacheLine queues about 2× as many atomic operations for int32 as for int64, and the time roughly doubles accordingly.
- This phenomenon is unrelated to the int32 type itself; it stems solely from each CacheLine holding more addresses under a dense layout.

The following diagram illustrates the queuing of atomic operations on the same CacheLine:

<img src="figures/case5.png" alt="local_his" style="width: 70%; height: auto;">

**Conclusion**: Do not choose int32 for densely packed GM addresses based on the intuition that "narrower is faster". Even when the addresses each thread accesses do not conflict, a narrower type packs more atomic-add addresses per CacheLine, so more atomic operations queue on each CacheLine and it ends up slower. For GM arrays that are frequently updated by atomic operations, padding or struct alignment can reduce the queuing overhead at the L2 CacheLine granularity.

---

## Performance Comparison Summary

**All-scenario Task Duration summary**:

| Scenario | Case | Memory | Data Type | Return Value | Scale | Task Duration (μs) |
|:---:|:---:|:---:|:---:|:---:|:---:|---:|
| 1 | Case 1 | GM | int32 | No | 1×1024 | 87.60 |
| 2 | Case 1 | UB | int32 | No | 1×1024 | 1.84 |
| 3 | Case 2 | GM | int32 | No | 8×1024 | 351.47 |
| 4 | Case 2 | UB | int32 | No | 8×1024 | 2.25 |
| 5 | Case 3 | GM | int32 | Yes | 1×2048 | 421.59 |
| 6 | Case 3 | GM | int32 | No | 1×2048 | 159.88 |
| 7 | Case 3 | UB | int32 | Yes | 1×2048 | 3.17 |
| 8 | Case 3 | UB | int32 | No | 1×2048 | 2.76 |
| 9 | Case 4 | GM | int32 | No | 48×256 | 1.89 |
| 10 | Case 4 | GM | int64 | No | 48×256 | 58.46 |
| 11 | Case 5 | GM | int32 | Yes | 48×256 | 110.95 |
| 12 | Case 5 | GM | int64 | Yes | 48×256 | 54.28 |

**Gains of the five controlled groups**:

| Optimization dimension | Key comparison | Gain | Recommendation |
|---|---|---|---|
| Memory tier | UB (1.84 μs) vs GM (87.60 μs) | ~48× | Temporary accumulations that can be done on UB should be performed on UB |
| Atomic organization | UB intra-block accumulation + GM writeback (2.25 μs) vs GM direct atomic add (351.47 μs) | ~156× | When multiple thread blocks accumulate to the same address, first accumulate in each block's UB, then write back a small number of times to GM |
| Return value | GM without return value (159.88 μs) vs with return value (421.59 μs) | ~2.6× | Do not use the return value unless the business logic requires it |
| Data type (optimized) | int32 (1.89 μs) vs int64 (58.46 μs) | ~31× | Prefer int32 when the return value is unused; its optimization is robust under same-address contention |
| Data type (unoptimized) | int32 (110.95 μs) vs int64 (54.28 μs) | ~0.5× | Under dense layout the narrower type queues more on each CacheLine; do not infer performance from type width alone |

## Tuning Advice

1. **Lower the memory tier of atomic operations first**: atomic accumulations that can be done in UB should not be done in GM.
2. **Use intra-block UB accumulation when multiple thread blocks accumulate to the same address**: first accumulate within each block's own UB, then have each block perform one GM atomic writeback, avoiding all threads queuing on GM and reducing the number of atomic operations landing on GM.
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
  | `SKIP_VALIDATION` | `ON`/`OFF` | Whether to skip result validation; default `OFF`. The UB-only scenarios of Case 1/3 have no GM result and automatically skip validation; recommended to set to `ON` when profiling GM-writeback scenarios with `msopprof` |

  The following output indicates that correctness validation passed.

  ```text
  Scenario 1: Case1 GM int32 single-address, no return value
  Workload: blocks=1 threads_per_block=1024 total_atomic_adds=1024
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
> **About Validation failed during profiling**: Apart from the UB-only scenarios of Case 1/3, the final accumulation result of each sample scenario is written back to a GM counter, which is zeroed only once when allocated on the host side; the kernel only accumulates and does not zero it on entry. `msopprof`'s warmup+replay executes the kernel repeatedly on the same GM memory, so the counter is accumulated multiple times. Under strict validation this reports `Validation failed` (the count is an integer multiple of the expected value).
>
> Although the UB intra-block accumulation scenario of Case 2 zeroes the intermediate UB accumulator on kernel entry (making it idempotent under replay), the block subtotal is still written back to the GM counter via `asc_atomic_add`. That GM counter is likewise accumulated multiple times under replay, so validation fails for it too.
>
> This is an inherent conflict between the replay mechanism and the validation logic. When profiling any scenario, it is recommended to skip validation with `-DSKIP_VALIDATION=ON`.
