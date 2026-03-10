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
* \file layout_struct.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_STRUCT_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_STRUCT_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/tensor/layout_fractal.h"
namespace AscendC {
namespace Te {
// struct Layout  
template <typename T, typename U, typename S>
struct MakeNzLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeNzLayout<T, U, S>(row, column);
}
};

struct MakeL0CLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeL0CLayout(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeNDLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeNDLayout<T, U, S>(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeDNLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeDNLayout<T, U, S>(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeZnLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeZnLayout<T, U, S>(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeZzLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeZzLayout<T, U, S>(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeNnLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) {
    return MakeNnLayout<T, U, S>(row, column);
}
};

template <typename T, typename U, typename S>
struct MakeScaleANDLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) { // 不转置(m, scaleK)
    return MakeScaleANDLayout<T, U, S>(row, column); // (m, scaleK)
}
};

template <typename T, typename U, typename S>
struct MakeScaleADNLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) { // 转置(m, scaleK)
    return MakeScaleADNLayout<T, U, S>(row, column); // 转置(m, scaleK)
}
};

template <typename T, typename U, typename S>
struct MakeScaleBNDLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) { // 不转置(scaleK, n)
    return MakeScaleBNDLayout<T, U, S>(row, column);  // (scaleK, n)
}
};

template <typename T, typename U, typename S>
struct MakeScaleBDNLayout {
__aicore__ inline decltype(auto) operator()(size_t row, size_t column) { // 转置(scaleK, n)
    return MakeScaleBDNLayout<T, U, S>(row, column); // (scaleK, n)
}
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_FRACTAL_H