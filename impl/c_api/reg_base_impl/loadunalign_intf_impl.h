/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/loadunalign_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vldas(dst, src_tmp);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, reinterpret_cast<__ubuf__ uint8_t*>(src));
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int64_t* src)
{
    if ASC_IS_AIV {
        vldas(dst, src);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src);
        vlda(dst, src_tmp, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, reinterpret_cast<__ubuf__ uint8_t*>(src), offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ half* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(
    vector_load_unalign& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ float* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_pre_impl(vector_load_unalign& dst, __ubuf__ int64_t* src, addr_reg offset)
{
    if ASC_IS_AIV {
        vlda(dst, src, offset);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(
            reinterpret_cast<vector_uint8_t&>(dst), src0, reinterpret_cast<__ubuf__ uint8_t*&>(src1), count,
            POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src0, reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(src1),
            count, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t*& src1, uint32_t count)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1, count, POST_UPDATE);
    }
}

// vldu
__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(reinterpret_cast<vector_uint8_t&>(dst), src0, offset, reinterpret_cast<__ubuf__ uint8_t*>(src1), inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(
            reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src0, offset,
            reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src1), inc);
    }
}

__simd_callee__ inline void asc_loadunalign_postupdate_impl(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1, addr_reg& offset, uint32_t inc)
{
    if ASC_IS_AIV {
        vldu(dst, src0, offset, src1, inc);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1)
{
    if ASC_IS_AIV {
        vldus(reinterpret_cast<vector_uint8_t&>(dst), src0, reinterpret_cast<__ubuf__ uint8_t*>(src1));
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1)
{
    if ASC_IS_AIV {
        vldus(dst, src0, src1);
    }
}

__simd_callee__ inline void asc_loadunalign_impl(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* src1_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(src1);
        vldus(reinterpret_cast<vector_fp4x2_e1m2_t&>(dst), src0, src1_tmp);
    }
}

// ==========asc_loadunalign_pre(int8_t/uint8_t/fp4x2_e2m1_t/fp4x2_e1m2_t/fp8_e5m2_t/fp8_e4m3fn_t/int16_t/uint16_t/half/bfloat16_t/int32_t/uint32_t/float/int64_t)==========
__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int8_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint8_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int4b_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ hifloat8_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int16_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint16_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ half* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ bfloat16_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int32_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint32_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ float* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int64_t* src)
{
    asc_loadunalign_pre_impl(dst, src);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int8_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint8_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e2m1_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp4x2_e1m2_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int4b_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e8m0_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e5m2_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ fp8_e4m3fn_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ hifloat8_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int16_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint16_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ half* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ bfloat16_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int32_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ uint32_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ float* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

__simd_callee__ inline void asc_loadunalign_pre(vector_load_unalign& dst, __ubuf__ int64_t* src, addr_reg offset)
{
    asc_loadunalign_pre_impl(dst, src, offset);
}

// ==========asc_loadunalign(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e2m1/e1m2)=========
__simd_callee__ inline void asc_loadunalign(vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_loadunalign(vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1)
{
    asc_loadunalign_impl(dst, src0, src1);
}

// ==========asc_loadunalign_postupdate(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e2m1/e1m2/e8m0)=========
__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t*& src1, uint32_t count)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int8_t& dst, vector_load_unalign& src0, __ubuf__ int8_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint8_t& dst, vector_load_unalign& src0, __ubuf__ uint8_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int16_t& dst, vector_load_unalign& src0, __ubuf__ int16_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint16_t& dst, vector_load_unalign& src0, __ubuf__ uint16_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int32_t& dst, vector_load_unalign& src0, __ubuf__ int32_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_uint32_t& dst, vector_load_unalign& src0, __ubuf__ uint32_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_half& dst, vector_load_unalign& src0, __ubuf__ half* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_float& dst, vector_load_unalign& src0, __ubuf__ float* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int64_t& dst, vector_load_unalign& src0, __ubuf__ int64_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_bfloat16_t& dst, vector_load_unalign& src0, __ubuf__ bfloat16_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e4m3fn_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e4m3fn_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_hifloat8_t& dst, vector_load_unalign& src0, __ubuf__ hifloat8_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e5m2_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e5m2_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e2m1_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e2m1_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp4x2_e1m2_t& dst, vector_load_unalign& src0, __ubuf__ fp4x2_e1m2_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_int4x2_t& dst, vector_load_unalign& src0, __ubuf__ int4b_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

__simd_callee__ inline void asc_loadunalign_postupdate(
    vector_fp8_e8m0_t& dst, vector_load_unalign& src0, __ubuf__ fp8_e8m0_t* src1, addr_reg& offset, uint32_t inc)
{
    asc_loadunalign_postupdate_impl(dst, src0, src1, offset, inc);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_INTF_IMPL_H
