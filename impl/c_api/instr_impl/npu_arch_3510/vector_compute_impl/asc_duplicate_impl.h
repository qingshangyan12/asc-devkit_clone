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
 * \file asc_duplicate_impl.h.h
 * \brief
 */

#ifndef IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_IMPL_H
#define IMPL_CAPI_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_DUPLICATE_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_duplicate_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_ZEROING);
    }
}

__simd_callee__ inline vector_uint8_t asc_duplicate_impl(vector_uint8_t src, vector_bool mask)
{
    vector_uint8_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int8_t asc_duplicate_impl(vector_int8_t src, vector_bool mask)
{
    vector_int8_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_duplicate_impl(vector_fp8_e4m3fn_t src, vector_bool mask)
{
    vector_fp8_e4m3fn_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_duplicate_impl(vector_fp8_e5m2_t src, vector_bool mask)
{
    vector_fp8_e5m2_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_fp8_e8m0_t asc_duplicate_impl(vector_fp8_e8m0_t src, vector_bool mask)
{
    vector_fp8_e8m0_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint16_t asc_duplicate_impl(vector_uint16_t src, vector_bool mask)
{
    vector_uint16_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int16_t asc_duplicate_impl(vector_int16_t src, vector_bool mask)
{
    vector_int16_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_half asc_duplicate_impl(vector_half src, vector_bool mask)
{
    vector_half dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_duplicate_impl(vector_bfloat16_t src, vector_bool mask)
{
    vector_bfloat16_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_uint32_t asc_duplicate_impl(vector_uint32_t src, vector_bool mask)
{
    vector_uint32_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_int32_t asc_duplicate_impl(vector_int32_t src, vector_bool mask)
{
    vector_int32_t dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline vector_float asc_duplicate_impl(vector_float src, vector_bool mask)
{
    vector_float dst;
    asc_duplicate_impl(dst, src, mask);
    return dst;
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_ZEROING);
    }
}

// vdup with merging mode
__simd_callee__ inline void asc_duplicate_merge_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_merge_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_LOWEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_uint8_t& dst, vector_uint8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_int8_t& dst, vector_int8_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_fp8_e4m3fn_t& dst, vector_fp8_e4m3fn_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_fp8_e5m2_t& dst, vector_fp8_e5m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_fp8_e8m0_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_uint16_t& dst, vector_uint16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_int16_t& dst, vector_int16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_half& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(
    vector_uint32_t& dst, vector_uint32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_int32_t& dst, vector_int32_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_duplicate_highest_merge_impl(vector_float& dst, vector_float src, vector_bool mask)
{
    if ASC_IS_AIV {
        vdup(dst, src, mask, POS_HIGHEST, MODE_MERGING);
    }
}
#endif
