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
    "impl/basic_api/dav_5161/kernel_operator_vec_binary_scalar_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"basic_api/kernel_tensor.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif

#ifndef ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#define ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#include "../kernel_utils.h"
#include "kernel_operator_common_impl.h"
#include "kernel_operator_vec_template_impl.h"
#include "../../../include/basic_api/reg_compute/kernel_reg_compute_intf.h"

namespace AscendC {
namespace Internal {
template <auto func, typename T, typename RegType>
__simd_vf__ inline void VecBinaryScalarLevel2VFImpl(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint32_t calCount)
{
    RegType srcReg;
    RegType dstReg;
    uint32_t count = static_cast<uint32_t>(calCount);
    Reg::MaskReg mask;
    constexpr uint32_t repeatStride = static_cast<uint32_t>(GetVecLen() / sizeof(T) * RegType::trait.REG_NUM);
    uint16_t repeatTime = static_cast<uint16_t>(CeilDivision(calCount, repeatStride));
    for (uint16_t i = 0; i < repeatTime; ++i) {
        mask = Reg::UpdateMask<T, RegType::trait>(count);
        Reg::LoadAlign(srcReg, src + i * repeatStride);
        func(dstReg, srcReg, scalarValue, mask);
        Reg::StoreAlign(dst + i * repeatStride, dstReg, mask);
    }
}

template <auto func, typename T>
__aicore__ inline void VecBinaryScalarLevel2ImplTemplate(
    __ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const uint32_t calCount)
{
    if constexpr (SupportBytes<T, 8>() || SupportType<T, complex32>()) {
        VecBinaryScalarLevel2VFImpl<func, T, Reg::RegTensor<T, Reg::RegTraitNumTwo>>(dst, src, scalarValue, calCount);
    } else {
        VecBinaryScalarLevel2VFImpl<func, T, Reg::RegTensor<T>>(dst, src, scalarValue, calCount);
    }
}
} // namespace Internal

// Mins::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void MinsImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& calCount)
{
    static_assert(
        (SupportType<T, uint8_t, int8_t, half, bfloat16_t, float, int16_t, int32_t, int64_t, uint64_t>()),
        "Mins not support current datatype!");
    if constexpr (SupportBytes<T, 8>()) {
        constexpr auto func = Reg::Mins<T, T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T, Reg::RegTraitNumTwo>>;
        Internal::VecBinaryScalarLevel2ImplTemplate<func, T>(dst, src, scalarValue, calCount);
    } else {
        constexpr auto func = Reg::Mins<T, T, Reg::MaskMergeMode::ZEROING, Reg::RegTensor<T>>;
        Internal::VecBinaryScalarLevel2ImplTemplate<func, T>(dst, src, scalarValue, calCount);
    }
}
/* **************************************************************************************************
 * LeakyRelu                                             *
 * ************************************************************************************************* */
namespace RegLeakyRelu {
template <typename T, typename RegT>
__simd_callee__ inline void LeakyRelu(RegT& dstReg, RegT& srcReg, T scalarValue, Reg::MaskReg& mask)
{
    vlrelu(dstReg, srcReg, scalarValue, mask, MODE_ZEROING);
}
} // namespace RegLeakyRelu

// LeakyRelu::Level 2
template <typename T, bool isSetMask = true>
__aicore__ inline void LeakyReluImpl(__ubuf__ T* dst, __ubuf__ T* src, T scalarValue, const int32_t& calCount)
{
    static_assert((SupportType<T, half, float>()), "LeakyRelu not support current datatype!");
    constexpr auto func = RegLeakyRelu::LeakyRelu<T, Reg::RegTensor<T>>;
    Internal::VecBinaryScalarLevel2ImplTemplate<func, T>(dst, src, scalarValue, calCount);
}
} // namespace AscendC
#endif // ASCENDC_MODULE_OPERATOR_VEC_BINARY_SCALAR_IMPL_H
#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_KERNEL_OPERATOR_VEC_BINARY_SCALAR_IMPL_H__
#endif
