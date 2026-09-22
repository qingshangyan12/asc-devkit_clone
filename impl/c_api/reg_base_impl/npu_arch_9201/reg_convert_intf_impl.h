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

__simd_callee__ inline void asc_float2half_rd_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rd_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rd_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rd_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rn_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rn_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rn_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rn_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rna_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rna_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rna_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rna_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ro_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ro_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ro_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ro_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ru_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ru_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ru_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_ru_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rz_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rz_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rz_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2half_rz_sat_merge(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

// ================asc_rcpe6m22bfloat16================
__simd_callee__ inline void asc_rcpe6m22bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e6m2_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_rcpe6m22bf16(dst, src, mask, PART_EVEN);
    }
}

__simd_callee__ inline void asc_rcpe6m22bfloat16(
    vector_bfloat16_t& dst, vector_fp8_e6m2_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_rcpe6m22bf16(dst, src, mask, PART_ODD);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_rcpe6m22bfloat16(
    vector_fp8_e6m2_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_rcpe6m22bfloat16(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_rcpe6m22bfloat16(
    vector_fp8_e6m2_t src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_rcpe6m22bfloat16(dst, src, mask, dst_pos);
    return dst;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
