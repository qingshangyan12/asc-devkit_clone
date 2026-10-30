# asc_lt_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:22:33.049Z pushedAt=2026-10-28T12:55:35.720Z nl=2 -->

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

**The `asc_lt_sync` API is deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_compare/asc_lt.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Compares src0 < src1 element by element. If the condition holds, the corresponding bit in the output result dst is set to 1; otherwise, it is set to 0.

## Prototype

- Synchronous computation

  ```cpp
  // Store the result in a register.
  __aicore__ inline void asc_lt_sync(__ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt_sync(__ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  // Output the result to the destination address.
  __aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src0, __ubuf__ half* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  __aicore__ inline void asc_lt_sync(__ubuf__ uint8_t* dst, __ubuf__ float* src0, __ubuf__ float* src1, uint8_t repeat, uint8_t dst_block_stride, uint8_t src0_block_stride, uint8_t src1_block_stride, uint8_t dst_repeat_stride, uint8_t src0_repeat_stride, uint8_t src1_repeat_stride)
  ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| ------------ | ------------ | ------------ |
| dst | Output | Start address of the destination operand (vector). |
| src0 | Input | Start address of the source operand (vector). |
| src1 | Input | Start address of the source operand (vector). |
| repeat | Input | Number of iterations. |
| dst_block_stride | Input | Address step between different DataBlocks within a single iteration of the destination operation. |
| src0_block_stride | Input | Address step between different DataBlocks within a single iteration of source operand 0. |
| src1_block_stride | Input | Address step between different DataBlocks within a single iteration of source operand 1. |
| dst_repeat_stride | Input | Address step between the same DataBlock across adjacent iterations of the destination operation. |
| src0_repeat_stride | Input | Address step between the same DataBlock across adjacent iterations of source operand 0. |
| src1_repeat_stride | Input | Address step between the same DataBlock across adjacent iterations of source operand 1. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst, src0, and src1 must be 32-byte aligned.
- dst is arranged into a binary result in little-endian order, corresponding to the comparison result of the data at the corresponding position in src.
- When the result is output to the destination address, the mask parameter does not take effect.
- When using the register-form API, use [asc_get_cmp_mask](../vector_compute/vector_compare/asc_get_cmp_mask.md) to obtain the comparison result.
- When the result is output to a register, a fixed 256B of data is processed, and the repeat field does not take effect.
- When the result is output to a register, if a bit in mask is 0 and the value of the corresponding bit in the output register does not change, the final result may retain residue from the previous computation. To initialize the register, call the [asc_set_cmp_mask](../vector_compute/vector_compare/asc_set_cmp_mask.md) API.

## Examples

```cpp
// The input is fixed to 128 elements.
constexpr uint32_t total_length = 128;
__ubuf__ uint8_t dst[total_length / 8];
__ubuf__ half src0[total_length];
__ubuf__ half src1[total_length];
uint8_t repeat = 1;
uint8_t dst_block_stride = 1;
uint8_t src0_block_stride = 1;
uint8_t src1_block_stride = 1;
uint8_t dst_repeat_stride = 8;
uint8_t src0_repeat_stride = 8;
uint8_t src1_repeat_stride = 8;
…… // Data transfer and synchronization operations.

// Store the result in a register and obtain the comparison result through the register.
asc_lt_sync(src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
…… // Synchronization operation.
asc_get_cmp_mask(dst); // Read the result.

// Output the result to the destination address.
asc_lt_sync(dst, src0, src1, repeat, dst_block_stride, src0_block_stride, src1_block_stride, dst_repeat_stride, src0_repeat_stride, src1_repeat_stride);
```
