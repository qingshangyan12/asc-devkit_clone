# asc_get_ar_spr (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:38:30.920Z pushedAt=2026-10-28T12:55:35.632Z nl=2 -->

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

The header file path is `"c_api/spr/spr.h"`.

**This API is deprecated. Use [asc_get_squeeze_status](../spr/asc_get_squeeze_status.md) instead.**

Reads the value of a specified special register.

## Prototype

```cpp
__aicore__ inline int64_t asc_get_ar_spr()
```

## Parameters

None

## Return Value

Returns the value in the special register as an int64_t.

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

None
