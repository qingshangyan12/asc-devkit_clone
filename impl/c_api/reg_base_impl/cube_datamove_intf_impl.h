/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/cube_datamove_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

// asc_copy_l0c2l1_impl.h
// half  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// half int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// int32_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, false);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// half int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// bfloat16_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// int32_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, 0, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, false, enable_nz2dn);
    }
}

// int4b_t float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int4b_t int32_t
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf_s4(
            (__cbuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// half float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// bfloat16_t float
__aicore__ inline void asc_copy_l0c2l1_impl(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_cbuf(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// asc_copy_l12l0a_impl.h
__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_ca(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0a_impl(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

// transpose

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0a_transpose_impl(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

// asc_copy_l0c2gm_impl.h
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int8  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// uint8  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int8  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// uint8  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
            static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2gm_impl(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// asc_copy_l0c2ub_impl.h
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int8  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// uint8  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int8  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// uint8  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
            unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode, enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post, false, eltwise_op, eltwise_antq_en,
            false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2ub_impl(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid,
            enable_clip_relu_pre, unit_flag_mode, static_cast<QuantMode_t>(quant_pre_mode), relu_pre_mode,
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(quant_post), relu_post, clip_relu_post,
            false, eltwise_op, eltwise_antq_en, false, c0_pad_en, false, broadcast_en, enable_nz2dn);
    }
}

// asc_copy_l12l0a_mx_impl.h
__aicore__ inline void asc_copy_l12l0a_mx_impl(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    }
}

// asc_copy_l12l0b_impl.h
__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    if ASC_IS_AIC {
        img2colv2_cbuf_to_cb(
            dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
            dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

__aicore__ inline void asc_copy_l12l0b_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 0);
    }
}

// transpose
__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

__aicore__ inline void asc_copy_l12l0b_transpose_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride, 1);
    }
}

// asc_copy_l12l0b_mx_impl.h
__aicore__ inline void asc_copy_l12l0b_mx_impl(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_mx(dst, (__cbuf__ void*)src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    }
}

// asc_set_l13d_rpt_impl.h
constexpr uint8_t ASC_L13D_RPT_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L13D_RPT_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L13D_RPT_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L13D_RPT_DST_START_POS_M_SHIFT = 48;

__aicore__ inline void asc_set_l13d_rpt_impl(asc_load3d_v2_config& config)
{
    if ASC_IS_AIC {
        set_l3d_rpt(config.config);
    }
}

__aicore__ inline void asc_set_l13d_rpt_impl(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_start_pos_m) << ASC_L13D_RPT_DST_START_POS_M_SHIFT) |
                          (static_cast<uint64_t>(dst_stride_k) << ASC_L13D_RPT_DST_STRIDE_K_SHIFT) |
                          (static_cast<uint64_t>(repeat_direction) << ASC_L13D_RPT_DIRECTION_SHIFT) |
                          (static_cast<uint64_t>(repeat_times) << ASC_L13D_RPT_REPEAT_TIMES_SHIFT) |
                          static_cast<uint64_t>(repeat_stride);
        set_l3d_rpt(config);
    }
}

// asc_set_l13d_fmatrix_impl.h
constexpr uint8_t ASC_L13D_FMATRIX_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_PAD_BOTTOM_SHIFT = 56;

__aicore__ inline void asc_set_l13d_fmatrix_impl(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix(config.config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_impl(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(fmatrix_w) |
                          (static_cast<uint64_t>(fmatrix_h) << ASC_L13D_FMATRIX_H_SHIFT) |
                          (static_cast<uint64_t>(pad_left) << ASC_L13D_FMATRIX_PAD_LEFT_SHIFT) |
                          (static_cast<uint64_t>(pad_right) << ASC_L13D_FMATRIX_PAD_RIGHT_SHIFT) |
                          (static_cast<uint64_t>(pad_top) << ASC_L13D_FMATRIX_PAD_TOP_SHIFT) |
                          (static_cast<uint64_t>(pad_bottom) << ASC_L13D_FMATRIX_PAD_BOTTOM_SHIFT);
        set_fmatrix(config);
    }
}

// asc_set_l13d_fmatrix_b_impl.h
constexpr uint8_t ASC_L13D_FMATRIX_B_H_SHIFT = 16;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_LEFT_SHIFT = 32;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_RIGHT_SHIFT = 40;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_TOP_SHIFT = 48;

constexpr uint8_t ASC_L13D_FMATRIX_B_PAD_BOTTOM_SHIFT = 56;

__aicore__ inline void asc_set_l13d_fmatrix_b_impl(asc_l13d_fmatrix_config& config)
{
    if ASC_IS_AIC {
        set_fmatrix_b(config.config);
    }
}

__aicore__ inline void asc_set_l13d_fmatrix_b_impl(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(fmatrix_w) |
                          (static_cast<uint64_t>(fmatrix_h) << ASC_L13D_FMATRIX_B_H_SHIFT) |
                          (static_cast<uint64_t>(pad_left) << ASC_L13D_FMATRIX_B_PAD_LEFT_SHIFT) |
                          (static_cast<uint64_t>(pad_right) << ASC_L13D_FMATRIX_B_PAD_RIGHT_SHIFT) |
                          (static_cast<uint64_t>(pad_top) << ASC_L13D_FMATRIX_B_PAD_TOP_SHIFT) |
                          (static_cast<uint64_t>(pad_bottom) << ASC_L13D_FMATRIX_B_PAD_BOTTOM_SHIFT);
        set_fmatrix_b(config);
    }
}

// asc_set_l0c2gm_lrelu_alpha_impl.h
__aicore__ inline void asc_set_l0c2gm_lrelu_alpha_impl(half& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha_impl(float& config)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(config);
    }
}

// asc_set_l0c_copy_lrelu_alpha_impl.h
__aicore__ inline void asc_set_l0c_copy_lrelu_alpha_impl(float scalar_relu_pre_alpha)
{
    if ASC_IS_AIC {
        set_lrelu_alpha(scalar_relu_pre_alpha);
    }
}

// asc_set_l0c_copy_prequant_impl.h
__aicore__ inline void asc_set_l0c_copy_prequant_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_quant_pre(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_prequant_impl(float scale, uint16_t offset, bool is_signed)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(is_signed) << 46) | (static_cast<uint64_t>(offset & 0x1FF) << 37) |
                          static_cast<uint64_t>(*(reinterpret_cast<uint32_t*>(&scale)) & 0xFFFFE000u);
        set_quant_pre(config);
    }
}

// asc_set_gm2l1_loop_size_impl.h
__aicore__ inline void asc_set_gm2l1_loop_size_impl(uint64_t loop1_size, uint64_t loop2_size)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_size_config config;
        config.loop1_size = loop1_size;
        config.loop2_size = loop2_size;
        set_loop_size_outtol1(config.config);
    }
}

// asc_set_gm2l1_loop1_stride_impl.h
__aicore__ inline void asc_set_gm2l1_loop1_stride_impl(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_stride_config config;
        config.src_stride = loop1_src_stride;
        config.dst_stride = loop1_dst_stride;
        set_loop1_stride_outtol1(config.config);
    }
}

// asc_set_gm2l1_loop2_stride_impl.h
__aicore__ inline void asc_set_gm2l1_loop2_stride_impl(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    if ASC_IS_AIC {
        asc_gm2l1_loop_stride_config config;
        config.src_stride = loop2_src_stride;
        config.dst_stride = loop2_dst_stride;
        set_loop2_stride_outtol1(config.config);
    }
}

// asc_set_gm2l1_pad_impl.h
__aicore__ inline void asc_set_gm2l1_pad_impl(uint32_t pad_val)
{
    if ASC_IS_AIC {
        set_pad_val_outtol1(pad_val);
    }
}

// asc_copy_l12l0b_trans_impl.h
__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(dst, src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap, src_frac_gap);
    }
}

__aicore__ inline void asc_copy_l12l0b_trans_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_transpose_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, index_id, repeat, src_stride, dst_gap, inc, dst_frac_gap,
            src_frac_gap);
    }
}

// asc_copy_gm2l1_align_impl.h
__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
            burst_src_stride, burst_dst_stride);
    }
}

// asc_copy_gm2l1_dn2nz_impl.h
__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

// asc_copy_gm2l1_nd2nz_impl.h
__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    }
}

// asc_copy_gm2l1_impl.h
constexpr uint8_t ASC_GM2L1_SID_DEFAULT = 0;

constexpr uint8_t ASC_GM2L1_DECOMP_MODE_DEFAULT = 0;

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(dst, src, 0, n_burst, len_burst, pad_func_mode, 0, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(dst, src, 0, n_burst, len_burst, static_cast<uint8_t>(pad_mode), 0, src_stride, dst_stride);
    }
}

// bfloat16_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// float
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// fp8_e4m3fn_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// fp8_e5m2_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// half
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// hifloat8_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// int16_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// int32_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// int8_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// uint16_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// uint32_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// uint8_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// fp4x2_e1m2_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// fp4x2_e2m1_t
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// void
__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, 0, decomp_mode, l2_cache_ctl);
    }
}

__aicore__ inline void asc_copy_gm2l1_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, static_cast<uint8_t>(l2_cache_mode));
    }
}

// asc_copy_l12fb_impl.h
__aicore__ inline void asc_copy_l12fb_impl(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size)
{
    if ASC_IS_AIC {
        copy_cbuf_to_fbuf(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
    }
}

__aicore__ inline void asc_copy_l12fb_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_l12bt_impl.h
// ========== void -> uint64_t ==========
__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// ========== bfloat16_t -> uint64_t ==========
__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// ========== half -> uint64_t ==========
__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// ========== float -> uint64_t ==========
__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// ========== int32_t -> uint64_t ==========
__aicore__ inline void asc_copy_l12bt_impl(
    uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_bt(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_l12bt_impl(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, 0, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_l12ub_impl.h
__aicore__ inline void asc_copy_l12ub_impl(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, int8_t sub_blockid, uint16_t burst_count, uint16_t burst_len,
    uint16_t src_gap, uint16_t dst_gap)
{
    if ASC_IS_AIC {
        copy_cbuf_to_ubuf(dst_addr, src_addr, sub_blockid, burst_count, burst_len, src_gap, dst_gap);
    }
}

// asc_fill_l1_impl.h
// asc_fill_l1_value half
__aicore__ inline void asc_fill_l1_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value float
__aicore__ inline void asc_fill_l1_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value int16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value int32_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value uint16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value uint32_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

__aicore__ inline void asc_fill_l1_impl(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix(dst, config.config, value);
    }
}

// asc_fill_l1_value bfloat16_t
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_bf16(dst, config.config, value);
    }
}

// create_cbuf_matrix_h
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_h(dst, config.config, value);
    }
}

// create_cbuf_matrix_ui
__aicore__ inline void asc_fill_l1_impl(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    if ASC_IS_AIC {
        create_cbuf_matrix_ui(dst, config.config, value);
    }
}

// asc_set_gm2l1_nz_para_impl.h
__aicore__ inline void asc_set_gm2l1_nz_para_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_mte2_nz_para(config);
    }
}

__aicore__ inline void asc_set_gm2l1_nz_para_impl(
    uint16_t matrix_num, uint16_t dst_nz_n_stride, uint16_t dst_nz_c0_stride, uint16_t dst_nz_matrix_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_nz_matrix_stride) << 48) |
                          (static_cast<uint64_t>(dst_nz_c0_stride) << 32) |
                          (static_cast<uint64_t>(dst_nz_n_stride) << 16) | static_cast<uint64_t>(matrix_num);
        set_mte2_nz_para(config);
    }
}

// asc_set_l12l0a_3d_padding_impl.h
constexpr uint8_t ASC_3D_PADDING_SHIFT_BIT = 8;

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding(config);
    }
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(int8_t padding_value)
{
    uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(padding_value));
    asc_set_l12l0a_3d_padding_impl((value << ASC_3D_PADDING_SHIFT_BIT) | value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(uint8_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl((static_cast<uint64_t>(padding_value) << ASC_3D_PADDING_SHIFT_BIT) | padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(int16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int16 = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(uint16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint16 = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(half padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_half = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(bfloat16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_bfloat16 = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(int32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int32 = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(uint32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint32 = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0a_3d_padding_impl(float padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_float = padding_value;
    asc_set_l12l0a_3d_padding_impl(bitcode.output);
}

// asc_set_gm2l1_padding_impl.h
__aicore__ inline void asc_set_gm2l1_padding_impl(uint64_t config) { asc_set_l12l0a_3d_padding_impl(config); }

__aicore__ inline void asc_set_gm2l1_padding_impl(int8_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(uint8_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(int16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(uint16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(half padding_value) { asc_set_l12l0a_3d_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding_impl(bfloat16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(int32_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(uint32_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_gm2l1_padding_impl(float padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

// asc_set_l0c2gm_channel_para_impl.h
constexpr uint8_t ASC_L0C_COPY_CHANNEL_PARA_SHIFT = 48;

__aicore__ inline void asc_set_l0c2gm_channel_para_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_channel_para(config);
    }
}

__aicore__ inline void asc_set_l0c_copy_channel_para_impl(uint16_t src_nz_fractal_stride)
{
    if ASC_IS_AIC {
        uint64_t config = static_cast<uint64_t>(src_nz_fractal_stride) << ASC_L0C_COPY_CHANNEL_PARA_SHIFT;
        set_channel_para(config);
    }
}

// asc_set_l0c2gm_config_impl.h
__aicore__ inline void asc_set_l0c2gm_config_impl(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = 0;
        config.relu_units = relu_pre;
        config.quant_units = quant_pre;
        config.unit_flag = enable_unit_flag;
        set_fpc(config.config);
    }
}

// asc_set_l0c2gm_nz2nd_impl.h
__aicore__ inline void asc_set_l0c2gm_nz2nd_impl(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (dst_nd_stride << 32) | (src_nd_stride << 16) | nd_num;
        set_loop3_para(config);
    }
}

// asc_set_l0c2gm_quant_post_impl.h
__aicore__ inline void asc_set_l0c2gm_quant_post_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_quant_post(config);
    }
}

// asc_set_l0c2gm_relu_alpha_impl.h
__aicore__ inline void asc_set_l0c2gm_relu_alpha_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_relu_alpha(config);
    }
}

// asc_set_l0c_copy_config_impl.h
__aicore__ inline void asc_set_l0c_copy_config_impl(
    uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool is_clean_unit_flag)
{
    if ASC_IS_AIC {
        asc_capi_fpc_reg_config config;
        config.config = 0;
        config.relu_units = relu_pre_addr;
        config.quant_units = quant_pre_addr;
        config.unit_flag = is_clean_unit_flag;
        set_fpc(config.config);
    }
}

// asc_set_l0c_copy_nz_para_impl.h
constexpr uint8_t ASC_L0C_COPY_NZ_PARA_SRC_SHIFT = 16;

constexpr uint8_t ASC_L0C_COPY_NZ_PARA_DST_SHIFT = 32;

__aicore__ inline void asc_set_l0c_copy_nz_para_impl(
    uint16_t matrix_num, uint16_t src_nz_matrix_stride, uint32_t dst_matrix_stride)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_matrix_stride) << ASC_L0C_COPY_NZ_PARA_DST_SHIFT) |
                          (static_cast<uint64_t>(src_nz_matrix_stride) << ASC_L0C_COPY_NZ_PARA_SRC_SHIFT) | matrix_num;
        set_loop3_para(config);
    }
}

// asc_set_l0c_copy_relu_alpha_impl.h
__aicore__ inline void asc_set_l0c_copy_relu_alpha_impl(float scalar_relu_pre_alpha)
{
    if ASC_IS_AIC {
        set_relu_alpha(static_cast<uint64_t>(*(reinterpret_cast<uint32_t*>(&scalar_relu_pre_alpha)) & 0xFFFFE000u));
    }
}

// asc_set_l12l0_padding_val_impl.h
__aicore__ inline void asc_set_l12l0_padding_val_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding_b(config);
    }
}

// asc_set_l12l0b_3d_padding_impl.h
constexpr uint8_t ASC_3D_PADDING_B_SHIFT_BIT = 8;

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_padding_b(config);
    }
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(int8_t padding_value)
{
    uint64_t value = static_cast<uint64_t>(static_cast<uint8_t>(padding_value));
    asc_set_l12l0b_3d_padding_impl((value << ASC_3D_PADDING_B_SHIFT_BIT) | value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(uint8_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(
        (static_cast<uint64_t>(padding_value) << ASC_3D_PADDING_B_SHIFT_BIT) | padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(int16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int16 = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(uint16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint16 = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(half padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_half = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(bfloat16_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_bfloat16 = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(int32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_int32 = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(uint32_t padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_uint32 = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

__aicore__ inline void asc_set_l12l0b_3d_padding_impl(float padding_value)
{
    asc_3d_padding_bitcode bitcode;
    bitcode.output = 0;
    bitcode.input_float = padding_value;
    asc_set_l12l0b_3d_padding_impl(bitcode.output);
}

// asc_set_l13d_padding_impl.h
__aicore__ inline void asc_set_l13d_padding_impl(uint64_t config)
{
    if ASC_IS_AIC {
        uint64_t paddingValue = 0;
        uint64_t padValueShiftBit = 8;
        paddingValue = (config << padValueShiftBit) | (config & 0xFF);
        set_padding(paddingValue);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(half config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_half = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(int16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_int16_t = config;
        set_padding(bitcode.output);
    }
}

__aicore__ inline void asc_set_l13d_padding_impl(uint16_t config)
{
    if ASC_IS_AIC {
        asc_scalar_bitcode bitcode;
        bitcode.input_uint16_t = config;
        set_padding(bitcode.output);
    }
}

// asc_set_l3d_rpt_b_impl.h
constexpr uint8_t ASC_L3D_RPT_B_REPEAT_TIMES_SHIFT = 16;

constexpr uint8_t ASC_L3D_RPT_B_DIRECTION_SHIFT = 24;

constexpr uint8_t ASC_L3D_RPT_B_DST_STRIDE_K_SHIFT = 32;

constexpr uint8_t ASC_L3D_RPT_B_DST_START_POS_M_SHIFT = 48;

__aicore__ inline void asc_set_l3d_rpt_b_impl(uint64_t config)
{
    if ASC_IS_AIC {
        set_l3d_rpt_b(config);
    }
}

__aicore__ inline void asc_set_l3d_rpt_b_impl(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    if ASC_IS_AIC {
        uint64_t config = (static_cast<uint64_t>(dst_start_pos_m) << ASC_L3D_RPT_B_DST_START_POS_M_SHIFT) |
                          (static_cast<uint64_t>(dst_stride_k) << ASC_L3D_RPT_B_DST_STRIDE_K_SHIFT) |
                          (static_cast<uint64_t>(repeat_direction) << ASC_L3D_RPT_B_DIRECTION_SHIFT) |
                          (static_cast<uint64_t>(repeat_times) << ASC_L3D_RPT_B_REPEAT_TIMES_SHIFT) |
                          static_cast<uint64_t>(repeat_stride);
        set_l3d_rpt_b(config);
    }
}

// cube_datamove_impl.h
constexpr uint8_t RELU_POST_DEFAULT = 0;

constexpr bool CLIP_RELU_POST_DEFAULT = false;

constexpr uint8_t ELTWISE_OP_DEFAULT = 0;

constexpr uint64_t QUANT_POST_DEFAULT = 0;

__aicore__ inline void asc_set_l0c2gm_config(uint64_t relu_pre, uint64_t quant_pre, bool enable_unit_flag)
{
    asc_set_l0c2gm_config_impl(relu_pre, quant_pre, enable_unit_flag);
}

__aicore__ inline void asc_set_l0c2gm_nz2nd(uint64_t nd_num, uint64_t src_nd_stride, uint64_t dst_nd_stride)
{
    asc_set_l0c2gm_nz2nd_impl(nd_num, src_nd_stride, dst_nd_stride);
}

__aicore__ inline void asc_set_l0c_copy_config(uint64_t relu_pre_addr, uint64_t quant_pre_addr, bool is_clean_unit_flag)
{
    asc_set_l0c_copy_config_impl(relu_pre_addr, quant_pre_addr, is_clean_unit_flag);
}

__aicore__ inline void asc_set_l0c_copy_nz_para(
    uint16_t matrix_num, uint16_t src_nz_matrix_stride, uint32_t dst_matrix_stride)
{
    asc_set_l0c_copy_nz_para_impl(matrix_num, src_nz_matrix_stride, dst_matrix_stride);
}

__aicore__ inline void asc_copy_l12l0a_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_mx(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

__aicore__ inline void asc_set_l0c_copy_prequant(uint64_t config) { asc_set_l0c_copy_prequant_impl(config); }

__aicore__ inline void asc_set_l0c_copy_prequant(float scale, uint16_t offset, bool is_signed)
{
    asc_set_l0c_copy_prequant_impl(scale, offset, is_signed);
}

__aicore__ inline void asc_set_l0c_copy_channel_para(uint16_t src_nz_fractal_stride)
{
    asc_set_l0c_copy_channel_para_impl(src_nz_fractal_stride);
}

__aicore__ inline void asc_set_l0c2gm_channel_para(uint64_t config) { asc_set_l0c2gm_channel_para_impl(config); }

__aicore__ inline void asc_set_l3d_rpt_b(uint64_t config) { asc_set_l3d_rpt_b_impl(config); }

__aicore__ inline void asc_set_l13d_rpt_b(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    asc_set_l3d_rpt_b_impl(repeat_stride, repeat_times, repeat_direction, dst_stride_k, dst_start_pos_m);
}

__aicore__ inline void asc_set_gm2l1_nz_para(uint64_t config) { asc_set_gm2l1_nz_para_impl(config); }

__aicore__ inline void asc_set_gm2l1_nz_para(
    uint16_t matrix_num, uint16_t dst_nz_n_stride, uint16_t dst_nz_c0_stride, uint16_t dst_nz_matrix_stride)
{
    asc_set_gm2l1_nz_para_impl(matrix_num, dst_nz_n_stride, dst_nz_c0_stride, dst_nz_matrix_stride);
}

__aicore__ inline void asc_set_l13d_padding(uint64_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(half config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(int16_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l13d_padding(uint16_t config) { asc_set_l13d_padding_impl(config); }

__aicore__ inline void asc_set_l12l0a_3d_padding(uint64_t config) { asc_set_l12l0a_3d_padding_impl(config); }

__aicore__ inline void asc_set_l12l0a_3d_padding(int8_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint8_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(int16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(half padding_value) { asc_set_l12l0a_3d_padding_impl(padding_value); }

__aicore__ inline void asc_set_l12l0a_3d_padding(bfloat16_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(int32_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(uint32_t padding_value)
{
    asc_set_l12l0a_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0a_3d_padding(float padding_value) { asc_set_l12l0a_3d_padding_impl(padding_value); }

__aicore__ inline void asc_set_l12l0b_3d_padding(uint64_t config) { asc_set_l12l0b_3d_padding_impl(config); }

__aicore__ inline void asc_set_l12l0b_3d_padding(int8_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint8_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(int16_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint16_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(half padding_value) { asc_set_l12l0b_3d_padding_impl(padding_value); }

__aicore__ inline void asc_set_l12l0b_3d_padding(bfloat16_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(int32_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(uint32_t padding_value)
{
    asc_set_l12l0b_3d_padding_impl(padding_value);
}

__aicore__ inline void asc_set_l12l0b_3d_padding(float padding_value) { asc_set_l12l0b_3d_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(uint64_t config) { asc_set_gm2l1_padding_impl(config); }

__aicore__ inline void asc_set_gm2l1_padding(int8_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(uint8_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(int16_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(uint16_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(half padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(bfloat16_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(int32_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(uint32_t padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_gm2l1_padding(float padding_value) { asc_set_gm2l1_padding_impl(padding_value); }

__aicore__ inline void asc_set_l12l0_padding_val(uint64_t config) { asc_set_l12l0_padding_val_impl(config); }

__aicore__ inline void asc_set_l0c2gm_quant_post(uint64_t config) { asc_set_l0c2gm_quant_post_impl(config); }

__aicore__ inline void asc_set_l0c2gm_relu_alpha(uint64_t config) { asc_set_l0c2gm_relu_alpha_impl(config); }

__aicore__ inline void asc_set_l0c_copy_relu_alpha(float scalar_relu_pre_alpha)
{
    asc_set_l0c_copy_relu_alpha_impl(scalar_relu_pre_alpha);
}

__aicore__ inline void asc_set_gm2l1_loop_size(uint64_t loop1_size, uint64_t loop2_size)
{
    asc_set_gm2l1_loop_size_impl(loop1_size, loop2_size);
}

__aicore__ inline void asc_set_gm2l1_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    asc_set_gm2l1_loop1_stride_impl(loop1_src_stride, loop1_dst_stride);
}

__aicore__ inline void asc_set_gm2l1_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    asc_set_gm2l1_loop2_stride_impl(loop2_src_stride, loop2_dst_stride);
}

__aicore__ inline void asc_set_gm2l1_pad(uint32_t pad_val) { asc_set_gm2l1_pad_impl(pad_val); }

// ==========asc_set_l13d_rpt==========
__aicore__ inline void asc_set_l13d_rpt(asc_load3d_v2_config& config) { asc_set_l13d_rpt_impl(config); }

__aicore__ inline void asc_set_l13d_rpt(
    uint16_t repeat_stride, uint8_t repeat_times, asc_l13d_repeat_direction repeat_direction, uint16_t dst_stride_k,
    uint16_t dst_start_pos_m)
{
    asc_set_l13d_rpt_impl(repeat_stride, repeat_times, repeat_direction, dst_stride_k, dst_start_pos_m);
}

// ==========asc_set_l13d_fmatrix==========
__aicore__ inline void asc_set_l13d_fmatrix(asc_l13d_fmatrix_config& config) { asc_set_l13d_fmatrix_impl(config); }

__aicore__ inline void asc_set_l13d_fmatrix(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    asc_set_l13d_fmatrix_impl(fmatrix_w, fmatrix_h, pad_left, pad_right, pad_top, pad_bottom);
}

// ==========asc_copy_l12l0a,
// uint8_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========

__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// transpose

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// ==========asc_copy_l12l0b,
// uint8_t/int8_t/fp8_e4m3fn_t/fp8_e5m2_t/hifloat8_t/half/bfloat16_t/int16_t/uint16_t/uint32_t/int32_t/float==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// transpose

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

// ==========asc_copy_l12l0b_trans,
// bfloat16_t/fp8_e4m3fn_t/fp8_e5m2_t/half/float/hifloat8_t/int32_t/int8_t/uint32_t/uint8_t/fp4x2_e1m2_t/fp4x2_e2m1_t/int4b_t==========
__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

__aicore__ inline void asc_copy_l12l0b_trans(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

// ==========asc_copy_l12l0a==========
__aicore__ inline void asc_copy_l12l0a(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0a(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_copy_l12l0b==========
__aicore__ inline void asc_copy_l12l0b(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

__aicore__ inline void asc_copy_l12l0b(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

// ==========asc_copy_gm2l1_align==========
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

// ==========asc_copy_gm2l1_dn2nz==========
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_dn2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

// ==========asc_copy_gm2l1_nd2nz==========
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0)
{
    asc_copy_gm2l1_nd2nz_impl(
        dst, src, src_d_value, l2_cache_mode, n_value, d_value, src_nd_matrix_stride, enable_small_c0);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

// ==========asc_copy_gm2l1==========
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_impl(dst, src, n_burst, len_burst, pad_func_mode, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_impl(dst, src, n_burst, len_burst, pad_mode, src_stride, dst_stride);
}

// =============asc_copy_l12fb===============
__aicore__ inline void asc_copy_l12fb(
    __fbuf__ void* dst, __cbuf__ void* src, uint16_t n_burst, uint16_t len_burst, uint16_t src_gap_size,
    uint16_t dst_gap_size)
{
    asc_copy_l12fb_impl(dst, src, n_burst, len_burst, src_gap_size, dst_gap_size);
}

__aicore__ inline void asc_copy_l12fb(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
}

// =============asc_copy_l12ub===============
__aicore__ inline void asc_copy_l12ub(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, int8_t sub_blockid, uint16_t burst_count, uint16_t burst_len,
    uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12ub_impl(dst_addr, src_addr, sub_blockid, burst_count, burst_len, src_gap, dst_gap);
}

//=============asc_copy_l12bt===============
// void -> uint64_t
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ void* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

// bfloat16_t -> uint64_t
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ bfloat16_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

// half -> uint64_t
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ half* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

// float -> uint64_t
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ float* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst, uint16_t source_gap,
    uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

// int32_t -> uint64_t
__aicore__ inline void asc_copy_l12bt(
    uint64_t dst, __cbuf__ int32_t* src, uint16_t conv_control, uint16_t n_burst, uint16_t len_burst,
    uint16_t source_gap, uint16_t dst_gap)
{
    asc_copy_l12bt_impl(dst, src, conv_control, n_burst, len_burst, source_gap, dst_gap);
}

__aicore__ inline void asc_copy_l12bt(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
}

//=============asc_copy_l0c2l1===============
// half  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// float  float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// half int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode, uint8_t relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post, bool clip_relu_post,
    uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// int8_t int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// uint8_t int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// int32_t int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, uint8_t eltwise_antq_en, bool c0_pad_en)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, enable_clip_relu_pre, unit_flag_mode, quant_pre_mode,
        relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post, eltwise_op,
        eltwise_antq_en, c0_pad_en);
}

// int4b_t float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int4b_t int32_t
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// half float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// bfloat16_t float
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// ==========asc_copy_l0c2gm===========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// half  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// float  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// ==========asc_copy_l0c2ub===========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// half  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int8  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// uint8  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// float  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int8  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// uint8  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

// l0c2gm new overloads
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
        static_cast<uint8_t>(enable_clip_relu_pre), static_cast<uint8_t>(unit_flag_mode),
        static_cast<uint64_t>(quant_pre_mode), static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
        QUANT_POST_DEFAULT, RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, ELTWISE_OP_DEFAULT, false, false, false,
        enable_nz2dn);
}

// l0c2l1 new overloads
__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2l1(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    asc_copy_l0c2l1_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, 0, static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        false, false, false, false, false, enable_nz2dn);
}

// l0c2ub new overloads
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, int8_t sub_blockid, asc_dual_dst_mode dual_dst_ctrl, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    asc_copy_l0c2ub_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(dual_dst_ctrl),
        static_cast<bool>(sub_blockid), static_cast<uint8_t>(enable_clip_relu_pre),
        static_cast<uint8_t>(unit_flag_mode), static_cast<uint64_t>(quant_pre_mode),
        static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd, QUANT_POST_DEFAULT, RELU_POST_DEFAULT,
        CLIP_RELU_POST_DEFAULT, false, false, false, false, enable_nz2dn);
}

// ==========asc_copy_gm2l1===========
// bfloat16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// float
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// fp8_e4m3fn_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// fp8_e5m2_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// half
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// hifloat8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// int16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// int32_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// int8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// uint16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// uint32_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// uint8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// fp4x2_e1m2_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// fp4x2_e2m1_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// void
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

// bfloat16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// float
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// fp8_e4m3fn_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// fp8_e5m2_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// half
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// hifloat8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// int16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// int32_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// int8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// uint16_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// uint32_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// uint8_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// fp4x2_e1m2_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// fp4x2_e2m1_t
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// void
__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, asc_load_l2_cache_mode l2_cache_mode)
{
    asc_copy_gm2l1_impl(dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, l2_cache_mode);
}

// ==========asc_fill_l1(half/float/int16_t/int32_t/uint16_t/uint32_t/bfloat16_t)==========
__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_fill_l1(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
}

__aicore__ inline void asc_set_l13d_fmatrix_b(asc_l13d_fmatrix_config& config) { asc_set_l13d_fmatrix_b_impl(config); }

__aicore__ inline void asc_set_l13d_fmatrix_b(
    uint16_t fmatrix_w, uint16_t fmatrix_h, uint8_t pad_left, uint8_t pad_right, uint8_t pad_top, uint8_t pad_bottom)
{
    asc_set_l13d_fmatrix_b_impl(fmatrix_w, fmatrix_h, pad_left, pad_right, pad_top, pad_bottom);
}

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(half& config) { asc_set_l0c2gm_lrelu_alpha_impl(config); }

__aicore__ inline void asc_set_l0c2gm_lrelu_alpha(float& config) { asc_set_l0c2gm_lrelu_alpha_impl(config); }

__aicore__ inline void asc_set_l0c_copy_lrelu_alpha(float scalar_relu_pre_alpha)
{
    asc_set_l0c_copy_lrelu_alpha_impl(scalar_relu_pre_alpha);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_CUBE_DATAMOVE_INTF_IMPL_H
