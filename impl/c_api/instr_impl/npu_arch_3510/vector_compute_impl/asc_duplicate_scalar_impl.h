/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file asc_duplicate_scalar_impl.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_SCALAR_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_SCALAR_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

// vdup
__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint8_t& dst, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int8_t& dst, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e5m2_t& dst, fp8_e5m2_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e8m0_t& dst, fp8_e8m0_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint16_t& dst, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int16_t& dst, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_half& dst, half value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_bfloat16_t& dst, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint32_t& dst, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int32_t& dst, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_float& dst, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_duplicate_scalar_impl(uint8_t value, vector_bool mask)
{
    vector_uint8_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_duplicate_scalar_impl(int8_t value, vector_bool mask)
{
    vector_int8_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate_scalar_impl(fp8_e4m3fn_t value, vector_bool mask)
{
    vector_fp8_e4m3fn_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate_scalar_impl(fp8_e5m2_t value, vector_bool mask)
{
    vector_fp8_e5m2_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate_scalar_impl(fp8_e8m0_t value, vector_bool mask)
{
    vector_fp8_e8m0_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_duplicate_scalar_impl(uint16_t value, vector_bool mask)
{
    vector_uint16_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_duplicate_scalar_impl(int16_t value, vector_bool mask)
{
    vector_int16_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_duplicate_scalar_impl(half value, vector_bool mask)
{
    vector_half dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_duplicate_scalar_impl(bfloat16_t value, vector_bool mask)
{
    vector_bfloat16_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_duplicate_scalar_impl(uint32_t value, vector_bool mask)
{
    vector_uint32_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_duplicate_scalar_impl(int32_t value, vector_bool mask)
{
    vector_int32_t dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_duplicate_scalar_impl(float value, vector_bool mask)
{
    vector_float dst;
    asc_duplicate_scalar_impl(dst, value, mask);
    return dst;
}

// vdup with merging mode
__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_uint8_t& dst, uint8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_int8_t& dst, int8_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(
    vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_fp8_e5m2_t& dst, fp8_e5m2_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_fp8_e8m0_t& dst, fp8_e8m0_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_uint16_t& dst, uint16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_int16_t& dst, int16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_half& dst, half value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_bfloat16_t& dst, bfloat16_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_uint32_t& dst, uint32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_int32_t& dst, int32_t value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_merge_impl(vector_float& dst, float value, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, value, mask, MODE_MERGING);
    }
}

// vbr
__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint8_t& dst, uint8_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int8_t& dst, int8_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint16_t& dst, uint16_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int16_t& dst, int16_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_uint32_t& dst, uint32_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_int32_t& dst, int32_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_half& dst, half value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_float& dst, float value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_bfloat16_t& dst, bfloat16_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e4m3fn_t& dst, fp8_e4m3fn_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e5m2_t& dst, fp8_e5m2_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline void asc_duplicate_scalar_impl(vector_fp8_e8m0_t& dst, fp8_e8m0_t value)
{
    if ASC_IS_AIV {
        vbr(dst, value);
    }
}

__simd_callee__ inline vector_uint8_t asc_duplicate_scalar_impl(uint8_t value)
{
    vector_uint8_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_duplicate_scalar_impl(int8_t value)
{
    vector_int8_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_duplicate_scalar_impl(uint16_t value)
{
    vector_uint16_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_duplicate_scalar_impl(int16_t value)
{
    vector_int16_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_duplicate_scalar_impl(uint32_t value)
{
    vector_uint32_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_duplicate_scalar_impl(int32_t value)
{
    vector_int32_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_half asc_duplicate_scalar_impl(half value)
{
    vector_half dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_float asc_duplicate_scalar_impl(float value)
{
    vector_float dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_duplicate_scalar_impl(bfloat16_t value)
{
    vector_bfloat16_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate_scalar_impl(fp8_e4m3fn_t value)
{
    vector_fp8_e4m3fn_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate_scalar_impl(fp8_e5m2_t value)
{
    vector_fp8_e5m2_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate_scalar_impl(fp8_e8m0_t value)
{
    vector_fp8_e8m0_t dst;
    asc_duplicate_scalar_impl(dst, value);
    return dst;
}

#endif
