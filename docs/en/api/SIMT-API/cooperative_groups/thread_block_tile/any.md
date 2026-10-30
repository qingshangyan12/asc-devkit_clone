# any

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:20:26.824Z pushedAt=2026-10-20T03:02:40.160Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/thread_block_tile/any_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Determines whether any thread in the `thread_block_tile` group has a non-zero input.

## Prototype

```cpp
int any(int predicate) const
```

## Parameters

**Table 1**  Parameters

| Name | Input/Output | Description |
| --- | --- | --- |
| predicate | Input | Operand. |

## Return Value

Returns 0 if the inputs of all threads in the `thread_block_tile` group are 0; otherwise, returns 1.

## Constraints

- When the template parameter `Size` of `thread_block_tile` is greater than 32, that is, when `Size` is 64, 128, 256, 512, 1024, or 2048 in a cross-Warp cooperative group scenario, all threads in the `thread_block_tile` group must be active; otherwise, the behavior is undefined.

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
        int result = tile4.any(1);                  // Returns 1.
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
        int result = tile4.any(1);                  // Returns 1.
        ...
    }
    ```
