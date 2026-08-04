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

**Operator Introduction**

`adaptive_max_pool3d_grad` scatters the gradient `grad` back to `gradInput` based on the `argmax` values saved during forward pooling, as shown in the following figure:

**Figure 1**  adaptive_max_pool3d_grad gradient scatter diagram
![](./figures/adaptive_max_pool3d_grad_function_overview.png)

The computation can be represented as:

```text
gradInput[nc, :] = 0                                             # Set all output positions to 0.
gradInput[nc, argmax[nc, od, oh, ow]] = grad[nc, od, oh, ow]     # Scatter each grad value to the position specified by argmax.
```

**Sample Specifications**

| Parameter | Shape | Format | Data Type | Description |
|:---|:---|:---|:---|:---|
| `gradInput` | Fixed to [1,4096,32,32,32] in this sample | NCDHW | FLOAT | Tensor to which gradients are scattered back. |
| `grad` | Default shape [1,4096,8,8,8] in this sample | NCDHW | FLOAT | Backward gradient value for each pooling window. |
| `argmax` | Same as `grad`, default shape [1,4096,8,8,8] in this sample | NCDHW | INT32 | Maximum-value index saved during forward pooling. Each value specifies the target position of one gradient. |

This operator consists of two steps: zeroing and scattering discrete data. Zeroing is a typical regular computation that suits SIMD, while the discrete gradient writes suit SIMT.

Note: To keep the sample simple and easy to understand, this implementation does not consider overlapping pooling windows, and the `gradInput` shape is fixed to [1,4096,32,32,32].

**Case Overview**

This sample implements the operator in four ways. The runtime argument selects the corresponding kernel implementation.

| Case | Runtime Argument | Implementation File | Thread Block Count | Threads per Thread Block | Comparison Purpose |
|---|---|---|---|---|---|
| 0 | `simd` | `adaptive_max_pool3d_grad_simd.h` | 64 | - | Show a pure SIMD implementation of the operator. |
| 1 | `scalar` | `adaptive_max_pool3d_grad_scalar.h` | 64 | - | Show an implementation that uses SIMD for zeroing and Scalar for the discrete writes. |
| 2 | `hybrid` | `adaptive_max_pool3d_grad_hybrid.h` | 64 | 512 | Show a hybrid approach that uses SIMD for contiguous zeroing and SIMT for discrete scatter. |
| 3 | `hybrid_ub` | `adaptive_max_pool3d_grad_hybrid_ub.h` | 64 | 512 | Show how Case 2 can be further improved by bringing UB into the picture. |

## Sample Implementation

The performance comparison uses `Task Duration(us)` reported by `msOpProf`. The data in this sample was collected on Ascend 950PR with 64 Vector Cores, an AI Vector frequency of 1650 MHz, and CANN 9.1.0, and the normal execution path was used to complete correctness verification.

The sample fixes `gradInput` to [1,4096,32,32,32], so each `NC` plane holds 32768 elements. Changing the D/H/W of `grad/argmax` (using the same value for all three) adjusts the pooling window size, which in turn controls the amount of discrete-write data (the D×H×W of `grad/argmax` is the **number of gradients scattered per `NC` plane**). The sample collects the latency of each implementation under different pooling window sizes.

### Case 0: SIMD API Zeroing and Gradient Scatter

**Implementation**

The key point of Case 0 is expressing the gradient scatter as a SIMD vector computation. The implementation approach is shown in the following figure.

**Figure 2**  Case 0 pure SIMD implementation approach
![](./figures/adaptive_max_pool3d_grad_simd_overview.png)

The three parts of the figure correspond to the three key points of the implementation:

- **Traversing each output position**: Each output position in `grad/argmax` has one gradient value and one `argmax` index. For each output position, the kernel derives the pooling window it covers on the input, and every input position `p` inside that window requires one round of vector computation. Matched positions are written with `grad` and unmatched positions are written with 0, so traversing the window also completes the zeroing of `gradInput`.
- **Constructing the SIMD vector**: Pure SIMD processes data in units of vectors, so this implementation treats the same position across 64 consecutive `NC` planes as one vector to process, where each SIMD lane corresponds to one `NC` plane. This sample splits `NC=4096` into 64 groups by `NC_TILE=64` and launches 64 Blocks, so each Block processes one group.
- **Compare-Select-Move**: The current input position `p` is copied to every lane of the vector, each lane checks whether `argmax` equals `p`, and then each lane selects one of two values to produce the `selected` vector. `selected` is stored contiguously by `NC` in UB, while in GM the same spatial position is separated by `input_spatial` across different `NC` planes. Therefore, before writing back, each value must first be reorganized into its own 32B slot in UB, and then stored to GM by using strided movement.

**Key Code**

```cpp
asc_duplicate_scalar(input_index_reg, static_cast<int32_t>(base_input_index + tile_index), active_mask);
asc_eq(cmp_mask, argmax_reg, input_index_reg, active_mask);
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

The pure SIMD implementation must enumerate every candidate position in the input pooling window and explicitly write 0 for unmatched input positions. When there are few gradients to scatter, each gradient covers a larger input window. When the gradient count increases, the number of `grad/argmax` loads and cross-`NC` write-backs also increases. Although this path is fully expressed with SIMD, it is not direct enough for index-based discrete writes, and its overall latency is much higher than the later implementations that use `argmax` directly for scatter.

**Conclusion**

The gradient scatter position is a discrete write. To use the SIMD API here, the code has to construct vectorized processing for the scatter path. This makes the code much more complex and difficult to write and read, and the performance is also poor, so pure SIMD is not well suited to this Scatter-style discrete write scenario.

> `adaptive_max_pool3d_grad_simd.h` in this sample is a simplified reference implementation. For a more complete implementation, see [adaptive_max_pool3d_grad_normal.h](https://gitcode.com/cann/ops-nn/blob/master/pooling/adaptive_max_pool3d_grad/op_kernel/adaptive_max_pool3d_grad_normal.h).

### Case 1: SIMD Zeroing + Scalar Gradient Scatter

**Implementation**

Case 1 first lets each Block own a complete `NC` slice. It uses SIMD to fill UB with zeros and continuously moves the zeros to the `gradInput` GM space of that slice for initialization. Then it moves `grad/argmax` into UB in chunks of `GRAD_TILE_COUNT=4096`, and a scalar loop directly computes the target input address and writes the gradient back to GM.

**Key Code**

```cpp
//...
__aicore__ inline void zero_input(__gm__ float* input_data, uint64_t count)
{
    __ubuf__ float zero_ub[ZERO_TILE_COUNT];
    asc_vf_call<fill_float_ub>(zero_ub, 0.0F, ZERO_TILE_COUNT);
    wait_v_to_mte3();

    for (uint64_t offset = 0; offset < count; offset += ZERO_TILE_COUNT) {
        const uint32_t copy_count = static_cast<uint32_t>(min<uint64_t>(ZERO_TILE_COUNT, count - offset));
        const uint32_t copy_bytes = copy_count * sizeof(float);
        asc_copy_ub2gm(input_data + offset, zero_ub, copy_bytes);
    }
    wait_mte3_to_scalar();
}

//...
for (uint32_t i = 0; i < tile_grad_count; ++i) {
    const uint64_t local_grad_index = static_cast<uint64_t>(grad_offset + i);
    const uint64_t local_nc_index = local_grad_index / output_spatial;
    const uint64_t target = local_nc_index * input_spatial + static_cast<uint64_t>(argmax_ub[i]);
    grad_input[target] = grad_ub[i];
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

Compared with Case 0, the zeroing step operates on contiguous memory, so Case 1 uses an API to move zeros from UB directly to GM, taking only 272.131us for global pooling. This shows that SIMD suits contiguous memory reads and writes very well. For the discrete gradient scatter, Case 1 no longer enumerates pooling windows and traverses every output position. Instead, it uses `argmax` directly to compute the target scatter address, which requires far less computation than Case 0 and therefore greatly reduces latency. However, as the number of gradients scattered per `NC` plane grows from 1 to 4096, the scalar loop count grows proportionally and becomes the main performance bottleneck.

**Conclusion**

In SIMD code, Main Scalar can directly handle discrete reads and writes. The code is much simpler than the vector construction used in Case 0, but the weakness is also clear: the parallelism is low, so latency increases as the amount of discrete write data grows.

> `adaptive_max_pool3d_grad_scalar.h` in this sample is a simplified reference implementation. For a more complete implementation, see [max_pool3d_grad_scatter_unified.h](https://gitcode.com/cann/ops-nn/blob/master/pooling/pool_3d_common/op_kernel/arch32/max_pool3d_grad_scatter_unified.h).

### Case 2: SIMD Zeroing + SIMT Gradient Scatter

**Implementation**

Case 2 keeps the SIMD zeroing approach from Case 1, but changes the gradient scatter stage to SIMT. Each Block still owns a non-overlapping `NC` slice, so zeroing and scatter only need ordering inside the Block and do not require cross-Block synchronization. The SIMT VF uses 512 threads to traverse the `grad/argmax` slice of the Block and directly writes `grad` to the GM position specified by `argmax`.

**Key Code**

```cpp
for (uint64_t local_grad_index = static_cast<uint64_t>(threadIdx.x);
     local_grad_index < block_grad_count;
     local_grad_index += static_cast<uint64_t>(blockDim.x)) {
    const uint64_t local_nc_index = static_cast<uint32_t>(local_grad_index) / output_spatial;
    const uint64_t input_index =
        local_nc_index * input_spatial + static_cast<uint64_t>(argmax[local_grad_index]);
    grad_input[input_index] = grad[local_grad_index];
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

For Scatter-style discrete reads and writes, SIMT multi-thread parallelism is more efficient than SIMD. The larger the amount of data to scatter, the greater the benefit.

### Case 3: Use UB to Further Improve Hybrid Programming Performance

**Implementation**

In Case 2, zeroing and gradient scatter are split into two independent steps, and the SIMT scatter stage writes directly back to GM. These two steps can be further fused: the kernel completes zeroing and gradient scatter in UB, and then moves the result to GM.

**Key Code**

```cpp
// Allocate a memory region in UB and initialize it to 0.
asc_vf_call<hybrid_ub_impl::fill_float_ub>(grad_input_ub, 0.0F, input_spatial);
// ... omitted code
// Scatter gradients for this data segment.
asc_vf_call<hybrid_ub_impl::scatter_grad<float, int32_t>>(
    dim3(hybrid_ub_impl::THREAD_COUNT), grad_ub, argmax_ub, grad_input_ub,
    input_spatial, output_spatial);
asc_copy_ub2gm_align(input_data + input_base_offset, grad_input_ub, active_input_bytes);
```

**Performance Data**

| D/H/W of `grad/argmax` | Gradients scattered per `NC` plane | Task Duration(us) |
|---:|---:|---:|
| 1 | 1 | 276.880 |
| 2 | 8 | 297.455 |
| 4 | 64 | 285.779 |
| 8 | 512 | 316.859 |
| 16 | 4096 | 454.797 |

**Performance Analysis**

When there are few gradients to scatter, there are also few discrete GM writes, and the extra cost of static UB caching plus full-plane write-back in Case 3 offsets the benefit. Therefore, the configurations that scatter 1 and 8 gradients are slightly slower than Case 2. As the gradient count grows, the path where SIMT performs discrete writes inside UB and MTE continuously writes back to GM becomes better. At 4096 gradients per `NC` plane, Case 3 reduces latency from 856.143us in Case 2 to 454.797us, achieving about 1.88x speedup.

**Conclusion**

When using hybrid programming, SIMD and SIMT performance optimization methods can be reused together, providing a higher optimization ceiling.

## Comparison Summary

The performance data of all cases is shown below:

| Gradients scattered per `NC` plane | Case 0 SIMD(us) | Case 1 Scalar(us) | Case 2 Hybrid(us) | Case 3 Hybrid UB(us) |
|---:|---:|---:|---:|---:|
| 1 | 6569.102 | 272.131 | 270.612 | 276.880 |
| 8 | 6699.820 | 298.759 | 282.933 | 297.455 |
| 64 | 6846.461 | 455.944 | 339.883 | 285.779 |
| 512 | 7745.685 | 2010.422 | 442.487 | 316.859 |
| 4096 | 13898.433 | 9140.783 | 856.143 | 454.797 |

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

  The commands above use the default `grad/argmax` shape [1,4096,8,8,8]. The `OUTPUT_DHW` option changes the D/H/W of `grad/argmax`, which lets you reproduce the other rows of the performance tables above. For example, to collect the row that scatters 4096 gradients per `NC` plane:

  ```bash
  mkdir -p build && cd build
  cmake -DCMAKE_ASC_ARCHITECTURES=dav-3510 -DOUTPUT_DHW=16 ..;make -j
  ./adaptive_max_pool3d_grad hybrid_ub
  ```

- Build option description.

  | Option | Value | Description |
  | ---------------- | ----------- | ----------------- |
  | `CMAKE_ASC_ARCHITECTURES` | `dav-3510` | NPU architecture. This sample supports only dav-3510 (Ascend 950PR/Ascend 950DT). |
  | `OUTPUT_DHW` | `1`, `2`, `4`, `8`, `16` | D/H/W of `grad/argmax`, using the same value for all three. Defaults to `8`. The number of gradients scattered per `NC` plane is the cube of this value. Use it to reproduce each row of the performance comparison tables. |

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
