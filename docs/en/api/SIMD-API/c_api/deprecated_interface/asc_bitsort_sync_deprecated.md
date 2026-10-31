# asc_bitsort_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:23:11.162Z pushedAt=2026-10-28T12:55:18.784Z nl=2 -->

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

**The `asc_bitsort_sync` API is deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_sort/asc_bitsort.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

A bitmap sort function that sorts 32 elements in one iteration. The data is stored in the structure described as follows:

Score and Index are stored in src0 and src1 respectively. The elements are sorted by Score (elements with larger Score values come first). The sorted Score and its corresponding Index are stored in dst as a (Score, Index) structure.

## Prototype

- Synchronous computation
  ```cpp
  __aicore__ inline void asc_bitsort_sync(__ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
  __aicore__ inline void asc_bitsort_sync(__ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
  ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| --------- | ----- | ----------------- |
| dst | Output | Start address of the destination operand (vector). |
| src0 | Input | Start address of the source operand (vector). |
| src1 | Input | Start address of the source operand (vector). |
| repeat | Input | Number of repeated iterations. Each iteration sorts 32 elements. In the next iteration, src0 and src1 each skip 32 elements, and dst skips 256 bytes. Value range: [0, 255]. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

s

```cpp
__ubuf__ half dst[256];
__ubuf__ half src0[256];
__ubuf__ uint32_t src1[256];
asc_bitsort_sync(dst, src0, src1, 8);
```
