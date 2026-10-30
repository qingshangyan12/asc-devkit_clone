# asc_copy_ub2l1_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:24:17.552Z pushedAt=2026-10-28T12:55:18.863Z nl=2 -->

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

The header file path is `"c_api/composite/vector_compute_composite.h"`.

**The `asc_copy_ub2l1_sync` API is deprecated. Use [asc_copy_ub2l1](../vector_datamove/asc_copy_ub2l1.md) and the synchronization API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies a contiguous block of data from the Unified Buffer (UB) to the L1 Buffer and performs synchronization after the copy is complete.

This API takes effect only on the AIV.

## Prototype

```cpp
__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination L1 Buffer. |
| src | Input | Start address of the source UB. |
| size | Input | Size of the data to copy, in bytes. |

## Return Value

None

## Pipeline Type

PIPE_MTE3

## Constraints

- This API returns directly when it is called on a non-AIV.
- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- Synchronization computation includes synchronization waiting.

## Examples

```cpp
constexpr uint32_t size = 32;
__ubuf__ uint8_t src[size];
__cbuf__ uint8_t dst[size];
asc_copy_ub2l1_sync(dst, src, size);
```
