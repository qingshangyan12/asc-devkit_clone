# asc_int162uint8 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:59:28.205Z pushedAt=2026-10-28T12:55:35.672Z nl=2 -->

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

**The prototype of this API, which controls the write position of the destination operand through the API suffix (including only the source operand, destination operand, and mask register parameters), is deprecated. Use the explicit position parameter overload of [asc_int162uint8](../reg_compute/reg_convert/asc_int162uint8.md) instead.**

Converts int16_t elements in src to uint8_t type and writes the results to dst. Both non-saturating and saturating modes are supported.

Because the bit-width ratio of the source operand to the destination operand is 2:1, data of one VL size must be split into two parts when writing. Depending on the API, the data is written to either odd-indexed or even-indexed positions.

## Prototype

```cpp
// Non-saturating mode. The result is written to position 0 in each group of dst, with direct truncation. mask is grouped in 2-bit units. Only when the LSB in a group is 1 is the int16_t element at the corresponding position selected.
__simd_callee__ inline void asc_int162uint8(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
// Non-saturating mode. The result is written to position 1 in each group of dst, with direct truncation. mask is grouped in 2-bit units. Only when the LSB in a group is 1 is the int16_t element at the corresponding position selected.
__simd_callee__ inline void asc_int162uint8_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
// Saturating mode. Values outside the [0, 255] range are clamped to the boundary. The result is written to position 0 in each group of dst. mask is grouped in 2-bit units. Only when the LSB in a group is 1 is the int16_t element at the corresponding position selected.
__simd_callee__ inline void asc_int162uint8_sat(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
// Saturating mode. Values outside the [0, 255] range are clamped to the boundary. The result is written to position 1 in each group of dst. mask is grouped in 2-bit units. Only when the LSB in a group is 1 is the int16_t element at the corresponding position selected.
__simd_callee__ inline void asc_int162uint8_sat_v2(vector_uint8_t& dst, vector_int16_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operation (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates which elements participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate in the computation are set to zero at the corresponding positions in the output.

## Examples

```cpp
__simd_vf__ inline void int162uint8_vf(__ubuf__ uint8_t* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint8_t dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162uint8(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
