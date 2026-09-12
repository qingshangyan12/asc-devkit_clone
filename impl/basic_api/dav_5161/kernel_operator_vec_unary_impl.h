/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/basic_api/dav_5161/kernel_operator_vec_unary_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_vec_intf.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#include "../kernel_utils.h"
#include "kernel_operator_common_impl.h"
#include "../../../include/basic_api/kernel_operator_sys_var_intf.h"
#include "kernel_operator_vec_template_impl.h"
#include "../../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"

namespace AscendC {
namespace Internal {
template <auto func, typename T, typename RegType>
__simd_vf__ inline void VecUnaryLevel2VFImpl(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    RegType srcReg;
    RegType dstReg;
    uint32_t sreg = static_cast<uint32_t>(count);
    Reg::MaskReg mask;
    constexpr uint32_t repeatStride = static_cast<uint32_t>(GetVecLen() / sizeof(T) * RegType::trait.REG_NUM);
    uint16_t repeatTime = static_cast<uint16_t>(CeilDivision(sreg, repeatStride));
    for (uint16_t i = 0; i < repeatTime; ++i) {
        mask = Reg::UpdateMask<T, RegType::trait>(sreg);
        Reg::LoadAlign(srcReg, src + i * repeatStride);
        func(dstReg, srcReg, mask);
        Reg::StoreAlign(dst + i * repeatStride, dstReg, mask);
    }
}

template <auto func, typename T>
__aicore__ inline void VecUnaryLevel2ImplTemplate(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    if constexpr (SupportBytes<T, 8>()) {
        VecUnaryLevel2VFImpl<func, T, Reg::RegTensor<T, Reg::RegTraitNumTwo>>(dst, src, count);
    } else {
        VecUnaryLevel2VFImpl<func, T, Reg::RegTensor<T>>(dst, src, count);
    }
}
} // namespace Internal

template <typename T>
__aicore__ inline void AbsImpl(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    static_assert(
        (SupportType<T, int8_t, half, int16_t, float, int32_t, int64_t>()),
        "current data type is not supported on current device!");
    if constexpr (SupportBytes<T, 8>()) {
        constexpr auto func = Reg::Abs<T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T, Reg::RegTraitNumTwo>>;
        Internal::VecUnaryLevel2ImplTemplate<func, T>(dst, src, count);
    } else {
        constexpr auto func = Reg::Abs<T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T>>;
        Internal::VecUnaryLevel2ImplTemplate<func, T>(dst, src, count);
    }
}

template <typename T>
__aicore__ inline void ReluImpl(__ubuf__ T* dst, __ubuf__ T* src, const uint32_t count)
{
    static_assert(
        (SupportType<T, half, float, int32_t, int64_t>()), "current data type is not supported on current device!");
    if constexpr (SupportBytes<T, 8>()) {
        constexpr auto func = Reg::Relu<T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T, Reg::RegTraitNumTwo>>;
        Internal::VecUnaryLevel2ImplTemplate<func, T>(dst, src, count);
    } else {
        constexpr auto func = Reg::Relu<T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T>>;
        Internal::VecUnaryLevel2ImplTemplate<func, T>(dst, src, count);
    }
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_UNARY_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_UNARY_IMPL_H__
#endif
