# asc_brcb_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:24:18.209Z pushedAt=2026-10-28T12:55:18.785Z nl=2 -->

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

**The `asc_brcb_sync` API is deprecated. Use the [corresponding non-synchronous API](../vector_compute/vector_broadcast/asc_brcb.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Given an input vector, each time 8 numbers are taken from the input vector and filled into 8 datablocks (32Bytes) of the result vector, with each number corresponding to one datablock.

## Prototype

- Synchronous high-dimensional split computation

    ```cpp
    __aicore__ inline void asc_brcb_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
    __aicore__ inline void asc_brcb_sync(__ubuf__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t dst_block_stride, uint16_t dst_repeat_stride, uint8_t repeat)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand (vector). |
| dst_block_stride | Input | Address stride between different DataBlocks within a single iteration of the target operand. |
| dst_repeat_stride | Input | Address stride of the same DataBlock between adjacent iterations of the target operand. |
| repeat | Input | Number of iterations. |

## Return Value

None

## Pipeline Type

PIPE_V

## Constraints

- src and dst cannot be the same memory address.

s

```cpp
constexpr uint32_t src_length = 16;
constexpr uint32_t dst_length = 256;
__ubuf__ uint16_t src[src_length];
__ubuf__ uint16_t dst[dst_length];
asc_brcb_sync(dst, src, 1, 8, 2);
```

Result example:

```
Input data src: [1 2 3 ... 16]
Output data dst: [1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 2 ... 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 15 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16 16]
```
