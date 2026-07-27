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
 * \file power_3510_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/power/power_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/power.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_POWER_POWER_C310_IMPL_H__
#endif
#ifndef IMPL_MATH_POWER_POWER_C310_IMPL_H
#define IMPL_MATH_POWER_POWER_C310_IMPL_H
#include "../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../../../../include/basic_api/kernel_tensor.h"
#include "kernel_tiling/kernel_tiling.h"
#include "power_common_utils.h"
#include "../../common/check.h"

namespace AscendC {
namespace PowerC310Impl {

constexpr Reg::CastTrait castTraitF16F32 = {Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING};
constexpr Reg::CastTrait castTraitF32F16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};
constexpr Reg::CastTrait castTraitF32I32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_ROUND};
constexpr Reg::CastTrait castTraitI32F32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_TRUNC};
constexpr Reg::CastTrait castTraitI8I16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_ROUND};
constexpr Reg::CastTrait castTraitI16I8 = {
    Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_ROUND};

namespace PowF {
constexpr float LOG2_LOWEST_VALUE = 1.175494351e-38f;
constexpr float LOG2_LOWEST_VALUE_MULS = 8388608.0f;

constexpr float LOG2_REDUCE_COEFF1 = 0.70710678f;
constexpr int32_t LOG2_REDUCE_COEFF2 = 0xff800000;
constexpr float LOG2_REDUCE_FMAF_COEFF1 = 1.19209290e-7f;

constexpr float LOG2_BEST_FMAF_COEFF1 = 0.37282535f;
constexpr float LOG2_BEST_FMAF_COEFF2 = 0.4097556f;
constexpr float LOG2_BEST_FMAF_COEFF3 = 0.57711965f;
constexpr float LOG2_BEST_FMAF_COEFF4 = 0.96179646f;
constexpr float LOG2_BEST_FMAF_COEFF5 = 2.88539f;
constexpr float LOG2_BEST_FMAF_COEFF6 = 0.00000003851926f;

constexpr float EXPF_BEST_FMAF_COEFF1 = 0.000152392517f;
constexpr float EXPF_BEST_FMAF_COEFF2 = 0.00133913534f;
constexpr float EXPF_BEST_FMAF_COEFF3 = 0.00961883925f;
constexpr float EXPF_BEST_FMAF_COEFF4 = 0.0555035882f;
constexpr float EXPF_BEST_FMAF_COEFF5 = 0.240226448f;
constexpr float EXPF_BEST_FMAF_COEFF6 = 0.693147182f;

constexpr int32_t EXPF_INTERVAL_CMP = -2097152000;
constexpr int32_t EXPF_INTERVAL_CAST = 2130706432;

constexpr float EXP_OVFL_UNFL_F = 152.0f;
constexpr int32_t INF = 0x7f800000;
constexpr int32_t NEG_INF = 0xff800000;
constexpr int32_t I32_NAN = 0x7f7fffff;
constexpr int32_t F32_NAN = 0x7fc00000;

constexpr int32_t R10_COEFF = 0x7F800000;
constexpr int32_t R12_COEFF = 0x7FFFFFFF;

constexpr int16_t COMPARE_ZERO_OFFSET = 31;
constexpr int16_t SHIFT_OFFSET = 23;
constexpr float F32_FRACTIONS = -23.0f;

__simd_callee__ inline void IsInfNum(Reg::MaskReg& infMask, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::MaskReg tmpMask;
    Reg::CompareScalar<int32_t, CMPMODE::EQ>(tmpMask, (Reg::RegTensor<int32_t>&)srcReg, INF, mask);
    Reg::CompareScalar<int32_t, CMPMODE::EQ>(infMask, (Reg::RegTensor<int32_t>&)srcReg, NEG_INF, mask);
    Reg::MaskOr(infMask, infMask, tmpMask, mask);
}

__simd_callee__ inline void IsNanNum(Reg::MaskReg& nanMask, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::Compare<float, CMPMODE::NE>(nanMask, srcReg, srcReg, mask);
}

__simd_callee__ inline void RFloor(Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::Truncate<float, RoundMode::CAST_FLOOR, Reg::MaskMergeMode::ZEROING>(dstReg, srcReg, mask);
}

__aicore__ inline void CompareNegZero(Reg::MaskReg& filterMask, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<uint32_t> tmpReg;
    Reg::ShiftRights(tmpReg, (Reg::RegTensor<uint32_t>&)srcReg, COMPARE_ZERO_OFFSET, mask);
    Reg::CompareScalar<uint32_t, CMPMODE::EQ>(filterMask, tmpReg, 0.0f, mask);
}

__aicore__ inline void CopySignF(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg1, Reg::RegTensor<float>& srcReg2, Reg::MaskReg& mask,
    Reg::MaskReg& cmpMask1, Reg::MaskReg& cmpMask2)
{
    Reg::RegTensor<float> tmpFloatReg, tmpFloatReg2;
    Reg::CompareScalar<float, CMPMODE::GE>(cmpMask1, srcReg2, 0.0f, mask);
    CompareNegZero(cmpMask2, srcReg2, mask);
    Reg::MaskAnd(cmpMask1, cmpMask1, cmpMask2, mask);
    Reg::Abs(tmpFloatReg, srcReg1, mask);
    Reg::Neg(tmpFloatReg2, tmpFloatReg, mask);
    Reg::Select(dstReg, tmpFloatReg, tmpFloatReg2, cmpMask1);
}

__simd_callee__ inline void FMaf(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg1, Reg::RegTensor<float>& srcReg2,
    Reg::RegTensor<float>& srcReg3, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> tmpReg = srcReg1;
    Reg::FusedMulDstAdd(tmpReg, srcReg2, srcReg3, mask);
    dstReg = tmpReg;
}

__simd_callee__ inline void FMaf(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg1, float scalarValue, Reg::RegTensor<float>& srcReg2,
    Reg::MaskReg& mask)
{
    Reg::RegTensor<float> tmpReg;
    Reg::Duplicate(tmpReg, scalarValue, mask);
    FMaf(dstReg, srcReg1, tmpReg, srcReg2, mask);
}

__simd_callee__ inline void GetLogFExt(
    Reg::RegTensor<float>& logHigh, Reg::RegTensor<float>& logLow, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> tmpIReg, tmpAReg;
    Reg::RegTensor<int32_t> tmpEReg, tmpIntReg;
    Reg::RegTensor<float> tmpFloatReg, tmpFloatReg2;
    Reg::Duplicate(tmpIReg, 0.0f, mask);
    /* init varaiable a and i:
     *  if (a < 1.175494351e-38f){ // 0x1.0p-126
     *      a = a * 8388608.0f; // 0x1.0p+23
     *      i = -23.0f;
     *   }
     */
    Reg::MaskReg cmpMask;
    Reg::CompareScalar<float, CMPMODE::LT>(cmpMask, srcReg, LOG2_LOWEST_VALUE, mask);
    Reg::Muls(tmpAReg, srcReg, LOG2_LOWEST_VALUE_MULS, mask);
    Reg::Duplicate(tmpFloatReg, F32_FRACTIONS, mask);
    Reg::Select(tmpIReg, tmpFloatReg, tmpIReg, cmpMask);
    // step 1: e = (__float_as_int (a) - __float_as_int (0.70710678f)) & 0xff800000;
    Reg::Duplicate(tmpFloatReg, LOG2_REDUCE_COEFF1, mask);
    Reg::Sub(tmpEReg, (Reg::RegTensor<int32_t>&)srcReg, (Reg::RegTensor<int32_t>&)tmpFloatReg, mask);
    Reg::Duplicate(tmpIntReg, LOG2_REDUCE_COEFF2, mask);
    Reg::And(tmpEReg, tmpEReg, tmpIntReg, mask);
    // step 2: m = __int_as_float (__float_as_int (a) - e);
    Reg::RegTensor<float> tmpMReg;
    Reg::Sub((Reg::RegTensor<int32_t>&)tmpMReg, (Reg::RegTensor<int32_t>&)srcReg, tmpEReg, mask);
    // step 3: i = fmaf ((float)e, 1.19209290e-7f, i);
    Reg::Cast<float, int32_t, castTraitF32I32>(tmpFloatReg, tmpEReg, mask);
    Reg::Axpy(tmpIReg, tmpFloatReg, LOG2_REDUCE_FMAF_COEFF1, mask);
    // step 4：p = m + 1.0f; m = m - 1.0f;
    Reg::RegTensor<float> tmpPReg;
    Reg::Adds(tmpPReg, tmpMReg, 1.0f, mask);
    Reg::Adds(tmpMReg, tmpMReg, -1.0f, mask);
    // step 5：r = 1.0f / p
    Reg::RegTensor<float> tmpRReg;
    Reg::Duplicate(tmpFloatReg, 1.0f, mask);
    Reg::Div(tmpRReg, tmpFloatReg, tmpPReg, mask);
    // step 6：qhi = m * r;
    Reg::RegTensor<float> tmpQHIReg, tmpQLOReg;
    Reg::Mul(tmpQHIReg, tmpMReg, tmpRReg, mask);
    // step 7：qhi1 = fmaf (qhi, -m, fmaf (qhi, -2.0f, m))
    Reg::Muls(tmpFloatReg, tmpQHIReg, -2.0f, mask);
    Reg::Add(tmpFloatReg, tmpFloatReg, tmpMReg, mask);
    Reg::Neg(tmpFloatReg2, tmpMReg, mask);

    Reg::Mul(tmpFloatReg2, tmpQHIReg, tmpFloatReg2, mask);
    Reg::Add(tmpFloatReg, tmpFloatReg2, tmpFloatReg, mask);

    // step 8：qlo = r * qhi1
    Reg::Mul(tmpQLOReg, tmpRReg, tmpFloatReg, mask);
    // step 9：s = qhi * qhi;
    Reg::RegTensor<float> tmpSReg;
    Reg::Mul(tmpSReg, tmpQHIReg, tmpQHIReg, mask);
    /*
     * step 10：
     * r =  0.37282535f;
     * r = fmaf (r, s, 0.4097556f)
     * r = fmaf (r, s, 0.57711965f)
     * r = fmaf (r, s, 0.96179646f)
     */
    Reg::Duplicate(tmpRReg, LOG2_BEST_FMAF_COEFF2, mask);
    Reg::Axpy<float>(tmpRReg, tmpSReg, LOG2_BEST_FMAF_COEFF1, mask);
    Reg::Duplicate(tmpFloatReg, LOG2_BEST_FMAF_COEFF3, mask);
    Reg::FusedMulDstAdd<float>(tmpRReg, tmpSReg, tmpFloatReg, mask);
    Reg::Duplicate(tmpFloatReg, LOG2_BEST_FMAF_COEFF4, mask);
    Reg::FusedMulDstAdd<float>(tmpRReg, tmpSReg, tmpFloatReg, mask);
    // step 11：r = r * s
    Reg::Mul(tmpRReg, tmpRReg, tmpSReg, mask);
    /*
     * step 12:
     * first_hi = fmaf(2.88539f, qhi, i)
     * first_lo = fmaf(2.88539f, qhi, i-first_hi)
     */
    Reg::RegTensor<float> tmpFHIReg, tmpFLOReg;
    FMaf(tmpFHIReg, tmpQHIReg, LOG2_BEST_FMAF_COEFF5, tmpIReg, mask);
    Reg::Sub(tmpFloatReg2, tmpIReg, tmpFHIReg, mask);
    FMaf(tmpFLOReg, tmpQHIReg, LOG2_BEST_FMAF_COEFF5, tmpFloatReg2, mask);
    /*
     * step 13:
     * GOOD:
     * last_lo= fmaf(r,qhi,fmaf(2.88539f,qlo,first_lo));
     * sum_lo=fmaf(0.00000003851926f,qhi,last_lo);
     * BETTER:
     * last_lo= fmaf(r,qhi,fmaf(2.88539f,qlo,r*qlo));
     * sum_lo=fmaf(0.00000003851926f,qhi,last_lo)+first_lo;
     * BEST:
     * last_lo= fmaf(r,qhi,fmaf(2.88539f,qlo,3.0f*r*qlo));
     * sum_lo=fmaf(0.00000003851926f,qhi,last_lo)+first_lo;
     */
    Reg::RegTensor<float> tmpLLOReg, tmpSLOReg;
    Reg::Mul(tmpLLOReg, tmpRReg, tmpQLOReg, mask);
    Reg::Axpy(tmpLLOReg, tmpQLOReg, LOG2_BEST_FMAF_COEFF5, mask);
    Reg::MulAddDst(tmpLLOReg, tmpRReg, tmpQHIReg, mask);
    Reg::Axpy(tmpLLOReg, tmpQHIReg, LOG2_BEST_FMAF_COEFF6, mask);
    Reg::Add(tmpSLOReg, tmpLLOReg, tmpFLOReg, mask);
    /*
     * step 14:
     * loghi = first_hi+sum_lo;
     * loglo = (first_hi - *loghi) + sum_lo;
     */
    Reg::Add(logHigh, tmpFHIReg, tmpSLOReg, mask);
    Reg::Sub(tmpFloatReg2, tmpFHIReg, logHigh, mask);
    Reg::Add(logLow, tmpFloatReg2, tmpSLOReg, mask);
}

__simd_callee__ inline void GetExpfUnchecked(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& tmPHIReg, Reg::RegTensor<float>& tmPLOReg, Reg::MaskReg& mask)
{
    /*
     * step 1:
     * r = fmaf (0.000152392517f, plo, 0.00133913534f);
     * r = fmaf (r, plo, 0.00961883925f);
     * r = fmaf (r, plo, 0.0555035882f);
     * r = fmaf (r, plo, 0.240226448f);
     * r = fmaf (r, plo, 0.693147182f);
     * r = fmaf (r, plo, 1.0f);
     */
    Reg::RegTensor<float> tmpFloatReg, tmpFloatReg2, tmpRReg;
    Reg::Duplicate(tmpRReg, EXPF_BEST_FMAF_COEFF2, mask);
    Reg::Axpy(tmpRReg, tmPLOReg, EXPF_BEST_FMAF_COEFF1, mask);
    Reg::Duplicate(tmpFloatReg, EXPF_BEST_FMAF_COEFF3, mask);
    Reg::FusedMulDstAdd(tmpRReg, tmPLOReg, tmpFloatReg, mask);
    Reg::Duplicate(tmpFloatReg, EXPF_BEST_FMAF_COEFF4, mask);
    Reg::FusedMulDstAdd(tmpRReg, tmPLOReg, tmpFloatReg, mask);
    Reg::Duplicate(tmpFloatReg, EXPF_BEST_FMAF_COEFF5, mask);
    Reg::FusedMulDstAdd(tmpRReg, tmPLOReg, tmpFloatReg, mask);
    Reg::Duplicate(tmpFloatReg, EXPF_BEST_FMAF_COEFF6, mask);
    Reg::FusedMulDstAdd(tmpRReg, tmPLOReg, tmpFloatReg, mask);
    Reg::Duplicate(tmpFloatReg, 1.0f, mask);
    Reg::FusedMulDstAdd(tmpRReg, tmPLOReg, tmpFloatReg, mask);

    // step2: r1 = (phi>0.0f) ? 0 : -2097152000;
    Reg::RegTensor<float> tmpF1Reg, tmpF2Reg;
    Reg::RegTensor<int32_t> tmpR1Reg, tmpR2Reg;
    Reg::MaskReg cmpMask;
    Reg::CompareScalar<float, CMPMODE::LE>(cmpMask, tmPHIReg, 0, mask);
    Reg::Duplicate(tmpR1Reg, EXPF_INTERVAL_CMP, cmpMask);
    // step3: r2 = r1 + 2130706432;
    Reg::Adds(tmpR2Reg, tmpR1Reg, EXPF_INTERVAL_CAST, mask);
    // step4: f1 = r * s32_to_f32(r2);
    Reg::Mul(tmpF1Reg, tmpRReg, (Reg::RegTensor<float>&)tmpR2Reg, mask);
    /*
     * step5: int32_t r5 = cvt_rzi(phi);
     * if (phi < 0.0f) return (int32_t)ceilf(phi);
     * else return (int32_t)floorf(phi);
     */
    Reg::RegTensor<int32_t> tmpR5Reg;
    Reg::MaskReg condMask;
    Reg::Cast<int32_t, float, castTraitI32F32>(tmpR5Reg, tmPHIReg, mask);
    // step6: f2 = s32_to_f32((r5 << 23) - r1);
    Reg::RegTensor<int32_t> tmpTReg;
    Reg::ShiftLefts(tmpR5Reg, tmpR5Reg, SHIFT_OFFSET, mask);
    Reg::Sub(tmpTReg, tmpR5Reg, tmpR1Reg, mask);
    tmpF2Reg = (Reg::RegTensor<float>&)tmpTReg;
    // step7: f1 * f2
    Reg::Mul(dstReg, tmpF1Reg, tmpF2Reg, mask);
}

__simd_callee__ inline void ComputeExpoOddInt(Reg::MaskReg& oddMask, Reg::RegTensor<float>& expReg, Reg::MaskReg& mask)
{
    // calculate exp is odd or not: expo_odd_int = fmaf (-2.0f, floorf (0.5f * b), b) == 1.0f;
    Reg::RegTensor<float> tmpFloatReg;
    Reg::Muls(tmpFloatReg, expReg, 0.5f, mask);
    RFloor(tmpFloatReg, tmpFloatReg, mask);
    FMaf(tmpFloatReg, tmpFloatReg, -2.0f, expReg, mask);
    Reg::CompareScalar<float, CMPMODE::EQ>(oddMask, tmpFloatReg, 1.0f, mask);
}

__simd_callee__ inline void ProcessSpecialCaseForPowF(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& baseReg, Reg::RegTensor<float>& expReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> tmpFloatReg, tmpFloatReg2;
    Reg::RegTensor<int32_t> tmpINFReg, tmpNANReg;
    Reg::MaskReg cmpMask1, cmpMask2, curMask;
    /*
     * bool p3_b_eq_0 = (b==0.0f);
     * bool p4_a_eq_1 = (a==1.0f);
     * if (p3_b_eq_0 || p4_a_eq_1)
     *      return 1.0f;
     * if (isnan(a) || isnan(b))
     *      return NAN;
     */
    Reg::CompareScalar<float, CMPMODE::EQ>(cmpMask1, expReg, 0.0f, mask);
    Reg::CompareScalar<float, CMPMODE::EQ>(cmpMask2, baseReg, 1.0f, mask);
    Reg::MaskOr(cmpMask2, cmpMask1, cmpMask2, mask);
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(dstReg, 1.0f, cmpMask2);
    Reg::MaskNot(curMask, cmpMask2, mask);
    IsNanNum(cmpMask1, baseReg, mask);
    IsNanNum(cmpMask2, expReg, mask);
    Reg::MaskOr(cmpMask2, cmpMask1, cmpMask2, curMask);
    Reg::Duplicate<int32_t, Reg::MaskMergeMode::MERGING>((Reg::RegTensor<int32_t>&)dstReg, F32_NAN, cmpMask2);
    Reg::MaskXor(curMask, cmpMask2, curMask, mask);
    /*
     * if (isinf(a) || (a==0.0f))
     *     int32_t r10=f32_to_s32(a);
     *     int32_t r11=r10^0x7F800000;
     *     bool p8_a_lower_0 = (b < 0.0f);
     *     int32_t r12 = (p8_a_lower_0 ? r11 : r10);
     *     int32_t r13= r12 & 0x7FFFFFFF;
     *     return s32_to_f32(p1_expo_odd_int?r12:r13);
     *
     */
    IsInfNum(cmpMask1, baseReg, curMask);
    Reg::CompareScalar<float, CMPMODE::EQ>(cmpMask2, baseReg, 0.0f, curMask);
    Reg::MaskOr(cmpMask1, cmpMask1, cmpMask2, mask);
    Reg::CompareScalar<float, CMPMODE::LT>(cmpMask2, expReg, 0.0f, cmpMask1);
    Reg::Duplicate((Reg::RegTensor<int32_t>&)tmpFloatReg, R10_COEFF, mask);
    Reg::Xor(
        (Reg::RegTensor<int32_t>&)tmpFloatReg, (Reg::RegTensor<int32_t>&)baseReg, (Reg::RegTensor<int32_t>&)tmpFloatReg,
        curMask);
    Reg::Select(tmpFloatReg, tmpFloatReg, baseReg, cmpMask2);
    Reg::Duplicate((Reg::RegTensor<int32_t>&)tmpFloatReg2, R12_COEFF, mask);
    Reg::And(
        (Reg::RegTensor<int32_t>&)tmpFloatReg2, (Reg::RegTensor<int32_t>&)tmpFloatReg,
        (Reg::RegTensor<int32_t>&)tmpFloatReg2, curMask);
    ComputeExpoOddInt(cmpMask2, expReg, mask);
    Reg::Select(tmpFloatReg, tmpFloatReg, tmpFloatReg2, cmpMask2);
    Reg::Select(dstReg, tmpFloatReg, dstReg, cmpMask1);
    Reg::MaskXor(curMask, cmpMask1, curMask, mask);
    /*
     * if (a < 0.0f)
     *    float tmp_r=p1_expo_odd_int?(-r):r;
     *     r = (b != floorf(b)) ? NAN : tmp_r;
     *
     */
    Reg::Neg(tmpFloatReg, dstReg, curMask);
    Reg::Select(tmpFloatReg, tmpFloatReg, dstReg, cmpMask2);
    RFloor(tmpFloatReg2, expReg, curMask);
    Reg::Compare<float, CMPMODE::NE>(cmpMask1, expReg, tmpFloatReg2, curMask);
    Reg::Duplicate<int32_t, Reg::MaskMergeMode::MERGING>((Reg::RegTensor<int32_t>&)tmpFloatReg, F32_NAN, cmpMask1);
    Reg::CompareScalar<float, CMPMODE::LT>(cmpMask2, baseReg, 0.0f, curMask);
    Reg::Select(dstReg, tmpFloatReg, dstReg, cmpMask2);
    /*
     *  if ((a == -1.0f) && isinf(b))
     *      r = 1.0f;
     *
     */
    Reg::CompareScalar<int32_t, CMPMODE::EQ>(cmpMask1, (Reg::RegTensor<int32_t>&)expReg, INF, curMask);
    Reg::CompareScalar<int32_t, CMPMODE::EQ>(cmpMask2, (Reg::RegTensor<int32_t>&)expReg, NEG_INF, curMask);
    Reg::MaskOr(cmpMask1, cmpMask1, cmpMask2, mask);
    Reg::CompareScalar<float, CMPMODE::EQ>(cmpMask2, baseReg, -1.0f, cmpMask1);
    Reg::Duplicate<float, Reg::MaskMergeMode::MERGING>(dstReg, 1.0f, cmpMask2);
}

__simd_callee__ inline void GetExpCore(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& tmpLHIReg, Reg::RegTensor<float>& tmpLLOReg,
    Reg::RegTensor<float>& expReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> tmpTHIReg, tmPHIReg, tmPLOReg, tmpRReg;
    // step 1: thi = lhi * b;
    Reg::Mul(tmpTHIReg, tmpLHIReg, expReg, mask);
    // step 2: phi = roundf(thi);
    Reg::Truncate<float, RoundMode::CAST_ROUND>(tmPHIReg, tmpTHIReg, mask);
    // step 3: plo = fmaf(lhi, b, -phi)+llo*b;
    Reg::RegTensor<float> tmpFloatReg, tmpFloatReg2;
    Reg::Neg(tmPLOReg, tmPHIReg, mask);
    Reg::MulAddDst(tmPLOReg, tmpLHIReg, expReg, mask);
    Reg::MulAddDst(tmPLOReg, tmpLLOReg, expReg, mask);
    // step 4: my_expf__improved(phi, plo, &r);
    GetExpfUnchecked(tmpRReg, tmPHIReg, tmPLOReg, mask);
    /*
     * step 5:
     * tmp_r =(thi < 0.0f) ?0.0f : MY_INF_F;
     * r = (fabsf(thi) > EXP_OVFL_UNFL_F) ? tmp_r : r;
     */
    Reg::RegTensor<int32_t> tmpINFReg;
    Reg::MaskReg cmpMask1, cmpMask2;
    Reg::CompareScalar<float, CMPMODE::GE>(cmpMask1, tmpTHIReg, 0.0f, mask);
    // mode zeroing dup inf/zero reg.
    Reg::Duplicate((Reg::RegTensor<int32_t>&)tmpFloatReg, INF, cmpMask1);
    Reg::Abs(tmpFloatReg2, tmpTHIReg, mask);
    Reg::CompareScalar<float, CMPMODE::GT>(cmpMask2, tmpFloatReg2, EXP_OVFL_UNFL_F, mask);
    Reg::Select(dstReg, tmpFloatReg, tmpRReg, cmpMask2);
}

template <typename T>
__simd_callee__ inline void LoadSrcData(
    Reg::RegTensor<float>& srcReg, __ubuf__ T* src0, uint16_t index, Reg::MaskReg& mask)
{
    Reg::RegTensor<T> srcTmpReg;
    if constexpr (std::is_same<T, half>::value || std::is_same<T, bfloat16_t>::value) {
        Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcTmpReg, src0 + index * B32_DATA_NUM_PER_REPEAT);
        Reg::Cast<float, T, castTraitF16F32>(srcReg, srcTmpReg, mask);
    } else {
        Reg::LoadAlign(srcReg, src0 + index * B32_DATA_NUM_PER_REPEAT);
    }
}

template <typename T>
__simd_callee__ inline void LoadSrcScalarData(Reg::RegTensor<float>& srcReg, const T scalarValue)
{
    Reg::RegTensor<T> srcTmpReg;
    Reg::MaskReg fullMask = Reg::CreateMask<T>();
    Reg::Duplicate(srcTmpReg, scalarValue, fullMask);
    if constexpr (std::is_same<T, half>::value || std::is_same<T, bfloat16_t>::value) {
        Reg::Cast<float, T, castTraitF16F32>(srcReg, srcTmpReg, fullMask);
    } else {
        srcReg = srcTmpReg;
    }
}

template <typename T>
__simd_callee__ inline void StoreDstData(
    __ubuf__ T* dst, Reg::RegTensor<float>& dstReg, uint16_t index, Reg::MaskReg& mask)
{
    Reg::RegTensor<T> dstTmpReg;
    if constexpr (std::is_same<T, half>::value || std::is_same<T, bfloat16_t>::value) {
        Reg::Cast<T, float, castTraitF32F16>(dstTmpReg, dstReg, mask);
        Reg::Pack<uint16_t, uint32_t, Reg::HighLowPart::LOWEST>(
            (Reg::RegTensor<uint16_t>&)dstTmpReg, (Reg::RegTensor<uint32_t>&)dstTmpReg);
        Reg::MaskPack(mask, mask);
        Reg::StoreAlign(dst + index * B32_DATA_NUM_PER_REPEAT, dstTmpReg, mask);
    } else {
        Reg::StoreAlign(dst + index * B32_DATA_NUM_PER_REPEAT, dstReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFBaseLogImpl(
    __ubuf__ float* tmpLHIBuffer, __ubuf__ float* tmpLLOBuffer, __ubuf__ T* src0, uint32_t calCount,
    uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpBaseReg, tmpDstReg;
    Reg::RegTensor<float> tmpLHIReg, tmpLLOReg;

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpBaseReg, src0, i, mask);

        Reg::Abs(tmpDstReg, tmpBaseReg, mask);
        GetLogFExt(tmpLHIReg, tmpLLOReg, tmpDstReg, mask);

        Reg::DataCopy(tmpLHIBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpLHIReg, mask);
        Reg::DataCopy(tmpLLOBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpLLOReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFBaseLogImpl(
    __ubuf__ float* tmpLHIBuffer, __ubuf__ float* tmpLLOBuffer, const T scalarValue, uint32_t calCount,
    uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpBaseReg, tmpDstReg;
    Reg::RegTensor<float> tmpLHIReg, tmpLLOReg;
    LoadSrcScalarData(tmpBaseReg, scalarValue);

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        Reg::Abs(tmpDstReg, tmpBaseReg, mask);
        GetLogFExt(tmpLHIReg, tmpLLOReg, tmpDstReg, mask);
        Reg::DataCopy(tmpLHIBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpLHIReg, mask);
        Reg::DataCopy(tmpLLOBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpLLOReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFExpImpl(
    __ubuf__ float* tmpExpBuffer, __ubuf__ float* tmpLogHighBuffer, __ubuf__ float* tmpLogLowBuffer, __ubuf__ T* src1,
    uint32_t calCount, uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpLHIReg, tmpLLOReg, tmpExpReg, tmpDstReg;

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpLHIReg, tmpLogHighBuffer, i, mask);
        LoadSrcData(tmpLLOReg, tmpLogLowBuffer, i, mask);
        LoadSrcData(tmpExpReg, src1, i, mask);
        GetExpCore(tmpDstReg, tmpLHIReg, tmpLLOReg, tmpExpReg, mask);
        Reg::StoreAlign(tmpExpBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpDstReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFExpImpl(
    __ubuf__ float* tmpExpBuffer, __ubuf__ float* tmpLogHighBuffer, __ubuf__ float* tmpLogLowBuffer,
    const T scalarValue, uint32_t calCount, uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpLHIReg, tmpLLOReg, tmpExpReg, tmpDstReg;
    LoadSrcScalarData(tmpExpReg, scalarValue);
    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpLHIReg, tmpLogHighBuffer, i, mask);
        LoadSrcData(tmpLLOReg, tmpLogLowBuffer, i, mask);
        GetExpCore(tmpDstReg, tmpLHIReg, tmpLLOReg, tmpExpReg, mask);
        Reg::StoreAlign(tmpExpBuffer + i * B32_DATA_NUM_PER_REPEAT, tmpDstReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFSpecialCaseImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, __ubuf__ float* tmpExpBuffer, uint32_t calCount,
    uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpBaseReg, src0, i, mask);
        LoadSrcData(tmpExpReg, src1, i, mask);
        Reg::LoadAlign(castDstReg, tmpExpBuffer + i * B32_DATA_NUM_PER_REPEAT);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, mask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFSpecialCaseImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const T scalarValue, __ubuf__ float* tmpExpBuffer, uint32_t calCount,
    uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;
    LoadSrcScalarData(tmpExpReg, scalarValue);
    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpBaseReg, src0, i, mask);
        Reg::LoadAlign(castDstReg, tmpExpBuffer + i * B32_DATA_NUM_PER_REPEAT);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, mask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void ComputePowFSpecialCaseImpl(
    __ubuf__ T* dst, const T scalarValue, __ubuf__ T* src1, __ubuf__ float* tmpExpBuffer, uint32_t calCount,
    uint16_t repeatTime)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;
    LoadSrcScalarData(tmpBaseReg, scalarValue);
    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        LoadSrcData(tmpExpReg, src1, i, mask);
        Reg::LoadAlign(castDstReg, tmpExpBuffer + i * B32_DATA_NUM_PER_REPEAT);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, mask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

__aicore__ inline void InitTmpBuffer(
    __ubuf__ uint32_t*& tmpBuffer, __ubuf__ float*& tmpLogBuffer, __ubuf__ float*& tmpExpBuffer,
    const uint32_t alignCount)
{
    tmpLogBuffer = (__ubuf__ float*)tmpBuffer;
    tmpExpBuffer = (__ubuf__ float*)((__ubuf__ uint8_t*)tmpLogBuffer + sizeof(float) * alignCount);
}

template <typename T>
__aicore__ inline void PowFComputeImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, __ubuf__ uint32_t* tmpBuffer, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);
    __ubuf__ float* tmpLowBuffer;
    __ubuf__ float* tmpHighBuffer;

    uint32_t alignCount = (calCount + 31) / 32 * 32;

    InitTmpBuffer(tmpBuffer, tmpHighBuffer, tmpLowBuffer, alignCount);
    __ubuf__ float* tmpExpBuffer = tmpHighBuffer;

    ComputePowFBaseLogImpl<T>(tmpHighBuffer, tmpLowBuffer, src0, calCount, repeatTime);
    ComputePowFExpImpl<T>(tmpExpBuffer, tmpHighBuffer, tmpLowBuffer, src1, calCount, repeatTime);
    ComputePowFSpecialCaseImpl<T>(dst, src0, src1, tmpExpBuffer, calCount, repeatTime);
}

template <typename T>
__aicore__ inline void PowFComputeImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const T& scalarValue, __ubuf__ uint32_t* tmpBuffer, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);
    __ubuf__ float* tmpLowBuffer;
    __ubuf__ float* tmpHighBuffer;

    uint32_t alignCount = (calCount + 31) / 32 * 32;

    InitTmpBuffer(tmpBuffer, tmpHighBuffer, tmpLowBuffer, alignCount);

    __ubuf__ float* tmpExpBuffer = tmpHighBuffer;

    ComputePowFBaseLogImpl<T>(tmpHighBuffer, tmpLowBuffer, src0, calCount, repeatTime);
    ComputePowFExpImpl<T>(tmpExpBuffer, tmpHighBuffer, tmpLowBuffer, scalarValue, calCount, repeatTime);
    ComputePowFSpecialCaseImpl<T>(dst, src0, scalarValue, tmpExpBuffer, calCount, repeatTime);
}

template <typename T>
__aicore__ inline void PowFComputeImpl(
    __ubuf__ T* dst, const T& scalarValue, __ubuf__ T* src1, __ubuf__ uint32_t* tmpBuffer, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);
    __ubuf__ float* tmpLowBuffer;
    __ubuf__ float* tmpHighBuffer;

    uint32_t alignCount = (calCount + 31) / 32 * 32;

    InitTmpBuffer(tmpBuffer, tmpHighBuffer, tmpLowBuffer, alignCount);
    __ubuf__ float* tmpExpBuffer = tmpHighBuffer;

    ComputePowFBaseLogImpl<T>(tmpHighBuffer, tmpLowBuffer, scalarValue, calCount, repeatTime);
    ComputePowFExpImpl<T>(tmpExpBuffer, tmpHighBuffer, tmpLowBuffer, src1, calCount, repeatTime);
    ComputePowFSpecialCaseImpl<T>(dst, scalarValue, src1, tmpExpBuffer, calCount, repeatTime);
}

/*********** PowF Intrinsic Impl **********/
__simd_callee__ inline void GetPowFIntrinsicCore(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& baseReg, Reg::RegTensor<float>& expReg, Reg::MaskReg& mask)
{
    // Compute dst = exp(exp * ln(|base|))
    Reg::RegTensor<float> tmpReg;
    Reg::Abs(tmpReg, baseReg, mask);
    Reg::Ln(tmpReg, tmpReg, mask);
    Reg::Mul(dstReg, expReg, tmpReg, mask);
    Reg::Exp(dstReg, dstReg, mask);
}

template <typename T>
__simd_vf__ inline void PowFIntrinsicTensorTensorImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint32_t calCount, uint16_t repeatTime)
{
    Reg::MaskReg mask, tmpMask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        tmpMask = mask;
        LoadSrcData(tmpBaseReg, src0, i, mask);
        LoadSrcData(tmpExpReg, src1, i, mask);
        GetPowFIntrinsicCore(castDstReg, tmpBaseReg, tmpExpReg, mask);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, tmpMask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void PowFIntrinsicTensorScalarImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const T scalarValue, uint32_t calCount, uint16_t repeatTime)
{
    Reg::MaskReg mask, tmpMask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;
    LoadSrcScalarData(tmpExpReg, scalarValue);

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        tmpMask = mask;
        LoadSrcData(tmpBaseReg, src0, i, mask);
        GetPowFIntrinsicCore(castDstReg, tmpBaseReg, tmpExpReg, mask);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, tmpMask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void PowFIntrinsicScalarTensorImpl(
    __ubuf__ T* dst, const T scalarValue, __ubuf__ T* src1, uint32_t calCount, uint16_t repeatTime)
{
    Reg::MaskReg mask, tmpMask;
    Reg::RegTensor<float> tmpBaseReg, tmpExpReg, castDstReg;
    LoadSrcScalarData(tmpBaseReg, scalarValue);

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<float>(calCount);
        tmpMask = mask;
        LoadSrcData(tmpExpReg, src1, i, mask);
        GetPowFIntrinsicCore(castDstReg, tmpBaseReg, tmpExpReg, mask);
        ProcessSpecialCaseForPowF(castDstReg, tmpBaseReg, tmpExpReg, tmpMask);
        StoreDstData(dst, castDstReg, i, mask);
    }
}

template <typename T>
__aicore__ inline void PowFIntrinsicImpl(__ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTimes = DivCeil(calCount, eleCountPerVL);
    PowFIntrinsicTensorTensorImpl<T>(dst, src0, src1, calCount, repeatTimes);
}

template <typename T>
__aicore__ inline void PowFIntrinsicImpl(__ubuf__ T* dst, __ubuf__ T* src0, const T& scalarValue, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTimes = DivCeil(calCount, eleCountPerVL);
    PowFIntrinsicTensorScalarImpl<T>(dst, src0, scalarValue, calCount, repeatTimes);
}

template <typename T>
__aicore__ inline void PowFIntrinsicImpl(__ubuf__ T* dst, const T& scalarValue, __ubuf__ T* src1, uint32_t calCount)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(float);
    uint16_t repeatTimes = DivCeil(calCount, eleCountPerVL);
    PowFIntrinsicScalarTensorImpl<T>(dst, scalarValue, src1, calCount, repeatTimes);
}
} // namespace PowF

namespace PowI {
constexpr int16_t SHIFT_ONE_BIT = 1;
constexpr int16_t BITS_PER_BYTE = 8;

template <typename T>
__simd_callee__ inline void GetPowI(
    Reg::RegTensor<T>& dstReg, Reg::RegTensor<T>& baseReg, Reg::RegTensor<T>& expReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<T> tmpReg, tmpReg2;
    Reg::MaskReg expMask;
    // step 1: result1 = result * a;
    Reg::Mul(tmpReg, dstReg, baseReg, mask);
    Reg::Duplicate(tmpReg2, 1, mask);
    // step 2: mask = b & 1;
    Reg::And(tmpReg2, expReg, tmpReg2, mask);
    // step 3: result = select(mask, result1, result);
    Reg::CompareScalar<T, CMPMODE::EQ>(expMask, tmpReg2, 1, mask);
    Reg::Select(dstReg, tmpReg, dstReg, expMask);
    // step4: b /= 2;
    Reg::ShiftRights(expReg, expReg, SHIFT_ONE_BIT, mask);
    // step5: a *= a;
    Reg::Mul(baseReg, baseReg, baseReg, mask);
}

template <typename T>
__simd_callee__ inline void ProcessSpecialCaseForPowI(
    Reg::RegTensor<T>& dstReg, Reg::RegTensor<T>& baseReg, Reg::RegTensor<T>& expReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<T> tmpRReg;
    Reg::MaskReg cmpMask1, cmpMask2, condMask;
    /*
     * special case 1:
     * if (exp == 0) || (base == 1) {
     *    r = 1;
     * }
     */
    Reg::CompareScalar<T, CMPMODE::EQ>(cmpMask1, expReg, 0, mask);
    Reg::CompareScalar<T, CMPMODE::EQ>(cmpMask2, baseReg, 1, mask);
    Reg::MaskOr(condMask, cmpMask1, cmpMask2, mask);
    Reg::Duplicate(tmpRReg, 1, mask);
    Reg::Select(dstReg, tmpRReg, dstReg, condMask);
    Reg::MaskXor(mask, mask, condMask, mask);

    if constexpr (SupportType<T, int8_t, int16_t, int32_t>()) {
        /*
         * special case 2:
         * else if (base != -1 && exp < 0) {
         *    r = 0;
         * }
         */
        Reg::CompareScalar<T, CMPMODE::NE>(cmpMask1, baseReg, -1, mask);
        Reg::CompareScalar<T, CMPMODE::LT>(cmpMask2, expReg, 0, mask);
        Reg::MaskAnd(condMask, cmpMask1, cmpMask2, mask);
        Reg::Duplicate(tmpRReg, 0, mask);
        Reg::Select(dstReg, tmpRReg, dstReg, condMask);

        /*
         * special case 3:
         * else if (base = -1 && exp < 0) {
         *    exp & 1 == 1 ? r = -1 : r = 1
         * }
         */
        Reg::MaskXor(mask, mask, condMask, mask);
        Reg::CompareScalar<T, CMPMODE::EQ>(cmpMask1, baseReg, -1, mask);
        Reg::CompareScalar<T, CMPMODE::LT>(cmpMask2, expReg, 0, mask);
        Reg::MaskAnd(condMask, cmpMask1, cmpMask2, mask);
        Reg::RegTensor<T> tmpOneReg, tmpNegOneReg;
        Reg::Duplicate(tmpOneReg, 1, mask);
        Reg::Duplicate(tmpNegOneReg, -1, mask);
        Reg::And(tmpRReg, expReg, tmpOneReg, condMask);
        Reg::CompareScalar<T, CMPMODE::EQ>(cmpMask1, tmpRReg, 1, condMask);
        Reg::Select(tmpRReg, tmpNegOneReg, tmpOneReg, cmpMask1);
        Reg::Select(dstReg, tmpRReg, dstReg, condMask);
    }
}

template <typename T>
__simd_callee__ inline void GetPowICompute(
    Reg::RegTensor<T>& dstReg, Reg::RegTensor<T>& baseReg, Reg::RegTensor<T>& expReg, Reg::MaskReg& mask,
    const uint16_t maxLoop)
{
    Reg::RegTensor<T> tmpBaseReg = baseReg;
    Reg::RegTensor<T> tmpExpReg = expReg;
    Reg::MaskReg tmpMask = mask;
    for (uint16_t j = 0; j < maxLoop; j++) {
        GetPowI(dstReg, tmpBaseReg, tmpExpReg, mask);
    }
    ProcessSpecialCaseForPowI(dstReg, baseReg, expReg, tmpMask);
}

template <typename T>
struct PowICastType {
    using type = T;
};

template <>
struct PowICastType<int8_t> {
    using type = int16_t;
};

template <>
struct PowICastType<uint8_t> {
    using type = uint16_t;
};

template <typename T>
__aicore__ inline uint16_t CountLeadingZeros(T x)
{
    if (x == 0)
        return sizeof(T) * 8;
    uint16_t count = 0;
    T mask = static_cast<T>(1 << (sizeof(T) * 8 - 1));
    while ((x & mask) == 0) {
        count++;
        x <<= 1;
    }
    return count;
}

template <typename T, typename ConvType>
__simd_callee__ inline void LoadSrcData(
    Reg::RegTensor<ConvType>& dstReg, __ubuf__ T* src, uint16_t index, Reg::MaskReg& mask)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(ConvType);
    Reg::RegTensor<T> srcTmpReg;
    if constexpr (sizeof(T) == 1) {
        Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B8>(srcTmpReg, src + index * eleCountPerVL);
        Reg::Cast<ConvType, T, castTraitI8I16>(dstReg, srcTmpReg, mask);
    } else {
        Reg::LoadAlign(dstReg, src + index * eleCountPerVL);
    }
}

template <typename T, typename ConvType>
__simd_callee__ inline void StoreDstData(
    __ubuf__ T* dst, Reg::RegTensor<ConvType>& dstReg, uint16_t index, Reg::MaskReg& mask)
{
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(ConvType);
    Reg::RegTensor<T> dstTmpReg;

    if constexpr (sizeof(T) == 1) {
        Reg::Pack<uint8_t, uint16_t, Reg::HighLowPart::LOWEST>(
            (Reg::RegTensor<uint8_t>&)dstTmpReg, (Reg::RegTensor<uint16_t>&)dstReg);
        Reg::MaskPack(mask, mask);
        Reg::StoreAlign(dst + index * eleCountPerVL, dstTmpReg, mask);
    } else {
        Reg::StoreAlign(dst + index * eleCountPerVL, dstReg, mask);
    }
}

template <typename T>
__aicore__ inline uint16_t GetMaxLoop(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, uint32_t calCount)
{
    if constexpr (sizeof(T) == 1) {
        return sizeof(T) * BITS_PER_BYTE;
    } else {
        AscendC::ReduceMax(dstTensor, srcTensor, srcTensor, calCount);
        event_t eventIdVToS = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::V_S));
        SetFlag<HardEvent::V_S>(eventIdVToS);
        WaitFlag<HardEvent::V_S>(eventIdVToS);
        T maxNum = dstTensor.GetValue(0);
        return sizeof(T) * BITS_PER_BYTE - CountLeadingZeros(maxNum);
    }
}

template <typename T>
__simd_vf__ inline void PowIComputeImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, __ubuf__ T* src1, uint32_t calCount, uint16_t maxLoop)
{
    using ConvType = typename PowICastType<T>::type;
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(ConvType);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);

    Reg::RegTensor<ConvType> baseReg, expReg;
    Reg::RegTensor<ConvType> initRetReg, dstReg;
    Reg::MaskReg mask;

    Reg::MaskReg fullMask = Reg::CreateMask<T>();
    Reg::Duplicate(initRetReg, 1, fullMask);
    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<ConvType>(calCount);

        LoadSrcData(baseReg, src0, i, mask);
        LoadSrcData(expReg, src1, i, mask);
        dstReg = initRetReg;
        GetPowICompute(dstReg, baseReg, expReg, mask, maxLoop);
        StoreDstData(dst, dstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void PowIComputeImpl(
    __ubuf__ T* dst, __ubuf__ T* src0, const T scalarValue, uint32_t calCount, uint16_t maxLoop)
{
    using ConvType = typename PowICastType<T>::type;
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(ConvType);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);

    Reg::RegTensor<ConvType> baseReg, expReg;
    Reg::RegTensor<ConvType> initRetReg, dstReg;
    Reg::MaskReg mask;

    Reg::MaskReg fullMask = Reg::CreateMask<T>();
    Reg::Duplicate(initRetReg, 1, fullMask);
    Reg::Duplicate(expReg, scalarValue, fullMask);

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<ConvType>(calCount);
        LoadSrcData(baseReg, src0, i, mask);
        dstReg = initRetReg;
        GetPowICompute(dstReg, baseReg, expReg, mask, maxLoop);
        StoreDstData(dst, dstReg, i, mask);
    }
}

template <typename T>
__simd_vf__ inline void PowIComputeImpl(
    __ubuf__ T* dst, const T scalarValue, __ubuf__ T* src1, uint32_t calCount, uint16_t maxLoop)
{
    using ConvType = typename PowICastType<T>::type;
    constexpr uint16_t eleCountPerVL = GetVecLen() / sizeof(ConvType);
    uint16_t repeatTime = DivCeil(calCount, eleCountPerVL);

    Reg::RegTensor<ConvType> baseReg, expReg;
    Reg::RegTensor<ConvType> initRetReg, dstReg;
    Reg::MaskReg mask;
    Reg::MaskReg fullMask = Reg::CreateMask<T>();
    Reg::Duplicate(initRetReg, 1, fullMask);
    Reg::Duplicate(baseReg, scalarValue, fullMask);

    for (uint16_t i = 0; i < repeatTime; i++) {
        mask = Reg::UpdateMask<ConvType>(calCount);
        LoadSrcData(expReg, src1, i, mask);
        dstReg = initRetReg;
        GetPowICompute(dstReg, baseReg, expReg, mask, maxLoop);
        StoreDstData(dst, dstReg, i, mask);
    }
}

} // namespace PowI
} // namespace PowerC310Impl

template <typename T, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowCheckType()
{
    if constexpr (config.algo == AscendC::PowerAlgo::DOUBLE_FLOAT_TECH) {
        static_assert(
            SupportType<T, half, float, bfloat16_t>(),
            "Type must be half/float/bfloat16 in double float tech algorithm.");
    }

    if constexpr (config.algo == AscendC::PowerAlgo::INTRINSIC) {
        static_assert(
            SupportType<T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float>(),
            "Type must be uint8_t/int8_t/uint16_t/int16_t/uint32_t/int32_t/half/float in intrinsic tech algorithm.");
    }
}

template <typename T>
__aicore__ inline constexpr bool IsFloatNum()
{
    return SupportType<T, float, half, bfloat16_t>();
}

template <typename T>
__aicore__ inline constexpr bool IsIntegerNum()
{
    return SupportType<T, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t>();
}

__aicore__ inline constexpr uint32_t GetPowerTmpBufferLiveNode()
{
    constexpr uint32_t tmpBufferLiveNode = sizeof(float) * 3;
    return tmpBufferLiveNode;
}

template <typename T>
__aicore__ inline uint32_t GetPowTmpBufferSize(const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    uint32_t sharedTmpBufferSize = sharedTmpBuffer.GetSize() / GetPowerTmpBufferLiveNode();
    return AlignUp(sharedTmpBufferSize, GetDataBlockSizeInBytes());
}

// PowImpl(tensor, tensor) float/half input
template <typename T, const PowerConfig& config>
__aicore__ inline void PowImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    __ubuf__ T* src0 = (__ubuf__ T*)src0Tensor.GetPhyAddr();
    __ubuf__ T* src1 = (__ubuf__ T*)src1Tensor.GetPhyAddr();
    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();

    if constexpr (IsFloatNum<T>()) {
        if constexpr (config.algo == PowerAlgo::INTRINSIC) {
            PowerC310Impl::PowF::PowFIntrinsicImpl(dst, src0, src1, calCount);
        } else {
            __ubuf__ uint32_t* tmpBuffer = (__ubuf__ uint32_t*)sharedTmpBuffer.GetPhyAddr();
            uint32_t sharedTmpBufferSize = GetPowTmpBufferSize<T>(sharedTmpBuffer);
            uint32_t count = calCount;
            uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, sharedTmpBufferSize));
            for (uint16_t i = 0; i < repeatTimes; i++) {
                uint32_t remainCount = count - sharedTmpBufferSize * i;
                uint32_t oneRepSize = remainCount < sharedTmpBufferSize ? remainCount : sharedTmpBufferSize;
                PowerC310Impl::PowF::PowFComputeImpl(
                    dst + i * sharedTmpBufferSize, src0 + i * sharedTmpBufferSize, src1 + i * sharedTmpBufferSize,
                    tmpBuffer, oneRepSize);
            }
        }
    } else if constexpr (IsIntegerNum<T>()) {
        uint16_t maxLoop = PowerC310Impl::PowI::GetMaxLoop(dstTensor, src1Tensor, calCount);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
        PowerC310Impl::PowI::PowIComputeImpl<T>(dst, src0, src1, calCount, maxLoop);
    }
}

// PowImpl(tensor, scalar) float input
template <typename T, const PowerConfig& config>
__aicore__ inline void PowImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const T& scalarValue,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    __ubuf__ T* base = (__ubuf__ T*)src0Tensor.GetPhyAddr();
    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();

    if constexpr (IsFloatNum<T>()) {
        if constexpr (config.algo == PowerAlgo::INTRINSIC) {
            PowerC310Impl::PowF::PowFIntrinsicImpl(dst, base, scalarValue, calCount);
        } else {
            __ubuf__ uint32_t* tmpBuffer = (__ubuf__ uint32_t*)sharedTmpBuffer.GetPhyAddr();
            uint32_t sharedTmpBufferSize = GetPowTmpBufferSize<T>(sharedTmpBuffer);
            uint32_t count = calCount;
            uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, sharedTmpBufferSize));
            for (uint16_t i = 0; i < repeatTimes; i++) {
                uint32_t remainCount = count - sharedTmpBufferSize * i;
                uint32_t oneRepSize = remainCount < sharedTmpBufferSize ? remainCount : sharedTmpBufferSize;
                PowerC310Impl::PowF::PowFComputeImpl(
                    dst + i * sharedTmpBufferSize, base + i * sharedTmpBufferSize, scalarValue, tmpBuffer, oneRepSize);
            }
        }
    } else if constexpr (IsIntegerNum<T>()) {
        uint16_t maxLoop =
            sizeof(T) * PowerC310Impl::PowI::BITS_PER_BYTE - PowerC310Impl::PowI::CountLeadingZeros(scalarValue);
        PowerC310Impl::PowI::PowIComputeImpl<T>(dst, base, scalarValue, calCount, maxLoop);
    }
}

// PowImpl(scalar, tensor) float input
template <typename T, const PowerConfig& config>
__aicore__ inline void PowImpl(
    const LocalTensor<T>& dstTensor, const T& scalarValue, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    __ubuf__ T* exp = (__ubuf__ T*)src1Tensor.GetPhyAddr();
    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();

    if constexpr (IsFloatNum<T>()) {
        if constexpr (config.algo == PowerAlgo::INTRINSIC) {
            PowerC310Impl::PowF::PowFIntrinsicImpl(dst, scalarValue, exp, calCount);
        } else {
            __ubuf__ uint32_t* tmpBuffer = (__ubuf__ uint32_t*)sharedTmpBuffer.GetPhyAddr();
            uint32_t sharedTmpBufferSize = GetPowTmpBufferSize<T>(sharedTmpBuffer);
            uint32_t count = calCount;
            uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, sharedTmpBufferSize));
            for (uint16_t i = 0; i < repeatTimes; i++) {
                uint32_t remainCount = count - sharedTmpBufferSize * i;
                uint32_t oneRepSize = remainCount < sharedTmpBufferSize ? remainCount : sharedTmpBufferSize;
                PowerC310Impl::PowF::PowFComputeImpl(
                    dst + i * sharedTmpBufferSize, scalarValue, exp + i * sharedTmpBufferSize, tmpBuffer, oneRepSize);
            }
        }
    } else if constexpr (IsIntegerNum<T>()) {
        uint16_t maxLoop = PowerC310Impl::PowI::GetMaxLoop(dstTensor, src1Tensor, calCount);
        event_t eventIdSToV = static_cast<event_t>(GetTPipePtr()->FetchEventID(HardEvent::S_V));
        SetFlag<HardEvent::S_V>(eventIdSToV);
        WaitFlag<HardEvent::S_V>(eventIdSToV);
        PowerC310Impl::PowI::PowIComputeImpl<T>(dst, scalarValue, exp, calCount, maxLoop);
    }
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const T& scalarValue, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    PowCheckType<T, config>();
    CheckTensorPos<T>(dstTensor, Hardware::UB, "dstTensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<T>(src1Tensor, Hardware::UB, "src1Tensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<uint8_t>(sharedTmpBuffer, Hardware::UB, "sharedTmpBuffer", "VECIN / VECOUT / VECCALC", "Power");
    CheckCalCount(calCount, "calCount", src1Tensor, "src1Tensor", "Power");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Power");

    PowImpl<T, config>(dstTensor, scalarValue, src1Tensor, sharedTmpBuffer, calCount);
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const T& scalarValue,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    PowCheckType<T, config>();
    CheckTensorPos<T>(dstTensor, Hardware::UB, "dstTensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<T>(src0Tensor, Hardware::UB, "src0Tensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<uint8_t>(sharedTmpBuffer, Hardware::UB, "sharedTmpBuffer", "VECIN / VECOUT / VECCALC", "Power");
    CheckCalCount(calCount, "calCount", src0Tensor, "src0Tensor", "Power");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Power");

    PowImpl<T, config>(dstTensor, src0Tensor, scalarValue, sharedTmpBuffer, calCount);
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    PowCheckType<T, config>();
    CheckTensorPos<T>(dstTensor, Hardware::UB, "dstTensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<T>(src0Tensor, Hardware::UB, "src0Tensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<T>(src1Tensor, Hardware::UB, "src1Tensor", "VECIN / VECOUT / VECCALC", "Power");
    CheckTensorPos<uint8_t>(sharedTmpBuffer, Hardware::UB, "sharedTmpBuffer", "VECIN / VECOUT / VECCALC", "Power");
    CheckCalCount(calCount, "calCount", src0Tensor, "src0Tensor", "Power");
    CheckCalCount(calCount, "calCount", src1Tensor, "src1Tensor", "Power");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Power");

    PowImpl<T, config>(dstTensor, src0Tensor, src1Tensor, sharedTmpBuffer, calCount);
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const LocalTensor<T>& src1Tensor,
    uint32_t calCount)
{
    LocalTensor<uint8_t> stackTensor;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(stackTensor);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
    PowerCommonImpl<T, isReuseSource, config>(dstTensor, src0Tensor, src1Tensor, stackTensor, calCount);
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const T& src0Scalar, const LocalTensor<T>& src1Tensor, uint32_t calCount)
{
    LocalTensor<uint8_t> stackTensor;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(stackTensor);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
    PowerCommonImpl<T, isReuseSource, config>(dstTensor, src0Scalar, src1Tensor, stackTensor, calCount);
}

template <typename T, bool isReuseSource = false, const PowerConfig& config = defaultPowerConfig>
__aicore__ inline void PowerCommonImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& src0Tensor, const T& src1Scalar, uint32_t calCount)
{
    LocalTensor<uint8_t> stackTensor;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(stackTensor);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
    PowerCommonImpl<T, isReuseSource, config>(dstTensor, src0Tensor, src1Scalar, stackTensor, calCount);
}
} // namespace AscendC
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_POWER_POWER_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_POWER_POWER_C310_IMPL_H__
#endif
