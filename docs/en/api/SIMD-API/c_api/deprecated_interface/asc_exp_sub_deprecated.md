# asc_exp_sub (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:38:46.493Z pushedAt=2026-10-28T12:55:18.889Z nl=2 -->

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

The header file path is `"c_api/reg_compute/compute/reg_fused.h"`.

**The `asc_exp_sub` prototype with the `half` input data type and without the `src_pos` parameter, as well as the `asc_exp_sub_v2` prototype, are deprecated. For the new API, see [asc_exp_sub](../reg_compute/reg_fused/asc_exp_sub.md).**

These APIs subtract `src1` from `src0` element-wise, compute the natural exponential function with the difference as the exponent, and write the result to `dst`.

### Migration Guide

**Table 1** Mapping between legacy and new prototypes

| Legacy prototype | New prototype |
| ------ | ------ |
| `asc_exp_sub(dst, vector_half src0, vector_half src1, mask)` | `asc_exp_sub_half2float(dst, src0, src1, mask, ASC_POSITION_EVEN)` |
| `asc_exp_sub_v2(dst, vector_half src0, vector_half src1, mask)` | `asc_exp_sub_half2float(dst, src0, src1, mask, ASC_POSITION_ODD)` |
| `asc_exp_sub_v2(dst, vector_float src0, vector_float src1, mask)` | `asc_exp_sub(dst, src0, src1, mask)` |

## Prototype

```c
__simd_callee__ inline void asc_exp_sub(vector_float& dst,
                                        vector_half src0,
                                        vector_half src1,
                                        vector_bool mask)

__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst,
                                           vector_half src0,
                                           vector_half src1,
                                           vector_bool mask)

__simd_callee__ inline void asc_exp_sub_v2(vector_float& dst,
                                           vector_float src0,
                                           vector_float src1,
                                           vector_bool mask)

```

## Parameters

**Table 2** Parameter description

| Parameter | Input/Output | Description |
| ------ | --------- | ---- |
| dst | Output | Destination operand (vector data register). |
| src0 | Input | Source operand 0 (vector data register). |
| src1 | Input | Source operand 1 (vector data register). |
| mask | Input | Mask register that indicates the elements involved in the computation. A value of 1 at the corresponding position means the element is involved in the computation, and 0 means it is not; elements not involved in the computation are set to 0 in the output. |

For details about vector data registers and mask registers, see [reg data type definition](../defs/type/data_type_definition.md).

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

None

## Examples

```cpp
vector_float dst;
vector_half src0;
vector_half src1;
vector_bool mask;

// Legacy API: Read the even-indexed elements of the half source operand.
asc_exp_sub(dst, src0, src1, mask);

// Replacement API: Explicitly specify the read position of the source operand.
asc_exp_sub_half2float(dst, src0, src1, mask, ASC_POSITION_EVEN);
```
