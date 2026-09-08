/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/storeunalign_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, *reinterpret_cast<vector_uint8_t*>(&src1), reinterpret_cast<__ubuf__ uint8_t*&>(dst));
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_impl(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(dst);
        vstus(src0, count, *reinterpret_cast<vector_fp4x2_e1m2_t*>(&src1), dst_tmp);
    }
}

// ==========偏移固定传入0，由用户自行更新目的操作数的地址=========
__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int8_t*& dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int16_t*& dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int32_t*& dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int64_t*& dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ half*& dst, vector_store_unalign& src0, vector_half src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ float*& dst, vector_store_unalign& src0, vector_float src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(
            src0, count, *reinterpret_cast<vector_uint8_t*>(&src1), reinterpret_cast<__ubuf__ uint8_t*&>(dst),
            POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        vstus(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int4b_t*& dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(dst);
        vstus(src0, count, *reinterpret_cast<vector_fp4x2_e1m2_t*>(&src1), dst_tmp, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1)
{
    if ASC_IS_AIV {
        vstur(src0, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(dst);
        vstur(src0, *reinterpret_cast<vector_fp4x2_e1m2_t*>(&src1), dst_tmp, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    if ASC_IS_AIV {
        pstu(src0, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    if ASC_IS_AIV {
        pstu(src0, src1, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    if ASC_IS_AIV {
        __ubuf__ uint32_t*& dst_tmp = reinterpret_cast<__ubuf__ uint32_t*&>(dst);
        pstu(src0, src1, dst_tmp);
    }
}

// ==========通过地址寄存器传入偏移，用户可以选择更新偏移或者更新目的操作数的地址=========
__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(
            src0, count, *reinterpret_cast<vector_uint8_t*>(&src1), reinterpret_cast<__ubuf__ uint8_t*>(dst),
            POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        vstu(src0, count, src1, dst, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_postupdate_impl(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, addr_reg& count)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(dst);
        vstu(src0, count, *reinterpret_cast<vector_fp4x2_e1m2_t*>(&src1), dst_tmp, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int8_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint8_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int16_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint16_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int32_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int64_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint32_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ half* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ float* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ bfloat16_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ hifloat8_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, reinterpret_cast<__ubuf__ uint8_t*>(dst), offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int4b_t* dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(dst);
        vstas(src, dst_tmp, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int8_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint8_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int16_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint16_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int32_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int64_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint32_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ half* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ float* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ bfloat16_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        vstar(src, dst);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int4b_t* dst, vector_store_unalign src)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(dst);
        vstar(src, dst_tmp);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int8_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ uint8_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int16_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ uint16_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int32_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int64_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ uint32_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ half* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ float* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ bfloat16_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ hifloat8_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, reinterpret_cast<__ubuf__ uint8_t*>(dst), offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        vsta(src, dst, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_impl(__ubuf__ int4b_t* dst, vector_store_unalign src, addr_reg offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t* dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*>(dst);
        vsta(src, dst_tmp, offset);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ int8_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ uint8_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ int16_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ uint16_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ int32_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ int64_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ uint32_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ half*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ float*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, reinterpret_cast<__ubuf__ uint8_t*&>(dst), offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        vstas(src, dst, offset, POST_UPDATE);
    }
}

__simd_callee__ inline void asc_storeunalign_post_postupdate_impl(
    __ubuf__ int4b_t*& dst, vector_store_unalign src, int32_t offset)
{
    if ASC_IS_AIV {
        __ubuf__ float4_e1m2x2_t*& dst_tmp = reinterpret_cast<__ubuf__ float4_e1m2x2_t*&>(dst);
        vstas(src, dst_tmp, offset, POST_UPDATE);
    }
}

// ==========asc_storeunalign(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e1m2/e2m1)=========
__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
{
    asc_storeunalign_impl(dst, src0, src1, count);
}

// ==========偏移固定传入0，由用户自行更新目的操作数的地址=========
// ==========asc_storeunalign_postupdate(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e1m2/e2m1)=========
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign& src0, vector_int8_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign& src0, vector_uint8_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign& src0, vector_int16_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_uint16_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign& src0, vector_int32_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_uint32_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign& src0, vector_int64_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half*& dst, vector_store_unalign& src0, vector_half src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float*& dst, vector_store_unalign& src0, vector_float src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign& src0, vector_bfloat16_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign& src0, vector_hifloat8_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign& src0, vector_int4x2_t src1, uint32_t count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign& src0, vector_bool src1)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1);
}

// ==========通过地址寄存器传入偏移，用户可以选择更新偏移或者更新目的操作数的地址=========
// ==========asc_storeunalign_postupdate(u8/s8/half/u16/s16/float/u32/s32/s64/bf16/e4m3/e5m2/e8m0/e1m2/e2m1)=========
__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int8_t* dst, vector_store_unalign& src0, vector_int8_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint8_t* dst, vector_store_unalign& src0, vector_uint8_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int16_t* dst, vector_store_unalign& src0, vector_int16_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint16_t* dst, vector_store_unalign& src0, vector_uint16_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int32_t* dst, vector_store_unalign& src0, vector_int32_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ uint32_t* dst, vector_store_unalign& src0, vector_uint32_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int64_t* dst, vector_store_unalign& src0, vector_int64_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ half* dst, vector_store_unalign& src0, vector_half src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ float* dst, vector_store_unalign& src0, vector_float src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ bfloat16_t* dst, vector_store_unalign& src0, vector_bfloat16_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign& src0, vector_fp8_e4m3fn_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ hifloat8_t* dst, vector_store_unalign& src0, vector_hifloat8_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e5m2_t* dst, vector_store_unalign& src0, vector_fp8_e5m2_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp8_e8m0_t* dst, vector_store_unalign& src0, vector_fp8_e8m0_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign& src0, vector_fp4x2_e2m1_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign& src0, vector_fp4x2_e1m2_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

__simd_callee__ inline void asc_storeunalign_postupdate(
    __ubuf__ int4b_t* dst, vector_store_unalign& src0, vector_int4x2_t src1, addr_reg& count)
{
    asc_storeunalign_postupdate_impl(dst, src0, src1, count);
}

//===========asc_storeunalign_post(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float/int64)===========
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

//===========asc_storeunalign_post(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float/int64)===========
__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int8_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint8_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e2m1_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp4x2_e1m2_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int4b_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e8m0_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e5m2_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ fp8_e4m3fn_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ hifloat8_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int16_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint16_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ half* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ bfloat16_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int32_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ uint32_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ float* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post(__ubuf__ int64_t* dst, vector_store_unalign src, addr_reg offset)
{
    asc_storeunalign_post_impl(dst, src, offset);
}

//===========asc_storeunalign_post_postupdate(int8/uint8/e2m1/e1m2/e8m0/e5m2/e4m3fn/int16/uint16/half/bfloat16/int32/uint32/float/int64)===========
__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int8_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint8_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e2m1_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp4x2_e1m2_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int4b_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e8m0_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e5m2_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ fp8_e4m3fn_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ hifloat8_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int16_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint16_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ half*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ bfloat16_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int32_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ uint32_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ float*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

__simd_callee__ inline void asc_storeunalign_post_postupdate(
    __ubuf__ int64_t*& dst, vector_store_unalign src, int32_t offset)
{
    asc_storeunalign_post_postupdate_impl(dst, src, offset);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_INTF_IMPL_H
