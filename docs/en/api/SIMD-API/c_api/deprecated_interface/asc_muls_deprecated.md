# asc_muls (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:34:49.959Z pushedAt=2026-10-28T12:55:35.745Z nl=2 -->

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

> This API is deprecated. Use [asc_mul_scalar_float2half_rn](../reg_compute/reg_fused/asc_mul_scalar_float2half_rn.md) instead.

The header file path is `"c_api/reg_compute/compute/reg_fused.h"`.

**This API is deprecated.**

Multiplies the source operands src and value, converts the result to the half type using the ROUND rounding mode, and writes the result to the destination operand dst according to mask. The calculation formula is as follows:

$$
dst_i = round\_to\_half(src_i \times value)
$$

## Prototype

```cpp
// Write data to even-indexed positions.
__simd_callee__ inline void asc_muls(vector_half& dst, vector_float src, float value, vector_bool mask)
// Write data to odd-indexed positions.
__simd_callee__ inline void asc_muls_v2(vector_half& dst, vector_float src, float value, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| value | Input | Source operand (scalar). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at a position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_float src;
vector_half dst;
float value = 1.0f;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_loadalign(src, src_addr);  // src_addr is the address of the externally input Unified Buffer (UB) memory space.
asc_muls(dst, src, value, mask);
```
