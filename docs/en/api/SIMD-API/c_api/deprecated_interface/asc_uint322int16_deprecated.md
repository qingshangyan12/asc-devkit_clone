# asc_uint322int16 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:51:09.729Z pushedAt=2026-10-28T12:55:49.518Z nl=2 -->

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

**The prototype of this API, which controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters), is deprecated. Use the explicit position parameter overload of [asc_uint322int16](../reg_compute/reg_convert/asc_uint322int16.md) instead.**

Converts uint32_t data to int16_t data, supporting saturation/non-saturation mode.

For details about the rounding mode and saturation/non-saturation mode, see [Rounding Mode](../reg_compute/reg_convert/rounding_mode.md).

Because the bit width ratio between the source operand and the destination operand is 2:1, the data of one VL size must be split into two parts when writing. Depending on the API, the data is written to either odd-indexed or even-indexed positions.

## Prototype

```cpp
// Write data to even-indexed positions in non-saturation mode.
__simd_callee__ inline void asc_uint322int16(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
// Write data to even-indexed positions in saturation mode.
__simd_callee__ inline void asc_uint322int16_sat(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
// Write data to odd-indexed positions in non-saturation mode.
__simd_callee__ inline void asc_uint322int16_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
// Write data to odd-indexed positions in saturation mode.
__simd_callee__ inline void asc_uint322int16_sat_v2(vector_int16_t& dst, vector_uint32_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate in the computation are set to zero at the corresponding positions in the output.
- When data is written to odd-indexed positions of the destination operand, the values at even-indexed positions are set to zero, and vice versa.
- When saturation mode and non-saturation mode are enabled, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_uint32_t src;
vector_int16_t dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the external Unified Buffer (UB) memory space.
asc_uint322int16(dst, src, mask);
```
