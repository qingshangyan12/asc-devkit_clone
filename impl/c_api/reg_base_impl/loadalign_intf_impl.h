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
#warning \
    "impl/c_api/reg_base_impl/loadalign_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_LOADALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_LOADALIGN_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

// offset = 0, norm
__simd_callee__ inline void asc_loadalign_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int64_t& dst, __ubuf__ int64_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint64_t& dst, __ubuf__ uint64_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, NORM);
    }
}

// offset = 0, brc
__simd_callee__ inline void asc_loadalign_brc_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

// offset = 0, upsample
__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, US);
    }
}

// offset = 0, downsample
__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, DS);
    }
}

// offset = 0, unpack
__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

// offset = 0, unpack_v2
__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, UNPK4_B8);
    }
}

// offset = 0, brc_v2
__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

// offset = 0, brc_v3
__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

// offset = 0, deintlv

// int32_t offset, norm
__simd_callee__ inline void asc_loadalign_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int64_t& dst, __ubuf__ int64_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint64_t& dst, __ubuf__ uint64_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, NORM);
    }
}

// int32_t offset, brc
__simd_callee__ inline void asc_loadalign_brc_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

// int32_t offset, upsample
__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, US);
    }
}

// int32_t offset, downsample
__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, DS);
    }
}

// int32_t offset, unpack
__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

// int32_t offset, unpack_v2
__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK4_B8);
    }
}

// int32_t offset, brc_v2
__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

// int32_t offset, brc_v3
__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

// int32_t offset, deintlv

__simd_callee__ inline void asc_loadalign_impl(
    vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(
            reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src),
            (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vsldb(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, (block_stride << 16u | repeat_stride & 0xFFFFU),
            mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_impl(
    vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

// asc_loadalign_brc_elem_impl forwards to asc_loadalign_brc_impl
__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

// asc_loadalign_unpack4_impl forwards to asc_loadalign_unpack_v2_impl
__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_unpack_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

// asc_loadalign_brc_datablock_impl forwards to asc_loadalign_brc_v2_impl
__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v2_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

// asc_loadalign_brc_elem2datablock_impl forwards to asc_loadalign_brc_v3_impl
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_half& dst, __ubuf__ half* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_float& dst, __ubuf__ float* src)
{
    asc_loadalign_brc_v3_impl(dst, src);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_int64_t& dst, __ubuf__ int64_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_uint64_t& dst, __ubuf__ uint64_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_impl(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, NORM);
    }
}

// =========BRC_B8(fp4x2_e2m1/fp4x2_e1m2/u8/s8/f8_e4m3/fp8_e5m2/fp8_e8m0)=========
__simd_callee__ inline void asc_loadalign_brc_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

// =========BRC_B16(u16/s16/bf16/half)=========
__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

// =========BRC_B32(u32/s32/float)=========
__simd_callee__ inline void asc_loadalign_brc_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

// =========US(B8/B16)=========
__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_impl(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, US);
    }
}

// =========DS(B8/B16)=========
__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_impl(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, DS);
    }
}

// DINTLV(B8/B16/B32)

// unpack(B8/B16/B32)
__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

// unpack v2
__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_v2_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

// brc_v2
__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v2_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

/// brc_v3(B16/B32)
__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_v3_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

// asc_loadalign_brc_elem_impl forwards to asc_loadalign_brc_impl
__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_impl(dst, src, offset);
}

// asc_loadalign_unpack4_impl forwards to asc_loadalign_unpack_v2_impl
__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_unpack4_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_unpack_v2_impl(dst, src, offset);
}

// asc_loadalign_brc_datablock_impl forwards to asc_loadalign_brc_v2_impl
__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_datablock_impl(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v2_impl(dst, src, offset);
}

// asc_loadalign_brc_elem2datablock_impl forwards to asc_loadalign_brc_v3_impl
__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_impl(
    vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadalign_brc_v3_impl(dst, src, offset);
}

// postupdate, norm

// postupdate, brc

// postupdate, upsample

// postupdate, downsample
__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate_impl(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, DS, POST_UPDATE);
    }
}

// postupdate, unpack

// postupdate, unpack_v2

// postupdate, brc_v2

// postupdate, brc_v3

// postupdate, deintlv

// asc_loadalign_brc_elem_postupdate_impl forwards to asc_loadalign_brc_postupdate_impl

// asc_loadalign_unpack4_postupdate_impl forwards to asc_loadalign_unpack_postupdate_v2_impl

// asc_loadalign_brc_datablock_postupdate_impl forwards to asc_loadalign_brc_postupdate_v2_impl

// asc_loadalign_brc_elem2datablock_postupdate_impl forwards to asc_loadalign_brc_postupdate_v3_impl

__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, US);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        plds(dst, src, 0, DS);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst0), reinterpret_cast<vector_fp4x2_e1m2_t&>(dst1), src_tmp, 0,
            DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(
            reinterpret_cast<vector_uint8_t&>(dst0), reinterpret_cast<vector_uint8_t&>(dst1),
            reinterpret_cast<__ubuf__ uint8_t*>(src), 0, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_half& dst0, vector_half& dst1, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(vector_float& dst0, vector_float& dst1, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, 0, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, NORM);
    }
}

// =========asc_loadalign(addr_reg)(begin)============
__simd_callee__ inline void asc_loadalign(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_int64_t& dst, __ubuf__ int64_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_uint64_t& dst, __ubuf__ uint64_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, NORM);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

// =========BRC_B16(u16/s16/bf16/half)=========
__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

// =========BRC_B32(u32/s32/float)=========
__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

// =========US(B8/B16)=========
__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, US);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, US);
    }
}

// =========DS(B8/B16)=========
__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        pld(dst, src, offset, DS);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bool& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, DS);
    }
}

// DINTLV(B8/B16/B32)
__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst0), reinterpret_cast<vector_fp4x2_e1m2_t&>(dst1), src_tmp, offset,
            DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst0), reinterpret_cast<vector_uint8_t&>(dst1),
            reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst0, dst1, src, offset, DINTLV_B32);
    }
}

// unpack(B8/B16/B32)
__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK_B32);
    }
}

// =========asc_loadalign(addr_reg)(end)============

__simd_callee__ inline void asc_loadalign_brc(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, US_B8);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_upsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DS_B8);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_downsample(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst0), reinterpret_cast<vector_fp4x2_e1m2_t&>(dst1), src_tmp, offset,
            DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(
            reinterpret_cast<vector_uint8_t&>(dst0), reinterpret_cast<vector_uint8_t&>(dst1),
            reinterpret_cast<__ubuf__ uint8_t*>(src), offset, DINTLV_B8);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_half& dst0, vector_half& dst1, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv(
    vector_float& dst0, vector_float& dst1, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_int64_t& dst, __ubuf__ int64_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(vector_uint64_t& dst, __ubuf__ uint64_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, NORM, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, US_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, US_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_upsample_postupdate(vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, US, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    asc_loadalign_downsample_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, DS_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, DS_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_downsample_postupdate(
    vector_bool& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        plds(dst, src, offset, DS, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, UNPK_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int8_t& dst0, vector_int8_t& dst1, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint8_t& dst0, vector_uint8_t& dst1, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e2m1_t& dst0, vector_fp4x2_e2m1_t& dst1, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp4x2_e1m2_t& dst0, vector_fp4x2_e1m2_t& dst1, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int4x2_t& dst0, vector_int4x2_t& dst1, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst0), reinterpret_cast<vector_fp4x2_e1m2_t&>(dst1), src_tmp, offset,
            DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e8m0_t& dst0, vector_fp8_e8m0_t& dst1, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e5m2_t& dst0, vector_fp8_e5m2_t& dst1, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_fp8_e4m3fn_t& dst0, vector_fp8_e4m3fn_t& dst1, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_hifloat8_t& dst0, vector_hifloat8_t& dst1, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(
            reinterpret_cast<vector_uint8_t&>(dst0), reinterpret_cast<vector_uint8_t&>(dst1), src_tmp, offset,
            DINTLV_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int16_t& dst0, vector_int16_t& dst1, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint16_t& dst0, vector_uint16_t& dst1, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_half& dst0, vector_half& dst1, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_bfloat16_t& dst0, vector_bfloat16_t& dst1, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_int32_t& dst0, vector_int32_t& dst1, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_uint32_t& dst0, vector_uint32_t& dst1, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_deintlv_postupdate(
    vector_float& dst0, vector_float& dst1, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst0, dst1, src, offset, DINTLV_B32, POST_UPDATE);
    }
}

// repeat stride模式读入
__simd_callee__ inline void asc_loadalign(
    vector_int8_t& dst, __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_uint8_t& dst, __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_int16_t& dst, __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_int32_t& dst, __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_int64_t& dst, __ubuf__ int64_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(
            reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src),
            (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_int4x2_t& dst, __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vsldb(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, (block_stride << 16u | repeat_stride & 0xFFFFU),
            mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_half& dst, __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign(
    vector_float& dst, __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int64_t& dst, __ubuf__ int64_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(
            reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*&>(src),
            (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, uint16_t block_stride, uint16_t repeat_stride,
    vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vsldb(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, (block_stride << 16u | repeat_stride & 0xFFFFU), mask,
            POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_half& dst, __ubuf__ half*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_postupdate(
    vector_float& dst, __ubuf__ float*& src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    if ASC_IS_AIV {
        vsldb(dst, src, (block_stride << 16u | repeat_stride & 0xFFFFU), mask, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BRC_B32);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, UNPK4_B8);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int4x2_t& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vld(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint8_t& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int8_t& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(reinterpret_cast<vector_uint8_t&>(dst), reinterpret_cast<__ubuf__ uint8_t*>(src), offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, BLK);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vlds(dst, src, 0, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int16_t& dst, __ubuf__ int16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_int32_t& dst, __ubuf__ int32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint16_t& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int16_t& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_half& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B16);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_uint32_t& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(
    vector_int32_t& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock(vector_float& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vld(dst, src, offset, E2B_B32);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, BRC_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem_postupdate(vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BRC_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_unpack4_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, UNPK4_B8, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int8_t& dst, __ubuf__ int8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint8_t& dst, __ubuf__ uint8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int4x2_t& dst, __ubuf__ int4b_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src);
        vlds(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src_tmp, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_hifloat8_t& dst, __ubuf__ hifloat8_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ uint8_t*& src_tmp = reinterpret_cast<__ubuf__ uint8_t*&>(src);
        vlds(reinterpret_cast<vector_uint8_t&>(dst), src_tmp, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, BLK, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int16_t& dst, __ubuf__ int16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint16_t& dst, __ubuf__ uint16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_half& dst, __ubuf__ half*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_bfloat16_t& dst, __ubuf__ bfloat16_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B16, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_int32_t& dst, __ubuf__ int32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_uint32_t& dst, __ubuf__ uint32_t*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadalign_brc_elem2datablock_postupdate(
    vector_float& dst, __ubuf__ float*& src, int32_t offset)
{
    if ASC_IS_AIV {
        vlds(dst, src, offset, E2B_B32, POST_UPDATE);
    }
}

__simd_callee__ inline vector_int4x2_t asc_loadalign(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign(__ubuf__ float* src)
{
    vector_float dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_datablock(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_brc_datablock(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_brc_datablock(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_datablock(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_datablock(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_datablock(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_datablock(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_datablock(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_brc_datablock(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_brc_datablock(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_brc_datablock(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_datablock(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign_brc_datablock(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign_brc_datablock(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign_brc_datablock(__ubuf__ float* src)
{
    vector_float dst;
    asc_loadalign_brc_datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_brc_elem(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_brc_elem(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_brc_elem(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_brc_elem(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_brc_elem(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_brc_elem(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_brc_elem(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_brc_elem(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_brc_elem(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign_brc_elem(__ubuf__ float* src)
{
    vector_float dst;
    asc_loadalign_brc_elem_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_brc_elem2datablock(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_brc_elem2datablock(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_brc_elem2datablock(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_brc_elem2datablock(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign_brc_elem2datablock(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign_brc_elem2datablock(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign_brc_elem2datablock(__ubuf__ float* src)
{
    vector_float dst;
    asc_loadalign_brc_elem2datablock_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_downsample(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_downsample(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_downsample(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_downsample(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_downsample(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_downsample(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_downsample(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_downsample(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_downsample(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_downsample(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_downsample(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_downsample(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_downsample(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_unpack(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_unpack(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_unpack(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_unpack(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_unpack(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_unpack(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign_unpack(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign_unpack(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign_unpack(__ubuf__ float* src)
{
    vector_float dst;
    asc_loadalign_unpack_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_unpack4(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_unpack4(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_unpack4(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_unpack4(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_unpack4(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_unpack4(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_unpack4(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_unpack4(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_unpack4(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_unpack4_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_upsample(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_upsample(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_upsample(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_upsample(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_upsample(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_upsample(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_upsample(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_upsample(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_upsample(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_upsample(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_upsample(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_upsample(__ubuf__ half* src)
{
    vector_half dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_upsample(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int4x2_t asc_loadalign_datablock_strided(
    __ubuf__ int4b_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_int4x2_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_loadalign_datablock_strided(
    __ubuf__ int8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_int8_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_loadalign_datablock_strided(
    __ubuf__ uint8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_uint8_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e2m1_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_fp4x2_e2m1_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp4x2_e1m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_fp4x2_e1m2_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_loadalign_datablock_strided(
    __ubuf__ hifloat8_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_hifloat8_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e8m0_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_fp8_e8m0_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e5m2_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_fp8_e5m2_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_loadalign_datablock_strided(
    __ubuf__ fp8_e4m3fn_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_fp8_e4m3fn_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_loadalign_datablock_strided(
    __ubuf__ int16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_int16_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_loadalign_datablock_strided(
    __ubuf__ uint16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_uint16_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_loadalign_datablock_strided(
    __ubuf__ half* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_half dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_loadalign_datablock_strided(
    __ubuf__ bfloat16_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_bfloat16_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_loadalign_datablock_strided(
    __ubuf__ int32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_int32_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_loadalign_datablock_strided(
    __ubuf__ uint32_t* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_uint32_t dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_loadalign_datablock_strided(
    __ubuf__ float* src, uint16_t block_stride, uint16_t repeat_stride, vector_bool mask)
{
    vector_float dst;
    asc_loadalign_impl(dst, src, block_stride, repeat_stride, mask);
    return dst;
}

// ========== return mask register APIs ==========
__simd_callee__ inline vector_bool asc_loadalign_mask(__ubuf__ uint32_t* src)
{
    vector_bool dst;
    asc_loadalign_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bool asc_loadalign_mask_downsample(__ubuf__ uint32_t* src)
{
    vector_bool dst;
    asc_loadalign_downsample_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bool asc_loadalign_mask_upsample(__ubuf__ uint32_t* src)
{
    vector_bool dst;
    asc_loadalign_upsample_impl(dst, src);
    return dst;
}

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/c_api/reg_base_impl/npu_arch_3510/loadalign_intf_impl.h"
#endif

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
