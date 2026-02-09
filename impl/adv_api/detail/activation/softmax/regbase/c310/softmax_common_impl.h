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
 * \file softmax_common_impl.h
 * \brief
 */
#ifndef IMPL_ACTIVATION_SOFTMAX_C310_SOFTMAX_COMMON_IMPL_H
#define IMPL_ACTIVATION_SOFTMAX_C310_SOFTMAX_COMMON_IMPL_H

#include "../../../../common/common.h"

namespace AscendC {
template <typename T>
__simd_callee__ inline void LoadIfNeedCast(
    Reg::RegTensor<float>& dstReg, __ubuf__ T* srcUb, Reg::MaskReg& preg)
{
    if constexpr (sizeof(T) == 2) {
        Reg::RegTensor<T> tmpReg;
        Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(tmpReg, srcUb);
        Reg::Cast<float, T, Internal::castTraitB16ToB32>(dstReg, tmpReg, preg);
    } else {
        Reg::LoadAlign<T, Reg::LoadDist::DIST_NORM>(dstReg, srcUb);
    }
}

template <typename T>
__simd_callee__ inline void LoadIfNeedCastM1(
    Reg::RegTensor<float>& dstReg, __ubuf__ T* srcUb, Reg::MaskReg& preg)
{
    if constexpr (sizeof(T) == 2) {
        Reg::RegTensor<T> castVreg;
        Reg::LoadAlign<T, Reg::LoadDist::DIST_BRC_B16>(castVreg, srcUb);
        Reg::UnPack<uint32_t, uint16_t>(
            (Reg::RegTensor<uint32_t>&)castVreg, (Reg::RegTensor<uint16_t>&)castVreg);
        Reg::Cast<float, T, Internal::castTraitB16ToB32>(dstReg, castVreg, preg);
    } else {
        Reg::LoadAlign<float, Reg::LoadDist::DIST_BRC_B32>(dstReg, srcUb);
    }
}

template <typename T>
__simd_callee__ inline void StoreIfNeedCastM1(
    __ubuf__ T* dstUb, Reg::RegTensor<float>& srcReg, Reg::MaskReg& preg)
{
    if constexpr (sizeof(T) == 2) {
        Reg::RegTensor<T> castVreg;
        Reg::Cast<T, float, Internal::castTraitB32ToB16>(castVreg, srcReg, preg);
        Reg::Pack<uint16_t, uint32_t>(
            (Reg::RegTensor<uint16_t>&)castVreg, (Reg::RegTensor<uint32_t>&)castVreg);
        Reg::StoreAlign<T, Reg::StoreDist::DIST_FIRST_ELEMENT_B16>(dstUb, castVreg, preg);
    } else {
        Reg::StoreAlign<float, Reg::StoreDist::DIST_FIRST_ELEMENT_B32>(dstUb, srcReg, preg);
    }
}

template <typename T>
__simd_callee__ inline void StoreIfNeedCast(
    __ubuf__ T* dstUb, Reg::RegTensor<float>& srcReg, Reg::MaskReg& preg)
{
    if constexpr (sizeof(T) == 2) {
        Reg::RegTensor<T> tmpReg;
        Reg::Cast<T, float, Internal::castTraitB32ToB16>(tmpReg, srcReg, preg);
        Reg::StoreAlign<T, Reg::StoreDist::DIST_PACK_B32>(dstUb, tmpReg, preg);
    } else {
        Reg::StoreAlign<T, Reg::StoreDist::DIST_NORM>(dstUb, srcReg, preg);
    }
}

template <typename T>
__simd_callee__ inline void LoadE2B(Reg::RegTensor<T>& dstReg, __ubuf__ T* srcUb)
{
    if constexpr (sizeof(T) == 2) {
        Reg::LoadAlign<T, Reg::LoadDist::DIST_E2B_B16>(dstReg, srcUb);
    } else {
        Reg::LoadAlign<T, Reg::LoadDist::DIST_E2B_B32>(dstReg, srcUb);
    }
}
} // namespace AscendC
#endif // IMPL_ACTIVATION_SOFTMAX_C310_SOFTMAX_COMMON_IMPL_H
