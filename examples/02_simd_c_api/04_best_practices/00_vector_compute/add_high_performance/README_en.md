# Add High-Performance Sample

## Overview

This sample implements float16 element-wise Add with the Ascend C C API. It provides scenarios with Ping/Pong Buffer optimization disabled and enabled. Both scenarios use the same inputs, core count, tiling, and vector arithmetic so that the effect of overlapping GM/UB data movement with Vector computation can be compared directly.

## Supported Products and CANN Versions

**Table1**  Supported Products and CANN Versions

| Product | CANN Version |
| --- | --- |
| Ascend 950PR/Ascend 950DT | >= CANN 9.1.0 |

## Directory Structure

```text
add_high_performance
|-- figures
|   `-- double_buffer.png    # Ping/Pong Buffer pipeline diagram
|-- scripts
|   |-- gen_data.py          # Generates input and golden data
|   `-- verify_result.py     # Verifies the output
|-- CMakeLists.txt           # Build configuration
|-- add.asc                  # Kernel and host implementation
|-- data_utils.h             # Binary file utilities
|-- README.md
`-- README_en.md
```

## Sample Description

The sample performs element-wise addition on two float16 matrices with shape `[8192, 8192]`:

```text
z = x + y
```

The input, output, and golden data used for accuracy verification are listed below. All data is stored contiguously in ND format in GM.

**Table2**  Input and Output Data Specifications

| Data | Input/Output | Data file | Data type | Shape | Description |
| --- | --- | --- | --- | --- | --- |
| x | Input | `input/input_x.bin` | float16 | `[8192, 8192]` | Randomly generated in the range `[1, 100)`. |
| y | Input | `input/input_y.bin` | float16 | `[8192, 8192]` | Randomly generated in the range `[1, 100)`. |
| z | Output | `output/output.bin` | float16 | `[8192, 8192]` | Result computed by the Kernel |
| golden | Golden data | `output/golden.bin` | float16 | `[8192, 8192]` | Generated using `(x + y).astype(np.float16)`. |

Both scenarios use the same multicore partitioning and tile size. The matrix is evenly partitioned along the M axis across 64 cores. Each core processes 128 contiguous rows, or 1048576 elements, in 49 iterations: 48 full-tile iterations followed by one tail iteration.

**Table3**  Multicore Partitioning and Tile Parameters

| Parameter | Value | Description |
| --- | --- | --- |
| Total matrix elements | 67108864 | `8192 * 8192` |
| Number of cores | 64 | Each core processes a contiguous range along the M axis. |
| Rows per core | 128 | `8192 / 64` |
| Elements per core | 1048576 | `128 * 8192` |
| Iterations per core | 49 | 48 full tiles and one tail tile |
| Valid elements in a full tile | 21760 | Corresponds to `DATA_COPY_LENGTH`. |
| Valid elements in the tail tile | 4096 | `1048576 - 48 * 21760` |
| Reserved length of each UB buffer | 21840 float16 elements | Corresponds to `MAX_DATA_COPY_LENGTH` and defines the UB address range of each buffer. |
| VF vector length | 256 bytes | Processes 128 float16 elements at a time; a full tile uses 170 VF iterations and the tail tile uses 32. |

The `SCENARIO_NUM` build option enables or disables Ping/Pong Buffer optimization. Both scenarios use identical input data, multicore partitioning, tile size, and vector computation. They differ only in the number of UB buffers and pipeline synchronization.

**Table4**  Scenario Configuration

| `SCENARIO_NUM` | Kernel | Scenario | UB buffer configuration | UB address space | Pipeline behavior |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | Ping/Pong Buffer optimization disabled | One x/y/z buffer group | `3 * 21840 * sizeof(half)` = 131040 bytes | Completes load, compute, and store before processing the next tile. |
| 1 | `add_double_buffer` | Ping/Pong Buffer optimization enabled | Ping/Pong x/y/z buffer groups | `6 * 21840 * sizeof(half)` = 262080 bytes | MTE2, Vector, and MTE3 process different tiles with cross-iteration overlap. |

### Scenario 0: Ping/Pong Buffer Optimization Disabled

When Ping/Pong Buffer optimization is disabled, the Kernel `add_baseline` allocates only one x/y/z buffer group. The three buffers are contiguous in UB. Each buffer reserves 21840 float16 elements, while the current iteration accesses only the valid elements specified by `current_length`.

**Table5**  UB Layout with Ping/Pong Buffer Optimization Disabled

| UB buffer | Start offset (float16 elements) | Valid elements in a full tile | Purpose |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | Stores x loaded from GM for the current iteration. |
| `y_local` | 21840 | 21760 | Stores y loaded from GM for the current iteration. |
| `z_local` | 43680 | 21760 | Stores the Add result for the current iteration. |

Each iteration processes one tile in the following order:

1. The MTE2 pipeline calls `asc_copy_gm2ub_align` twice to load the current x and y tiles from GM to UB.
2. After loading the inputs, MTE2 calls `asc_unlock(PIPE_MTE2, 0)` to release Mutex 0. The Vector pipeline waits for the inputs with `asc_lock(PIPE_V, 0)` and then calls `add_vf`. A full tile executes 170 `asc_add` operations in groups of 128 float16 elements, while the tail tile executes 32 operations.
3. Vector releases Mutex 0 after computation. MTE3 acquires the same Mutex and calls `asc_copy_ub2gm_align` to store z from UB to GM.
4. When MTE2 acquires Mutex 0 in the next iteration, it waits for the preceding MTE3 stage to release the Mutex before overwriting the same UB addresses. After all iterations, one `asc_sync_pipe(PIPE_ALL)` completes the final synchronization.

Therefore, while the Vector pipeline is computing iteration n, MTE2 cannot load the inputs for iteration n+1. Similarly, while MTE3 is storing the result of iteration n, computation for iteration n+1 cannot begin. The pipelines follow their dependencies within each iteration but cannot overlap across iterations. This scenario provides the performance baseline with Ping/Pong Buffer optimization disabled.

### Scenario 1: Ping/Pong Buffer Optimization Enabled

When Ping/Pong Buffer optimization is enabled, the Kernel `add_double_buffer` divides UB into two independent x/y/z buffer groups. Even-numbered iterations use Ping and Mutex 0, while odd-numbered iterations use Pong and Mutex 1. Adjacent iterations use different buffers and Mutexes, so they neither access the same UB addresses nor block on the same synchronization resource.

**Table6**  Buffer Layout with Ping/Pong Buffer Optimization Enabled

| Iteration | Buffer group | Mutex ID | x start offset | y start offset | z start offset |
| --- | --- | --- | --- | --- | --- |
| Even | Ping | 0 | 0 | 21840 | 43680 |
| Odd | Pong | 1 | 65520 | 87360 | 109200 |

All offsets in the table are measured in float16 elements. Each group still reserves 21840 float16 elements for each of x, y, and z, so the data movement and Vector workload per iteration are identical to those with Ping/Pong Buffer optimization disabled. When the optimization is enabled, the Kernel does not perform a full pipeline synchronization at the end of each iteration. Instead, `asc_lock` and `asc_unlock` use a Mutex to manage the pipeline dependencies of each buffer group.

**Table7**  Synchronization Dependencies with Ping/Pong Buffer Optimization Enabled

| Pipeline stage | Mutex ID | Synchronization operation | Enforced dependency |
| --- | --- | --- | --- |
| MTE2 load | 0 for Ping, 1 for Pong | Acquire with `asc_lock(PIPE_MTE2, mutex_id)`, load x and y, and then call `asc_unlock`. | Before reusing a group, wait for its preceding MTE3 store to finish. |
| Vector computation | Matches the current buffer group | Acquire with `asc_lock(PIPE_V, mutex_id)`, call `add_vf`, and then call `asc_unlock`. | Vector waits for the current MTE2 stage to release the Mutex and cannot read incomplete inputs. |
| MTE3 store | Matches the current buffer group | Acquire with `asc_lock(PIPE_MTE3, mutex_id)`, store z, and then call `asc_unlock`. | MTE3 waits for the current Vector stage to release the Mutex and cannot store an incomplete result. |

Each iteration selects Mutex 0 or 1 using `loop_idx & 1`, then acquires and releases that Mutex on the MTE2, Vector, and MTE3 pipelines in sequence. The same Mutex chains the three stages of one buffer group, while different Mutexes do not block one another. After all 49 iterations have been issued, the Kernel calls `asc_sync_pipe(PIPE_ALL)` once to ensure that the final data movement and computation have completed. See [asc_lock](../../../../../docs/zh/api/SIMD-API/C-API/sync/asc_lock.md) and [asc_unlock](../../../../../docs/zh/api/SIMD-API/C-API/sync/asc_unlock.md) for interface constraints and the double-buffer pattern.

For example, iteration 0 uses Ping and Mutex 0, iteration 1 uses Pong and Mutex 1, and iteration 2 reuses Ping and Mutex 0. While Vector processes the Ping data for iteration 0, MTE2 can load the Pong data for iteration 1. While MTE3 stores the Ping result for iteration 0, Vector can process the Pong data for iteration 1. Iteration 2 waits for iteration 0 MTE3 to release Mutex 0 before reusing Ping. MTE2, Vector, and MTE3 can therefore process adjacent iterations concurrently and hide part of the GM-to-UB and UB-to-GM data movement latency.

The main differences between the two scenarios are summarized below.

**Table8**  Ping/Pong Buffer Optimization Comparison

| Comparison | Scenario 0: Ping/Pong Buffer optimization disabled | Scenario 1: Ping/Pong Buffer optimization enabled |
| --- | --- | --- |
| UB buffers | One x/y/z group, 131040 bytes | Two x/y/z groups, 262080 bytes |
| Addresses used by adjacent iterations | The same; reuse waits for the previous iteration to finish. | Different; alternates between Ping and Pong. |
| End-of-iteration synchronization | Uses Mutex 0 to chain iterations and performs one full synchronization after all iterations. | Uses Mutex 0/1 for Ping/Pong dependencies and performs one full synchronization after all iterations. |
| Cross-iteration pipeline overlap | Not supported | MTE2, Vector, and MTE3 can process different iterations. |
| Optimization cost | Lower UB usage | Doubles the UB address space to enable pipeline overlap. |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

## Performance Comparison

The scenarios differ only in buffer count and synchronization. Collect both scenarios on the same hardware, CANN version, and system load, and verify accuracy before every performance collection.

### Test Conditions

**Table9**  Performance Test Configuration

| Configuration | Value |
| --- | --- |
| Chip version | Ascend 950PR |
| CANN version | To be added (CANN 9.1.0 or later) |
| NPU architecture | `dav-3510` |
| Input/output data type | float16 |
| Input/output shape | `[8192, 8192]` |
| AI Vector Cores | 64 |
| Tile length | 21760 float16 elements; the tail tile contains 4096 float16 elements |
| Number of measurements | Warm up each scenario once, collect at least five valid runs, and report the median Task Duration |

Keep the device frequency, power mode, and system load consistent during testing, and ensure that no other task occupies the NPU. Both scenarios must use the same input data, core count, tiling parameters, and build options.

### Performance Metrics

Use msOpProf to collect performance data and inspect the following fields in `PipeUtilization.csv`.

**Table10**  Performance Metric Descriptions

| Metric | Description |
| --- | --- |
| `Task Duration(μs)` | Total Task execution time. Use this metric for the end-to-end comparison. |
| `aiv_time(μs)` | Task execution time on AI Vector Core. |
| `aiv_vec_time(μs)` / `aiv_vec_ratio` | Vector instruction duration and ratio. |
| `aiv_scalar_time(μs)` / `aiv_scalar_ratio` | Scalar instruction duration and ratio. |
| `aiv_mte2_time(μs)` / `aiv_mte2_ratio` | Duration and ratio of GM-to-UB transfers on the MTE2 pipeline. |
| `aiv_mte3_time(μs)` / `aiv_mte3_ratio` | Duration and ratio of UB-to-GM transfers on the MTE3 pipeline. |

With Ping/Pong Buffer optimization enabled, different pipelines can overlap. Therefore, pipeline durations or ratios cannot be simply added to obtain `Task Duration`. Evaluate the optimization primarily by `Task Duration`, and use the MTE2, Vector, and MTE3 metrics to determine whether the bottleneck changes.

### Collection Procedure

Run the following commands from the sample directory. `SCENARIO_NUM=0` disables Ping/Pong Buffer optimization, while `SCENARIO_NUM=1` enables it. Use a separate build directory for each scenario to prevent CMake cache interference.

```bash
SCENARIO_NUM=0
BUILD_DIR=build_s${SCENARIO_NUM}
cmake -S . -B ${BUILD_DIR} -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM}
cmake --build ${BUILD_DIR} -j

cd ${BUILD_DIR}
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin

# Warm up before collecting performance data.
./demo
msopprof ./demo
cat ./OPPROF_*/PipeUtilization.csv
cd ..
```

Change `SCENARIO_NUM` to `1` and repeat the procedure. Collect at least five valid runs for each scenario, record the `Task Duration` of each run, and enter the median in the result table. Use the other pipeline metrics from the run corresponding to that median.

### Ascend 950PR Performance Comparison

The table below compares the performance data measured on the Ascend 950PR chip.

**Table11**  Ping/Pong Buffer Optimization Performance Comparison

| Scenario | Configuration | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Scenario 0 | Ping/Pong Buffer optimization disabled | 271.472 | 270.18 | 11.117 | 0.041 | 1.553 | 0.006 | 188.566 | 0.698 | 67.182 | 0.249 |
| Scenario 1 | Ping/Pong Buffer optimization enabled | 268.456 | 267.51 | 10.993 | 0.041 | 1.944 | 0.007 | 258.8 | 0.967 | 137.686 | 0.515 |

**Optimization Effect Analysis**:

Using Scenario 0 as the baseline, the speedup after enabling Ping/Pong Buffer optimization is

$$
S = \frac{T_{\text{baseline}}}{T_{\text{ping-pong}}}
  = \frac{271.472}{268.456}
  \approx 1.011
$$

The end-to-end duration reduction is

$$
R = \frac{T_{\text{baseline}} - T_{\text{ping-pong}}}{T_{\text{baseline}}} \times 100\%
  = \frac{271.472 - 268.456}{271.472} \times 100\%
  \approx 1.11\%
$$

- `Task Duration` decreases from 271.472μs to 268.456μs, a reduction of 3.016μs and an end-to-end improvement of approximately 1.11%.
- `aiv_time` decreases from 270.18μs to 267.51μs, a reduction of approximately 0.99%.
- `aiv_vec_time` decreases from 11.117μs to 10.993μs, a change of approximately 1.12%, indicating that the Vector workload and computational efficiency are essentially unchanged.
- `aiv_scalar_time` increases by 0.391μs. Although this is a relative increase of approximately 25.18%, its absolute duration and ratio remain small and have little impact on overall performance.

**Theoretical Performance Analysis**:

For this sample, a theoretical Vector duration can be calculated for the Add computation alone, excluding data movement, pipeline synchronization, instruction scheduling, and other overhead. The Ascend 950PR processes 128 half elements per cycle at 1.65GHz. With 64 AI Vector Cores, the theoretical Vector duration is

$$
T_{\text{theory}}
= \frac{M \times N}{128 \times 1.65 \times 10^9 \times \text{core count}}
= \frac{8192 \times 8192}{128 \times 1.65 \times 10^9 \times 64}
\approx 4.965\ \mu s
$$

The measured `aiv_vec_time` values are 11.117μs and 10.993μs, approximately 2.2 times the theoretical Vector duration. The theoretical value represents only the Add instruction duration at ideal throughput and excludes practical overhead such as VF loop control, address calculation, data movement, and pipeline synchronization. It therefore cannot be used as the theoretical end-to-end duration of the entire Kernel. The similar Vector durations in both scenarios, which are also much shorter than the MTE2 duration, show that pure Vector computation is not the end-to-end bottleneck.

#### Double Buffer Optimization Summary

- **Current effect**: Ping/Pong Buffer uses two independent UB buffer groups to overlap MTE2 loading, Vector computation, and MTE3 storing across iterations. It reduces `Task Duration` by 3.016μs and improves end-to-end performance by approximately 1.11%.
- **Advantages**: It hides part of the pipeline wait time and increases read-write concurrency for consecutive tiles without changing the result or the workload of each iteration. Workloads whose computation and data movement durations are similar generally benefit more.
- **Limitations**: UB usage increases from 131040 bytes to 262080 bytes, while Ping/Pong address selection and synchronization introduce additional overhead. In this sample, the measured Vector duration is approximately 11μs, while MTE2 takes more than 188μs. This large imbalance limits the amount of computation time that double buffering can hide, so the overall gain remains constrained by GM data movement.
- **Future optimization**: Retain 512-byte alignment and the current L2 Cache bypass strategy while evaluating different tile sizes and core-count configurations, improving contiguous GM access and transfer parameters, and reducing Scalar address-calculation and synchronization overhead. In practical workloads, operator fusion can also increase the amount of computation and data reuse per load, improving the benefit of overlapping computation with data movement.

**Pipeline and Bandwidth Analysis**:

In Scenario 0, the MTE2, MTE3, and Vector pipeline ratios sum to 98.8%, indicating that the pipelines execute mostly serially. In Scenario 1, the sum is 152.3%, exceeding 100%, which shows that loading, computation, and storing overlap across iterations after Ping/Pong Buffer is enabled.

The MTE2 and MTE3 durations reported by the profiler include pipeline execution and wait time. After the optimization is enabled, `aiv_mte2_time` increases from 188.566μs to 258.8μs and `aiv_mte3_time` increases from 67.182μs to 137.686μs. These values cannot be interpreted directly as an increase in the latency of each individual data transfer; they must be evaluated together with pipeline overlap and bandwidth.

Scenario 0 does not use Ping/Pong Buffer, so its read and write pipelines execute mostly serially. Its read bandwidth is estimated from the total input data volume:

$$
D_{\text{read}} = M \times N \times sizeof(half) \times 2 = 268435456\ \text{B}
$$

$$
BW_{\text{read}} = \frac{D_{\text{read}}}{T_{mte2}}
= \frac{268435456}{188.566 \times 10^{-6}}
\approx 1.424\ \text{TB/s}
$$

In Scenario 1, MTE2 and MTE3 execute concurrently after Ping/Pong Buffer is enabled. The read-write mixed bandwidth is estimated from the total input and output data volume:

$$
D_{\text{write}} = M \times N \times sizeof(half) = 134217728\ \text{B}
$$

$$
BW_{\text{rw}} = \frac{D_{\text{read}} + D_{\text{write}}}{T_{mte2}}
= \frac{402653184}{258.8 \times 10^{-6}}
\approx 1.556\ \text{TB/s}
$$

Scenario 1 has an `aiv_mte2_ratio` of 96.7%, showing that GM data movement remains the main bottleneck after optimization. Ping/Pong Buffer improves mixed read-write throughput through pipeline overlap. However, because Vector computation is much shorter than data movement, only a limited amount of computation time can be hidden, resulting in an end-to-end improvement of approximately 1.11%.

## Build and Run

Configure the CANN environment:

```bash
source ${install_path}/cann/set_env.sh
```

Build and run one scenario from this sample directory:

```bash
SCENARIO_NUM=0
mkdir -p build && cd build
cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=${SCENARIO_NUM} ..
make -j
python3 ../scripts/gen_data.py
./demo
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Set `SCENARIO_NUM=1` and rerun CMake, build, execution, and verification to test the scenario with Ping/Pong Buffer optimization enabled. Clear the CMake cache before switching scenarios.

**Table12**  Build Options

| Build option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode |
| `SCENARIO_NUM` | `0`, `1` (default) | Ping/Pong Buffer optimization disabled or enabled |

A successful verification prints:

```text
test pass!
```

## Functional Debugging

### printf

The `printf` interface provides formatted output for SIMD and SIMT debugging. It can print the core index, tiling parameters, or intermediate state from Kernel code. Include `utils/debug/asc_printf.h` before using the [printf](../../../../../docs/zh/api/Utils-API/调测接口/printf.md) interface.

This sample uses the C API. Call [asc_get_block_idx](../../../../../docs/zh/api/SIMD-API/C-API/sys_var/asc_get_block_idx.md) to obtain the current core index instead of using the C++ API `AscendC::GetBlockIdx`. For example:

```cpp
#include "utils/debug/asc_printf.h"

__vector__ __global__ void add_debug()
{
    printf("add blockIdx=%ld\n", asc_get_block_idx());
}
```

> **Note:** The `printf` interface increases Kernel execution time and affects performance measurements. Use it only during functional debugging. Remove or disable printing before performance collection, or set `ASCENDC_DUMP=0` to disable printing as needed.
