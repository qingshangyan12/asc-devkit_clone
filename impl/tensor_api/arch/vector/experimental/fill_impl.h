/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_IMPL_H
#define IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_IMPL_H

#include "tensor_api/arch/vector/experimental/fill.h"

namespace asc {
namespace te {
namespace detail {

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> duplicate_scalar(DataType value, reg_tensor<bool> mask)
{
    static_assert(N == 1, "fill currently supports only one register result");

    reg_tensor<DataType> result(mask.reg);
    asc_duplicate_scalar(result.reg, value, mask.reg);
    return result;
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> duplicate_register(reg_tensor<DataType> value, reg_tensor<bool> mask)
{
    static_assert(N == 1, "fill currently supports only one register result");

    reg_tensor<DataType> result(mask.reg);
    asc_duplicate(result.reg, value.reg, mask.reg);
    return result;
}

} // namespace detail

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(DataType value, reg_tensor<bool> mask)
{
    return detail::duplicate_scalar<N>(value, mask);
}

template <std::size_t N, typename DataType>
__simd_callee__ inline reg_tensor<DataType> fill(reg_tensor<DataType> value, reg_tensor<bool> mask)
{
    return detail::duplicate_register<N>(value, mask);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_ARCH_VECTOR_EXPERIMENTAL_FILL_IMPL_H
