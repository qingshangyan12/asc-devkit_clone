# shfl

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T09:33:56.600Z pushedAt=2026-10-27T11:36:23.225Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/coalesced_group/shfl_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

A data exchange interface for threads within a `coalesced_group`. It directly reads the value in the register of a specified thread in the group without using shared memory.

## Prototype

```cpp
template <typename T>
T shfl(T var, int src_rank) const
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| var | Input | Data used by the thread for exchange. Supported data types: half, int32_t, uint32_t, float, half2, int64_t, uint64_t, bfloat16_t, bfloat16x2_t. |
| src_rank | Input | Rank of the thread, within the group, that holds the desired `var` value. When `src_rank` is greater than or equal to the number of threads in the group, the `var` value of the thread whose rank is `src_rank % num_threads()` is obtained. |

## Return Value

Returns the `var` value input by the specified thread in the `coalesced_group`.

## Constraints

None

## Examples

In the sample code, the conditional branch groups all threads with even thread IDs in a warp into a `coalesced_group`. The result returned by the `shfl()` interface of each thread in the group is shown in the following figure.

**Figure 1**  shfl result diagram  
![](../../../figures/coalesced_group_shfl.png)

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        if (threadIdx.x % 2 == 0) {
            coalesced_group active = coalesced_threads();
            uint32_t result = active.shfl(active.thread_rank() + 100, 3);
        }
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
        if (threadIdx.x % 2 == 0) {
            coalesced_group active = coalesced_threads();
            uint32_t result = active.shfl(active.thread_rank() + 100, 3);
        }
        ...
    }
    ```
