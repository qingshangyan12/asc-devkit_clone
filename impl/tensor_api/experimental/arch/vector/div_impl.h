#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning "div_impl.h is an internal header. Include tensor_api/experimental/arch/vector/basic_arithmetic.h instead."
#endif

#ifndef IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_DIV_IMPL_H
#define IMPL_TENSOR_API_EXPERIMENTAL_ARCH_VECTOR_DIV_IMPL_H

#include "c_api/reg_compute/compute/reg_arith.h"
#include "c_api/reg_compute/compute/reg_broadcast.h"
#include "c_api/reg_compute/compute/reg_compare.h"
#include "c_api/reg_compute/compute/reg_fused.h"
#include "c_api/reg_compute/compute/reg_logic.h"
#include "c_api/reg_compute/compute/reg_permute_sel.h"

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

    asc_duplicate_scalar(src0Reg, exponentMask, mask);
    asc_and(src0ExpBits, (vector_uint32_t&)src0, src0Reg, mask);
    asc_shiftright_scalar(src0ExpBits, src0ExpBits, 23, mask);
    asc_add_scalar(src0Exp, (vector_int32_t&)src0ExpBits, -exponentBias, mask);
    asc_lt_scalar(needScaleMask, src0Exp, precisionThreshold, mask);

    asc_duplicate_scalar(thresholdVec, precisionThreshold, mask);
    asc_sub(k, thresholdVec, src0Exp, needScaleMask);
    asc_add_scalar(newExp, k, exponentBias, mask);
    asc_shiftleft_scalar(scaleBits, (vector_int32_t&)newExp, 23, mask);

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
__simd_callee__ inline void div_subnormal(
    vector_float& dst, vector_float src0, vector_float src1, vector_bool mask)
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

}

template <bool precisionMode, div_algo algo, typename T>
__simd_callee__ inline reg_tensor<T> div(const reg_tensor<T>& src0, const reg_tensor<T>& src1)
{
    static_assert(detail::supports_div_v<T>, "div does not support this element type");
    static_assert(
        algo == div_algo::INTRINSIC || algo == div_algo::PRECISION_0ULP_FTZ_TRUE,
        "div does not support this algorithm");
    static_assert(!precisionMode || AscendC::Std::is_same_v<T, float>, "div precision modes only support float");
    static_assert(precisionMode || algo == div_algo::INTRINSIC,
        "div_algo::PRECISION_0ULP_FTZ_TRUE requires precisionMode=true");

    auto mask = src0.mask;
    reg_tensor<T> dst;
    if constexpr (!precisionMode) {
#if defined(__ASC_FTZ__)
        asc_div(dst.reg, src0.reg, src1.reg, mask);
#else
        if constexpr (AscendC::Std::is_same_v<T, float>) {
            detail::div_subnormal<false>(dst.reg, src0.reg, src1.reg, mask);
        } else {
            asc_div(dst.reg, src0.reg, src1.reg, mask);
        }
#endif
    } else {
        if constexpr (algo == div_algo::PRECISION_0ULP_FTZ_TRUE) {
            detail::div_precision(dst.reg, src0.reg, src1.reg, mask, true);
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

}
}
}

#endif
