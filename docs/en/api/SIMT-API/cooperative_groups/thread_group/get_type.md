# get_type

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-16T11:29:41.583Z pushedAt=2026-10-20T03:02:40.192Z -->

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
<!-- @ref: asc-devkit/res/docs/zh/api/SIMT-API/cooperative_groups/thread_group/get_type_res.md#id1 -->

## Description

The header file path is `"simt_api/cooperative_groups.h"`.

Obtains the cooperative group type represented by the `thread_group` object.

## Prototype

```cpp
group_type get_type() const
```

## Parameters

None

## Return Value

Returns the cooperative group type. The possible enum values are as follows:

| Value | Description |
| --- | --- |
| `group_type::thread_block_type` | thread_block cooperative group. |
| `group_type::tiled_group_type` | thread_block_tile cooperative group. |
| `group_type::coalesced_group_type` | coalesced_group cooperative group. |
| `group_type::grid_group_type` | grid_group cooperative group, supported only in SIMT programming scenarios. |

## Constraints

None

## Examples

- SIMT programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __global__ void simt_kernel(...)
    {
        thread_group tile = tiled_partition(this_thread_block(), 16);
        group_type type = tile.get_type(); // Returns group_type::tiled_group_type.
        ...
    }
    ```

- SIMD and SIMT hybrid programming scenario:

    ```cpp
    #include "simt_api/cooperative_groups.h"

    using namespace cooperative_groups;
    __simt_vf__ inline void simt_kernel(...)
    {
        thread_group tile = tiled_partition(this_thread_block(), 16);
        group_type type = tile.get_type(); // Returns group_type::tiled_group_type.
        ...
    }
    ```
