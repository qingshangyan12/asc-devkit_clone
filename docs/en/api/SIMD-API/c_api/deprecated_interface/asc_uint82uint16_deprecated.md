# asc_uint82uint16 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T04:55:19.684Z pushedAt=2026-10-28T12:55:49.530Z nl=2 -->

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

**The prototype that controls the source operand read position through the API suffix (including only the three parameters: source operand, destination operand, and mask register) is deprecated. Use the explicit position parameter overload of [asc_uint82uint16](../reg_compute/reg_convert/asc_uint82uint16.md) instead.**

Converts each `uint8_t` element in the source operand `src` to `uint16_t` based on `mask`, and writes the result to `dst`. Because the bit-width ratio between the source operand and destination operand types is 1:2, the data of one `VL` size must be divided into two parts when reading, and the position parameter selects the odd-indexed or even-indexed positions of the input data. The reference pseudocode is as follows:

```python
# Position determination is only for explaining the API semantics. The actual position parameter is a compile-time constant.
# VL indicates the bit width of the vector data register, which is 256 bytes.
def asc_uint82uint16(dst, src, mask, src_pos):
    offset = 0 if src_pos == ASC_POSITION_EVEN else 1
    for i in range(VL // 2):
        src_index = 2 * i + offset
        dst[i] = uint16(src[src_index]) if mask[src_index] else 0
```

### Migration Notes

**Table 1** Mapping between old and new position parameters

| Old Prototype Position Suffix | New Prototype Position Parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_uint82uint16](../reg_compute/reg_convert/asc_uint82uint16.md).

## Prototype

```c
__simd_callee__ inline void asc_uint82uint16(vector_uint16_t& dst,
                                             vector_uint8_t src,
                                             vector_bool mask,
                                             std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)

__simd_callee__ inline void asc_uint82uint16(vector_uint16_t& dst,
                                             vector_uint8_t src,
                                             vector_bool mask,
                                             std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description                                                                                                       |
| ------ | --------- | ------------------------------------------------------------------------------------------------------------------ |
| dst  | Output      | Destination operand (vector data register).                                                                                     |
| src  | Input      | Source operand (vector data register).                                                                                       |
| mask | Input      | Mask register that controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |
| src_pos | Input | Position selection tag (compiler tag type), of type `std::integral_constant<asc_position_mode, asc_position_mode::EVEN>` or `std::integral_constant<asc_position_mode, asc_position_mode::ODD>`. When set to `ASC_POSITION_EVEN`, the even-indexed positions of the source operand are read; when set to `ASC_POSITION_ODD`, the odd-indexed positions of the source operand are read. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- The data types of `src` and `dst` must match the function prototype.
- When the mask bit in `mask` is 0, the corresponding element in `dst` is set to 0.

## Examples

```c
vector_uint8_t src;
vector_half dst;
vector_bool mask = asc_create_mask_b8(PAT_ALL);
asc_loadalign(src, src_addr); // src_addr is the externally input Unified Buffer (UB) memory space address.
asc_uint82uint16(dst, src, mask);
```
