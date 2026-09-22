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
    "impl/c_api/reg_base_impl/npu_arch_9201/reg_convert_intf_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "c_api/asc_simd.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_CONVERT_INTF_IMPL_H
#define IMPL_C_API_REG_BASE_IMPL_NPU_ARCH_9201_REG_CONVERT_INTF_IMPL_H

#include "impl/c_api/reg_base_impl/utils_impl.h"

__simd_callee__ inline void asc_half2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_sat(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_half2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_half2e4m3_rn(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_half2e4m3_rn_sat(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_half2e4m3_rn(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e4m3fn_t asc_half2e4m3_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_half2e4m3_rn_sat(dst, src, mask, pos);
    return dst;
}

__simd_callee__ inline void asc_half2e4m3_rn_v2(vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e4m3_rn_sat_v2(vector_fp8_e4m3fn_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_half2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_sat(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_fp8_e5m2_t dst;
    asc_half2e5m2_rn(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_fp8_e5m2_t dst;
    asc_half2e5m2_rn_sat(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_fp8_e5m2_t dst;
    asc_half2e5m2_rn(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_fp8_e5m2_t asc_half2e5m2_rn_sat(
    vector_half src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_fp8_e5m2_t dst;
    asc_half2e5m2_rn_sat(dst, src, mask, pos);
    return dst;
}

__simd_callee__ inline void asc_half2e5m2_rn_v2(vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}
__simd_callee__ inline void asc_half2e5m2_rn_sat_v2(vector_fp8_e5m2_t& dst, vector_half src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_e6m22bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e6m2_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt_e6m22bf16(dst, src, mask, PART_EVEN);
    }
}
__simd_callee__ inline void asc_e6m22bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e6m2_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt_e6m22bf16(dst, src, mask, PART_ODD);
    }
}
__simd_callee__ inline vector_bfloat16_t asc_e6m22bfloat16(
    vector_fp8_e6m2_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_bfloat16_t dst;
    asc_e6m22bfloat16(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_bfloat16_t asc_e6m22bfloat16(
    vector_fp8_e6m2_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_bfloat16_t dst;
    asc_e6m22bfloat16(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline void asc_e6m22bfloat16_v2(vector_bfloat16_t& dst, vector_fp8_e6m2_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_e6m22bf16(dst, src, mask, PART_ODD);
    }
}

__simd_callee__ inline void asc_e8m02bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN>)
{
    if ASC_IS_AIV {
        vcvt_e8m02bf16(dst, src, mask, PART_EVEN);
    }
}
__simd_callee__ inline void asc_e8m02bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD>)
{
    if ASC_IS_AIV {
        vcvt_e8m02bf16(dst, src, mask, PART_ODD);
    }
}
__simd_callee__ inline vector_bfloat16_t asc_e8m02bfloat16(
    vector_fp8_e8m0_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> pos)
{
    vector_bfloat16_t dst;
    asc_e8m02bfloat16(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline vector_bfloat16_t asc_e8m02bfloat16(
    vector_fp8_e8m0_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> pos)
{
    vector_bfloat16_t dst;
    asc_e8m02bfloat16(dst, src, mask, pos);
    return dst;
}
__simd_callee__ inline void asc_e8m02bfloat16_v2(vector_bfloat16_t& dst, vector_fp8_e8m0_t src, vector_bool mask)
{
    if ASC_IS_AIV {
        vcvt_e8m02bf16(dst, src, mask, PART_ODD);
    }
}

#endif
