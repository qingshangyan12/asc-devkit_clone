# asc_set_l13d_padding (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:37:41.869Z pushedAt=2026-10-28T12:55:49.489Z nl=2 -->

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

**The `asc_set_l13d_padding` API is deprecated. Use the [asc_set_l12l0a_3d_padding](../cube_datamove/asc_set_l12l0a_3d_padding.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

Sets the Pad attribute description, which is used to configure the padding value when calling the [asc_copy_l12l0a](../cube_datamove/asc_copy_l12l0a/asc_copy_l12l0a.md) API.

## Prototype

```cpp
__aicore__ inline void asc_set_l13d_padding(uint64_t config)
__aicore__ inline void asc_set_l13d_padding(half config)
__aicore__ inline void asc_set_l13d_padding(int16_t config)
__aicore__ inline void asc_set_l13d_padding(uint16_t config)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|------------|------------|-----------|
| config | Input | Padding value. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```cpp
constexpr uint64_t config = 0;
asc_set_l13d_padding(config);
```
