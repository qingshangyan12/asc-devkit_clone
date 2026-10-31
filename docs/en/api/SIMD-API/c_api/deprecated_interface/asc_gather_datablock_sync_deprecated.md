# asc_gather_datablock_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:34:26.894Z pushedAt=2026-10-28T12:55:35.623Z nl=2 -->

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

**The `asc_gather_datablock_sync` API is deprecated. Use the [corresponding asynchronous API](../vector_compute/vector_gather/asc_gather_datablock.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Given an input vector and an address offset vector, this API gathers the input vector into the destination vector at the DataBlock granularity according to the offset addresses.

## Prototype

- Synchronous computation

  ```cpp
  __aicore__ inline void asc_gather_datablock_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat)
  __aicore__ inline void asc_gather_datablock_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, __ubuf__ uint32_t* src_offset, uint16_t dst_repeat_stride, uint8_t dst_block_stride, uint8_t repeat)
  ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand (vector). |
| src_offset | Input | Address offset of each DataBlock in the source operand. |
| dst_repeat_stride | Input | Address stride of the same DataBlock between adjacent iterations of the destination operand. |
| dst_block_stride | Input | Address stride between different DataBlocks within a single iteration of the destination operand. |
| repeat | Input | Number of iterations. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints
- The value of src_offset must ensure bit-width alignment of the src element type.
- The address obtained by adding src_offset to src must not exceed the range of the Unified Buffer (UB) data size.
- The starting addresses of dst and src must be 32-byte aligned.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).

## Examples

```cpp
constexpr uint64_t total_length = 128;    // total_length indicates the data length involved in the computation.
__ubuf__ uint32_t src_offset[total_length * sizeof(uint32_t) / 32];
__ubuf__ uint32_t dst[total_length];
__ubuf__ uint32_t src[total_length];
asc_gather_datablock_sync(dst, src, src_offset, 8, 1, 2);
// src [1, 2, 3, ..., 126, 127, 128] 128 uint32_t data elements
// src_offset [0, 32, 64, ..., 416, 448, 480]
// dst [1, 2, 3, ..., 126, 127, 128]
```
