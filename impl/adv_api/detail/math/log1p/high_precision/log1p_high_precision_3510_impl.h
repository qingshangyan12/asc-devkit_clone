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
 * \file log1p_high_precision_3510_impl.h
 * \brief High-precision SIMD implementation of log1p(x) = log(1+x) for AscendC 3510 architecture
 * 
 * This implementation provides enhanced accuracy through:
 * 1. Multi-branch processing based on input magnitude
 * 2. Higher-order Taylor expansion for small values
 * 3. Special handling for edge cases (x = -1, x < -1)
 * 4. Numerical stability improvements for large values
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/log1p/high_precision/log1p_high_precision_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/log1p.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H__
#endif
#ifndef IMPL_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H
#define IMPL_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "kernel_pop_stack_buffer.h"
#include "kernel_tiling/kernel_tiling.h"
#include "../../common/common.h"
#include "../../common/check.h"

namespace AscendC {
namespace Log1pHighPrecision {

constexpr Reg::CastTrait castTraitB16ToB32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};
constexpr Reg::CastTrait castTraitB32ToB16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};

constexpr float LOG1P_EPSILON = 1.0e-7f;
constexpr float LOG1P_TAYLOR_THRESHOLD = 0.5f;
constexpr float LOG1P_MID_THRESHOLD = 1.5f;
constexpr float LOG1P_ONE = 1.0f;
constexpr float LOG1P_NEG_ONE = -1.0f;
constexpr float LOG1P_ZERO = 0.0f;

constexpr float LOG1P_TAYLOR_COEFFS[] = {
    1.0f,
    -0.5f,
    0.3333333333333333f,
    -0.25f,
    0.2f,
    -0.16666666666666666f,
    0.14285714285714285f,
    -0.125f,
    0.1111111111111111f,
    -0.1f,
    0.09090909090909091f,
    -0.08333333333333333f,
    0.07692307692307693f,
    -0.07142857142857142f,
    0.06666666666666667f,
    -0.0625f
};

constexpr uint8_t LOG1P_TAYLOR_ORDER = 16;

template <typename T>
__simd_vf__ inline void Log1pTaylorExpandHighOrder(
    Reg::RegTensor<float>& resultReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> powReg;
    Reg::RegTensor<float> tmpReg;
    Reg::RegTensor<float> xSquaredReg;
    
    Reg::Duplicate(powReg, xReg);
    Reg::Duplicate(resultReg, xReg);
    Reg::Mul(xSquaredReg, xReg, xReg, mask);
    
    for (uint8_t i = 2; i <= LOG1P_TAYLOR_ORDER; ++i) {
        if (i % 2 == 0) {
            Reg::Mul(powReg, powReg, xSquaredReg, mask);
        }
        Reg::Muls(tmpReg, powReg, LOG1P_TAYLOR_COEFFS[i - 1], mask);
        Reg::Add(resultReg, resultReg, tmpReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void Log1pSmallValueCompute(
    Reg::RegTensor<float>& resultReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> onePlusXReg;
    Reg::RegTensor<float> logReg;
    Reg::RegTensor<float> taylorReg;
    Reg::MaskReg taylorMask;
    
    Reg::Adds(onePlusXReg, xReg, LOG1P_ONE, mask);
    Reg::CompareScalar<float, CMPMODE::LT>(taylorMask, xReg, LOG1P_TAYLOR_THRESHOLD, mask);
    Reg::MaskReg negMask;
    Reg::CompareScalar<float, CMPMODE::LT>(negMask, xReg, LOG1P_ZERO, mask);
    Reg::MaskReg absTaylorMask;
    Reg::RegTensor<float> absXReg;
    Reg::Abs(absXReg, xReg, mask);
    Reg::CompareScalar<float, CMPMODE::LT>(absTaylorMask, absXReg, LOG1P_TAYLOR_THRESHOLD, mask);
    
    Log1pTaylorExpandHighOrder<T>(taylorReg, xReg, absTaylorMask);
    
    Reg::Ln(logReg, onePlusXReg, mask);
    
    Reg::Select(resultReg, logReg, taylorReg, absTaylorMask);
}

template <typename T>
__simd_vf__ inline void Log1pMidValueCompute(
    Reg::RegTensor<float>& resultReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> onePlusXReg;
    Reg::RegTensor<float> yReg;
    Reg::RegTensor<float> ySquaredReg;
    Reg::RegTensor<float> numeratorReg;
    Reg::RegTensor<float> denominatorReg;
    Reg::RegTensor<float> logYReg;
    
    Reg::Adds(onePlusXReg, xReg, LOG1P_ONE, mask);
    Reg::Adds(yReg, xReg, LOG1P_ONE, mask);
    Reg::Mul(ySquaredReg, yReg, yReg, mask);
    
    Reg::Adds(numeratorReg, yReg, LOG1P_ONE, mask);
    Reg::Subs(numeratorReg, numeratorReg, ySquaredReg, mask);
    
    Reg::Adds(denominatorReg, yReg, LOG1P_ONE, mask);
    
    Reg::Div(yReg, numeratorReg, denominatorReg, mask);
    Reg::Ln(logYReg, yReg, mask);
    
    Reg::Ln(resultReg, onePlusXReg, mask);
}

template <typename T>
__simd_vf__ inline void Log1pLargeValueCompute(
    Reg::RegTensor<float>& resultReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> oneOverXReg;
    Reg::RegTensor<float> onePlusOneOverXReg;
    Reg::RegTensor<float> logOnePlusOneOverXReg;
    Reg::RegTensor<float> logXReg;
    Reg::RegTensor<float> tmpReg;
    
    Reg::Reciprocal(oneOverXReg, xReg, mask);
    Reg::Adds(onePlusOneOverXReg, oneOverXReg, LOG1P_ONE, mask);
    Reg::Ln(logOnePlusOneOverXReg, onePlusOneOverXReg, mask);
    
    Reg::Ln(logXReg, xReg, mask);
    
    Reg::Add(resultReg, logXReg, logOnePlusOneOverXReg, mask);
}

template <typename T>
__simd_vf__ inline void Log1pHighPrecisionImplVF(
    __ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t sregLower = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    
    Reg::MaskReg mask;
    Reg::MaskReg smallMask;
    Reg::MaskReg midMask;
    Reg::MaskReg largeMask;
    Reg::MaskReg negOneMask;
    Reg::MaskReg invalidMask;
    Reg::MaskReg nearZeroMask;
    
    Reg::RegTensor<T> srcVreg;
    Reg::RegTensor<T> dstVreg;
    Reg::RegTensor<float> xReg;
    Reg::RegTensor<float> absXReg;
    Reg::RegTensor<float> resultReg;
    Reg::RegTensor<float> smallResultReg;
    Reg::RegTensor<float> midResultReg;
    Reg::RegTensor<float> largeResultReg;
    Reg::RegTensor<float> nanReg;
    Reg::RegTensor<float> negInfReg;
    Reg::RegTensor<float> tmpReg;
    
    Reg::Duplicate(nanReg, std::numeric_limits<float>::quiet_NaN());
    Reg::Duplicate(negInfReg, -std::numeric_limits<float>::infinity());
    
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount);
        
        if constexpr (std::is_same_v<T, half>) {
            Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcVreg, src + i * sregLower);
            Reg::Cast<float, T, castTraitB16ToB32>(xReg, srcVreg, mask);
        } else {
            Reg::LoadAlign(xReg, src + i * sregLower);
        }
        
        Reg::Abs(absXReg, xReg, mask);
        
        Reg::CompareScalar<float, CMPMODE::EQ>(negOneMask, xReg, LOG1P_NEG_ONE, mask);
        
        Reg::CompareScalar<float, CMPMODE::LT>(invalidMask, xReg, LOG1P_NEG_ONE, mask);
        
        Reg::CompareScalar<float, CMPMODE::LT>(nearZeroMask, absXReg, LOG1P_EPSILON, mask);
        
        Reg::CompareScalar<float, CMPMODE::LT>(smallMask, absXReg, LOG1P_TAYLOR_THRESHOLD, mask);
        
        Reg::CompareScalar<float, CMPMODE::LT>(midMask, absXReg, LOG1P_MID_THRESHOLD, mask);
        Reg::MaskReg tmpMask;
        Reg::AndNot(tmpMask, smallMask, midMask);
        midMask = tmpMask;
        
        Reg::MaskReg largeMaskAll;
        Reg::CompareScalar<float, CMPMODE::GE>(largeMaskAll, absXReg, LOG1P_MID_THRESHOLD, mask);
        Reg::AndNot(largeMask, smallMask, largeMaskAll);
        Reg::AndNot(largeMask, midMask, largeMask);
        
        Log1pSmallValueCompute<T>(smallResultReg, xReg, smallMask);
        
        Log1pMidValueCompute<T>(midResultReg, xReg, midMask);
        
        Log1pLargeValueCompute<T>(largeResultReg, xReg, largeMask);
        
        Reg::Duplicate(resultReg, 0.0f);
        Reg::Select(resultReg, resultReg, smallResultReg, smallMask);
        Reg::Select(resultReg, resultReg, midResultReg, midMask);
        Reg::Select(resultReg, resultReg, largeResultReg, largeMask);
        
        Reg::Select(resultReg, resultReg, xReg, nearZeroMask);
        
        Reg::Select(resultReg, resultReg, negInfReg, negOneMask);
        
        Reg::Select(resultReg, resultReg, nanReg, invalidMask);
        
        if constexpr (std::is_same_v<T, half>) {
            Reg::Cast<T, float, castTraitB32ToB16>(dstVreg, resultReg, mask);
            Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(dst + i * sregLower, dstVreg, mask);
        } else {
            Reg::StoreAlign(dst + i * sregLower, resultReg, mask);
        }
    }
}

} // namespace Log1pHighPrecision

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pHighPrecisionImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    
    static_assert(
        (std::is_same_v<T, half> || std::is_same_v<T, float>), 
        "current data type is not supported on current device!");
    
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Log1pHighPrecision");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Log1pHighPrecision");
    
    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* src = (__ubuf__ T*)srcTensor.GetPhyAddr();
    
    constexpr uint32_t sregLower = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, sregLower));
    
    Log1pHighPrecision::Log1pHighPrecisionImplVF<T>(dst, src, calCount, repeatTimes);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pHighPrecisionImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Log1pHighPrecisionImpl<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pHighPrecisionImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, 
    const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
{
    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN, VECOUT, VECCALC");
    Log1pHighPrecisionImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pHighPrecisionImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, 
    const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    Log1pHighPrecisionImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

} // namespace AscendC

#endif // IMPL_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_HIGH_PRECISION_3510_IMPL_H__
#endif