# asc_int162uint32 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:59:01.057Z pushedAt=2026-10-28T12:55:35.670Z nl=2 -->

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

**The prototype of this API, which controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters), is deprecated. Use the explicit position parameter overload of [asc_int162uint32](../reg_compute/reg_convert/asc_int162uint32.md) instead.**

Converts the int16_t elements in src to uint32_t and writes the results to dst. The conversion converts 16-bit signed integers to 32-bit unsigned integers.

Because the bit-width ratio between the source operand and destination operand types is 1:2, the data of one VL size must be divided into two parts when reading. Depending on the API, select the positions with odd or even input data indices.

## Prototype

```cpp
// Select the even-indexed elements of src (indices 0, 2, 4, ...) for conversion. The mask is grouped in 2-bit units, and the int16_t element at the corresponding position is selected only when the LSB within the group is 1.
__simd_callee__ inline void asc_int162uint32(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
// Select the odd-indexed elements of src (indices 1, 3, 5, ...) for conversion. The mask is grouped in 2-bit units, and the int16_t element at the corresponding position is selected only when the LSB within the group is 1.
__simd_callee__ inline void asc_int162uint32_v2(vector_uint32_t& dst, vector_int16_t src, vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --- | --- | --- |
| dst | Output | Destination operand (vector data register). |
| src | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register), which indicates the elements that participate in the computation. A value of 1 at the corresponding position means the element participates in the computation, and 0 means it does not. Elements not selected by the mask are set to zero in the output. |

For details about vector data registers and mask registers, see [reg Data Type Definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

The mask controls whether the source operand participates in the computation. Elements of the source operand that do not participate in the computation are set to zero at the corresponding positions in the output.

## Examples

```cpp
__simd_vf__ inline void int162uint32_vf(__ubuf__ uint32_t* dst_addr, __ubuf__ int16_t* src_addr, uint32_t count, uint16_t one_repeat_size, uint16_t repeat_time)
{
    vector_uint32_t dst;
    vector_int16_t src;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b16(count);
        asc_loadalign(src, src_addr + i * one_repeat_size);
        asc_int162uint32(dst, src, mask);
        asc_storealign(dst_addr + i * one_repeat_size, dst, mask);
    }
}
```
