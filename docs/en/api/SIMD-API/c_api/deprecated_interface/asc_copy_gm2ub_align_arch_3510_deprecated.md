# asc_copy_gm2ub_align_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:40:48.646Z pushedAt=2026-10-28T12:55:18.811Z nl=2 -->

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

**The high-dimensional splitting data movement API is deprecated. Use the `asc_load_l2_cache_mode` type enumeration values to configure the L2 Cache management policy. For the new API form, see [asc_copy_gm2ub_align high-dimensional slicing data movement API](../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md#high-dimensional-slicing-data-movement).**

Provides unaligned data movement, moving data from Global Memory (GM) to Unified Buffer (UB), and supports 8-bit/16-bit/32-bit data type movement.

This API uses the high-dimensional splitting movement mode. If the moved data length is not 32-byte aligned, the moved data is padded to 32-byte alignment. You can select Normal mode or Compact mode by configuring the `dst_stride` parameter. The following two padding modes are supported in non-32-byte-aligned scenarios:

- Manual padding: Call `asc_set_copy_pad_val` to set the padding value before the movement, and set `enable_constant_pad` to `true`.
- Automatic padding: Set `enable_constant_pad` to `false`, and the hardware automatically pads dummy data, whose value is the value of the first element of the data block.

The data movement modes are described as follows:

- Compact mode: Set `dst_stride` to `len_burst`, and set `left_padding_num` and `right_padding_num` to 0. Multiple data blocks are tightly arranged in the UB and are treated as a whole for 32-byte padding. If the total length of all data blocks is not 32-byte aligned, only the end of the whole is padded to 32-byte alignment using the manual or automatic padding mode described above.
- Normal mode: Set `dst_stride` to a value that is not equal to `len_burst` and is 32-byte aligned. The interval between the start addresses of adjacent destination data blocks is specified by `dst_stride`, and intervals can be reserved between data blocks without tight arrangement.
    - When both `left_padding_num` and `right_padding_num` are 0, each data block is padded to 32-byte alignment individually using the manual or automatic padding mode described above.
    - When `left_padding_num` or `right_padding_num` is non-zero, `enable_constant_pad` does not take effect, and you must call `asc_set_copy_pad_val` to configure the padding value before the copy. If the data block length plus the total bytes of the left and right padding is not 32-byte aligned, the hardware continues to pad dummy data on the right side to 32-byte alignment, and the value of the dummy data is the value configured by `asc_set_copy_pad_val`.

For example, when copying three data blocks of 48 bytes each without left or right padding:

- In Compact mode, set `dst_stride` to 48. The start offsets of the data blocks in the UB are 0, 48, and 96, respectively, and 16 bytes are padded only at the end of the 144 bytes of valid data.
- In Normal mode, set `dst_stride` to 64. The start offsets of the data blocks in the UB are 0, 64, and 128, respectively, and each data block is padded with 16 bytes separately.

When only one data block is copied, or when `len_burst` is already 32-byte aligned and there is no left or right padding, the copy results of the two modes are the same.

## Prototype

- High-dimensional splitting copy

    ```cpp
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    __aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint16_t n_burst, uint32_t len_burst, uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
    ```


## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source GM. Must be 1-byte aligned. |
| n_burst | Input | Number of consecutive data blocks to copy. Value range: [0, 4095]. |
| len_burst | Input | Length of each consecutive data block to copy, in bytes. Value range: [0, 2097151]. |
| left_padding_num | Input | Number of elements to pad on the left side of the consecutive data blocks. The padding data size corresponding to this parameter cannot exceed 32 bytes. Must be set to 0 in Compact mode. |
| right_padding_num | Input | Number of elements to pad on the right side of the consecutive data blocks. The padding data size corresponding to this parameter cannot exceed 32 bytes. Must be set to 0 in Compact mode. |
| enable_constant_pad | Input | When both `left_padding_num` and `right_padding_num` are 0, configures the padding mode for non-aligned scenarios. The values are described as follows:  <br>&bull; `true`: Manual padding, where the padding value is the value set by the `asc_set_copy_pad_val` API. <br>&bull; `false`: Automatic padding, where the hardware pads dummy data, and the value of the dummy data is the value of the first element of the data block.<br>When `left_padding_num` or `right_padding_num` is non-zero, this parameter does not take effect. |
| l2_cache_mode | Input | L2 Cache management policy configuration. The type of this parameter is `uint8_t`. It is recommended to use an enumeration value of the [asc_load_l2_cache_mode](../defs/enum/asc_load_l2_cache_mode.md) type instead. |
| src_stride | Input | Distance between adjacent consecutive data blocks of the source operand (the interval between the head of the preceding data block and the head of the following data block), in bytes.<br>When only one data block is copied, that is, when `n_burst` is set to 1, this parameter can be set to 0. |
| dst_stride | Input | Distance between adjacent consecutive data blocks of the destination operand (the interval between the head of the preceding data block and the head of the following data block), in bytes, used to select the data copy mode.<br>&bull; Equal to `len_burst`: Compact mode, where the destination data blocks are tightly arranged in the UB, and `dst_stride` supports byte alignment.<br>&bull; Not equal to `len_burst`: Normal mode, where `dst_stride` must meet the 32-byte alignment requirement.<br>When only one data block is copied, that is, when `n_burst` is set to 1, this parameter can be set to 0. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- When either `n_burst` or `len_burst` is 0, this API is treated as a NOP (no operation).
- If you need to execute multiple `asc_copy_gm2ub_align` instructions and the destination addresses of the `asc_copy_gm2ub_align` instructions overlap, insert synchronization instructions ([asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md)) to serialize the multiple `asc_copy_gm2ub_align` instructions and prevent abnormal data.
- When `left_padding_num` or `right_padding_num` is non-zero, `enable_constant_pad` does not take effect. You must call `asc_set_copy_pad_val` to configure the padding value before the movement. The padding data size corresponding to `left_padding_num` and `right_padding_num` must not exceed 32 bytes each.
- When `dst_stride` is not equal to `len_burst`, `dst_stride` must be 32-byte aligned.
