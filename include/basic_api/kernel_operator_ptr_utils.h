/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file kernel_operator_ptr_utils.h
 * \brief Helpers for Basic API pointer / Tensor dual input.
 */
#ifndef ASCENDC_MODULE_OPERATOR_PTR_UTILS_H
#define ASCENDC_MODULE_OPERATOR_PTR_UTILS_H

#include "kernel_tensor.h"
#include "../utils/std/type_traits.h"

namespace AscendC {

/*!
 * \brief Extract underlying hardware pointer from a bare pointer or Tensor.
 * \tparam U pointer type or LocalTensor / GlobalTensor
 */
template <typename U>
__aicore__ inline auto GetUnderlyingPtr(const U& val)
{
    if constexpr (Std::is_pointer_v<U>) {
        return val;
    } else {
        return val.GetPhyAddr();
    }
}

/*!
 * \brief Element type of a (possibly address-space qualified) pointer.
 */
template <typename PtrT>
using ElemType = Std::remove_pointer_t<Std::remove_cv_t<PtrT>>;

} // namespace AscendC

#endif // ASCENDC_MODULE_OPERATOR_PTR_UTILS_H
