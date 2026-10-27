/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_ADV_API_NORMALIZATION_3510_TYPE_CHECK_H
#define IMPL_ADV_API_NORMALIZATION_3510_TYPE_CHECK_H

#include "../../../../include/basic_api/kernel_basic_intf.h"

namespace AscendC {
namespace Internal {
template <typename U, typename T, const auto& config>
__aicore__ inline void CheckNormalization3510Types()
{
    static_assert(SupportType<T, half, float, bfloat16_t>(), "current data type is not supported on current device!");
    if constexpr (IsSameType<T, half>::value) {
        static_assert(SupportType<U, half, float>(), "current data type is not supported on current device!");
    } else if constexpr (IsSameType<T, bfloat16_t>::value) {
        static_assert(SupportType<U, bfloat16_t, float>(), "current data type is not supported on current device!");
    } else if constexpr (IsSameType<T, float>::value) {
        static_assert(SupportType<U, float>(), "current data type is not supported on current device!");
    }
    static_assert(config.isOnlyOutput == false, "current value is not supported on current device!");
}
} // namespace Internal
} // namespace AscendC

#endif // IMPL_ADV_API_NORMALIZATION_3510_TYPE_CHECK_H
