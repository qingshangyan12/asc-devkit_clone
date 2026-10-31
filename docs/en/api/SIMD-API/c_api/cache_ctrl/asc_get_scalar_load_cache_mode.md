# asc_get_scalar_load_cache_mode

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-21T02:56:18.867Z pushedAt=2026-10-28T12:21:43.272Z nl=2 -->

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

The header file path is `"c_api/cache_ctrl/cache_ctrl.h"`.

Reads the L2 cache management policy of the current scalar load path and returns an [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md) enumeration value. This value can be set through the [asc_set_scalar_cache_mode](asc_set_scalar_cache_mode.md) API. For details about the policy, see that API.

It is commonly used to save the original configuration before temporarily switching the policy, or to perform readback verification after setting.

## Prototype

```c
__aicore__ inline asc_load_l2_cache_mode asc_get_scalar_load_cache_mode()
```

## Parameters

None

## Return Value

The L2 cache management policy of the current scalar load path, of type [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md).

## Pipeline Type

PIPE_S

## Constraints

This API reflects only the scalar load-side policy and is independent of the scalar store side and `l2_cache_mode` in the vector movement API parameters.

## Examples

This API is used to save the current policy of the scalar load path. For a complete call example, see [Scenario A of asc_set_scalar_cache_mode](asc_set_scalar_cache_mode.md#asc_set_scalar_cache_mode_scenario_a).
