# asc_float2half (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:47:40.441Z pushedAt=2026-10-28T12:55:18.907Z nl=2 -->

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

**The prototype of this API that controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters) is deprecated. Use the explicit position parameter overload of [asc_float2half](../reg_compute/reg_convert/asc_float2half.md) instead.**

Converts float data to half data, supporting multiple rounding modes and saturation modes.

For details about rounding modes and saturation/non-saturation modes, see [Rounding Modes and Saturation Modes](../reg_compute/reg_convert/rounding_mode.md).

Because the bit width ratio between the source operand and the destination operand is 2:1, data of one VL size must be split into two parts when written. Depending on the API, data is written to positions with odd indices or even indices.

## Prototype

```cpp
// RINT rounding mode, non-saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_rn(vector_half& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_rn_sat(vector_half& dst, vector_float src, vector_bool mask)
// RINT rounding mode, non-saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_rn_v2(vector_half& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_rn_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_rna(vector_half& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_rna_sat(vector_half& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, data is written to odd-indexed positions.
__simd_callee__ inline void asc_float2half_rna_v2(vector_half& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, data is written to odd-indexed positions.
__simd_callee__ inline void asc_float2half_rna_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_rd(vector_half& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_rd_sat(vector_half& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, non-saturation mode, data is written to odd-indexed positions.
__simd_callee__ inline void asc_float2half_rd_v2(vector_half& dst, vector_float src, vector_bool mask)
// FLOOR rounding mode, saturation mode, data is written to odd-indexed positions.
__simd_callee__ inline void asc_float2half_rd_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, non-saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_ru(vector_half& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, saturation mode, data is written to even-indexed positions.
__simd_callee__ inline void asc_float2half_ru_sat(vector_half& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, non-saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_ru_v2(vector_half& dst, vector_float src, vector_bool mask)
// CEIL rounding mode, saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_ru_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, non-saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_rz(vector_half& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_rz_sat(vector_half& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, non-saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_rz_v2(vector_half& dst, vector_float src, vector_bool mask)
// TRUNC rounding mode, saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_rz_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
// ODD rounding mode, non-saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_ro(vector_half& dst, vector_float src, vector_bool mask)
// ODD rounding mode, saturation mode, data written to positions with even indices.
__simd_callee__ inline void asc_float2half_ro_sat(vector_half& dst, vector_float src, vector_bool mask)
// ODD rounding mode, non-saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_ro_v2(vector_half& dst, vector_float src, vector_bool mask)
// ODD rounding mode, saturation mode, data written to positions with odd indices.
__simd_callee__ inline void asc_float2half_ro_sat_v2(vector_half& dst, vector_float src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position indicates that the element participates in the computation, and 0 indicates that it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate in the computation are set to zero at the corresponding positions in the output.
- When data is written to positions with odd indices in the destination operand, the values at positions with even indices are set to zero, and vice versa.
- When saturation mode and non-saturation mode are enabled, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_float src;
vector_half dst;
vector_bool mask = asc_create_mask_b32(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the externally input Unified Buffer (UB) memory space.
asc_float2half_rn(dst, src, mask);
```
