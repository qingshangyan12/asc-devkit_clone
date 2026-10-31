# asc_set_store_atomic_config_v2 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:38:17.858Z pushedAt=2026-10-28T12:55:49.491Z nl=2 -->

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

The header file path is `"c_api/atomic/scalar_atomic.h"`.

**This API is deprecated. Use [asc_atomic_add](../atomic/scalar_atomic/asc_atomic_add.md) for atomic add operations.**

Sets the atomic operation enable bit and the atomic operation type value.

## Prototype

```c
[[deprecated("NOTICE: asc_set_store_atomic_config_v2 is deprecated."
             "Please use asc_atomic_add instead for atomic add operation.")]]
__aicore__ inline void asc_set_store_atomic_config_v2(uint16_t type, uint16_t op)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| type | Input | Atomic operation enable bit.<br>1: Enable atomic operations, and the data type of the operation data is int32_t.<br>4: Enable atomic operations, and the data type of the operation data is half.<br>5: Enable atomic operations, and the data type of the operation data is float.<br>7: Enable atomic operations, and the data type of the operation data is bfloat16_t.<br>9: Enable atomic operations, and the data type of the operation data is int16_t.<br>10: Enable atomic operations, and the data type of the operation data is int8_t.<br>Other values have no specific meaning. |
| op | Input | Atomic operation type.<br>2: Sum operation.<br>Other values have no specific meaning. |

## Return Value

None

## Pipeline Type

PIPE_S

## Constraints

None

## Examples

```c
uint16_t type = 1;
uint16_t op = 2;
asc_set_store_atomic_config_v2(type, op);
```
