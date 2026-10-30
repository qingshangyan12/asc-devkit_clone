# asc_half2hif8 (Deprecated)

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T02:43:54.017Z pushedAt=2026-10-28T12:55:35.645Z nl=2 -->

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

**The prototype of this API that controls the destination operand write position through the API suffix (including only the source operand, destination operand, and mask register parameters) is deprecated. Use the explicit position parameter overload of [asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md) instead.**

Converts each `half` element in `src` that participates in the computation to the `hifloat8_t` type based on `mask`, and writes the result to `dst`. Because the bit-width ratio between the source operand and destination operand types is 2:1, the data of one `VL` size must be split into two parts when writing, and the API suffix determines whether to write to the even-indexed or odd-indexed positions of `dst`. The pseudocode is as follows:

```python
def asc_half2hif8_rna(dst, src, mask):
    for i in range(128):
        if mask[i]:
            dst[2 * i] = hifloat8(src[i])   # half -> hifloat8, ROUND rounding, write to even index
        else:
            dst[2 * i] = 0
    for i in range(128):
        dst[2 * i + 1] = 0                  # Set odd index to zero

def asc_half2hif8_rna_v2(dst, src, mask):
    for i in range(128):
        dst[2 * i] = 0                      # Set even index to zero
        if mask[i]:
            dst[2 * i + 1] = hifloat8(src[i])  # half -> hifloat8, ROUND rounding, write to odd index
        else:
            dst[2 * i + 1] = 0
```

For details about the rounding mode and saturation/non-saturation mode, see [rounding mode](../reg_compute/reg_convert/rounding_mode.md).

### Migration Guide

**Table 1** Mapping between old and new position parameters

| old prototype position suffix | new prototype position parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_half2hif8](../reg_compute/reg_convert/asc_half2hif8.md).

## Prototype

### Template Prototype (Placeholder Form)

```c
__simd_callee__ inline void asc_half2hif8_<round_mode><sat_mode><position_mode>(vector_hifloat8_t& dst,
                                                                                vector_half src,
                                                                                vector_bool mask)
```

**The placeholders are described as follows:**

- `<round_mode>` indicates the supported rounding mode, which can be `rna` (`ROUND`) or `rh` (`HYBRID`).
- `<sat_mode>` indicates the saturation/non-saturation mode. When empty, it indicates the non-saturation mode; when set to `_sat`, it indicates the saturation mode.
- `<position_mode>` indicates the position where `dst` is written. When empty, the result is written to the even-indexed positions of `dst`; when set to `_v2`, the result is written to the odd-indexed positions of `dst`.

### Prototype Example

```c
// ROUND rounding mode, non-saturation mode, write to the even-indexed positions of dst.
__simd_callee__ inline void asc_half2hif8_rna(vector_hifloat8_t& dst,
                                              vector_half src,
                                              vector_bool mask)
```

## Parameters

**Table 2** Parameters

| Parameter | Input/Output | Description |
| ------ | --------- | ---- |
| dst  | Output      | Destination operand (vector data register). |
| src  | Input      | Source operand (vector data register). |
| mask | Input      | Mask register that controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- A function prototype that outputs the result through a reference parameter returns directly when called on a non-AIV.
- This API is called within a Vector Function (a function marked with `__simd_vf__`).
- `mask` must be assigned a value in advance through the mask setting API before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect valid element positions.
- When data is written to odd-indexed positions of the destination operand, the values at even-indexed positions are set to zero, and vice versa.
- When using saturation or non-saturation mode, configure the `ctrl` register. For details, see [asc_set_ctrl](../spr/asc_set_ctrl.md).
- When the `mask` bit is 0, the corresponding element of `dst` is set to 0.

## Examples

```cpp
vector_hifloat8_t dst;
vector_half src;
vector_bool mask;
mask = asc_create_mask_b16(PAT_ALL);
asc_half2hif8_rna(dst, src, mask);         // ROUND rounding mode, non-saturation mode, write to even-indexed positions of dst.
asc_half2hif8_rna_sat(dst, src, mask);     // ROUND rounding mode, saturation mode, write to even-indexed positions of dst.
asc_half2hif8_rna_v2(dst, src, mask);      // ROUND rounding mode, non-saturating mode, write to the odd-indexed positions of dst.
asc_half2hif8_rna_sat_v2(dst, src, mask);  // ROUND rounding mode, saturating mode, write to the odd-indexed positions of dst.
```
