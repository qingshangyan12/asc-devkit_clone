# asc_e1m2x22bfloat16 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:31:48.784Z pushedAt=2026-10-28T12:55:18.879Z nl=2 -->

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

**The prototype of this API without explicit positional parameters is deprecated. Use the explicit positional parameter overload of [asc_e1m2x22bfloat16](../reg_compute/reg_convert/asc_e1m2x22bfloat16.md) instead.**

Converts the fp4x2_e1m2_t type to the bfloat16_t type.

Because the bit-width ratio of the source operand to the destination operand is 1:4, when reading data, one VL-sized data block must be divided into four parts, and index 0, index 1, index 2, or index 3 is selected depending on the API.

## Prototype

```cpp
// Position for reading data at index 0.
__simd_callee__ inline void asc_e1m2x22bfloat16(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
// Position for reading data at index 1.
__simd_callee__ inline void asc_e1m2x22bfloat16_v2(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
// Position for reading data at index 2.
__simd_callee__ inline void asc_e1m2x22bfloat16_v3(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
// Position for reading data at index 3.
__simd_callee__ inline void asc_e1m2x22bfloat16_v4(vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :------ | :----- | :----------------- |
| dst    | Output    | Destination operand (vector data register). |
| src    | Input    | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. An element participates in the computation when the corresponding bit is 1, and does not participate when the bit is 0. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_fp4x2_e1m2_t src;
vector_bfloat16_t dst;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the Unified Buffer (UB) memory space address input from outside.
asc_e1m2x22bfloat16(dst, src, mask);
```
