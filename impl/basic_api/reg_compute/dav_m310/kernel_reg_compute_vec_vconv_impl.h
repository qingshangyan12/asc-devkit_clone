/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file kernel_reg_compute_vec_vconv_impl.h
 * \brief
 */

#if !defined(_ASCENDC_INCLUDE_INTERNAL_HEADERS_)
#pragma message("kernel_reg_compute_vec_vconv_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use kernel_reg_compute_intf.h and use public functions or variables defined in interface headers files.")
#define _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_VCONV_IMPL__
#endif

#ifndef ASCENDC_MODULE_REG_COMPUTE_VEC_VCONV_IMPL_H
#define ASCENDC_MODULE_REG_COMPUTE_VEC_VCONV_IMPL_H

#include "../../../../include/basic_api/reg_compute/kernel_reg_compute_vec_duplicate_intf.h"

namespace AscendC {
namespace Reg {
constexpr int16_t SHIFT_ONE_BIT = 1;
constexpr int16_t SHIFT_EIGHT_BIT = 8;
constexpr int16_t SHIFT_SEVEN_BIT = 7;
constexpr uint16_t BF16_INF = 0x7f80;
constexpr uint16_t BF16_NAN = 0x7fc0;

template <typename T, typename U, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool partCondition =
        SupportType<Tuple<T, U>, Tuple<uint16_t, uint8_t>, Tuple<int16_t, int8_t>, Tuple<uint32_t, uint16_t>,
        Tuple<uint32_t, int16_t>, Tuple<int32_t, int16_t>, Tuple<float, half>, Tuple<half, uint8_t>, Tuple<half, int8_t>,
        Tuple<float, int16_t>>();
    constexpr bool ppCondition = SupportType<Tuple<T, U>, Tuple<uint32_t, uint8_t>, Tuple<int32_t, int8_t>,
        Tuple<half, int4x2_t>, Tuple<int16_t, int4x2_t>>();
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    if constexpr (partCondition) {
        // vcvt_ii u82u16/s82s16/u162u32/s162u32/s162s32
        // vcvt_ff f162f32
        // vcvt_if u82f16/s82f16/s162f32
        static_assert(SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
            "current cast api RegLayout Mode is not supported on current device!");
        constexpr auto partModeValue = std::integral_constant<::Part, static_cast<::Part>(layoutMode)>();
    } else if constexpr (ppCondition) {
        constexpr auto ppModeValue = std::integral_constant<::Part_T, static_cast<::Part_T>(layoutMode)>();
        if constexpr (SupportType<Tuple<T, U>, Tuple<half, int4x2_t>>()) {
            // vcvt_if s42f16
            vcvt_s42f16(dstReg, srcReg, mask, ppModeValue, modeValue);
        } else if constexpr (SupportType<Tuple<T, U>, Tuple<int16_t, int4x2_t>>()) {
            // vcvt_if s42s16
            vcvt_s42s16(dstReg, srcReg, mask, ppModeValue, modeValue);
        } else {
            // vcvt_ii u82u32/s82s32/
            vcvt(dstReg, srcReg, mask, ppModeValue, modeValue);
        }
    } else {
        static_assert(!(partCondition && ppCondition), "current cast data type is not supported on current device!");
    }
}

template <typename T, typename U, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, layoutMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, layoutMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T, typename U, SatMode satMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool partCondition =
        SupportType<Tuple<T, U>, Tuple<uint8_t, uint16_t>, Tuple<uint8_t, int16_t>, Tuple<uint16_t, uint32_t>,
        Tuple<int16_t, uint32_t>, Tuple<uint16_t, int32_t>, Tuple<int16_t, int32_t>>();
    constexpr bool ppCondition = SupportType<Tuple<T, U>, Tuple<uint8_t, uint32_t>, Tuple<uint8_t, int32_t>,
        Tuple<int4x2_t, int16_t>>();
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(satMode)>();
    if constexpr (partCondition) {
        static_assert(SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
                      "current cast api RegLayout Mode is not supported on current device!");
        // vcvt_ii u162u8/s162u8/u322u16/u322s16/s322u16/s322s16
        constexpr auto partModeValue = std::integral_constant<::Part, static_cast<::Part>(layoutMode)>();
        vcvt(dstReg, srcReg, mask, satModeValue, partModeValue, modeValue);
    } else if constexpr (ppCondition) {
        constexpr auto ppModeValue = std::integral_constant<::Part_T, static_cast<::Part_T>(layoutMode)>();
        if constexpr (SupportType<Tuple<T, U>, Tuple<int4x2_t, int16_t>>()) {
            // vcvt_ii s162s4x2
            RegTensor<half> halfTmp;
            constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
            vcvt(halfTmp, srcReg, mask, roundModeValue, modeValue);
            vcvt_f162s4(dstReg, halfTmp, mask, roundModeValue, satModeValue, ppModeValue, modeValue);
        } else {
            // vcvt_ii u322u8/s322u8
            vcvt(dstReg, srcReg, mask, satModeValue, ppModeValue, modeValue);
        }
    } else {
        static_assert(!(partCondition && ppCondition), "current cast data type is not supported on current device!");
    }
}

template <typename T, typename U, SatMode satMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, satMode, layoutMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, satMode, layoutMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T, typename U, RoundMode roundMode, SatMode satMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool partCondition = SupportType<Tuple<T, U>, Tuple<int16_t, float>, Tuple<uint8_t, half>,
        Tuple<int8_t, half>, Tuple<half, float>>();
    constexpr bool conditionNoneToRint = SupportType<Tuple<T, U>, Tuple<half, float>, Tuple<int8_t, half>,
        Tuple<uint8_t, half>>();
    constexpr bool ppCondition = SupportType<Tuple<T, U>, Tuple<int4x2_t, half>>();
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(satMode)>();
    constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<roundMode>()>();
    if constexpr (partCondition) {
        // vcvt_fi f322s16/f162u8/f162s8
        // vcvt_ff f322f16
        static_assert(SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
                      "current cast api RegLayout Mode is not supported on current device!");
        if constexpr (conditionNoneToRint && (roundMode == RoundMode::CAST_NONE)) {
            constexpr auto roundModeNoneToRint = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
            constexpr auto partModeValue = std::integral_constant<::Part, static_cast<::Part>(layoutMode)>();
            vcvt(dstReg, srcReg, mask, roundModeNoneToRint, satModeValue, partModeValue, modeValue);
        } else {
            constexpr auto partModeValue = std::integral_constant<::Part, static_cast<::Part>(layoutMode)>();
            vcvt(dstReg, srcReg, mask, roundModeValue, satModeValue, partModeValue, modeValue);
        }
    } else if constexpr (ppCondition) {
        constexpr auto ppModeValue = std::integral_constant<::Part_T, static_cast<::Part_T>(layoutMode)>();
        // vcvt_fi f162s4x2
        if constexpr (SupportType<Tuple<T, U>, Tuple<int4x2_t, half>>()) {
            if(roundMode == RoundMode::CAST_NONE) {
                constexpr auto roundModeNoneToRint = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
                vcvt_f162s4(dstReg, srcReg, mask, roundModeNoneToRint, satModeValue, ppModeValue, modeValue);
            } else {
                vcvt_f162s4(dstReg, srcReg, mask, roundModeValue, satModeValue, ppModeValue, modeValue);
            }
        }
    } else {
        static_assert(!(partCondition && ppCondition), "current cast data type is not supported on current device!");
    }
}

template <typename T, typename U, RoundMode roundMode, SatMode satMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, roundMode, satMode, layoutMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, roundMode, satMode, layoutMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T, typename U, RoundMode roundMode, SatMode satMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool partCondition = SupportType<Tuple<T, U>, Tuple<int32_t, float>, Tuple<int16_t, half>>();
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr auto satModeValue =
        std::integral_constant<::RoundingSaturation, static_cast<::RoundingSaturation>(satMode)>();
    constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<roundMode>()>();
    if constexpr (partCondition) {
        // vcvt_fi f322s32/f162s16
        vcvt(dstReg, srcReg, mask, roundModeValue, satModeValue, modeValue);
    } else {
        static_assert(!(partCondition && ppCondition), "current cast data type is not supported on current device!");
    }
}

template <typename T, typename U, RoundMode roundMode, SatMode satMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, roundMode, satMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, roundMode, satMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

template <typename T, typename U, RoundMode roundMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool partCondition = SupportType<Tuple<T, U>, Tuple<int32_t, half>>();
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<roundMode>()>();
    if constexpr (partCondition) {
        // vcvt_fi f162s32
        static_assert(SupportEnum<layoutMode, RegLayout::ZERO, RegLayout::ONE>(),
                      "current cast api RegLayout Mode is not supported on current device!");
        constexpr auto partModeValue = std::integral_constant<::Part, static_cast<::Part>(layoutMode)>();
        vcvt(dstReg, srcReg, mask, roundModeValue, partModeValue, modeValue);
    } else {
        static_assert(!partCondition, "current cast data type is not supported on current device!");
    }
}

template <typename T, typename U, RoundMode roundMode, RegLayout layoutMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, roundMode, layoutMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, roundMode, layoutMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

// vcvt_if s162f16/s322f32
template <typename T, typename U, RoundMode roundMode, MaskMergeMode mode>
__simd_callee__ inline void CastOperator(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    constexpr bool conditionNoneToRint = SupportType<Tuple<T, U>, Tuple<half, int16_t>, Tuple<float, int32_t>>();
    if constexpr (conditionNoneToRint && (roundMode == RoundMode::CAST_NONE)) {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
        vcvt(dstReg, srcReg, mask, roundModeValue, modeValue);
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<roundMode>()>();
        vcvt(dstReg, srcReg, mask, roundModeValue, modeValue);
    }
}

template <typename T, typename U, RoundMode roundMode, MaskMergeMode mode>
__simd_callee__ inline void CastImpl(RegTensor<T>& dstReg, RegTensor<U>& srcReg, MaskReg& mask)
{
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING, MaskMergeMode::MERGING>(),
                  "current Cast api only supported Mode ZEROING/MERGING on current device!");
    static_assert(SupportType<Tuple<T, U>, Tuple<half, int16_t>, Tuple<float, int32_t>>(),
                  "current cast data type is not supported on current device!");
    if constexpr (mode == MaskMergeMode::ZEROING) {
        CastOperator<T, U, roundMode, mode>(dstReg, srcReg, mask);
    } else if constexpr (mode == MaskMergeMode::MERGING) {
        RegTensor<T> dstCopyReg;
        CastOperator<T, U, roundMode, MaskMergeMode::ZEROING>(dstCopyReg, srcReg, mask);
        CopyMerging(dstReg, dstCopyReg, mask);
    }
}

// truncate f162f16/f322f32
template <typename T, RoundMode roundMode, MaskMergeMode mode, typename U>
__simd_callee__ inline void TruncateImpl(U& dstReg, U& srcReg, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING>(),
                  "current trunc api only supported Mode ZEROING on current device!");
    static_assert(SupportType<ActualT, half, float>(),
                  "current trunc data type is not supported on current device!");
    if constexpr (roundMode == RoundMode::CAST_NONE) {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<RoundMode::CAST_RINT>()>();
        vtrc(dstReg, srcReg, roundModeValue, mask, modeValue);
    } else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        constexpr auto roundModeValue = std::integral_constant<::ROUND, GetRound<roundMode>()>();
        vtrc(dstReg, srcReg, roundModeValue, mask, modeValue);
    }
}

template <typename T, typename U, const CastTrait& trait, typename S, typename V>
__simd_callee__ inline void CastImpl(S& dstReg, V& srcReg, MaskReg& mask)
{
    using ActualT = typename S::ActualT;
    using ActualU = typename V::ActualT;
    static_assert(std::is_same_v<T, DefaultType> || std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(std::is_same_v<U, DefaultType> || std::is_same_v<U, ActualU>, "U type is not correct!");
    if constexpr (trait.mrgMode == MaskMergeMode::MERGING) {
        static_assert(SupportEnum<trait.layoutMode, RegLayout::UNKNOWN, RegLayout::ZERO>(),
            "current Cast api only supported MERGING Mode for RegLayout::UNKNOWN/ZERO on current device!");
        static_assert(!(SupportType<ActualT, fp4x2_e2m1_t, fp4x2_e1m2_t, int4x2_t>() ||
                        SupportType<ActualU, fp4x2_e2m1_t, fp4x2_e1m2_t, int4x2_t>()),
            "current Cast api do not support MERGING Mode for fp4x2_e2m1_t/fp4x2_e1m2_t/int4x2_t on current device!");
    }
    constexpr bool layoutMerge = SupportType<Tuple<ActualT, ActualU>, Tuple<uint16_t, uint8_t>, Tuple<int16_t, int8_t>,
        Tuple<uint32_t, uint16_t>, Tuple<uint32_t, int16_t>, Tuple<int32_t, int16_t>, Tuple<float, half>,
        Tuple<half, uint8_t>, Tuple<half, int8_t>, Tuple<float, int16_t>, Tuple<uint32_t, uint8_t>,
        Tuple<int32_t, int8_t>, Tuple<half, int4x2_t>, Tuple<int16_t, int4x2_t>>();
    constexpr bool satLayMergeCast = SupportType<Tuple<ActualT, ActualU>, Tuple<uint8_t, uint16_t>,
        Tuple<uint8_t, int16_t>, Tuple<uint16_t, uint32_t>, Tuple<int16_t, uint32_t>, Tuple<uint16_t, int32_t>,
        Tuple<int16_t, int32_t>, Tuple<uint8_t, uint32_t>, Tuple<uint8_t, int32_t>, Tuple<int4x2_t, int16_t>>();
    constexpr bool rndSatLayoutMergeCast = SupportType<Tuple<ActualT, ActualU>, Tuple<int16_t, float>,
        Tuple<uint8_t, half>, Tuple<int8_t, half>, Tuple<half, float>, Tuple<int4x2_t, half>>();
    constexpr bool rndSatMergeCast =
        SupportType<Tuple<ActualT, ActualU>, Tuple<int32_t, float>, Tuple<int16_t, half>, Tuple<half, bfloat16_t>>();
    constexpr bool rndLayoutMergeCast = SupportType<Tuple<ActualT, ActualU>, Tuple<int32_t, half>>();
    constexpr bool rndMergeCast =
        SupportType<Tuple<ActualT, ActualU>, Tuple<half, int16_t>, Tuple<float, int32_t>>();
    constexpr bool allNotSupport = !(layoutMerge && satLayMergeCast && rndSatLayoutMergeCast && rndSatMergeCast &&
        rndLayoutMergeCast && rndMergeCast);
    if constexpr (layoutMerge) {
        CastImpl<ActualT, ActualU, trait.layoutMode, trait.mrgMode>(dstReg, srcReg, mask);
    } else if constexpr (satLayMergeCast) {
        CastImpl<ActualT, ActualU, trait.satMode, trait.layoutMode, trait.mrgMode>(dstReg, srcReg, mask);
    } else if constexpr (rndSatLayoutMergeCast) {
        CastImpl<ActualT, ActualU, trait.roundMode, trait.satMode, trait.layoutMode, trait.mrgMode>(
            dstReg, srcReg, mask);
    } else if constexpr (rndSatMergeCast) {
        CastImpl<ActualT, ActualU, trait.roundMode, trait.satMode, trait.mrgMode>(dstReg, srcReg, mask);
    } else if constexpr (rndLayoutMergeCast) {
        CastImpl<ActualT, ActualU, trait.roundMode, trait.layoutMode, trait.mrgMode>(dstReg, srcReg, mask);
    } else if constexpr (rndMergeCast) {
        CastImpl<ActualT, ActualU, trait.roundMode, trait.mrgMode>(dstReg, srcReg, mask);
    } else {
        static_assert(allNotSupport, "current cast data type is not supported on current device!");
    }
}
} // namespace Reg
} // namespace AscendC
#endif // ASCENDC_MODULE_REG_COMPUTE_VEC_VCONV_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_VCONV_IMPL__)
#undef _ASCENDC_INCLUDE_INTERNAL_HEADERS_
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_REG_COMPUTE_VEC_VCONV_IMPL__
#endif

