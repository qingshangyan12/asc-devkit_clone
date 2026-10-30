# asc_and (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:17:02.042Z pushedAt=2026-10-28T12:55:18.772Z nl=2 -->

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

**The floating-point overload of this API is deprecated. Use the [asc_and](../reg_compute/reg_logic/asc_and.md) integer overload of the same bit width based on the data type instead.**

Performs a bitwise AND (&) operation on the source operands `src0` and `src1` based on `mask`, and writes the result to the destination operand `dst`.

The calculation formula is as follows:

$$
dst_i = src0_i \& src1_i
$$

This API takes effect only on AIV.

## Prototype

```c
__simd_callee__ inline void asc_and(vector_<dtype>& dst,
                                    vector_<dtype> src0,
                                    vector_<dtype> src1,
                                    vector_bool mask)
```

### Data Types Supported by dtype

`dtype` can be one of the following: `fp8_e5m2_t`, `fp8_e4m3fn_t`, `half`, `bfloat16_t`, and `float`.

### Prototype Example

```c
__simd_callee__ inline void asc_and(vector_half& dst,
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
| mask | Input | Source operand mask (mask register). Elements whose corresponding bit is 1 participate in the computation, and elements whose corresponding bit is 0 do not. Elements that do not participate in the computation are set to 0 in the output. |

## Return Value

None

## Constraints

- This API returns directly when it is not called by the AIV.
- `mask` must be assigned a value in advance through the [mask setting API](../defs/type/data_type_definition.md#mask-register) before being passed in. The content of an unassigned mask register is undefined, which may cause incorrect positions of valid elements.
- The number of elements participating in the computation is VL divided by `sizeof(dtype)`.
- When a bit of `mask` is 0, the corresponding bit of `dst` is written as 0.

s

The following example uses the deprecated overload of the `half` type only to illustrate compatible invocation. For new code, migrate to the integer overload of the same bit width as described in Description.

```cpp
vector_half dst;
vector_half src0;
vector_half src1;
vector_bool mask = asc_create_mask_b16(PAT_ALL);
asc_and(dst, src0, src1, mask);
```
