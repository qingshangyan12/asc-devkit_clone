# L2 Cache Splitting Optimization Sample

## Overview

This sample compares two implementations of the same fixed workload:

- Negative no-split: do not split for L2 Cache reuse; each round processes the whole 384MB GM range.
- Positive L2-split: split the 384MB range into four 96MB L2 batches under the 100MB effective working-set threshold, then finish both rounds of one batch before moving to the next batch.

The sample uses one 384MB GM data buffer. Each round computes `data = data + 1` in place, so the result after two rounds is `input + 2`. Both implementations issue the same logical GM-to-UB copies, UB-to-GM copies, and Adds operations. The difference is the L2 Cache working-set size between repeated accesses.

## Supported Products

**Table 1**  Supported products

| Product | Architecture | CANN version |
| --- | --- | --- |
| Atlas A2 training and inference series | dav-2201 | >= 9.0.0 |
| Atlas A3 training and inference series | dav-2201 | >= 9.0.0 |
| Ascend 950PR/Ascend 950DT | dav-3510 | >= 9.1.0 |

## Directory Structure

```text
10_add_l2_cache_split/
|-- add_l2cache_nosplit.asc       // Negative kernel entry and host main: process the full 384MB range
|-- add_l2cache_split.asc         // Positive kernel entry and host main: process by L2 batches
|-- add_l2cache_split_test.asc    // ACL initialization test placeholder, not included in CMake targets
|-- l2_cache_kernel.h             // Fixed workload parameters and shared device logic
|-- l2_cache_runner.h             // Shared host runtime: ACL setup, input read, kernel launch, output writeback
|-- data_utils.h                  // Binary file read/write utilities
|-- CMakeLists.txt                // Generates demo_nosplit and demo_split
|-- scripts/
|   `-- gen_data.py               // Generates fixed 384MB input/input.bin
|-- README.md                     // Chinese documentation
`-- README_en.md                  // English documentation
```

## Sample Design

### Fixed Workload

The `data` buffer contains 201,326,592 half elements and occupies 384MB in GM. Each of the two rounds reads and writes the full 384MB buffer, so the logical GM-to-UB read traffic per kernel is 768MB, and the logical UB-to-GM write traffic is also 768MB.

```text
Logical GM-to-UB reads = 384MB/round * 2 rounds = 768MB
Logical UB-to-GM writes = 384MB/round * 2 rounds = 768MB
```

Logical GM-to-UB traffic is not the same as physical GM-to-L2 traffic. An L2 Cache hit does not read main memory, while an L2 Cache miss fetches data into L2 Cache.

### Negative No-Split

The negative implementation does not split for L2 Cache reuse. Each round directly processes the complete `data[0, 384MB)` range:

```text
Round 0: data[0, 384MB)
Round 1: data[0, 384MB)
```

Each core still moves only one UB tile at a time, but a round keeps visiting new GM address ranges until it reaches the end of the 384MB buffer. Because 384MB exceeds the 100MB effective working-set threshold, early data from Round 0 can be evicted by later data in the same scan. When Round 1 starts again from the beginning of the range, those early addresses are more likely to miss L2 Cache and be fetched from GM again.

### Positive L2-Split

With a 100MB effective L2 Cache working-set threshold, ceiling division gives four L2 batches. The 384MB range is then split evenly into four 96MB contiguous ranges:

```text
l2BatchNum       = ceil(384MB / 100MB) = 4
l2BatchBytes     = 384MB / 4 = 96MB
l2BatchElements  = 96MB / sizeof(half) = 50331648
```

B0 through B3 are logical index ranges inside the single `data` buffer, not additional buffers. The positive implementation executes:

```text
Batch B0: Round 0 -> Round 1
Batch B1: Round 0 -> Round 1
Batch B2: Round 0 -> Round 1
Batch B3: Round 0 -> Round 1
```

Both rounds of one batch run back to back with no other large range inserted between them, so the second round is more likely to read the first-round result from L2 Cache.

### Core Split And UB Tiles

The shared device logic first splits a data range by 32-byte DataBlocks, then assigns the blocks to 20 Vector cores. One positive 96MB L2 batch contains 3,145,728 DataBlocks. The first eight cores process 157,287 DataBlocks, or 2,516,592 half elements, and the remaining cores process 157,286 DataBlocks, or 2,516,576 elements.

The negative implementation processes the complete 384MB range in each round, and the 20 cores jointly cover `data[0, 384MB)`. Inside each core, both implementations split work into UB tiles with `MAX_DATA_COPY_LEN = 16384` half elements. UB tiling solves the Unified Buffer capacity limit; it is not the L2 Cache split granularity. A loop that keeps visiting new GM addresses can still fill and evict L2 Cache.

### Inter-Core Synchronization

The negative implementation calls hardware `AscendC::SyncAll()` after each full 384MB round. The positive implementation calls `AscendC::SyncAll()` after each L2-batch/round stage, ensuring that all cores finish the current UB-to-GM stores before the next stage begins. Hardware synchronization does not need an extra GM synchronization workspace.

## Test Cases

Only the negative and positive implementations of the fixed workload are retained:

**Table 2**  Negative and positive test cases

| Case | Kernel | L2 Cache split | Active working set | Data elements | Data size | Cores | Rounds |
| --- | --- | --- | ---: | ---: | ---: | ---: | ---: |
| Negative | `l2_cache_nosplit` | Disabled | 384MB | 201326592 | 384MB | 20 | 2 |
| Positive | `l2_cache_split` | 4 x 96MB batches | 96MB | 201326592 | 384MB | 20 | 2 |

Both implementations read `input/input.bin` generated by `scripts/gen_data.py` and write their results under `output/`. For performance attribution, manually compare `GM_to_UB_datas(KB)` in the profile reports and confirm that the logical GM-to-UB traffic is close.

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

The build targets are:

| Target | Source file | Kernel entry | Output file |
| --- | --- | --- | --- |
| `demo_nosplit` | `add_l2cache_nosplit.asc` | `l2_cache_nosplit` | `output/no_split.bin` |
| `demo_split` | `add_l2cache_split.asc` | `l2_cache_split` | `output/l2_split.bin` |

The output files are:

```text
output/no_split.bin
output/l2_split.bin
```

At runtime, the program prints the case name, total data size, L2 Cache split state, active working-set size, logical GM-to-UB traffic, Vector core count, L2 batch count, and compute round count. Each case verifies its output after execution: `[PASS]` is printed on success, while `[FAIL]` is printed on failure together with the first mismatched element index, input value, actual output value, and expected output value. The two output files should be byte-identical, and every `half` element should equal `input + 2`.

## Profile Collection And Analysis

### Collect Profiles

Launch the two implementations in separate processes so the first kernel's cache state does not affect the second:

```bash
msopprof --output=./prof_384m_inplace_no_split_1 \
  ./build/demo_nosplit

msopprof --output=./prof_384m_inplace_l2_split_1 \
  ./build/demo_split
```

Use a new output directory for every independent collection and alternate which implementation is collected first. For five pairs, use `prof_384m_inplace_no_split_1` through `prof_384m_inplace_no_split_5` and `prof_384m_inplace_l2_split_1` through `prof_384m_inplace_l2_split_5`.

After collection, manually inspect the `OPPROF_*` report files under each profile directory. Pair the no-split and L2-split directories by collection order, first confirm that total `GM_to_UB_datas(KB)` values are close, then compare L2 Cache hit rates, MTE2 time, and `Task Duration(us)`.

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

### Measured Results For The 384MB Workload

The following data was recollected on 2026-07-29 with the current no-split and L2-split implementations, CANN 9.1.0, `dav-2201`, NPU run mode, Device 0, 20 Vector cores, and a 1650MHz NPU frequency. In the direct NPU run, `output/no_split.bin` and `output/l2_split.bin` were byte-identical, and all 201,326,592 half elements matched `input + 2`.

The profile runs used the default warm-up behavior. Five no-split and five L2-split profiles were collected with alternating launch order. The profile directories are `prof_384m_inplace_20260729_093359_no_split_1` through `prof_384m_inplace_20260729_093359_no_split_5`, and `prof_384m_inplace_20260729_093359_l2_split_1` through `prof_384m_inplace_20260729_093359_l2_split_5`.

The logical GM-to-UB traffic difference was below 1% in all five pairs, so the A/B fairness requirement is met.

**Table 4**  Per-pair fairness and speedup

| Pair | Negative `Task Duration(us)` | Positive `Task Duration(us)` | Logical GM-to-UB difference | Speedup |
| --- | ---: | ---: | ---: | ---: |
| 1 | 1439.1888 | 1068.6614 | 0.1123% | 1.3467x |
| 2 | 1440.6688 | 1064.2812 | 0.2153% | 1.3537x |
| 3 | 1435.9087 | 1071.9015 | 0.1046% | 1.3396x |
| 4 | 1440.5088 | 1057.2611 | 0.2550% | 1.3625x |
| 5 | 1437.0487 | 1069.6814 | 0.1018% | 1.3434x |

**Table 5**  Median metrics

| Metric | Negative no-split | Positive L2-split | Change |
| --- | ---: | ---: | ---: |
| `Task Duration(us)` | 1439.1888 | 1068.6614 | 25.75% lower |
| `aiv_time(us)` | 1438.8497 | 1068.1552 | 25.76% lower |
| `aiv_mte2_time(us)` | 1305.0142 | 899.7586 | 31.05% lower |
| `aiv_mte2_active_bw(GB/s)` | 28.8393 | 41.8707 | 45.19% higher |
| `aiv_gm_to_ub_bw(GB/s)` | 26.1645 | 35.2861 | 34.86% higher |
| `GM_to_UB_datas(KB)` | 789086.3750 | 790067.5000 | 0.12% higher |
| `read_main_memory_datas(KB)` | 789067.6250 | 790046.2500 | 0.12% higher |
| `aiv_read_hit_rate(%)` | 1.1922 | 48.7031 | 47.5108 percentage points higher |
| `aiv_write_hit_rate(%)` | 99.9988 | 99.9444 | 0.0544 percentage points lower |
| `aiv_total_hit_rate(%)` | 21.4666 | 59.2068 | 37.7402 percentage points higher |

The measured result is that the positive implementation does not reduce logical GM-to-UB traffic. Instead, reducing the repeated-access working set from 384MB to 96MB increases the probability that second-round reads hit L2 Cache. The median L2 read hit rate increased from 1.1922% to 48.7031%, median MTE2 load time dropped from 1305.0142us to 899.7586us, and median `Task Duration(us)` dropped from 1439.1888us to 1068.6614us. The median paired speedup is 1.3467x.

The median `read_main_memory_datas(KB)` counter stays nearly flat, like the logical GM-to-UB traffic. With the current architecture and profiler semantics, this counter alone must not be used as proof that physical main-memory reads decreased. Attribute the result by first confirming logical-traffic fairness, then checking L2 read hit rate, MTE2 time, and task duration together. Profiler diagnostics show that this sample has low Vector compute utilization, so the observed benefit mainly comes from the load path and improved access locality.

## Acceptance Criteria

- Both `dav-2201` and `dav-3510` builds pass.
- The `GM_to_UB_datas(KB)` difference of each profile pair is below 1%.
- The positive implementation has a higher L2 Cache read hit rate, lower MTE2 time, and lower `Task Duration(us)` than the negative implementation.
