/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/npu_arch_3510/composite_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/cube_compute_intf_impl.h"
#include "impl/c_api/reg_base_impl/cube_datamove_intf_impl.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"
#include "impl/c_api/reg_base_impl/vector_compute_intf_impl.h"
#include "impl/c_api/reg_base_impl/vector_datamove_intf_impl.h"

// asc_mmad_mx_impl.h
__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_mx_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

// asc_mmad_impl.h
__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

__aicore__ inline void asc_mmad_sync_impl(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
    asc_sync_post_process();
}

// cube_compute_impl.h
ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e1m2_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e1m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp4x2_e2m1_t* a_matrix, __cb__ fp4x2_e2m1_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad_mx and asc_sync)
__aicore__ inline void asc_mmad_mx_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_mx_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ bfloat16_t* a_matrix, __cb__ bfloat16_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e4m3fn_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e4m3fn_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ fp8_e5m2_t* a_matrix, __cb__ fp8_e5m2_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ half* a_matrix, __cb__ half* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ float* a_matrix, __cb__ float* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ int32_t* c_matrix, __ca__ int8_t* a_matrix, __cb__ int8_t* b_matrix, uint16_t left_height, uint16_t n_dim,
    uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source, bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mmad and asc_sync)
__aicore__ inline void asc_mmad_sync(
    __cc__ float* c_matrix, __ca__ hifloat8_t* a_matrix, __cb__ hifloat8_t* b_matrix, uint16_t left_height,
    uint16_t n_dim, uint16_t right_width, uint8_t unit_flag, bool disable_gemv, bool c_matrix_source,
    bool c_matrix_init_val)
{
    asc_mmad_sync_impl(
        c_matrix, a_matrix, b_matrix, left_height, n_dim, right_width, unit_flag, disable_gemv, c_matrix_source,
        c_matrix_init_val);
}

// asc_copy_l0c2l1_impl.h
__aicore__ inline void asc_copy_l0c2l1_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2l1_sync_impl(
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
    asc_sync_post_process();
}

// asc_copy_l12l0a_impl.h
__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_sync_impl(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0a_transpose_sync_impl(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

// asc_copy_l0c2gm_impl.h
__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2gm_sync_impl(
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
    asc_sync_post_process();
}

// asc_copy_l0c2ub_impl.h
__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l0c2ub_sync_impl(
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
    asc_sync_post_process();
}

// asc_copy_l12l0a_mx_impl.h
__aicore__ inline void asc_copy_l12l0a_mx_sync_impl(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    asc_sync_post_process();
}

// asc_copy_l12l0b_impl.h
__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_sync_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_transpose_sync_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
    asc_sync_post_process();
}

// asc_copy_l12l0b_mx_impl.h
__aicore__ inline void asc_copy_l12l0b_mx_sync_impl(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_mx_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
    asc_sync_post_process();
}

// asc_copy_l12l0b_trans_impl.h
__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12l0b_trans_sync_impl(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
    asc_sync_post_process();
}

// asc_copy_gm2l1_align_impl.h
__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select, l2_cache_mode,
        src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_align_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
    asc_sync_post_process();
}

// asc_copy_gm2l1_dn2nz_impl.h
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_dn2nz_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

// asc_copy_gm2l1_nd2nz_impl.h
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_nd2nz_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_impl(dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
    asc_sync_post_process();
}

// asc_copy_gm2l1_impl.h
__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_impl(dst, src, n_burst, len_burst, pad_func_mode, src_stride, dst_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2l1_sync_impl(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
    asc_sync_post_process();
}

// asc_copy_l12fb_impl.h
__aicore__ inline void asc_copy_l12fb_sync_impl(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_l12bt_impl.h
__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_l12bt_sync_impl(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_l12ub_impl.h
__aicore__ inline void asc_copy_l12ub_sync_impl(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst,
    uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12ub_impl(dst_addr, src_addr, sub_blockid, n_burst, len_burst, src_gap, dst_gap);
    asc_sync_post_process();
}

// asc_fill_l1_impl.h
__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

__aicore__ inline void asc_fill_l1_sync_impl(
    __cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_impl(dst, value, config);
    asc_sync_post_process();
}

// cube_datamove_impl.h
ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_mx and asc_sync)
__aicore__ inline void asc_copy_l12l0a_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_mx_sync_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_mx and asc_sync)
__aicore__ inline void asc_copy_l12l0b_mx_sync(
    uint64_t dst, __cbuf__ fp8_e8m0_t* src, uint16_t x_start_pos, uint16_t y_start_pos, uint8_t x_step, uint8_t y_step,
    uint16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_mx_sync_impl(dst, src, x_start_pos, y_start_pos, x_step, y_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0a_transpose_sync(
    __ca__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0a_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_sync_impl(dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t m_start_position, uint16_t k_start_position, uint8_t m_step,
    uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_transpose and asc_sync)
__aicore__ inline void asc_copy_l12l0b_transpose_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t m_start_position, uint16_t k_start_position,
    uint8_t m_step, uint8_t k_step, int16_t src_stride, uint16_t dst_stride)
{
    asc_copy_l12l0b_transpose_sync_impl(
        dst, src, m_start_position, k_start_position, m_step, k_step, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride, uint16_t dst_gap,
    uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e1m2_t* dst, __cbuf__ fp4x2_e1m2_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ fp4x2_e2m1_t* dst, __cbuf__ fp4x2_e2m1_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b_trans and asc_sync)
__aicore__ inline void asc_copy_l12l0b_trans_sync(
    __cb__ int4b_t* dst, __cbuf__ int4b_t* src, uint16_t index_id, uint8_t repeat, uint16_t src_stride,
    uint16_t dst_gap, uint16_t dst_frac_gap, uint16_t src_frac_gap)
{
    asc_copy_l12l0b_trans_sync_impl(dst, src, index_id, repeat, src_stride, dst_gap, dst_frac_gap, src_frac_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0a and asc_sync)
__aicore__ inline void asc_copy_l12l0a_sync(
    __ca__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0a_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ bfloat16_t* dst, __cbuf__ bfloat16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e4m3fn_t* dst, __cbuf__ fp8_e4m3fn_t* src, uint16_t k_extension, uint16_t m_extension,
    uint16_t k_start_pt, uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ fp8_e5m2_t* dst, __cbuf__ fp8_e5m2_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ half* dst, __cbuf__ half* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ float* dst, __cbuf__ float* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ hifloat8_t* dst, __cbuf__ hifloat8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int32_t* dst, __cbuf__ int32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int8_t* dst, __cbuf__ int8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint32_t* dst, __cbuf__ uint32_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint8_t* dst, __cbuf__ uint8_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ int16_t* dst, __cbuf__ int16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12l0b and asc_sync)
__aicore__ inline void asc_copy_l12l0b_sync(
    __cb__ uint16_t* dst, __cbuf__ uint16_t* src, uint16_t k_extension, uint16_t m_extension, uint16_t k_start_pt,
    uint16_t m_start_pt, uint8_t stride_w, uint8_t stride_h, uint8_t filter_w, uint8_t filter_h,
    uint8_t dilation_filter_w, uint8_t dilation_filter_h, bool filter_size_w, bool filter_size_h, bool transpose,
    bool f_matrix_ctrl, uint16_t channel_size)
{
    asc_copy_l12l0b_sync_impl(
        dst, src, k_extension, m_extension, k_start_pt, m_start_pt, stride_w, stride_h, filter_w, filter_h,
        dilation_filter_w, dilation_filter_h, filter_size_w, filter_size_h, transpose, f_matrix_ctrl, channel_size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_align and asc_sync)
__aicore__ inline void asc_copy_gm2l1_align_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t n_burst, uint32_t len_burst, uint8_t left_padding_count,
    uint8_t right_padding_count, bool data_select_bit, uint8_t l2_cache_ctl, uint64_t burst_src_stride,
    uint32_t burst_dst_stride)
{
    asc_copy_gm2l1_align_sync_impl(
        dst, src, n_burst, len_burst, left_padding_count, right_padding_count, data_select_bit, l2_cache_ctl,
        burst_src_stride, burst_dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_dn2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_dn2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_dn2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl,
    uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ half* dst, __gm__ half* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ float* dst, __gm__ float* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1_nd2nz and asc_sync)
__aicore__ inline void asc_copy_gm2l1_nd2nz_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t loop1_src_stride, uint8_t l2_cache_ctl, uint16_t n_value,
    uint32_t d_value, uint64_t loop4_src_stride, bool smallc0_en)
{
    asc_copy_gm2l1_nd2nz_sync_impl(
        dst, src, loop1_src_stride, l2_cache_ctl, n_value, d_value, loop4_src_stride, smallc0_en);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, uint8_t pad_func_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2l1_sync_impl(dst, src, n_burst, len_burst, pad_func_mode, src_stride, dst_stride);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12fb and asc_sync)
__aicore__ inline void asc_copy_l12fb_sync(__fbuf__ void* dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12fb_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12ub and asc_sync)
__aicore__ inline void asc_copy_l12ub_sync(
    __ubuf__ void* dst_addr, __cbuf__ void* src_addr, bool sub_blockid, uint16_t n_burst, uint16_t len_burst,
    uint16_t src_gap, uint16_t dst_gap)
{
    asc_copy_l12ub_sync_impl(dst_addr, src_addr, sub_blockid, n_burst, len_burst, src_gap, dst_gap);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ void* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ half* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ float* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l12bt and asc_sync)
__aicore__ inline void asc_copy_l12bt_sync(uint64_t dst, __cbuf__ int32_t* src, uint32_t size)
{
    asc_copy_l12bt_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2l1 and asc_sync)
__aicore__ inline void asc_copy_l0c2l1_sync(
    __cbuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2l1_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode, uint64_t quant_pre_mode,
    uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post, uint8_t relu_post,
    bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en, bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2gm and asc_sync)
__aicore__ inline void asc_copy_l0c2gm_sync(
    __gm__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t l2_cache_mode, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2gm_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, l2_cache_mode, enable_clip_relu_pre, unit_flag_mode,
        quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post, clip_relu_post,
        eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ fp8_e4m3fn_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ hifloat8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ float* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ bfloat16_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ half* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride, uint16_t src_stride,
    uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ uint8_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int32_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ float* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_l0c2ub and asc_sync)
__aicore__ inline void asc_copy_l0c2ub_sync(
    __ubuf__ int4b_t* dst, __cc__ int32_t* src, uint16_t n_size, uint16_t m_size, uint32_t dst_stride,
    uint16_t src_stride, uint8_t dual_dst_ctrl, bool sub_blockid, uint8_t enable_clip_relu_pre, uint8_t unit_flag_mode,
    uint64_t quant_pre_mode, uint8_t relu_pre_mode, bool enable_channel_split, bool enable_nz2nd, uint64_t quant_post,
    uint8_t relu_post, bool clip_relu_post, uint8_t eltwise_op, bool eltwise_antq_en, bool c0_pad_en, bool broadcast_en,
    bool enable_nz2dn)
{
    asc_copy_l0c2ub_sync_impl(
        dst, src, n_size, m_size, dst_stride, src_stride, dual_dst_ctrl, sub_blockid, enable_clip_relu_pre,
        unit_flag_mode, quant_pre_mode, relu_pre_mode, enable_channel_split, enable_nz2nd, quant_post, relu_post,
        clip_relu_post, eltwise_op, eltwise_antq_en, c0_pad_en, broadcast_en, enable_nz2dn);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2l1 and asc_sync)
__aicore__ inline void asc_copy_gm2l1_sync(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, uint8_t decomp_mode, uint8_t l2_cache_ctl)
{
    asc_copy_gm2l1_sync_impl(
        dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, decomp_mode, l2_cache_ctl);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ half* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ float* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ int32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ uint32_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, bfloat16_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, half value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_fill_l1 and asc_sync)
__aicore__ inline void asc_fill_l1_sync(__cbuf__ bfloat16_t* dst, uint32_t value, const asc_fill_value_config& config)
{
    asc_fill_l1_sync_impl(dst, value, config);
}

// asc_bitsort_impl.h
__aicore__ inline void asc_bitsort_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
    asc_sync_post_process();
}

__aicore__ inline void asc_bitsort_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_impl(dst, src0, src1, repeat);
    asc_sync_post_process();
}

// asc_mrgsort4_impl.h
__aicore__ inline void asc_mrgsort4_sync_impl(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
    asc_sync_post_process();
}

__aicore__ inline void asc_mrgsort4_sync_impl(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
    asc_sync_post_process();
}

// asc_transpose_impl.h
__aicore__ inline void asc_transpose_sync_impl(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_impl(dst, src);
    asc_sync_post_process();
}

__aicore__ inline void asc_transpose_sync_impl(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_impl(dst, src);
    asc_sync_post_process();
}

// asc_transto5hd_impl.h
__aicore__ inline void asc_transto5hd_b8_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
    asc_sync_post_process();
}

__aicore__ inline void asc_transto5hd_b16_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_impl(dst, src, repeat, dst_stride, src_stride);
    asc_sync_post_process();
}

__aicore__ inline void asc_transto5hd_b32_sync_impl(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_impl(dst, src, repeat, dst_stride, src_stride);
    asc_sync_post_process();
}

// vector_compute_impl.h
ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bitsort and asc_sync)
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ half* dst, __ubuf__ half* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl(dst, src0, src1, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_bitsort and asc_sync)
__aicore__ inline void asc_bitsort_sync(
    __ubuf__ float* dst, __ubuf__ float* src0, __ubuf__ uint32_t* src1, int32_t repeat)
{
    asc_bitsort_sync_impl(dst, src0, src1, repeat);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mrgsort4 and asc_sync)
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ half* dst, __ubuf__ half* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_mrgsort4 and asc_sync)
__aicore__ inline void asc_mrgsort4_sync(
    __ubuf__ float* dst, __ubuf__ float* src[ASC_C_API_MRGSORT_ELEMENT_LEN], uint8_t repeat, uint16_t element_length_0,
    uint16_t element_length_1, uint16_t element_length_2, uint16_t element_length_3, bool if_exhausted_suspension,
    uint8_t valid_bit)
{
    asc_mrgsort4_sync_impl(
        dst, src, repeat, element_length_0, element_length_1, element_length_2, element_length_3,
        if_exhausted_suspension, valid_bit);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_transpose and asc_sync)
__aicore__ inline void asc_transpose_sync(__ubuf__ int16_t* dst, __ubuf__ int16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_transpose and asc_sync)
__aicore__ inline void asc_transpose_sync(__ubuf__ uint16_t* dst, __ubuf__ uint16_t* src)
{
    asc_transpose_sync_impl(dst, src);
}

[[deprecated("NOTICE: asc_transto5hd_b8_sync is deprecated. "
             "Please use asc_transto5hd_b8 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b8_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride, bool dst_high_half,
    bool src_high_half)
{
    asc_transto5hd_b8_sync_impl(dst, src, repeat, dst_stride, src_stride, dst_high_half, src_high_half);
}

[[deprecated("NOTICE: asc_transto5hd_b16_sync is deprecated. "
             "Please use asc_transto5hd_b16 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b16_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b16_sync_impl(dst, src, repeat, dst_stride, src_stride);
}

[[deprecated("NOTICE: asc_transto5hd_b32_sync is deprecated. "
             "Please use asc_transto5hd_b32 and asc_sync instead.")]]
__aicore__ inline void asc_transto5hd_b32_sync(
    ub_addr8_t dst, ub_addr8_t src, uint8_t repeat, uint16_t dst_stride, uint16_t src_stride)
{
    asc_transto5hd_b32_sync_impl(dst, src, repeat, dst_stride, src_stride);
}

// asc_copy_gm2ub_impl.h
__aicore__ inline void asc_copy_gm2ub_sync_impl(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_ub2gm_impl.h
__aicore__ inline void asc_copy_ub2gm_sync_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_gm2ub_align_impl.h
__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_gm2ub_align_sync_impl(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_ub2gm_align_impl.h
__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

__aicore__ inline void asc_copy_ub2gm_align_sync_impl(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_ub2l1_impl.h
__aicore__ inline void asc_copy_ub2l1_sync_impl(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_impl(dst, src, size);
    asc_sync_post_process();
}

// asc_copy_ub2ub_impl.h
__aicore__ inline void asc_copy_ub2ub_sync_impl(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, size);
    asc_sync_post_process();
}

// vector_datamove_impl.h
ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub_align and asc_sync)
__aicore__ inline void asc_copy_gm2ub_align_sync(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm_align and asc_sync)
__aicore__ inline void asc_copy_ub2gm_align_sync(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2l1 and asc_sync)
__aicore__ inline void asc_copy_ub2l1_sync(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_gm2ub and asc_sync)
__aicore__ inline void asc_copy_gm2ub_sync(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2gm and asc_sync)
__aicore__ inline void asc_copy_ub2gm_sync(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_sync_impl(dst, src, size);
}

ASC_DEPRECATED(9.2.0, "2028/09/03", asc_copy_ub2ub and asc_sync)
__aicore__ inline void asc_copy_ub2ub_sync(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_sync_impl(dst, src, size);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_3510_COMPOSITE_INTF_IMPL_H
