# shfl_xor

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:24:23.607Z pushedAt=2026-10-20T03:02:40.175Z -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 inference products: Not supported
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas inference products AI Core: Not supported
- Atlas inference products Vector Core: Not supported
<!-- end id5 -->
<!-- npu="910" id6 -->
- Atlas training products: Not supported
<!-- end id6 -->
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/thread_block_tile/shfl_xor_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Obtains the `var` value input by the thread whose rank is the result of a bitwise XOR operation between the current thread's rank and `lane_mask`.

## Prototype

```cpp
template <typename T>
T shfl_xor(T var, unsigned int lane_mask) const
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| var | Input | Input operand used by the thread for exchange. Supported data types: half, int32_t, uint32_t, float, half2, int64_t, uint64_t, bfloat16_t, bfloat16x2_t. |
| lane_mask | Input | Operand for the XOR operation with the current thread's rank. |

## Return Value

The `var` value of the specified thread within the `thread_block_tile` group.

## Constraints

- Only the scenario where the template parameter `Size` of `thread_block_tile` is less than or equal to 32 is supported, that is, only thread subgroups within a single warp are supported.
- `lane_mask` must be less than the number of threads in the `thread_block_tile` group.

## Examples

Divide the thread block into groups of four threads, and obtain the `var` value input by the thread whose rank is the result of a bitwise XOR operation between the current thread's rank and `lane_mask` within the group.

**Figure 1**  Diagram of the return result of the shfl_xor API  
![](../../../figures/thread_block_tile_shfl_xor.png "thread_block_tile_shfl_xor")

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        thread_block block = this_thread_block();
        auto tile4 = tiled_partition<4>(block);
        uint32_t result = tile4.shfl_xor(threadIdx.x + 100, 1);
        ...
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __simt_vf__ inline void simt_kernel(...)
    {
        ...
        thread_block block = this_thread_block();
        auto tile4 = tiled_partition<4>(block);
        uint32_t result = tile4.shfl_xor(threadIdx.x + 100, 1);
        ...
    }
    ```
