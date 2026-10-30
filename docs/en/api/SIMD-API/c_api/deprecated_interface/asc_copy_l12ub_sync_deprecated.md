# asc_copy_l12ub_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:18:13.146Z pushedAt=2026-10-28T12:55:18.854Z nl=2 -->

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
- Atlas inference product AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference product Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

**The `asc_copy_l12ub_sync` API is deprecated. Use the [corresponding asynchronous API](../cube_datamove/asc_copy_l12ub.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Moves data from the L1 Buffer to the Unified Buffer (UB).

## Prototype

- Synchronous data movement
    ```cpp
    __aicore__ inline void asc_copy_l12ub_sync(__ubuf__ void* dst_addr, __cbuf__ void* src_addr, int8_t sub_blockid, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap, uint16_t dst_gap)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst_addr | Output | Start address of the destination operand. |
| src_addr | Input | Start address of the source operand. |
| sub_blockid | Input | Sub-block ID to use. |
| burst_count | Input | Number of consecutive data blocks to move. |
| burst_len | Input | Length of each consecutive data block to move. The unit is DataBlock (32 bytes). |
| src_gap | Input | Gap between adjacent consecutive data blocks of the source operand (the gap between the end of the preceding data block and the start of the following data block).<br>Unit: DataBlock (32 bytes). |
| dst_gap | Input | Gap between adjacent consecutive data blocks of the destination operand (the gap between the end of the preceding data block and the start of the following data block).<br>Unit: DataBlock (32 bytes). |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- Because this API moves data between the Cube core and the Vector core, if a kernel calls this API, it cannot use the `__cube__` or `__vector__` execution space modifier; otherwise, execution fails.

## Examples

```cpp
constexpr uint16_t burst_count = 1;
constexpr uint16_t burst_len = 1;
constexpr uint16_t src_gap = 0;
constexpr uint16_t dst_gap = 1;
__cbuf__ half src[256];
__ubuf__ half dst[256];
asc_copy_l12ub_sync(dst, src, false, burst_count, burst_len, src_gap, dst_gap);
```
