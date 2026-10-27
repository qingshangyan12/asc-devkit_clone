/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_NORMALIZATION_3510_LOAD_H
#define IMPL_ADV_API_NORMALIZATION_3510_LOAD_H

#include "../../../../include/basic_api/kernel_basic_intf.h"

namespace AscendC {
namespace Internal {
template <typename T>
__simd_callee__ inline void LoadNormalization3510Input(
    __ubuf__ T* src, Reg::RegTensor<float>& dstReg, Reg::MaskReg& dstPreg, uint32_t srcOffset)
{
    if constexpr (IsSameType<T, half>::value || IsSameType<T, bfloat16_t>::value) {
        Reg::RegTensor<T> srcOrigin;
        Reg::LoadAlign<T, Reg::LoadDist::DIST_UNPACK_B16>(srcOrigin, src + srcOffset);
        Reg::Cast<float, T, layoutZMrgZ>(dstReg, srcOrigin, dstPreg);
    } else { // this branch: only support float
        Reg::LoadAlign(dstReg, src + srcOffset);
    }
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_NORMALIZATION_3510_LOAD_H
