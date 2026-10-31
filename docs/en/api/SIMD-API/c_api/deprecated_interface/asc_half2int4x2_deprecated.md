# asc_half2int4x2 (Deprecated)

<!-- md-trans-meta sourceCommit=28b6cff9e78c6de0dfa0a7b19c256fb6ad06a312 translatedAt=2026-10-23T02:48:59.198Z pushedAt=2026-10-28T12:55:35.653Z nl=2 -->

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

**The prototype of this API without explicit positional parameters is deprecated. Use the explicit positional parameter overload of [asc_half2int4x2](../reg_compute/reg_convert/asc_half2int4x2.md) instead.**

Converts half data to int4x2 type, and supports multiple rounding modes and saturation/non-saturation modes.

For details about rounding modes and saturation/non-saturation modes, see [rounding mode](../reg_compute/reg_convert/rounding_mode.md).

## Prototype

```cpp
// FLOOR rounding mode, non-saturation mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rd(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rd_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, saturation mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rd_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, saturation mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rd_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rd_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, saturation mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rd_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, non-saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rd_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// FLOOR rounding mode, saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rd_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, non-saturating mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rn(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, non-saturating mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rn_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, saturating mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rn_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, saturating mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rn_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, non-saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rn_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rn_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, non-saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rn_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// RINT rounding mode, saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rn_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, non-saturating mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rna(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, non-saturating mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rna_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, saturating mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rna_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, saturating mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rna_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, non-saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rna_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rna_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, non-saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rna_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// ROUND rounding mode, saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rna_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, non-saturating mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_ru(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, non-saturating mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_ru_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, saturation mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_ru_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, saturation mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_ru_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, non-saturation mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_ru_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, saturation mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_ru_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, non-saturation mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_ru_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// CEIL rounding mode, saturation mode, select the position at index 3.  
__simd_callee__ inline void asc_half2int4x2_ru_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, non-saturation mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rz(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, non-saturation mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rz_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, saturation mode, select the position at index 0.
__simd_callee__ inline void asc_half2int4x2_rz_sat(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, saturation mode, select the position at index 1.
__simd_callee__ inline void asc_half2int4x2_rz_sat_v2(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, non-saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rz_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, saturating mode, select the position at index 2.
__simd_callee__ inline void asc_half2int4x2_rz_sat_v3(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, non-saturating mode, select the position at index 3.
__simd_callee__ inline void asc_half2int4x2_rz_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
// TRUNC rounding mode, saturating mode, select the position at index 3. 
__simd_callee__ inline void asc_half2int4x2_rz_sat_v4(vector_int4x2_t& dst, vector_half src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

|Parameter|Input/Output|Description|
| ------------ | ------------ | ------------ |
|dst|Output|Destination operand (vector data register).|
|src|Input|Source operand (vector data register).|
|mask|Input|Source operand mask (mask register), which indicates which elements participate in the computation. A value of 1 at a position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output.|

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- When enabling the saturating mode and non-saturating mode, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_half src;
vector_int4x2_t dst;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the externally input Unified Buffer (UB) memory space.
asc_half2int4x2_rna(dst, src, mask);
```
