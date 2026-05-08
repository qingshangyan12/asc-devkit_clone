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
 * \file log1p_3510_impl.h
 * \brief High-performance SIMD implementation of log1p(x) = log(1+x) for AscendC 3510 architecture
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/log1p/log1p_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/log1p.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif
#ifndef IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H
#define IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "kernel_pop_stack_buffer.h"
#include "kernel_tiling/kernel_tiling.h"
#include "../../common/common.h"
#include "../../common/check.h"

namespace AscendC {
namespace Log1pInternal {

constexpr Reg::CastTrait castTraitB16ToB32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};
constexpr Reg::CastTrait castTraitB32ToB16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};

constexpr float LOG1P_THRESHOLD = 0.5f;
constexpr float LOG1P_ONE = 1.0f;
constexpr float LOG1P_NEG_ONE = -1.0f;

constexpr float LOG1P_TAYLOR_COEFFS[] = {
    1.0f,
    -0.5f,
    -0.3333333333333333f,
    -0.25f,
    -0.2f,
    -0.16666666666666666f,
    -0.14285714285714285f,
    -0.125f
};

constexpr uint8_t LOG1P_TAYLOR_ORDER = 8;

template <typename T>
__simd_vf__ inline void Log1pTaylorExpand(
    Reg::RegTensor<float>& resultReg, Reg::RegTensor<float>& xReg, Reg::MaskReg& mask)
{
    Reg::RegTensor<float> powReg;
    Reg::RegTensor<float> tmpReg;
    
    Reg::Duplicate(powReg, 1.0f);
    Reg::Duplicate(resultReg, 0.0f);
    
    for (uint8_t i = 1; i <= LOG1P_TAYLOR_ORDER; ++i) {
        Reg::Mul(powReg, powReg, xReg, mask);
        Reg::Muls(tmpReg, powReg, LOG1P_TAYLOR_COEFFS[i - 1], mask);
        Reg::Add(resultReg, resultReg, tmpReg, mask);
    }
}

template <typename T>
__simd_vf__ inline void Log1pImplVFHighPrecision(
    __ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount, uint16_t repeatTimes)
{
    constexpr uint32_t sregLower = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    
    Reg::MaskReg mask;
    Reg::MaskReg taylorMask;
    Reg::RegTensor<T> srcVreg;
    Reg::RegTensor<T> dstVreg;
    Reg::RegTensor<float> xReg;
    Reg::RegTensor<float> absXReg;
    Reg::RegTensor<float> onePlusXReg;
    Reg::RegTensor<float> taylorReg;
    Reg::RegTensor<float> logReg;
    Reg::RegTensor<float> resultReg;
    
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount);
        
        if constexpr (std::is_same_v<T, half>) {
            Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcVreg, src + i * sregLower);
            Reg::Cast<float, T, castTraitB16ToB32>(xReg, srcVreg, mask);
        } else {
            Reg::LoadAlign(xReg, src + i * sregLower);
        }
        
        Reg::Abs(absXReg, xReg, mask);
        Reg::CompareScalar<float, CMPMODE::LT>(taylorMask, absXReg, LOG1P_THRESHOLD, mask);
        
        Log1pTaylorExpand<T>(taylorReg, xReg, taylorMask);
        
        Reg::Adds(onePlusXReg, xReg, LOG1P_ONE, mask);
        Reg::Ln(logReg, onePlusXReg, mask);
        
        Reg::Select(resultReg, logReg, taylorReg, taylorMask);
        
        if constexpr (std::is_same_v<T, half>) {
            Reg::Cast<T, float, castTraitB32ToB16>(dstVreg, resultReg, mask);
            Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(dst + i * sregLower, dstVreg, mask);
        } else {
            Reg::StoreAlign(dst + i * sregLower, resultReg, mask);
        }
    }
}

} // namespace Log1pInternal

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
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
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Log1p");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Log1p");
    
    __ubuf__ T* dst = (__ubuf__ T*)dstTensor.GetPhyAddr();
    __ubuf__ T* src = (__ubuf__ T*)srcTensor.GetPhyAddr();
    
    constexpr uint32_t sregLower = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, sregLower));
    
    Log1pInternal::Log1pImplVFHighPrecision<T>(dst, src, calCount, repeatTimes);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, 
    const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
{
    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN, VECOUT, VECCALC");
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, 
    const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

} // namespace AscendC

#endif // IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif