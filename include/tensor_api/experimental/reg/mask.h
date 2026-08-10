/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H

#include <cstdint>

#include "tensor_api/experimental/reg/reg_tensor.h"

namespace asc {
namespace te {

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> all_mask();

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> none_mask();

template <typename DataType>
__simd_callee__ inline reg_tensor<bool> update_mask(uint32_t& remain);

} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/experimental/reg/mask_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H
