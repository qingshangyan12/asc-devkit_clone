# asc_set_l0c2gm_lrelu_alpha (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:34:00.178Z pushedAt=2026-10-28T12:55:49.480Z nl=2 -->

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

**`asc_set_l0c2gm_lrelu_alpha` API is deprecated. Use the [asc_set_l0c_copy_lrelu_alpha](../cube_datamove/asc_set_l0c_copy_lrelu_alpha.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Used to set the Leaky ReLU alpha value used during computation by the asc_copy_l0c2l1 or asc_copy_l0c2gm API. This value supports only the half and float data types.

## Prototype

```cpp
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config)
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config)
```

## Parameters

**Table 1** Parameter description

| config | Input | Leaky ReLU alpha value. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```cpp
half config = 0.5;
asc_set_l0c2gm_lrelu_alpha(config);
```
