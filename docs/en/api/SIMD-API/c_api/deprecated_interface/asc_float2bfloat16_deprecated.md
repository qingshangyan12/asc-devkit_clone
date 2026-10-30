# asc_float2bfloat16 (Deprecated)

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T01:43:12.835Z pushedAt=2026-10-28T12:55:18.901Z nl=2 -->

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

**The prototype of this API that controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register) is deprecated. Use the explicit positional parameter overload of [asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md) instead.**

Converts each `float` value in `src` to `bfloat16_t` based on `mask`, supports multiple rounding modes and saturation modes, and writes the result to `dst`.
Because the bit-width ratio of the source operand to the destination operand is 2:1, the data of one `VL` size must be split into two parts when writing. Depending on the API, data is written to either the odd-indexed or even-indexed positions. The pseudocode is as follows:

```python
def asc_float2bfloat16_rn(dst, src, mask):
    for i in range(64):
        if mask[i]:
            dst[2 * i] = bfloat16(src[i])   # float -> bfloat16, RINT rounding, write to even index
        else:
            dst[2 * i] = 0
    for i in range(64):
        dst[2 * i + 1] = 0                  # Set odd index to zero

def asc_float2bfloat16_rn_v2(dst, src, mask):
    for i in range(64):
        dst[2 * i] = 0                      # Set even index to zero
        if mask[i]:
            dst[2 * i + 1] = bfloat16(src[i])  # float -> bfloat16, RINT rounding, write to odd index
        else:
            dst[2 * i + 1] = 0
```

For details about rounding modes, see [Rounding Mode](../reg_compute/reg_convert/rounding_mode.md).

### Migration Notes

**Table 1** Mapping between old and new position parameters

| Old prototype position suffix | New prototype position parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_float2bfloat16](../reg_compute/reg_convert/asc_float2bfloat16.md).

## Prototype

### Template Prototype (Placeholder Form)

```c
__simd_callee__ inline void asc_float2bfloat16_<round_mode><sat_mode><position_mode>(vector_bfloat16_t& dst,
                                                                                     vector_float src,
                                                                                     vector_bool mask)
```

**The placeholders are described as follows:**

- `<round_mode>` indicates the supported rounding modes, including `rd` (`FLOOR`), `rn` (`RINT`), `rna` (`ROUND`), `ru` (`CEIL`), and `rz` (`TRUNC`).
- `<sat_mode>` indicates the saturation/non-saturation mode. When empty, it indicates the non-saturation mode; when set to `_sat`, it indicates the saturation mode.
- `<position_mode>` indicates the position where `dst` is written. When empty, the result is written to the even-indexed positions of `dst`; when set to `_v2`, the result is written to the odd-indexed positions of `dst`.

### Prototype Example

```c
// RINT rounding mode, non-saturation mode, data written to even-indexed positions.
__simd_callee__ inline void asc_float2bfloat16_rn(vector_bfloat16_t& dst,
                                                  vector_float src,
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
- When the `mask` bit is 0, the corresponding element in `dst` is set to 0.
- When the result is written to odd-index positions of `dst`, the even-index positions are set to 0; when the result is written to even-index positions, the odd-index positions are set to 0.
- When using saturation or non-saturation mode, configure the `ctrl` register. For details, see [asc_set_ctrl](../spr/asc_set_ctrl.md).

## Examples

```cpp
vector_bfloat16_t dst;
vector_float src;
vector_bool mask;
mask = asc_create_mask_b32(PAT_ALL);
asc_float2bfloat16_rd(dst, src, mask);    // FLOOR rounding mode, non-saturation mode. Convert src to vector_bfloat16_t and write the result to even-index positions of dst.
asc_float2bfloat16_rd_sat(dst, src, mask);    // FLOOR rounding mode, saturation mode. Convert src to vector_bfloat16_t and write the result to even-index positions of dst.
asc_float2bfloat16_rd_v2(dst, src, mask);    // FLOOR rounding mode, non-saturation mode. Convert src to vector_bfloat16_t and write the result to odd-index positions of dst.
asc_float2bfloat16_rd_sat_v2(dst, src, mask);    // FLOOR rounding mode, saturation mode. Convert src to vector_bfloat16_t and write the result to odd-index positions of dst.
```
