# Add High-Performance Sample

## Overview

This sample implements float16 element-wise Add with the Ascend C C API and compares single buffering with Ping/Pong double buffering. The double-buffer implementation alternates between two UB buffer groups so that input loading, Vector computation, and output storing can overlap across iterations.

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

Both scenarios use the same core partitioning and tile parameters. The matrix is split along the M axis across 64 cores, with each core processing 128 contiguous rows. Table3 shows the calculations.

**Table3**  Multicore Partitioning and Tile Parameters

| Parameter | Value | Description |
| --- | --- | --- |
| Total matrix elements | 67108864 | `8192 * 8192` |
| Number of cores | 64 | Each core processes a contiguous range along the M axis. |
| Rows per core | 128 | `8192 / 64` |
| Elements per core | 1048576 | `128 * 8192` |
| Reserved length of each UB buffer | 21840 float16 elements | Corresponds to `MAX_DATA_COPY_LENGTH` and defines the UB address range of each buffer. |
| Valid elements in a full tile | 21760 | Corresponds to `DATA_COPY_LENGTH`. |
| Valid elements in the tail tile | 4096 | `1048576 % 21760` |
| Iterations per core | 49 | `1048576 / 21760 = 48` with 4096 elements remaining, giving 48 full tiles and one tail tile. |
| VF vector length | 256 bytes | Processes 128 float16 elements at a time; a full tile uses 170 VF iterations and the tail tile uses 32. |

The `SCENARIO_NUM` build option selects single buffering or Ping/Pong double buffering. The two implementations differ only in the number of UB buffers and their synchronization; all other compute parameters are the same.

**Table4**  Scenario Configuration

| `SCENARIO_NUM` | Kernel | Scenario | UB buffer configuration | UB address space | Pipeline behavior |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | Ping/Pong Buffer optimization disabled | One x/y/z buffer group | `3 * 21840 * sizeof(half)` = 131040 bytes | Completes load, compute, and store before processing the next tile. |
| 1 | `add_double_buffer` | Ping/Pong Buffer optimization enabled | Ping/Pong x/y/z buffer groups | `6 * 21840 * sizeof(half)` = 262080 bytes | MTE2, Vector, and MTE3 process different tiles with cross-iteration overlap. |

### Scenario 0: Ping/Pong Buffer Optimization Disabled

`add_baseline` allocates one contiguous x/y/z buffer group. Each buffer reserves 21840 float16 elements, and `current_length` specifies the number of valid elements accessed in the current iteration.

**Table5**  UB Layout with Ping/Pong Buffer Optimization Disabled

| UB buffer | Start offset (float16 elements) | Valid elements in a full tile | Purpose |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | Stores x loaded from GM for the current iteration. |
| `y_local` | 21840 | 21760 | Stores y loaded from GM for the current iteration. |
| `z_local` | 43680 | 21760 | Stores the Add result for the current iteration. |

Each iteration processes one tile in the following order:

1. MTE2 calls `asc_copy_gm2ub_align` twice to load the current x and y tiles from GM to UB.
2. After MTE2 releases Mutex 0, Vector acquires it and calls `add_vf`. A full tile executes 170 `asc_add` operations, while the tail tile executes 32.
3. Vector releases Mutex 0, after which MTE3 acquires it and calls `asc_copy_ub2gm_align` to store z in GM.
4. MTE3 releases Mutex 0 before MTE2 can overwrite the same UB addresses in the next iteration. After the loop, `asc_sync_pipe(PIPE_ALL)` waits for all operations to finish.

All three stages share one Mutex and run in the order MTE2, Vector, and MTE3. The next iteration cannot reuse UB until the current store finishes, so iterations do not overlap. This implementation serves as the performance baseline.

### Scenario 1: Ping/Pong Buffer Optimization Enabled

`add_double_buffer` allocates Ping and Pong x/y/z buffer groups in UB. Even iterations use Ping and odd iterations use Pong. Each group has two Mutexes: an input Mutex protects the loading and reading of x/y, and an output Mutex protects the writing and storing of z.

**Table6**  Buffer Layout with Ping/Pong Buffer Optimization Enabled

| Iteration | Buffer group | Input Mutex ID | Output Mutex ID | x start offset | y start offset | z start offset |
| --- | --- | --- | --- | --- | --- | --- |
| Even | Ping | 0 | 2 | 0 | 21840 | 43680 |
| Odd | Pong | 1 | 3 | 65520 | 87360 | 109200 |

Offsets are measured in float16 elements, and each buffer reserves 21840 elements. Double buffering does not change the amount of data moved or computed per iteration; `asc_lock` and `asc_unlock` only control when each buffer group can be reused.

**Table7**  Synchronization Dependencies with Ping/Pong Buffer Optimization Enabled

| Pipeline stage | Mutex ID | Synchronization operation | Enforced dependency |
| --- | --- | --- | --- |
| MTE2 load | Input Mutex: 0 for Ping, 1 for Pong | Acquire the input Mutex, load x and y, and then release it. | Before rewriting x and y in a group, wait for the preceding Vector stage to finish reading them. |
| Vector computation | Input and output Mutexes for the current group | Acquire both Mutexes, call `add_vf`, and then release both. | Wait for the current x/y load and for the preceding MTE3 stage to finish reading z in the same group. |
| MTE3 store | Output Mutex: 2 for Ping, 3 for Pong | Acquire the output Mutex, store z, and then release it. | Wait for the current Vector stage to finish writing z. |

`loop_idx & 1` selects Ping or Pong. Input Mutexes are 0/1, and the corresponding output Mutexes are 2/3. Once Vector has finished reading x/y, MTE2 can load the next tile into the same group's input buffers. MTE3 independently waits on the output Mutex for z to be ready. Because MTE2 does not access z and MTE3 does not access x/y, these transfers can run concurrently. After all 49 iterations have been issued, one `asc_sync_pipe(PIPE_ALL)` completes the final synchronization. See [asc_lock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_lock.md) and [asc_unlock](../../../../../docs/zh/api/SIMD-API/c_api/sync/asc_unlock.md) for API details.

For example, iteration 0 uses Ping with Mutexes 0/2, iteration 1 uses Pong with 1/3, and iteration 2 returns to Ping with 0/2. Once iteration 0 Vector has finished reading x/y, iteration 2 MTE2 can overwrite Ping x/y while iteration 0 MTE3 is still reading Ping z. Iteration 2 Vector must still wait for that MTE3 store to finish before writing Ping z.

Table8 summarizes the main differences between the two implementations.

**Table8**  Ping/Pong Buffer Optimization Comparison

| Comparison | Scenario 0: Ping/Pong Buffer optimization disabled | Scenario 1: Ping/Pong Buffer optimization enabled |
| --- | --- | --- |
| UB buffers | One x/y/z group, 131040 bytes | Two x/y/z groups, 262080 bytes |
| Addresses used by adjacent iterations | The same; reuse waits for the previous iteration to finish. | Different; alternates between Ping and Pong. |
| Pipeline synchronization | Uses Mutex 0 to chain iterations and performs one full synchronization after all iterations. | Uses input Mutexes 0/1 and output Mutexes 2/3 for Ping/Pong, then performs one full synchronization after all iterations. |
| Cross-iteration pipeline overlap | Not supported | MTE2, Vector, and MTE3 can process different iterations; MTE2 loads and MTE3 stores for the same group can also overlap. |
| Optimization cost | Lower UB usage | Doubles the UB address space to enable pipeline overlap. |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

The diagram shows how MTE2, Vector, and MTE3 overlap while processing adjacent tiles.

## Performance Comparison

The two scenarios change only the buffer count and synchronization. For a valid comparison, use the same hardware, CANN version, input data, and system load, and verify accuracy before collecting performance data.

### Test Conditions

**Table9**  Performance Test Configuration

| Configuration | Value |
| --- | --- |
| Chip version | Ascend 950PR |
| CANN version | ANN 9.2.0 |
| NPU architecture | `dav-3510` |
| Input/output data type | float16 |
| Input/output shape | `[8192, 8192]` |
| AI Vector Cores | 64 |
| Tile length | 21760 float16 elements; the tail tile contains 4096 float16 elements |
| Number of measurements | Warm up each scenario once, collect at least five valid runs, and report the median Task Duration |

Keep the device frequency and power mode unchanged during testing, and ensure that no other task occupies the NPU. Use the same input data, core count, tiling parameters, and build options for both scenarios.

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

Ping/Pong double buffering overlaps the pipelines, so their durations or ratios cannot be added directly. Use `Task Duration` for the end-to-end comparison and the other metrics to identify where time is spent.

### Ascend 950PR Performance Comparison

Table11 shows the measurements collected on the Ascend 950PR.

**Table11**  Ping/Pong Buffer Optimization Performance Comparison

| Scenario | Configuration | Task Duration(μs) | aiv_time(μs) | aiv_vec_time(μs) | aiv_vec_ratio | aiv_scalar_time(μs) | aiv_scalar_ratio | aiv_mte2_time(μs) | aiv_mte2_ratio | aiv_mte3_time(μs) | aiv_mte3_ratio |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| Scenario 0 | Ping/Pong Buffer optimization disabled | 273.59 | 272.31 | 11.225 | 0.041 | 1.739 | 0.006 | 190.77 | 0.701 | 66.739 | 0.245 |
| Scenario 1 | Ping/Pong Buffer optimization enabled | 267.841 | 266.36 | 10.958 | 0.041 | 2.153 | 0.008 | 262.328 | 0.985 | 145.807 | 0.547 |

**End-to-End Result**:

Using Scenario 0 as the baseline, the speedup after enabling Ping/Pong Buffer optimization is

$$
S = \frac{T_{\text{baseline}}}{T_{\text{ping-pong}}}
  = \frac{273.59}{267.841}
  \approx 1.021
$$

The end-to-end duration reduction is

$$
R = \frac{T_{\text{baseline}} - T_{\text{ping-pong}}}{T_{\text{baseline}}} \times 100\%
  = \frac{273.59 - 267.841}{273.59} \times 100\%
  \approx 2.10\%
$$

`Task Duration` decreases from 273.59μs to 267.841μs, a reduction of 5.749μs or 2.10%. `aiv_time` also decreases by 5.95μs, following the same trend as the end-to-end duration.

The two `aiv_vec_time` values are 11.225μs and 10.958μs, a difference of 0.267μs, so Vector compute time is essentially unchanged. `aiv_scalar_time` increases by 0.414μs but remains below 1% of the total duration and has little effect on end-to-end performance.

**Theoretical Performance Analysis**:

At 128 half elements per cycle, a frequency of 1.65GHz, and 64 AI Vector Cores, the ideal Vector duration for Add is

$$
T_{\text{theory}}
= \frac{M \times N}{128 \times 1.65 \times 10^9 \times \text{core count}}
= \frac{8192 \times 8192}{128 \times 1.65 \times 10^9 \times 64}
\approx 4.965\ \mu s
$$

The theoretical value includes only the Add instruction and excludes VF loop control, address calculation, data movement, and synchronization. The measured `aiv_vec_time` is about 11μs, or 2.2 times the theoretical value, but is still much shorter than MTE2. Pure Vector computation is therefore not the main cost.

**Pipeline and Bandwidth Analysis**:

In Scenario 0, the MTE2, MTE3, and Vector ratios sum to 98.7%, so the three stages run mostly serially. In Scenario 1, they sum to 157.3%, showing that loading, computation, and storing overlap across iterations.

The MTE2 and MTE3 durations reported by the profiler include both execution and wait time. Double buffering changes the overlap between pipelines, so a higher value does not mean that an individual transfer became slower. The following estimates provide additional context.

Scenario 0 runs its read and write pipelines mostly serially. Its read bandwidth is estimated from the total input volume:

$$
D_{\text{read}} = M \times N \times sizeof(half) \times 2 = 268435456\ \text{B}
$$

$$
BW_{\text{read}} = \frac{D_{\text{read}}}{T_{mte2}}
= \frac{268435456}{190.77 \times 10^{-6}}
\approx 1.407\ \text{TB/s}
$$

In Scenario 1, MTE2 and MTE3 run concurrently. Its mixed read-write bandwidth is estimated from the total input and output volume:

$$
D_{\text{write}} = M \times N \times sizeof(half) = 134217728\ \text{B}
$$

$$
BW_{\text{rw}} = \frac{D_{\text{read}} + D_{\text{write}}}{T_{mte2}}
= \frac{402653184}{262.328 \times 10^{-6}}
\approx 1.535\ \text{TB/s}
$$

Under these definitions, Scenario 0 reaches an estimated read bandwidth of 1.407TB/s, while Scenario 1 reaches an estimated mixed read-write bandwidth of 1.535TB/s. Because the definitions differ, these values describe data movement in each scenario and should not be treated as a direct optimization ratio.

Scenario 1 has an `aiv_mte2_ratio` of 98.5%, so GM data movement remains the main cost. Vector computation is short, which limits how much wait time double buffering can hide; the end-to-end duration decreases by 2.10%.

#### Conclusion

Ping/Pong double buffering increases UB usage from 131040 bytes to 262080 bytes and allows MTE2, Vector, and MTE3 to overlap across iterations. In this test, `Task Duration` decreases by 5.749μs, or 2.10%.

GM data movement still accounts for most of the execution time, while Vector computation is relatively short. Further optimization should focus on tile length, contiguous GM access, and the L2 Cache policy.

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

The `printf` interface provides formatted output for SIMD and SIMT debugging. It can print the core index, tiling parameters, or intermediate state from Kernel code. Include `utils/debug/asc_printf.h` before using the [printf](../../../../../docs/zh/api/Utils-API/tuning_interface/printf.md) interface.

This sample uses the C API. Call [asc_get_block_idx](../../../../../docs/zh/api/SIMD-API/c_api/sys_var/asc_get_block_idx.md) to obtain the current core index instead of using the C++ API `AscendC::GetBlockIdx`. For example:

```cpp
#include "utils/debug/asc_printf.h"

__vector__ __global__ void add_debug()
{
    printf("add blockIdx=%ld\n", asc_get_block_idx());
}
```

> **Note:** The `printf` interface increases Kernel execution time and affects performance measurements. Use it only during functional debugging. Remove or disable printing before performance collection, or set `ASCENDC_DUMP=0` to disable printing as needed.
