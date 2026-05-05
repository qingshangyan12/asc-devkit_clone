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
 * \brief High-precision log1p(x) = log(1+x) implementation.
 *        For small |x| uses polynomial approximation to avoid
 *        catastrophic cancellation in (1+x). For larger |x| computes
 *        log(1+x) directly via Reg::Log.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/log1p/log1p_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/log1p.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif
#ifndef IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H
#define IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H
#include "kernel_basic_intf.h"
#include "kernel_tensor.h"
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)

namespace AscendC {
namespace Reg {
namespace Log1p {
// -------- Cast traits for half <-> float conversion --------
constexpr Reg::CastTrait log1pCastTraitF16F32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING};
constexpr Reg::CastTrait log1pCastTraitF32F16 = {
    Reg::RegLayout::ZERO, Reg::SatMode::NO_SAT, Reg::MaskMergeMode::ZEROING, RoundMode::CAST_RINT};

// -------- Threshold: |x| < THRESHOLD -> polynomial; else -> log(1+x) --------
constexpr float LOG1P_THRESHOLD = 0.125f;

// -------- Taylor series coefficients for log1p(x) on [-0.125, 0.125] -------
// P(x) = 1 - x/2 + x^2/3 - x^3/4 + x^4/5 - x^5/6 + x^6/7 - x^7/8
// log1p(x) = x * P(x)
constexpr float LOG1P_C0 = 1.0f;
constexpr float LOG1P_C1 = -0.5f;
constexpr float LOG1P_C2 = 0.333333343f;
constexpr float LOG1P_C3 = -0.25f;
constexpr float LOG1P_C4 = 0.2f;
constexpr float LOG1P_C5 = -0.166666672f;
constexpr float LOG1P_C6 = 0.142857149f;
constexpr float LOG1P_C7 = -0.125f;
constexpr float LOG1P_SCALAR_ONE = 1.0f;
constexpr float LOG1P_SCALAR_NEG_ONE = -1.0f;

/* ============================================================================
 * Function 1: Core computation on RegTensor registers with Mask control.
 * Operates on data already loaded into vector registers. The MaskReg mask
 * parameter controls which lanes within the 256B register are valid.
 *
 * Two-path approach:
 *   Path A (|x| < threshold): polynomial approximation P(x) * x
 *   Path B (|x| >= threshold): Reg::Log(1 + x)
 *   Selection via Compare + Select.
 *
 * Temporary registers polyReg / absReg / lnReg are caller-provided to
 * avoid internal allocation. They will be overwritten.
 * ============================================================================ */
__simd_callee__ inline void Log1pCompute(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg, Reg::RegTensor<float>& polyReg,
    Reg::RegTensor<float>& absReg, Reg::RegTensor<float>& lnReg, Reg::MaskReg mask)
{
    // ------- Path A: Polynomial evaluation (Horner inner-to-outer) ---------
    // r = C7
    Reg::Duplicate(polyReg, LOG1P_C7, mask);

    // r = r*x + C6                                     = C7*x + C6
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C6, mask);

    // r = r*x + C5                                     = C7*x^2 + C6*x + C5
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C5, mask);

    // r = r*x + C4
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C4, mask);

    // r = r*x + C3
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C3, mask);

    // r = r*x + C2
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C2, mask);

    // r = r*x + C1
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C1, mask);

    // r = r*x + C0                                     = P(x)
    Reg::Mul(polyReg, polyReg, srcReg, mask);
    Reg::Adds(polyReg, polyReg, LOG1P_C0, mask);

    // resultA = x * P(x)                               = log1p(x)
    Reg::Mul(polyReg, polyReg, srcReg, mask);

    // ------- Path B: log(1+x) via hardware log ------
    // u = 1 + x
    Reg::Adds(lnReg, srcReg, LOG1P_SCALAR_ONE, mask);
    // resultB = log(u)
    Reg::Log(lnReg, lnReg, mask);

    // ------- Selection: choose path based on |x| < threshold -------
    // absReg = max(x, -x) = |x|
    Reg::Muls(absReg, srcReg, LOG1P_SCALAR_NEG_ONE, mask);
    Reg::Max(absReg, absReg, srcReg, mask);

    Reg::MaskReg smallMask;
    Reg::CompareScalar<float, CMPMODE::LT>(smallMask, absReg, LOG1P_THRESHOLD, mask);

    // if |x| < threshold -> use polynomial; else -> use log(1+x)
    Reg::Select(dstReg, polyReg, lnReg, smallMask);
}

/* ============================================================================
 * Function 2: Data movement UB <-> RegTensor.
 *
 * Moves data from Unified Buffer (UB) into vector registers, invokes
 * Log1pCompute for the per-element math, then stores results back to UB.
 * Each RegTensor holds GetVecLen() bytes. For float, one repetition
 * processes GetVecLen()/sizeof(float) elements. For half, data is UNPACK'd
 * on load (half -> float, doubling the element count per register),
 * computed in float, and PACK'd on store (float -> half).
 *
 * Parameters:
 *   dst      - destination UB pointer (T*)
 *   src      - source UB pointer (T*)
 *   calCount - total number of valid elements to process
 *   repeat   - number of register-sized chunks to iterate over
 * ============================================================================ */
template <typename T>
__simd_vf__ inline void Log1pDataMove(__ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount, uint16_t repeat)
{
    Reg::RegTensor<T> srcReg;
    Reg::RegTensor<T> dstReg;
    Reg::RegTensor<float> srcTmp;
    Reg::RegTensor<float> dstTmp;
    Reg::RegTensor<float> polyReg;
    Reg::RegTensor<float> absReg;
    Reg::RegTensor<float> lnReg;
    Reg::MaskReg mask;
    constexpr uint32_t oneRepSize = GetVecLen() / sizeof(float);

    for (uint16_t i = 0; i < repeat; i++) {
        mask = Reg::UpdateMask<float>(calCount);

        if constexpr (std::is_same<T, half>::value) {
            // Load half data from UB, unpack to float registers
            Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcReg, src + i * oneRepSize);
            Reg::Cast<float, half, log1pCastTraitF16F32>(srcTmp, srcReg, mask);

            // Compute log1p in float precision
            Log1pCompute(dstTmp, srcTmp, polyReg, absReg, lnReg, mask);

            // Cast back to half and pack for store
            Reg::Cast<half, float, log1pCastTraitF32F16>(dstReg, dstTmp, mask);
            Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(dst + i * oneRepSize, dstReg, mask);
        } else {
            // Float path: direct load, compute, store
            Reg::LoadAlign(srcReg, src + i * oneRepSize);
            Log1pCompute(dstReg, srcReg, polyReg, absReg, lnReg, mask);
            Reg::StoreAlign(dst + i * oneRepSize, dstReg, mask);
        }
    }
}

/* ============================================================================
 * Entry point: computes repeat count from calCount and oneRepSize,
 * then delegates to Log1pDataMove.
 * ============================================================================ */
template <typename T>
__aicore__ inline void Log1pPolynomialImpl(__ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount)
{
    constexpr uint32_t oneRepSize = GetVecLen() / sizeof(float);
    uint16_t repeat = CeilDivision(calCount, oneRepSize);
    Log1pDataMove<T>(dst, src, calCount, repeat);
}

} // namespace Log1p
} // namespace Reg

/* ============================================================================
 * High-level public entry points with parameter validation.
 * Supports automatic stack buffer allocation and user-provided buffers.
 * ============================================================================ */

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }

    // Auto-allocate temporary buffer from stack
    LocalTensor<uint8_t> sharedTmpBuffer;
    bool ans = PopStackBuffer<uint8_t, TPosition::LCM>(sharedTmpBuffer);
    ASCENDC_ASSERT((ans), { KERNEL_LOG(KERNEL_ERROR, "PopStackBuffer Error!"); });
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }

    static_assert(SupportType<T, half, float>(), "Log1p only supports half/float data type on current device!");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(sharedTmpBuffer, "sharedTmpBuffer", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Log1p");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Log1p");

    Reg::Log1p::Log1pPolynomialImpl(
        (__ubuf__ T*)dstTensor.GetPhyAddr(), (__ubuf__ T*)srcTensor.GetPhyAddr(), calCount);
}

} // namespace AscendC
#endif // __NPU_ARCH__ guard
#endif // IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif
