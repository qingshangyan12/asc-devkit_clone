# asc_copy_ub2gm_sync_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:24:25.906Z pushedAt=2026-10-28T12:55:18.864Z nl=2 -->

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

**`asc_copy_ub2gm_sync` API is deprecated. Use [the corresponding non-synchronous API](../vector_datamove/asc_copy_ub2gm/asc_copy_ub2gm_arch_3510.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies data from Unified Buffer (UB) to Global Memory (GM).

## Prototype

- Synchronous computation

```cpp
__aicore__ inline void asc_copy_ub2gm_sync(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Destination GM address. |
| src | Input | Source UB address. |
| size | Input | Size of the data to copy (in bytes). |

## Return Value

None

## Pipeline Type

PIPE_MTE3

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If the UB address of this instruction overlaps with that of other instructions, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the instructions and prevent abnormal data.
- Synchronous computation includes synchronous waiting.
- When the first n data copy APIs are used, the data size to be copied must be 32-byte aligned.
- When src_stride is not equal to burst_len, src_stride must be 32-byte aligned.

## Examples

```cpp
// total_length indicates the total length of the data to be copied. dst is the externally input half-type GM memory.
constexpr uint64_t total_length = 128;
__ubuf__ half src[total_length];
asc_copy_ub2gm_sync(dst, src, total_length * sizeof(half));
```
