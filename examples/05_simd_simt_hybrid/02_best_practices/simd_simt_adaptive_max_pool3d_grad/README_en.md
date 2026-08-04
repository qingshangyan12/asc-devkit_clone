# adaptive_max_pool3d_grad Operator Sample Implemented with SIMD and SIMT Hybrid Programming

## Overview

This sample uses the adaptive_max_pool3d_grad operator to compare the implementation complexity and performance of SIMD and hybrid programming approaches. It shows that hybrid programming can provide both good performance and a better development experience when contiguous and discrete memory accesses coexist.

## Supported Products

- Ascend 950PR/Ascend 950DT

## Supported CANN Version

- CANN >= 9.1.0

## Directory Structure

```text
├── simd_simt_adaptive_max_pool3d_grad
│   ├── figures                              // Image resources for README.
│   ├── adaptive_max_pool3d_grad_common.h    // Pipeline sync-wait helpers shared by the four cases.
│   ├── adaptive_max_pool3d_grad_hybrid_ub.h // SIMD zeroing and SIMT static-UB scatter implementation.
│   ├── adaptive_max_pool3d_grad_hybrid.h    // SIMD zeroing and SIMT GM scatter implementation.
│   ├── adaptive_max_pool3d_grad_scalar.h    // SIMD zeroing and scalar GM scatter implementation.
│   ├── adaptive_max_pool3d_grad_simd.h      // SIMD vector API implementation for zeroing and scatter.
│   ├── adaptive_max_pool3d_grad_host.asc    // Unified main entry and host-side execution logic.
│   ├── CMakeLists.txt                       // Build project file.
│   ├── README.md                            // Chinese sample documentation.
│   └── README_en.md                         // English sample documentation.
```

## Sample Description

**Operator Introduction:**

`adaptive_max_pool3d_grad` scatters the gradient `grad` to the corresponding positions of `output` based on the indices given by `argmax`, and fills 0 at the positions not covered by `argmax`, as shown in the following figure:

**Figure 1**  adaptive_max_pool3d_grad function diagram
![](./figures/adaptive_max_pool3d_grad_function_overview.png)

The computation can be represented as:

```text
output[nc, :] = 0                                       # Set all output positions to 0.
output[nc, argmax[nc, d, h, w]] = grad[nc, d, h, w]     # Scatter each grad value to the position specified by argmax.
```

**Sample Specifications:**

| Parameter | Shape | Format | Data Type | Description |
|:---|:---|:---|:---|:---|
| `output` | [1,4096,32,32,32] | NCDHW | FLOAT | Tensor that stores the computation result. |
| `grad` | [1,4096,N,N,N] | NCDHW | FLOAT | Holds the gradient values. In this sample, N can be 1/2/4/8/16. |
| `argmax` | [1,4096,N,N,N] | NCDHW | INT32 | Holds the index in `output` where each gradient of `grad` is written. In this sample the values range from 0 to 32767, and the shape matches `grad`. |

This operator consists of two steps: zeroing and scattering discrete data. Zeroing is a typical regular computation that suits SIMD, while the discrete gradient writes suit SIMT.

**Case Overview:**

This sample implements the operator in four ways, so as to compare the performance differences between implementations in a scenario that involves both discrete and regular computation.

| Case | Runtime Argument | Implementation File | Thread Block Count | Threads per Thread Block | Comparison Purpose |
|---|---|---|---|---|---|
| 0 | `simd` | `adaptive_max_pool3d_grad_simd.h` | 64 | - | Show a pure SIMD implementation of the operator. |
| 1 | `scalar` | `adaptive_max_pool3d_grad_scalar.h` | 64 | - | Show an implementation that uses SIMD for zeroing and Scalar for the discrete writes. |
| 2 | `hybrid` | `adaptive_max_pool3d_grad_hybrid.h` | 64 | 512 | Show a hybrid approach that uses SIMD for contiguous zeroing and SIMT for discrete scatter. |
| 3 | `hybrid_ub` | `adaptive_max_pool3d_grad_hybrid_ub.h` | 64 | 512 | Show how Case 2 can be further improved by bringing UB into the picture. |

## Sample Implementation

The sample fixes `output` to [1,4096,32,32,32], so each `NC` plane holds 32\*32\*32=32768 elements. All cases launch 64 Blocks and each Block handles the zeroing and gradient scatter of 1\*4096/64=64 **consecutive** planes, so the Blocks do not affect each other and no inter-core synchronization is needed. The D/H/W of `grad/argmax` (using the same value for all three) ranges over 1/2/4/8/16, so there is no overlap between the regions occupied by the gradients, which means the gradients can be written back with a plain assignment instead of an accumulation. The sample collects the latency of each implementation under different gradient counts.

**Performance Metrics**

| Column | Description |
|:---|:---|
| D/H/W of `grad/argmax` | Value of the D, H, and W dimensions of `grad` and `argmax`. All three are the same and are configured by the `GRAD_DHW` build option, which accepts 1/2/4/8/16. |
| Gradients scattered per `NC` plane | Number of gradients to scatter on each `NC` plane at this configuration. It equals the D×H×W of `grad/argmax` and measures the amount of discrete-write data. |
| Task Duration(us) | Overall Task latency reported by `msOpProf`, including the time to dispatch to the accelerator, the execution time on the accelerator, and the response completion time. The performance comparison in this sample is based on this column. |

### Case 0: SIMD API Zeroing and Gradient Scatter

**Implementation**

SIMD operates on vectors rather than individual elements. The vectors that Case 0 chooses to operate on are shown in the following figure:

**Figure 2**  Case 0 pure SIMD vector construction approach
![](./figures/adaptive_max_pool3d_grad_simd_overview.png)

The key steps of the pure SIMD implementation are as follows:

- **Traversing each gradient**: Each gradient in `grad/argmax` has one value and one `argmax` index. For each gradient value g and its corresponding coordinate a, the kernel derives the region g occupies in `output` and traverses every position `p` inside that region. If p equals a, that position is filled with g; otherwise it is filled with 0.
- **Constructing the SIMD vector**: Pure SIMD processes data in units of vectors, so this implementation treats the same position `p` across 64 consecutive `NC` planes in output as one vector to process, output_index_reg. The vectors grad_reg and argmax_reg are obtained in the same way, and then the SIMD API is used to compute the final result through the Compare-Select-Move steps.

**Key Code**

```cpp
asc_duplicate_scalar(output_index_reg, static_cast<int32_t>(base_output_index + tile_index), active_mask);
asc_eq(cmp_mask, argmax_reg, output_index_reg, active_mask);
asc_select(selected_reg, grad_reg, zero_reg, cmp_mask);
asc_storealign(selected_tile + tile_index * NC_TILE, selected_reg, active_mask);
```

**Performance Data**

| D/H/W of `grad/argmax` | Gradients scattered per `NC` plane | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 6569.102 |
| 2 | 8 | 6699.820 |
| 4 | 64 | 6846.461 |
| 8 | 512 | 7745.685 |
| 16 | 4096 | 13898.433 |

**Performance Analysis**

Under the implementation of Case 0, every position in `output` has to go through steps such as compare, select, scatter, and move. The amount of computation is very large, and the result is high latency.

The last row (4096 gradients), where latency nearly doubles to 13898.433us, is caused by the regions becoming too fragmented. At this point the region is only 2 elements wide along W, so the number of `select_w_tile` calls doubles while each call performs only 2 comparisons internally. The total comparison count is still unchanged, but every call has to pay the fixed overhead again: updating the mask, generating the zero vector, and two aligned loads. The more fragmented the regions, the less opportunity there is to amortize that fixed overhead, and it eventually accumulates into a new bottleneck. The same effect is already present in the first four rows, but at a small enough scale that the constant main workload masks it.

**Conclusion**

The gradient scatter position is a discrete write. To use the SIMD API here, the code constructs vectors and implements the discrete write in a mask-select manner. This makes the code much more complex and difficult to write and read, and the performance is also poor, so pure SIMD is not well suited to this Scatter-style discrete write scenario.

> `adaptive_max_pool3d_grad_simd.h` in this sample is a simplified version of the commercial operator implementation. For the complete implementation, see [adaptive_max_pool3d_grad_normal.h](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/adaptive_max_pool3d_grad_normal.h).

### Case 1: SIMD Zeroing + Scalar Gradient Scatter

**Implementation**

Case 1 splits zeroing and gradient scatter into two stages. Within each Block, SIMD first fills all the `NC` planes it owns with zeros, and then it traverses `grad/argmax` and uses a scalar loop to directly compute the target address and write in the gradient value.

**Key Code**

```cpp
//...
__aicore__ inline void zero_output(__gm__ float* output_data, uint64_t count)
{
    __ubuf__ float zero_ub[ZERO_TILE_COUNT];
    asc_vf_call<fill_float_ub>(zero_ub, 0.0F, ZERO_TILE_COUNT);
    wait_v_to_mte3();

    for (uint64_t offset = 0; offset < count; offset += ZERO_TILE_COUNT) {
        const uint32_t copy_count = static_cast<uint32_t>(min<uint64_t>(ZERO_TILE_COUNT, count - offset));
        const uint32_t copy_bytes = copy_count * sizeof(float);
        asc_copy_ub2gm(output_data + offset, zero_ub, copy_bytes);
    }
    wait_mte3_to_scalar();
}

//...
for (uint32_t i = 0; i < tile_grad_count; ++i) {
    const uint64_t local_grad_index = static_cast<uint64_t>(grad_offset + i);
    const uint64_t local_nc_index = local_grad_index / grad_spatial;
    const uint64_t target = local_nc_index * output_spatial + static_cast<uint64_t>(argmax_ub[i]);
    output[target] = grad_ub[i];
}
```

**Performance Data**

| D/H/W of `grad/argmax` | Gradients scattered per `NC` plane | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 272.131 |
| 2 | 8 | 298.759 |
| 4 | 64 | 455.944 |
| 8 | 512 | 2010.422 |
| 16 | 4096 | 9140.783 |

**Performance Analysis**

Compared with Case 0, the zeroing step operates on contiguous memory, so Case 1 uses a SIMD API to move zeros from UB directly to GM, taking only 272.131us when each `NC` plane scatters a single gradient. This shows that SIMD suits contiguous memory reads and writes very well. For the discrete gradient scatter, Case 1 no longer enumerates each gradient's region and traverses every position inside it. Instead, it uses `argmax` directly to compute the target scatter address, which requires far less computation than Case 0 and therefore greatly reduces latency. However, as the number of gradients scattered per `NC` plane grows from 1 to 4096, the scalar loop count grows proportionally and becomes the main performance bottleneck.

**Conclusion**

In SIMD code, Main Scalar can directly handle discrete reads and writes. The code is much simpler than the vector construction used in Case 0, but the weakness is also clear: the parallelism is low, so latency increases as the amount of discrete write data grows.

> `adaptive_max_pool3d_grad_scalar.h` in this sample is a simplified version of the commercial operator implementation. For the complete implementation, see [max_pool3d_grad_scatter_unified.h](https://gitcode.com/cann/ops-nn/blob/master/pooling/pool_3d_common/op_kernel/arch32/max_pool3d_grad_scatter_unified.h).

### Case 2: SIMD Zeroing + SIMT Gradient Scatter

**Implementation**

Case 2 keeps the SIMD zeroing approach from Case 1, but changes the gradient scatter stage to SIMT, where each thread handles the computation of one (or several) gradient values.

**Key Code**

```cpp
__simt_vf__ __launch_bounds__(THREAD_COUNT) inline void scatter_grad(
    const __gm__ float* grad, const __gm__ int32_t* argmax, __gm__ float* output, uint32_t block_nc_count, uint32_t output_spatial, uint32_t grad_spatial)
{
    const uint64_t block_grad_count = static_cast<uint64_t>(block_nc_count) * grad_spatial;
    for (uint64_t local_grad_index = static_cast<uint64_t>(threadIdx.x); local_grad_index < block_grad_count;
         local_grad_index += static_cast<uint64_t>(blockDim.x)) {
        const uint64_t local_nc_index = static_cast<uint32_t>(local_grad_index) / grad_spatial;
        const uint64_t output_index = local_nc_index * output_spatial + static_cast<uint64_t>(argmax[local_grad_index]);
        output[output_index] = grad[local_grad_index];
    }
}
```

**Performance Data**

| D/H/W of `grad/argmax` | Gradients scattered per `NC` plane | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 270.612 |
| 2 | 8 | 282.933 |
| 4 | 64 | 339.883 |
| 8 | 512 | 442.487 |
| 16 | 4096 | 856.143 |

**Performance Analysis**

Case 2 parallelizes the scalar scatter loop in Case 1, and the benefit becomes more significant as the number of gradients to scatter increases. At 4096 gradients per `NC` plane, Case 2 reduces latency from 9140.783us in Case 1 to 856.143us, achieving about 10.68x speedup.

**Conclusion**

For Scatter-style discrete reads and writes, SIMT multi-thread parallelism is more efficient than Main Scalar. The larger the amount of data to scatter, the greater the benefit.

### Case 3: Use UB to Further Improve Hybrid Programming Performance

**Implementation**

Building on the hybrid programming of Case 2, the common SIMT optimization techniques still apply. Here the two stages are merged into one: the kernel completes zeroing and gradient scatter in UB, and then moves the result to GM.

**Key Code**

```cpp
// Allocate a memory region in UB and initialize it to 0.
asc_vf_call<hybrid_ub_impl::fill_float_ub>(output_ub, 0.0F, output_spatial);
// ... omitted code
// Scatter gradients for this data segment.
asc_vf_call<hybrid_ub_impl::scatter_grad<float, int32_t>>(
    dim3(hybrid_ub_impl::THREAD_COUNT), grad_data + grad_base_offset,
    argmax_data + grad_base_offset, output_ub, output_spatial, grad_spatial);
asc_copy_ub2gm_align(output_data + output_base_offset, output_ub, active_output_bytes);
```

**Performance Data**

| D/H/W of `grad/argmax` | Gradients scattered per `NC` plane | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 277.865 |
| 2 | 8 | 280.293 |
| 4 | 64 | 290.998 |
| 8 | 512 | 342.587 |
| 16 | 4096 | 696.464 |

**Performance Analysis**

When there are few gradients to scatter, there are also few discrete GM writes, and the extra cost of static UB caching plus full-plane write-back in Case 3 offsets the benefit. Therefore, the configuration that scatters 1 gradient is roughly on par with Case 2. As the gradient count grows, the path where SIMT performs discrete writes inside UB and MTE continuously writes back to GM becomes better. At 4096 gradients per `NC` plane, Case 3 reduces latency from 856.143us in Case 2 to 696.464us, achieving about 1.23x speedup.

**Conclusion**

When using hybrid programming, SIMD and SIMT performance optimization methods can be reused together, providing a higher optimization ceiling.

## Comparison Summary

The performance data of all cases is shown below:

| Gradients scattered per `NC` plane | Case 0 SIMD(us) | Case 1 Scalar(us) | Case 2 Hybrid(us) | Case 3 Hybrid UB(us) |
|---:|---:|---:|---:|---:|
| 1 | 6569.102 | 272.131 | 270.612 | 277.865 |
| 8 | 6699.820 | 298.759 | 282.933 | 280.293 |
| 64 | 6846.461 | 455.944 | 339.883 | 290.998 |
| 512 | 7745.685 | 2010.422 | 442.487 | 342.587 |
| 4096 | 13898.433 | 9140.783 | 856.143 | 696.464 |

This sample contains two parts: contiguous data processing for zeroing and discrete data processing for gradient scatter. Based on the performance data above and the four kernel implementations, this scenario is more suitable for SIMD and SIMT hybrid programming. Hybrid programming provides a better development experience while achieving good performance.

> Data zeroing can also be implemented with SIMT, but its performance is lower than the hybrid version. For the implementation, see [adaptive_max_pool3d_grad_simt.h](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/arch35/adaptive_max_pool3d_grad_simt.h).

## Build and Run

Run the following steps in the root directory of this sample to build and execute it.

- Configure environment variables.

  Configure environment variables based on the [CANN development kit installation method](../../../../docs/en/quick_start.md#prepare&install) of the current environment.

  ```bash
  source ${install_path}/cann/set_env.sh
  ```

  > **Note:** `${install_path}` is the CANN installation directory. If no installation directory is specified, the default directory is `/usr/local/Ascend`.

- Run the sample.

  Run the following commands in this sample directory.

  ```bash
  mkdir -p build && cd build                           # Create and enter the build directory.
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 ..;make -j  # Build the project.
  ./adaptive_max_pool3d_grad simd                      # Run the sample with the simd kernel implementation.
  ./adaptive_max_pool3d_grad scalar                    # Run the sample with the scalar kernel implementation.
  ./adaptive_max_pool3d_grad hybrid                    # Run the sample with the hybrid kernel implementation.
  ./adaptive_max_pool3d_grad hybrid_ub                 # Run the sample with the hybrid_ub kernel implementation.
  ```

  The commands above use the default `grad/argmax` shape [1,4096,8,8,8]. The `GRAD_DHW` option changes the D/H/W of `grad/argmax`, which lets you reproduce the other rows of the performance tables above. For example, to collect the row that scatters 4096 gradients per `NC` plane:

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DGRAD_DHW=16 ..;make -j
  ./adaptive_max_pool3d_grad hybrid_ub
  ```

- Build option description.

  | Option | Value | Description |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT). |
  | `GRAD_DHW` | `1`, `2`, `4`, `8`, `16` | D/H/W of `grad/argmax`, using the same value for all three. Defaults to `8`. The number of gradients scattered per `NC` plane is the D×H×W of `grad/argmax`. Use it to reproduce each row of the performance comparison tables. |

- Runtime parameter description.

  The command format of the sample executable is as follows:

  ```bash
  ./adaptive_max_pool3d_grad <implementation> [profile]
  ```

  | Parameter | Value | Mandatory | Description |
  | --------- | ----- | --------- | ----------- |
  | `implementation` | `simd`, `scalar`, `hybrid`, `hybrid_ub` | Yes | Selects the kernel implementation version to run. |
  | `profile` | `profile` | No | Performance analysis mode parameter. After this parameter is specified, the sample only executes the operator and skips result accuracy comparison, which facilitates performance data collection with msOpProf. Performance collection may affect computation results. |

- Execution result.

  The following output indicates that the accuracy comparison succeeds.

  ```text
  test pass!
  ```

## Performance Analysis

### msOpProf Tool Introduction

msOpProf is a single-operator performance analysis tool. It provides two usage modes: `msopprof` and `msopprof simulator`. This tool helps users locate issues in operator memory access, operator code, and instructions, enabling comprehensive operator tuning. It currently supports performance data collection and automatic parsing in different run modes (on-device or simulation) and for different file forms (executable file or operator binary `.o` file).

- On-device performance collection.

  On-device performance collection directly measures the runtime of an operator on an Ascend AI Processor. This method is suitable for quickly locating operator performance issues in an on-device environment.

  Use `msOpProf` to collect detailed performance data:

  ```bash
  msopprof ./adaptive_max_pool3d_grad simd profile   # Analyze performance.
  ```

  - Performance data description.

    After the command is complete, a directory named in the `OPPROF_{timestamp}_XXX` format is generated in the default directory. The performance data directory structure is shown below:

    ```bash
    ├──dump                       # Raw performance data. Users do not need to inspect it.
    ├──ArithmeticUtilization.csv  # Cube/vector instruction cycle ratio.
    ├──L2Cache.csv                # L2 Cache hit rate, which affects MTE2. Plan data movement properly to improve the hit rate.
    ├──Memory.csv                 # UB, L1, and main memory read/write bandwidth.
    ├──MemoryL0.csv               # L0A, L0B, and L0C read/write bandwidth.
    ├──MemoryUB.csv               # Vector and Scalar read/write bandwidth to UB.
    ├──OpBasicInfo.csv            # Basic operator information.
    ├──PipeUtilization.csv        # Compute unit and movement unit latency and ratio.
    ├──ResourceConflictRatio.csv  # Ratio of UB bank group, bank conflict, and resource conflict events among all instructions.
    └──visualize_data.bin         # MindStudio Insight presentation file.
    ```

View the detailed performance analysis result:

```bash
# View Task Duration and other data.
cat ./OPPROF_*/OpBasicInfo.csv
```
