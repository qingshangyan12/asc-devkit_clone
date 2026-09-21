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

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rd_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rn_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rna_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_ru_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e1m2x2_rz_merge(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e1m2_t asc_bfloat162e1m2x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e1m2_t dst;
    asc_bfloat162e1m2x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rd_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rd_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rd_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rn_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rn_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rna_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rna_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_ru_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_ru_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_ru_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_bfloat162e2m1x2_rz_merge(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp4x2_e2m1_t asc_bfloat162e2m1x2_rz_merge(
    vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp4x2_e2m1_t dst;
    asc_bfloat162e2m1x2_rz_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_float2bfloat16_rd_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rd_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rd_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rd_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rd_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rd_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rd_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rd_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rd_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline void asc_float2bfloat16_rn_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rn_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rn_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rn_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rn_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rn_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rn_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rn_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rn_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline void asc_float2bfloat16_rna_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rna_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rna_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rna_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rna_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rna_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rna_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rna_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rna_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline void asc_float2bfloat16_ru_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_ru_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_ru_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_ru_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_ru_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_ru_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_ru_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_ru_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_ru_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline void asc_float2bfloat16_rz_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rz_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rz_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2bfloat16_rz_sat_merge(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_MERGING);
    }
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rz_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rz_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rz_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline vector_bfloat16_t asc_float2bfloat16_rz_sat_merge(
    vector_float src, vector_bool mask, std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    vector_bfloat16_t dst;
    asc_float2bfloat16_rz_sat_merge(dst, src, mask, dst_pos);
    return dst;
}

__simd_callee__ inline void asc_float2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e4m3_rn_sat_merge(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e4m3fn_t asc_float2e4m3_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e4m3fn_t dst;
    asc_float2e4m3_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_float2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2e5m2_rn_sat_merge(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_fp8_e5m2_t asc_float2e5m2_rn_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_fp8_e5m2_t dst;
    asc_float2e5m2_rn_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_float2hif8_rh_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rh_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rh_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rh_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline void asc_float2hif8_rna_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P0, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P1, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P2, MODE_MERGING);
    }
}

__simd_callee__ inline void asc_float2hif8_rna_sat_merge(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P3, MODE_MERGING);
    }
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

__simd_callee__ inline vector_hifloat8_t asc_float2hif8_rna_sat_merge(
    vector_float src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER>
        dst_quarter_pos)
{
    vector_hifloat8_t dst;
    asc_float2hif8_rna_sat_merge(dst, src, mask, dst_quarter_pos);
    return dst;
}

#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
