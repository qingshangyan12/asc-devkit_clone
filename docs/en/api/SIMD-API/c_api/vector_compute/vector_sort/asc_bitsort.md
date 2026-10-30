# asc_bitsort

<!-- md-trans-meta sourceCommit=e430b2c74d537898700e8c0eb634eda72eb98460 translatedAt=2026-10-23T03:54:34.324148+00:00 pushedAt=2026-10-27T02:26:34.619Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/vector_compute/compute/vector_sort.h"`.

Bitmap sorting function. Each iteration sorts 32 elements, and the data is stored in the following structure:

Score and Index are stored in src0 and src1, respectively. Sorting is performed by Score in descending order. The sorted Score and its corresponding Index are stored together in dst in the (Score, Index) structure.

## Prototype

```cpp
// Placeholder form.
__aicore__ inline void asc_bitsort(__ubuf__ <dtype>* dst,
                                   __ubuf__ <dtype>* src0,
                                   __ubuf__ uint32_t* src1,
                                   int32_t repeat)
```

### Data Types Supported by dtype

The data types supported by `dtype` are `half` and `float`.

### Typical Prototype Example

```cpp
__aicore__ inline void asc_bitsort(__ubuf__ float* dst,
                                   __ubuf__ float* src0,
                                   __ubuf__ uint32_t* src1,
                                   int32_t repeat)
```

## Parameters

**Table 1** Parameters

| Parameter       | Input/Output | Description                |
| --------- | ----- | ----------------- |
| dst       | Output    | Start address of the destination operand (vector). |
| src0      | Input    | Start address of the source operand (vector). |
| src1      | Input    | Start address of the source operand (vector). |
| repeat    | Input    | Number of iterations. Each iteration sorts 32 elements. For the next iteration, src0 and src1 each advance by 32 elements, and dst advances by 256 Byte. Value range: [0, 255]. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

## Examples

```cpp
__ubuf__ half dst[256];
__ubuf__ half src0[256];
__ubuf__ uint32_t src1[256];
asc_bitsort(dst, src0, src1, 8);
```
