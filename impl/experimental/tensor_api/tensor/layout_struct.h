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

template <typename T>
struct MakeNzLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) const {
         return MakeNzLayout<T, U, S>(row, column);
    }
};

struct MakeL0CLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeL0CLayout<U, S>(row, column);
    }
};

template <typename T>
struct MakeNDLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeNDLayout<T, U, S>(row, column);
    }
};

template <typename T>
struct MakeDNLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeDNLayout<T, U, S>(row, column);
    }
};

template <typename T>
struct MakeZnLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeZnLayout<T, U, S>(row, column);
    }
};

template <typename T>
struct MakeZzLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeZzLayout<T, U, S>(row, column);
    }
};

template <typename T>
struct MakeNnLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) {
        return MakeNnLayout<T, U, S>(row, column);
    }
};

template <typename T>
struct MakeScaleANDLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) { // 不转置(m, scaleK)
        return MakeScaleANDLayout<T, U, S>(row, column); // (m, scaleK)
    }
};

template <typename T>
struct MakeScaleADNLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) { // 转置(m, scaleK)
        return MakeScaleADNLayout<T, U, S>(row, column); // 转置(m, scaleK)
    }
};

template <typename T>
struct MakeScaleBNDLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) { // 不转置(scaleK, n)
        return MakeScaleBNDLayout<T, U, S>(row, column);  // (scaleK, n)
    }
};

template <typename T>
struct MakeScaleBDNLayoutOp {
    template <typename U, typename S>
    __aicore__ inline decltype(auto) operator()(U row, S column) { // 转置(scaleK, n)
        return MakeScaleBDNLayout<T, U, S>(row, column); // (scaleK, n)
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_STRUCT_H