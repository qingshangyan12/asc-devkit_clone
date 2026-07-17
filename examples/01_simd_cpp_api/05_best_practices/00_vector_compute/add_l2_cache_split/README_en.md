# L2 Cache Tiling Optimization

## Overview

This sample demonstrates the performance impact of the **L2 Cache tiling** strategy using vector addition as an example. By comparing two implementations — **without tiling (negative example)** and **with tiling (positive example)** — it shows how L2 Cache tiling improves data transfer efficiency when data volume exceeds the L2 Cache capacity (assuming the AI processor's L2 Cache size is 192 MiB).

**Key differences**:
- **Negative example (nosplit)**: Data is evenly partitioned by core count. Each core processes a fixed-size data block. When the total data exceeds L2 Cache size, the L2Cache hit rate is low (the Cache only retains the most recently accessed data; previously accessed data is evicted), causing subsequent accesses to reload from GM, leading to cache misses and becoming the performance bottleneck.

**Figure 1** Without L2 Cache Tiling (Negative Example)
![](../../../../../docs/zh/guide/figures/未使能L2-Cache切分.png "Without L2-Cache Tiling")

- **Positive example (split)**: Global tiling is performed based on L2 Cache capacity. The total data is split into multiple tiles that each fit within L2 Cache. All cores collaboratively process the first tile, then move on to the next. Data reuse can directly read from L2 Cache, improving bandwidth utilization.

**Figure 2** With L2 Cache Tiling (Positive Example)
![](../../../../../docs/zh/guide/figures/使能L2-Cache切分.png "With L2-Cache Tiling")

## Supported Products and CANN Versions

| Product | CANN Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series/Atlas A3 Inference Series | >= CANN 9.0.0 |
| Atlas A2 Training Series/Atlas A2 Inference Series | >= CANN 9.0.0 |

## Directory Structure

```
├── add_l2_cache_split
│   ├── CMakeLists.txt               // Build configuration
│   ├── data_utils.h                 // Data read/write utilities
│   ├── l2_cache_optimizer.h         // L2 Cache tiling optimizer
│   ├── add_l2cache_split.asc        // Positive example: with L2 Cache tiling
│   ├── add_l2cache_nosplit.asc      // Negative example: without L2 Cache tiling
│   ├── scripts
│   │   ├── gen_data.py              // Generate test data
│   │   └── verify_result.py         // Verify computation results
│   ├── README.md                    // Sample documentation (Chinese)
│   └── README_en.md                 // Sample documentation (English)
```

## Sample Description

**Functionality**:

  This sample implements vector addition of `TOTAL_LEN = 40000000` half-precision elements.

  The Add formula is:

$$
  z = x + y
$$

  - x: input, length 40000000, data type half (40000000 × 2B = 80000000B, i.e., 76.29 MiB);
  - y: input, length 40000000, data type half (40000000 × 2B = 80000000B, i.e., 76.29 MiB);
  - z: output, length 40000000, data type half (40000000 × 2B = 80000000B, i.e., 76.29 MiB);

  Total data size is 40000000 × 2B × 3 = 240000000B (i.e., 228.88 MiB), which exceeds the Atlas A2 series L2 Cache capacity (192 MiB) — a typical scenario that triggers L2 Cache tiling.

## Core Implementation Comparison

### Data Partitioning Strategy

| Aspect | Negative (nosplit) | Positive (split) |
|:---|:---|:---|
| Partitioning | Even partition by core count | Global tiling by L2Cache capacity |
| Data view | Each core independently processes a fixed segment | All 20 cores collaboratively process the same tile, batch by batch |
| L2Cache awareness | Not aware of L2Cache capacity | Aware via `L2CacheOptimizer` |
| Core utilization | All 20 cores active | All 20 cores active (participate in every tile) |
| L2Cache hit | All cores access non-overlapping data simultaneously; total exceeds L2Cache → **Cache miss** | 100 MiB per tile (controlled by L2_TILE_THRESHOLD=100×1024²) fits in L2Cache; next-round access hits → **Cache hit** |
| Instantaneous L2Cache working set | 240000000B/228.88 MiB (20 cores × 40000000/20 × 3 × 2B) | up to 100 MiB (controlled by L2_TILE_THRESHOLD=100×1024², total data per tile) |

### Negative Example Key Code (add_l2cache_nosplit.asc)

Data is evenly partitioned by core count. Each core independently processes its fixed-size block. Within each core, data is further split into `TILE_NUM=2` sub-tiles processed alternately. The outer `LOOP_COUNT=2` repeats processing of the same data. When returning to the first tile, L2Cache has already been replaced by later tile data, causing a cache miss:

```cpp
int64_t blockLen = Ceil(totalLen_, blockNum);
int64_t blockOffset = curBlockIdx * blockLen;
int64_t tileLen = Ceil(curBlockLen, TILE_NUM);

for (int32_t i = 0; i < LOOP_COUNT; i++) {
    for (int32_t j = 0; j < TILE_NUM; j++) {
        // Global offset of the j-th tile
        int64_t tileOffset = blockOffset + j * tileLen;
        // Chunked transfer by MAX_DATA_COPY_LEN + pipelined compute
        for (int64_t k = 0; k < totalBlocks; k++) {
            DataCopy x, DataCopy y, Add, DataCopy z
        }
        PipeBarrier<PIPE_ALL>();  // Sync between tiles
    }
}
```

### Positive Example Key Code (add_l2cache_split.asc)

`L2CacheOptimizer` splits the global data into multiple tiles based on the L2Cache threshold. **All 20 cores collaboratively process the same tile**, with per-core data evenly partitioned within each tile. After processing, `PipeBarrier` synchronizes all cores before entering the next tile:

```cpp
L2CacheOptimizer l2Opt(totalLen_, blockNum);

for (int64_t tileIdx = 0; tileIdx < l2Opt.GetTileNum(); tileIdx++) {
    int64_t tileLen = l2Opt.GetBlockLen(tileIdx);
    int64_t tileOffset = l2Opt.GetBlockOffset(tileIdx);

    // Within this tile, evenly split data among cores
    int64_t perCoreLen = Ceil(tileLen, blockNum);
    int64_t coreStart = tileOffset + curBlockIdx * perCoreLen;
    int64_t coreLen = min(perCoreLen, tileLen - curBlockIdx * perCoreLen);

    // Chunked transfer by MAX_DATA_COPY_LEN + pipelined compute
    for (int64_t i = 0; i < totalBlocks; i++) {
        DataCopy x, DataCopy y, Add, DataCopy z
    }

    PipeBarrier<PIPE_ALL>();  // All cores sync before next tile
}
```

### L2CacheOptimizer Splitting Logic

> Note: `L2_TILE_THRESHOLD = 100 × 1024² = 100 MiB` is an example threshold. Different hardware has different L2Cache sizes (e.g., Atlas A2 is 192 MiB, Ascend 950 series may be larger). Adjust this value based on your target hardware. It is recommended to set the threshold slightly below the actual L2Cache capacity to leave headroom for other data.

```cpp
constexpr int64_t L2_TILE_THRESHOLD = 100 * 1024 * 1024; // 100 MiB threshold, adjust for actual L2Cache size on target hardware

L2CacheOptimizer(int64_t totalLen, int64_t blockNum) {
    int64_t totalByteSize = totalLen * sizeof(half) * 3;
    if (totalByteSize <= L2_TILE_THRESHOLD) {
        // Data fits in L2Cache, no tiling needed
        tileNum_ = blockNum_;
    } else {
        // Split into multiple tiles based on L2Cache threshold
        baseTileLen_ = L2_TILE_THRESHOLD / (3 * sizeof(half));
        tileNum_ = Ceil(totalLen_, baseTileLen_);
    }
}
```

## Build and Run

- Configure environment variables

  Configure the environment variables according to the installation method of the CANN development kit.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Build

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..
  make -j
  ```

  Two executables will be generated:
  - `demo_split`: Positive example, with L2 Cache tiling
  - `demo_nosplit`: Negative example, without L2 Cache tiling

  Build options:

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | NPU architecture: dav-2201 for Atlas A2/A3 series, dav-3510 for Ascend 950 series |

- Generate test data and run

  ```bash
  python3 ../scripts/gen_data.py    # Generate input test data
  ./demo_split                      # Run positive example
  ./demo_nosplit                    # Run negative example
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin  # Verify accuracy
  ```

## Performance Profiling and Analysis

### Profiling with msopprof

```bash
# Profile positive example
msopprof --output=./prof_split ./demo_split

# Profile negative example
msopprof --output=./prof_nosplit ./demo_nosplit
```

After profiling, an `OPPROF_*` directory is generated with the following structure:

```
OPPROF_<timestamp>/
├── OpBasicInfo.csv              // Operator basic info and Task Duration
├── PipeUtilization.csv          // Per-unit time, ratio, effective bandwidth (per core)
├── L2Cache.csv                  // L2Cache read/write hit rate (per core)
├── MemoryL0.csv                 // L0A/L0B/L0C read/write bandwidth
├── Memory.csv                   // UB, L1, and main memory r/w bandwidth
├── MemoryUB.csv                 // Vector and scalar to UB r/w bandwidth
├── ArithmeticUtilization.csv    // Cube/vector instruction cycle ratio
└── ResourceConflictRatio.csv    // UB bank group / bank conflict ratio
```

### Performance Metrics

The performance data below was collected on the Atlas A2 Training Series and Ascend 950 Series with CANN 9.1.0. Key metrics are defined as follows:

| Field | Meaning | Source File |
|:---:|:---|:---|
| Task Duration(μs) | Total task time, including scheduling and accelerator execution | OpBasicInfo.csv |
| aiv_time(μs) | Task execution time on AI Vector Core | PipeUtilization.csv |
| aiv_mte2_time(μs) | mte2 instruction time (GM→UB data transfer), reflecting read efficiency | PipeUtilization.csv |
| aiv_mte2_ratio | mte2 instruction cycle ratio in aiv_total_cycles | PipeUtilization.csv |
| aiv_mte3_time(μs) | mte3 instruction time (UB→GM data transfer), reflecting write efficiency | PipeUtilization.csv |
| aiv_mte2_active_bw(GB/s) | mte2 effective r/w bandwidth = read data volume / mte2 time. Higher bandwidth indicates better GM access efficiency | PipeUtilization.csv |
| aiv_vec_time(μs) | vec instruction time (vector computation) | PipeUtilization.csv |
| aiv_write_hit_rate(%) | L2Cache write hit rate | L2Cache.csv |
| aiv_read_hit_rate(%) | L2Cache read hit rate | L2Cache.csv |
| aiv_gm_to_ub_bw(GB/s) | GM→UB average effective bandwidth (mte2 transfer efficiency metric on 3510) | Memory.csv |
| GM_to_UB_datas(KB) | GM→UB data volume per core | Memory.csv |

### Measured Performance Data (Atlas A2 Training Series)

The following data is aggregated from `PipeUtilization.csv` (20 cores per-core) and `L2Cache.csv`.

**Table 1 Overall Performance Summary (OpBasicInfo + PipeUtilization)**

| Metric | Negative (nosplit) | Positive (split) | Speedup |
|:---|---:|---:|---:|
| Task Duration(μs) | 415.1 | **224.6** | **1.85x** |
| aiv_time(μs) | 402.9 | 215.2 | 1.87x |
| aiv_mte2_time(μs) | 353.9 (87.9%) | 187.4 (87.1%) | **1.89x** |
| aiv_mte3_time(μs) | 111.1 (27.6%) | 53.0 (24.6%) | **2.10x** |
| aiv_vec_time(μs) | 41.2 (10.2%) | 20.6 (9.6%) | 2.00x |

**Table 2 L2Cache Hit Rate Comparison (L2Cache Per-Core Average)**

| Metric | Negative (nosplit) | Positive (split) |
|:---|---:|---:|
| aiv_write_hit_rate | 1.6% | **2.6%** |
| aiv_read_hit_rate | 0.60% | **0.72%** |
| aiv_total_hit_rate | 0.71% | **0.94%** |
| Instantaneous L2Cache working set | 240000000B/228.88 MiB (20 cores × 40000000/20 × 3 × 2B) | up to 100 MiB (controlled by L2_TILE_THRESHOLD=100×1024², total data per tile) |

**Table 3 MTE2 Effective Bandwidth Per-Core Comparison (PipeUtilization blk0)**

| Metric | Negative (nosplit) | Positive (split) |
|:---|---:|---:|
| aiv_mte2_active_bw | 41.3 GB/s | **57.9 GB/s** |
| aiv_mte2_time (blk0) | 361.2 μs | 128.8 μs |
| aiv_mte2_ratio (blk0) | 89.0% | 84.8% |

### Measured Performance Data (Ascend 950 Series)

The following data was collected on the Ascend 950 Series (dav-3510, 28 cores) with CANN 9.1.0, aggregated from `msopprof` profiling results.

> Note: On the 3510 platform, `PipeUtilization.csv` does **not** directly provide `aiv_mte2_time` and `aiv_mte3_time` fields. mte2/mte3 metrics should be obtained from `Memory.csv` via `aiv_gm_to_ub_bw` (GM→UB effective bandwidth) and `aiv_mte2_ratio`.

**Table 1 Overall Performance Summary**

| Metric | Negative (nosplit) | Positive (split) | Speedup | Source |
|:---|---:|---:|---:|:---|
| Task Duration(μs) | 309.1 | **149.3** | **2.07x** | OpBasicInfo.csv |
| aiv_time(μs) | 300.0 | 145.7 | 2.06x | PipeUtilization.csv |
| aiv_vec_time(μs) | 29.8 | 14.9 | 2.00x | PipeUtilization.csv |
| aiv_gm_to_ub_bw(GB/s) | 35.6 | 36.6 | +3% | Memory.csv |
| aiv_mte2_ratio | 88.3% | 87.8% | — | Memory.csv |
| GM_to_UB_datas(KB/core) | 11193 | 5597 | — | Memory.csv |

**Table 2 L2Cache Hit Rate Comparison (Per-Core Average)**

| Metric | Negative (nosplit) | Positive (split) | Source |
|:---|---:|---:|:---|
| aiv_write_hit_rate | 32.29% | 20.04% | L2Cache.csv |
| aiv_read_hit_rate | ~0% | ~0% | L2Cache.csv |

**Table 3 Compute Unit Utilization (Per-Core Average)**

| Metric | Negative (nosplit) | Positive (split) | Source |
|:---|---:|---:|:---|
| aiv_vec_ratio | 10.1% | 10.5% | ArithmeticUtilization.csv |

**3510 Platform Performance Analysis**

**1. Data Transfer Volume and Speedup Source**

In the negative example, each core reads 11193 KB (x+y combined), while the positive example reads 5597 KB — exactly half. The reason: `LOOP_COUNT=2` in the negative example causes the same data to be read twice, whereas the positive example processes only one round per core. `aiv_gm_to_ub_bw` (effective bandwidth) is nearly identical for both (~36 GB/s), indicating bandwidth is not the bottleneck — the speedup comes purely from **halving the transferred data volume**.

**2. mte2 Remains Dominant**

mte2 instruction ratio on the 3510 still dominates (negative 88.3%, positive 87.8%), consistent with the A2 platform. Add is an I/O-bound operation; vector computation accounts for only ~10%.

**3. L2Cache Hit Rate Interpretation**

On the 3510, `L2Cache.csv` has a different format than A2, distinguishing close/far hits. `read_hit_rate` is ~0% because Add's streaming access reads each address only once with no reuse. `write_hit_rate` (negative 32.29% > positive 20.04%) appears counterintuitive — the negative example has a higher write hit rate yet worse performance. This is because `LOOP_COUNT=2` writes the same addresses repeatedly, and the profiler counts these as "hits" even though they still trigger write-allocation and do not actually save bandwidth.

**4. Cross-Platform Comparison (A2 vs 3510)**

| Platform | Cores | Task Duration Speedup | gm_to_ub_bw Difference |
|------|------|-------------------|-------------------|
| Atlas A2 (2201) | 20 | 1.85x | +40% (41.3→57.9 GB/s) |
| Ascend 950 (3510) | 28 | 2.07x | +3% (35.6→36.6 GB/s) |

On A2, L2Cache tiling simultaneously improves effective bandwidth (indicating data can hit the L2Cache path), while on 3510, bandwidth is virtually unchanged — the speedup stems entirely from reducing transferred data volume rather than bandwidth improvement. The acceleration paths differ across platforms, but the conclusion is consistent: L2Cache tiling effectively reduces unnecessary data transfer.

### Performance Analysis

**1. MTE2 transfer efficiency is the key differentiator**

In the negative example, 20 cores each process independent data. At any moment, the L2Cache working set is 240000000B (228.88 MiB), exceeding the 192 MiB capacity, causing frequent cache misses requiring GM re-reads. The positive example splits data into 3 tiles based on L2Cache capacity, each tile up to 100 MiB (total size of x, y, z three tensor data per tile = L2_TILE_THRESHOLD), fitting entirely in L2Cache:

- Positive example L2Cache hit rate (0.94%) is 32% higher than the negative example (0.71%)
- MTE2 effective bandwidth improves from 41.3 GB/s to 57.9 GB/s (+40%)
- MTE2 absolute time drops from 353.9 μs to 187.4 μs (-47%)

**2. Positive Example Three-Phase Per-Tile Breakdown (PipeUtilization per-core)**

| Tile | Data Range | Per-Core Data | Per-Core mte2 Time | Per-Core aiv_time |
|:---|---:|---:|---:|---:|
| Tile 0 | [0, 17.5M) | 0.87M half | ~129 μs | ~152 μs |
| Tile 1 | [17.5M, 34.9M) | 0.87M half | ~190 μs | ~215 μs |
| Tile 2 | [34.9M, 40M) | 0.25M half | ~139 μs | ~163 μs |

The three tiles are processed sequentially with `PipeBarrier<PIPE_ALL>()` synchronization between them. All 20 cores participate in each tile. Tile 2 has a smaller data volume, but its per-core aiv_time is still ~163 μs due to synchronization overhead. Total aiv_time is the cumulative time of the slowest core (~215 μs).

**3. Negative Example L2Cache Thrashing Mechanism**

In the negative example, `LOOP_COUNT=2` repeats processing of the same data twice (simulating a data reuse scenario in training). Within each iteration, `TILE_NUM=2` alternately processes two data segments:

```
Round 1: [Tile0 → Tile1] → L2Cache finally holds Tile1
Round 2: [Tile0 → Tile1] → Returning to Tile0, L2Cache holds Tile1 → Cache miss
```

`PipeBarrier` ensures correct data consistency across tile boundaries, but Tile0 data in round 2 must be reloaded from GM. All 20 cores are simultaneously busy, causing L2Cache thrashing.

**4. Theoretical Speedup Source**

L2Cache r/w mixed bandwidth is approximately 7 TB/s, while GM bandwidth is approximately 1.6 TB/s — a ~4.4x difference. Reasons the measured 1.85x speedup is lower than the theoretical value:

- The positive example incurs 3 × `PipeBarrier<PIPE_ALL>` synchronization overhead
- L2Cache hit rate, although improved (0.94% vs 0.71%), remains low in absolute terms — this is inherent to the streaming access pattern of Add operations where data is read only once; cache hits only occur during recursive/repeated access
- The MTE2 engine's own bandwidth ceiling is far below L2Cache theoretical bandwidth

## Principle Illustration

**Figure 1** Without L2 Cache Tiling (Negative Example)

Data is partitioned per core, with each core transferring data via GM→L2Cache→UB. When the outer loop returns to the initial data, L2Cache holds the tail data and must reload from GM:

```
Core 0: [Tile0→Tile1→Tile0→Tile1]  ← On second access to Tile0, L2 already holds Tile1
Core 1: [Tile0→Tile1→Tile0→Tile1]
 ...

Timeline: Cores process independently → L2Cache frequent eviction → inefficient bandwidth
```

**Figure 2** With L2 Cache Tiling (Positive Example)

Data is first tiled globally by L2Cache capacity. All cores collaboratively process the same tile, then move to the next:

```
Tile0: Core0|Core1|...|CoreN  → L2Cache fully holds Tile0 data
Tile1: Core0|Core1|...|CoreN  → L2Cache fully holds Tile1 data

Timeline: All cores synchronized → L2Cache efficiently utilized → improved bandwidth
```
