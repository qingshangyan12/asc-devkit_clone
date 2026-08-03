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
 * \brief Ndtri implementation for NPU architecture 3510.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/ndtri/ndtri_3510_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/ndtri.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_3510_IMPL_H__
#endif

#ifndef IMPL_MATH_NDTRI_NDTRI_3510_IMPL_H
#define IMPL_MATH_NDTRI_NDTRI_3510_IMPL_H

#include "../../../../../include/basic_api/kernel_basic_intf.h"
#include "../../../../../include/basic_api/kernel_tensor.h"
#include "../../common/check.h"
#ifdef ASCENDC_CPU_DEBUG
#include "../../api_check/kernel_check/math/ndtri/ndtri_check.h"
#endif // ASCENDC_CPU_DEBUG
#include "../../api_check/kernel_api_check.h"

namespace AscendC {
namespace NdtriInternal {

union FloatU32Union {
    constexpr __aicore__ FloatU32Union(uint32_t value) : u32(value) {}
    float f32;
    uint32_t u32;
};

constexpr FloatU32Union POSITIVE_INFINITY(0x7f800000U);
constexpr FloatU32Union NEGATIVE_INFINITY(0xff800000U);
constexpr FloatU32Union QUIET_NAN(0x7fc00000U);
constexpr float NDTRI_CENTER_REGION_BOUNDARY = 0.425f;
constexpr float NDTRI_NEAR_TAIL_BOUNDARY = 5.0f;
constexpr float NDTRI_TAIL_PROBABILITY_SCALE = 1.8446744073709552e19f;
constexpr float NDTRI_TAIL_LOG_SCALE_OFFSET = -44.3614195558364998f;

__simd_callee__ inline void MulAdds(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& lhsReg, Reg::RegTensor<float>& rhsReg, const float scalar,
    Reg::MaskReg mask)
{
    Reg::Mul(dstReg, lhsReg, rhsReg, mask);
    Reg::Adds(dstReg, dstReg, scalar, mask);
}

// Task formula: sqrt(2) * sum(c_k * x^(2k + 1)), x = 2p - 1. The first 24 terms cover |x| <= 0.85.
__simd_callee__ inline void ComputeTaskSeries(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& qReg, Reg::MaskReg mask)
{
    constexpr float C0 = 0.88622695207595825f;
    constexpr float C1 = 0.23201367259025574f;
    constexpr float C2 = 0.12755617499351501f;
    constexpr float C3 = 0.086552128195762634f;
    constexpr float C4 = 0.064959615468978882f;
    constexpr float C5 = 0.051731280982494354f;
    constexpr float C6 = 0.042836721986532211f;
    constexpr float C7 = 0.036465927958488464f;
    constexpr float C8 = 0.031689006835222244f;
    constexpr float C9 = 0.027980633080005646f;
    constexpr float C10 = 0.025022275745868683f;
    constexpr float C11 = 0.022609863430261612f;
    constexpr float C12 = 0.020606780424714088f;
    constexpr float C13 = 0.018918218091130257f;
    constexpr float C14 = 0.017476370558142662f;
    constexpr float C15 = 0.01623150147497654f;
    constexpr float C16 = 0.015146315097808838f;
    constexpr float C17 = 0.014192315749824047f;
    constexpr float C18 = 0.013347364030778408f;
    constexpr float C19 = 0.012594005092978477f;
    constexpr float C20 = 0.011918296106159687f;
    constexpr float C21 = 0.011308969929814339f;
    constexpr float C22 = 0.010756825096905231f;
    constexpr float C23 = 0.010254274122416973f;
    constexpr float SQRT_TWO = 1.4142135623730951f;

    Reg::RegTensor<float> xReg;
    Reg::RegTensor<float> xSquaredReg;
    Reg::RegTensor<float> seriesReg;
    Reg::Muls(xReg, qReg, 2.0f, mask);
    Reg::Mul(xSquaredReg, xReg, xReg, mask);

    Reg::Duplicate(seriesReg, C23, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C22, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C21, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C20, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C19, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C18, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C17, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C16, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C15, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C14, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C13, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C12, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C11, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C10, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C9, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C8, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C7, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C6, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C5, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C4, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C3, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C2, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C1, mask);
    MulAdds(seriesReg, seriesReg, xSquaredReg, C0, mask);

    Reg::Mul(dstReg, xReg, seriesReg, mask);
    Reg::Muls(dstReg, dstReg, SQRT_TWO, mask);
}

// AS241 tail approximation for sqrt(-ln(min(p, 1-p))) <= 5.
__simd_callee__ inline void ComputeNearTail(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& rReg, Reg::MaskReg mask)
{
    constexpr float C0 = 1.42343711074968357734f;
    constexpr float C1 = 4.63033784615654529590f;
    constexpr float C2 = 5.76949722146069140550f;
    constexpr float C3 = 3.64784832476320460504f;
    constexpr float C4 = 1.27045825245236838258f;
    constexpr float C5 = 0.241780725177450611770f;
    constexpr float C6 = 0.0227238449892691845833f;
    constexpr float C7 = 0.000774545014278341407640f;
    constexpr float D0 = 1.0f;
    constexpr float D1 = 2.05319162663775882187f;
    constexpr float D2 = 1.67638483018380384940f;
    constexpr float D3 = 0.689767334985100004550f;
    constexpr float D4 = 0.148103976427480074590f;
    constexpr float D5 = 0.0151986665636164571966f;
    constexpr float D6 = 0.000547593808499534494600f;
    constexpr float D7 = 1.05075007164441684324e-9f;

    Reg::RegTensor<float> zReg;
    Reg::RegTensor<float> numeratorReg;
    Reg::RegTensor<float> denominatorReg;
    Reg::Adds(zReg, rReg, -1.6f, mask);

    Reg::Duplicate(numeratorReg, C7, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C6, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C5, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C4, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C3, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C2, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C1, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, C0, mask);

    Reg::Duplicate(denominatorReg, D7, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D6, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D5, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D4, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D3, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D2, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D1, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, D0, mask);
    Reg::Div(dstReg, numeratorReg, denominatorReg, mask);
}

// AS241 tail approximation for sqrt(-ln(min(p, 1-p))) > 5.
__simd_callee__ inline void ComputeFarTail(
    Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& rReg, Reg::MaskReg mask)
{
    constexpr float E0 = 6.65790464350110377720f;
    constexpr float E1 = 5.46378491116411436990f;
    constexpr float E2 = 1.78482653991729133580f;
    constexpr float E3 = 0.29656057182850489123f;
    constexpr float E4 = 0.026532189526576123093f;
    constexpr float E5 = 0.00124266094738807843860f;
    constexpr float E6 = 2.71155556874348757815e-5f;
    constexpr float E7 = 2.01033439929228813265e-7f;
    constexpr float F0 = 1.0f;
    constexpr float F1 = 0.599832206555887937690f;
    constexpr float F2 = 0.136929880922735805310f;
    constexpr float F3 = 0.0148753612908506148525f;
    constexpr float F4 = 0.00078686913114561325910f;
    constexpr float F5 = 1.84631831751005468180e-5f;
    constexpr float F6 = 1.42151175831644588870e-7f;
    constexpr float F7 = 2.04426310338993978564e-15f;

    Reg::RegTensor<float> zReg;
    Reg::RegTensor<float> numeratorReg;
    Reg::RegTensor<float> denominatorReg;
    Reg::Adds(zReg, rReg, -5.0f, mask);

    Reg::Duplicate(numeratorReg, E7, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E6, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E5, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E4, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E3, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E2, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E1, mask);
    MulAdds(numeratorReg, numeratorReg, zReg, E0, mask);

    Reg::Duplicate(denominatorReg, F7, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F6, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F5, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F4, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F3, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F2, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F1, mask);
    MulAdds(denominatorReg, denominatorReg, zReg, F0, mask);
    Reg::Div(dstReg, numeratorReg, denominatorReg, mask);
}

__simd_callee__ inline void Compute(Reg::RegTensor<float>& dstReg, Reg::RegTensor<float>& srcReg, Reg::MaskReg mask)
{
    Reg::RegTensor<float> qReg;
    Reg::RegTensor<float> absQReg;
    Reg::RegTensor<float> centralReg;
    Reg::RegTensor<float> tailProbabilityReg;
    Reg::RegTensor<float> logInputReg;
    Reg::RegTensor<float> tailRadiusReg;
    Reg::RegTensor<float> nearTailReg;
    Reg::RegTensor<float> farTailReg;
    Reg::RegTensor<float> tailReg;
    Reg::RegTensor<float> tmpReg;
    Reg::RegTensor<float> positiveInfReg;
    Reg::RegTensor<float> negativeInfReg;
    Reg::RegTensor<float> nanReg;
    Reg::MaskReg centerMask;
    Reg::MaskReg nearTailMask;
    Reg::MaskReg negativeMask;
    Reg::MaskReg specialMask;
    Reg::MaskReg nanMask;

    Reg::Adds(qReg, srcReg, -0.5f, mask);
    Reg::Abs(absQReg, qReg, mask);
    ComputeTaskSeries(centralReg, qReg, mask);

    Reg::Muls(tmpReg, srcReg, -1.0f, mask);
    Reg::Adds(tmpReg, tmpReg, 1.0f, mask);
    Reg::Min(tailProbabilityReg, srcReg, tmpReg, mask);
    // Scale before Ln so float32 subnormal probabilities remain representable. Restore the log scale afterwards.
    Reg::Muls(logInputReg, tailProbabilityReg, NDTRI_TAIL_PROBABILITY_SCALE, mask);
    Reg::Ln(tailRadiusReg, logInputReg, mask);
    Reg::Adds(tailRadiusReg, tailRadiusReg, NDTRI_TAIL_LOG_SCALE_OFFSET, mask);
    Reg::Neg(tailRadiusReg, tailRadiusReg, mask);
    Reg::Sqrt(tailRadiusReg, tailRadiusReg, mask);
    ComputeNearTail(nearTailReg, tailRadiusReg, mask);
    ComputeFarTail(farTailReg, tailRadiusReg, mask);

    Reg::CompareScalar<float, CMPMODE::LE>(nearTailMask, tailRadiusReg, NDTRI_NEAR_TAIL_BOUNDARY, mask);
    Reg::Select(tailReg, nearTailReg, farTailReg, nearTailMask);
    Reg::CompareScalar<float, CMPMODE::LT>(negativeMask, qReg, 0.0f, mask);
    Reg::Neg(tmpReg, tailReg, mask);
    Reg::Select(tailReg, tmpReg, tailReg, negativeMask);

    Reg::CompareScalar<float, CMPMODE::LE>(centerMask, absQReg, NDTRI_CENTER_REGION_BOUNDARY, mask);
    Reg::Select(dstReg, centralReg, tailReg, centerMask);

    Reg::Duplicate(positiveInfReg, POSITIVE_INFINITY.f32, mask);
    Reg::Duplicate(negativeInfReg, NEGATIVE_INFINITY.f32, mask);
    Reg::Duplicate(nanReg, QUIET_NAN.f32, mask);

    Reg::CompareScalar<float, CMPMODE::EQ>(specialMask, srcReg, 0.0f, mask);
    Reg::Select(dstReg, negativeInfReg, dstReg, specialMask);
    Reg::CompareScalar<float, CMPMODE::EQ>(specialMask, srcReg, 1.0f, mask);
    Reg::Select(dstReg, positiveInfReg, dstReg, specialMask);

    Reg::CompareScalar<float, CMPMODE::LT>(specialMask, srcReg, 0.0f, mask);
    Reg::CompareScalar<float, CMPMODE::GT>(nearTailMask, srcReg, 1.0f, mask);
    Reg::MaskOr(specialMask, specialMask, nearTailMask, mask);
    Reg::Compare<float, CMPMODE::NE>(nanMask, srcReg, srcReg, mask);
    Reg::MaskOr(specialMask, specialMask, nanMask, mask);
    Reg::Select(dstReg, nanReg, dstReg, specialMask);
}

template <bool isReuseSource = false>
__simd_vf__ inline void NdtriImpl(__ubuf__ float* dstUb, __ubuf__ float* srcUb, uint32_t calCount)
{
    constexpr uint32_t elementsPerRepeat = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    const uint32_t repeatTimes = CeilDivision(calCount, elementsPerRepeat);
    Reg::RegTensor<float> dstReg;
    Reg::RegTensor<float> srcReg;
    for (uint32_t i = 0; i < repeatTimes; ++i) {
        Reg::MaskReg mask = Reg::UpdateMask<float>(calCount);
        Reg::LoadAlign(srcReg, srcUb + i * elementsPerRepeat);
        Compute(dstReg, srcReg, mask);
        Reg::StoreAlign(dstUb + i * elementsPerRepeat, dstReg, mask);
    }
}
} // namespace NdtriInternal

template <typename T, bool isReuseSource = false>
__aicore__ inline void NdtriImpl(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    CHECK_FUNC_HIGHLEVEL_API(Ndtri, (T, isReuseSource), (dstTensor, srcTensor, calCount));
    static_assert(SupportType<T, float>(), "Ndtri only supports float data type on current device!");
    CheckTensorPosition(dstTensor, "dstTensor", "VECIN, VECOUT, VECCALC");
    CheckTensorPosition(srcTensor, "srcTensor", "VECIN, VECOUT, VECCALC");
    CheckCalCount(calCount, "calCount", srcTensor, "srcTensor", "Ndtri");
    CheckCalCount(calCount, "calCount", dstTensor, "dstTensor", "Ndtri");
    if (calCount == 0) {
        return;
    }

    __ubuf__ float* dstUb = (__ubuf__ float*)dstTensor.GetPhyAddr();
    __ubuf__ float* srcUb = (__ubuf__ float*)srcTensor.GetPhyAddr();
    NdtriInternal::NdtriImpl<isReuseSource>(dstUb, srcUb, calCount);
}

} // namespace AscendC

#endif // IMPL_MATH_NDTRI_NDTRI_3510_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_3510_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_NDTRI_NDTRI_3510_IMPL_H__
#endif
