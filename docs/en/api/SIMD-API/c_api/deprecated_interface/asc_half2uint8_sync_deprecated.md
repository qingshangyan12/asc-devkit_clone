# asc_half2uint8_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T02:52:21.335Z pushedAt=2026-10-28T12:55:35.658Z nl=2 -->

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

**The `asc_half2uint8_sync`, `asc_half2uint8_rn_sync`, `asc_half2uint8_rd_sync`, `asc_half2uint8_rna_sync`, `asc_half2uint8_ru_sync`, and `asc_half2uint8_rz_sync` APIs are deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_convert/asc_half2uint8.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Converts half data to uint8_t, supporting multiple rounding modes:

- NONE rounding mode: Represents the RINT rounding mode when the conversion involves precision loss, and represents no rounding when no precision loss is involved.
- RINT rounding mode: Round to nearest, with ties rounding to even.
- ROUND rounding mode: Round half away from zero.
- FLOOR rounding mode: Round toward negative infinity.
- CEIL rounding mode: Round toward positive infinity.
- TRUNC rounding mode: Round toward zero.

## Prototype

- Synchronous computation

    ```cpp
    // NONE rounding mode
    __aicore__ inline void asc_half2uint8_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    // RINT rounding mode
    __aicore__ inline void asc_half2uint8_rn_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    // FLOOR rounding mode
    __aicore__ inline void asc_half2uint8_rd_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    // ROUND rounding mode
    __aicore__ inline void asc_half2uint8_rna_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    // CEIL rounding mode
    __aicore__ inline void asc_half2uint8_ru_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    // TRUNC rounding mode
    __aicore__ inline void asc_half2uint8_rz_sync(__ubuf__ uint8_t* dst, __ubuf__ half* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand (vector). |
| count | Input | Number of elements involved in the computation. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- For operand address overlap constraints, see [General Address Overlap Constraints](../general_description_and_constraints.md#general-address-overlap-constraints).
- The start addresses of dst and src must be 32-byte aligned.

## Examples

```cpp
//total_length indicates the total length of the data to be converted.
constexpr uint32_t total_length = 128;
__ubuf__ half src[total_length];
__ubuf__ uint8_t dst[total_length];
asc_half2uint8_sync(dst, src, total_length);
```
