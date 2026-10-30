# asc_int4x22bfloat16 (Deprecated)

<!-- md-trans-meta sourceCommit=28b6cff9e78c6de0dfa0a7b19c256fb6ad06a312 translatedAt=2026-10-23T03:06:47.500Z pushedAt=2026-10-28T12:55:35.687Z nl=2 -->

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

**The prototype of this API without explicit positional parameters is deprecated. Use the explicit positional parameter overload of [asc_int4x22bfloat16](../reg_compute/reg_convert/asc_int4x22bfloat16.md) instead.**

`int4x2_t` packs two consecutive 4-bit signed integers into one 8-bit storage unit. This API converts the 4-bit signed integers involved in the computation to the `bfloat16_t` type based on `mask`.

Because the bit width of each 4-bit signed integer is less than 1 byte, the two 4-bit signed integers in the same `int4x2_t` storage unit jointly correspond to one mask bit in `mask` during actual processing. When this mask bit is 1, the two 4-bit signed integers participate in the conversion together, and the conversion result is two consecutively stored `bfloat16_t` elements. When it is 0, the corresponding two `bfloat16_t` elements are both set to 0.

## Prototype

```cpp
// Read the data at index 0.
__simd_callee__ inline void asc_int4x22bfloat16(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
// Read the data at index 1.
__simd_callee__ inline void asc_int4x22bfloat16_v2(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
// Read the data at index 2.
__simd_callee__ inline void asc_int4x22bfloat16_v3(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
// Read the data at index 3.
__simd_callee__ inline void asc_int4x22bfloat16_v4(vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter    | Input/Output | Description                |
| :------ | :----- | :----------------- |
| dst    | Output    | Destination operand (vector data register).            |
| src    | Input    | Source operand (vector data register).             |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

None

## Examples

```cpp
vector_int4x2_t src;
vector_bfloat16_t dst;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the externally input Unified Buffer (UB) memory space.
asc_int4x22bfloat16(dst, src, mask);
```
