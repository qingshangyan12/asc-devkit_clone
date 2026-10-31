# asc_half2float (Deprecated)

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T02:42:34.686Z pushedAt=2026-10-28T12:55:35.642Z nl=2 -->

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

**The prototype that controls the source operand read position through the API suffix (including only the source operand, destination operand, and mask register parameters) has been deprecated. Use the explicit position parameter overload of [asc_half2float](../reg_compute/reg_convert/asc_half2float.md) instead.**

Converts each `half`-type element in `src` that participates in the computation to `float` type according to `mask`, and writes the result to `dst`. Because the bit-width ratio between the source operand and the destination operand is 1:2, the data of one `VL` size must be divided into two parts when reading. Based on the API suffix, read the even-indexed or odd-indexed positions of `src`. The pseudocode is as follows:

```python
def asc_half2float(dst, src, mask):
    for i in range(64):
        if mask[2 * i]:
            dst[i] = float(src[2 * i])   # half -> float, read even index
        else:
            dst[i] = 0

def asc_half2float_v2(dst, src, mask):
    for i in range(64):
        if mask[2 * i + 1]:
            dst[i] = float(src[2 * i + 1])  # half -> float, read odd index
        else:
            dst[i] = 0
```

### Migration

**Table 1** Mapping between legacy and new position parameters

| legacy prototype position suffix | new prototype position parameter |
| --- | --- |
| No position suffix | `ASC_POSITION_EVEN` |
| `_v2` | `ASC_POSITION_ODD` |

During migration, remove the position suffix from the function name and pass the corresponding position constant as the fourth parameter. For the new prototype, see [asc_half2float](../reg_compute/reg_convert/asc_half2float.md).

## Prototype

### Template Prototype (Placeholder Form)

```c
__simd_callee__ inline void asc_half2float<position_mode>(vector_float& dst,
                                                          vector_half src,
                                                          vector_bool mask)
```

**The placeholders are described as follows:**

- `<position_mode>` specifies the position from which `src` is read. When empty, the result reads the even-indexed positions of `src`; when set to `_v2`, the result reads the odd-indexed positions of `src`.

### Prototype Example

```c
// Read the even-indexed positions of src.
__simd_callee__ inline void asc_half2float(vector_float& dst,
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

- Function prototypes that output results through reference parameters return directly when called on non-AIV.
- This API is called within a Vector Function (a function marked with `__simd_vf__`).
- `mask` must be assigned in advance through the mask setting API before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect valid element positions.
- When a mask bit in `mask` is 0, the corresponding element in `dst` is set to 0.

## Examples

```cpp
vector_float dst;
vector_half src;
vector_bool mask;
mask = asc_create_mask_b16(PAT_ALL);
asc_half2float(dst, src, mask);     // Read the positions with even indices in src.
asc_half2float_v2(dst, src, mask);  // Read the positions with odd indices in src.
```
