# asc_set_atomic_none (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:50:33.345Z pushedAt=2026-10-28T12:55:35.776Z nl=2 -->

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

**This API is deprecated. Use [asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md) instead.**

Used together with [asc_set_atomic_add](../atomic/datamove_atomic/asc_set_atomic_add.md), [asc_set_atomic_max](../atomic/datamove_atomic/asc_set_atomic_max.md), and [asc_set_atomic_min](../atomic/datamove_atomic/asc_set_atomic_min.md) to disable the atomic operation after it completes, preventing it from affecting subsequent data movement.

## Prototype

```c
[[deprecated("NOTICE: asc_set_atomic_none is deprecated. Please use asc_disable_dma_atomic instead.")]]
__aicore__ inline void asc_set_atomic_none()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

See [the example of asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md#examples).
