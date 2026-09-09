/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/c_api/reg_base_impl/loadunalign_composite_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_LOADUNALIGN_COMPOSITE_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/loadunalign_intf_impl.h"

__simd_callee__ inline void asc_load_impl(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_int64_t& dst, __ubuf__ int64_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load_impl(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_int8_t& dst, __ubuf__ int8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_uint8_t& dst, __ubuf__ uint8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_int16_t& dst, __ubuf__ int16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_uint16_t& dst, __ubuf__ uint16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_int32_t& dst, __ubuf__ int32_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_uint32_t& dst, __ubuf__ uint32_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_half& dst, __ubuf__ half* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_float& dst, __ubuf__ float* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_int64_t& dst, __ubuf__ int64_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_bfloat16_t& dst, __ubuf__ bfloat16_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_fp8_e4m3fn_t& dst, __ubuf__ fp8_e4m3fn_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_hifloat8_t& dst, __ubuf__ hifloat8_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_fp8_e5m2_t& dst, __ubuf__ fp8_e5m2_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_fp8_e8m0_t& dst, __ubuf__ fp8_e8m0_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_fp4x2_e2m1_t& dst, __ubuf__ fp4x2_e2m1_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_fp4x2_e1m2_t& dst, __ubuf__ fp4x2_e1m2_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

__simd_callee__ inline void asc_load(vector_int4x2_t& dst, __ubuf__ int4b_t* src)
{
    if ASC_IS_AIV {
        vector_load_unalign ureg;
        asc_loadunalign_pre_impl(ureg, src);
        asc_loadunalign_impl(dst, ureg, src);
    }
}

// ==========return-value load APIs==========
// ========== return vector data register APIs ==========
__simd_callee__ inline vector_int4x2_t asc_load(__ubuf__ int4b_t* src)
{
    vector_int4x2_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_load(__ubuf__ int8_t* src)
{
    vector_int8_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint8_t asc_load(__ubuf__ uint8_t* src)
{
    vector_uint8_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_load(__ubuf__ fp4x2_e2m1_t* src)
{
    vector_fp4x2_e2m1_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_load(__ubuf__ fp4x2_e1m2_t* src)
{
    vector_fp4x2_e1m2_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_load(__ubuf__ hifloat8_t* src)
{
    vector_hifloat8_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_load(__ubuf__ fp8_e8m0_t* src)
{
    vector_fp8_e8m0_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_load(__ubuf__ fp8_e5m2_t* src)
{
    vector_fp8_e5m2_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_load(__ubuf__ fp8_e4m3fn_t* src)
{
    vector_fp8_e4m3fn_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_load(__ubuf__ int16_t* src)
{
    vector_int16_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_load(__ubuf__ uint16_t* src)
{
    vector_uint16_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_half asc_load(__ubuf__ half* src)
{
    vector_half dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_load(__ubuf__ bfloat16_t* src)
{
    vector_bfloat16_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_load(__ubuf__ int32_t* src)
{
    vector_int32_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_load(__ubuf__ uint32_t* src)
{
    vector_uint32_t dst;
    asc_load_impl(dst, src);
    return dst;
}

__simd_callee__ inline vector_float asc_load(__ubuf__ float* src)
{
    vector_float dst;
    asc_load_impl(dst, src);
    return dst;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
