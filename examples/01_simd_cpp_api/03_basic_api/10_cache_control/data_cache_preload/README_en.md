# DataCachePreload Indirect Index Preload Sample

## Overview
This sample demonstrates how to use [DataCachePreload](../../../../../docs/api/SIMD-API/基础API/缓存控制/DataCachePreload.md) in an indirect index (random-hopping) access scenario: when the Scalar unit reads scattered data from GM, DataCachePreload is used to proactively load upcoming Cache Lines into DCache, reducing Scalar memory access wait time and improving performance.

## Supported Products and CANN Software Versions

| Product | CANN Software Version |
|------|-------------|
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |
| Atlas A3 Training Series Products/Atlas A3 Inference Series Products | >= CANN 9.0.0 |
| Atlas A2 Training Series Products/Atlas A2 Inference Series Products | >= CANN 9.0.0 |

## Directory Structure

```
├── data_cache_preload
│   ├── scripts
│   │   ├── gen_data.py             // Input data and ground truth generation script
│   │   └── verify_result.py        // Verification script for output vs golden data
│   ├── CMakeLists.txt              // Build project file
│   ├── data_utils.h                // Data read/write utility functions
│   ├── data_cache_preload.asc      // Ascend C sample implementation & invocation
│   ├── test.sh                     // One-click build and run script
│   └── README.md                   // Sample documentation
```

## Sample Description

This sample uses SCENARIO_NUM to control the execution branch, with different values corresponding to whether DataCachePreload is used.

| SCENARIO_NUM | Scenario | Description |
|-----------------|---------|------|
| 1 | Indirect index accumulation (no preload) | Direct Scalar reads without DataCachePreload |
| 2 | Indirect index accumulation (with preload) | Uses DataCachePreload to pre-fetch next Cache Line into DCache |

### Formula and Sample Specifications

Both scenarios share the same formula for indirect index accumulation:

$$
result = \sum_{i=0}^{255} data\_gm[indices[i]]
$$

- data_gm is a 64KB random uint8_t data buffer
- indices contains 256 random offsets, each pointing to a different Cache-Line-aligned position
- Since indices are randomly scattered across 64KB, the vast majority of Scalar reads trigger a DCache miss
- DataCachePreload asynchronously pre-fetches the next Cache Line while reading current data, forming a pipeline between Scalar reads and GM to DCache data movement

#### SCENARIO_NUM=1 (Indirect Index Accumulation, without DataCachePreload)
- Sample Specifications:
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">Sample Type(OpType)</td><td colspan="4" align="center">DataCachePreloadNoPreload</td></tr>
  <tr><td rowspan="2" align="center">Sample Input</td><td align="center">name</td><td align="center">size</td><td align="center">data type</td><td align="center">Description</td></tr>
  <tr><td align="center">data_gm</td><td align="center">[65536]</td><td align="center">uint8_t</td><td align="center">64KB random data buffer (1024 Cache Lines)</td></tr>
  <tr><td align="center"></td><td align="center">indices</td><td align="center">[256]</td><td align="center">uint32_t</td><td align="center">Random indices, Cache-Line-aligned</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">result</td><td align="center">[1]</td><td align="center">uint32_t</td><td align="center">Accumulated sum</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">kernel_no_preload</td></tr>
  </table>

#### SCENARIO_NUM=2 (Indirect Index Accumulation, with DataCachePreload)
- Sample Specifications:
  <table border="1" align="center">
  <tr><td rowspan="1" align="center">Sample Type(OpType)</td><td colspan="4" align="center">DataCachePreloadWithPreload</td></tr>
  <tr><td rowspan="2" align="center">Sample Input</td><td align="center">name</td><td align="center">size</td><td align="center">data type</td><td align="center">Description</td></tr>
  <tr><td align="center">data_gm</td><td align="center">[65536]</td><td align="center">uint8_t</td><td align="center">64KB random data buffer (1024 Cache Lines)</td></tr>
  <tr><td align="center"></td><td align="center">indices</td><td align="center">[256]</td><td align="center">uint32_t</td><td align="center">Random indices, Cache-Line-aligned</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">result</td><td align="center">[1]</td><td align="center">uint32_t</td><td align="center">Accumulated sum</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">kernel_with_preload</td></tr>
  </table>

## Sample Implementation

- How to Use DataCachePreload
  1. Create a `GlobalTensor<uint64_t>` bound to data_gm:
     ```cpp
     AscendC::GlobalTensor<uint64_t> dataGlobal;
     dataGlobal.SetGlobalBuffer(reinterpret_cast<__gm__ uint64_t*>(data_gm), size);
     ```
  2. Compute Cache-Line-aligned offset: `cacheOffset = idx & ~63ULL` (align to 64B boundary)
  3. Call `DataCachePreload(dataGlobal, cacheOffset)` to load 64B into DCache

- Constraints
  - Each call preloads exactly 1 Cache Line (64B)
  - Consecutive calls should not exceed 4, otherwise the Reservation Station may stall
  - Preload is an async instruction that can overlap with subsequent computation/memory access

## Performance Observation

When profiling with `msprof`, compare the execution time of the two kernels:
- `kernel_no_preload`: High DCache miss rate, large Scalar read latency
- `kernel_with_preload`: Preloads the next round's data in advance, significantly improving DCache hit rate

### Performance Comparison

The following performance data was collected on **Atlas A2/A3 series products (dav-2201 architecture)**. The data below is the average of 5 independent runs to eliminate random fluctuations. **DataCachePreload achieves a 1.64x speedup in end-to-end execution time for indirect index access scenarios.**

| Metric | kernel_no_preload | kernel_with_preload | Improvement |
|------|------------------|---------------------|---------|
| **Task Duration (us)** | 44.000 | 26.760 | **1.64x speedup** |
| **aiv_total_cycles** | 78,230 | 47,189 | **Reduced by 39.7%** |
| **aiv_scalar_time (us)** | 41.942 | 24.622 | **Reduced by 41.3%** |
| **aiv_scalar_ratio** | 96.5% | 93.9% | Scalar remains the main bottleneck |

> **Metric Descriptions:**
> - **Task Duration**: End-to-end kernel execution time, the most direct performance indicator
> - **aiv_total_cycles**: Total cycles of the Vector core, reflecting combined computation + memory access overhead at hardware level
> - **aiv_scalar_time**: Scalar unit execution time; in indirect index scenarios, Scalar memory access is the main bottleneck
> - **aiv_scalar_ratio**: Percentage of Scalar time in total time, used to identify performance bottlenecks

#### Detailed Data from 5 Independent Runs

| Run | no_preload Duration(us) | with_preload Duration(us) | Speedup |
|---------|------------------------|--------------------------|--------|
| Run 1 | 43.780 | 26.900 | 1.63x |
| Run 2 | 43.400 | 27.200 | 1.60x |
| Run 3 | 43.820 | 27.140 | 1.61x |
| Run 4 | 45.160 | 26.300 | 1.72x |
| Run 5 | 43.840 | 26.260 | 1.67x |
| **Average** | **44.000** | **26.760** | **1.64x** |

### Conclusion

**DataCachePreload demonstrates significant optimization on Atlas A2/A3 series products**, with `kernel_with_preload` showing substantially lower Task Duration, aiv_total_cycles, and aiv_scalar_time compared to `kernel_no_preload`. In indirect index (random-hopping) scenarios, preloading the next round's data into DCache via DataCachePreload **effectively hides Scalar memory access latency, achieving approximately 1.64x performance improvement**.

## Compilation and Execution

Execute the following steps in this sample's root directory.

- Configure Environment Variables
  Please refer to the [installation guide](../../../../../docs/quick_start.md#prepare&install) to configure environment variables based on your CANN installation.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```
  > **Note:** `${install_path}` is the CANN package installation path; defaults to `/usr/local/Ascend`.

- Sample Execution
  This sample uses the `SCENARIO_NUM` parameter to run the two kernel functions independently:
  - `SCENARIO_NUM=1`: Runs `kernel_no_preload` (without DataCachePreload)
  - `SCENARIO_NUM=2`: Runs `kernel_with_preload` (with DataCachePreload)

  Run the following commands in the sample root directory:
  ```bash
  SCENARIO_NUM=1;                                                           # Set scenario number (1=no preload, 2=with preload)
  mkdir -p build && cd build;                                               # Create and enter build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j;                      # Build project (default NPU mode)
  python3 ../scripts/gen_data.py                                            # Generate test input data
  msprof ./demo $SCENARIO_NUM
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin   # Verify results
  ```

  For CPU debug or NPU simulation modes, add `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim`:

  ```bash
  cmake -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # CPU debug mode
  cmake -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-2201 ..;make -j; # NPU simulation mode
  ```

  > **Note:** Run `rm CMakeCache.txt` in the build directory before switching build modes.

- Build Options

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU, CPU debug, or NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-2201` (default), `dav-3510` | dav-2201 for Atlas A2/A3 series, dav-3510 for Ascend 950 series |
  | `SCENARIO_NUM` | `1` (default), `2` | Scenario: 1 (no preload), 2 (with preload) |

- Result
  The output below indicates that the results are correct:
  ```bash
  test pass!
  ```
