# thread_rank

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T10:50:39.712Z pushedAt=2026-10-16T10:53:40.529Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/coalesced_group/thread_rank_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Obtains the rank of the current thread within the `coalesced_group`, where the rank starts from 0.

## Prototype

```cpp
unsigned int thread_rank() const
```

## Parameters

None

## Return Value

The rank of the current thread within the `coalesced_group`.

## Constraints

None

## Examples

In the sample code, the conditional branch groups all threads with even thread IDs in a warp into a `coalesced_group`. The result returned by the `thread_rank` interface for each thread in the group is shown in the following figure.

**Figure 1**  Rank of each thread in the coalesced_group  
![](../../../figures/coalesced_group_rank.png "coalesced_group_rank")

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        if (threadIdx.x % 2 == 0) {
            coalesced_group active = coalesced_threads();
            unsigned int rank = active.thread_rank();
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
            unsigned int rank = active.thread_rank();
        }
        ...
    }
    ```
