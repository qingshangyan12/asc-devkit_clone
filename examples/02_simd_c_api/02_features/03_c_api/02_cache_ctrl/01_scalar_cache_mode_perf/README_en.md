## Scalar Cache Mode Performance Verification Sample (C_API)

## Overview

This sample verifies the functional effect of `asc_set_scalar_cache_mode` on scalar unit GM access, observed via `msopprof` L2Cache metrics.

Difference from [00_scalar_cache_mode](../00_scalar_cache_mode):
- `00_scalar_cache_mode`: Verifies set→get register round-trip consistency (interface contract)
- `01_scalar_cache_mode_perf`: Verifies actual cache effect of different cache modes on scalar GM access (functional effect)

## Supported Products

- Ascend 950PR/Ascend 950DT

## Directory Structure

```
├── 01_scalar_cache_mode_perf
│   ├── scripts
│   │   └── gen_data.py              // Input/golden data generation script
│   ├── CMakeLists.txt               // Build configuration
│   ├── scalar_cache_mode_perf.asc   // Sample entry (kernel invocation + main)
│   ├── scalar_cache_mode_perf.h     // Kernel implementation (4 cases)
│   ├── data_utils.h                 // File I/O utilities
│   ├── README.md                    // Documentation (Chinese)
│   └── README_en.md                 // Documentation (English)
```

## Cache Hierarchy and API Cooperation

```
Scalar Unit ──→ Data Cache (L1, per-core private) ──→ L2 Cache (shared) ──→ GM
                       │                           │
                  asc_dci() clears          asc_set_scalar_cache_mode() controls policy
                  asc_dcci_entire_out() flushes
```

| API | Role | Description |
|-----|------|-------------|
| `asc_set_scalar_cache_mode` | **Under test** | Sets L2 read/write channel cache policy |
| `__gm__` pointer dereference | **Trigger** | Scalar GM read/write through full cache path |
| `asc_dci()` | **Isolation** | Clears L1 Data Cache for clean test start |
| `asc_sync_data_barrier(DSB_ALL)` | **Prerequisite** | Must wait for all memory accesses before `asc_dci()` |
| `asc_dcci_entire_out()` | **Flush** | Flushes L1 dirty data to GM after scalar writes |
| `msopprof --aic-metrics=L2Cache` | **Observation** | Collects L2 read/write hit/miss counters |

## Sample Description

### Case 1: Read channel NORMAL_PERSISTENT (reused data)

- **Design intent**: With persistent mode, repeated reads of the same data should hit L2 on pass 2.
- **Working set**: 16384 floats (64KB), much larger than L1 Data Cache, much smaller than L2 Cache (~128MB).
- **L2 policy**: `asc_load_l2_cache_mode::NORMAL_PERSISTENT`

### Case 2: Read channel NOTALLOC_KEEP (streaming data)

- **Design intent**: With no L2 allocation, repeated reads should still miss L2 on pass 2.
- **L2 policy**: `asc_load_l2_cache_mode::NOTALLOC_KEEP`

### Case 3: Write channel NORMAL_FIRST_VICTIM

- **Design intent**: With L2 enabled, scalar GM writes should allocate L2 cache lines. Data correctness verified (dst[i] == float(i)).
- **L2 policy**: `asc_store_l2_cache_mode::NORMAL_FIRST_VICTIM`

### Case 4: Write channel NOTALLOC_CLEAN

- **Design intent**: With L2 bypass, scalar GM writes should not allocate L2 cache lines. Data correctness verified.
- **L2 policy**: `asc_store_l2_cache_mode::NOTALLOC_CLEAN`

## Performance Metrics

Use `msopprof --ai-core=on --aic-metrics=L2Cache ./scalar_cache_mode_perf` to collect:

| Field | Description |
|:---:|:---|
| Task Duration(μs) | Total task execution time |
| aiv_read_local_l2_hit | Read cache hit count |
| aiv_read_local_l2_miss | Read cache miss count |
| aiv_read_local_l2_victim | Read cache miss with eviction count |
| aiv_write_cache_hit | Write cache hit count |
| aiv_write_cache_miss_allocate | Write cache miss with allocation count |

### Expected Performance Comparison

**Read channel (Case 1 vs Case 2)**

| Case | Mode | aiv_read_local_l2_hit | aiv_read_local_l2_miss | Description |
|:---:|:---|:---:|:---:|:---|
| 1 | NORMAL_PERSISTENT | High | Low | Pass 2 hits resident data |
| 2 | NOTALLOC_KEEP | ≈0 | High | L2 not allocated, always miss |

**Write channel (Case 3 vs Case 4)**

| Case | Mode | aiv_write_cache_miss_allocate | Description |
|:---:|:---|:---:|:---|
| 3 | NORMAL_FIRST_VICTIM | Non-zero | Write allocates L2 cache lines |
| 4 | NOTALLOC_CLEAN | ≈0 | Write bypasses L2, no allocation |

## Build and Run

- Set environment variables

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

- Build and run (Case 1 example)

  ```bash
  SCENARIO_NUM=1
  mkdir -p build && cd build
  cmake -DSCENARIO_NUM=$SCENARIO_NUM -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..
  make -j
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM
  ./scalar_cache_mode_perf
  ```

- Performance profiling

  ```bash
  msopprof --ai-core=on --aic-metrics=L2Cache ./scalar_cache_mode_perf
  cat ./PROF_*/L2Cache.csv
  ```

- Build options

  | Parameter | Description | Options | Default |
  |------|------|--------|--------|
  | `SCENARIO_NUM` | Scenario number | 1-4 | 1 |
  | `CMAKE_ASC_ARCHITECTURES` | NPU architecture | `dav-3510` (only) | `dav-3510` |
