/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#ifndef IMPL_INSTR_IMPL_CUBE_DATAMOVE_IMPL_COPY_MATRIX_CC_TO_GM_IMPL_NPU_ARCH_2201_ASC_FIXPIPE_L0C2OUT_VREQ8_IMPL_H
#define IMPL_INSTR_IMPL_CUBE_DATAMOVE_IMPL_COPY_MATRIX_CC_TO_GM_IMPL_NPU_ARCH_2201_ASC_FIXPIPE_L0C2OUT_VREQ8_IMPL_H

#include "c_api_instr_impl/utils_c_api_impl/utils_c_api_impl.h"

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ half* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ half* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ half* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ half* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ float* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ float* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ bfloat16_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ bfloat16_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ int8_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ int8_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ int8_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ int8_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ uint8_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ uint8_t* dst, __cc__ float* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ uint8_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ uint8_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ int16_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ int16_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_impl(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, QuantMode_t::VREQ8, relu_pre, channel_split, nz2nd_en);
    }
}

__aicore__ inline void asc_fixpipe_l0c2out_vreq8_sync_impl(__gm__ int32_t* dst, __cc__ int32_t* src, uint8_t sid, 
    uint16_t n_size, uint16_t m_size, uint16_t dst_stride_dst_d, uint16_t src_stride, uint8_t unit_flag_mode,
    uint8_t relu_pre, bool channel_split, bool nz2nd_en)
{
    asc_fixpipe_l0c2out_vreq8_impl(dst, src, sid, n_size, m_size,dst_stride_dst_d,
                             src_stride, unit_flag_mode, relu_pre, channel_split, nz2nd_en);
    asc_sync_post_process();
}

#endif