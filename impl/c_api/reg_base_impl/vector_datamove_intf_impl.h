/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message("impl/c_api/reg_base_impl/vector_datamove_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_intf_impl.h"

// asc_set_ub2gm_loop_size_impl.h
__aicore__ inline void asc_set_ub2gm_loop_size_impl(uint32_t loop1_size, uint32_t loop2_size)
{
    if ASC_IS_AIV {
        uint64_t config = static_cast<uint64_t>(loop2_size) << 21;
        config |= static_cast<uint64_t>(loop1_size);
        set_loop_size_ubtoout(config);
    }
}

// asc_set_ub2gm_loop1_stride_impl.h
__aicore__ inline void asc_set_ub2gm_loop1_stride_impl(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    if ASC_IS_AIV {
        uint64_t config = loop1_src_stride << 40;
        config |= loop1_dst_stride;
        set_loop1_stride_ubtoout(config);
    }
}

// asc_set_ub2gm_loop2_stride_impl.h
__aicore__ inline void asc_set_ub2gm_loop2_stride_impl(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    if ASC_IS_AIV {
        uint64_t config = loop2_src_stride << 40;
        config |= loop2_dst_stride;
        set_loop2_stride_ubtoout(config);
    }
}

// asc_set_gm2ub_loop_size_impl.h
__aicore__ inline void asc_set_gm2ub_loop_size_impl(uint64_t loop1_size, uint64_t loop2_size)
{
    if ASC_IS_AIV {
        asc_gm2ub_loop_size_config config;
        config.loop1_size = loop1_size;
        config.loop2_size = loop2_size;
        set_loop_size_outtoub(config.config);
    }
}

// asc_set_gm2ub_loop1_stride_impl.h
__aicore__ inline void asc_set_gm2ub_loop1_stride_impl(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    if ASC_IS_AIV {
        asc_gm2ub_loop_stride_config config;
        config.src_stride = loop1_src_stride;
        config.dst_stride = loop1_dst_stride;
        set_loop1_stride_outtoub(config.config);
    }
}

// asc_set_gm2ub_loop2_stride_impl.h
__aicore__ inline void asc_set_gm2ub_loop2_stride_impl(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    if ASC_IS_AIV {
        asc_gm2ub_loop_stride_config config;
        config.src_stride = loop2_src_stride;
        config.dst_stride = loop2_dst_stride;
        set_loop2_stride_outtoub(config.config);
    }
}

// asc_set_ndim_loop_stride_impl.h
__aicore__ inline void asc_set_ndim_loop0_stride_impl(uint64_t dst_stride, uint64_t src_stride)
{
    if ASC_IS_AIV {
        uint64_t config = (src_stride << 20) | (dst_stride & 0xfffff);
        set_loop0_stride_nddma(config);
    }
}

__aicore__ inline void asc_set_ndim_loop1_stride_impl(uint64_t dst_stride, uint64_t src_stride)
{
    if ASC_IS_AIV {
        uint64_t config = (src_stride << 20) | (dst_stride & 0xfffff);
        set_loop1_stride_nddma(config);
    }
}

__aicore__ inline void asc_set_ndim_loop2_stride_impl(uint64_t dst_stride, uint64_t src_stride)
{
    if ASC_IS_AIV {
        uint64_t config = (src_stride << 20) | (dst_stride & 0xfffff);
        set_loop2_stride_nddma(config);
    }
}

__aicore__ inline void asc_set_ndim_loop3_stride_impl(uint64_t dst_stride, uint64_t src_stride)
{
    if ASC_IS_AIV {
        uint64_t config = (src_stride << 20) | (dst_stride & 0xfffff);
        set_loop3_stride_nddma(config);
    }
}

__aicore__ inline void asc_set_ndim_loop4_stride_impl(uint64_t dst_stride, uint64_t src_stride)
{
    if ASC_IS_AIV {
        uint64_t config = (src_stride << 20) | (dst_stride & 0xfffff);
        set_loop4_stride_nddma(config);
    }
}

// asc_ndim_copy_gm2ub_impl.h
//===========asc_ndim_copy_gm2ub(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, static_cast<uint8_t>(l2_cache_mode));
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b8(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b16(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

__aicore__ inline void asc_ndim_copy_gm2ub_impl(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    uint8_t cache_mode)
{
    if ASC_IS_AIV {
        nddma_out_to_ub_b32(
            (__ubuf__ void*)dst, (__gm__ void*)src, 0, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size,
            loop0_lp_count, loop0_rp_count, padding_mode, cache_mode);
    }
}

// asc_set_ndim_pad_count_impl.h
__aicore__ inline void asc_set_ndim_pad_count_impl(asc_ndim_pad_count_config& config)
{
    if ASC_IS_AIV {
        set_pad_cnt_nddma(config.config);
    }
}

// asc_set_ndim_pad_value_impl.h
//===========asc_set_ndim_pad_value(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_ndim_pad_value_impl(int8_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(uint8_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(int16_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(uint16_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(half pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(bfloat16_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(int32_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(uint32_t pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_ndim_pad_value_impl(float pad_value)
{
    if ASC_IS_AIV {
        return set_pad_val_nddma(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

// asc_set_copy_pad_val_impl.h
//===========asc_set_copy_pad_val(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_copy_pad_val_impl(int8_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint8_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint8_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(int16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(half pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(bfloat16_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint16_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(int32_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(uint32_t pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

__aicore__ inline void asc_set_copy_pad_val_impl(float pad_value)
{
    if ASC_IS_AIV {
        set_mov_pad_val(*reinterpret_cast<uint32_t*>(&pad_value));
    }
}

// asc_copy_gm2ub_impl.h
__aicore__ inline void asc_copy_gm2ub_impl(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride,
    uint16_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            (__ubuf__ uint8_t*)dst, (__gm__ uint8_t*)src, 0, burst_count, burst_len, 0, 0, false, 0, src_stride,
            dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_impl(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, 1, size, 0, 0);
}

// asc_copy_ub2gm_impl.h
__aicore__ inline void asc_copy_ub2gm_impl(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t dst_stride,
    uint16_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(dst, src, 0, burst_count, burst_len, 0, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_impl(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, 1, size, 0, 0);
}

// asc_copy_gm2ub_align_impl.h
#ifndef ASC_C_API_DEFAULT_ZERO_VALUE
#define ASC_C_API_DEFAULT_ZERO_VALUE 0
#endif

#ifndef ASC_C_API_DEFAULT_NBURST
#define ASC_C_API_DEFAULT_NBURST 1
#endif

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            static_cast<uint8_t>(l2_cache_mode), src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, uint8_t l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    if ASC_IS_AIV {
        copy_gm_to_ubuf_align_v2(
            dst, src, 0, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad,
            l2_cache_mode, src_stride, dst_stride);
    }
}

__aicore__ inline void asc_copy_gm2ub_align_impl(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, false,
        ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE);
}

#undef ASC_C_API_DEFAULT_ZERO_VALUE

#undef ASC_C_API_DEFAULT_NBURST

// asc_copy_ub2gm_align_impl.h
#ifndef ASC_C_API_DEFAULT_ZERO_VALUE
#define ASC_C_API_DEFAULT_ZERO_VALUE 0
#endif

#ifndef ASC_C_API_DEFAULT_NBURST
#define ASC_C_API_DEFAULT_NBURST 1
#endif

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, static_cast<uint8_t>(l2_cache_mode),
            dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t l2_cache_mode,
    uint64_t dst_stride, uint32_t src_stride)
{
    if ASC_IS_AIV {
        copy_ubuf_to_gm_align_v2(
            (__gm__ void*)dst, (__ubuf__ void*)src, 0, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
    }
}

__aicore__ inline void asc_copy_ub2gm_align_impl(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(
        dst, src, ASC_C_API_DEFAULT_NBURST, size, ASC_C_API_DEFAULT_ZERO_VALUE, ASC_C_API_DEFAULT_ZERO_VALUE,
        ASC_C_API_DEFAULT_ZERO_VALUE);
}

#undef ASC_C_API_DEFAULT_ZERO_VALUE

#undef ASC_C_API_DEFAULT_NBURST

// asc_copy_ub2l1_impl.h
__aicore__ inline void asc_copy_ub2l1_impl(
    __cbuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_cbuf(dst, src, 0, burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2l1_impl(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_copy_ub2ub_impl.h
__aicore__ inline void asc_copy_ub2ub_impl(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    if ASC_IS_AIV {
        copy_ubuf_to_ubuf(dst, src, 0, burst_count, burst_len, src_gap, dst_gap);
    }
}

__aicore__ inline void asc_copy_ub2ub_impl(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, 1, size / ASC_C_API_ONE_DATABLOCK_SIZE, 0, 0);
}

// asc_ndim_copy_dci_impl.h
__aicore__ inline void asc_ndim_copy_dci_impl()
{
    if ASC_IS_AIV {
        nd_dma_dci();
    }
}

// vector_datamove_impl.h
//===========asc_set_ub2gm_loop_size=============
__aicore__ inline void asc_set_ub2gm_loop_size(uint32_t loop1_size, uint32_t loop2_size)
{
    asc_set_ub2gm_loop_size_impl(loop1_size, loop2_size);
}

//===========asc_set_ub2gm_loop1_stride===========
__aicore__ inline void asc_set_ub2gm_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    asc_set_ub2gm_loop1_stride_impl(loop1_src_stride, loop1_dst_stride);
}

//===========asc_set_ub2gm_loop2_stride===========
__aicore__ inline void asc_set_ub2gm_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    asc_set_ub2gm_loop2_stride_impl(loop2_src_stride, loop2_dst_stride);
}

__aicore__ inline void asc_set_gm2ub_loop_size(uint64_t loop1_size, uint64_t loop2_size)
{
    asc_set_gm2ub_loop_size_impl(loop1_size, loop2_size);
}

__aicore__ inline void asc_set_gm2ub_loop1_stride(uint64_t loop1_src_stride, uint64_t loop1_dst_stride)
{
    asc_set_gm2ub_loop1_stride_impl(loop1_src_stride, loop1_dst_stride);
}

__aicore__ inline void asc_set_gm2ub_loop2_stride(uint64_t loop2_src_stride, uint64_t loop2_dst_stride)
{
    asc_set_gm2ub_loop2_stride_impl(loop2_src_stride, loop2_dst_stride);
}

__aicore__ inline void asc_set_ndim_loop0_stride(uint64_t dst_stride, uint64_t src_stride)
{
    asc_set_ndim_loop0_stride_impl(dst_stride, src_stride);
}

__aicore__ inline void asc_set_ndim_loop1_stride(uint64_t dst_stride, uint64_t src_stride)
{
    asc_set_ndim_loop1_stride_impl(dst_stride, src_stride);
}

__aicore__ inline void asc_set_ndim_loop2_stride(uint64_t dst_stride, uint64_t src_stride)
{
    asc_set_ndim_loop2_stride_impl(dst_stride, src_stride);
}

__aicore__ inline void asc_set_ndim_loop3_stride(uint64_t dst_stride, uint64_t src_stride)
{
    asc_set_ndim_loop3_stride_impl(dst_stride, src_stride);
}

__aicore__ inline void asc_set_ndim_loop4_stride(uint64_t dst_stride, uint64_t src_stride)
{
    asc_set_ndim_loop4_stride_impl(dst_stride, src_stride);
}

//===========asc_set_ndim_pad_count===========
__aicore__ inline void asc_set_ndim_pad_count(asc_ndim_pad_count_config& config)
{
    asc_set_ndim_pad_count_impl(config);
}

//===========asc_set_ndim_pad_value(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_ndim_pad_value(int8_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(uint8_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(int16_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(uint16_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(half pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(bfloat16_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(int32_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(uint32_t pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

__aicore__ inline void asc_set_ndim_pad_value(float pad_value) { return asc_set_ndim_pad_value_impl(pad_value); }

//===========asc_ndim_copy_gm2ub(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e2m1_t* dst, __gm__ fp4x2_e2m1_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp4x2_e1m2_t* dst, __gm__ fp4x2_e1m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e8m0_t* dst, __gm__ fp8_e8m0_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ half* dst, __gm__ half* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

__aicore__ inline void asc_ndim_copy_gm2ub(
    __ubuf__ float* dst, __gm__ float* src, uint32_t loop0_size, uint32_t loop1_size, uint32_t loop2_size,
    uint32_t loop3_size, uint32_t loop4_size, uint8_t loop0_lp_count, uint8_t loop0_rp_count, bool padding_mode,
    asc_load_l2_cache_mode l2_cache_mode)
{
    asc_ndim_copy_gm2ub_impl(
        dst, src, loop0_size, loop1_size, loop2_size, loop3_size, loop4_size, loop0_lp_count, loop0_rp_count,
        padding_mode, l2_cache_mode);
}

// ==========asc_copy_gm2ub_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ half* dst, __gm__ half* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ float* dst, __gm__ float* src, uint32_t burst_count, uint32_t burst_len, uint8_t left_padding_num,
    uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode, uint64_t src_stride,
    uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(
    __ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    uint8_t left_padding_num, uint8_t right_padding_num, bool enable_constant_pad, asc_load_l2_cache_mode l2_cache_mode,
    uint64_t src_stride, uint32_t dst_stride)
{
    asc_copy_gm2ub_align_impl(
        dst, src, burst_count, burst_len, left_padding_num, right_padding_num, enable_constant_pad, l2_cache_mode,
        src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint8_t* dst, __gm__ uint8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int8_t* dst, __gm__ int8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint16_t* dst, __gm__ uint16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int16_t* dst, __gm__ int16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ uint32_t* dst, __gm__ uint32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ int32_t* dst, __gm__ int32_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ half* dst, __gm__ half* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ float* dst, __gm__ float* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ hifloat8_t* dst, __gm__ hifloat8_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ bfloat16_t* dst, __gm__ bfloat16_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e5m2_t* dst, __gm__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_gm2ub_align(__ubuf__ fp8_e4m3fn_t* dst, __gm__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_gm2ub_align_impl(dst, src, size);
}

// ==========asc_copy_ub2gm_align(int8_t/uint8_t/fp8_e5m2_t/fp8_e4m3fn_t/hifloat8_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float)==========
__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ half* dst, __ubuf__ half* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ float* dst, __ubuf__ float* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(
    __gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t burst_count, uint32_t burst_len,
    asc_store_l2_cache_mode l2_cache_mode, uint64_t dst_stride, uint32_t src_stride)
{
    asc_copy_ub2gm_align_impl(dst, src, burst_count, burst_len, l2_cache_mode, dst_stride, src_stride);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint8_t* dst, __ubuf__ uint8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int8_t* dst, __ubuf__ int8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint16_t* dst, __ubuf__ uint16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int16_t* dst, __ubuf__ int16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ uint32_t* dst, __ubuf__ uint32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ int32_t* dst, __ubuf__ int32_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ half* dst, __ubuf__ half* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ float* dst, __ubuf__ float* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ hifloat8_t* dst, __ubuf__ hifloat8_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ bfloat16_t* dst, __ubuf__ bfloat16_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e5m2_t* dst, __ubuf__ fp8_e5m2_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm_align(__gm__ fp8_e4m3fn_t* dst, __ubuf__ fp8_e4m3fn_t* src, uint32_t size)
{
    asc_copy_ub2gm_align_impl(dst, src, size);
}

//=============asc_ndim_copy_dci===============
__aicore__ inline void asc_ndim_copy_dci() { asc_ndim_copy_dci_impl(); }

//=============asc_copy_ub2l1===============
__aicore__ inline void asc_copy_ub2l1(__cbuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2l1_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2l1(
    __cbuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_ub2l1_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

//=============asc_copy_gm2ub===============
__aicore__ inline void asc_copy_gm2ub(
    __ubuf__ void* dst, __gm__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_stride,
    uint16_t dst_stride)
{
    asc_copy_gm2ub_impl(dst, src, burst_count, burst_len, src_stride, dst_stride);
}

__aicore__ inline void asc_copy_gm2ub(__ubuf__ void* dst, __gm__ void* src, uint32_t size)
{
    asc_copy_gm2ub_impl(dst, src, size);
}

//=============asc_copy_ub2gm===============
__aicore__ inline void asc_copy_ub2gm(__gm__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2gm_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2gm(
    __gm__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t dst_stride,
    uint16_t src_stride)
{
    asc_copy_ub2gm_impl(dst, src, burst_count, burst_len, dst_stride, src_stride);
}

//=============asc_copy_ub2ub===============
__aicore__ inline void asc_copy_ub2ub(__ubuf__ void* dst, __ubuf__ void* src, uint32_t size)
{
    asc_copy_ub2ub_impl(dst, src, size);
}

__aicore__ inline void asc_copy_ub2ub(
    __ubuf__ void* dst, __ubuf__ void* src, uint16_t burst_count, uint16_t burst_len, uint16_t src_gap,
    uint16_t dst_gap)
{
    asc_copy_ub2ub_impl(dst, src, burst_count, burst_len, src_gap, dst_gap);
}

//===========asc_set_copy_pad_val(int8/uint8/int16/uint16/half/bfloat16/int32/uint32/float)===========
__aicore__ inline void asc_set_copy_pad_val(int8_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint8_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(int16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(half pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(bfloat16_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(int32_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(uint32_t pad_value) { asc_set_copy_pad_val_impl(pad_value); }

__aicore__ inline void asc_set_copy_pad_val(float pad_value) { asc_set_copy_pad_val_impl(pad_value); }

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_VECTOR_DATAMOVE_INTF_IMPL_H
