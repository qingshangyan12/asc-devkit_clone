# ballot

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:20:37.882Z pushedAt=2026-10-20T03:02:40.161Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/thread_block_tile/ballot_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Determines whether the input of each thread in the `thread_block_tile` group is non-zero.

## Prototype

```cpp
unsigned int ballot(int predicate) const
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| predicate | Input | Operand. |

## Return Value

A 32-bit unsigned integer: if the `predicate` input by a thread in the `thread_block_tile` group is not 0, the bit corresponding to the thread's rank in the return value is 1; otherwise, it is 0.

## Constraints

Only the scenario where the template parameter `Size` of `thread_block_tile` is less than or equal to 32 is supported, that is, only thread subgroups within a single warp are supported.

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        ...
        thread_block block = this_thread_block();
        auto tile4 = tiled_partition<4>(block);
        uint32_t result = tile4.ballot(1);          // Returns 0xf.
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
        uint32_t result = tile4.ballot(1);          // Returns 0xf.
        ...
    }
    ```
