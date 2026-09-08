# Vector Multiply-Add Performance Tuning Sample

## Overview

Element-wise vector multiply-add is a basic vector operation that accumulates the element-wise products of two vectors onto a target vector.

This sample uses element-wise vector multiply-add as a vehicle to demonstrate how to perform instruction optimization with atomic add. Unlike the [atomic_histogram sample](../atomic_histogram/README_en.md), in this sample each thread accesses non-overlapping addresses, so **there is no same-address contention and atomicity itself is not a functional requirement**; using atomic add is a deliberately introduced optimization.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Software Version

- \>= CANN 9.1.0

## Directory Structure

```text
├── vector_muladd_atomic
│   ├── CMakeLists.txt         // Sample build script
│   ├── vector_muladd_atomic.asc       // Ascend C SIMT kernel implementation & Host invocation sample
│   ├── README.md
│   └── README_en.md
```

## Sample Description

This sample performs vector multiply-add on three float arrays, accumulating the element-wise products of `b` and `c` onto `a`.

- Formula:

  ```text
  a[i] += b[i] * c[i],  i = 0, 1, ..., element_count - 1
  ```

  The values of all three arrays are small integer multiples of negative powers of 2 (0.5/0.25/0.125), so the products and sums are exactly representable in float; therefore validation can use exact comparison without setting a floating-point tolerance.

- Sample Specifications:

  <table>
  <tr><td align="center">Sample Type (OpType)</td><td colspan="4" align="center">Atomic Optimization</td></tr>
  <tr><td rowspan="4" align="center">Sample Input</td><td align="center">name</td><td align="center">shape</td><td align="center">data type</td><td align="center">format</td></tr>
  <tr><td align="center">a</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">b</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td align="center">c</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Sample Output</td><td align="center">a</td><td align="center">[4194304]</td><td align="center">float</td><td align="center">ND</td></tr>
  <tr><td rowspan="1" align="center">Kernel Name</td><td colspan="4" align="center">vector_muladd_plain / vector_muladd_atomic</td></tr>
  </table>

## Sample Implementation

### Case Implementation Overview

This sample uses 2 Cases to form an optimization comparison. The kernel and accumulation method for each Case are shown in the table below.

| Case | Kernel | Computation Method |
|---|---|---|
| 0 | vector_muladd_plain | a[index] += b[index] * c[index] |
| 1 | vector_muladd_atomic | asc_atomic_add(&a[index], b[index] * c[index]) |

### Performance Metrics

| Metric | Description |
| --- | --- |
| Task Duration (μs) | Total Task time, including scheduling to the accelerator, execution time on the accelerator, and response completion time |

---

### Case 0: a[i] += b[i] * c[i]

**Sample Goal**: Each thread directly performs the `a[index] += b[index] * c[index]` computation, serving as the performance baseline.

**Core Implementation**: Launches 64 thread blocks (64 is the AIV core count actually queried at runtime by this code), each with 2048 threads. Each thread traverses the entire array with a fixed stride equal to the total number of threads `gridDim.x * blockDim.x`; for each element processed, it performs one multiply-add: compute `b[i] * c[i]`, accumulate onto the old value of `a[i]`, and write the result back to `a[i]`.

The thread must first read the old value of `a[i]` before it can participate in the accumulation.

The corresponding kernel implementation is as follows.

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_plain(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        a[index] += b[index] * c[index];
    }
}
```

**Performance Data**:

| Case | Task Duration (μs) |
|:---:|:---:|
| 0 | 51.70 |

---

### Case 1: asc_atomic_add(&a[i], b[i] * c[i])

**Sample Goal**: Use [asc_atomic_add()](../../../../../docs/zh/api/SIMT-API/atomic_operations/asc_atomic_add.md) to hand the multiply-add off to atomic add so that threads do not need to read the old value of `a[i]`, and verify the performance gain from issuing one fewer read instruction.

**Core Implementation**: The number of thread blocks, the number of threads, and the traversal method are exactly the same as Case 0. Each thread only computes the product `b[i] * c[i]`, then issues one atomic add on `a[i]`; the old value of `a[i]` does not need to be read.

> **Note:** In this Case, each thread accesses non-overlapping addresses, so there is no same-address contention. Atomic add is used not because atomicity is a functional requirement, but to leverage the property that atomic add "completes the accumulation in place without reading the old value", thereby reducing the loop body instruction count. This contrasts with the [atomic_histogram sample](../atomic_histogram/README_en.md): in the latter, atomicity is functionally required, and the GM atomic operations must be optimized away due to high contention; this sample, in contrast, deliberately introduces atomic operations as an optimization.

The corresponding kernel implementation is as follows.

```cpp
__global__ __launch_bounds__(THREADS_PER_BLOCK) void vector_muladd_atomic(
    float* a, const float* b, const float* c, uint32_t element_count)
{
    uint32_t index = blockIdx.x * blockDim.x + threadIdx.x;
    const uint32_t stride = gridDim.x * blockDim.x;
    for (; index < element_count; index += stride) {
        asc_atomic_add(&a[index], b[index] * c[index]);
    }
}
```

**Performance Data**:

| Case | Task Duration (μs) |
|:---:|:---:|
| 1 | 47.72 |

**Analysis**:

- **Read/write instruction count is reduced.** Case 0's loop body contains 3 reads and 1 write; Case 1 eliminates the read instruction for `a[i]`, containing 2 reads and 1 write.

**Conclusion**: By handing the accumulation off to atomic add, threads save the read instruction that reads the old value of `a[i]`, reducing latency by 7.7%. The gain comes from threads issuing one fewer read instruction.

---

## Performance Summary

| Case | Accumulation Method | Thread Reads | Task Duration (μs) | Duration Relative to Baseline |
|:---:|:---:|:---:|:---:|:---:|
| 0 | a[i] += b[i] * c[i] | 3 | 51.70 | 1× |
| 1 | asc_atomic_add | 2 | 47.72 | **0.92×** |

The latency reduction is 7.7%. Although the thread read count drops from 3 to 2 (a 33% reduction), the actual latency reduction is only 7.7%.

- **The latency reduction is far smaller than the read-count reduction because the dominant cost of reading GM is the data round-trip latency, not instruction issue time.** Issuing a read instruction only consumes a short issue time; afterward, most of the time is spent waiting for data to return from GM. The waits for the two read instructions overlap, so removing one saves only the issue time of that instruction; the data round-trip latency still has to be waited out.

## Tuning Recommendations

**Prefer atomic add for accumulation-type scenarios**: When the old value of the target address is used only for accumulation and does not participate in other computations, switching to atomic add saves the instruction that reads the old value, reducing the instruction count.

**Note the applicability precondition**: The gain of this optimization comes from threads issuing one fewer instruction, not from atomicity, nor from reducing the amount of data transferred. If the accessed addresses contend, atomic operations on the same address can only execute serially, which instead causes significant performance loss (see the [atomic_histogram sample](../atomic_histogram/README_en.md) and the [atomic_add_perf sample](../../../02_features/01_api_features/02_atomic_operation/atomic_add_perf/README_en.md) for details). Before use, confirm that the addresses each thread writes to `a` do not overlap.

## Build and Run

Run the following steps in the root directory of this sample to build and execute the sample.

- Configure Environment Variables

  Configure environment variables based on the [installation method](../../../../../docs/en/quick_start.md#prepare&install) of the CANN development kit in the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN package installation directory. When no installation directory is specified, the default installation path is `/usr/local/Ascend`.

- Run the Sample

  Run the following commands in this sample directory.

  ```bash
  mkdir -p build && cd build                                # Create and enter the build directory
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..               # Configure the project
  make -j                                                   # Build the sample
  ./vector_muladd_atomic 0                                  # Run Case 0 (baseline)
  ./vector_muladd_atomic 1                                  # Run Case 1 (optimization)
  ```

  Build Options Description:

  | Option | Values | Description |
  |------|--------|------|
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture: this sample only supports dav-3510 (Ascend 950PR/Ascend 950DT) |
  | `SKIP_VALIDATION` | `ON`/`OFF` | Whether to skip result validation, default `OFF`. Recommended to set to `ON` when collecting performance with `msopprof` |

  The following output indicates that the accuracy verification is successful.

  ```text
  Case 0 (plain update): 4194304 elements, 64 blocks, 2048 threads per block
  [Success] Case accuracy verification passed.
  ```

## Performance Analysis

Use the `msopprof` tool to obtain detailed performance data:

```bash
msopprof ./vector_muladd_atomic 0    # Collect Case 0 performance
msopprof ./vector_muladd_atomic 1    # Collect Case 1 performance
```

> **Regarding Validation failed during performance collection:** In this sample, the accumulation target `a` is initialized on the host side at allocation, and the kernel only accumulates on it. The `msopprof` warmup+replay re-executes the kernel on the same GM memory, causing `a` to be accumulated multiple times; therefore, strict validation mode reports `Validation failed`. This is an inherent conflict between the replay mechanism and the validation logic. When collecting performance, it is recommended to rebuild with `-DSKIP_VALIDATION=ON` before running `msopprof` to skip validation.

After the command completes, a folder named "OPPROF_{timestamp}_XXX" is generated in the default directory. The performance data folder structure is as follows:

```text
├──dump                       # Raw performance data, users do not need to focus on this
├──ArithmeticUtilization.csv  # cube/vector instruction cycle ratio
├──L2Cache.csv                # L2 Cache hit rate
├──Memory.csv                 # UB, L1, and main memory read/write bandwidth rates
├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth rates
├──MemoryUB.csv               # Vector and Scalar to UB read/write bandwidth rates
├──OpBasicInfo.csv            # Operator basic information
├──PipeUtilization.csv        # Compute unit and transfer unit time consumption and ratio
├──ResourceConflictRatio.csv  # UB bank group, bank conflict, and resource conflict ratio across all instructions
└──visualize_data.bin         # MindStudio Insight presentation file
```
