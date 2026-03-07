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

// shape
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeNzShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<FRACTAL_FIXED>{}, row,
                                 Std::Int<C0_SIZE / sizeof(T)>{}, column);
    } else if (Std::is_integral_v<U>) {
        return NZShapeFormat<T, row, column>{}; 
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeZnShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<C0_SIZE / sizeof(T)>{}, row,
                                 Std::Int<FRACTAL_FIXED>{}, column);
    } else if (Std::is_integral_v<U>) {
        return ZNShapeFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeDNShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<1>{}, row, Std::Int<1>{}, column);
    } else if (Std::is_integral_v<U>) {
        return DNShapeFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeNDShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<1>{}, row, Std::Int<1>{}, column);
    } else if (Std::is_integral_v<U>) {
        return NDShapeFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeZzShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<FRACTAL_FIXED>{}, row,
                                 Std::Int<C0_SIZE / sizeof(T)>{}, column);
    } else if (Std::is_integral_v<U>) {
        return ZZShapeFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleANDShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeNDShape<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleADNShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeDNShape<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleBNDShape(U row, S column) { 
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeNDShape<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleBDNShape(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeDNShape<Std::ignore_t, U, S>(row, column);
    }
}

// make stride 
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeNzStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<C0_SIZE / sizeof(T)>{}, row, 
                             Std::Int<1>{}, column);
    } else if (Std::is_integral_v<U>) {
        return NZStrideFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeZnStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<1>{}, row, Std::Int<C0_SIZE / sizeof(T)>{}, column);
    } else if (Std::is_integral_v<U>) {
        return ZNStrideFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeDNStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<0>{}, row, Std::Int<0>{}, column);
    } else if (Std::is_integral_v<U>) {
        return DNStrideFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeNDStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<0>{}, row, Std::Int<0>{}, column);
    } else if (Std::is_integral_v<U>) {
        return NDStrideFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeZzStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return Make2Params2Tuple(MakeTupleCons{}, Std::Int<C0_SIZE / sizeof(T)>{}, row, Std::Int<1>{}, column);
    } else if (Std::is_integral_v<U>) {
        return ZZStrideFormat<T, row, column>{};
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleANDStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeNDStride<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleADNStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeDNStride<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleBNDStride(U row, S column) { 
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeNDStride<Std::ignore_t, U, S>(row, column);
    }
}

template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeScaleBDNStride(U row, S column) {
    if constexpr (Std::is_same_v<U, size_t>) {
        return MakeDNStride<Std::ignore_t, U, S>(row, column);
    }
}

// make coord
template <typename T, typename U, typename S>
__aicore__ inline decltype(auto) MakeFractalCoord(U point1, S point2) {
    return Make2Params2Tuple(MakeTupleCons{}, Std::Int<0>{}, point1, Std::Int<0>{}, point2);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_FRACTAL_H