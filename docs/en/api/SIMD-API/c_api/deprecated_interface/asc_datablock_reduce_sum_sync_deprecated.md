# asc_datablock_reduce_sum_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:28:44.953Z pushedAt=2026-10-28T12:55:18.872Z nl=2 -->

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

**The `asc_datablock_reduce_sum_sync` API is deprecated. Use the [corresponding asynchronous API](../vector_compute/vector_reduce/asc_datablock_reduce_sum.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Sums all elements in each DataBlock.

Source operands are added pairwise using a binary tree. For example, to sum 128 half-type data elements, each DataBlock can compute 16 half-type data elements, which are divided into 8 DataBlocks for computation. Within each DataBlock, elements are added pairwise using a binary tree.

Note that during pairwise addition, if the computation result is greater than 65504, the result is stored as 65504. For example, if the source operands are [60000,60000,-30000,100], first 60000+60000 overflows and the result is 65504, then -30000+100=-29900 is computed, and finally 65504-29900=35604.

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_datablock_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :----| :-----| :-----|
| dst | Output | Start address of the destination operand (vector). |
| src  | Input | Start address of the source operand (vector). |
| count | Input | Number of elements involved in the computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- The start addresses of dst and src must be 32-byte aligned.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).


## Examples

```cpp
constexpr uint32_t src_length = 256;
constexpr uint32_t dst_length = 16;
__ubuf__ half src[src_length];
__ubuf__ half dst[dst_length];

// Use the high-dimensional split computation API. Set the mask.
asc_set_mask_norm();
asc_set_vector_mask(0xffffffffffffffff, 0xffffffffffffffff);    // Set all elements in each iteration to participate in the computation.

// Repeat 256B each time, repeat twice, with no interval.
asc_datablock_reduce_sum_sync(dst, src, 2, 1, 1, 8);
```

Result example:

```
Input data src: [1 1 1 ... 1]
Output data dst: [16 16 16 ... 16]
```
