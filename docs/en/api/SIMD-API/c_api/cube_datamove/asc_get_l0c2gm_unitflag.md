# asc_get_l0c2gm_unitflag

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-22T13:18:04.946Z pushedAt=2026-10-28T12:45:45.460Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
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

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

When in-line quantization is performed during data movement, call this API to obtain the unit_flag setting.

## Prototype

```cpp
__aicore__ inline uint64_t asc_get_l0c2gm_unitflag()
```

## Parameters

None

## Return Value

unit_flag setting. unit_flag enables fine-grained parallelism between cube computation instructions and cube movement instructions. When this feature is enabled, the hardware moves out the computation result as soon as it finishes computing each fractal. This feature does not apply to L0C Buffer accumulation scenarios.

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```cpp
uint64_t unitflag_value = asc_get_l0c2gm_unitflag();
```
