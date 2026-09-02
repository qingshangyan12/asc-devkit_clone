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

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_ASC_PREFETCH_L2CACHE_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_9201_CUBE_DATAMOVE_IMPL_ASC_PREFETCH_L2CACHE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_9201/utils_impl.h"

// ==========asc_prefetch_gm2l2cache_impl==========

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ int8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ uint8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ hifloat8_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ fp8_e5m2_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ fp8_e4m3fn_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ int16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ uint16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ half* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ bfloat16_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ int32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ uint32_t* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_impl(
    __gm__ float* src, uint32_t burst_num, uint32_t burst_len, uint64_t burst_src_stride)
{
    prefetch_gm_to_l2cache(src, burst_num, burst_len, burst_src_stride);
}

// ==========asc_prefetch_gm2l2cache_dn2nz_impl==========

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_dn2nz_impl(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_dn2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

// ==========asc_prefetch_gm2l2cache_nd2nz_impl==========

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ int8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ uint8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ hifloat8_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ fp8_e5m2_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ fp8_e4m3fn_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ int16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ uint16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ half* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ bfloat16_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ int32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ uint32_t* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

__aicore__ inline void asc_prefetch_gm2l2cache_nd2nz_impl(
    __gm__ float* src, uint64_t loop1_src_stride, uint16_t n_value, uint32_t d_value, uint64_t loop4_src_stride)
{
    prefetch_gm_to_l2cache_nd2nz(src, loop1_src_stride, n_value, d_value, loop4_src_stride);
}

// ==========asc_prefetch_stop_impl==========

__aicore__ inline void asc_prefetch_stop_impl() { stop_prefetch_l2cache(); }

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
