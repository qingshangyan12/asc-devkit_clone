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
 * \file bessel_i0_3510_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "experimental/impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/bessel_i0.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H__
#endif
#ifndef IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H
#define IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H
#include "../../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../../../../../include/basic_api/kernel_tensor.h"
#include "../../../../../../impl/basic_api/kernel_pop_stack_buffer.h"
#include "../../../../../../impl/adv_api/detail/common/check.h"

namespace AscendC {
namespace BesselI0API {

/*
 * BesselI0(x) = I0(x), modified Bessel function of the first kind, order 0. Aligns with torch.i0 /
 * prims.bessel_i0. I0 is EVEN, so we work on a = |x| and split at a = 3.75 (Abramowitz & Stegun):
 *
 *   region1  a <= 3.75   9.8.1:  t = (a/3.75)^2 ; I0 = P1(t)                      (approx err < 1.6e-7)
 *   region2  a >  3.75   9.8.2:  t = 3.75/a     ; I0 = exp(a) * P2(t) / sqrt(a)   (approx err < 1.9e-7)
 *
 * region2 uses a SHIFT-FOLD of the exponential:  I0 = exp(a - 3.75) * (P2S(t) / sqrt(a)), where
 * P2S = P2 * e^3.75 is folded into the coefficients at compile time (zero runtime cost). Two reasons:
 *   1. exp(a) alone overflows fp32 (max 3.4e38) at a > 88.72, yet the true I0 stays finite up to
 *      a ~ 91.9 because the P2/sqrt(a) factor is ~0.04. exp(a-3.75) is strictly smaller than I0
 *      throughout region2, so it never overflows before I0 itself does -> the finite band is covered
 *      and matches the (fp64) scipy.special.i0 golden across it.
 *   2. The exponential's relative error enters the result exactly ONCE (1*delta). A split form
 *      exp(a/2) used twice would double it. Measured tolerance (host model, vs scipy): region2 passes
 *      the 1e-4 requirement as long as Reg::Exp is accurate to < ~6e-5; region1 carries no exp.
 *
 * Measured (fp32 host model, vs scipy.special.i0, function-shaped sampling): max rel err 5.4e-7,
 * 0/31001 exceed 1e-4 (~180x margin, assuming a correctly-rounded exp).
 *
 * Boundaries need NO explicit handling -- all emerge naturally from the two branches + lane-pick Select:
 *   x == 0   -> region1 t=0 -> I0 = 1.0 exact
 *   +-Inf    -> region2: P2S/sqrt(a) -> 0, exp(Inf) -> Inf, Inf*0 = NaN  (matches scipy AND torch.i0)
 *   NaN      -> region2 all-NaN
 */

constexpr float BESSEL_I0_BP = 3.75f;          // branch point: region1 iff |x| <= this
constexpr float BESSEL_I0_INV_BP = 0.26666668f; // fp32(1/3.75); region1 t = (a*INV_BP)^2 via Muls

// region1 (A&S 9.8.1): Horner in t=(a/3.75)^2, highest degree first
constexpr float BESSEL_I0_P1[7] = {0.0045813f, 0.0360768f, 0.2659732f, 1.2067492f,
                                   3.0899424f, 3.5156229f, 1.0f};
// region2 (A&S 9.8.2): Horner in t=3.75/a, highest degree first, ALREADY MULTIPLIED BY e^3.75
// (shift-fold). Original A&S coeffs were {3.92377e-3, -1.647633e-2, 2.635537e-2, -2.057706e-2,
// 9.16281e-3, -1.57565e-3, 2.25319e-3, 1.328592e-2, 3.9894228e-1}; each is scaled by e^3.75=42.521084.
constexpr float BESSEL_I0_P2S[9] = {1.66842952e-01f,  -7.00591385e-01f, 1.12065899e+00f,
                                    -8.74958873e-01f, 3.89612585e-01f,  -6.69983402e-02f,
                                    9.58080739e-02f,  5.64931691e-01f,  1.69634590e+01f};

// Horner step: acc = acc*x + c, with a SINGLE rounding.
// MulDstAdd(dst, a, b) computes dst = dst*a + b. Duplicate+MulDstAdd costs the same 2 instructions
// as Mul+Adds but rounds once instead of twice, so it is strictly better.
__simd_callee__ inline void BesselI0FmaStep(
    Reg::RegTensor<float>& acc, Reg::RegTensor<float>& x, const float c, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> cReg;
    Reg::Duplicate(cReg, c, mask);
    Reg::MulDstAdd(acc, x, cReg, mask);
}

__simd_callee__ inline void BesselI0Compute(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> a, t1, t2, r1, r2, poly, sqa, ea, three75, nanReg;
    Reg::MaskReg mRegion1, mInf;

    Reg::Abs(a, xReg, mask); // I0 is even

    // ---------- region1: I0 = P1((a/3.75)^2)
    Reg::Muls(t1, a, BESSEL_I0_INV_BP, mask);
    Reg::Mul(t1, t1, t1, mask); // t1 = (a/3.75)^2
    Reg::Duplicate(r1, BESSEL_I0_P1[0], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[1], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[2], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[3], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[4], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[5], mask);
    BesselI0FmaStep(r1, t1, BESSEL_I0_P1[6], mask);

    // ---------- region2 (shift-fold): I0 = exp(a-3.75) * (P2S(3.75/a) / sqrt(a))
    Reg::Duplicate(three75, BESSEL_I0_BP, mask);
    Reg::Div(t2, three75, a, mask); // t2 = 3.75/a
    Reg::Duplicate(poly, BESSEL_I0_P2S[0], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[1], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[2], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[3], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[4], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[5], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[6], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[7], mask);
    BesselI0FmaStep(poly, t2, BESSEL_I0_P2S[8], mask);

    Reg::Sqrt(sqa, a, mask);
    Reg::Div(poly, poly, sqa, mask); // P2S / sqrt(a)
    Reg::Adds(ea, a, -BESSEL_I0_BP, mask);
    Reg::Exp(ea, ea, mask);          // exp(a - 3.75)
    Reg::Mul(r2, ea, poly, mask);

    // ---------- combine: lane pick (never an arithmetic blend). Both branches are evaluated on every
    // lane, including the a<=3.75 lanes where region2's Div(3.75, 0..) and P2S/sqrt overflow to
    // Inf/NaN. Safe ONLY because Reg::Select never touches the discarded lane. Do NOT turn this into a
    // multiply-by-mask (0 * NaN = NaN).
    Reg::Compares<float, CMPMODE::LE>(mRegion1, a, BESSEL_I0_BP, mask);
    Reg::Select(dstReg, r1, r2, mRegion1);

    // ---------- boundary patch: |x|==Inf -> NaN (matches scipy AND torch.i0). This is EXPLICIT, not
    // natural: the natural region2 value is exp(Inf)*(P2S/sqrt(Inf)) = exp(Inf)*0, which is NaN ONLY IF
    // Reg::Exp(Inf)==+Inf. If the hardware exp CLAMPS to FLT_MAX instead, it becomes FLT_MAX*0 = 0 and
    // the +-Inf lanes would silently return 0. Forcing it here is robust to either exp behaviour. a=|x|
    // so a==+Inf covers both signs; large-but-finite |x| (golden +Inf) still overflows to +Inf in r2.
    // x==0 -> 1 and NaN -> NaN remain natural (region1 exact / region2 NaN propagation).
    Reg::Duplicate(nanReg, __builtin_nanf(""), mask);
    Reg::Compares<float, CMPMODE::EQ>(mInf, a, __builtin_inff(), mask);
    Reg::Select(dstReg, nanReg, dstReg, mInf);
}

template <typename T, bool isReuseSource = false>
__simd_vf__ inline void BesselI0CoreImpl(
    __ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calCount, uint16_t repeatTimes)
{
    Reg::MaskReg mask;
    Reg::RegTensor<float> xReg, dstReg;
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount); // POST_UPDATE: consumes 64 per call
        Reg::LoadAlign(xReg, srcUb + i * B32_DATA_NUM_PER_REPEAT);
        BesselI0Compute(dstReg, xReg, mask);
        // load unmasked (over-read stays inside our own 32B-aligned UB buffer), store MASKED so the
        // tail repeat cannot clobber elements past calCount.
        Reg::StoreAlign(dstUb + i * B32_DATA_NUM_PER_REPEAT, dstReg, mask);
    }
}
} // namespace BesselI0API

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0CheckParams(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    static_assert(SupportType<T, float>(), "current data type is not supported on current device!");
    CheckTensorPos<T>(dstTensor, Hardware::UB, "dstTensor", "VECIN / VECCALC / VECOUT", "BesselI0");
    CheckTensorPos<T>(srcTensor, Hardware::UB, "srcTensor", "VECIN / VECCALC / VECOUT", "BesselI0");
    CheckTensorPos<uint8_t>(sharedTmpBuffer, Hardware::UB, "sharedTmpBuffer", "VECIN / VECCALC / VECOUT", "BesselI0");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "BesselI0");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "BesselI0");
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0Impl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    BesselI0CheckParams<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
    // sharedTmpBuffer is accepted for API parity but is UNUSED on 3510: every intermediate lives in a
    // vector register, so the API touches UB exactly twice per repeat (one load, one store).
    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    uint16_t repeatTimes = CeilDivision(calCount, B32_DATA_NUM_PER_REPEAT);
    BesselI0API::BesselI0CoreImpl<T, isReuseSource>(dstUb, srcUb, calCount, repeatTimes);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0Impl(
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
    BesselI0Impl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

} // namespace AscendC

#endif // IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H__
#endif
