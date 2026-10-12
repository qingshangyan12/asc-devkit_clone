#ifndef IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_VCONV_ASC_CAST_IMPL_H
#define IMPL_C_API_INSTR_IMPL_NPU_ARCH_3510_VECTOR_COMPUTE_IMPL_VCONV_ASC_CAST_IMPL_H

#include "impl/c_api/instr_impl/npu_arch_3510/utils_impl.h"

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42bf16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42f16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int4x2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt_s42s16(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e1m2_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp4x2_e2m1_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_A, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_F, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_C, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int4x2_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt_f162s4(dst, src, mask, ROUND_Z, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e4m3fn_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e1m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_fp4x2_e2m1_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_fp8_e5m2_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_hifloat8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint32_t& dst, vector_uint8_t src, vector_bool mask,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_bfloat16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_float& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e4m3fn_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_fp8_e5m2_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_A, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, ROUND_A, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_F, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, ROUND_F, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, ROUND_C, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, ROUND_Z, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, ROUND_Z, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_half& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RO> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_O, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_hifloat8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RH> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_H, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_bfloat16_t src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> src_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int64_t& dst, vector_float src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RN> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_R, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RNA> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_A, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RD> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_F, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RU> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_C, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_half src, vector_bool mask,
    std::integral_constant<asc_cast_round_mode, asc_cast_round_mode::RZ> round_mode,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, ROUND_Z, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_int32_t& dst, vector_int64_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint16_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_int32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::EVEN> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_EVEN, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint16_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_mode, asc_position_mode::ODD> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_ODD, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::NO_SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_DISABLE, PART_P3, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FIRST_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P0, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_SECOND_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P1, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_THIRD_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P2, MODE_ZEROING);
    }
}

__simd_callee__ inline void asc_cast_impl(
    vector_uint8_t& dst, vector_uint32_t src, vector_bool mask,
    std::integral_constant<asc_cast_saturation_mode, asc_cast_saturation_mode::SAT> saturation_mode,
    std::integral_constant<asc_position_quarter_mode, asc_position_quarter_mode::DISPERSE_FOURTH_QUARTER> dst_pos)
{
    if ASC_IS_AIV {
        vcvt(dst, src, mask, RS_ENABLE, PART_P3, MODE_ZEROING);
    }
}

#endif
