/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H

#if !defined(ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#pragma message( \
    "impl/c_api/reg_base_impl/storeunalign_composite_intf_impl.h is an internal header. Please include c_api/asc_simd.h instead.")
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H
#endif

#include "c_api/defs/defs.h"
#include "impl/c_api/reg_base_impl/utils_impl.h"
#include "impl/c_api/reg_base_impl/storeunalign_intf_impl.h"
#include "impl/c_api/reg_base_impl/sys_var_intf_impl.h"

__simd_callee__ inline void asc_store_impl(__ubuf__ int8_t* dst, vector_int8_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(int8_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint8_t* dst, vector_uint8_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(uint8_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int16_t* dst, vector_int16_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(int16_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint16_t* dst, vector_uint16_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(uint16_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int32_t* dst, vector_int32_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(int32_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint32_t* dst, vector_uint32_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(uint32_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ half* dst, vector_half src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(half);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ float* dst, vector_float src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(float);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int64_t* dst, vector_int64_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(int64_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(bfloat16_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(fp8_e4m3fn_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(hifloat8_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(fp8_e5m2_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(fp8_e8m0_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(fp4x2_e2m1_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(fp4x2_e1m2_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int4b_t* dst, vector_int4x2_t src)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        constexpr uint32_t count = asc_get_vf_len() / sizeof(int4b_t);
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

// asc_store(with count)
__simd_callee__ inline void asc_store_impl(__ubuf__ int8_t* dst, vector_int8_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint8_t* dst, vector_uint8_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int16_t* dst, vector_int16_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint16_t* dst, vector_uint16_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int32_t* dst, vector_int32_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ uint32_t* dst, vector_uint32_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ half* dst, vector_half src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ float* dst, vector_float src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int64_t* dst, vector_int64_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

__simd_callee__ inline void asc_store_impl(__ubuf__ int4b_t* dst, vector_int4x2_t src, uint32_t count)
{
    if ASC_IS_AIV {
        vector_store_unalign ureg;
        asc_storeunalign_postupdate_impl(dst, ureg, src, count);
        asc_storeunalign_post_impl(dst, ureg, 0);
    }
}

//===========asc_store==================
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst, vector_int8_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ uint8_t* dst, vector_uint8_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ int16_t* dst, vector_int16_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ uint16_t* dst, vector_uint16_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ int32_t* dst, vector_int32_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ uint32_t* dst, vector_uint32_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ half* dst, vector_half src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ float* dst, vector_float src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ int64_t* dst, vector_int64_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src) { asc_store_impl(dst, src); }

__simd_callee__ inline void asc_store(__ubuf__ int4b_t* dst, vector_int4x2_t src) { asc_store_impl(dst, src); }

// ==========asc_store(with count)============
__simd_callee__ inline void asc_store(__ubuf__ int8_t* dst, vector_int8_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ uint8_t* dst, vector_uint8_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ int16_t* dst, vector_int16_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ uint16_t* dst, vector_uint16_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ int32_t* dst, vector_int32_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ uint32_t* dst, vector_uint32_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ half* dst, vector_half src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ float* dst, vector_float src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ int64_t* dst, vector_int64_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ bfloat16_t* dst, vector_bfloat16_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ fp8_e8m0_t* dst, vector_fp8_e8m0_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ fp8_e4m3fn_t* dst, vector_fp8_e4m3fn_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ hifloat8_t* dst, vector_hifloat8_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ fp8_e5m2_t* dst, vector_fp8_e5m2_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e2m1_t* dst, vector_fp4x2_e2m1_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ fp4x2_e1m2_t* dst, vector_fp4x2_e1m2_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

__simd_callee__ inline void asc_store(__ubuf__ int4b_t* dst, vector_int4x2_t src, uint32_t count)
{
    asc_store_impl(dst, src, count);
}

#if defined(ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef ASCENDC_C_API_INTERNAL_IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H
#endif

#endif // IMPL_C_API_REG_BASE_IMPL_STOREUNALIGN_COMPOSITE_INTF_IMPL_H
