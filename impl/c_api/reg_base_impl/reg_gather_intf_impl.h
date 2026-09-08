/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/reg_gather_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/common_impl.h"

// ==========源操作数和目的数都为寄存器=========
__simd_callee__ inline void asc_gather_impl(vector_int8_t& dst, vector_int8_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_uint8_t& dst, vector_uint8_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_int16_t& dst, vector_int16_t src, vector_uint16_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_uint16_t& dst, vector_uint16_t src, vector_uint16_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_int32_t& dst, vector_int32_t src, vector_uint32_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_uint32_t& dst, vector_uint32_t src, vector_uint32_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_float& dst, vector_float src, vector_uint32_t index)
{
    if ASC_IS_AIV {
        vselr(reinterpret_cast<vector_uint32_t&>(dst), *reinterpret_cast<vector_uint32_t*>(&src), index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_half& dst, vector_half src, vector_uint16_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_uint16_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(reinterpret_cast<vector_uint8_t&>(dst), *reinterpret_cast<vector_uint8_t*>(&src), index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

__simd_callee__ inline void asc_gather_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_uint8_t index)
{
    if ASC_IS_AIV {
        vselr(dst, src, index);
    }
}

// ==========源操作数和目的数都为寄存器=========
// ==========asc_gather(u8/s8/half/u16/s16/float/u32/s32/bf16/e4m3/e5m2/e8m0)=========

__simd_callee__ inline vector_int8_t asc_gather(vector_int8_t src, vector_uint8_t index)
{
    vector_int8_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_gather(vector_uint8_t src, vector_uint8_t index)
{
    vector_uint8_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_gather(vector_int16_t src, vector_uint16_t index)
{
    vector_int16_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_gather(vector_uint16_t src, vector_uint16_t index)
{
    vector_uint16_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_gather(vector_int32_t src, vector_uint32_t index)
{
    vector_int32_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_gather(vector_uint32_t src, vector_uint32_t index)
{
    vector_uint32_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_float asc_gather(vector_float src, vector_uint32_t index)
{
    vector_float dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_half asc_gather(vector_half src, vector_uint16_t index)
{
    vector_half dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_gather(vector_bfloat16_t src, vector_uint16_t index)
{
    vector_bfloat16_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_gather(vector_fp8_e4m3fn_t src, vector_uint8_t index)
{
    vector_fp8_e4m3fn_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_gather(vector_hifloat8_t src, vector_uint8_t index)
{
    vector_hifloat8_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_gather(vector_fp8_e5m2_t src, vector_uint8_t index)
{
    vector_fp8_e5m2_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_gather(vector_fp8_e8m0_t src, vector_uint8_t index)
{
    vector_fp8_e8m0_t dst;
    asc_gather_impl(dst, src, index);
    return dst;
}

__simd_callee__ inline void asc_gather(vector_int8_t& dst, vector_int8_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_uint8_t& dst, vector_uint8_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_int16_t& dst, vector_int16_t src, vector_uint16_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_uint16_t& dst, vector_uint16_t src, vector_uint16_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_int32_t& dst, vector_int32_t src, vector_uint32_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_uint32_t& dst, vector_uint32_t src, vector_uint32_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_float& dst, vector_float src, vector_uint32_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_half& dst, vector_half src, vector_uint16_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_uint16_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_hifloat8_t& dst, vector_hifloat8_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

__simd_callee__ inline void asc_gather(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_uint8_t index)
{
    asc_gather_impl(dst, src, index);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_REG_GATHER_INTF_IMPL_H
