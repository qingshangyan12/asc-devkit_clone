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
 * \brief SIMD register-based implementation of log1p(x) = log(1 + x) for Ascend 3510/5102/3003/3113.
 *
 * Algorithm:
 *   - For |x| < 2^-4 (0.0625): Use 7th-order Taylor series to avoid catastrophic cancellation in (1 + x).
 *     log1p(x) = x - x^2/2 + x^3/3 - x^4/4 + x^5/5 - x^6/6 + x^7/7
 *     Evaluated via Horner's method for numerical stability.
 *   - For |x| >= 2^-4: Directly compute log(1 + x) using Reg::Log intrinsic,
 *     since (1 + x) retains sufficient precision.
 *   - Branch selection uses Reg::CompareScalar + Reg::Select for predicated SIMD execution.
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

namespace AscendC {

// 7th-order Taylor coefficients for log(1+x) = sum_{k=1}^{7} (-1)^{k+1} * x^k / k
// Evaluated as x * (1 + x * (c2 + x * (c3 + x * (c4 + x * (c5 + x * (c6 + x * c7))))))
namespace Log1pInternal {
constexpr float LOG1P_C7 = 1.0f / 7.0f;
constexpr float LOG1P_C6 = -1.0f / 6.0f;
constexpr float LOG1P_C5 = 1.0f / 5.0f;
constexpr float LOG1P_C4 = -1.0f / 4.0f;
constexpr float LOG1P_C3 = 1.0f / 3.0f;
constexpr float LOG1P_C2 = -1.0f / 2.0f;
constexpr float LOG1P_C1 = 1.0f;

constexpr float LOG1P_ONE = 1.0f;

// Threshold for switching between Taylor series and direct log(1+x).
// For |x| < 2^-4, Taylor series provides full float32 precision without
// the cancellation error inherent in computing (1 + x) for small x.
constexpr float LOG1P_TAYLOR_THRESHOLD = 6.25e-2f; // 2^-4
} // namespace Log1pInternal

template <typename T>
__simd_vf__ inline void Log1pImpl(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t calCount)
{
    static_assert(
        (std::is_same_v<T, half> || std::is_same_v<T, float>), "current data type is not supported on current device!");

    // All computation is performed in float32 for precision.
    // For half inputs, data is unpacked to float32, computed, then packed back.
    constexpr uint32_t sregLower = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = CeilDivision(calCount, sregLower);

    static constexpr Reg::CastTrait castTraitB16ToB32 = {
        Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};
    static constexpr Reg::CastTrait castTraitB32ToB16 = {
        Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};

    if constexpr (std::is_same_v<T, float>) {
        Reg::RegTensor<float> xReg;
        Reg::RegTensor<float> xAbsReg;
        Reg::RegTensor<float> oneReg;
        Reg::RegTensor<float> uReg;
        Reg::RegTensor<float> taylorReg;
        Reg::RegTensor<float> directReg;
        Reg::RegTensor<float> resultReg;
        Reg::MaskReg mask;
        Reg::MaskReg cmpMask;
        uint32_t sreg = calCount;

        for (uint16_t i = 0; i < repeatTimes; ++i) {
            mask = Reg::UpdateMask<float>(sreg);
            Reg::LoadAlign(xReg, src + i * sregLower);

            // Mask for |x| < threshold: elements in the Taylor regime.
            Reg::Abs(xAbsReg, xReg, mask);
            Reg::CompareScalar<float, CMPMODE::LT>(
                cmpMask, xAbsReg, Log1pInternal::LOG1P_TAYLOR_THRESHOLD, mask);

            // ---- Taylor series via Horner's method ----
            // Horner form: x * (1 + x * (c2 + x * (c3 + x * (c4 + x * (c5 + x * (c6 + x * c7))))))
            Reg::Duplicate(taylorReg, Log1pInternal::LOG1P_C7, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C6, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C5, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C4, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C3, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C2, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C1, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);

            // ---- Direct computation: log(1 + x) ----
            Reg::Duplicate(oneReg, Log1pInternal::LOG1P_ONE, mask);
            Reg::Add(uReg, xReg, oneReg, mask);
            Reg::Log(directReg, uReg, mask);

            // ---- Predicated selection ----
            // cmpMask == 1  =>  |x| < threshold  =>  select Taylor
            // cmpMask == 0  =>  |x| >= threshold =>  select direct
            Reg::Select(resultReg, taylorReg, directReg, cmpMask);

            Reg::StoreAlign(dst + i * sregLower, resultReg, mask);
        }
    } else if constexpr (std::is_same_v<T, half>) {
        Reg::RegTensor<T> srcReg;
        Reg::RegTensor<T> dstReg;
        Reg::RegTensor<float> xReg;
        Reg::RegTensor<float> xAbsReg;
        Reg::RegTensor<float> oneReg;
        Reg::RegTensor<float> uReg;
        Reg::RegTensor<float> taylorReg;
        Reg::RegTensor<float> directReg;
        Reg::RegTensor<float> resultReg;
        Reg::MaskReg mask;
        Reg::MaskReg cmpMask;
        uint32_t sreg = calCount;

        for (uint16_t i = 0; i < repeatTimes; ++i) {
            mask = Reg::UpdateMask<float>(sreg);
            Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcReg, src + i * sregLower);
            Reg::Cast<float, half, castTraitB16ToB32>(xReg, srcReg, mask);

            Reg::Abs(xAbsReg, xReg, mask);
            Reg::CompareScalar<float, CMPMODE::LT>(
                cmpMask, xAbsReg, Log1pInternal::LOG1P_TAYLOR_THRESHOLD, mask);

            // ---- Taylor series via Horner's method ----
            Reg::Duplicate(taylorReg, Log1pInternal::LOG1P_C7, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C6, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C5, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C4, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C3, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C2, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);
            Reg::Adds(taylorReg, taylorReg, Log1pInternal::LOG1P_C1, mask);
            Reg::Mul(taylorReg, taylorReg, xReg, mask);

            // ---- Direct computation: log(1 + x) ----
            Reg::Duplicate(oneReg, Log1pInternal::LOG1P_ONE, mask);
            Reg::Add(uReg, xReg, oneReg, mask);
            Reg::Log(directReg, uReg, mask);

            // ---- Predicated selection ----
            Reg::Select(resultReg, taylorReg, directReg, cmpMask);

            Reg::Cast<half, float, castTraitB32ToB16>(dstReg, resultReg, mask);
            Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(dst + i * sregLower, dstReg, mask);
        }
    }
}
} // namespace AscendC
#endif // IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif
