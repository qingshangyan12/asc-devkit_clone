# asc_mrgsort4_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:28:16.214Z pushedAt=2026-10-28T12:55:35.737Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
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

**The `asc_mrgsort4_sync` API is deprecated. Use the [corresponding asynchronous API](../vector_compute/vector_sort/asc_mrgsort4.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Merges up to four already-sorted queues into a single queue, with the results sorted in descending order by the score field.

The data processed by asc_mrgsort4 is generally the output of the asc_bitsort instruction. The queue structure Region Proposal is as follows:
- The data type is float, and each structure occupies 8 bytes.

![ ](../../../figures/mrgsort4_01.png)

- The data type is half, and each structure also occupies 8 bytes, with 2 bytes reserved in the middle.

![ ](../../../figures/mrgsort4_02.png)

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_mrgsort4_sync(__ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension, uint8_t valid_bit)
    __aicore__ inline void asc_mrgsort4_sync(__ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0, uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension, uint8_t valid_bit)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| ------------ | ------------ | ------------ |
| dst | Output | Start address of the destination operand (vector). |
| repeat | Input | Number of iterations. In each iteration, the source and destination operands are computed and the total length of the four queues is skipped. |
| element_length_0 | Input | Length of source queue 0. |
| element_length_1 | Input | Length of source queue 1. |
| element_length_2 | Input | Length of source queue 2. |
| element_length_3 | Input | Length of source queue 3. |
| if_exhausted_suspension | Input | Whether the instruction needs to stop after a queue is exhausted. |
| valid_bit | Input | Number of valid queues. Valid values are as follows:<br>3: The first two queues are valid.<br>7: The first three queues are valid.<br>15: All four queues are valid.  |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- When repeat=1, one iteration is performed, and the element_length_..., if_exhausted_suspension, and valid_bit parameters can be set freely.
- When repeat>1, multiple iterations are performed, and the following conditions must be met: four source queues are included, the four source queues have the same length and are stored contiguously in memory, valid_bit is 15, and if_exhausted_suspension is false.
- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

## Examples

```cpp
// This example performs merge sort on eight sorted Region Proposal queues, each containing 32 elements.
// Step 1: Sort four 32-element src queues at a time, with repeat set to 2, to obtain two sorted 128-element new tmp queues.
// Step 2: Perform merge sort on the two sorted 128-element new tmp queues to obtain the final 256-element dst queue.

constexpr uint16_t element_length = 32;
// Each element contains a float32 score and a 4-byte index, occupying 8 bytes, equivalent to 2 * sizeof(float).
constexpr uint16_t array_length = element_length * 2;
__ubuf__ float src_workspace[array_length * 8];
__ubuf__ float tmp_workspace[array_length * 8];
__ubuf__ float dst_workspace[array_length * 8];

__ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN];
__ubuf__ float* dst;
uint8_t valid_bit;

// Each iteration can process 4 queues, for a total of 2 iterations.
src[0] = &src_workspace[0];
src[1] = &src_workspace[1 * array_length];
src[2] = &src_workspace[2 * array_length];
src[3] = &src_workspace[3 * array_length];
dst = tmp_workspace;
valid_bit = 15;
asc_mrgsort4_sync(dst, src, 2, element_length, element_length, element_length, element_length, false, valid_bit);

// Iterate once to process 2 queues.
src[0] = &tmp_workspace[0];
src[1] = &tmp_workspace[1 * 4 * array_length];
dst = dst_workspace;
valid_bit = 3;
asc_mrgsort4_sync(dst, src, 1, 4 * element_length, 4 * element_length, 0, 0, false, valid_bit);
```
