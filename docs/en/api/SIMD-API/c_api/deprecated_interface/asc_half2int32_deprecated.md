# asc_half2int32 (deprecated)

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T02:45:56.130Z pushedAt=2026-10-28T12:55:35.648Z nl=2 -->

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

**The prototype that controls the source operand read position through the API suffix (including only three parameters: source operand, destination operand, and mask register) is deprecated. Use the explicit position parameter overload of [asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md) instead.**

Converts each `half` element in `src` that participates in computation to `int32_t` based on `mask`, and writes the result to `dst`. Because the bit-width ratio of the source operand type to the destination operand type is 1:2, a `VL`-sized data block must be split into two parts when reading data, and the API suffix determines whether to read the even-indexed or odd-indexed positions of `src`. The pseudocode is as follows:

```python
def asc_half2int32_rd(dst, src, mask):
    for i in range(64):
        if mask[2 * i]:
            dst[i] = int32(src[2 * i])   # half -> int32, FLOOR rounding, read even indices
        else:
            dst[i] = 0

def asc_half2int32_rd_v2(dst, src, mask):
    for i in range(64):
        if mask[2 * i + 1]:
            dst[i] = int32(src[2 * i + 1])  # half -> int32, FLOOR rounding, read odd indices
        else:
            dst[i] = 0
```

For details about the rounding mode, see [rounding mode](../reg_compute/reg_convert/rounding_mode.md).

### Migration Guide

**Table 1** Mapping between deprecated and new position parameters

| Deprecated prototype position suffix | New prototype position parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_half2int32](../reg_compute/reg_convert/asc_half2int32.md).

## Prototype

### Template Prototype (Placeholder Form)

```c
__simd_callee__ inline void asc_half2int32_<round_mode><position_mode>(vector_int32_t& dst,
                                                                       vector_half src,
                                                                       vector_bool mask)
```

**The placeholders are described as follows:**

- `<round_mode>` indicates the supported rounding mode, which can be `rd` (`FLOOR`), `rn` (`RINT`), `rna` (`ROUND`), `ru` (`CEIL`), or `rz` (`TRUNC`).
- `<position_mode>` indicates the position from which `src` is read. When it is empty, the result is read from the even-indexed positions of `src`; when it is `_v2`, the result is read from the odd-indexed positions of `src`.

### Prototype Example

```c
// FLOOR rounding mode, reading the even-indexed positions of src.
__simd_callee__ inline void asc_half2int32_rd(vector_int32_t& dst,
                                              vector_half src,
                                              vector_bool mask)
```

## Parameters

**Table 2** Parameter description

| Parameter | Input/Output | Description |
| ------ | --------- | ---- |
| dst  | Output      | Destination operand (vector data register). |
| src  | Input      | Source operand (vector data register). |
| mask | Input      | Mask register that controls whether each element participates in the computation. When the bit corresponding to an element in `mask` is 1, the element participates in the computation; when it is 0, the element does not participate. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Constraints

- A function prototype that outputs results through reference parameters returns directly when called on a non-AIV.
- This API is called within a Vector Function (a function marked with `__simd_vf__`).
- `mask` must be assigned in advance through the mask setting API before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect valid element positions.
- When a mask bit in `mask` is 0, the corresponding element in `dst` is set to 0.

## Examples

```cpp
vector_int32_t dst;
vector_half src;
vector_bool mask;
mask = asc_create_mask_b16(PAT_ALL);
asc_half2int32_rd(dst, src, mask);     // FLOOR rounding mode, reading the positions with even indices in src.
asc_half2int32_rd_v2(dst, src, mask);  // FLOOR rounding mode, reading the positions with odd indices in src.
```
