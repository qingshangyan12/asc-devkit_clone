# Add High-Performance Sample

## Overview

This sample implements float16 element-wise Add with the Ascend C C API. It provides scenarios with Ping/Pong Buffer optimization disabled and enabled. Both scenarios use the same inputs, core count, tiling, and vector arithmetic so that the effect of overlapping GM/UB data movement with Vector computation can be compared directly.

## Supported Products and CANN Versions

**Table 1**  Supported Products and CANN Versions

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
|-- c_api_add.asc            # Kernel and host implementation
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

**Table 2**  Input and Output Data Specifications

| Data | Input/Output | Data file | Data type | Shape | Description |
| --- | --- | --- | --- | --- | --- |
| x | Input | `input/input_x.bin` | float16 | `[8192, 8192]` | Randomly generated in the range `[1, 100)`. |
| y | Input | `input/input_y.bin` | float16 | `[8192, 8192]` | Randomly generated in the range `[1, 100)`. |
| z | Output | `output/output.bin` | float16 | `[8192, 8192]` | Result computed by the Kernel |
| golden | Golden data | `output/golden.bin` | float16 | `[8192, 8192]` | Generated using `(x + y).astype(np.float16)`. |

Both scenarios use the same multicore partitioning and tile size. The matrix is evenly partitioned along the M axis across 64 cores. Each core processes 128 contiguous rows, or 1048576 elements, in 49 iterations: 48 full-tile iterations followed by one tail iteration.

**Table 3**  Multicore Partitioning and Tile Parameters

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

**Table 4**  Scenario Configuration

| `SCENARIO_NUM` | Kernel | Scenario | UB buffer configuration | UB address space | Pipeline behavior |
| --- | --- | --- | --- | --- | --- |
| 0 | `add_baseline` | Ping/Pong Buffer optimization disabled | One x/y/z buffer group | `3 * 21840 * sizeof(half)` = 131040 bytes | Completes load, compute, and store before processing the next tile. |
| 1 | `add_double_buffer` | Ping/Pong Buffer optimization enabled | Ping/Pong x/y/z buffer groups | `6 * 21840 * sizeof(half)` = 262080 bytes | MTE2, Vector, and MTE3 process different tiles with cross-iteration overlap. |

### Scenario 0: Ping/Pong Buffer Optimization Disabled

When Ping/Pong Buffer optimization is disabled, the Kernel `add_baseline` allocates only one x/y/z buffer group. The three buffers are contiguous in UB. Each buffer reserves 21840 float16 elements, while the current iteration accesses only the valid elements specified by `current_length`.

**Table 5**  UB Layout with Ping/Pong Buffer Optimization Disabled

| UB buffer | Start offset (float16 elements) | Valid elements in a full tile | Purpose |
| --- | --- | --- | --- |
| `x_local` | 0 | 21760 | Stores x loaded from GM for the current iteration. |
| `y_local` | 21840 | 21760 | Stores y loaded from GM for the current iteration. |
| `z_local` | 43680 | 21760 | Stores the Add result for the current iteration. |

Each iteration processes one tile in the following order:

1. The MTE2 pipeline calls `asc_copy_gm2ub_align` twice to load the current x and y tiles from GM to UB.
2. The `MTE2 -> V` synchronization ensures that both inputs have been loaded before the Vector pipeline calls `add_vf`. A full tile executes 170 `asc_add` operations in groups of 128 float16 elements, while the tail tile executes 32 operations.
3. The `V -> MTE3` synchronization ensures that computation has finished before the MTE3 pipeline calls `asc_copy_ub2gm_align` to store z from UB to GM.
4. At the end of every iteration, `asc_sync_pipe(PIPE_ALL)` waits for MTE2, Vector, and MTE3 to finish before the next iteration can overwrite the same UB addresses.

Therefore, while the Vector pipeline is computing iteration n, MTE2 cannot load the inputs for iteration n+1. Similarly, while MTE3 is storing the result of iteration n, computation for iteration n+1 cannot begin. The pipelines follow their dependencies within each iteration but cannot overlap across iterations. This scenario provides the performance baseline with Ping/Pong Buffer optimization disabled.

### Scenario 1: Ping/Pong Buffer Optimization Enabled

When Ping/Pong Buffer optimization is enabled, the Kernel `add_double_buffer` divides UB into two independent x/y/z buffer groups. Even-numbered iterations use Ping with `EVENT_ID0`, while odd-numbered iterations use Pong with `EVENT_ID1`. Adjacent iterations therefore do not access the same UB addresses.

**Table 6**  Buffer Layout with Ping/Pong Buffer Optimization Enabled

| Iteration | Buffer group | Event ID | x start offset | y start offset | z start offset |
| --- | --- | --- | --- | --- | --- |
| Even | Ping | `EVENT_ID0` | 0 | 21840 | 43680 |
| Odd | Pong | `EVENT_ID1` | 65520 | 87360 | 109200 |

All offsets in the table are measured in float16 elements. Each group still reserves 21840 float16 elements for each of x, y, and z, so the data movement and Vector workload per iteration are identical to those with Ping/Pong Buffer optimization disabled. When the optimization is enabled, the Kernel does not perform a full pipeline synchronization at the end of each iteration. Instead, `asc_sync_notify` and `asc_sync_wait` manage the producer-consumer dependencies of each buffer group independently.

**Table 7**  Synchronization Dependencies with Ping/Pong Buffer Optimization Enabled

| Synchronization direction | Enforced dependency | Buffer reuse condition |
| --- | --- | --- |
| `MTE2 -> V` | Vector can read x and y only after they have been loaded. | Prevents Vector from reading incomplete input data. |
| `V -> MTE2` | MTE2 can load subsequent data into an input group only after Vector has finished reading x and y. | Prevents MTE2 from overwriting inputs still in use. |
| `V -> MTE3` | MTE3 can store z only after Vector has finished writing the result. | Prevents MTE3 from storing an incomplete result. |
| `MTE3 -> V` | Vector can write to an output group again only after MTE3 has finished storing z. | Prevents Vector from overwriting a result still being stored. |

Before entering the loop, the Kernel marks both input and output buffer groups as available. During the loop, each iteration waits only for the state associated with its Event ID. As soon as Vector finishes reading one x/y group, it notifies MTE2 that the input buffers can be reused without waiting for z in the same group to be stored. After all 49 iterations have been issued, the Kernel waits for both Event ID groups and calls `asc_sync_pipe(PIPE_ALL)` once to ensure that the final data movement and computation have completed.

For example, iteration 0 uses Ping, iteration 1 uses Pong, and iteration 2 reuses Ping. While Vector processes the Ping data for iteration 0, MTE2 can load the Pong data for iteration 1. While MTE3 stores the Ping result for iteration 0, Vector can process the Pong data for iteration 1. Once Vector has finished reading the Ping inputs for iteration 0, MTE2 can refill the Ping input buffers for iteration 2. MTE2, Vector, and MTE3 can therefore process different iterations concurrently and hide part of the GM-to-UB and UB-to-GM data movement latency.

The main differences between the two scenarios are summarized below.

**Table 8**  Ping/Pong Buffer Optimization Comparison

| Comparison | Scenario 0: Ping/Pong Buffer optimization disabled | Scenario 1: Ping/Pong Buffer optimization enabled |
| --- | --- | --- |
| UB buffers | One x/y/z group, 131040 bytes | Two x/y/z groups, 262080 bytes |
| Addresses used by adjacent iterations | The same; reuse waits for the previous iteration to finish. | Different; alternates between Ping and Pong. |
| End-of-iteration synchronization | Calls `asc_sync_pipe(PIPE_ALL)` in every iteration. | Uses two Event ID groups for local dependencies and performs one full synchronization after all iterations. |
| Cross-iteration pipeline overlap | Not supported | MTE2, Vector, and MTE3 can process different iterations. |
| Optimization cost | Lower UB usage | Doubles the UB address space to enable pipeline overlap. |

<p align="center">
  <img src="figures/double_buffer.png" width="50%">
</p>

## Performance Comparison

The scenarios differ only in buffer count and synchronization. Build `SCENARIO_NUM=0` and `SCENARIO_NUM=1` separately and collect the Kernel Task Duration with a profiling tool. Use the duration with Ping/Pong Buffer optimization disabled as the baseline:

```text
performance improvement = duration with optimization disabled / duration with optimization enabled - 1
```

The benefit depends on the balance between data movement and computation. Ping/Pong Buffer optimization generally helps most when these costs are comparable; a workload limited entirely by GM bandwidth may see less improvement.

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
./c_api_add_example
python3 ../scripts/verify_result.py output/output.bin output/golden.bin
```

Set `SCENARIO_NUM=1` and rerun CMake, build, execution, and verification to test the scenario with Ping/Pong Buffer optimization enabled. Clear the CMake cache before switching scenarios.

**Table 9**  Build Options

| Build option | Values | Description |
| --- | --- | --- |
| `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture |
| `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode |
| `SCENARIO_NUM` | `0`, `1` (default) | Ping/Pong Buffer optimization disabled or enabled |

A successful verification prints:

```text
test pass!
```
