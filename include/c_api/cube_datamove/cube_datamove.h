/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef INCLUDE_C_API_CUBE_DATAMOVE_H
#define INCLUDE_C_API_CUBE_DATAMOVE_H

#include "c_api_instr_impl/cube_datamove_c_api_impl/cube_datamove_c_api_impl.h"

// ==========asc_copy_l12l0b, 2D, uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float/int4x2_t==========
__aicore__ inline void asc_copy_l12l0b(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint16_t * dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint16_t * dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ int16_t * dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int16_t * dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ half * dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half * dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ float * dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float * dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b(__cb__ void * dst, __cbuf__ void* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ void * dst, __cbuf__ void* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

// ==========asc_copy_l12l0b, 3D, half/bfloat16_t/uint32_t/int32_t/float==========
__aicore__ inline void asc_copy_l12l0b(__cb__ half * dst, __cbuf__ half* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ half * dst, __cbuf__ half* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ half * dst, __cbuf__ half* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b(__cb__ float * dst, __cbuf__ float* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sync(__cb__ float * dst, __cbuf__ float* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

// ==========asc_copy_l12l0a, 2D, uint8_t/int8_t/uint16_t/int16_t/half/bfloat16_t/uint32_t/int32_t/float/int4x2_t==========
__aicore__ inline void asc_copy_l12l0a(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ uint16_t * dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ uint16_t * dst, __cbuf__ uint16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ int16_t * dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ int16_t * dst, __cbuf__ int16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ half * dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ half * dst, __cbuf__ half* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ float * dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ float * dst, __cbuf__ float* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a(__cb__ void * dst, __cbuf__ void* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ void * dst, __cbuf__ void* src,
    uint16_t start_index, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap, uint8_t sid, bool transpose, uint8_t addr_mode);

// ==========asc_copy_l12l0a, 3D, uint8_t/int8_t/half/bfloat16_t/uint32_t/int32_t/float/int4x2_t==========
__aicore__ inline void asc_copy_l12l0a(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ uint8_t * dst, __cbuf__ uint8_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ int8_t * dst, __cbuf__ int8_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ half * dst, __cbuf__ half* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ half * dst, __cbuf__ half* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ bfloat16_t * dst, __cbuf__ bfloat16_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ uint32_t * dst, __cbuf__ uint32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ int32_t * dst, __cbuf__ int32_t* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ float * dst, __cbuf__ float* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ float * dst, __cbuf__ float* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a(__cb__ void * dst, __cbuf__ void* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0a_sync(__cb__ void * dst, __cbuf__ void* src,
    uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h,
    uint8_t filter_w, uint8_t filter_h, uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h,
    bool transpose, bool f_matrix_ctrl, uint16_t channel_size);

__aicore__ inline void asc_copy_l12l0b_sparse(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

__aicore__ inline void asc_copy_l12l0b_sparse_sync(__cb__ int8_t* dst, __cbuf__ int8_t* src, __cbuf__ int8_t* index, uint16_t start_index, uint8_t repeat);

__aicore__ inline void asc_set_l1_3d_size(uint64_t value);

// ==========asc_load_image_to_cbuf(half/int8_t)==========
__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid);

__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ half* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid);

__aicore__ inline void asc_load_image_to_cbuf(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid);

__aicore__ inline void asc_load_image_to_cbuf_sync(__cbuf__ int8_t* dst, uint16_t hor_size, uint16_t ver_size,
    uint16_t hor_start_pos, uint16_t ver_start_pos, uint16_t src_hor_res, uint8_t top_pad_size, uint8_t bot_pad_size,
    uint16_t left_pad_size, uint16_t right_pad_size, uint8_t sid);

// ==========asc_fill_l0a_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0a_value(__ca__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0a_value_sync(__ca__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

// ==========asc_fill_l0b_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l0b_value(__cb__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l0b_value_sync(__cb__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

// ==========asc_fill_l1_value(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_fill_l1_value_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst,
                                      uint16_t len_burst, uint16_t source_gap, uint16_t dst_gap);

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size);

__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size);

// ==========copy_l12l0b_trans_impl==========
__aicore__ inline void copy_l12l0b_trans(__cb__ half* dst, __cbuf__ half* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint16_t frac_stride);

__aicore__ inline void copy_l12l0b_trans(__cb__ float* dst, __cbuf__ float* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride);

__aicore__ inline void copy_l12l0b_trans(__cb__ int32_t* dst, __cbuf__ int32_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride);

__aicore__ inline void copy_l12l0b_trans(__cb__ int8_t* dst, __cbuf__ int8_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride);

__aicore__ inline void copy_l12l0b_trans(__cb__ uint32_t* dst, __cbuf__ uint32_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride);

__aicore__ inline void copy_l12l0b_trans(__cb__ uint8_t* dst, __cbuf__ uint8_t* src,  uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_stride, bool addrmode, uint64_t frac_stride);

__aicore__ inline void set_l0c_copy_params(uint16_t nd_num, uint16_t src_nd_stride, uint16_t dst_nd_stride);

#endif