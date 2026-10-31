# asc_int322uint16 (Deprecated)

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T03:05:03.649Z pushedAt=2026-10-28T12:55:35.684Z nl=2 -->

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

**The prototype of this API that controls the destination operand write position through an API suffix (including only the source operand, destination operand, and mask register parameters) is deprecated. Use the explicit position parameter overload of [asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md) instead.**

Converts each `int32_t` element in `src` to `uint16_t` based on `mask`, supports different saturation modes, and writes the result to `dst`. Because the bit-width ratio of the source operand to the destination operand is 2:1, a `VL`-sized data block must be split into two parts when writing data. Depending on the API, data is written to either odd-indexed or even-indexed positions. Refer to the following pseudocode:

```python
def asc_int322uint16(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[2 * i] = uint16(src[i])   # int32 -> uint16, write to even index.
        else:
            dst[2 * i] = 0
    for i in range(64):
        dst[2 * i + 1] = 0                # Set odd index to zero.

def asc_int322uint16_v2(dst, src, mask):
    for i in range(64):
        dst[2 * i] = 0                    # Set even index to zero.
        if mask[i]:
            dst[2 * i + 1] = uint16(src[i])  # int32 -> uint16, write to odd index.
        else:
            dst[2 * i + 1] = 0
```

For details about rounding modes and saturation/non-saturation modes, see [Rounding Mode](../reg_compute/reg_convert/rounding_mode.md).

### Migration

**Table 1** Mapping between old and new position parameters

| Old prototype position suffix | New prototype position parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_int322uint16](../reg_compute/reg_convert/asc_int322uint16.md).

## Prototype

### Template Prototype (Placeholder Form)

```c
__simd_callee__ inline void asc_int322uint16<sat_mode><position_mode>(vector_uint16_t& dst,
                                                                      vector_int32_t src,
                                                                      vector_bool mask)
```

**The placeholders are described as follows:**

- `<sat_mode>` indicates the saturation/non-saturation mode. When empty, it indicates the non-saturation mode; when set to `_sat`, it indicates the saturation mode.
- `<position_mode>` indicates the position where `dst` is written. When empty, the result is written to the even-indexed positions of `dst`; when set to `_v2`, the result is written to the odd-indexed positions of `dst`.

### Prototype Example

```c
// Non-saturation mode, with data written to even-indexed positions.
__simd_callee__ inline void asc_int322uint16(vector_uint16_t& dst,
                                             vector_int32_t src,
                                             vector_bool mask)
```

## Parameters

**Table 2** Parameter description

| Parameter | Input/Output | Description                                                                                                               |
| ------ | --------- | ------------------------------------------------------------------------------------------------------------------ |
| dst  | Output      | Destination operand (vector data register).                                                                                     |
| src  | Input      | Source operand (vector data register).                                                                                       |
| mask | Input      | Mask register that controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- The data types of `src` and `dst` must match the function prototype.
- When the `mask` bit is 0, the corresponding element of `dst` is set to 0.

## Examples

```cpp
__simd_vf__ inline void int322uint16_vf(__ubuf__ int32_t* src_addr, __ubuf__ uint16_t* dst_addr, uint32_t count, uint16_t one_repeat_size, uint16_t one_block_size, uint16_t repeat_time)
{
    vector_int32_t src;
    vector_uint16_t dst;
    vector_bool mask;
    for (uint16_t i = 0; i < repeat_time; ++i) {
        mask = asc_update_mask_b32(count);
        asc_loadalign_postupdate(src, src_addr, one_repeat_size);
        asc_int322uint16(dst, src, mask);
        asc_storealign_pack_postupdate(dst_addr, dst, one_block_size, mask);
    }
}
```
