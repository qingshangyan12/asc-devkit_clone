# Scalar Atomic Operations Example

## Overview

This example introduces scalar atomic addition and scalar atomic compare-and-swap on Global Memory (GM) addresses through the [`AtomicAdd`](../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md) and [`AtomicCas`](../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicCas.md) APIs. Atomic operations ensure consistency between concurrent atomic updates to the same address.

Scalar atomic operations use the scalar pipeline (PIPE_S), while GM-to-UB [`DataCopy`](../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md) uses the transfer pipeline (PIPE_MTE2). When they access the same GM address with a data dependency, the compiler does not insert synchronization automatically. Scenario 3 demonstrates the required intra-core synchronization with [`SetFlag/WaitFlag`](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md) and [`PipeBarrier`](../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md).

> **API tip:** In addition to the `AtomicAdd` and `AtomicCas` APIs used in this example, Ascend C also provides `AtomicExch`, `AtomicMax`, and `AtomicMin` APIs. The calling method is the same as `AtomicAdd`; simply replace the function name to switch.

## Supported Products and CANN Versions

| Product | CANN Version |
|---------|-------------|
| Ascend 950PR&950DT products | >= CANN 9.1.0 |

## Directory Structure

```
├── scalar_atomic_operations
│   ├── scripts
│   │   ├── gen_data.py               // Input data and ground truth generation script
│   │   └── verify_result.py          // Verification script to check output data against ground truth
│   ├── CMakeLists.txt                // Build configuration file
│   ├── data_utils.h                  // Data read/write functions
│   ├── scalar_atomic_operations.asc  // Ascend C example implementation & invocation example
│   └── README.md                     // Example description document
```

## Detailed Scenario Description

This example switches between different scenarios through the build parameter `SCENARIO_NUM`:

**Scenario 1: AtomicAdd Atomic Addition**
  Three cores execute in parallel, each atomically adding 1 to the first element of GM (dst[0]). The final value of dst[0] is the initial value plus 3.

**Scenario 2: AtomicCas Atomic Compare-and-Swap**
  Three cores execute in parallel, each checking whether the first element of GM equals the expected value 1. If equal, it is replaced with newValue 2; if not equal, no modification is made.

**Scenario 3: Intra-Core Synchronization for Scalar Atomic Operation and Data Movement**
  Only one core is launched to execute `AtomicAdd`. After the atomic operation, `SetFlag<HardEvent::S_MTE2>` and `WaitFlag<HardEvent::S_MTE2>` establish the dependency from the scalar pipeline to the GM-to-UB transfer pipeline. `DataCopy` then reads the GM data into UB, and `PipeBarrier<PIPE_ALL>` waits for previously issued pipeline operations to complete. This scenario demonstrates the intra-core synchronization constraint without introducing inter-core access competition.

<table border="2">
<caption>Table 1: Scenario Configuration Comparison</caption>
<tr><th>scenarioNum</th><th>Atomic Operation API</th><th>Output Shape</th><th>Data Type</th><th>Description</th></tr>
<tr><td>1</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md">AtomicAdd</a></td><td>[1, 256]</td><td>int32</td><td>Three cores perform atomic addition of 1 on the first GM element in parallel</td></tr>
<tr><td>2</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicCas.md">AtomicCas</a></td><td>[1, 256]</td><td>uint32</td><td>Three cores perform atomic compare-and-swap on the first GM element in parallel (if value is 1, replace with 2)</td></tr>
<tr><td>3</td><td><a href="../../../../../docs/zh/api/SIMD-API/basic_api/atomic_operations/AtomicAdd.md">AtomicAdd</a>, <a href="../../../../../docs/zh/api/SIMD-API/basic_api/memory_vector_compute/data_move/DataCopy_GMAndUB_continuous.md">DataCopy</a>, <a href="../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/SetFlag_WaitFlag_ISASI.md">SetFlag/WaitFlag</a>, <a href="../../../../../docs/zh/api/SIMD-API/basic_api/sync_control/intra_core_sync/PipeBarrier_ISASI.md">PipeBarrier</a></td><td>[1, 256]</td><td>int32</td><td>One core performs atomic addition and demonstrates intra-core synchronization with GM-to-UB data movement</td></tr>
</table>

## Build and Run

Run the following steps in the root directory of this example to build and run it.
- Configure environment variables
  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit on the current environment.
  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the example

  Run the following commands in the example directory.
  ```bash
  SCENARIO_NUM=1  # Set the scenario number (values: 1, 2, 3)
  mkdir -p build && cd build;      # Create and enter the build directory
  cmake .. -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j;    # Build the project
  python3 ../scripts/gen_data.py -scenarioNum $SCENARIO_NUM   # Generate test input data
  ./demo                           # Run the compiled executable to execute the example
  python3 ../scripts/verify_result.py output/output.bin output/golden.bin -scenarioNum $SCENARIO_NUM  # Verify whether the output result is correct
  ```

  To use CPU debug or NPU simulation mode, add the `-DCMAKE_ASC_RUN_MODE=cpu` or `-DCMAKE_ASC_RUN_MODE=sim` parameter.

  Examples:

  ```bash
  cmake .. -DCMAKE_ASC_RUN_MODE=cpu -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # CPU debug mode
  cmake .. -DCMAKE_ASC_RUN_MODE=sim -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DSCENARIO_NUM=$SCENARIO_NUM;make -j; # NPU simulation mode
  ```

  > **Notice:** Clear the cmake cache before switching build modes. Run `rm CMakeCache.txt` in the build directory and re-run cmake.

- Build option description

  | Option | Values | Description |
  |--------|--------|-------------|
  | `CMAKE_ASC_RUN_MODE` | `npu` (default), `cpu`, `sim` | Run mode: NPU execution, CPU debug, NPU simulation |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` (default) | NPU architecture: AtomicAdd and AtomicCas only support dav-3510 (corresponding to Ascend 950PR&950DT products) |
  | `SCENARIO_NUM` | `1` (default), `2`, `3` | Scenario number: 1 (multi-core AtomicAdd), 2 (multi-core AtomicCas), 3 (single-core AtomicAdd with data-movement synchronization) |

- Execution result

  The following execution result indicates that the precision comparison is successful.

  ```bash
  test pass!
  ```
