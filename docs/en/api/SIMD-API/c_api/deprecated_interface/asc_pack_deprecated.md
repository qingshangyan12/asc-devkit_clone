# asc_pack (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:41:53.488Z pushedAt=2026-10-28T12:55:35.759Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_permute_sel.h"`.

**This API is deprecated. Use [asc_pack_to_low](../reg_compute/reg_permute_sel/asc_pack_to_low.md) and [asc_pack_to_high](../reg_compute/reg_permute_sel/asc_pack_to_high.md) instead.**

Selects the low 8 bits (b16) or low 16 bits (b32) of the elements in the source operand and writes them to the low half or high half of the destination operand. When the operand is a mask register, selects the bits at even indices and writes them to the low half or high half of the destination operand.

- asc_pack: Writes the source operand to the low half of the destination operand.

- asc_pack_v2: Writes the source operand to the high half of the destination operand.

## Prototype

```cpp
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_uint16_t src)
__simd_callee__ inline void asc_pack(vector_uint8_t& dst, vector_int16_t src)
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_uint32_t src)
__simd_callee__ inline void asc_pack(vector_uint16_t& dst, vector_int32_t src)
__simd_callee__ inline void asc_pack(vector_bool& dst, vector_bool src)
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_uint16_t src)
__simd_callee__ inline void asc_pack_v2(vector_uint8_t& dst, vector_int16_t src)
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_uint32_t src)
__simd_callee__ inline void asc_pack_v2(vector_uint16_t& dst, vector_int32_t src)
__simd_callee__ inline void asc_pack_v2(vector_bool& dst, vector_bool src)
```

## Parameters

**Table 1** Parameter description

| Parameter       | Input/Output | Description               |
| --------- | ----- | ---------------- |
| dst       | Output    | Destination operand (vector data register/mask register).            |
| src | Input    | Source operand (vector data register/mask register).            |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_uint8_t dst;
vector_uint16_t src;
asc_pack(dst, src);    // Write the low 8 bits of src to the low half of dst.
asc_pack_v2(dst, src);    // Write the low 8 bits of src to the high half of dst.
```
