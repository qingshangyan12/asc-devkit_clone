# asc_float2bfloat16

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:54:34.250720+00:00 pushedAt=2026-10-27T02:26:34.116Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/vector_compute/compute/vector_convert.h"`.

Converts float data to bfloat16_t and supports multiple rounding modes:

- RINT rounding mode: round to nearest, ties to even.
- ROUND rounding mode: round half up.
- FLOOR rounding mode: round toward negative infinity.
- CEIL rounding mode: round toward positive infinity.
- TRUNC rounding mode: round toward zero.

## Prototype

- Computes the first n contiguous data elements.
    ```cpp
    // RINT rounding mode.
    __aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
    // ROUND rounding mode.
    __aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
    // FLOOR rounding mode.
    __aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
    // CEIL rounding mode.
    __aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
    // TRUNC rounding mode.
    __aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint32_t count)
    ```

- High-dimensional slicing computation

    ```cpp
    // RINT rounding mode.
    __aicore__ inline void asc_float2bfloat16_rn(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    // ROUND rounding mode.
    __aicore__ inline void asc_float2bfloat16_rna(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    // FLOOR rounding mode.
    __aicore__ inline void asc_float2bfloat16_rd(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    // CEIL rounding mode.
    __aicore__ inline void asc_float2bfloat16_ru(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    // TRUNC rounding mode.
    __aicore__ inline void asc_float2bfloat16_rz(__ubuf__ bfloat16_t* dst, __ubuf__ float* src, uint8_t repeat, uint16_t dst_block_stride, uint16_t src_block_stride, uint16_t dst_repeat_stride, uint16_t src_repeat_stride)
    ```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
|----|-----|-----|
| dst | Output | Start address of the destination operand (vector). |
| src  | Input | Start address of the source operand (vector).|
| count | Input | Number of elements involved in the computation. |
| repeat |Input | Number of iterations. |
| dst_block_stride |Input | Address stride between different DataBlocks of the destination operand within a single iteration. |
| src_block_stride |Input | Address stride between different DataBlocks of the source operand within a single iteration. |
| dst_repeat_stride |Input | Address stride of the same DataBlock of the destination operand between adjacent iterations. |
| src_repeat_stride |Input | Address stride of the same DataBlock of the source operand between adjacent iterations. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

## Examples

```cpp
constexpr uint32_t total_length = 256;
__ubuf__ float src[total_length];
__ubuf__ bfloat16_t dst[total_length];
asc_float2bfloat16_rn(dst, src, total_length);
```
