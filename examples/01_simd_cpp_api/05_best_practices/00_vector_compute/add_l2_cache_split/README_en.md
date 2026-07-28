# L2 Cache Splitting Optimization Sample

## Overview

This sample compares two implementations with identical work and different L2 Cache-level loop orders:

- Negative round-first: `Round -> L2 Batch -> UB Tile`
- Positive L2-batch-first: `L2 Batch -> Round -> UB Tile`

The sample uses one 384MiB GM data buffer. Each round computes `data = data + 1` in place, so the result after two rounds is `input + 2`. Both implementations issue the same GM-to-UB copies, UB-to-GM copies, Adds operations, and inter-core synchronizations, so their performance difference comes only from traversal order.

## Supported Products

**Table 1**  Supported products

| Product | Architecture | CANN version |
| --- | --- | --- |
| Atlas A2 training and inference series | dav-2201 | >= 9.0.0 |
| Atlas A3 training and inference series | dav-2201 | >= 9.0.0 |
| Ascend 950PR/Ascend 950DT | dav-3510 | >= 9.1.0 |

## Directory Structure

```text
add_l2_cache_split/
|-- add_l2cache_nosplit.asc
|-- add_l2cache_split.asc
|-- l2_cache_kernel.h
|-- l2_cache_runner.h
|-- data_utils.h
|-- CMakeLists.txt
|-- scripts/
|   `-- gen_data.py
|-- README.md
`-- README_en.md
```

## Sample Design

The `data` buffer contains 201,326,592 half elements and occupies 384MiB in GM. With a 100MiB effective L2 Cache working-set threshold, ceiling division produces four L2 batches. The data is then split evenly into four contiguous, non-overlapping 96MiB ranges. Each L2 batch contains 50,331,648 half elements.

Each round reads and writes the complete 384MiB buffer at the same GM addresses. The logical GM-to-UB read traffic per kernel is therefore 768MiB, and the logical UB-to-GM write traffic is also 768MiB. Logical GM-to-UB traffic is not the same as physical GM-to-L2 traffic: an L2 Cache hit does not read main memory, while an L2 Cache miss fetches data into L2 Cache.

B0 through B3 are logical index ranges within the single `data` buffer, not additional buffers. Every round loads the current range into UB, applies `Adds`, and stores the result back to the same GM addresses. Splitting limits the repeatedly accessed range to 96MiB under the 100MiB threshold.

One L2 batch contains 3,145,728 32-byte DataBlocks. With 20 Vector cores, the first eight cores process 157,287 DataBlocks, or 2,516,592 half elements, and the remaining cores process 157,286 DataBlocks, or 2,516,576 elements. Each core processes 153 full UB tiles plus one aligned tail tile. A full UB tile holds 16,384 elements; the two tail sizes are 9,840 and 9,824 elements.

For L2 batches B0 through B3, round-first executes:

```text
Round 0: B0 -> B1 -> B2 -> B3
Round 1: B0 -> B1 -> B2 -> B3
```

Processing B1 through B3 can evict B0 before Round 1 returns to it. L2-batch-first instead executes:

```text
Batch B0: Round 0 -> Round 1
Batch B1: Round 0 -> Round 1
Batch B2: Round 0 -> Round 1
Batch B3: Round 0 -> Round 1
```

The second round of each batch is therefore more likely to read the first-round result from L2 Cache. Software `SyncAll` is called after each batch/round stage. Its GM workspace uses 32 bytes per core and is initialized to zero.

## Test Cases

Only the negative and positive implementations of the fixed workload are retained:

**Table 2**  Negative and positive test cases

| Case | Kernel | Loop order | Data elements | Data size | Cores | Batches | Rounds |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| Negative | `l2_cache_nosplit` | Round-first | 201326592 | 384MiB | 20 | 4 | 2 |
| Positive | `l2_cache_split` | L2-batch-first | 201326592 | 384MiB | 20 | 4 | 2 |

Both implementations read `input/input.bin` and write their results under `output/`. For performance attribution, manually compare `GM_to_UB_datas(KB)` in the profile reports and confirm that the logical GM-to-UB traffic is close.

## Build And Run

Run from this directory:

```bash
python3 scripts/gen_data.py

cmake -S . -B build -DCMAKE_ASC_ARCHITECTURES=dav-2201
cmake --build build -j

./build/demo_nosplit
./build/demo_split
```

`demo_nosplit` and `demo_split` are separate processes and do not accept `--mode`. `CMAKE_ASC_ARCHITECTURES` accepts `dav-2201` and `dav-3510`; use the architecture matching the current NPU.

## Profile Collection And Analysis

### Collect Profiles

Launch the two implementations in separate processes so the first kernel's cache state does not affect the second:

```bash
msopprof --output=./prof_384m_inplace_round_first_1 \
  ./build/demo_nosplit

msopprof --output=./prof_384m_inplace_l2_batch_first_1 \
  ./build/demo_split
```

Use a new output directory for every independent collection and alternate which implementation is collected first. For five pairs, use suffixes `_1` through `_5` for both directory groups.

After collection, manually inspect the `OPPROF_*` report files under each profile directory. Pair the round-first and batch-first directories by collection order, first confirm that total `GM_to_UB_datas(KB)` values are close, then compare L2 Cache hit rates, MTE2 time, and `Task Duration(us)`.

### Report Files

```text
OPPROF_<timestamp>/
|-- OpBasicInfo.csv
|-- PipeUtilization.csv
|-- L2Cache.csv
|-- Memory.csv
|-- MemoryL0.csv
|-- MemoryUB.csv
|-- ArithmeticUtilization.csv
`-- ResourceConflictRatio.csv
```

### Key Metrics

**Table 3**  Profile metrics

| Field | Meaning | Aggregation | Source |
| --- | --- | --- | --- |
| `Task Duration(us)` | Kernel task duration | Kernel record | `OpBasicInfo.csv` |
| `aiv_time(us)` | Vector Core execution time | Maximum across cores | `PipeUtilization.csv` |
| `aiv_mte2_time(us)` | MTE2 load time | Mean across cores | `PipeUtilization.csv` |
| `aiv_mte2_active_bw(GB/s)` | Active MTE2 bandwidth | Mean across cores | `PipeUtilization.csv` |
| `aiv_gm_to_ub_bw(GB/s)` | Effective GM-to-UB bandwidth | Mean across cores | `Memory.csv` |
| `GM_to_UB_datas(KB)` | Logical GM-to-UB traffic | Sum across cores | `Memory.csv` |
| `read_main_memory_datas(KB)` | Main-memory read counter | Sum across cores | `Memory.csv` |
| `aiv_read_hit_rate(%)` | L2 read hit rate | Mean across cores | `L2Cache.csv` |
| `aiv_write_hit_rate(%)` | L2 write hit rate | Mean across cores | `L2Cache.csv` |
| `aiv_total_hit_rate(%)` | L2 total hit rate | Mean across cores | `L2Cache.csv` |

`aiv_gm_to_ub_bw(GB/s)` is effective bandwidth, not GM traffic. Interpret `read_main_memory_datas(KB)` together with L2 hit rates and MTE2 time because its semantics can vary by architecture and profiler version.

### Measured Results For The 384MiB Workload

Existing profiles were collected with a different data size or the previous two-buffer implementation. They do not represent the current single-buffer, 384MiB in-place workload and must not be mixed into new results. The following data was recollected on 2026-07-28 with the current sample, CANN 9.1.0, `dav-2201`, 20 Vector cores, and a 1650MHz NPU frequency. The profile directories are `prof_384m_inplace_20260728_085319_round_first_1` through `prof_384m_inplace_20260728_085319_round_first_5`, and `prof_384m_inplace_20260728_085319_batch_first_1` through `prof_384m_inplace_20260728_085319_batch_first_5`.

The logical GM-to-UB traffic difference was below 1% in all five pairs, so the A/B fairness requirement is met.

**Table 4**  Per-pair fairness and speedup

| Pair | Negative `Task Duration(us)` | Positive `Task Duration(us)` | Logical GM-to-UB difference | Speedup |
| --- | ---: | ---: | ---: | ---: |
| 1 | 1475.3495 | 1071.5614 | 0.0435% | 1.3768x |
| 2 | 1468.6694 | 1056.6211 | 0.1516% | 1.3900x |
| 3 | 1475.9095 | 1072.4814 | 0.0401% | 1.3762x |
| 4 | 1470.4694 | 1054.2811 | 0.1670% | 1.3948x |
| 5 | 1470.3694 | 1075.2015 | 0.0543% | 1.3675x |

**Table 5**  Median metrics

| Metric | Negative round-first | Positive L2-batch-first | Change |
| --- | ---: | ---: | ---: |
| `Task Duration(us)` | 1470.4694 | 1071.5614 | 27.13% lower |
| `aiv_time(us)` | 1470.2946 | 1071.1370 | 27.15% lower |
| `aiv_mte2_time(us)` | 1333.5366 | 905.1114 | 32.13% lower |
| `aiv_mte2_active_bw(GB/s)` | 28.2165 | 41.6265 | 47.53% higher |
| `aiv_gm_to_ub_bw(GB/s)` | 25.6037 | 35.1896 | 37.44% higher |
| `GM_to_UB_datas(KB)` | 789575.0000 | 790076.8750 | 0.06% higher |
| `read_main_memory_datas(KB)` | 789592.5000 | 790121.2500 | 0.07% higher |
| `aiv_read_hit_rate(%)` | 1.2606 | 48.7423 | 47.4817 percentage points higher |
| `aiv_write_hit_rate(%)` | 99.9988 | 99.9451 | 0.0537 percentage points lower |
| `aiv_total_hit_rate(%)` | 21.5098 | 59.2343 | 37.7245 percentage points higher |

The measured result is that the positive implementation does not reduce logical GM-to-UB traffic. Instead, processing both rounds of one L2 batch back to back increases the probability that the second-round read hits L2 Cache. The median L2 read hit rate increased from 1.2606% to 48.7423%, median MTE2 time dropped from 1333.5366us to 905.1114us, and median `Task Duration(us)` dropped from 1470.4694us to 1071.5614us. The median paired speedup is 1.3768x.

The median `read_main_memory_datas(KB)` counter stays nearly flat, like the logical GM-to-UB traffic. With the current architecture and profiler semantics, this counter alone must not be used as proof that physical main-memory reads decreased. Attribute the result by first confirming logical-traffic fairness, then checking L2 read hit rate, MTE2 time, and task duration together. Profiler diagnostics show that this sample has low Vector compute utilization, so the observed benefit mainly comes from the load path and improved access locality.
