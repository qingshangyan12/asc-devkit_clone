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

#endif