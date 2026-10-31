# asc_pair_reduce_sum_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:41:11.470Z pushedAt=2026-10-28T12:55:35.758Z nl=2 -->

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

**The `asc_pair_reduce_sum_sync` API is deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_reduce/asc_pair_reduce_sum.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Performs a reduction operation on the input data to obtain the sum. Adjacent (odd-even) elements are summed in pairs. For example, for (a1, a2, a3, a4, a5, a6...), the adjacent pairs are summed as (a1+a2, a3+a4, a5+a6...).

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ half* dst, __ubuf__ half* src, uint32_t count)
    __aicore__ inline void asc_pair_reduce_sum_sync(__ubuf__ float* dst, __ubuf__ float* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

|Parameter|Input/Output|Description|
|------------|------------|-----------|
| dst     | Output     | Start address of the destination operand (vector). |
| src     | Input     | Start address of the source operand (vector). |
| count   | Input     | Number of elements involved in the continuous computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

## Examples

```cpp
// Example: Compute the sum of adjacent elements of 256 half values in 2 iterations, processing 128 elements per iteration and outputting 64 results.
constexpr uint32_t src_length = 256;
constexpr uint32_t dst_length = 128;
__ubuf__ half src[src_length];
__ubuf__ half dst[dst_length];

// Use the high-dimensional split computation API. Set the mask.
asc_set_mask_norm();
asc_set_vector_mask(0xffffffffffffffff, 0xffffffffffffffff);    // Set all elements in each iteration to participate in the computation.

// 256B per repeat, 2 repeats, no gap.
asc_pair_reduce_sum_sync(dst, src, 2, 1, 1, 8);
```

Result example:

```
Input data src: [1 2 3 ... 16 17 ... 32 ... 225... 256]
Output data dst: [3 7 ... 511]
```
