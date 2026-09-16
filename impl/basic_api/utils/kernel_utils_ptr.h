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
 * \file kernel_utils_ptr.h
 * \brief Unify Tensor / hardware pointer address extraction for CUBE Basic API pointer overloads.
 */
#ifndef ASCENDC_MODULE_UTILS_PTR_H
#define ASCENDC_MODULE_UTILS_PTR_H

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

#endif
