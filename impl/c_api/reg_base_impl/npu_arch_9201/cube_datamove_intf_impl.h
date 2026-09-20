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

#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/npu_arch_9201/utils_impl.h"

constexpr uint8_t ASC_GM2L1_SID_DEFAULT = 0;
constexpr uint8_t ASC_GM2L1_DECOMP_MODE_DEFAULT = 0;

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t n_burst, uint32_t len_burst, asc_channel_pad_mode pad_mode,
    bool enable_pre_load_ignore_sync, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_v2(
            dst, src, 0, n_burst, len_burst, static_cast<uint8_t>(pad_mode), 0, src_stride, dst_stride,
            enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ float* dst, __gm__ float* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ half* dst, __gm__ half* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ void* dst, __gm__ void* src, uint32_t m_start_position, uint32_t k_start_position, uint16_t dst_stride,
    uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync, asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_copy_gm2l1(
    __cbuf__ hif4_scale* dst, __gm__ hif4_scale* src, uint32_t m_start_position, uint32_t k_start_position,
    uint16_t dst_stride, uint16_t m_step, uint16_t k_step, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIC {
        load_gm_to_cbuf_2dv2_s4(
            dst, src, m_start_position, k_start_position, dst_stride, m_step, k_step, ASC_GM2L1_SID_DEFAULT,
            ASC_GM2L1_DECOMP_MODE_DEFAULT, enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode));
    }
}
__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_count,
    uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2l1_align(
    __cbuf__ hif4_scale* dst, __gm__ hif4_scale* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_count, uint8_t right_padding_count, bool enable_data_select, bool enable_pre_load_ignore_sync,
    asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_count, right_padding_count, enable_data_select,
            enable_pre_load_ignore_sync, static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}
__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_dn2nz(
    __cbuf__ hif4_scale* dst, __gm__ hif4_scale* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_dn2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}
__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ half* dst, __gm__ half* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode, uint16_t n_value,
    uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0, bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ float* dst, __gm__ float* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int16_t* dst, __gm__ int16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int32_t* dst, __gm__ int32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ int8_t* dst, __gm__ int8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint16_t* dst, __gm__ uint16_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint32_t* dst, __gm__ uint32_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ uint8_t* dst, __gm__ uint8_t* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

__aicore__ inline void asc_copy_gm2l1_nd2nz(
    __cbuf__ hif4_scale* dst, __gm__ hif4_scale* src, uint64_t src_d_value, asc_load_l2_cache_mode l2_cache_mode,
    uint16_t n_value, uint32_t d_value, uint64_t src_nd_matrix_stride, bool enable_small_c0,
    bool enable_pre_load_ignore_sync)
{
    if ASC_IS_AIC {
        copy_gm_to_cbuf_multi_nd2nz(
            dst, src, 0, src_d_value, static_cast<uint8_t>(l2_cache_mode), n_value, d_value, src_nd_matrix_stride,
            enable_small_c0, enable_pre_load_ignore_sync);
    }
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
