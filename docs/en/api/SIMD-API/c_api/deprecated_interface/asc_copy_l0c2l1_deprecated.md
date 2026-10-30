# asc_copy_l0c2l1 (Deprecated)

<!-- md-trans-meta sourceCommit=754fb95859dead826727a591e674261c6a829a08 translatedAt=2026-10-23T00:48:18.282Z pushedAt=2026-10-28T12:55:18.821Z nl=2 -->

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

**The legacy parameter form of `asc_copy_l0c2l1` and the `asc_copy_l0c2l1_sync` API are deprecated. Use the [asc_copy_l0c2l1](../cube_datamove/asc_copy_l0c2l1/asc_copy_l0c2l1_arch_3510.md) API and the [asc_sync](../sync/intra_core_sync/asc_sync.md) API instead.**

The header file path is `"c_api/cube_datamove/cube_datamove.h"`.

This API copies the matrix computation result from the L0C Buffer to the L1 Buffer, and during the copy it can be combined with other APIs to configure various inline capabilities. (Quantization mode support is not yet complete. For the specific supported modes, see the APIs included in [Prototype](#prototype).)

The following figure shows the valid combinations, intermediate data types, and data paths of inline quantization, inline ReLU, inline format conversion, inline channel split, and inline channel merge. In the figure, F32-\>F16 and F32-\>BF16 are Cast, and the rest are inline scalar/tensor quantization modes.

**Figure 1** Inline function combinations of asc_copy_l0c2l1

![](../../../figures/C_API_L0C2L1_Function_Combination_950.png)

The supported enumeration values of the quant_pre_mode quantization mode parameter are as follows:

- NoQuant: Quantization is not enabled.
- DEQF16: Quantizes int32_t to half using scalar quantization.
- VDEQF16: Quantizes int32_t to half using tensor quantization.
- REQ4: int32_t quantized to int4b_t, scalar quantization
- VREQ4: int32_t quantized to int4b_t, tensor quantization
- REQ8: int32_t quantized to int8_t/uint8_t, scalar quantization
- VREQ8: int32_t quantized to int8_t/uint8_t, tensor quantization
- QS322BF16_PRE: int32_t quantized to bfloat16_t, scalar quantization
- VQS322BF16_PRE: int32_t quantized to bfloat16_t, tensor quantization
- QF322F16_PRE: float quantized to half, scalar quantization
- VQF322F16_PRE: float quantized to half, tensor quantization
- QF322BF16_PRE: float quantized to bfloat16_t, scalar quantization
- VQF322BF16_PRE: float quantized to bfloat16_t, tensor quantization
- F322F16: float cast to half, with the cast mode set to CAST_RINT
- F322BF16: float cast to bfloat16_t, with the cast mode set to CAST_RINT
- QF322S4_PRE: float quantized to int4b_t, scalar quantization
- VQF322S4_PRE: float quantized to int4b_t, tensor quantization
- QF322B8_PRE: float quantized to int8_t/uint8_t, scalar quantization
- VQF322B8_PRE: float quantized to int8_t/uint8_t, tensor quantization
- QF322FP8_PRE: float quantized to fp8_e4m3fn_t, scalar quantization
- VQF322FP8_PRE: float quantized to fp8_e4m3fn_t, tensor quantization
- QF322HIF8_PRE: float quantized to hifloat8_t (Half to Away Round), scalar quantization
- VQF322HIF8_PRE: float quantized to hifloat8_t (Half to Away Round), tensor quantization
- QF322HIF8_PRE_HYBRID: float quantized to hifloat8_t (Hybrid Round), scalar quantization
- VQF322HIF8_PRE_HYBRID: float quantized to hifloat8_t (Hybrid Round), tensor quantization
- QF322F32_PRE: float quantized to float, scalar quantization. The precision can reach two thousandths but cannot reach two ten-thousandths. If two ten-thousandths precision is required, use the [AscendDeQuant](../../adv_api/quantization/AscendDequant.md) high-level API.
- VQF322F32_PRE: float quantized to float, tensor quantization. The precision can reach two thousandths but cannot reach two ten-thousandths. If two ten-thousandths precision is required, use the [AscendDeQuant](../../adv_api/quantization/AscendDequant.md) high-level API.

## Prototype

```cpp
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    uint64_t quant_post, uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op,
    bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
```

## Parameters

**Table 1** Parameter description

| Parameter | Input/Output | Description |
|:-----------------|:------|:-----------------------------------|
| dst | Output | Start address of the destination operand. The data format is NZ, ND, or DN, and the address must be 32-byte aligned. |
| src | Input | Start address of the source operand. The data format is NZ, and the address must be 64-byte aligned. |
| n_size | Input | Size of the source NZ matrix in the N direction. Value range: [1, 4095]. |
| m_size | Input | Size of the source NZ matrix in the M direction.<br>&nbsp;&nbsp;&bull; If NZ2ND is not enabled, value range: [1, 65535].<br>&nbsp;&nbsp;&bull; If NZ2ND is enabled, value range: [1, 8192]. |
| dst_stride | Input | &nbsp;&nbsp;&bull; If NZ2ND is not enabled, dst_stride indicates the start address offset between adjacent Z layouts in the destination NZ matrix. The value must not be 0. Unit: element.<br>&nbsp;&nbsp;&bull; If NZ2ND/NZ2DN is enabled, dst_stride indicates the number of elements in each row of the destination ND matrix. The value must not be 0. Unit: element. |
| src_stride | Input | Start address offset between adjacent Z layouts in the source NZ matrix. Value range: [0, 65535]. Unit: C0_Size (16*sizeof(T), where T is the data type of src). |
| l2_cache_mode | Input | Invalid parameter. You do not need to pay attention to it. Pass 0. |
| enable_clip_relu_pre | Input | Whether to enable Clip ReLU. It must be used together with Normal ReLU, and quantization must be enabled. Values:<br>&nbsp;&nbsp;&bull; 0: Clip ReLU is not enabled.<br>&nbsp;&nbsp;&bull; 1: Clip ReLU is enabled (scalar mode). |
| unit_flag_mode | Input | unit_flag is a fine-grained parallel feature for mmad instructions and matrix move-out instructions. When enabled, the hardware moves out the computation result after computing each fractal. Values:<br>&nbsp;&nbsp;&bull; 0: unit_flag is not enabled.<br>&nbsp;&nbsp;&bull; 2: unit_flag is enabled. After the hardware executes the instruction, the unit flag bit is not reset.<br>&nbsp;&nbsp;&bull; 3: unitFlag is enabled. After the hardware executes the instruction, the unit flag bit is reset.<br>When this feature is enabled, set the unit_flag value of both mmad instructions and matrix move-out instructions to 2 or 3. |
| quant_pre_mode | Input | Quantization parameter in the preprocessing stage. For values, see [Description](#description). |
| relu_pre_mode | Input | ReLU mode control in the preprocessing stage. Values:<br>&nbsp;&nbsp;&bull; 0: ReLU is not enabled.<br>&nbsp;&nbsp;&bull; 1: Normal ReLU is enabled.<br>&nbsp;&nbsp;&bull; 2: Scalar ReLU is enabled.<br>&nbsp;&nbsp;&bull; 3: Vector ReLU is enabled. |
| enable_channel_split | Input | Whether to enable channel split. Channel Split can be enabled only when both src and dst are float, and Channel Split and NZ2ND cannot be enabled at the same time.<br>&nbsp;&nbsp;&bull; false: not enabled.<br>&nbsp;&nbsp;&bull; true: enabled. |
| enable_nz2nd | Input | Whether to enable NZ2ND.<br>&nbsp;&nbsp;&bull; false: not enabled.<br>&nbsp;&nbsp;&bull; true: enabled. |
| quant_post | Input | Invalid parameter. You do not need to pay attention to it. Pass 0. |
| relu_post | Input | Invalid parameter. You do not need to pay attention to it. Pass 0. |
| clip_relu_post | Input | Invalid parameter. You do not need to pay attention to it. Pass false. |
| eltwise_op | Input | Invalid parameter. You do not need to pay attention to it. Pass 0. |
| eltwise_antq_en | Input | Invalid parameter. You do not need to pay attention to it. Pass false or 0. |
| c0_pad_en | Input | Invalid parameter. You do not need to pay attention to it. Pass false. |
| broadcast_en | Input | Invalid parameter. You do not need to pay attention to it. Pass false. |
| enable_nz2dn | Input | Whether to enable NZ2DN.<br>&nbsp;&nbsp;&bull; false: not enabled.<br>&nbsp;&nbsp;&bull; true: enabled. |

## Return Value

None

## Pipeline Type

PIPE_FIX

## Constraints

- For the space size and alignment requirements of each storage unit, see [Storage Unit Description](../general_description_and_constraints.md#storage-unit-description).

## Examples

```cpp
// dst and src correspond to the output address of the destination operand and the input address of the source operand, respectively.
constexpr uint64_t total_length = 128 * 64;
__cbuf__ int32_t dst[total_length];
__cc__ int32_t src[total_length];
// The remaining input parameters are passed with default values.
uint16_t n_size = 64;
uint16_t m_size = 128;
uint32_t dst_stride = 2048;
uint16_t src_stride = 128;
uint8_t enable_clip_relu_pre = 0;
uint8_t unit_flag_mode = 0;
uint64_t quant_pre_mode = QuantMode_t::NoQuant;
uint8_t relu_pre_mode = 0;
bool enable_channel_split = false;
bool enable_nz2nd = false;
uint64_t quant_post = 0;  // Invalid parameter.
uint8_t relu_post = 0;  // Invalid Parameter.
bool clip_relu_post = false;  // Invalid Parameter.
uint8_t eltwise_op = 0;  // Invalid Parameter.
uint8_t eltwise_antq_en = 0;  // Invalid Parameter.
bool c0_pad_en = false;  // Invalid Parameter.
asc_copy_l0c2l1(
    dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode,
    quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
    clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en);
```
