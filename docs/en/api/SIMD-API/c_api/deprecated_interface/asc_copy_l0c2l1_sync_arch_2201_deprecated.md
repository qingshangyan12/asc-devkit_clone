# asc_copy_l0c2l1_sync_arch_2201 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:47:31.276Z pushedAt=2026-10-28T12:55:18.818Z nl=2 -->

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

**The `asc_copy_l0c2l1_sync` API is deprecated. Use the [corresponding non-synchronous API](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_2201.md) and the synchronous API [asc_sync](../sync/intra_core_sync/asc_sync.md) instead.**

After matrix computation is complete, the result is quantized and then moved to the L1 Buffer. There are nine quantization modes:
- NoQuant: Quantization is disabled.
- DEQF16: Quantize int32_t to half. The quantization result does not support the INF_NAN mode.
- F322BF16: Quantize float to bfloat16_t. The quantization result does not support the INF_NAN mode.
- F322F16: Quantize float to half. The quantization result does not support the INF_NAN mode.
- QF322B8_PRE: Quantize float to uint8_t/int8_t. Scalar quantization.
- REQ8: Quantize int32_t to uint8_t/int8_t. Scalar quantization.
- VDEQF16: Quantize int32_t to half. The Quantization Result does not support the INF_NAN mode.
- VQF322B8_PRE: Quantize float to uint8_t/int8_t. Vector quantization.
- VREQ8: Quantize int32_t to uint8_t/int8_t. Vector quantization.

In addition, this data movement instruction also supports quantization of the int4b_t type. The related quantization types are divided into two categories: quantizing int32_t to int4b_t, and quantizing float to int4b_t.

## Prototype

- Synchronous data movement

    ```cpp
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ half* dst, __cc__ float* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ int16_t* dst, __cc__ int32_t* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void* dst, __cc__ float* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    __aicore__ inline void asc_copy_l0c2l1_sync(__cbuf__ void* dst, __cc__ int32_t* src, uint16_t n_size,uint16_t m_size, uint32_t dst_stride, uint16_t src_stride, uint8_t unit_flag_mode, uint64_t quant_pre, uint8_t relu_pre, bool enable_channel_split, bool enable_nz2nd)
    ```

## Parameters


**Table 1** Parameter description

| Parameter | Input/Output | Description |
|:---------------------|:------|:---------------------------------------------------------------------------------------------------------------------------------|
| dst | Output | Start address of the destination operand (vector). |
| src | Input | Start address of the source operand. |
| n_size | Input | Size of the source NZ matrix in the N direction.<br/>&bull; When NZ2ND is disabled, value range: [1, 4095];<br/>&bull; When NZ2ND is enabled, value range: [1, 4095]. |
| m_size | Input | Size of the source NZ matrix in the M direction.<br/>&bull; When NZ2ND is disabled, value range: [1, 65535];<br/>&bull; When NZ2ND is enabled, value range: [1, 8192]. |
| dst_stride | Input | &bull; When NZ2ND is disabled, the start address offset between adjacent Z layouts in the destination NZ matrix. The value cannot be 0. Unit: element.<br/>&bull; When NZ2ND/NZ2DN is enabled, the number of elements in each row of the destination ND matrix. The value cannot be 0. Unit: element. |
| src_stride | Input | Start address offset between adjacent Z layouts in the source NZ matrix. Value range: [0, 65535]. Unit: C0_Size (16*sizeof(T), where T is the data type of src). |
| unit_flag_mode | Input | Related to the unit_flag parameter. Values are as follows:<br/>&bull; 0: reserved value;<br/>&bull; 2: enable unit_flag. After the hardware finishes executing the instruction, the register is not set;<br/>&bull; 3: enable unit_flag. After the hardware finishes executing the instruction, unit_flag is disabled. |
| quant_pre | Input | Quantization parameter. For values, see [Description](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_2201.md#description). |
| relu_pre | Input | Enable relu. |
| enable_channel_split | Input | Whether to enable channel splitting. The default value is false, which means the function is disabled. Channel splitting can be enabled only when both src and dst are float, and channel_split and NZ2ND cannot be enabled at the same time. |
| enable_nz2nd | Input | NZ2ND switch.<br/>&bull; false: disabled;<br/>&bull; true: enabled. |

## Return Value

None

## Pipeline Type

PIPE_FIX

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).
- If multiple asc_copy_l0c2l1 instructions need to be executed and their destination addresses overlap, insert synchronization instructions to serialize the multiple asc_copy_l0c2l1 instructions and prevent abnormal data.

## Examples

```cpp
// dst and src correspond to the output address of the destination operand and the input address of the source operand, respectively.
constexpr uint64_t total_length = 256;
__cbuf__ int32_t dst[total_length];
__cc__ int32_t src[total_length];
// The remaining input parameters are passed with default values.
uint16_t n_size = 16;
uint16_t m_size = 16;
uint32_t dst_stride = 8;
uint16_t src_stride = 8;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre = QuantMode_t::NoQuant;
uint8_t relu_pre = 0;
bool channel_split = false;
bool enable_nz2nd = false;
asc_copy_l0c2l1_sync(dst, src, n_size, m_size, dst_stride, src_stride, unit_flag_mode, quant_pre, relu_pre, enable_channel_split, enable_nz2nd);
```
