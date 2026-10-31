# asc_copy_gm2l1_sync_arch_2201 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:38:13.101Z pushedAt=2026-10-28T12:55:18.808Z nl=2 -->

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

**The `asc_copy_gm2l1_sync`, `asc_copy_gm2l1_pad1_sync`, `asc_copy_gm2l1_pad2_sync`, `asc_copy_gm2l1_pad3_sync`, `asc_copy_gm2l1_pad4_sync`, `asc_copy_gm2l1_pad5_sync`, `asc_copy_gm2l1_pad6_sync`, `asc_copy_gm2l1_pad7_sync`, and `asc_copy_gm2l1_pad8_sync` APIs are deprecated. Use the [corresponding non-synchronous APIs](../cube_datamove/asc_copy_gm2l1/asc_copy_gm2l1_arch_2201.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Copies cube data from Global Memory to L1 Buffer and supports different pad modes.

## Prototype

- Normal copy mode (synchronous)

    ```cpp
    // No pad operation is performed.
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // In units of 32B, insert 31B of pad for every 1B.
    __aicore__ inline void asc_copy_gm2l1_pad1_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // In units of 32B, insert 15B of pad for every 1B.
    __aicore__ inline void asc_copy_gm2l1_pad2_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // In units of 32B, insert 14B of pad for every 2B.
    __aicore__ inline void asc_copy_gm2l1_pad3_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // In units of 32B, insert 12B of pad for every 4B.
    __aicore__ inline void asc_copy_gm2l1_pad4_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // In units of 32B, insert 8B of pad for every 8B.
    __aicore__ inline void asc_copy_gm2l1_pad5_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // Delete the highest 28B of every 32B and store the lowest 4B contiguously.
    __aicore__ inline void asc_copy_gm2l1_pad6_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // Delete the highest 24B of every 32B and store the lowest 8B contiguously.
    __aicore__ inline void asc_copy_gm2l1_pad7_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    // Delete the highest 16B of every 32B and store the lowest 16B contiguously.
    __aicore__ inline void asc_copy_gm2l1_pad8_sync(__cbuf__ void* dst, __gm__ void* src, uint32_t size)
    ```

- 2D copy mode (synchronous)

    ```cpp
    // Copy bfloat16_t matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy half matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ half* dst, __gm__ half* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy float matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ float* dst, __gm__ float* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy int32_t matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int32_t* dst, __gm__ int32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy int8_t matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ int8_t* dst, __gm__ int8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy uint32_t matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    // Copy uint8_t matrix data in 2D mode (synchronous).
    __aicore__ inline void asc_copy_gm2l1_sync(__cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint16_t base_idx, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap)
    ```

## Parameters

### Normal Copy Mode Parameters

**Table 1** Normal copy mode parameters

| Parameter       | Input/Output | Description                                                              |
|:----------|:------|:----------------------------------------------------------------|
| dst       | Output    | Start address of the destination operand (vector).                                                 |
| src       | Input    | Start address of the source operand.                                                      |
| size      | Input    | Size of the data to copy (in bytes).                                                     |
| dst_gap   | Input    | Gap between adjacent consecutive data blocks of the destination operand (the gap between the end of the preceding data block and the start of the following data block).<br>Unit: DataBlock (32 bytes). |

### 2D Copy Mode Parameters

**Table 2** 2D copy mode parameters

| Parameter  | Input/Output | Description |
| :----- | :------- | :------- |
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand (vector). |
| base_idx | Input | Fractal cube ID, indicating that the copy starts from the specified fractal in the source operand (0 indicates the first fractal cube in the source operand). Value range: [0, 65535]. Unit: 512B. Default value: 0. |
| repeat | Input | Number of iterations. |
| src_stride |Input| Interval between the start addresses of the previous and next fractals of the source operand in adjacent iterations. Unit: 512B. Value range: [0, 65535]. Default value: 0. |
| dst_gap | Input | Interval between the end address of the previous fractal and the start address of the next fractal of the destination operand in adjacent iterations. Unit: 512B. Value range: [0, 65536]. Default value: 0. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If multiple asc_copy_gm2l1 instructions need to be executed and their destination addresses overlap, insert synchronization instructions to serialize the multiple asc_copy_gm2l1 instructions and prevent abnormal data.

s

```cpp
// Normal data copy mode.
// total_length indicates the total length of the data to be copied.
constexpr uint64_t total_length = 128;
__cbuf__ half dst[total_length];
// Number of consecutive data blocks to be copied.
constexpr uint16_t n_burst = 1;
// Length of each consecutive data block, which is 512 half elements here.
constexpr uint16_t len_burst = 512 * sizeof(half) / 32;
// The source operand is copied consecutively.
constexpr uint16_t src_gap = 0;
// The destination operand is laid out consecutively.
constexpr uint16_t dst_gap = 0;
// src indicates the start address of the source operand.
asc_copy_gm2l1_sync((__cbuf__ void*)dst, (__gm__ void*)src, n_burst, len_burst, src_gap, dst_gap);

// 2D data copy mode.
// The starting block of the copy is 1.
constexpr uint16_t base_idx = 1;
// The number of copy iterations is 2.
constexpr uint8_t repeat = 2;
// The input copy stride is 0 bytes, and the output copy stride is 1024 bytes.
constexpr uint16_t src_stride = 0;
constexpr uint16_t dst_gap = 1;
// src indicates the input address of the source operand.
__cbuf__ half dst[256];
asc_copy_gm2l1_sync(dst, src, base_idx, repeat, src_stride, dst_gap);
```
