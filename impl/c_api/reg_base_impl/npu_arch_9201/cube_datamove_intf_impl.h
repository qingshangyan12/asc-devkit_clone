/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/npu_arch_9201/cube_datamove_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use \"#include \"c_api/asc_simd.h\"\" and use public functions or variables defined in interface headers files.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

constexpr uint8_t RELU_POST_DEFAULT = 0;

constexpr bool CLIP_RELU_POST_DEFAULT = false;

constexpr uint8_t ELTWISE_OP_DEFAULT = 0;

constexpr uint64_t QUANT_POST_DEFAULT = 0;

// ==========asc_copy_l12l0a==========
// hif4x2_t
__aicore__ inline void asc_copy_l12l0a(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(
            (__ca__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 0);
    }
}

// transpose
__aicore__ inline void asc_copy_l12l0a_transpose(
    __ca__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_ca_s4(
            (__ca__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 1);
    }
}

// ==========asc_copy_l12l0b==========
// hif4x2_t
__aicore__ inline void asc_copy_l12l0b(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 0);
    }
}

// transpose
__aicore__ inline void asc_copy_l12l0b_transpose(
    __cb__ hif4x2_t* dst, __cbuf__ hif4x2_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    if ASC_IS_AIC {
        load_cbuf_to_cb_s4(
            (__cb__ void*)dst, (__cbuf__ void*)src, m_start_position, k_start_position, m_step, k_step, src_stride,
            dst_stride, 1);
    }
}

// ==========asc_copy_l0c2gm==========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), false,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm(
            dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode), quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            false, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            quant_pre_rnd, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            false, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2gm(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_store_l2_cache_mode l2_cache_mode, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode,
    asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd,
    bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_gm_s4(
            (__gm__ void*)dst, src, 0, n_size, m_size, dst_stride, src_stride, static_cast<uint8_t>(l2_cache_mode),
            quant_pre_rnd, enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// ==========asc_copy_l0c2ub==========
// bfloat16_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// fp8_e4m3fn_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// hifloat8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// float  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// bfloat16_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// half  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode, asc_relu_pre_mode relu_pre_mode,
    bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn, bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int8_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// uint8_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int32_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub(
            dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  float
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

// int4b_t  int32_t
__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, false, enable_clip_relu_pre,
            static_cast<uint8_t>(unit_flag_mode), quant_pre_mode, static_cast<uint8_t>(relu_pre_mode),
            enable_channel_split, enable_nz2nd, static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT,
            CLIP_RELU_POST_DEFAULT, false, ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

__aicore__ inline void asc_copy_l0c2ub(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, bool quant_pre_rnd, asc_unit_flag_mode unit_flag_mode, asc_quant_mode quant_pre_mode,
    asc_relu_pre_mode relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, bool enable_nz2dn,
    bool enable_clip_relu_pre)
{
    if ASC_IS_AIC {
        copy_matrix_cc_to_ub_s4(
            (__ubuf__ void*)dst, src, n_size, m_size, dst_stride, src_stride, 0, false, quant_pre_rnd,
            enable_clip_relu_pre, static_cast<uint8_t>(unit_flag_mode), quant_pre_mode,
            static_cast<uint8_t>(relu_pre_mode), enable_channel_split, enable_nz2nd,
            static_cast<QuantMode_post>(QUANT_POST_DEFAULT), RELU_POST_DEFAULT, CLIP_RELU_POST_DEFAULT, false,
            ELTWISE_OP_DEFAULT, false, false, false, false, false, enable_nz2dn);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
