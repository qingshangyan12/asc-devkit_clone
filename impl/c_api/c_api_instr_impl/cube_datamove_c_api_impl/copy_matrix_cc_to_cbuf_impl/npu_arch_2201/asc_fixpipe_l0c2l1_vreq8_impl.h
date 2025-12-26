/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_C_API_INSTR_CUBE_DMAMOVE_COPY_MATRIX_CC_TO_CBUF_NPU_ARCH_2201_FIXPIPE_L0C2L1_VREQ8_IMPL_H
#define IMPL_C_API_INSTR_CUBE_DMAMOVE_COPY_MATRIX_CC_TO_CBUF_NPU_ARCH_2201_FIXPIPE_L0C2L1_VREQ8_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

//asc_fixpipe_l0c2l1_vreq8 half float
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ half *dst, __cc__ float *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode,  uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ half *dst, __cc__ float *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

//asc_fixpipe_l0c2l1_vreq8 int8_t float
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ int8_t *dst, __cc__ float *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ int8_t *dst, __cc__ float *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

//asc_fixpipe_l0c2l1_vreq8 half int32_t
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ half *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ half *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

//asc_fixpipe_l0c2l1_vreq8 int16_t int32_t
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ int16_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ int16_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

//asc_fixpipe_l0c2l1_vreq8 int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ int8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ int8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

//asc_fixpipe_l0c2l1_vreq8 uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_vreq8_impl(__cbuf__ uint8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    if ASC_IS_AIC{
        copy_matrix_cc_to_cbuf(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, QuantMode_t::VREQ8,
            relu_pre, channel_split, nd2nz_en);
    }
}

__aicore__ inline void asc_copy_l0c2l1_vreq8_sync_impl(__cbuf__ uint8_t *dst, __cc__ int32_t *src, uint8_t sid, uint16_t n_size,
    uint16_t m_size, uint32_t dst_stride_dst_d, uint16_t src_stride, uint8_t uint_flag_mode, uint8_t relu_pre,
    bool channel_split, bool nd2nz_en)
{
    asc_copy_l0c2l1_vreq8_impl(dst, src, sid, n_size, m_size, dst_stride_dst_d, src_stride, uint_flag_mode, relu_pre,
        channel_split, nd2nz_en);
    asc_sync_post_process();
}

#endif