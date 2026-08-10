/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

#ifndef INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H
#define INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H

#include <cstddef>
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

template <std::size_t N = 1, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(DataType value, reg_tensor<bool> mask = all_mask<DataType>());

template <std::size_t N = 1, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(reg_tensor<DataType> value,
                                                 reg_tensor<bool> mask = all_mask<DataType>());

} // namespace te
} // namespace asc

#include "impl/tensor_api/experimental/reg/mask_impl.h"

#endif // INCLUDE_TENSOR_API_EXPERIMENTAL_REG_MASK_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
