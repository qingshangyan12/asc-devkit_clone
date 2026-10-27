/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_CLASSIFICATION_CONSTANTS_H
#define IMPL_ADV_API_CLASSIFICATION_CONSTANTS_H

#include "../../../../include/basic_api/kernel_basic_intf.h"

namespace AscendC {
namespace Internal {
template <typename T>
__simd_callee__ inline void PrepareClassificationConstants(Reg::RegTensor<T>& vReg0, Reg::RegTensor<T>& vReg1)
{
    if constexpr (Std::is_same_v<T, bool>) {
        Reg::Duplicate((Reg::RegTensor<uint8_t>&)vReg0, 0u);
        Reg::Duplicate((Reg::RegTensor<uint8_t>&)vReg1, 1u);
    } else {
        Reg::Duplicate(vReg0, 0.0f);
        Reg::Duplicate(vReg1, 1.0f);
    }
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_CLASSIFICATION_CONSTANTS_H
