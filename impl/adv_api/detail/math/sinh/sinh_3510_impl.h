/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file sinh_3510_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/sinh/sinh_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/sinh.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_SINH_SINH_C310_IMPL_H__
#endif
#ifndef IMPL_MATH_SINH_SINH_C310_IMPL_H
#define IMPL_MATH_SINH_SINH_C310_IMPL_H
#include "../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../../../../include/basic_api/kernel_tensor.h"
#include "../../common/check.h"

namespace AscendC {
namespace SinhInternal {
// Computes sinh values based on input types.
// According formula: sinh(x) = (e^x - e^(-x))/2 = e^(x-ln2) - 0.25/(e^(x-ln2)).
template <typename T>
__simd_vf__ inline void SinhCompute(__ubuf__ T* dstUb, __ubuf__ T* srcUb, uint32_t calCount, uint16_t repeatTimes)
{
    constexpr float taylorC2 = 0.16666667f;
    constexpr float taylorC3 = 0.008333347f;
    constexpr float taylorC4 = 0.00019841270f;
    constexpr float taylorC5 = 0.0000027557319f;
    constexpr float one = 1.0f;
    constexpr float zero = 0.0f;
    constexpr float quarter = 0.25f;
    constexpr float overflow = 89.4159927368164f;
    constexpr float negLnTwoHi = -0.6931471824645996f;
    constexpr float lnTwoLo = 1.9046542e-9f;
    const float inf = __builtin_huge_valf();
    constexpr uint32_t vlSize = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    static constexpr Reg::CastTrait sinhCastTraitUpper = {
        Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};
    static constexpr Reg::CastTrait sinhCastTraitLower = {
        Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};
    static constexpr Reg::DivSpecificMode sinhDivMode = {Reg::MaskMergeMode::ZEROING, true};
    Reg::MaskReg sinhMask;
    Reg::MaskReg negativeMask;
    Reg::MaskReg selectMask;
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<float> xReg;
    Reg::RegTensor<float> absReg;
    Reg::RegTensor<float> x2Reg;
    Reg::RegTensor<float> polyReg;
    Reg::RegTensor<float> tmpReg;
    Reg::RegTensor<float> expReg;
    Reg::RegTensor<float> resultReg;
    Reg::RegTensor<T> dstReg;
    for (uint16_t i = 0; i < repeatTimes; i++) {
        sinhMask = Reg::UpdateMask<float>(calCount);
        if constexpr (SupportBytes<T, 2>()) {
            Reg::LoadAlign<half, Reg::LoadDist::DIST_UNPACK_B16>(srcReg, srcUb + i * vlSize);
            Reg::Cast<float, half, sinhCastTraitUpper>(xReg, srcReg, sinhMask);
        } else {
            Reg::LoadAlign(xReg, srcUb + i * vlSize);
        }

        Reg::Abs(absReg, xReg, sinhMask);

        // |x| < 1: sinh(x) = x + x^3 * P(x^2).
        Reg::Mul(x2Reg, xReg, xReg, sinhMask);
        Reg::Duplicate(polyReg, taylorC5);
        Reg::Mul(polyReg, polyReg, x2Reg, sinhMask);
        Reg::Adds(polyReg, polyReg, taylorC4, sinhMask);
        Reg::Mul(polyReg, polyReg, x2Reg, sinhMask);
        Reg::Adds(polyReg, polyReg, taylorC3, sinhMask);
        Reg::Mul(polyReg, polyReg, x2Reg, sinhMask);
        Reg::Adds(polyReg, polyReg, taylorC2, sinhMask);
        Reg::Mul(tmpReg, x2Reg, xReg, sinhMask);
        Reg::Mul(polyReg, polyReg, tmpReg, sinhMask);
        Reg::Add(polyReg, xReg, polyReg, sinhMask);

        // |x| >= 1: offset exp with compensated |x| - ln(2).
        Reg::Adds(tmpReg, absReg, negLnTwoHi, sinhMask);
        Reg::Sub(expReg, tmpReg, absReg, sinhMask);
        Reg::Sub(resultReg, tmpReg, expReg, sinhMask);
        Reg::Sub(resultReg, absReg, resultReg, sinhMask);
        Reg::Neg(x2Reg, expReg, sinhMask);
        Reg::Adds(x2Reg, x2Reg, negLnTwoHi, sinhMask);
        Reg::Add(resultReg, resultReg, x2Reg, sinhMask);
        Reg::Adds(resultReg, resultReg, lnTwoLo, sinhMask);

        Reg::Exp(expReg, tmpReg, sinhMask);
        Reg::CompareScalar<float, CMPMODE::LT>(selectMask, absReg, overflow, sinhMask);
        Reg::Mul(x2Reg, expReg, resultReg, selectMask);
        Reg::Add(x2Reg, expReg, x2Reg, selectMask);
        Reg::Select(expReg, x2Reg, expReg, selectMask);

        Reg::Duplicate(tmpReg, quarter);
        Reg::Div<float, &sinhDivMode>(tmpReg, tmpReg, expReg, sinhMask);
        Reg::Sub(expReg, expReg, tmpReg, sinhMask);

        Reg::CompareScalar<float, CMPMODE::LT>(negativeMask, xReg, zero, sinhMask);
        Reg::Neg(tmpReg, expReg, sinhMask);
        Reg::Select(expReg, tmpReg, expReg, negativeMask);

        Reg::CompareScalar<float, CMPMODE::LT>(selectMask, absReg, one, sinhMask);
        Reg::Select(resultReg, polyReg, expReg, selectMask);

        // Use the first FP32 input whose correctly rounded sinh overflows.
        Reg::CompareScalar<float, CMPMODE::GE>(selectMask, absReg, overflow, sinhMask);
        Reg::Duplicate(tmpReg, inf);
        Reg::Neg(expReg, tmpReg, sinhMask);
        Reg::Select(tmpReg, expReg, tmpReg, negativeMask);
        Reg::Select(resultReg, tmpReg, resultReg, selectMask);

        if constexpr (SupportBytes<T, 2>()) {
            Reg::Cast<half, float, sinhCastTraitLower>(dstReg, resultReg, sinhMask);
            Reg::StoreAlign<half, Reg::StoreDist::DIST_PACK_B32>(dstUb + i * vlSize, dstReg, sinhMask);
        } else {
            Reg::StoreAlign(dstUb + i * vlSize, resultReg, sinhMask);
        }
    }
}
} // namespace SinhInternal

template <typename T, bool isReuseSource = false>
__aicore__ inline void SinhImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN/VECOUT/VECCALC");
    SinhImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void SinhImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    static_assert(SupportType<T, half, float>(), "Sinh only support half/float data type on current device!");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN/VECOUT/VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN/VECOUT/VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Sinh");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Sinh");

    __ubuf__ T* dstUb = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* srcUb = (__ubuf__ T*)srcTensor.GetPhyAddr();
    constexpr int32_t vlSize = static_cast<int32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, vlSize));
    SinhInternal::SinhCompute<T>(dstUb, srcUb, calCount, repeatTimes);
}
} // namespace AscendC

#endif // IMPL_MATH_SINH_SINH_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_SINH_SINH_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_SINH_SINH_C310_IMPL_H__
#endif
