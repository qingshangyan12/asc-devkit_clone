# shfl_down

<!-- md-trans-meta sourceCommit=343a6e89d2704a866eb5b4d939aacaa8ebe1820b translatedAt=2026-10-27T09:32:12.243Z pushedAt=2026-10-27T11:36:23.223Z nl=2 -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/coalesced_group/shfl_down_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Obtains the data of the thread that is `delta` positions behind the current thread within the `coalesced_group`.

## Prototype

```cpp
template <typename T>
T shfl_down(T var, unsigned int delta) const
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| var | Input | Input operand used by the thread for exchange. Supported data types: half, int32_t, uint32_t, float, half2, int64_t, uint64_t, bfloat16_t, bfloat16x2_t. |
| delta | Input | Backward offset, relative to the current thread, of the thread whose `var` value is to be obtained within the group. |

## Return Value

Returns the `var` value input by the thread that is `delta` positions behind the current thread within the `coalesced_group`. If the offset exceeds the group range, returns the `var` value input by the current thread.

## Constraints

None

## Examples

In the conditional branch of the sample code, all threads with even thread IDs in a warp form a `coalesced_group`, and the result returned by the `shfl_down` interface for each thread in the group is shown in the following figure.

**Figure 1**  shfl_down result diagram  
![](../../../figures/coalesced_group_shfl_down.png "shfl_down result diagram")

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        if (threadIdx.x % 2 == 0) {
            coalesced_group active = coalesced_threads();
            uint32_t result = active.shfl_down(active.thread_rank() + 100, 2);
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
            uint32_t result = active.shfl_down(active.thread_rank() + 100, 2);
        }
        ...
    }
    ```
