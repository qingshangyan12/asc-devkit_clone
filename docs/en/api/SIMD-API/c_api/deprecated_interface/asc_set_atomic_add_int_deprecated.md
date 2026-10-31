# asc_set_atomic_add_int (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:48:51.569Z pushedAt=2026-10-28T12:55:35.774Z nl=2 -->

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

**This API is deprecated. Use [asc_set_atomic_add_int32](../atomic/datamove_atomic/asc_set_atomic_add.md) instead.**

Enables atomic accumulation for subsequent data transfers from Unified Buffer (UB)/L0C Buffer/L1 Buffer to Global Memory. The accumulated data type is int32_t.

## Prototype

```c
[[deprecated("NOTICE: asc_set_atomic_add_int is deprecated. Please use asc_set_atomic_add_int32 instead.")]]
__aicore__ inline void asc_set_atomic_add_int()
```

## Parameters

None

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

- After use, clear the atomic operation state (see [asc_disable_dma_atomic](../atomic/datamove_atomic/asc_disable_dma_atomic.md)) to avoid affecting the functionality of subsequent related instructions.
- This instruction does not clear the GM data before execution. You need to manually add a clearing operation when required.

## Examples

See the examples in [asc_set_atomic_add](../atomic/datamove_atomic/asc_set_atomic_add.md).
