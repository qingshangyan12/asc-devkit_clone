# asc_copy_l12l0b

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

用于搬运存放在L1 Buffer里的512B大小的矩阵到L0B Buffer里，包含2D格式搬运、2D格式转置搬运、3D格式搬运。

## 函数原型

- 常规搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 同步常规搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 转置搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 同步转置搬运2D格式

     ```cpp

    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
    __aicore__ inline void asc_copy_l12l0b_transpose_sync(__cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)

    ```

- 高维切分搬运3D格式

    ```cpp

    __aicore__ inline void asc_copy_l12l0b(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b(__cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)

    ```

- 同步高维切分搬运3D格式

    ```cpp

    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)
    __aicore__ inline void asc_copy_l12l0b_sync(__cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose, bool f_matrix_ctrl, uint16_t channel_size)

    ```

## 参数说明

**表1** 2D格式参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0B Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。  |
| m_start_position | 输入 | 以M*K矩阵为例，源矩阵M轴方向的起始位置，单位为16个元素。 |
| k_start_position | 输入 | 以M*K矩阵为例，源矩阵K轴方向的起始位置，单位为32B。 |
| m_step | 输入 | 以M*K矩阵为例，源矩阵M轴方向搬运长度，单位为16个元素。取值范围：[0, 255]。进行转置搬运时，还需满足以下额外约束：<br>数据位宽为4时，m_step必须为4的倍数；<br>数据位宽为8时，m_step必须为2的倍数；<br>数据位宽为16时，m_step必须为1的倍数；<br>数据位宽为32时，m_step无额外约束。 |
| k_step | 输入 | 以M*K矩阵为例，源矩阵M轴方向搬运长度，单位为32B。取值范围：[0. 255]。进行转置搬运时，还需满足以下额外约束：<br>数据位宽为4、8或16时，k_step没有额外约束；<br>数据位宽为32时，k_step必须是2的倍数。 |
| src_stride | 输入 | 以M*K矩阵为例，源矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512B。 |
| dst_stride | 输入 | 以M*K矩阵为例，目标矩阵K方向前一个分形起始地址与后一个分形起始地址的间隔，单位为512B。 |

**表2** 3D格式参数说明
| 参数名 | 输入/输出 | 描述 |
| :--- | :--- | :--- |
| dst | 输出 | 目的L0B Buffer地址。 |
| src | 输入 | 源L1 Buffer地址。 |
| k_extension | 输入 | 该指令在目的操作数width维度的传输长度，取值范围：[1, 65535]。<br>如果不覆盖最右侧的分形，对于half类型，应为16的倍数，对于int8_t/uint8_t类型，应为32的倍数；<br>如果覆盖最右侧的分形，则无倍数要求。 |
| m_extension | 输入 | 该指令在目的操作数height维度的传输长度，取值范围：[1, 65535]。<br>如果不覆盖最下侧的分形，对于half/int8_t/uint8_t类型，应为16的倍数；<br>如果覆盖最下侧的分形，则无倍数要求。 |
| k_start_pt | 输入 | 该指令在目的操作数width维度的起点，取值范围：[0, 65535]。<br>对于half类型，应为16的倍数；<br>对于int8_t/uint8_t类型，应为32的倍数。 |
| m_start_pt | 输入 | 该指令在目的操作数height维度的起点，取值范围：[0, 65535]。<br>如果不覆盖最下侧的分形，对于half/int8_t/uint8_t，应为16的倍数；<br>如果覆盖最下侧的分形，则无倍数要求。|
| stride_w | 输入 | 卷积核在源操作数width维度滑动的步长，取值范围：[1, 63]。 |
| stride_h | 输入 | 卷积核在源操作数height维度滑动的步长，取值范围：[1, 63]。 |
| filter_w | 输入 | 卷积核width，取值范围：[1, 255]。 |
| filter_h | 输入 | 卷积核height，取值范围：[1, 255]。 |
| dilation_filter_w | 输入 | 卷积核width膨胀系数，取值范围：[1, 255]。 |
| dilation_filter_h | 输入 | 卷积核height膨胀系数，取值范围：[1, 255]。 |
| filter_size_w | 输入 | 是否在filter_w的基础上将卷积核width增加256个元素。true表示增加；false表示不增加。 |
| filter_size_h | 输入 | 是否在filter_h的基础上将卷积核height增加256个元素。true表示增加；false表示不增加。 |
| transpose | 输入 | 是否启用转置功能，对整个目标矩阵进行转置，仅在源操作数为half类型时有效。true表示启用；false表示不启用。 |
| f_matrix_ctrl | 输入 | 表示asc_copy_l12l0b指令从左矩阵还是右矩阵获取FeatureMap的属性描述，当前只支持设置为false。 |
| channel_size | 输入 | 源操作数的通道数，取值范围：[1, 63]。<br>N为正整数。 <br>对于uint32_t/int32_t/float，channelSize可取值为4，N * 8，N * 8 + 4；<br>对于half/bfloat16，channelSize可取值为4，8，N * 16，N * 16 + 4，N * 16 + 8；<br>对于int8_t/uint8_t，channelSize可取值为4，8，16， 32 * N，N * 32 + 4，N * 32 + 8，N * 32 + 16；<br>对于int4b_t，ChannelSize可取值为8，16，32，N * 64，N * 64 + 8，N * 64 + 16，N * 64 + 32。|

## 返回值说明

无

## 流水类型

PIPE_MTE1

## 约束说明

- 各存储单元的空间大小和对齐要求请参考[存储单元说明](../../general_description_and_constraints.md#存储单元说明)。
- 操作数地址重叠约束请参考[通用地址重叠约束](../../general_description_and_constraints.md#通用地址重叠约束)。

### 通用约束

- **本接口仅在AIC上生效，非AIC调用直接返回。**
- dst起始地址需512字节对齐（L0B Buffer对齐要求），否则会导致搬运异常。
- src起始地址需32字节对齐（L1 Buffer对齐要求），2D矩阵搬运模式下需32字节对齐；3D格式img2col搬运模式下按channel_size×sizeof(dtype) 对齐（channel_size≥dtype阈值时32字节对齐，channel_size较小时按对应字节数对齐）。不满足触发异常。
- 如果需要执行多条本接口指令，且目的地址存在重叠，需要插入同步指令（`asc_sync_notify`和`asc_sync_wait`），保证多个指令串行化，防止出现异常数据。
- L0B Buffer容量上限：L0B Buffer总容量64KB，dst偏移+搬运大小不可越界，否则触发写溢出异常。
- L1 Buffer容量上限：L1 Buffer总容量512KB，src偏移+源矩阵占用大小不可越界，否则触发异常。

### 2D矩阵搬运模式约束

- m_step或k_step为0时不执行搬运，本接口被视为NOP（空操作）。
- 2D矩阵搬运模式下不执行转置。如需转置，请使用对应的转置搬运接口。
- src_stride为有符号数，可为负值；负值表示源端沿K维反向取数。dst_stride不可为负值。
- 源矩阵在L1 Buffer中的占用不可超过L1 Buffer容量（含地址下溢），目的矩阵在L0B Buffer中的占用不可超过L0B Buffer容量，否则触发地址溢出异常。

### 3D格式img2col搬运模式约束

- 调用本模式前须先通过配套setter接口预先配置Feature Map尺寸、padding填充值、repeat控制与L1 Buffer容量上限等参数，再调用本接口，否则搬运行为未定义。
- 3D格式搬运到L0B Buffer时会自动进行转置，transpose参数无效，不支持非转置场景。
- k_extension、m_extension或channel_size为0时不执行搬运，本接口被视为NOP（空操作）。
- filter_w为0且filter_size_w为false，或filter_h为0且filter_size_h为false时不执行搬运，本接口被视为NOP（空操作）。
- 调用本模式前，必须通过[asc_set_l3d_rpt_b](../asc_set_l3d_rpt_b.md)设置目的端步长。
- stride_w不可为0，stride_h不可为0，否则触发异常。
- channel_size需按dtype对齐基准对齐：b32须为8的倍数或8的倍数加4、b16须为16的倍数或等于4、8、b8须为32的倍数或等于4、8、16，否则触发对齐异常。
- k_start_pt需按dtype对应分形列数对齐：b32为8的倍数、b16为16的倍数、b8为32的倍数，否则触发对齐异常。
- m_start_pt取值范围 [0, 32767]，传入>32767的值触发异常。
- b4 dtype在3D格式img2col搬运模式下不被支持。
- Feature Map尺寸（含padding）须≥卷积核尺寸（含dilation），即fm_w+pad_l+pad_r≥ (filter_w − 1) ×dilation_filter_w+1，fm_h+pad_t+pad_b≥ (filter_h − 1) ×dilation_filter_h+1，否则触发异常。
- fm_w×fm_h×channel_size×sizeof(dtype) 不可超过L1 Buffer容量，否则触发L1 Buffer容量溢出异常。
- 第一个卷积核窗口位置须≤Feature Map尺寸（含padding）− 卷积核尺寸（含dilation），否则触发异常。
- k_start_pt+k_extension不可超过img2col展开后矩阵的K维总尺寸，m_start_pt+m_extension不可超过M维总尺寸，否则触发异常。
- stride_w传入>63的值会截断处理（如64→0、65→1）。截断后实际生效值偏小，导致W方向卷积滑动步长小于预期，img2col展开后的矩阵行数增多，与预期Feature Map尺寸不匹配。截断后若值为0会同时触发stride_w不可为0的异常。
- stride_h传入>63的值会截断处理（如64→0、65→1）。截断后实际生效值偏小，导致H方向卷积滑动步长小于预期，img2col展开后的矩阵行数增多，与预期Feature Map尺寸不匹配。截断后若值为0会同时触发stride_h不可为0的异常。

## 3D数据格式说明

要求输入的feature map和filter的格式是NC1HWC0，其中C0是最低维度而且C0是固定值为16（对于u8/s8类型为32），C1=C/C0。

为了简化场景，以下场景假设输入的feature map的channel为4，即Ci=4。输入feature maps在L1 Buffer中的形状为(Hi,Wi,Ci)，经过load3dv1处理后在L0B Buffer的数据形状为(Wo\*Ho, Hk\*Wk\*Ci)。其中Wo和Ho是卷积后输出的shape，Hk和Wk是filter的shape。

直观的来看，img2col的过程就是filter在feature map上扫过，将对应feature map的数据展开成输出数据的每一行的过程。filter首先在W方向上滑动Wo步，然后在H方向上走一步然后重复以上过程，最终输出Wo * Ho行数据。下图中红色和黄色的数据分别代表第一行和第二行。数字表示原始输入数据，filter和输出数据三者之间的关联关系。可以看到，load3dv1首先在输入数据的Ci维度搬运对应于00的4个数，然后搬运对应于01的四个数，最终这一行的大小为Hk\*Wk\*Ci即3\*3\*4=36个数。

- 对应的feature map格式如下图：

![ ](../../figures/load3d_01.png)

- 对应的filter的格式如下图：

![ ](../../figures/load3d_02.png)

其中n为filter的个数，可以看出维度排布为(Hk,Wk,Ci,n)，但是需要注意的是下图的格式还需要根据Mmad中B矩阵的格式转换。

实际操作中，由于存储空间或者计算能力限制，我们通常会将整个卷积计算分块，一次只搬运并计算一小块数据。

![ ](../../figures/load3d_03.png)

对于L0B Buffer中的feature map来说有两种方案，水平分块和垂直分块。分别对应参数中repeatMode的0和1。

注：下图中的分形矩阵大小为4x4，实际应该为16x16 (对于u8/s8类型为16x32)

repeatMode =0时，每次repeat会改变在filter窗口中读取数据点的位置，然后跳到下一个C0的位置。

![ ](../../figures/load3d_04.png)

repeatMode =1的时候filter窗口中读取数据的位置保持不变，每个repeat在feature map中前进C0个元素。

![ ](../../figures/load3d_05.png)

## 调用示例

```cpp
__cb__ bfloat16_t dst[256];
__cbuf__ bfloat16_t src[256];
uint16_t m_start_position = 8;
uint16_t k_start_position = 2;
uint8_t m_step = 4;
uint8_t k_step = 4;
int16_t src_stride = 1;
uint16_t dst_stride = 1;
asc_copy_l12l0b(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
```
