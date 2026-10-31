# asc_copy_gm2ub_align

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T03:54:34.334522+00:00 pushedAt=2026-10-27T02:26:34.687Z nl=2 -->

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
- Atlas inference products AI Core: Not supported
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas inference products Vector Core: Not supported
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas training products: Not supported
<!-- end id7 -->

## Description

The header file path is `"c_api/vector_datamove/vector_datamove.h"`.

Supports unaligned data copying from Global Memory (GM) to Unified Buffer (UB) for 8-bit/16-bit/32-bit data types.

This API supports the following two data movement modes:

- Copying the first n data items

    If the copied data length is not 32-byte aligned, the copied data is padded to 32-byte alignment. The following two padding modes are supported:

    - Manual padding: Call [asc_set_copy_pad_val](../asc_set_copy_pad_val.md) to configure the padding value before the copy.
    - Automatic padding: When `asc_set_copy_pad_val` is not called, the hardware automatically pads dummy data, and the value of the dummy data is the value of the first element of the data block.

- High-dimensional slicing data movement

    High-dimensional slicing data movement uses Normal mode. If the length of a single data block specified by `burst_len` is not 32-byte aligned, each data block specified by `burst_count` is padded to 32-byte alignment separately. The following two padding modes are supported for non-32-byte-aligned scenarios:

    - Manual padding: Call `asc_set_copy_pad_val` to set the padding value before the copy.
    - Automatic padding: When `asc_set_copy_pad_val` is not called, the hardware automatically pads dummy data, and the value of the dummy data is the value of the first element of the data block.

    For loop movement mode configuration APIs, see [asc_set_gm2ub_loop_size](../asc_set_gm2ub_loop_size.md), [asc_set_gm2ub_loop1_stride](../asc_set_gm2ub_loop1_stride.md), and [asc_set_gm2ub_loop2_stride](../asc_set_gm2ub_loop2_stride.md). Specifically, `asc_set_gm2ub_loop_size` is used to set the iteration counts of the two loops, and `asc_set_gm2ub_loop1_stride` and `asc_set_gm2ub_loop2_stride` are used to set the start address offsets of the source operand and destination operand data blocks between adjacent iterations of the inner loop `loop1` and the outer loop `loop2`, respectively. For product support information, the descriptions in the respective API documents prevail.

## Prototype

- Copying the first n data items

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
    ```

- High-dimensional slicing data movement

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint16_t burst_count, uint32_t burst_len, uint8_t left_padding_num, uint8_t right_padding_num, uint32_t src_gap, uint32_t dst_gap)
    ```

## Parameters

**Table 1** Parameters

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source GM. Must be 1-byte aligned. |
| size | Input | Size of the copied data, in bytes. Value range: [0, 2097151]. |
| burst_count | Input | Number of contiguous data blocks to be copied. Value range: [0, 4095]. |
| burst_len | Input | Length of each contiguous data block to be copied, in bytes. Value range: [0, 2097151]. |
| left_padding_num | Input | Number of elements to be padded on the left side of the contiguous data blocks to be copied. The padding data size corresponding to this parameter cannot exceed 32 bytes. |
| right_padding_num | Input | Number of elements to be padded on the right side of the contiguous data blocks to be copied. The padding data size corresponding to this parameter cannot exceed 32 bytes. |
| src_gap | Input | Gap between adjacent contiguous data blocks of the source operand (the gap between the end of the preceding data block and the start of the following data block), in bytes. |
| dst_gap | Input | Gap between adjacent contiguous data blocks of the destination operand (the gap between the end of the preceding data block and the start of the following data block), in DataBlock (32 bytes). |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../../general_description_and_constraints.md#storage-unit-description).
- When either `burst_count` or `burst_len` is 0, this API is treated as a NOP (no operation).
- When `size` is 0, this API is treated as a NOP (no operation).
- If the UB address ranges accessed by this instruction overlap with those accessed by other instructions, you must insert synchronization instructions [asc_sync_notify](../../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../../sync/intra_core_sync/asc_sync_wait.md) to ensure that the instructions execute serially and prevent abnormal data from occurring.
- The padding data size corresponding to `left_padding_num` and `right_padding_num` must not exceed 32 bytes.

## Examples

```cpp
// total_length refers to the total length of the data involved in the copy. src is the externally input GM buffer.
constexpr uint64_t total_length = 128;
__ubuf__ uint8_t dst[total_length];
asc_copy_gm2ub_align(dst, src, total_length * sizeof(uint8_t));
```
