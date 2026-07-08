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
 * \brief dav-3510 Reg implementation for experimental BesselI0 high-level API.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h is an internal header file and must not be used directly. Please use \"#include \"adv_api/math/bessel_i0.h\"\".")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_3510_IMPL_H__
#endif

#ifndef IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H
#define IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H

#include "kernel_operator.h"

namespace AscendC {
namespace BesselI0API {

constexpr uint32_t BESSEL_I0_POS_INF = 0x7F800000U;
constexpr float BESSEL_I0_BOUNDARY = 9.0f;
constexpr float BESSEL_I0_HALF = 0.5f;
constexpr float BESSEL_I0_ONE = 1.0f;

__simd_callee__ inline void FmaScalar(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& mulReg, const float scalar, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> scalarReg;
    Reg::Duplicate(scalarReg, scalar, mask);
    Reg::FusedMulDstAdd(dstReg, mulReg, scalarReg, mask);
}

__simd_callee__ inline void ComputeSmall(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& absReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> squareReg;
    Reg::Mul(squareReg, absReg, absReg, mask);

    Reg::Duplicate(dstReg, 1.551427e-19f, mask);
    FmaScalar(dstReg, squareReg, 1.4492505e-17f, mask);
    FmaScalar(dstReg, squareReg, 1.0687647e-14f, mask);
    FmaScalar(dstReg, squareReg, 2.3349575e-12f, mask);
    FmaScalar(dstReg, squareReg, 4.7306625e-10f, mask);
    FmaScalar(dstReg, squareReg, 6.7778003e-8f, mask);
    FmaScalar(dstReg, squareReg, 0.0000067820783f, mask);
    FmaScalar(dstReg, squareReg, 0.00043402583f, mask);
    FmaScalar(dstReg, squareReg, 0.015625f, mask);
    FmaScalar(dstReg, squareReg, 0.25f, mask);
    FmaScalar(dstReg, squareReg, 1.0f, mask);
}

__simd_callee__ inline void ComputeLarge(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& absReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> oneReg;
    Reg::RegTensor<float> reciprocalReg;
    Reg::RegTensor<float> sqrtReg;
    Reg::RegTensor<float> expHalfReg;
    Reg::RegTensor<float> expMinusReg;
    Reg::RegTensor<float> expPlusReg;

    Reg::Duplicate(oneReg, BESSEL_I0_ONE, mask);
    Reg::Div(reciprocalReg, oneReg, absReg, mask);

    Reg::Duplicate(dstReg, 0.34872168f, mask);
    FmaScalar(dstReg, reciprocalReg, -0.0054563344f, mask);
    FmaScalar(dstReg, reciprocalReg, 0.033347155f, mask);
    FmaScalar(dstReg, reciprocalReg, 0.027889195f, mask);
    FmaScalar(dstReg, reciprocalReg, 0.04987063f, mask);
    FmaScalar(dstReg, reciprocalReg, 0.39894226f, mask);

    Reg::Sqrt(sqrtReg, absReg, mask);
    Reg::Div(dstReg, dstReg, sqrtReg, mask);

    Reg::Muls(expHalfReg, absReg, BESSEL_I0_HALF, mask);
    Reg::Exp(expHalfReg, expHalfReg, mask);
    Reg::Adds(expMinusReg, expHalfReg, -BESSEL_I0_ONE, mask);
    Reg::Adds(expPlusReg, expHalfReg, BESSEL_I0_ONE, mask);
    Reg::Mul(expHalfReg, expMinusReg, expPlusReg, mask);
    Reg::Mul(expHalfReg, dstReg, expHalfReg, mask);
    Reg::Add(dstReg, expHalfReg, dstReg, mask);
}

__simd_callee__ inline void Compute(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> absReg;
    Reg::RegTensor<float> smallReg;
    Reg::RegTensor<float> largeReg;
    Reg::RegTensor<uint32_t> infU32Reg;
    Reg::MaskReg largeMask;
    Reg::MaskReg infMask;

    Reg::Abs(absReg, srcReg, mask);
    ComputeSmall(smallReg, absReg, mask);
    ComputeLarge(largeReg, absReg, mask);

    Reg::Compares<float, CMPMODE::GE>(largeMask, absReg, BESSEL_I0_BOUNDARY, mask);
    Reg::Select(dstReg, largeReg, smallReg, largeMask);

    Reg::Duplicate(infU32Reg, BESSEL_I0_POS_INF, mask);
    Reg::Compare<float, CMPMODE::EQ>(infMask, absReg, (Reg::RegTensor<float>&)infU32Reg, mask);
    Reg::Select(dstReg, (Reg::RegTensor<float>&)infU32Reg, dstReg, infMask);
}

template <typename T>
__simd_vf__ inline void BesselI0CoreImpl(__ubuf__ T* dstUb, __ubuf__ T* srcUb, const uint32_t calCount)
{
    constexpr uint32_t oneRepeatSize = GetVecLen() / sizeof(T);
    const uint32_t repeatTimes = CeilDivision(calCount, oneRepeatSize);
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;

    for (uint32_t i = 0; i < repeatTimes; ++i) {
        const uint32_t offset = i * oneRepeatSize;
        const uint32_t validCount = (calCount - offset) < oneRepeatSize ? (calCount - offset) : oneRepeatSize;
        Reg::MaskReg mask = Reg::UpdateMask<T>(validCount);
        Reg::LoadAlign(srcReg, srcUb + offset);
        Compute(dstReg, srcReg, mask);
        Reg::StoreAlign(dstUb + offset, dstReg, mask);
    }
}

} // namespace BesselI0API

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0Impl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(SupportType<T, float>(), "BesselI0 only supports float in this experimental version.");
    ASCENDC_ASSERT((calCount <= srcTensor.GetSize()), { KERNEL_LOG(KERNEL_ERROR, "calCount exceeds srcTensor size"); });
    ASCENDC_ASSERT((calCount <= dstTensor.GetSize()), { KERNEL_LOG(KERNEL_ERROR, "calCount exceeds dstTensor size"); });

    __ubuf__ T* dstUb = reinterpret_cast<__ubuf__ T*>(dstTensor.GetPhyAddr());
    __ubuf__ T* srcUb = reinterpret_cast<__ubuf__ T*>(srcTensor.GetPhyAddr());
    asc_vf_call<BesselI0API::BesselI0CoreImpl<T>>(dstUb, srcUb, calCount);
}

} // namespace AscendC

#endif // IMPL_MATH_BESSEL_I0_BESSEL_I0_3510_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_3510_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_BESSEL_I0_3510_IMPL_H__
#endif

