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
 * \file kernel_operator_ptr_utils.h
 * \brief Helpers that unify Tensor and hardware-pointer arguments for Basic API.
 */

#ifndef ASCENDC_MODULE_OPERATOR_PTR_UTILS_H
#define ASCENDC_MODULE_OPERATOR_PTR_UTILS_H

#include "../utils/std/type_traits.h"

#if defined(__clang__) && (__CCE__)
#define ASCENDC_ENABLE_HW_PTR_API 1
#endif

namespace AscendC {
template <typename U>
__aicore__ inline auto GetUnderlyingPtr(const U& val)
{
    if constexpr (Std::is_pointer_v<U>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}
} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_PTR_UTILS_H
