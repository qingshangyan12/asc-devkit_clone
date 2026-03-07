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
#ifndef IMPL_TENSOR_API_TENSOR_MAKE_FRACTAL_H
#define IMPL_TENSOR_API_TENSOR_MAKE_FRACTAL_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/tensor/layout_dispatch.h"

namespace AscendC {
namespace Te {

template <typename T0, typename T1, typename T2, typename T3>
__aicore__ inline decltype(auto) GetTuple(T0&& t0, T1&& t1, T2&& t2, T3&& t3) {
    return Make2Params2Tuple(MakeTupleCons{}, t0, t1, t2, t3);
}

// shape
template <typename T>
__aicore__ inline decltype(auto) MakeNzShape(size_t row, size_t column) {
    return GetTuple(Std::Int<FRACTAL_FIXED>{}, row, Std::Int<C0_SIZE / sizeof(T)>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZnShape(size_t row, size_t column) {
    return GetTuple(Std::Int<C0_SIZE / sizeof(T)>{}, row, Std::Int<FRACTAL_FIXED>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeDNShape(size_t row, size_t column) {
    return GetTuple(Std::Int<1>{}, row, Std::Int<1>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeNDShape(size_t row, size_t column) {
    return GetTuple(Std::Int<1>{}, row, Std::Int<1>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZzShape(size_t row, size_t column) {
    return GetTuple(Std::Int<FRACTAL_FIXED>{}, row, Std::Int<C0_SIZE / sizeof(T)>{}, column);
}

// make stride 
template <typename T>
__aicore__ inline decltype(auto) MakeNzStride(size_t row, size_t column) {
    return GetTuple(Std::Int<C0_SIZE / sizeof(T)>{}, row, Std::Int<1>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZnStride(size_t row, size_t column) {
    return GetTuple(Std::Int<1>{}, row, Std::Int<C0_SIZE / sizeof(T)>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeDNStride(size_t row, size_t column) {
    return GetTuple(Std::Int<0>{}, row, Std::Int<0>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeNDStride(size_t row, size_t column) {
    return GetTuple(Std::Int<0>{}, row, Std::Int<0>{}, column);
}

template <typename T>
__aicore__ inline decltype(auto) MakeZzStride(size_t row, size_t column) {
    return GetTuple(Std::Int<C0_SIZE / sizeof(T)>{}, row, Std::Int<1>{}, column);
}

// make coord
template <typename T>
__aicore__ inline decltype(auto) MakeFractalCoord(size_t row, size_t column) {
    return GetTuple(Std::Int<0>{}, row, Std::Int<0>{}, column);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_FRACTAL_H