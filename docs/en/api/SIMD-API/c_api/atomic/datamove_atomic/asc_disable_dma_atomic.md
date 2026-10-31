# asc_disable_dma_atomic

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-14T08:39:45.583Z pushedAt=2026-10-19T12:13:56.590Z nl=2 -->

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

Used together with [asc_set_atomic_add](asc_set_atomic_add.md), [asc_set_atomic_max](asc_set_atomic_max.md), and [asc_set_atomic_min](asc_set_atomic_min.md) to disable the atomic operation after it completes, preventing it from affecting subsequent data movement.

## Prototype

```c
__aicore__ inline void asc_disable_dma_atomic()
```

## Parameters

None

## Return Value

None

## Pipeline Type

`PIPE_S`

## Constraints

None

## Examples

**asc_disable_dma_atomic** is used together with atomic operation APIs such as [**asc_set_atomic_add**](asc_set_atomic_add.md). For an example, see [Examples of asc_set_atomic_add](asc_set_atomic_add.md#examples).
