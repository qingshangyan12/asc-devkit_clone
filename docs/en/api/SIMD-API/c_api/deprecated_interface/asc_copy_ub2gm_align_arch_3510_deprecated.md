# asc_copy_ub2gm_align_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T01:19:58.112Z pushedAt=2026-10-28T12:55:18.857Z nl=2 -->

## Applicable Products

<!-- npu="950" id1 -->
- Ascend 950PR&950DT products: Supported
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 products: Not supported
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 products: Not supported
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

**The high-dimensional splitting data movement API is deprecated. Use the `asc_store_l2_cache_mode` type enumeration to configure the L2 Cache management policy. For the new API, see [asc_copy_ub2gm_align high-dimensional slicing data movement API](../vector_datamove/asc_copy_ub2gm_align/asc_copy_ub2gm_align_arch_3510.md#high-dimensional-slicing-data-movement).**

Provides non-aligned data movement, moving data from the Unified Buffer (UB) to Global Memory (GM), and supports 8-bit/16-bit/32-bit data type movement.

This API uses the high-dimensional splitting movement method and does not support setting a fill value. If the length of the data to be moved is not 32-byte aligned, the hardware supplements dummy data when reading UB data to pad the moved data to 32-byte alignment. The supplemented dummy data is discarded when writing to GM. You can select Normal mode or Compact mode by configuring the `src_stride` parameter:

- Compact mode: Set `src_stride` to `len_burst`. Multiple data blocks are tightly arranged in the UB and are treated as a whole for 32-byte padding. If the total length of all data blocks is not 32-byte aligned, dummy data is supplemented only at the end of the whole to reach 32-byte alignment, and the supplemented dummy data is discarded when writing to GM.
- Normal mode: Set `src_stride` to a value that is not equal to `len_burst` and satisfies 32-byte alignment. Each data block is individually processed for 32-byte alignment.
    - If `len_burst` is 32-byte aligned, no dummy data is read, and all data read from the UB is written to GM.
    - If `len_burst` is not 32-byte aligned, dummy data is supplemented to reach 32-byte alignment when reading UB data, and the supplemented dummy data is discarded when writing to GM.

For example, when moving three data blocks of 48 bytes each:

- In Compact mode, set `src_stride` to 48. The starting offsets of the data blocks in the UB are 0, 48, and 96, and 16 bytes of dummy data are supplemented only at the end of the 144 bytes of valid data.
- In Normal mode, set `src_stride` to 64. The start offsets of the data blocks in the UB are 0, 64, and 128 in sequence, and 16 bytes of dummy data are appended to each data block.

In both modes, the dummy data is not written to the GM. When only one data block is copied, or when `len_burst` is already 32-byte aligned, the copy results of the two modes are the same.

## Prototype

- High-dimensional splitting copy

    ```cpp
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    __aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint16_t n_burst, uint32_t len_burst, uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
    ```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination GM. Requires 1-byte alignment. |
| src | Input | Start address of the source UB. Requires 32-byte alignment. |
| n_burst | Input | Number of consecutive data blocks to copy. Value range: [0, 4095]. |
| len_burst | Input | Length of each consecutive data block to copy, in bytes. Value range: [0, 2097151]. |
| l2_cache_mode | Input | L2 Cache management policy configuration. The type of this parameter is `uint8_t`. It is recommended to use an enumeration value of the [asc_store_l2_cache_mode](../defs/enum/asc_store_l2_cache_mode.md) type instead. |
| dst_stride | Input | Distance between adjacent consecutive data blocks of the destination operand (the interval between the head of the preceding data block and the head of the following data block), in bytes.<br>When only one data block is copied, that is, when `n_burst` is set to 1, this parameter can be set to 0. |
| src_stride | Input | Distance between adjacent consecutive data blocks of the source operand (the interval between the head of the preceding data block and the head of the following data block), in bytes. It is used to select the data copy mode.<br>&bull; Equal to `len_burst`: Compact mode. The source data blocks are tightly arranged in the UB, and `src_stride` supports byte alignment.<br>&bull; Not equal to `len_burst`: Normal mode. `src_stride` must meet the 32-byte alignment requirement. |

## Return Value

None

## Pipeline Type

PIPE_MTE3

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- When either `n_burst` or `len_burst` is 0, this API is treated as a NOP (no operation).
- If multiple `asc_copy_ub2gm_align` instructions need to be executed and the destination addresses of the `asc_copy_ub2gm_align` instructions overlap, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to ensure serialization of multiple `asc_copy_ub2gm_align` instructions and prevent abnormal data.
- When `src_stride` is not equal to `len_burst`, `src_stride` must be 32-byte aligned.
