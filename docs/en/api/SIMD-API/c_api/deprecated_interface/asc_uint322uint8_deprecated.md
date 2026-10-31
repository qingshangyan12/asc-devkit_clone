# asc_uint322uint8 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:52:48.813Z pushedAt=2026-10-28T12:55:49.522Z nl=2 -->

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

The header file path is `"c_api/reg_compute/reg_convert.h"`.

**The prototype of this API without explicit position parameters is deprecated. Use the explicit position parameter overload of [asc_uint322uint8](../reg_compute/reg_convert/asc_uint322uint8.md) instead.**

Converts uint32_t data to uint8_t, supporting saturation/non-saturation modes.

For details about rounding modes and saturation/non-saturation modes, see [Rounding Modes and Saturation Modes](../reg_compute/reg_convert/rounding_mode.md).

Because the bit width ratio of the source operand to the destination operand is 4:1, data of one VL size must be divided into four parts when written, and index 0, index 1, index 2, or index 3 is selected depending on the API.

## Prototype

```cpp
// Non-saturation mode. Write data to the position of index 0.
__simd_callee__ inline void asc_uint322uint8(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// Saturation mode. Write data to the position of index 0.
__simd_callee__ inline void asc_uint322uint8_sat(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// Non-saturation mode. Write data to the position of index 1.
__simd_callee__ inline void asc_uint322uint8_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// Saturation mode. Write data to the position of index 1.
__simd_callee__ inline void asc_uint322uint8_sat_v2(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// In non-saturation mode, write data to index 2.
__simd_callee__ inline void asc_uint322uint8_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// In saturation mode, write data to index 2.
__simd_callee__ inline void asc_uint322uint8_sat_v3(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// In non-saturation mode, write data to index 3.
__simd_callee__ inline void asc_uint322uint8_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
// In saturation mode, write data to index 3.
__simd_callee__ inline void asc_uint322uint8_sat_v4(vector_uint8_t& dst, vector_uint32_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| ------------ | ------------ | ------------ |
| dst        | Output    | Destination operand (vector data register). |
| src        | Input    | Source operand (vector data register). |
| mask       | Input    | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by `mask` are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- When saturation mode and non-saturation mode are enabled, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_uint32_t src;
vector_uint8_t dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the external input Unified Buffer (UB) memory space.
asc_uint322uint8(dst, src, mask);
```
