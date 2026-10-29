/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning \
    "impl/tensor_api/experimental/arch/vector/basic_arithmetic_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"tensor_api/experimental/arch/vector/basic_arithmetic.h\"\" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#include "impl/tensor_api/experimental/arch/utils/reg_utils.h"
#include "tensor_api/experimental/arch/vector/mask_reg_compute.h"

namespace asc {
namespace te {
namespace experimental {
namespace detail {

__simd_callee__ inline vector_float div_flush_subnormal(vector_float src, vector_bool mask)
{
    vector_uint32_t bits;
    vector_uint32_t exponent;
    vector_uint32_t exponentMask;
    vector_uint32_t sign;
    vector_uint32_t zero;
    vector_bool exponentIsZero;
    vector_float dst;
    asc_duplicate_scalar(exponentMask, 0x7F800000u, mask);
    asc_duplicate_scalar(sign, 0x80000000u, mask);
    asc_duplicate_scalar(zero, 0u, mask);
    asc_and(exponent, (vector_uint32_t&)src, exponentMask, mask);
    asc_eq(exponentIsZero, exponent, zero, mask);
    asc_and(bits, (vector_uint32_t&)src, sign, mask);
    asc_select(dst, (vector_float&)bits, src, exponentIsZero);
    return dst;
}

__simd_callee__ inline void div_precision(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask, bool forceFtz)
{
    constexpr uint32_t infNanBound = 0xff800000u;
    constexpr uint32_t signBit = 0x80000000u;
    constexpr uint32_t exponentMask = 0x7F800000u;
    constexpr int32_t exponentBias = 127;
    constexpr int32_t precisionThreshold = -64;

    if (forceFtz) {
        src0 = div_flush_subnormal(src0, mask);
        src1 = div_flush_subnormal(src1, mask);
    }

    vector_float z;
    vector_float tmpDst;
    vector_float r;
    vector_float y;
    vector_float rPre;
    vector_float rNext;
    vector_float zPre;
    vector_float zNext;
    vector_float signVector;
    vector_uint32_t infNan;
    vector_uint32_t src0Reg;
    vector_uint32_t src0ExpBits;
    vector_uint32_t scaleBits;
    vector_int32_t src0Exp;
    vector_int32_t k;
    vector_int32_t thresholdVec;
    vector_int32_t newExp;
    vector_bool cmpMask;
    vector_bool infNanMask;
    vector_bool zeroMask;
    vector_bool needScaleMask;

    asc_duplicate_scalar((vector_uint32_t&)signVector, signBit, mask);
    asc_div(z, src0, src1, mask);
    asc_or(infNan, (vector_uint32_t&)z, (vector_uint32_t&)signVector, mask);
    tmpDst = z;
    asc_eq_scalar(zeroMask, z, 0.0f, mask);
    asc_ge_scalar(infNanMask, (vector_uint32_t&)infNan, infNanBound, mask);
    asc_or(infNanMask, infNanMask, zeroMask, mask);

    asc_duplicate_scalar(src0Reg, exponentMask, mask);
    asc_and(src0ExpBits, (vector_uint32_t&)src0, src0Reg, mask);
    asc_shiftright_scalar(src0ExpBits, src0ExpBits, 23, mask);
    asc_add_scalar(src0Exp, (vector_int32_t&)src0ExpBits, -exponentBias, mask);
    asc_lt_scalar(needScaleMask, src0Exp, precisionThreshold, mask);

    asc_duplicate_scalar(thresholdVec, precisionThreshold, mask);
    asc_sub(k, thresholdVec, src0Exp, needScaleMask);
    asc_add_scalar(newExp, k, exponentBias, mask);
    asc_shiftleft_scalar(scaleBits, (vector_uint32_t&)newExp, 23, mask);

    asc_mul((vector_float&)src0Reg, src0, (vector_float&)scaleBits, mask);
    asc_mul((vector_float&)src0ExpBits, src1, (vector_float&)scaleBits, mask);
    vector_float aScaled = (vector_float&)src0Reg;
    vector_float bScaled = (vector_float&)src0ExpBits;

    asc_mul_scalar(y, bScaled, -1.0f, mask);
    r = aScaled;
    asc_mula(r, z, y, mask);
    rPre = aScaled;
    rNext = aScaled;
    asc_add_scalar((vector_int32_t&)zPre, (vector_int32_t&)z, -1, mask);
    asc_add_scalar((vector_int32_t&)zNext, (vector_int32_t&)z, 1, mask);
    asc_mula(rPre, zPre, y, mask);
    asc_mula(rNext, zNext, y, mask);
    asc_abs(r, r, mask);
    asc_abs(rPre, rPre, mask);
    asc_abs(rNext, rNext, mask);
    asc_lt(cmpMask, r, rPre, mask);
    asc_select(r, r, rPre, cmpMask);
    asc_select(z, z, zPre, cmpMask);
    asc_lt(cmpMask, rNext, r, mask);
    asc_select(z, zNext, z, cmpMask);
    asc_select(dst, tmpDst, z, infNanMask);
}

__simd_callee__ inline void div_sticky(
    vector_bool& stickyPositive, vector_bool& stickyZero, vector_float q, vector_float src0, vector_float src1,
    vector_bool mask)
{
    vector_float negSrc1;
    vector_float residual;
    vector_uint32_t signMask;
    vector_uint32_t src0Sign;
    vector_uint32_t zeroBits;
    vector_bool residualGtZero;
    vector_bool residualLtZero;
    vector_bool src0Positive;
    vector_bool src0Negative;
    vector_bool positiveByPositiveSrc;
    vector_bool positiveByNegativeSrc;

    asc_mul_scalar(negSrc1, src1, -1.0f, mask);
    residual = src0;
    asc_mula(residual, q, negSrc1, mask);
    asc_gt_scalar(residualGtZero, residual, 0.0f, mask);
    asc_lt_scalar(residualLtZero, residual, 0.0f, mask);
    asc_eq_scalar(stickyZero, residual, 0.0f, mask);
    asc_duplicate_scalar(signMask, 0x80000000u, mask);
    asc_and(src0Sign, (vector_uint32_t&)src0, signMask, mask);
    asc_duplicate_scalar(zeroBits, 0u, mask);
    asc_eq(src0Positive, src0Sign, zeroBits, mask);
    asc_not(src0Negative, src0Positive, mask);
    asc_and(positiveByPositiveSrc, residualGtZero, src0Positive, mask);
    asc_and(positiveByNegativeSrc, residualLtZero, src0Negative, mask);
    asc_or(stickyPositive, positiveByPositiveSrc, positiveByNegativeSrc, mask);
}

template <bool useSticky>
__simd_callee__ inline void div_scale_round(
    vector_float& dst, vector_int32_t scale, vector_bool stickyPositive, vector_bool stickyZero, vector_bool mask)
{
    constexpr uint32_t signExtractor = 0x80000000u;
    constexpr uint32_t exponentExtractor = 0x7F800000u;
    constexpr uint32_t mantissaMask = 0x007FFFFFu;
    constexpr uint32_t implicitBit = 0x00800000u;
    constexpr uint32_t inf = 0x7F800000u;

    vector_uint32_t qSign;
    vector_uint32_t qExponent;
    vector_uint32_t qMantissa;
    vector_uint32_t m24;
    vector_uint32_t resultBits;
    vector_uint32_t tmpU0;
    vector_uint32_t tmpU1;
    vector_uint32_t tmpU2;
    vector_uint32_t oneU;
    vector_uint32_t zeroU;
    vector_int32_t e;
    vector_int32_t drop;
    vector_int32_t dropMinusOne;
    vector_int32_t tmpI0;
    vector_int32_t tmpI1;
    vector_bool overflowMask;
    vector_bool notOverflowMask;
    vector_bool normalMask;
    vector_bool subnormalMask;
    vector_bool zeroMask;
    vector_bool subnormalRoundMask;
    vector_bool remGtHalfMask;
    vector_bool remEqHalfMask;
    vector_bool keptOddMask;
    vector_bool tieUpMask;
    vector_bool roundUpMask;

    asc_duplicate_scalar(tmpU0, signExtractor, mask);
    asc_and(qSign, (vector_uint32_t&)dst, tmpU0, mask);
    asc_duplicate_scalar(tmpU0, exponentExtractor, mask);
    asc_and(qExponent, (vector_uint32_t&)dst, tmpU0, mask);
    asc_shiftright_scalar(qExponent, qExponent, 23, mask);
    asc_duplicate_scalar(tmpI0, 127, mask);
    asc_sub(e, (vector_int32_t&)qExponent, tmpI0, mask);
    asc_add(e, e, scale, mask);
    asc_duplicate_scalar(tmpU0, mantissaMask, mask);
    asc_and(qMantissa, (vector_uint32_t&)dst, tmpU0, mask);
    asc_duplicate_scalar(tmpU0, implicitBit, mask);
    asc_or(m24, qMantissa, tmpU0, mask);

    asc_gt_scalar(overflowMask, e, 127, mask);
    asc_duplicate_scalar(tmpU0, inf, overflowMask);
    asc_or(resultBits, qSign, tmpU0, overflowMask);
    asc_select((vector_uint32_t&)dst, resultBits, (vector_uint32_t&)dst, overflowMask);
    asc_not(notOverflowMask, overflowMask, mask);
    asc_ge_scalar(normalMask, e, -126, notOverflowMask);
    asc_add_scalar(tmpI1, e, 127, normalMask);
    asc_shiftleft_scalar(tmpU0, (vector_uint32_t&)tmpI1, 23, normalMask);
    asc_or(resultBits, qSign, tmpU0, normalMask);
    asc_or(resultBits, resultBits, qMantissa, normalMask);
    asc_select((vector_uint32_t&)dst, resultBits, (vector_uint32_t&)dst, normalMask);

    asc_not(subnormalMask, normalMask, notOverflowMask);
    asc_mul_scalar(tmpI0, (vector_int32_t&)e, -1, subnormalMask);
    asc_add_scalar(tmpI0, tmpI0, -126, subnormalMask);
    drop = tmpI0;
    asc_gt_scalar(zeroMask, drop, 24, subnormalMask);
    asc_select((vector_uint32_t&)dst, qSign, (vector_uint32_t&)dst, zeroMask);
    asc_not(subnormalRoundMask, zeroMask, subnormalMask);
    asc_duplicate_scalar(oneU, 1u, subnormalRoundMask);
    asc_duplicate_scalar(zeroU, 0u, subnormalRoundMask);
    asc_shiftright(tmpU0, m24, (vector_int32_t&)drop, subnormalRoundMask);
    asc_shiftleft(tmpU1, oneU, (vector_int32_t&)drop, subnormalRoundMask);
    asc_sub(tmpU1, tmpU1, oneU, subnormalRoundMask);
    asc_and(tmpU2, m24, tmpU1, subnormalRoundMask);
    asc_add_scalar(dropMinusOne, drop, -1, subnormalRoundMask);
    asc_shiftleft(tmpU1, oneU, (vector_int32_t&)dropMinusOne, subnormalRoundMask);
    asc_gt(remGtHalfMask, tmpU2, tmpU1, subnormalRoundMask);
    asc_eq(remEqHalfMask, tmpU2, tmpU1, subnormalRoundMask);
    asc_and(tmpU2, tmpU0, oneU, subnormalRoundMask);
    asc_ne(keptOddMask, tmpU2, zeroU, subnormalRoundMask);
    if constexpr (useSticky) {
        vector_bool stickyZeroOddMask;
        asc_and(stickyZeroOddMask, stickyZero, keptOddMask, subnormalRoundMask);
        asc_or(tieUpMask, stickyPositive, stickyZeroOddMask, subnormalRoundMask);
        asc_and(tieUpMask, tieUpMask, remEqHalfMask, subnormalRoundMask);
    } else {
        asc_and(tieUpMask, remEqHalfMask, keptOddMask, subnormalRoundMask);
    }
    asc_or(roundUpMask, remGtHalfMask, tieUpMask, subnormalRoundMask);
    asc_add(tmpU2, tmpU0, oneU, roundUpMask);
    asc_select(tmpU0, tmpU2, tmpU0, roundUpMask);
    asc_or(resultBits, qSign, tmpU0, subnormalRoundMask);
    asc_select((vector_uint32_t&)dst, resultBits, (vector_uint32_t&)dst, subnormalRoundMask);
}

template <bool useSticky>
__simd_callee__ inline void div_subnormal(vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
{
    constexpr uint32_t exponentExtractor = 0x807FFFFF;
    constexpr uint32_t exponentNormalizer = 0x3F800000;
    constexpr uint32_t inf = 0x7F800000;
    constexpr uint32_t nan = 0x7FC00000;
    constexpr float minNormal = 0x1p-126f;
    constexpr float normalizeScale = 0x1p23f;

    vector_float src0Abs;
    vector_float src1Abs;
    vector_float src0Subnormal;
    vector_float src1Subnormal;
    vector_float src0All;
    vector_float src1All;
    vector_float src0Norm;
    vector_float src1Norm;
    vector_float minNormalReg;
    vector_uint32_t tmp0;
    vector_uint32_t src0Exponent;
    vector_uint32_t src1Exponent;
    vector_int32_t scale;
    vector_int32_t tmp1;
    vector_bool maskSrc0Normal;
    vector_bool maskSrc0Subnormal;
    vector_bool maskSrc1Normal;
    vector_bool maskSrc1Subnormal;
    vector_bool maskTmp;
    vector_bool maskNan;
    vector_bool maskInf;
    vector_bool maskSrc0Zero;
    vector_bool maskSrc1Zero;
    vector_bool maskValid;
    vector_bool stickyPositive;
    vector_bool stickyZero;

    asc_duplicate_scalar(minNormalReg, minNormal, mask);
    asc_abs(src0Abs, src0, mask);
    asc_abs(src1Abs, src1, mask);
    asc_duplicate_scalar(tmp0, inf, mask);
    asc_eq(maskInf, (vector_uint32_t&)src0Abs, tmp0, mask);
    asc_eq(maskTmp, (vector_uint32_t&)src1Abs, tmp0, mask);
    asc_or(maskValid, maskInf, maskTmp, mask);
    asc_duplicate_scalar(tmp0, 0u, mask);
    asc_eq(maskSrc0Zero, (vector_uint32_t&)src0Abs, tmp0, mask);
    asc_or(maskValid, maskValid, maskSrc0Zero, mask);
    asc_eq(maskSrc1Zero, (vector_uint32_t&)src1Abs, tmp0, mask);
    asc_or(maskValid, maskValid, maskSrc1Zero, mask);
    asc_not(maskValid, maskValid, mask);

    asc_lt(maskSrc0Subnormal, src0Abs, minNormalReg, mask);
    asc_not(maskSrc0Normal, maskSrc0Subnormal, mask);
    asc_mul_scalar(src0Subnormal, src0, normalizeScale, maskSrc0Subnormal);
    asc_lt(maskSrc1Subnormal, src1Abs, minNormalReg, mask);
    asc_not(maskSrc1Normal, maskSrc1Subnormal, mask);
    asc_mul_scalar(src1Subnormal, src1, normalizeScale, maskSrc1Subnormal);
    asc_select(src0All, src0, src0Subnormal, maskSrc0Normal);
    asc_select(src1All, src1, src1Subnormal, maskSrc1Normal);

    asc_duplicate_scalar(tmp0, exponentExtractor, mask);
    asc_and((vector_uint32_t&)src0Norm, (vector_uint32_t&)src0All, tmp0, maskValid);
    asc_and((vector_uint32_t&)src1Norm, (vector_uint32_t&)src1All, tmp0, maskValid);
    asc_duplicate_scalar(tmp0, exponentNormalizer, mask);
    asc_add((vector_uint32_t&)src0Norm, (vector_uint32_t&)src0Norm, tmp0, maskValid);
    asc_add((vector_uint32_t&)src1Norm, (vector_uint32_t&)src1Norm, tmp0, maskValid);
    asc_select(src0Norm, src0Norm, src0All, maskValid);
    asc_select(src1Norm, src1Norm, src1All, maskValid);

    if constexpr (useSticky) {
        div_precision(dst, src0Norm, src1Norm, mask, false);
        div_sticky(stickyPositive, stickyZero, dst, src0Norm, src1Norm, maskValid);
    } else {
        asc_div(dst, src0Norm, src1Norm, mask);
    }

    asc_duplicate_scalar(tmp0, inf, mask);
    asc_and(src0Exponent, (vector_uint32_t&)src0All, tmp0, mask);
    asc_and(src1Exponent, (vector_uint32_t&)src1All, tmp0, mask);
    asc_shiftright_scalar(src0Exponent, src0Exponent, 23, mask);
    asc_shiftright_scalar(src1Exponent, src1Exponent, 23, mask);
    asc_sub(scale, (vector_int32_t&)src0Exponent, (vector_int32_t&)src1Exponent, mask);
    asc_add_scalar(tmp1, scale, -23, maskSrc0Subnormal);
    asc_select(scale, tmp1, scale, maskSrc0Subnormal);
    asc_add_scalar(tmp1, scale, 23, maskSrc1Subnormal);
    asc_select(scale, tmp1, scale, maskSrc1Subnormal);
    div_scale_round<useSticky>(dst, scale, stickyPositive, stickyZero, maskValid);
    asc_ne(maskNan, src0Abs, src0Abs, mask);
    asc_ne(maskTmp, src1Abs, src1Abs, mask);
    asc_or(maskNan, maskNan, maskTmp, mask);
    asc_duplicate_scalar((vector_uint32_t&)tmp0, nan, mask);
    asc_select(dst, (vector_float&)tmp0, dst, maskNan);
}

__simd_callee__ inline half div_half_from_bits(uint16_t bits)
{
    union half_bits {
        half value;
        uint16_t bits;
    } value;
    value.bits = bits;
    return value.value;
}

__simd_callee__ inline void div_subnormal_half(vector_half& dst, vector_half src0, vector_half src1, vector_bool mask)
{
    constexpr uint16_t exponentExtractor = 0x83FF;
    constexpr uint16_t signExtractor = 0x8000;
    constexpr uint16_t exponentNormalizer = 0x3C00;
    constexpr uint16_t f16Inf = 0x7C00;
    constexpr uint16_t nanBits = 0x7E00;
    constexpr uint16_t minDenormal = 0x0001;
    constexpr uint16_t scaleEnlargeBits = 0x6400;
    constexpr uint16_t scaleReduceBits = 0x1400;

    vector_half maxSubnormal;
    vector_uint16_t tmp0;
    vector_int16_t tmp1;
    vector_uint16_t tmp2;
    vector_half src0Abs;
    vector_half src0Subnormal;
    vector_half src0Norm;
    vector_half src0All;
    vector_half src0AbsNorm;
    vector_half src1Abs;
    vector_half src1Subnormal;
    vector_half src1Norm;
    vector_half src1All;
    vector_half src1AbsNorm;
    vector_bool mask0;
    vector_bool maskSrc0Normal;
    vector_bool maskSrc0Subnormal;
    vector_bool maskSrc1Normal;
    vector_bool maskSrc1Subnormal;
    vector_bool maskTmp;
    vector_bool maskNan;
    vector_bool maskInf;
    vector_bool maskSrc0Zero;
    vector_bool maskSrc1Zero;
    vector_bool maskValid;
    vector_bool maskNorm;
    vector_uint16_t src0Exponent;
    vector_uint16_t src1Exponent;
    vector_half z1;
    vector_half z2;
    vector_int16_t scale;
    vector_uint16_t dstExponent;
    vector_uint16_t dstSign;

    const half subnormalThreshold = div_half_from_bits(0x0400);
    const half normalizeScaleEnlarge = div_half_from_bits(scaleEnlargeBits);
    const half normalizeScaleReduce = div_half_from_bits(scaleReduceBits);

    asc_duplicate_scalar(maxSubnormal, subnormalThreshold, mask);
    asc_abs(src0Abs, src0, mask);
    asc_abs(src1Abs, src1, mask);
    asc_duplicate_scalar(tmp0, f16Inf, mask);
    asc_eq(maskInf, (vector_uint16_t&)src0Abs, tmp0, mask);
    asc_eq(maskTmp, (vector_uint16_t&)src1Abs, tmp0, mask);
    asc_or(maskValid, maskInf, maskTmp, mask);
    asc_duplicate_scalar(tmp0, 0u, mask);
    asc_eq(maskSrc0Zero, (vector_uint16_t&)src0Abs, tmp0, mask);
    asc_or(maskValid, maskValid, maskSrc0Zero, mask);
    asc_eq(maskSrc1Zero, (vector_uint16_t&)src1Abs, tmp0, mask);
    asc_or(maskValid, maskValid, maskSrc1Zero, mask);
    asc_not(maskValid, maskValid, mask);

    asc_lt(maskSrc0Subnormal, src0Abs, maxSubnormal, mask);
    asc_not(maskSrc0Normal, maskSrc0Subnormal, mask);
    asc_mul_scalar(src0Subnormal, src0, normalizeScaleEnlarge, maskSrc0Subnormal);
    asc_lt(maskSrc1Subnormal, src1Abs, maxSubnormal, mask);
    asc_not(maskSrc1Normal, maskSrc1Subnormal, mask);
    asc_mul_scalar(src1Subnormal, src1, normalizeScaleEnlarge, maskSrc1Subnormal);
    asc_select(src0All, src0, src0Subnormal, maskSrc0Normal);
    asc_select(src1All, src1, src1Subnormal, maskSrc1Normal);

    asc_duplicate_scalar(tmp0, exponentExtractor, mask);
    asc_and((vector_uint16_t&)src0Norm, (vector_uint16_t&)src0All, tmp0, maskValid);
    asc_and((vector_uint16_t&)src1Norm, (vector_uint16_t&)src1All, tmp0, maskValid);
    asc_duplicate_scalar(tmp0, exponentNormalizer, mask);
    asc_add((vector_uint16_t&)src0Norm, (vector_uint16_t&)src0Norm, tmp0, maskValid);
    asc_add((vector_uint16_t&)src1Norm, (vector_uint16_t&)src1Norm, tmp0, maskValid);
    asc_select(src0Norm, src0Norm, src0All, maskValid);
    asc_select(src1Norm, src1Norm, src1All, maskValid);
    asc_abs(src0AbsNorm, src0Norm, maskValid);
    asc_abs(src1AbsNorm, src1Norm, maskValid);
    asc_le(maskNorm, src0AbsNorm, src1AbsNorm, maskValid);

    asc_div(dst, src0Norm, src1Norm, mask);
    asc_and(mask0, maskSrc0Subnormal, maskSrc1Normal, mask);
    asc_mul_scalar(z1, dst, normalizeScaleReduce, mask0);
    asc_select(dst, z1, dst, mask0);
    asc_and(mask0, maskSrc0Normal, maskSrc1Subnormal, mask);
    asc_mul_scalar(z1, dst, normalizeScaleEnlarge, mask0);
    asc_select(dst, z1, dst, mask0);

    asc_duplicate_scalar(tmp0, signExtractor, mask);
    asc_and(dstSign, (vector_uint16_t&)dst, tmp0, mask);

    asc_duplicate_scalar(tmp0, f16Inf, mask);
    asc_and(src0Exponent, (vector_uint16_t&)src0All, tmp0, mask);
    asc_and(src1Exponent, (vector_uint16_t&)src1All, tmp0, mask);
    asc_and(dstExponent, (vector_uint16_t&)dst, tmp0, mask);
    asc_shiftright_scalar(src0Exponent, src0Exponent, 10, mask);
    asc_shiftright_scalar(src1Exponent, src1Exponent, 10, mask);
    asc_shiftright_scalar(dstExponent, dstExponent, 10, mask);
    asc_sub(scale, (vector_int16_t&)src0Exponent, (vector_int16_t&)src1Exponent, mask);
    asc_add_scalar(scale, scale, 15, mask);

    asc_duplicate_scalar(tmp1, static_cast<int16_t>(-9), mask);
    asc_eq(mask0, scale, tmp1, mask);
    asc_and(mask0, mask0, maskValid, mask);
    asc_duplicate_scalar(tmp0, minDenormal, mask0);
    asc_add((vector_uint16_t&)z1, dstSign, tmp0, mask0);
    asc_duplicate_scalar(tmp2, 0u, mask0);
    asc_add((vector_uint16_t&)z2, dstSign, tmp2, mask0);
    asc_select(z1, z2, z1, maskNorm);
    asc_select(dst, z1, dst, mask0);
    asc_not(mask0, mask0, mask);
    asc_and(maskValid, mask0, maskValid, mask);

    asc_lt_scalar(mask0, scale, static_cast<int16_t>(-9), mask);
    asc_and(mask0, mask0, maskValid, mask);
    asc_duplicate_scalar(tmp0, 0u, mask);
    asc_add((vector_uint16_t&)z1, dstSign, tmp0, mask0);
    asc_select(dst, z1, dst, mask0);
    asc_not(mask0, mask0, mask);
    asc_and(maskValid, mask0, maskValid, mask);

    asc_duplicate_scalar(tmp0, 31u, mask);
    asc_eq(mask0, scale, (vector_int16_t&)tmp0, mask);
    asc_and(mask0, mask0, maskValid, mask);
    asc_duplicate_scalar(tmp1, 1, mask0);
    asc_sub(tmp1, scale, tmp1, mask0);
    asc_select(scale, tmp1, scale, mask0);
    asc_mul_scalar(z1, dst, div_half_from_bits(0x4000), mask0);
    asc_select(dst, z1, dst, mask0);

    asc_gt(mask0, scale, (vector_int16_t&)tmp0, mask);
    asc_and(mask0, mask0, maskValid, mask);
    asc_duplicate_scalar(tmp0, f16Inf, mask);
    asc_add((vector_uint16_t&)z1, dstSign, tmp0, mask0);
    asc_select(dst, z1, dst, mask0);
    asc_not(mask0, mask0, mask);
    asc_and(maskValid, mask0, maskValid, mask);

    asc_duplicate_scalar(tmp0, 0u, maskValid);
    asc_gt(mask0, scale, (vector_int16_t&)tmp0, maskValid);
    asc_shiftleft_scalar(tmp1, scale, 10, mask0);
    asc_mul(z1, dst, (vector_half&)tmp1, mask0);
    asc_select(dst, z1, dst, mask0);
    asc_not(mask0, mask0, maskValid);
    asc_duplicate_scalar(tmp0, 0x0200u, mask0);
    asc_abs(scale, scale, mask0);
    asc_shiftright((vector_uint16_t&)scale, tmp0, (vector_int16_t&)scale, mask0);
    asc_mul(z1, dst, (vector_half&)scale, mask0);
    asc_select(dst, z1, dst, mask0);

    asc_ne(maskNan, src0Abs, src0Abs, mask);
    asc_ne(maskTmp, src1Abs, src1Abs, mask);
    asc_or(maskNan, maskNan, maskTmp, mask);
    asc_duplicate_scalar(tmp0, nanBits, mask);
    asc_select(dst, (vector_half&)tmp0, dst, maskNan);
}

} // namespace detail

template <typename T>
__simd_callee__ inline reg_tensor<T> ln(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "ln does not support this element type");
    reg_tensor<T> dst;
    asc_ln(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> log(const reg_tensor<T>& src)
{
    return ln(src);
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_add_sub_v<T>, "operator+ does not support this element type");
    reg_tensor<T> dst;
    asc_add(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_add_sub_v<T>, "scalar operator+ does not support this element type");
    reg_tensor<T> dst;
    asc_add_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator+(const T& scalar, const reg_tensor<T>& src)
{
    static_assert(detail::supports_add_sub_v<T>, "scalar operator+ does not support this element type");
    reg_tensor<T> dst;
    asc_add_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> neg(const reg_tensor<T>& src)
{
    static_assert(detail::supports_neg_v<T>, "neg does not support this element type");
    reg_tensor<T> dst;
    asc_neg(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_add_sub_v<T>, "operator- does not support this element type");
    reg_tensor<T> dst;
    asc_sub(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_add_sub_v<T>, "scalar operator- does not support this element type");
    auto scalar_reg = detail::make_reg_operand<T>(scalar, src.mask);
    reg_tensor<T> dst;
    asc_sub(dst.reg, src.reg, scalar_reg.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator-(const T& scalar, const reg_tensor<T>& src)
{
    static_assert(detail::supports_add_sub_v<T>, "scalar operator- does not support this element type");
    auto scalar_reg = detail::make_reg_operand<T>(scalar, src.mask);
    reg_tensor<T> dst;
    asc_sub(dst.reg, scalar_reg.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_mul_v<T>, "operator* does not support this element type");
    reg_tensor<T> dst;
    asc_mul(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_mul_scalar_v<T>, "scalar operator* does not support this element type");
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator*(const T& scalar, const reg_tensor<T>& src)
{
    static_assert(detail::supports_mul_scalar_v<T>, "scalar operator* does not support this element type");
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_min_max_v<T>, "max does not support this element type");
    reg_tensor<T> dst;
    asc_max(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_min_max_v<T>, "max does not support this element type");
    reg_tensor<T> dst;
    asc_max_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> max(const T& scalar, const reg_tensor<T>& src)
{
    return max(src, scalar);
}

template <typename T>
__simd_callee__ inline reg_tensor<T> abs(const reg_tensor<T>& src)
{
    static_assert(detail::supports_abs_v<T>, "abs does not support this element type");
    reg_tensor<T> dst;
    asc_abs(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> exp(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "exp supports half and float");
    reg_tensor<T> dst;
    asc_exp(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> sqrt(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "sqrt supports half and float");
    reg_tensor<T> dst;
    asc_sqrt(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> log2(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "log2 does not support this element type");
    constexpr float ln2_reciprocal = 1.4426950408889634;
    if constexpr (AscendC::Std::is_same_v<T, float>) {
        reg_tensor<T> natural_log;
        asc_ln(natural_log.reg, src.reg, src.mask);
        reg_tensor<T> dst;
        asc_mul_scalar(dst.reg, natural_log.reg, ln2_reciprocal, src.mask);
        dst.mask = src.mask;
        return dst;
    } else {
        vector_half f16_reg_low;
        vector_half f16_reg_high;
        vector_float f32_reg_low;
        vector_float f32_reg_high;
        vector_half tmp_reg;
        vector_bool mask_low;
        vector_bool mask_high;
        const auto mask_all = all_mask<float>();

        asc_unpack_lower(mask_low, src.mask);
        asc_unpack_upper(mask_high, src.mask);

        asc_intlv(f16_reg_low, f16_reg_high, src.reg, src.reg);

        f32_reg_low = asc_half2float(f16_reg_low, mask_all.reg, ASC_POSITION_EVEN);
        f32_reg_high = asc_half2float(f16_reg_high, mask_all.reg, ASC_POSITION_EVEN);

        asc_ln(f32_reg_low, f32_reg_low, mask_all.reg);
        asc_ln(f32_reg_high, f32_reg_high, mask_all.reg);
        asc_mul_scalar(f32_reg_low, f32_reg_low, ln2_reciprocal, mask_all.reg);
        asc_mul_scalar(f32_reg_high, f32_reg_high, ln2_reciprocal, mask_all.reg);

        f16_reg_low = asc_float2half_rn(f32_reg_low, mask_low, ASC_POSITION_EVEN);
        f16_reg_high = asc_float2half_rn(f32_reg_high, mask_high, ASC_POSITION_EVEN);

        asc_deintlv(f16_reg_low, tmp_reg, f16_reg_low, tmp_reg);
        asc_deintlv(f16_reg_high, tmp_reg, tmp_reg, f16_reg_high);

        reg_tensor<T> dst;
        const auto select_mask = make_mask<mask_pattern::half, half>();
        asc_select(dst.reg, f16_reg_low, f16_reg_high, select_mask.reg);
        dst.mask = src.mask;
        return dst;
    }
}

template <typename T>
__simd_callee__ inline reg_tensor<T> log10(const reg_tensor<T>& src)
{
    static_assert(detail::supports_float_math_v<T>, "log10 does not support this element type");
    const T ln10_reciprocal = static_cast<T>(0.43429448190325176);
    reg_tensor<T> natural_log;
    asc_ln(natural_log.reg, src.reg, src.mask);
    reg_tensor<T> dst;
    asc_mul_scalar(dst.reg, natural_log.reg, ln10_reciprocal, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> relu(const reg_tensor<T>& src)
{
    static_assert(detail::supports_relu_v<T>, "relu supports half, int32_t, and float");
    reg_tensor<T> dst;
    asc_relu(dst.reg, src.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> prelu(const reg_tensor<T>& src, const reg_tensor<T>& slope)
{
    static_assert(detail::supports_float_math_v<T>, "prelu supports half and float");
    reg_tensor<T> dst;
    asc_prelu(dst.reg, src.reg, slope.reg, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> leaky_relu(const reg_tensor<T>& src, const T& slope)
{
    static_assert(detail::supports_float_math_v<T>, "leaky_relu does not support this element type");
    reg_tensor<T> dst;
    asc_leakyrelu(dst.reg, src.reg, slope, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_pair<T, bool> add_carry(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "add_carry supports int32_t and uint32_t");
    reg_pair<T, bool> result;
    asc_add(result.second.reg, result.first.reg, src0.reg, src1.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_pair<T, bool> add_carry(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "add_carry supports int32_t and uint32_t");
    reg_pair<T, bool> result;
    asc_addc(result.second.reg, result.first.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_pair<T, bool> sub_carry(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_carry_v<T>, "sub_carry supports int32_t and uint32_t");
    reg_pair<T, bool> result;
    asc_sub(result.second.reg, result.first.reg, src0.reg, src1.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_pair<T, bool> sub_carry(
    const reg_tensor<T>& src0, const reg_tensor<T>& src1, const reg_tensor<bool>& carry_src)
{
    static_assert(detail::supports_carry_v<T>, "sub_carry supports int32_t and uint32_t");
    reg_pair<T, bool> result;
    asc_subc(result.second.reg, result.first.reg, src0.reg, src1.reg, carry_src.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_pair<T> mull(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_mull_v<T>, "mull does not support this element type");
    reg_pair<T> result;
    asc_mull(result.first.reg, result.second.reg, src0.reg, src1.reg, src0.mask);
    result.first.mask = src0.mask;
    result.second.mask = src0.mask;
    return result;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> operator/(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_div_v<T>, "operator/ does not support this element type");
    reg_tensor<T> dst;
    asc_div(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <bool precisionMode, div_algo algo, typename T>
__simd_callee__ inline reg_tensor<T> div(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_div_v<T>, "div does not support this element type");
    static_assert(
        algo == div_algo::INTRINSIC || algo == div_algo::PRECISION_0ULP_FTZ_TRUE,
        "div does not support this algorithm");
    static_assert(!precisionMode || AscendC::Std::is_same_v<T, float>, "div precision modes only support float");
    static_assert(
        precisionMode || algo == div_algo::INTRINSIC, "div_algo::PRECISION_0ULP_FTZ_TRUE requires precisionMode=true");

    auto mask = src0.mask;
    reg_tensor<T> dst;
    if constexpr (!precisionMode) {
#if defined(__ASC_FTZ__)
        asc_div(dst.reg, src0.reg, src1.reg, mask);
#else
        if constexpr (AscendC::Std::is_same_v<T, float>) {
            detail::div_subnormal<false>(dst.reg, src0.reg, src1.reg, mask);
        } else if constexpr (AscendC::Std::is_same_v<T, half>) {
            detail::div_subnormal_half(dst.reg, src0.reg, src1.reg, mask);
        } else {
            asc_div(dst.reg, src0.reg, src1.reg, mask);
        }
#endif
    } else {
        if constexpr (algo == div_algo::PRECISION_0ULP_FTZ_TRUE) {
            detail::div_precision(dst.reg, src0.reg, src1.reg, mask, false);
        } else {
#if defined(__ASC_FTZ__)
            detail::div_precision(dst.reg, src0.reg, src1.reg, mask, false);
#else
            detail::div_subnormal<true>(dst.reg, src0.reg, src1.reg, mask);
#endif
        }
    }
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min(dst.reg, src0.reg, src1.reg, src0.mask);
    dst.mask = src0.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const reg_tensor<T>& src, const T& scalar)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

template <typename T>
__simd_callee__ inline reg_tensor<T> min(const T& scalar, const reg_tensor<T>& src)
{
    static_assert(detail::supports_min_max_v<T>, "min does not support this element type");
    reg_tensor<T> dst;
    asc_min_scalar(dst.reg, src.reg, scalar, src.mask);
    dst.mask = src.mask;
    return dst;
}

} // namespace experimental
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_BASIC_ARITHMETIC_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_BASIC_ARITHMETIC_IMPL_H
#endif
