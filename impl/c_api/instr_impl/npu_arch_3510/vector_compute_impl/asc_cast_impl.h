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
#define ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CAST_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_ASC_CAST_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_rint_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_round_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_floor_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_ceil_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_trunc_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_rint_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_round_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_floor_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_ceil_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_trunc_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_rint_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_round_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_floor_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_ceil_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_trunc_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_int162half_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_int162half_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_int162half_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_int162half_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_int162half_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_int4x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_int4x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_int4x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_int4x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_int4x22half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_int4x22half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_int4x22half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_int4x22half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_int4x22int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_int4x22int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_int4x22int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_int4x22int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e1m2x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_bfloat162e2m1x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_half2int4x2_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_e4m32float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_e4m32float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_e4m32float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_e4m32float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_e1m2x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_e2m1x22bfloat16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_bfloat162float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_bfloat162float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_e5m22float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_e5m22float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_e5m22float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_e5m22float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_hif82float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_hif82float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_hif82float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_hif82float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int162float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int162float_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_hif82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_hif82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_uint82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_uint82half_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int82int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int82int16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int162int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int162int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_int82int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_int82int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_int82int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_int82int32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int322int64_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int322int64_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_uint82uint16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_uint82uint16_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int162uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int162uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_uint162uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_uint162uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    asc_uint82uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    asc_uint82uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    asc_uint82uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    asc_uint82uint32_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2bfloat16_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2bfloat16_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_half2bfloat16_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_half2bfloat16_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_half2bfloat16_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_half2bfloat16_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_half2bfloat16_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    asc_int322float_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    asc_int322float_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    asc_int322float_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    asc_int322float_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    asc_int322float_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int642float_rn_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int642float_rn_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int642float_rna_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int642float_rna_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int642float_rd_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int642float_rd_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int642float_ru_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int642float_ru_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_int642float_rz_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_int642float_rz_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2e4m3_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2e5m2_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_bfloat162half_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_bfloat162half_rn_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_bfloat162half_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_bfloat162half_rna_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_bfloat162half_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_bfloat162half_rd_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_bfloat162half_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_bfloat162half_ru_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_bfloat162half_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_bfloat162half_rz_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_ro_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_ro_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2half_ro_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2half_ro_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_float2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2hif8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2hif8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2hif8_rh_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2hif8_rh_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int16_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int16_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_half2int16_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_half2int16_rn_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_half2int16_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_half2int16_rna_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_half2int16_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_half2int16_rd_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_half2int16_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_half2int16_ru_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_half2int16_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_half2int16_rz_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_bfloat162int32_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_bfloat162int32_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_float2int32_rn_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_float2int32_rn_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_float2int32_rna_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_float2int32_rna_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_float2int32_rd_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_float2int32_rd_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_float2int32_ru_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_float2int32_ru_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    asc_float2int32_rz_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    asc_float2int32_rz_sat_impl(dst, src, mask);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2int32_rn_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2int32_rn_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2int32_rna_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2int32_rna_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2int32_rd_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2int32_rd_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2int32_ru_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2int32_ru_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    asc_half2int32_rz_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    asc_half2int32_rz_impl(dst, src, mask, src_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_float2int64_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_float2int64_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2int8_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2int8_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rn_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rn_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rna_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rna_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rd_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rd_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_ru_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_ru_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rz_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_half2uint8_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_half2uint8_rz_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int322int16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int322int16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int322int16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int322int16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint322int16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint322int16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint322int16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint322int16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int642int32_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int642int32_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int642int32_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int642int32_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int322uint16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int322uint16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int322uint16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int322uint16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint322uint16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint322uint16_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint322uint16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint322uint16_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int162uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int162uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_int162uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_int162uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_int322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_int322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_int322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_int322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_int322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_int322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_int322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_int322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint162uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint162uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    asc_uint162uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    asc_uint162uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_uint322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_uint322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_uint322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_uint322uint8_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    asc_uint322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    asc_uint322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    asc_uint322uint8_sat_impl(dst, src, mask, dst_pos);
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    asc_uint322uint8_sat_impl(dst, src, mask, dst_pos);
}
#endif

#if defined(UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_C_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
