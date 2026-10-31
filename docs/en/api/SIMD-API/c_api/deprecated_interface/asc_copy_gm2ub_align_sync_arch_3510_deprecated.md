# asc_copy_gm2ub_align_sync_arch_3510 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:42:21.563Z pushedAt=2026-10-28T12:55:18.812Z nl=2 -->

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

The header file path is `"c_api/composite/vector_compute_composite.h"`.

**The `asc_copy_gm2ub_align_sync` API is deprecated. Use [asc_copy_gm2ub_align contiguous data movement](../vector_datamove/asc_copy_gm2ub_align/asc_copy_gm2ub_align_arch_3510.md#contiguous-data-movement) and the synchronization API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

Provides unaligned data transfer, moving data from Global Memory (GM) to Unified Buffer (UB), and supports 8-bit/16-bit/32-bit data type transfer.

This API is a synchronous transfer API. If the transferred data length is not 32-byte aligned, the transferred data is padded to 32-byte alignment. The following two padding methods are supported:

- Manual padding: Call [asc_set_copy_pad_val](../vector_datamove/asc_set_copy_pad_val.md) to configure the padding value before the transfer.
- Automatic padding: The hardware automatically pads dummy data, whose value is the value of the first element of the data block.

## Prototype

```cpp
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
| :--- | :--- | :--- |
| dst | Output | Start address of the destination UB. Must be 32-byte aligned. |
| src | Input | Start address of the source GM. Must be 1-byte aligned. |
| size | Input | Size of the transferred data, in bytes. Value range: [0, 2097151]. |

## Return Value

None

## Pipeline Type

PIPE_MTE2

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- When `size` is 0, this API is treated as a NOP (no-op).
- If the UB address of this instruction overlaps with that of other instructions, insert synchronization instructions [asc_sync_notify](../sync/intra_core_sync/asc_sync_notify.md) and [asc_sync_wait](../sync/intra_core_sync/asc_sync_wait.md) to serialize the instructions and prevent abnormal data.
- Synchronous computation includes synchronous wait.
- If `size` is not 32-byte aligned, the data transfer is padded to 32-byte alignment, and the destination UB must reserve space for the padded size. For manual padding, call `asc_set_copy_pad_value` to configure the padding value. For automatic padding, the hardware fills dummy data, whose value is the first element of the data block.
