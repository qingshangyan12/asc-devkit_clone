/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/* !
 * \file kernel_micro_vec_cmpsel_impl.h
 * \brief
 */
#ifndef ASCENDC_MODULE_MICRO_VEC_FUSED_IMPL_H
#define ASCENDC_MODULE_MICRO_VEC_FUSED_IMPL_H

#include "kernel_micro_common_impl.h"
#include "../../../include/micro_api/kernel_micro_struct_intf.h"

namespace AscendC {
namespace MicroAPI {
template <typename T0, typename T1, typename T2, RegLayout layout, typename T3, typename T4>
__simd_callee__ inline void FusedMulsCastImpl(T3& dstReg, T4& srcReg, T2 scalarValue, MaskReg& mask)
{
    ASCENDC_ASSERT(false, {
            KERNEL_LOG(KERNEL_ERROR, "MulsCast api is not supported on current device"); });
}

template <typename T, MaskMergeMode mode, typename U>
__simd_callee__ inline void FusedAbsSubImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(std::is_same_v<T, DefaultType> || std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, uint8_t, int8_t, uint16_t, int16_t, uint32_t, int32_t, half, float, int64_t>(),
                  "current data type is not supported on current device!");
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING>(),
                  "current AbsSub api only supported Mode ZEROING on current device!");
    if constexpr(sizeof(ActualT) == 8) {
        if constexpr (CheckRegTrait<U, RegTraitNumOne>()) {
            MaskReg maskTrait2;
            MaskPack(maskTrait2, mask);
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg0;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoSrcReg1;
            RegTensor<ActualT, RegTraitNumTwo> traitTwoDstReg;
            B64TraitOneToTraitTwo(traitTwoSrcReg0, srcReg0);
            B64TraitOneToTraitTwo(traitTwoSrcReg1, srcReg1);
            Sub(traitTwoDstReg, traitTwoSrcReg0, traitTwoSrcReg1, maskTrait2);
            Abs(traitTwoDstReg, traitTwoDstReg, maskTrait2);
            B64TraitTwoToTraitOne(dstReg, traitTwoDstReg);
        } else if constexpr (CheckRegTrait<U, RegTraitNumTwo>()) {
            Sub(dstReg, srcReg0, srcReg1, mask);
            Abs(dstReg, dstReg, mask);
        }
    }
    else {
        constexpr auto modeValue = GetMaskMergeMode<mode>();
        vabsdif(dstReg, srcReg0, srcReg1, mask, modeValue);
    }
}

template <typename T, typename U, RegLayout layout, MaskMergeMode mode, typename S, typename V>
__simd_callee__ inline void FusedExpSubImpl(S& dstReg, V& srcReg0, V& srcReg1, MaskReg& mask)
{
    ASCENDC_ASSERT(false, {
            KERNEL_LOG(KERNEL_ERROR, "ExpSub api is not supported on current device"); });
}

template <typename T, MaskMergeMode mode = MaskMergeMode::ZEROING, typename U>
__simd_callee__ inline void FusedMulDstAddImpl(U& dstReg, U& srcReg0, U& srcReg1, MaskReg& mask)
{
    using ActualT = typename U::ActualT;
    static_assert(std::is_same_v<T, DefaultType> || std::is_same_v<T, ActualT>, "T type is not correct!");
    static_assert(SupportType<ActualT, half, float>(),
                  "current data type is not supported on current device!");
    static_assert(SupportEnum<mode, MaskMergeMode::ZEROING>(), "MulDstAdd only support Mode ZEROING");
    constexpr auto modeValue = GetMaskMergeMode<mode>();
    vmadd(dstReg, srcReg0, srcReg1, mask, modeValue);
}
} // namespace MicroAPI
} // namespace AscendC
#endif // ASCENDC_MODULE_MICRO_VEC_FUSED_IMPL_H