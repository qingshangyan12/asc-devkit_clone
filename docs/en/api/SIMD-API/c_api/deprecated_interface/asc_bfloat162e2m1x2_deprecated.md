# asc_bfloat162e2m1x2 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:19:40.488Z pushedAt=2026-10-28T12:55:18.777Z nl=2 -->

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

**The prototype of this API without explicit position parameters is deprecated. Use the overload with explicit position parameters of [asc_bfloat162e2m1x2](../reg_compute/reg_convert/asc_bfloat162e2m1x2.md) instead.**

Converts the bfloat16_t type to the fp4x2_e2m1_t type and supports multiple rounding modes.

For details about rounding modes, see [Rounding Modes](../reg_compute/reg_convert/rounding_mode.md).

Because the bit width ratio between the source operand and the destination operand is 4:1, data of one VL size must be divided into four parts when writing data. Select index 0, index 1, index 2, or index 3 based on the API used.

## Prototype

```cpp
// RINT rounding mode. Write data to index 0.
__simd_callee__ inline void asc_bfloat162e2m1x2_rn(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// RINT rounding mode. Write data to index 1.
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// RINT rounding mode. Write data to index 2.
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// RINT rounding mode. Write data to index 3.
__simd_callee__ inline void asc_bfloat162e2m1x2_rn_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// ROUND rounding mode, data is written to index 0.
__simd_callee__ inline void asc_bfloat162e2m1x2_rna(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// ROUND rounding mode, data is written to index 1.
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// ROUND rounding mode, data is written to index 2.
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// ROUND rounding mode, data is written to index 3.
__simd_callee__ inline void asc_bfloat162e2m1x2_rna_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// FLOOR rounding mode, data is written to index 0.
__simd_callee__ inline void asc_bfloat162e2m1x2_rd(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// FLOOR rounding mode, data is written to index 1.
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// FLOOR rounding mode, data is written to index 2.
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// FLOOR rounding mode, data is written to index 3.
__simd_callee__ inline void asc_bfloat162e2m1x2_rd_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// CEIL rounding mode, data is written to index 0.
__simd_callee__ inline void asc_bfloat162e2m1x2_ru(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// CEIL rounding mode, data is written to index 1.
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// CEIL rounding mode. Write data to index 2.
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// CEIL rounding mode. Write data to index 3.
__simd_callee__ inline void asc_bfloat162e2m1x2_ru_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// TRUNC rounding mode. Write data to index 0.
__simd_callee__ inline void asc_bfloat162e2m1x2_rz(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// TRUNC rounding mode. Write data to index 1.
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v2(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// TRUNC rounding mode. Write data to index 2.
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v3(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
// TRUNC rounding mode. Write data to index 3.
__simd_callee__ inline void asc_bfloat162e2m1x2_rz_v4(vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

s

```cpp
vector_bfloat16_t src;
vector_fp4x2_e2m1_t dst;
vector_bool mask;
asc_bfloat162e2m1x2_rn(dst, src, mask);
```
