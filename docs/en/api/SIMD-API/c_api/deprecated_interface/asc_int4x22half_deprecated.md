# asc_int4x22half (Deprecated)

<!-- md-trans-meta sourceCommit=28b6cff9e78c6de0dfa0a7b19c256fb6ad06a312 translatedAt=2026-10-23T03:08:51.122Z pushedAt=2026-10-28T12:55:35.690Z nl=2 -->

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

**The prototype of this API without explicit position parameters is deprecated. Use the overload with explicit position parameters of [asc_int4x22half](../reg_compute/reg_convert/asc_int4x22half.md) instead.**

`int4x2_t` packs two consecutive 4-bit signed integers into one 8-bit storage unit. This API converts the 4-bit signed integers involved in the computation to the `half` type based on `mask`.

Because the bit width of each 4-bit signed integer is less than 1 byte, during actual processing, the two 4-bit signed integers in the same `int4x2_t` storage unit jointly correspond to one mask bit in `mask`. When this mask bit is 1, the two 4-bit signed integers are converted together, and the conversion result is two consecutively stored `half` elements. When it is 0, the corresponding two `half` elements are both set to 0.

## Prototype

```cpp
// Position where the data read index is 0.
__simd_callee__ inline void asc_int4x22half(vector_half& dst, vector_int4x2_t src, vector_bool mask)
// Position where the data read index is 1.
__simd_callee__ inline void asc_int4x22half_v2(vector_half& dst, vector_int4x2_t src, vector_bool mask)
// Position where the data read index is 2.
__simd_callee__ inline void asc_int4x22half_v3(vector_half& dst, vector_int4x2_t src, vector_bool mask)
// Position where the data read index is 3.
__simd_callee__ inline void asc_int4x22half_v4(vector_half& dst, vector_int4x2_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description                                                                   |
|:------| :--- |:---------------------------------------------------------------------|
| dst   | Output | Destination operand (vector data register).                                                      |
| src   | Input | Source operand (vector data register).                                                            |
| mask  | Input | Source operand mask (mask register), which indicates the elements involved in the computation. When the corresponding position is 1, the element is involved in the computation; when it is 0, the element is not involved. Elements not selected by mask are set to 0 in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

None

## Examples

```cpp
vector_half dst;
vector_int4x2_t src;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the address of the external Unified Buffer (UB) memory space.
asc_int4x22half(dst, src, mask);
```
