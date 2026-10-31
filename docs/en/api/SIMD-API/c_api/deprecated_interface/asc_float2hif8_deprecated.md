# asc_float2hif8 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:49:20.633Z pushedAt=2026-10-28T12:55:18.909Z nl=2 -->

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

**The prototype of this API without explicit position parameters is deprecated. Use the explicit position parameter overload of [asc_float2hif8](../reg_compute/reg_convert/asc_float2hif8.md) instead.**

Converts the float type to the hifloat8_t type, supporting multiple rounding modes and saturated/non-saturated modes.

For details about rounding modes and saturated/non-saturated modes, see [Rounding Mode](../reg_compute/reg_convert/rounding_mode.md).

Because the bit width ratio between the source operand and the destination operand is 4:1, the data of one VL size must be divided into four parts when writing. Select index 0, index 1, index 2, or index 3 depending on the API.

## Prototype

```cpp
// ROUND rounding mode, non-saturated mode, data written to index 0.
__simd_callee__ inline void asc_float2hif8_rna(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturated mode, data written to index 0.
__simd_callee__ inline void asc_float2hif8_rna_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturated mode, data written to index 1.
__simd_callee__ inline void asc_float2hif8_rna_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturated mode, data written to index 1.
__simd_callee__ inline void asc_float2hif8_rna_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, data written to the position at index 2.
__simd_callee__ inline void asc_float2hif8_rna_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, data written to the position at index 2.
__simd_callee__ inline void asc_float2hif8_rna_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, non-saturation mode, data written to the position at index 3.
__simd_callee__ inline void asc_float2hif8_rna_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// ROUND rounding mode, saturation mode, data written to the position at index 3.
__simd_callee__ inline void asc_float2hif8_rna_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, non-saturation mode, data written to the position at index 0.
__simd_callee__ inline void asc_float2hif8_rh(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, saturation mode, data written to the position at index 0.
__simd_callee__ inline void asc_float2hif8_rh_sat(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, non-saturation mode, data written to the position at index 1.
__simd_callee__ inline void asc_float2hif8_rh_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, saturation mode, data written to the position at index 1.
__simd_callee__ inline void asc_float2hif8_rh_sat_v2(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, non-saturation mode, data written to the position at index 2.
__simd_callee__ inline void asc_float2hif8_rh_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, saturation mode, data written to the position at index 2.
__simd_callee__ inline void asc_float2hif8_rh_sat_v3(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, non-saturating mode, data written to index 3.
__simd_callee__ inline void asc_float2hif8_rh_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
// HYBRID rounding mode, saturating mode, data written to index 3.
__simd_callee__ inline void asc_float2hif8_rh_sat_v4(vector_hifloat8_t& dst, vector_float src, vector_bool mask)
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

- When saturating and non-saturating modes are enabled, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_float src;
vector_hifloat8_t dst;
vector_bool mask;
asc_float2hif8_rna(dst, src, mask);
```
