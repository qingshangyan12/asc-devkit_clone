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
 * \file asinh_3510_impl.h
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/asinh/asinh_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/asinh.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_ASINH_ASINH_C310_IMPL_H__
#endif
#ifndef DETAIL_MATH_ASINH_ASINH_C310_IMPL_H
#define DETAIL_MATH_ASINH_ASINH_C310_IMPL_H
#include "../../../../../include/basic_api/kernel_tensor.h"
#include "../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../common/check.h"
#include "../../common/common.h"

namespace AscendC {
namespace Internal {
constexpr float ASINH_ONE = 1.0f;
constexpr float ASINH_NEG_ONE = -1.0f;
constexpr float ASINH_ZERO = 0.0f;
constexpr float ASINH_S_MIN = 1.0e-45f;
constexpr float ASINH_S_MAX = 3.4028235e34f;
constexpr float ASINH_LN_TWO = 0.6931472f;
constexpr float ASINH_SMALL_THRESHOLD = 0.00024414063f;
constexpr float ASINH_DIRECT_LOG_THRESHOLD = 10.0f;
constexpr float ASINH_ASYMPTOTIC_THRESHOLD = 268435456.0f;

template <typename T>
__simd_vf__ inline void AsinhImplVF(__ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount, uint16_t repeatTimes)
{
    Reg::RegTensor<T> srcVreg;
    Reg::RegTensor<T> dstVreg;
    Reg::RegTensor<float> xReg;
    Reg::RegTensor<float> absReg;
    Reg::RegTensor<float> bReg;
    Reg::RegTensor<float> rReg;
    Reg::RegTensor<float> sReg;
    Reg::RegTensor<float> tmpReg;
    Reg::MaskReg mask;
    Reg::MaskReg selectMask;
    Reg::MaskReg signMask;
    constexpr int32_t oneRepElm = static_cast<int32_t>(GetVecLen() / sizeof(float));
    for (uint16_t i = 0; i < repeatTimes; ++i) {
        mask = Reg::UpdateMask<float>(calCount);
        if constexpr (sizeof(T) == sizeof(half)) {
            Reg::LoadAlign<half, Reg::LoadDist::DIST_UNPACK_B16>(srcVreg, src + i * oneRepElm);
            Reg::Cast<float, half, castTraitB16ToB32>(xReg, srcVreg, mask);
        } else {
            Reg::LoadAlign(xReg, src + i * oneRepElm);
        }

        Reg::Abs(absReg, xReg, mask);

        // Compute r = |x| + sqrt(1 + |x|^2) - 1 without forming |x|^2.
        Reg::Duplicate(bReg, ASINH_ONE);
        Reg::Div(bReg, bReg, absReg, mask);
        Reg::Mul(rReg, bReg, bReg, mask);
        Reg::Adds(rReg, rReg, ASINH_ONE, mask);
        Reg::Sqrt(rReg, rReg, mask);
        Reg::Add(rReg, rReg, bReg, mask);
        Reg::Div(rReg, absReg, rReg, mask);
        Reg::Add(rReg, absReg, rReg, mask);

        // log1p-style compensation for |x| < 10.
        Reg::Adds(sReg, rReg, ASINH_ONE, mask);
        Reg::Adds(bReg, sReg, ASINH_NEG_ONE, mask);
        Reg::Maxs(bReg, bReg, ASINH_S_MIN, mask);
        Reg::Mins(bReg, bReg, ASINH_S_MAX, mask);
        Reg::Ln(sReg, sReg, mask);

        Reg::Duplicate(tmpReg, ASINH_ONE);
        Reg::CompareScalar<float, CMPMODE::LT>(selectMask, absReg, ASINH_DIRECT_LOG_THRESHOLD, mask);
        Reg::Select(rReg, rReg, tmpReg, selectMask);
        Reg::Select(bReg, bReg, tmpReg, selectMask);
        Reg::Mul(sReg, sReg, rReg, mask);
        Reg::Div(sReg, sReg, bReg, mask);

        // Use log(|x|) + ln(2) for very large inputs to avoid intermediate overflow.
        Reg::Ln(rReg, absReg, mask);
        Reg::Adds(rReg, rReg, ASINH_LN_TWO, mask);
        Reg::Duplicate(bReg, ASINH_ONE);
        Reg::Div(bReg, bReg, absReg, mask);
        Reg::Mul(bReg, bReg, bReg, mask);
        Reg::Add(rReg, rReg, bReg, mask);
        Reg::Adds(bReg, rReg, ASINH_ZERO, mask);

        Reg::Duplicate(tmpReg, ASINH_S_MAX);
        Reg::CompareScalar<float, CMPMODE::LT>(selectMask, absReg, ASINH_DIRECT_LOG_THRESHOLD, mask);
        Reg::Select(rReg, rReg, tmpReg, selectMask);
        Reg::CompareScalar<float, CMPMODE::GE>(selectMask, absReg, ASINH_ASYMPTOTIC_THRESHOLD, mask);
        Reg::Select(rReg, bReg, rReg, selectMask);

        // NaN compares false; restore its correction lane to preserve NaN propagation.
        Reg::Compare<float, CMPMODE::EQ>(selectMask, absReg, absReg, mask);
        Reg::Select(rReg, rReg, bReg, selectMask);
        Reg::Min(sReg, sReg, rReg, mask);

        Reg::CompareScalar<float, CMPMODE::LT>(selectMask, absReg, ASINH_SMALL_THRESHOLD, mask);
        Reg::Select(sReg, absReg, sReg, selectMask);
        Reg::Neg(tmpReg, sReg, mask);
        Reg::CompareScalar<float, CMPMODE::GE>(signMask, xReg, ASINH_ZERO, mask);
        Reg::Select(rReg, sReg, tmpReg, signMask);

        if constexpr (sizeof(T) == sizeof(half)) {
            Reg::Cast<half, float, castTraitB32ToB16>(dstVreg, rReg, mask);
            Reg::StoreAlign<half, Reg::StoreDist::DIST_PACK_B32>(dst + i * oneRepElm, dstVreg, mask);
        } else {
            Reg::StoreAlign(dst + i * oneRepElm, rReg, mask);
        }
    }
}
} // namespace Internal

template <typename T, bool isReuseSource = false>
__aicore__ inline void AsinhImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(SupportType<T, half, float>(), "Asinh only support half/float data type on current device!");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN/VECOUT/VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN/VECOUT/VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Asinh");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Asinh");
    constexpr int32_t oneRepElm = static_cast<int32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = static_cast<uint16_t>(CeilDivision(calCount, oneRepElm));
    Internal::AsinhImplVF<T>(
        (__ubuf__ T*)dstTensor.GetPhyAddr(), (__ubuf__ T*)srcTensor.GetPhyAddr(), calCount, repeatTimes);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void AsinhImpl(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    AsinhImpl<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void AsinhImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN/VECOUT/VECCALC");
    AsinhImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

} // namespace AscendC
#endif // DETAIL_MATH_ASINH_ASINH_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_ASINH_ASINH_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_ASINH_ASINH_C310_IMPL_H__
#endif
