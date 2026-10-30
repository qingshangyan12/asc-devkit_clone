# asc_mul_cast_half2uint8_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:32:44.228Z pushedAt=2026-10-28T12:55:35.743Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Not supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Supported
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

The header file path is `"c_api/composite/vector_compute_composite.h"`.

**The `asc_mul_cast_half2uint8_sync` API is deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_fused/asc_mul_cast_half2uint8.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Computes the product element-wise and converts the result to the uint8_t type. The calculation formula is as follows:
$$
dst_i = Cast(src0_i * src1_i)
$$

- Precision conversion rule: The product of the source operands is rounded using the RINT rounding mode (round half to even), and overflow is handled by saturation.

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_mul_cast_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Start address of the destination operand (vector). |
| src0 | Input | Start address of source operand 0 (vector). |
| src1 | Input | Start address of source operand 1 (vector). |
| count | Input | Number of elements involved in the computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- The start addresses of dst, src0, and src1 must be 32-byte aligned.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).

## Examples

```cpp
// total_length indicates the length of data involved in the computation.
constexpr uint32_t total_length = 128;
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
__ubuf__ uint8_t dst[total_length];
asc_mul_cast_half2uint8_sync(dst, src0, src1, total_length);
```
