# asc_copy_l12l0a

## 产品支持情况

<!-- npu="950" id1 -->
- Ascend 950PR/Ascend 950DT：支持
<!-- end id1 -->
<!-- npu="A3" id2 -->
- Atlas A3 训练系列产品/Atlas A3 推理系列产品：不支持
<!-- end id2 -->
<!-- npu="910b" id3 -->
- Atlas A2 训练系列产品/Atlas A2 推理系列产品：不支持
<!-- end id3 -->
<!-- npu="310b" id4 -->
- Atlas 200I/500 A2 推理产品：不支持
<!-- end id4 -->
<!-- npu="310p" id5 -->
- Atlas 推理系列产品AI Core：不支持
<!-- end id5 -->
<!-- npu="310p" id6 -->
- Atlas 推理系列产品Vector Core：不支持
<!-- end id6 -->
<!-- npu="910" id7 -->
- Atlas 训练系列产品：不支持
<!-- end id7 -->

## 功能说明

用于搬运存放在L1 Buffer里的512B大小的矩阵到L0A Buffer里，包含2D格式搬运、2D格式转置搬运、3D格式搬运。

## 函数原型

- 常规搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 同步常规搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 转置搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 同步转置搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0a_transpose_sync(__ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    
    ```

- 高维切分搬运3D格式

    ```cpp

    __aicore__ inline void asc_copy_l12l0a(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a(__ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)

    ```

- 同步高维切分搬运3D格式

```cpp

    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0a_sync(__ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)

```

## 参数说明

**表1** 2D格式参数说明
| 参数名              | 输入/输出 | 描述 |
|:-----------------| :--- |:-------------------------------------------------------------------------------------------------------------------------------------------------------------|
| dst              | 输出 | 目的L0A Buffer地址。 |
| src              | 输入 | 源L1 Buffer地址。 |
| m_start_position | 输入 | 以M*K矩阵为例，源矩阵M轴方向的起始位置，单位为16个元素。 |
| k_start_position | 输入 | 以M*K矩阵为例，源矩阵K轴方向的起始位置，单位为32B。 |
| m_step           | 输入 | 以M*K矩阵为例，源矩阵M轴方向搬运长度，单位为16个元素。取值范围：[0, 255]。当进行转置搬运时，还需满足以下额外约束：<br>数据位宽为4时，m_step必须为4的倍数；<br>数据位宽为8时，m_step必须为2的倍数；<br>数据位宽为16时，m_step必须为1的倍数；<br>数据位宽为32时，m_step无额外约束。 |
| k_step           | 输入 | 以M*K矩阵为例，源矩阵M轴方向搬运长度，单位为32B。取值范围：[0. 255]。当进行转置搬运时，还需满足以下额外约束：<br>数据位宽为4、8或16时，k_step没有额外约束；<br>数据位宽为32时，k_step必须是2的倍数。 |
| src_stride       | 输入 | 以M*K矩阵为例，源矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512B。 |
| dst_stride       | 输入 | 以M*K矩阵为例，目标矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512B。 |

**表2** 3D格式参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0A Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| k_extension | 输入 | 该指令在目的操作数width维度的传输长度。<br>如果不覆盖最右侧的分形，对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数；<br>如果覆盖最右侧的分形，则无倍数要求。取值范围：[1, 65535]。 |
| m_extension | 输入 | 该指令在目的操作数height维度的传输长度。<br>如果不覆盖最下侧的分形，对于half/int8_t/uint8_t类型，应为16的倍数；<br>如果覆盖最下侧的分形，则无倍数要求。取值范围：[1, 65535]。 |
| k_start_pt | 输入 | 该指令在目的操作数width维度的起点。对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数。取值范围：[0, 65535] |
| m_start_pt | 输入 | 该指令在目的操作数height维度的起点，如果不覆盖最下侧的分形，对于half/int8_t/uint8_t，应为16的倍数；如果覆盖最下侧的分形，则无倍数要求。取值范围：[0, 65535]。|
| stride_w | 输入 | 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。 |
| stride_h | 输入 | 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。 |
| filter_w | 输入 | 卷积核width，取值范围：[1, 255]。 |
| filter_h | 输入 | 卷积核height，取值范围：[1, 255]。 |
| dilation_filter_w | 输入 | 卷积核width膨胀系数，取值范围：[1, 255]。 |
| dilation_filter_h | 输入 | 卷积核height膨胀系数，取值范围：[1, 255]。 |
| filter_size_w | 输入 | 是否在filter_w的基础上将卷积核width增加256个元素。true表示增加；false表示不增加。 |
| filter_size_h | 输入 | 是否在filter_h的基础上将卷积核height增加256个元素。true表示增加；false表示不增加。 |
| transpose | 输入 | 是否启用转置功能，对整个目标矩阵进行转置，仅在源操作数为half类型时有效。true表示启用；false表示不启用。 |
| f_matrix_ctrl | 输入 | 表示asc_copy_l12l0a指令从左矩阵还是右矩阵获取FeatureMap的属性描述，当前只支持设置为false。 |
| channel_size | 输入 | 源操作数的通道数，取值范围：[1, 63]。 <br>对于uint32_t/int32_t/float，channelSize可取值为4，N * 8，N * 8 + 4；<br>对于half/bfloat16，channelSize可取值为4，8，N * 16，N * 16 + 4，N * 16 + 8；<br>对于int8_t/uint8_t，channelSize可取值为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；<br>对于int4b_t，ChannelSize可取值为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。N为正整数。|

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- 本接口仅在AIC上生效，非AIC调用直接返回。
- dst起始地址需512字节对齐（L0A Buffer端口对齐要求），否则搬运异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- L0A Buffer容量上限：L0A Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发异常。

### 2D分形搬运模式约束

- m_step不可为0，否则指令被置为NOP不执行并打印警告。
- k_step不可为0，否则指令被置为NOP不执行并打印警告。
- dst_stride即使k_step为1也不可为0，否则触发异常。
- 开启transpose时需按dtype满足分形对齐约束：`int4b_t`需m_step为4的倍数；`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`需m_step为2的倍数；`bfloat16_t`/`half`/`int16_t`/`uint16_t`/`fp4x2_e2m1_t`/`fp4x2_e1m2_t`需m_step为1的倍数；`float`/`int32_t`/`uint32_t`需k_step为2的倍数，否则触发地址对齐异常。
- 开启transpose时仅支持`int4b_t`、`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`、`bfloat16_t`/`half`/`int16_t`/`uint16_t`/`fp4x2_e2m1_t`/`fp4x2_e1m2_t`、`float`/`int32_t`/`uint32_t`四类dtype，其他dtype触发异常。

### 3D img2col搬运模式约束

- k_extension不可为0，否则指令被置为NOP不执行并打印警告。
- m_extension不可为0，否则指令被置为NOP不执行并打印警告。
- channel_size不可为0，否则指令被置为NOP不执行并打印警告。
- stride_w与stride_h不可为0，传入0硬件按1处理。
- m_start_pt不可超过32767，否则触发异常。
- 须先调用配套setter接口预先配置feature map描述（fm_w、fm_h、pad_l、pad_r、pad_t、pad_b）、填充值与填充模式、L1 Buffer容量上限，再调用本接口，否则feature map描述使用未初始化的配置值产生错误结果。如需使用repeat模式，须先调用配套setter接口配置repeat方向、次数与步长。
- feature map尺寸fm_w与fm_h须>0且≤32767，否则指令被置为NOP不执行。
- feature map（含padding）须≥卷积核（含dilation）尺寸，即fm_w+pad_l+pad_r≥ (filter_w − 1) ×dilation_filter_w+1，fm_h+pad_t+pad_b≥ (filter_h − 1) ×dilation_filter_h+1，否则触发异常。
- feature map（不含padding）与通道数须满足fm_w×fm_h×channel_size×sizeof(dtype) ≤L1 Buffer容量，否则触发异常。
- k_start_pt+k_extension不可超过img2col后K维总长（=filter_h×filter_w×channel_size），m_start_pt+m_extension不可超过img2col后M维总长（=Hstep×Wstep，其中Hstep=floor((fm_h+pad_t+pad_b − (filter_h − 1) ×dilation_filter_h − 1) /stride_h) +1，Wstep同理），否则触发异常。
- channel_size需按dtype满足对齐约束：`b32`系列（`float`/`int32_t`/`uint32_t`）需为8的倍数或8的倍数+4；`b16`系列（`half`/`bfloat16_t`/`int16_t`/`uint16_t`）需为16的倍数、或4、或8、或16的倍数+4、或16的倍数+8；`b8`系列（`int8_t`/`uint8_t`/`fp8_e4m3fn_t`/`fp8_e5m2_t`/`hifloat8_t`）需为32的倍数、或4、或8、或16、或32的倍数+4、或32的倍数+8、或32的倍数+16。k_start_pt需按同基准对齐，否则触发异常。
- channel_size不可超过dtype上限：`b32`系列≤65532、`b16`系列≤65528、`b8`系列≤65520，否则触发异常。
- img2col后矩阵尺寸受K/M维起点位宽限制：fm_w×fm_h须<2^16，Wstep×Hstep须<2^15，否则触发异常。
- transpose仅在`b8`/`b16`/`b32`系列dtype时解码使能，其他dtype时transpose字段被忽略恒为false。
- 若需同时使用本接口的3D img2col模式与历史3D模式，须在两者之间插入同步指令保证串行化，防止feature map配置串扰。
- `b4`系列dtype（`int4b_t`、`fp4x2_e2m1_t`、`fp4x2_e1m2_t`）不支持，本模式重载未提供这些dtype，传入会触发异常。
- stride_w传入>63的值会按位宽截断处理（如64→0、65→1），硬件不报错。截断后实际生效值偏小，导致W维卷积步长与预期不符，img2col后的窗口位置错位，提取的feature map数据与预期不一致。参数类型为`uint8_t`（0~255），截断后若值为0会被硬件按1处理。
- stride_h传入>63的值会按位宽截断处理（如64→0、65→1），硬件不报错。截断后实际生效值偏小，导致H维卷积步长与预期不符，img2col后的窗口位置错位，提取的feature map数据与预期不一致。参数类型为`uint8_t`（0~255），截断后若值为0会被硬件按1处理。

## 3D数据格式说明

要求输入的feature map和filter的格式是NC1HWC0，其中C0是最低维度而且C0是固定值为16（对于u8/s8类型为32），C1=C/C0。

为了简化场景，以下场景假设输入的feature map的channel为4，即Ci=4。输入feature maps在L1 Buffer中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在L0A Buffer的数据形状为(Wo\*Ho, Hk\*Wk\*Ci)。其中Wo和Ho是卷积后输出的shape，Hk和Wk是filter的shape。

直观的来看，img2col的过程就是filter在feature map上扫过，将对应feature map的数据展开成输出数据的每一行的过程。filter首先在W方向上滑动Wo步，然后在H方向上走一步然后重复以上过程，最终输出Wo * Ho行数据。下图中红色和黄色的数据分别代表第一行和第二行。数字表示原始输入数据，filter和输出数据三者之间的关联关系。可以看到，load3dv1首先在输入数据的Ci维度搬运对应于00的4个数，然后搬运对应于01的四个数，最终这一行的大小为Hk\*Wk\*Ci即3\*3\*4=36个数。

- 对应的feature map格式如下图：

![ ](../../figures/load3d_01.png)

- 对应的filter的格式如下图：

![ ](../../figures/load3d_02.png)

其中n为filter的个数，可以看出维度排布为(Hk,Wk,Ci,n)，但是需要注意的是下图的格式还需要根据Mmad中B矩阵的格式转换。

实际操作中，由于存储空间或者计算能力限制，我们通常会将整个卷积计算分块，一次只搬运并计算一小块数据。

![ ](../../figures/load3d_03.png)

对于L0A Buffer中的feature map来说有两种方案，水平分块和垂直分块。分别对应参数中repeatMode的0和1。

注：下图中的分形矩阵大小为4x4，实际应该为16x16 (对于u8/s8类型为16x32)

repeatMode =0时，每次repeat会改变在filter窗口中读取数据点的位置，然后跳到下一个C0的位置。

![ ](../../figures/load3d_04.png)

repeatMode =1的时候filter窗口中读取数据的位置保持不变，每个repeat在feature map中前进C0个元素。

![ ](../../figures/load3d_05.png)

## 调用示例

```cpp
__ca__ bfloat16_t dst[256];
__cbuf__ bfloat16_t src[256];
uint16_t m_start_position = 8;
uint16_t k_start_position = 2;
uint8_t m_step = 4;
uint8_t k_step = 4;
int16_t src_stride = 1;
uint16_t dst_stride = 1;
asc_copy_l12l0a(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```
