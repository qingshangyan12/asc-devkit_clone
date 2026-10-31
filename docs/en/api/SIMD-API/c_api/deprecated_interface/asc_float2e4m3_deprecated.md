# asc_float2e4m3 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:44:20.522Z pushedAt=2026-10-28T12:55:18.898Z nl=2 -->

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

**The prototype of this API without explicit positional parameters is deprecated. Use the explicit positional parameter overload of [asc_float2e4m3](../reg_compute/reg_convert/asc_float2e4m3.md) instead.**

Converts the float type to the fp8_e4m3fn_t type, supporting the RINT rounding mode and saturated/non-saturated modes.

For details about the rounding mode and saturated/non-saturated modes, see [rounding mode](../reg_compute/reg_convert/rounding_mode.md).

Because the bit-width ratio of the source operand to the destination operand is 4:1, data of one VL size must be divided into four parts when writing, and index 0, index 1, index 2, or index 3 is selected depending on the API.

## Prototype

```cpp
// RINT rounding mode, non-saturated mode, data written to index 0.
__simd_callee__ inline void asc_float2e4m3_rn(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturated mode, data written to index 0.
__simd_callee__ inline void asc_float2e4m3_rn_sat(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, non-saturated mode, data written to index 1.
__simd_callee__ inline void asc_float2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturated mode, data written to index 1.
__simd_callee__ inline void asc_float2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, non-saturating mode, data written to index 2.
__simd_callee__ inline void asc_float2e4m3_rn_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturating mode, data written to index 2.
__simd_callee__ inline void asc_float2e4m3_rn_sat_v3(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, non-saturating mode, data written to index 3.
__simd_callee__ inline void asc_float2e4m3_rn_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)
// RINT rounding mode, saturating mode, data written to index 3.
__simd_callee__ inline void asc_float2e4m3_rn_sat_v4(vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask)

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

- When enabling saturating and non-saturating modes, configure the ctrl register. For details about the ctrl register, see [asc_set_ctrl.md](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_float src;
vector_fp8_e4m3fn_t dst;
vector_bool mask;
asc_float2e4m3_rn(dst, src, mask);
```
