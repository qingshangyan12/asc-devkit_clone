/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_H
#define INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_H

#include <cstddef>

#include "tensor_api/arch/vector/experimental/mask.h"

namespace asc {
namespace te {

template <std::size_t N = 1, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(DataType value, reg_tensor<bool> mask = all_mask<DataType>());

template <std::size_t N = 1, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(reg_tensor<DataType> value,
                                                 reg_tensor<bool> mask = all_mask<DataType>());

} // namespace te
} // namespace asc

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "impl/tensor_api/arch/vector/experimental/fill_impl.h"
#endif

#endif // INCLUDE_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_H
