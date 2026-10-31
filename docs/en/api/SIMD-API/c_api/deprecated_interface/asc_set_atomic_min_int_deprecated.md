# asc_set_atomic_min_int (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:50:15.805Z pushedAt=2026-10-28T12:55:35.778Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
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

The header file path is `"c_api/atomic/datamove_atomic.h"`.

**This API is deprecated. Use [asc_set_atomic_min_int32](../atomic/datamove_atomic/asc_set_atomic_min.md) instead.**

Sets the computation result to be transferred to GM by atomic comparison. Before the copy, the int32_t data to be transferred is compared element-wise with the existing data in GM, and the minimum value is written to GM.

## Prototype

```c
[[deprecated("NOTICE: asc_set_atomic_min_int is deprecated. Please use asc_set_atomic_min_int32 instead.")]]
__aicore__ inline void asc_set_atomic_min_int()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

After use, call [asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md) to disable the atomic minimum operation, so as not to affect the subsequent related instructions.

## Examples

See the examples in [asc_set_atomic_min](../atomic/datamove_atomic/asc_set_atomic_min.md).
