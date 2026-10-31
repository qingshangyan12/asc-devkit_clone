# asc_get_icache_preload_status

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-21T02:54:26.166Z pushedAt=2026-10-28T12:21:43.266Z nl=2 -->

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

The header file path is `"c_api/cache_ctrl/cache_ctrl.h"`.

This is a debugging API. Call it after [asc_icache_preload](./asc_icache_preload.md) to obtain the ICache PreLoad status:

- A return value of 0 indicates that the ICache PreLoad is complete.
- A return value of 1 indicates that the ICache PreLoad is not complete.

## Prototype

```c
__aicore__ inline int64_t asc_get_icache_preload_status()
```

## Parameters

None

## Return Value

Returns the ICache prefetch status value of the `int64_t` type. Only the valid bits take the following values:

- Returns `0`: The ICache prefetch is idle, and the previous prefetch is complete.
- Returns `1`: The ICache prefetch is in progress and not yet complete.

## Pipeline Type

PIPE_S

## Constraints

This API is used only for status query in debugging and performance analysis scenarios, and does not affect the execution flow of the prefetch itself.

## Examples

This API must be used after calling [asc_icache_preload](./asc_icache_preload.md). For a complete call example, see [Examples of asc_icache_preload](./asc_icache_preload.md#examples).
