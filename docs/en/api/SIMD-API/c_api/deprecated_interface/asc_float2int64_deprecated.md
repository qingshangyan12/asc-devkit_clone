# asc_float2int64 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:34:30.715Z pushedAt=2026-10-28T12:55:35.626Z nl=2 -->

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

**The prototype that controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters) is deprecated. Use the explicit position parameter overload of [asc_float2int64](../reg_compute/reg_convert/asc_float2int64.md) instead.**

Converts float data to int64_t and supports multiple rounding modes as well as saturation/non-saturation modes.

For details about rounding modes and saturation/non-saturation modes, see [Rounding Modes and Saturation Modes](../reg_compute/reg_convert/rounding_mode.md).

Because the bit-width ratio between the source operand and the destination operand is 1:2, data of one VL size must be split into two parts when reading. Depending on the API, select the positions with odd or even input data indices.

## Prototype

```cpp
// RINT rounding mode, non-saturation mode, data read at even indices.
__simd_callee__ inline void asc_float2int64_rn(vector_int64_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturation mode, data read at even indices.
__simd_callee__ inline void asc_float2int64_rn_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, non-saturation mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_rn_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturation mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_rn_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_rna(vector_int64_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_rna_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, position where the data read index is odd.
__simd_callee__ inline void asc_float2int64_rna_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, position where the data read index is odd.
__simd_callee__ inline void asc_float2int64_rna_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_rd(vector_int64_t& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_rd_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, position where the data read index is odd.
__simd_callee__ inline void asc_float2int64_rd_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, saturation mode, position where the data read index is odd.
__simd_callee__ inline void asc_float2int64_rd_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, non-saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_ru(vector_int64_t& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, saturation mode, position where the data read index is even.
__simd_callee__ inline void asc_float2int64_ru_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, non-saturating mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_ru_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, saturating mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_ru_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, non-saturating mode, data read at even indices.
__simd_callee__ inline void asc_float2int64_rz(vector_int64_t& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, saturating mode, data read at even indices.
__simd_callee__ inline void asc_float2int64_rz_sat(vector_int64_t& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, non-saturating mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_rz_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, saturating mode, data read at odd indices.
__simd_callee__ inline void asc_float2int64_rz_sat_v2(vector_int64_t& dst, vector_float src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operation (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at a position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate are set to zero at the corresponding positions in the output.
- When enabling the saturating mode or the non-saturating mode, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_float src;
vector_int64_t dst;
vector_bool mask;
mask = asc_create_mask_b32(PAT_ALL);
asc_float2int64_rn(dst, src, mask);
```
