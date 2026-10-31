# asc_copy_l12l0c_sync (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:15:35.416Z pushedAt=2026-10-28T12:55:18.852Z nl=2 -->

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

The header file path is `"c_api/composite/cube_datamove_composite.h"`.

**The `asc_copy_l12l0c_sync` API is deprecated. Use the [corresponding asynchronous API](../cube_datamove/asc_copy_l12l0c.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies a matrix from the L1 Buffer to the L0C Buffer. The supported data types are as follows:
- bfloat16_t->bfloat16_t
- half->half
- float->half
- float->bfloat16_t
- float->float
- int32_t->int32_t
- uint32_t->uint32_t

## Prototype

- Synchronous copy

    ```cpp
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ half* dst, __cbuf__ half* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ half* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ bfloat16_t* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ float* dst, __cbuf__ float* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ int32_t* dst, __cbuf__ int32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    __aicore__ inline void asc_copy_l12l0c_sync(__cc__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap, uint16_t dst_gap)
    ```

## Parameters


**Table 1** Parameter description

| Parameter | Input/Output | Description                                                              |
| :--- |:------|:----------------------------------------------------------------|
| dst | Output    | Start address of the destination operand.|
| src | Input    | Start address of the source operand.|
| n_burst | Input    | Number of consecutive data blocks to copy. Value range: [1, 4095]. |
| len_burst | Input    | Length of each contiguous data block to be copied, in 32 bytes. Value range: [1, 65535]. |
| src_gap | Input    | Gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the source operand. |
| dst_gap | Input    | Gap between the end address of the first fractal of the previous iteration and the start address of the first fractal of the next iteration of the destination operand. |

## Return Value

None

## Pipeline Type

PIPE_MTE1

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If multiple asc_copy_l12l0c instructions need to be executed and their destination addresses overlap, insert synchronization instructions to serialize the multiple asc_copy_l12l0c instructions and prevent abnormal data.

## Examples

```cpp
// One contiguous data block to be copied.
constexpr uint16_t n_burst = 1;
// The length of each contiguous data block to be copied is 32 bytes.
constexpr uint16_t len_burst = 1;
// The gap between the start addresses of the previous and next fractals of the source operand in adjacent iterations is 64 bytes.
constexpr uint16_t src_gap = 1;
// The gap between the start addresses of the previous and next fractals of the destination operand in adjacent iterations is 32 bytes.
constexpr uint16_t dst_gap = 0;
__cbuf__ half src[256];
__cc__ half dst[256];
asc_copy_l12l0c_sync(dst, src, n_burst, len_burst, src_gap, dst_gap);
```
