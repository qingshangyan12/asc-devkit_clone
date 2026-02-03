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
 * \file make_fractal.h
 * \brief
 */
#ifndef EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_FRACTAL_H
#define EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_FRACTAL_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/detail/tensor/make_layout_impl.h"

// algorithm
namespace AscendC {
template <typename T>
__aicore__ inline decltype(auto) MakeNZLayout(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForNZ<T>(row, column);
}

template <>
__aicore__ inline decltype(auto) MakeNZLayout<Std::ignore_t>(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForNZ<uint16_t>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeRowMajorLayout(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForND<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeColumnMajorLayout(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForDN<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZNLayout(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForZN<T>(row, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZZLayout(size_t row, size_t column) {
    return TensorInternal::MakeLayoutForZZ<T>(row, column);
}
} // namespace AscendC

#endif // EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_FRACTAL_H
