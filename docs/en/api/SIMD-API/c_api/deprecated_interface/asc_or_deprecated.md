# asc_or (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:39:39.153Z pushedAt=2026-10-28T12:55:35.756Z nl=2 -->

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

**The floating-point overloads of this API are deprecated. Use the equal-bit-width integer overloads of [asc_or](../reg_compute/reg_logic/asc_or.md) instead, based on the data type.**

Performs a bitwise OR (|) operation on the source operands `src0` and `src1` according to `mask`, and writes the result to the destination operand `dst`.

The calculation formula is as follows:

$$
dst_i = src0_i \,|\, src1_i
$$

This API takes effect only on the AIV.

## Prototype

```c
__simd_callee__ inline void asc_or(vector_<dtype>& dst,
                                   vector_<dtype> src0,
                                   vector_<dtype> src1,
                                   vector_bool mask)
```

### Data Types Supported by dtype

`dtype` can be `half` or `float`.

### Prototype Example

```c
__simd_callee__ inline void asc_or(vector_half& dst,
                                   vector_half src0,
                                   vector_half src1,
                                   vector_bool mask)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :--- |
| dst | Output | Destination operand (vector data register). |
| src0 | Input | Source operand (vector data register). |
| src1 | Input | Source operand (vector data register). |
| mask | Input | Source operand mask (mask register). When the corresponding bit is 1, the element participates in the computation; when it is 0, the element does not participate, and elements that do not participate are set to 0 in the output. |

## Return Value

None

## Constraints

- This API returns directly when it is not called on the AIV.
- `mask` must be assigned a value through the [mask setting API](../defs/type/data_type_definition.md#mask-register) before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect positions of valid elements.
- The number of elements participating in the computation is VL divided by `sizeof(dtype)`.
- When a bit of `mask` is 0, the corresponding bit of `dst` is written as 0.

## Examples

The following example uses the deprecated `half` overload only to illustrate compatible calls. For new code, migrate to the integer overload of the same bit width as described in the description.

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_or(dst, src0, src1, mask);
```
