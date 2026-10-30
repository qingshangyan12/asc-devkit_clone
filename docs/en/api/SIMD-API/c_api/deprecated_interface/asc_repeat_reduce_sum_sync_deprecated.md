# asc_repeat_reduce_sum_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:45:37.528Z pushedAt=2026-10-28T12:55:35.768Z nl=2 -->

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

**The `asc_repeat_reduce_sum_sync` API is deprecated. Use the [corresponding asynchronous API](../vector_compute/vector_reduce/asc_repeat_reduce_sum.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Sums all elements within each Repeat.

The source operands are added using a binary tree approach with pairwise addition. Taking the summation of 256 half-type data as an example, each Repeat can compute 128 half-type data, which are divided into two Repeats for computation. Within each Repeat, pairwise addition is performed using a binary tree approach.

Note that during pairwise addition, when a computation result is greater than 65504, the result is saved as 65504. For example, given source operands [60000, 60000, -30000, 100], first 60000 + 60000 overflows, and the result is 65504; then -30000 + 100 = -29900 is computed; finally 65504 - 29900 = 35604 is computed.

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_repeat_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|----|-----|-----|
| dst | Output | Start address of the destination operand (vector). |
| src  | Input | Start address of the source operand (vector). |
| count | Input | Number of elements involved in the computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start address of dst must be 2-byte aligned when the input type is half, and 4-byte aligned when the input type is float.
- The start address of src must be 32-byte aligned.


## Examples


```cpp
// Calculate the sum of 256 half values in two iterations. The result consists of two groups of sums.
constexpr uint32_t total_length = 256;
__ubuf__ half src[total_length];
constexpr uint32_t dst_length = 2;
__ubuf__ half dst[dst_length];

// Use the high-dimensional split computation API, which requires setting the mask.
asc_set_mask_norm();
asc_set_vector_mask(0xffffffffffffffff, 0xffffffffffffffff);    // Set all elements in each iteration to participate in the computation.

uint8_t repeat = 2;
// The unit is 2 bytes, stored contiguously.
uint16_t dst_repeat_stride = 1;
uint16_t src_block_stride = 1;
uint16_t src_repeat_stride = 8;
asc_repeat_reduce_sum_sync(dst, src, repeat, dst_repeat_stride, src_block_stride, src_repeat_stride);
```

Result example:

```
Input data src: [1, 1, 1, ..., 1]
Output data dst: [128, 128]
```
