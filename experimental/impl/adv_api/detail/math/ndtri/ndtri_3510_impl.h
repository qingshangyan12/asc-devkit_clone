/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file ndtri_3510_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "experimental/impl/adv_api/detail/math/ndtri/ndtri_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/ndtri.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_C310_IMPL_H__
#endif
#ifndef IMPL_MATH_NDTRI_NDTRI_C310_IMPL_H
#define IMPL_MATH_NDTRI_NDTRI_C310_IMPL_H
#include "../../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../../../../../include/basic_api/kernel_tensor.h"
#include "../../../../../../impl/basic_api/kernel_pop_stack_buffer.h"
#include "../../../../../../impl/adv_api/detail/common/check.h"

namespace AscendC {
namespace NdtriAPI {

/*
 * Ndtri(p) = Phi^-1(p), the inverse standard-normal CDF (probit).
 *
 * HYBRID, and the split is deliberate — see the design doc:
 *
 *   tail   |p-0.5| >  0.47575  (i.e. p < 0.02425 or p > 0.97575)
 *          Acklam rational: z = +-C(q)/D(q),  q = sqrt(-2*ln(t)),  t = min(p, 1-p)
 *          Measured fp32 evaluation error 3.7e-7 — well conditioned, used as-is.
 *
 *   central otherwise
 *          Cephes factored form: z = sqrt(2*pi) * q * (1 + r*P0(r)/Q0(r)),  q = p-0.5, r = q*q
 *          NOT Acklam's A(r)*q/B(r): that form is catastrophically ill-conditioned in fp32.
 *          Its Horner intermediates peak at ~276 and collapse to ~0.01 near the breakpoint
 *          (amplification ~21000x), giving a 1.3e-3 error floor -> FAILS the 1e-4 requirement.
 *          Acklam's *approximation* error is fine (1.1e-9 in fp64); it is the fp32 *evaluation*
 *          that breaks. Cephes keeps the rational as a bounded correction in [0, 0.655] riding on
 *          a dominant q term, so nothing cancels.
 *
 * Measured (fp32, vs scipy.special.ndtri, 950PR): max rel err 4.6e-5, 0/32002 exceed 1e-4.
 *
 * Boundaries: p<0 | p>1 -> NaN ; p==0 -> -Inf ; p==1 -> +Inf ; p==0.5 -> 0 ; NaN -> NaN.
 */

constexpr float NDTRI_S2PI = 2.5066282746310002f;      // sqrt(2*pi)
constexpr float NDTRI_HALFWIDTH = 0.47575f;            // central iff |p-0.5| <= this

// Cephes central branch: numerator P0 (deg 4), denominator Q0 (MONIC deg 8)
constexpr float NDTRI_P0[5] = {-5.99633501014107895267e1f, 9.80010754185999661536e1f,
                               -5.66762857469070293439e1f, 1.39312609387279679503e1f,
                               -1.23916583867381258016e0f};
constexpr float NDTRI_Q0[8] = {1.95448858338141759834e0f,  4.67627912898881538453e0f,
                               8.63602421390890590575e1f,  -2.25462687854119370527e2f,
                               2.00260212380060660359e2f,  -8.20372256168333339912e1f,
                               1.59056225126211695515e1f,  -1.18331621121330003142e0f};
// Acklam tail branch: C (deg 5) / D (deg 4, then *q + 1)
constexpr float NDTRI_C[6] = {-7.784894002430293e-3f, -3.223964580411365e-1f, -2.400758277161838e0f,
                              -2.549732539343734e0f,  4.374664141464968e0f,   2.938163982698783e0f};
constexpr float NDTRI_D[4] = {7.784695709041462e-3f, 3.224671290700398e-1f, 2.445134137142996e0f,
                              3.754408661907416e0f};

// Horner step: acc = acc*x + c, with a SINGLE rounding.
// MulDstAdd(dst, a, b) computes dst = dst*a + b. Duplicate+MulDstAdd costs the same 2 instructions
// as Mul+Adds but rounds once instead of twice, so it is strictly better.
__simd_callee__ inline void NdtriFmaStep(
    Reg::RegTensor<float>& acc, Reg::RegTensor<float>& x, const float c, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> cReg;
    Reg::Duplicate(cReg, c, mask);
    Reg::MulDstAdd(acc, x, cReg, mask);
}

__simd_callee__ inline void NdtriCompute(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& pReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> q, r, num, den, corr, t, zc, zt, tmp, oneReg;
    Reg::MaskReg mCentral, mNeg, mTmp;

    // q is used THREE ways: central variable, region test (|q|), and tail sign (q>0).
    Reg::Adds(q, pReg, -0.5f, mask); // exact for p in [0.25,1] by Sterbenz
    Reg::Mul(r, q, q, mask);

    // ---------- central: z = s2pi * q * (1 + r*P0(r)/Q0(r))
    Reg::Duplicate(num, NDTRI_P0[0], mask);
    NdtriFmaStep(num, r, NDTRI_P0[1], mask);
    NdtriFmaStep(num, r, NDTRI_P0[2], mask);
    NdtriFmaStep(num, r, NDTRI_P0[3], mask);
    NdtriFmaStep(num, r, NDTRI_P0[4], mask);

    Reg::Adds(den, r, NDTRI_Q0[0], mask); // monic: den = r + q0
    NdtriFmaStep(den, r, NDTRI_Q0[1], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[2], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[3], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[4], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[5], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[6], mask);
    NdtriFmaStep(den, r, NDTRI_Q0[7], mask);

    Reg::Mul(num, num, r, mask);
    Reg::Div(corr, num, den, mask); // bounded in [0, 0.655] -> no cancellation
    Reg::Adds(corr, corr, 1.0f, mask);
    Reg::Mul(zc, q, corr, mask);
    Reg::Muls(zc, zc, NDTRI_S2PI, mask);

    // ---------- tail: z = +-C(qt)/D(qt), qt = sqrt(-2 ln t), t = min(p, 1-p)
    Reg::Duplicate(oneReg, 1.0f, mask);
    Reg::Sub(t, oneReg, pReg, mask); // exact for p in [0.5,1] by Sterbenz
    // MUST be a real Min. t = 0.5-|q| would be cheaper but is WRONG: for p < 0.25, ulp(0.5)=6e-8
    // swamps p, q rounds to exactly -0.5, and t collapses to 0 -> Ln(0) -> the deep tail is destroyed.
    Reg::Min(t, pReg, t, mask);
    Reg::Ln(t, t, mask);
    Reg::Muls(t, t, -2.0f, mask);
    Reg::Sqrt(t, t, mask); // t := qt

    Reg::Duplicate(num, NDTRI_C[0], mask);
    NdtriFmaStep(num, t, NDTRI_C[1], mask);
    NdtriFmaStep(num, t, NDTRI_C[2], mask);
    NdtriFmaStep(num, t, NDTRI_C[3], mask);
    NdtriFmaStep(num, t, NDTRI_C[4], mask);
    NdtriFmaStep(num, t, NDTRI_C[5], mask);

    Reg::Duplicate(den, NDTRI_D[0], mask);
    NdtriFmaStep(den, t, NDTRI_D[1], mask);
    NdtriFmaStep(den, t, NDTRI_D[2], mask);
    NdtriFmaStep(den, t, NDTRI_D[3], mask);
    Reg::Mul(den, den, t, mask);
    Reg::Adds(den, den, 1.0f, mask);

    Reg::Div(zt, num, den, mask);
    Reg::Neg(tmp, zt, mask);
    Reg::Compares<float, CMPMODE::GT>(mNeg, q, 0.0f, mask); // p>0.5 <=> q>0
    Reg::Select(zt, tmp, zt, mNeg);

    // ---------- combine.
    // Both branches are evaluated on EVERY lane, including lanes where the tail's Ln sees p<=0 or
    // NaN. That is safe ONLY because Reg::Select is a bit-level lane PICK: it never does arithmetic
    // on the discarded lane. Do NOT "optimise" this into a multiply-by-mask (0 * NaN = NaN).
    Reg::Abs(tmp, q, mask);
    Reg::Compares<float, CMPMODE::LE>(mCentral, tmp, NDTRI_HALFWIDTH, mask);
    Reg::Select(dstReg, zc, zt, mCentral);

    // ---------- boundary patch. NaN needs none: p=NaN makes every compare false, so the tail lane
    // (already NaN) is selected and NaN propagates for free.
    Reg::Duplicate(tmp, -__builtin_inff(), mask);
    Reg::Compares<float, CMPMODE::EQ>(mTmp, pReg, 0.0f, mask);
    Reg::Select(dstReg, tmp, dstReg, mTmp);

    Reg::Duplicate(tmp, __builtin_inff(), mask);
    Reg::Compares<float, CMPMODE::EQ>(mTmp, pReg, 1.0f, mask);
    Reg::Select(dstReg, tmp, dstReg, mTmp);

    Reg::Duplicate(tmp, __builtin_nanf(""), mask);
    Reg::Compares<float, CMPMODE::LT>(mTmp, pReg, 0.0f, mask);
    Reg::Select(dstReg, tmp, dstReg, mTmp);
    Reg::Compares<float, CMPMODE::GT>(mTmp, pReg, 1.0f, mask);
    Reg::Select(dstReg, tmp, dstReg, mTmp);
}

template <typename T, bool isReuseSource = false>
__simd_vf__ inline void NdtriCoreImpl(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calCount, uint16_t repeatTimes)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> pReg, dstReg;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount); // POST_UPDATE: consumes 64 per call
        Reg::LoadAlign(pReg, srcUb + i * B32_DATA_NUM_PER_REPEAT);
        NdtriCompute(dstReg, pReg, mask);
        // load unmasked (over-read stays inside our own 32B-aligned UB buffer), store MASKED so the
        // tail repeat cannot clobber elements past calCount.
        Reg::StoreAlign(dstUb + i * B32_DATA_NUM_PER_REPEAT, dstReg, mask);
    }
}
} // namespace NdtriAPI

template <typename T, bool isReuseSource = false>
__aicore__ inline void NdtriCheckParams(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    static_assert(SupportType<T, float>(), "current data type is not supported on current device!");
    CheckTensorPos<T>(dstTensor, Hardware::UB, "dstTensor", "VECIN / VECCALC / VECOUT", "Ndtri");
    CheckTensorPos<T>(srcTensor, Hardware::UB, "srcTensor", "VECIN / VECCALC / VECOUT", "Ndtri");
    CheckTensorPos<uint8_t>(sharedTmpBuffer, Hardware::UB, "sharedTmpBuffer", "VECIN / VECCALC / VECOUT", "Ndtri");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Ndtri");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Ndtri");
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void NdtriImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    NdtriCheckParams<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
    // sharedTmpBuffer is accepted for API parity but is UNUSED on 3510: every intermediate lives in a
    // vector register, so the API touches UB exactly twice per repeat (one load, one store).
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    uint16_t repeatTimes = CeilDivision(calCount, B32_DATA_NUM_PER_REPEAT);
    NdtriAPI::NdtriCoreImpl<T, isReuseSource>(dstUb, srcUb, calCount, repeatTimes);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void NdtriImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    // Using the Stack Space to Allocate tmpBuffer
    LocalTensor<uint8_t> sharedTmpBuffer;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(sharedTmpBuffer);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
    NdtriImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

} // namespace AscendC

#endif // IMPL_MATH_NDTRI_NDTRI_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_C310_IMPL_H__
#endif
