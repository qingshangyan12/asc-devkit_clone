/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/reg_reduce_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"

__simd_callee__ inline void asc_reduce_max_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_max_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_max_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_max_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_max_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_max_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_min_datablock_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmin(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_pair_reduce_sum_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_pair_reduce_sum_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcpadd(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock half
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock float
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock uint16_t
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock int16_t
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock uint32_t
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

// asc_reduce_max_datablock int32_t
__simd_callee__ inline void asc_reduce_max_datablock_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgmax(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_reduce_sum_datablock_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcgadd(dst, src, mask, MODE_ZEROING);
    }
}

// ==========asc_pair_reduce_sum(half/float)==========
__simd_callee__ inline void asc_pair_reduce_sum(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_pair_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_pair_reduce_sum(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_pair_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline vector_half asc_pair_reduce_sum(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_pair_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_pair_reduce_sum(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_pair_reduce_sum_impl(dst, src, mask);
    return dst;
}

// ==========asc_reduce_max(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_max(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_max_impl(dst, src, mask);
}

__simd_callee__ inline vector_int16_t asc_reduce_max(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_max(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_max(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_max(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_max(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_max(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_max_impl(dst, src, mask);
    return dst;
}

// ==========asc_reduce_min(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_min_impl(dst, src, mask);
}

__simd_callee__ inline vector_int16_t asc_reduce_min(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_min(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_min(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_min(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_min(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_min(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_min_impl(dst, src, mask);
    return dst;
}

// ==========asc_reduce_min_datablock(uint16_t/int16_t/half/uint32_t/int32_t/float)==========
__simd_callee__ inline void asc_reduce_min_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_min_datablock(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_min_datablock_impl(dst, src, mask);
}

__simd_callee__ inline vector_int16_t asc_reduce_min_datablock(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_min_datablock(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_min_datablock(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_min_datablock(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_min_datablock(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_min_datablock(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_min_datablock_impl(dst, src, mask);
    return dst;
}

//==================asc_reduce_max_datablock====================
__simd_callee__ inline void asc_reduce_max_datablock(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max_datablock(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max_datablock(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max_datablock(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_max_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_max_datablock_impl(dst, src, mask);
}

__simd_callee__ inline vector_int16_t asc_reduce_max_datablock(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_max_datablock(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_reduce_max_datablock(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_max_datablock(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_max_datablock(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_max_datablock(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_max_datablock_impl(dst, src, mask);
    return dst;
}

// ==========asc_reduce_sum(int16_t/int32_t/uint16_t/uint32_t/half/float)==========
__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_sum_impl(dst, src, mask);
}

__simd_callee__ inline vector_int32_t asc_reduce_sum(vector_int16_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_sum(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_sum(vector_uint16_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_sum(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_reduce_sum(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_sum(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_sum_impl(dst, src, mask);
    return dst;
}

// ==========asc_reduce_sum_datablock(half/float/uint16_t/int16_t/uint32_t/int32_t)==========
__simd_callee__ inline void asc_reduce_sum_datablock(vector_half& dst, vector_half src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum_datablock(vector_float& dst, vector_float src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint16_t src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int16_t src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum_datablock(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline void asc_reduce_sum_datablock(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    asc_reduce_sum_datablock_impl(dst, src, mask);
}

__simd_callee__ inline vector_half asc_reduce_sum_datablock(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_reduce_sum_datablock(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_sum_datablock(vector_uint16_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_sum_datablock(vector_int16_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_reduce_sum_datablock(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_reduce_sum_datablock(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_reduce_sum_datablock_impl(dst, src, mask);
    return dst;
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_REG_REDUCE_INTF_IMPL_H
