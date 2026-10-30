# asc_deq_int162b8_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:28:54.268Z pushedAt=2026-10-28T12:55:18.874Z nl=2 -->

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

**The `asc_deq_int162b8_h_sync` and `asc_deq_int162b8_l_sync` APIs are deprecated. Use the [corresponding asynchronous APIs](../vector_compute/vector_fused/asc_deq_int162b8.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Converts int16_t data to int8_t or uint8_t and stores the data in the high half-block or low half-block of each DataBlock. Before using this API, call [asc_set_deq_scale](../vector_compute/vector_fused/asc_set_deq_scale.md) to set the quantization parameters.

- asc_deq_int162b8_h: Stores the data in the high half-block of each DataBlock.
- asc_deq_int162b8_l: Stores the data in the low half-block of each DataBlock.

As shown in the following figure:
![](../../../figures/asc_deq_int162b8_halfblock.png)

The calculation formula is as follows:

$$
dst_i = (src_i * scale) + offset
$$

## Prototype

- Synchronous computation

    ```cpp
    __aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
    __aicore__ inline void asc_deq_int162b8_h_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
    __aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ int8_t* dst, __ubuf__ int16_t* src, uint32_t count)
    __aicore__ inline void asc_deq_int162b8_l_sync(__ubuf__ uint8_t* dst, __ubuf__ int16_t* src, uint32_t count)
    ```

## Parameters

**Table 1** Parameter description

| Parameter  | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand (vector). |
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
constexpr uint64_t total_length = 128;    // total_length indicates the length of data involved in the computation.
__ubuf__ int16_t src[total_length];
__ubuf__ int8_t dst[total_length];
float scale = 1.0;        // The quantization parameter is 1.
int16_t offset = 0;       // No offset.
bool sign_mode = true;    // The quantization result is signed (dst is of the int8_t type).
asc_set_deq_scale(scale, offset, sign_mode);    // The calculation formula is dst = src.
asc_deq_int162b8_h_sync(dst, src, total_length);    // Convert src to the int8_t type and store it in the high half block of each DataBlock in dst.
```
