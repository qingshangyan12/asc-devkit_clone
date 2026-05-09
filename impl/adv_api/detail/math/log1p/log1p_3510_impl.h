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
 * \brief
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

namespace AscendC {
namespace Log1pAPI {

constexpr uint32_t LOG1P_CONST_3F400000 = 0x3F400000u;
constexpr uint32_t LOG1P_CONST_FF800000 = 0xFF800000u;
constexpr uint32_t LOG1P_CONST_40800000 = 0x40800000u;
constexpr uint32_t LOG1P_CONST_7F800000 = 0x7F800000u;
constexpr uint32_t LOG1P_CONST_80000000 = 0x80000000u;

constexpr uint32_t LOG1P_POLY_C0 = 0xBD39BF78u;
constexpr uint32_t LOG1P_POLY_C1 = 0x3DD80012u;
constexpr uint32_t LOG1P_POLY_C2 = 0xBE0778E0u;
constexpr uint32_t LOG1P_POLY_C3 = 0x3E146475u;
constexpr uint32_t LOG1P_POLY_C4 = 0xBE2A68DDu;
constexpr uint32_t LOG1P_POLY_C5 = 0x3E4CAF9Eu;
constexpr uint32_t LOG1P_POLY_C6 = 0xBE800042u;
constexpr uint32_t LOG1P_POLY_C7 = 0x3EAAAAE6u;

constexpr uint32_t LOG1P_LN2 = 0x3F317218u;

constexpr Reg::CastTrait castTraitI32F32 = {
    Reg::RegLayout::ZERO, Reg::SatMode::UNKNOWN, Reg::MaskMergeMode::ZEROING, RoundMode::UNKNOWN};

template <typename T>
__simd_vf__ inline void Log1pCompute(__ubuf__ T* dst, __ubuf__ T* src, uint32_t calCount)
{
    static_assert((std::is_same_v<T, float>), "Log1p only supports float on current device!");
    constexpr uint32_t oneRepSize = static_cast<uint32_t>(GetVecLen() / sizeof(float));
    uint16_t repeatTimes = CeilDivision(calCount, oneRepSize);

    Reg::RegTensor<float> srcReg, x1Reg, tReg, nln2Reg, pReg, resultReg, tmpFReg, specInfReg, negZeroReg;
    Reg::RegTensor<uint32_t> r3Reg, r4Reg, tmpUReg;
    Reg::MaskReg mask, fullMask, maskSpecial, maskSpecialSub1, maskSpecialAndSub1, maskZero, maskSpecialAndZero;

    fullMask = Reg::CreateMask<float, Reg::MaskPattern::ALL>();

    for (uint16_t i = 0; i < repeatTimes; ++i) {
        uint32_t sreg = calCount;
        mask = Reg::UpdateMask<float>(sreg);

        Reg::LoadAlign(srcReg, src + i * oneRepSize);

        Reg::RegTensor<uint32_t>& r1Reg = (Reg::RegTensor<uint32_t>&)srcReg;

        Reg::Adds(x1Reg, srcReg, 1.0f, mask);
        Reg::RegTensor<uint32_t>& r2Reg = (Reg::RegTensor<uint32_t>&)x1Reg;

        Reg::Duplicate<uint32_t>(tmpUReg, LOG1P_CONST_3F400000);
        Reg::Sub(r3Reg, r2Reg, tmpUReg, mask);

        Reg::Duplicate<uint32_t>(tmpUReg, LOG1P_CONST_FF800000);
        Reg::And(r4Reg, r3Reg, tmpUReg, mask);

        Reg::RegTensor<uint32_t> f9UReg;
        Reg::Sub(f9UReg, r1Reg, r4Reg, mask);
        Reg::RegTensor<float>& f9Reg = (Reg::RegTensor<float>&)f9UReg;

        Reg::RegTensor<uint32_t> f10UReg;
        Reg::Duplicate<uint32_t>(tmpUReg, LOG1P_CONST_40800000);
        Reg::Sub(f10UReg, tmpUReg, r4Reg, mask);
        Reg::RegTensor<float>& f10Reg = (Reg::RegTensor<float>&)f10UReg;

        Reg::Duplicate<float>(tReg, 0.25f);
        Reg::Fma(tReg, f10Reg, -1.0f, mask);
        Reg::Add(tReg, tReg, f9Reg, mask);

        Reg::Cast<float, int32_t, castTraitI32F32>(nln2Reg, (Reg::RegTensor<int32_t>&)r4Reg, mask);
        Reg::Muls(nln2Reg, nln2Reg, 1.1920928955078125e-7f, mask);

        Reg::Duplicate((Reg::RegTensor<uint32_t>&)pReg, LOG1P_POLY_C0);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C1);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C2);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C3);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C4);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C5);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C6);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_POLY_C7);
        Reg::Fma(pReg, tReg, tmpFReg, mask);
        Reg::Fma(pReg, tReg, -0.5f, mask);

        Reg::Mul(resultReg, tReg, pReg, mask);
        Reg::Fma(resultReg, tReg, tReg, mask);

        Reg::Duplicate((Reg::RegTensor<uint32_t>&)tmpFReg, LOG1P_LN2);
        Reg::Fma(tmpFReg, nln2Reg, resultReg, mask);

        Reg::CompareScalar<uint32_t, CMPMODE::GE>(maskSpecial, r1Reg, LOG1P_CONST_7F800000, mask);
        Reg::CompareScalar<int32_t, CMPMODE::GE>(maskSpecialSub1, (Reg::RegTensor<int32_t>&)r1Reg,
            -1082130431, mask);
        Reg::And(maskSpecialAndSub1, maskSpecial, maskSpecialSub1, fullMask);

        Reg::Duplicate((Reg::RegTensor<uint32_t>&)specInfReg, LOG1P_CONST_7F800000);
        Reg::Fma(specInfReg, srcReg, specInfReg, maskSpecialAndSub1);

        Reg::CompareScalar<float, CMPMODE::EQ>(maskZero, srcReg, 0.0f, mask);
        Reg::And(maskSpecialAndZero, maskSpecial, maskZero, fullMask);

        Reg::Duplicate((Reg::RegTensor<uint32_t>&)negZeroReg, LOG1P_CONST_80000000);

        Reg::Select(tmpFReg, specInfReg, tmpFReg, maskSpecialAndSub1);
        Reg::Select(tmpFReg, negZeroReg, tmpFReg, maskSpecialAndZero);

        Reg::StoreAlign(dst + i * oneRepSize, tmpFReg, mask);
    }
}

} // namespace Log1pAPI
} // namespace AscendC
#endif // IMPL_MATH_LOG1P_LOG1P_C310_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_C310_IMPL_H__
#endif
