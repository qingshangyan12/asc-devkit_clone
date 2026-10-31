# asc_int642int32 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:11:14.706Z pushedAt=2026-10-28T12:55:35.696Z nl=2 -->

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

**The prototypes of this API that control the write position of the destination operand through the API suffix (including only the source operand, destination operand, and mask register parameters) are deprecated. Use the explicit position parameter overload of [asc_int642int32](../reg_compute/reg_convert/asc_int642int32.md) instead.**

Converts an int64_t source operand to int32_t and supports different saturation modes.

For details about rounding modes and saturation/non-saturation modes, see [Rounding Mode](../reg_compute/reg_convert/rounding_mode.md).

Because the bit-width ratio between the source operand and the destination operand is 2:1, data of one VL size must be split into two parts when written. Depending on the API, the data is written to either the odd-indexed positions or the even-indexed positions.

## Prototype

```cpp
// Non-saturation mode. Data is written to even-indexed positions.
__simd_callee__ inline void asc_int642int32(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
// Saturation mode. Data is written to even-indexed positions.
__simd_callee__ inline void asc_int642int32_sat(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
// Non-saturation mode. Data is written to odd-indexed positions.
__simd_callee__ inline void asc_int642int32_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
// Saturation mode. Data is written to odd-indexed positions.
__simd_callee__ inline void asc_int642int32_sat_v2(vector_int32_t& dst, vector_int64_t src, vector_bool mask)
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
- To enable saturation mode or non-saturation mode, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_int32_t dst;
vector_int64_t src;
vector_bool mask;
mask = asc_create_mask_b32(PAT_ALL); // Currently, mask supports only b8, b16, and b32 modes. A full mask of a lower bit width can be used as a full mask for a higher-bit-width data type.
asc_int642int32(dst, src, mask);
asc_int642int32_sat(dst, src, mask);
asc_int642int32_v2(dst, src, mask);
asc_int642int32_sat_v2(dst, src, mask);
```
