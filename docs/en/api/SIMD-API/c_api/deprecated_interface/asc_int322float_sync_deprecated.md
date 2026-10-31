# asc_int322float_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:00:39.155Z pushedAt=2026-10-28T12:55:35.674Z nl=2 -->

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

**The `asc_int322float_sync`, `asc_int322float_rn_sync`, `asc_int322float_rna_sync`, `asc_int322float_rd_sync`, `asc_int322float_ru_sync`, and `asc_int322float_rz_sync` APIs are deprecated. Use the [corresponding asynchronous API](../vector_compute/vector_convert/asc_int322float.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Converts int32_t data to float, supporting multiple rounding modes:

- RINT rounding mode: round half to even.
- ROUND rounding mode: round half away from zero.
- FLOOR rounding mode: round toward negative infinity.
- CEIL rounding mode: round toward positive infinity.
- TRUNC rounding mode: round toward zero.

## Prototype

- Synchronous computation

    ```cpp
  // Indicates RINT rounding mode when the conversion has precision loss, and no rounding when there is no precision loss.
  __aicore__ inline void asc_int322float_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // RINT rounding mode
  __aicore__ inline void asc_int322float_rn_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // ROUND rounding mode
  __aicore__ inline void asc_int322float_rna_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // FLOOR rounding mode
  __aicore__ inline void asc_int322float_rd_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // CEIL rounding mode
  __aicore__ inline void asc_int322float_ru_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
  // TRUNC rounding mode
  __aicore__ inline void asc_int322float_rz_sync(__ubuf__ float* dst, __ubuf__ int32_t* src, uint32_t count)
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
// total_length indicates the length of data involved in the computation.
constexpr uint32_t total_length = 32;
__ubuf__ float dst[total_length];
__ubuf__ int32_t src[total_length];
asc_int322float_sync(dst, src, total_length);
```
