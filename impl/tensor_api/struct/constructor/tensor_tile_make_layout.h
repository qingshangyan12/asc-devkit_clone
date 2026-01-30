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
 * \file tensor_tile_make_layout.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_STRUCT_CONSTRUCTOR_TENSOR_TILE_MAKE_LAYOUT_H
#define IMPL_TENSOR_API_STRUCT_CONSTRUCTOR_TENSOR_TILE_MAKE_LAYOUT_H

#include "impl/tensor_api/struct/definition/tensor_tile_tensor.h"
#include "impl/tensor_api/utils/tensor_tile_utils.h"
#include "impl/tensor_api/struct/constructor/tensor_tile_tuple_algorithm.h"

namespace AscendC {
namespace TileInternal
{
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShapeImpl(const Ts&... t)
{
    return {t...};
}

template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStrideImpl(const Ts&... t)
{
    return {t...};
}

template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...> MakeTileImpl(const Ts&... t)
{
    return {t...};
}

template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoordImpl(const Ts&... t)
{
    return {t...};
}

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayoutImpl(const T& shape, const U& stride)
{
    static_assert(Std::is_tuple_v<T> && Std::is_tuple_v<U>, "Shape or Stride is not tuple!");
    return Layout<T, U>(shape, stride);
}

// shape = ((x1, x2, ..., xn), (y1, y2, ..., yn))
// stride[0][0] = 1; stride[0][i] = shape[0][i-1]*shape[1][i-1]*stride[0][i-1]; stride[1][i] = shape[0][i]*stride[0][i]
template <size_t I, typename Row, typename Col>
struct StrideRowElem {
    __aicore__ static inline constexpr auto value(const Row& row, const Col& col) {
        if constexpr (I == 0) {
            return Std::Int<1>{};
        } else {
            return Std::get<I - 1>(row) * Std::get<I - 1>(col) *
                StrideRowElem<I - 1, Row, Col>::value(row, col);
        }
    }
};

template <size_t I, typename Row, typename Col>
struct StrideColElem {
    __aicore__ static inline constexpr auto value(const Row& row, const Col& col) {
        return Std::get<I>(row) * StrideRowElem<I, Row, Col>::value(row, col);
    }
};

template <typename Row, typename Col, size_t... Is>
__aicore__ inline constexpr auto BuildStrideRowImpl(const Row& row, const Col& col,
    Std::index_sequence<Is...>) {
    return MakeStrideImpl(StrideRowElem<Is, Row, Col>::value(row, col)...);
}

template <typename Row, typename Col, size_t... Is>
__aicore__ inline constexpr auto BuildStrideColImpl(const Row& row, const Col& col,
    Std::index_sequence<Is...>) {
    return MakeStrideImpl(StrideColElem<Is, Row, Col>::value(row, col)...);
}

template <typename Shape>
__aicore__ inline constexpr auto ComputeStride(const Shape& shape) {
    static_assert(Std::is_tuple_v<Shape> && Std::tuple_size_v<Shape> == 2,
        "Shape must be tuple of two tuples");
    const auto& row = Std::get<0>(shape);
    const auto& col = Std::get<1>(shape);
    static_assert(Std::tuple_size_v<Std::remove_cvref_t<decltype(row)>> ==
        Std::tuple_size_v<Std::remove_cvref_t<decltype(col)>>,
        "Shape rows must have same length");
    constexpr size_t N = Std::tuple_size_v<Std::remove_cvref_t<decltype(row)>>;
    using Row = Std::remove_cvref_t<decltype(row)>;
    using Col = Std::remove_cvref_t<decltype(col)>;
    auto stride0 = BuildStrideRowImpl(row, col, Std::make_index_sequence<N>{});
    auto stride1 = BuildStrideColImpl(row, col, Std::make_index_sequence<N>{});
    return MakeStrideImpl(stride0, stride1);
}

// shape = (x1, x2, x3, ...) -> stride = (1, x1, x1*x2, x1*x2*x3, ...)
template <size_t I, typename Shape>
struct FlatStrideElem {
    __aicore__ static inline constexpr auto value(const Shape& shape) {
        if constexpr (I == 0) {
            return Std::Int<1>{};
        } else {
            return FlatStrideElem<I - 1, Shape>::value(shape) * Std::get<I - 1>(shape);
        }
    }
};

template <typename Shape, size_t... Is>
__aicore__ inline constexpr auto BuildFlatStrideImpl(const Shape& shape,
    Std::index_sequence<Is...>) {
    return MakeStrideImpl(FlatStrideElem<Is, Shape>::value(shape)...);
}

template <typename Shape>
__aicore__ inline constexpr auto ComputeFlatStride(const Shape& shape) {
    static_assert(Std::is_tuple_v<Shape>, "Shape must be tuple");
    constexpr size_t N = Std::tuple_size_v<Shape>;
    return BuildFlatStrideImpl(shape, Std::make_index_sequence<N>{});
}

template <typename Shape>
__aicore__ inline constexpr auto MakeLayoutImpl(const Shape& shape)
{
    static_assert(Std::is_tuple_v<Shape>, "Shape  is not tuple!");
    using T = Std::remove_cvref_t<decltype(Std::get<0>(shape))>;
    if constexpr(Std::is_tuple_v<T>){
        return MakeLayoutImpl(shape, ComputeStride(shape));
    } else {
        return MakeLayoutImpl(shape, ComputeFlatStride(shape));
    }
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto RankImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::tuple_size_v<Shape> == Std::tuple_size_v<Stride>, "The dimensions of the Shape and Stride are not the same.");
    return layout.template Rank<Is...>();
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetShapeImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return layout.template GetShape<Is...>();
}

template<typename T>
__aicore__ inline constexpr auto GetShapeImpl(const T& shape)
{
    static_assert(Std::is_tuple_v<T> || Std::is_integral_v<T>, "shape is not a tuple or integer");
    return shape;
}

template<size_t I, size_t... Is, typename Tuple>
__aicore__ inline constexpr auto GetShapeImpl(const Tuple& shape)
{
    if constexpr (Std::is_tuple_v<Tuple>) {
        return GetShapeImpl<Is...>(Std::get<I>(shape));
    } else {
        return GetTuple<I,Is...>(shape);
    }
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetStrideImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return layout.template GetStride<Is...>();
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return MakeLayoutImpl(GetTuple<Is...>(layout.GetShape()),
                            GetTuple<Is...>(layout.GetStride()));
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto SelectImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return MakeLayoutImpl(SelectTuple<Is...>(layout.GetShape()),
                            SelectTuple<Is...>(layout.GetStride()));
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto ShapeSizeImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return layout.template ShapeSize<Is...>();
}

template <typename Shape, typename Stride>
__aicore__ inline constexpr auto CapacityImpl(const Layout<Shape, Stride>& layout)
{
    static_assert(Std::is_tuple_v<Shape> && Std::is_tuple_v<Stride>, "Shape or Stride is not tuple!");
    return layout.Capacity();
}

struct CoshapeSum {
    template <typename... Args>
    __aicore__ inline constexpr auto operator()(const Args&... args) const {
        return (Std::Int<0>{} + ... + args);
    }
};

struct CoshapeCompute {
    template <typename T, typename U>
    __aicore__ inline constexpr auto operator()(const T& shape, const U& stride) const {
        if constexpr (Std::is_tuple_v<T> && Std::is_tuple_v<U>) {
            static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<U>, "Mismatched ranks");
            return TileInternal::TransformApply(shape, stride, CoshapeCompute{}, CoshapeSum{});
        } else {
            auto m1Shape = shape - Std::Int<1>{};
            auto absStride = stride < 0 ? -stride : stride;
            return m1Shape * absStride;
        }
    }
};

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto CoshapeImpl(const Layout<Shape, Stride>& layout)
{
    auto shape = GetShapeImpl<Is...>(layout);
    auto stride = GetStrideImpl<Is...>(layout);
    auto coCoord = CoshapeCompute{}(shape, stride);
    return coCoord + Std::Int<1>{};
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto CosizeImpl(const Layout<Shape, Stride>& layout)
{
    return TupleSize(CoshapeImpl<Is...>(layout));
}

// NZ
template <typename T>
__aicore__ inline decltype(auto) MakeLayoutForNZ(size_t row, size_t column)
{
    auto shape = MakeShapeImpl(MakeShapeImpl(Std::Int<FRACTAL_FIXED>{}, row / FRACTAL_FIXED),
        MakeShapeImpl(Std::Int<C0_SIZE / sizeof(T)>{}, column / (C0_SIZE / sizeof(T))));
    auto stride = MakeStrideImpl(MakeStrideImpl(Std::Int<C0_SIZE / sizeof(T)>{}, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>{}),
        MakeStrideImpl(Std::Int<1>{}, C0_SIZE / sizeof(T) * row));
    return MakeLayoutImpl(shape, stride);
}

template <typename T, size_t row, size_t column>
using NZShapeFormat = Shape<Shape<Std::Int<FRACTAL_FIXED>, Std::Int<row / FRACTAL_FIXED>>,
    Shape<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<column / (C0_SIZE / sizeof(T))>>>;

template <typename T, size_t row, size_t column>
using NZStrideFormat = Stride<Stride<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>>,
    Stride<Std::Int<1>, Std::Int<C0_SIZE / sizeof(T) * row>>>;

// ND
template <typename T>
__aicore__ inline decltype(auto) MakeLayoutForND(size_t row, size_t column)
{
    auto shape = MakeShapeImpl(MakeShapeImpl(Std::Int<1>{}, row), MakeShapeImpl(Std::Int<1>{}, column));
    auto stride = MakeStrideImpl(MakeStrideImpl(Std::Int<0>{}, column), MakeStrideImpl(Std::Int<0>{},  Std::Int<1>{}));
    return MakeLayoutImpl(shape, stride);
}

template <typename T, size_t row, size_t column>
using NDShapeFormat = Shape<Shape<Std::Int<1>, Std::Int<row>>, Shape<Std::Int<1>, Std::Int<column>>>;

template <typename T, size_t row, size_t column>
using NDStrideFormat = Stride<Stride<Std::Int<0>, Std::Int<column>>, Stride<Std::Int<0>, Std::Int<1>>>;

// DN
template <typename T>
__aicore__ inline decltype(auto) MakeLayoutForDN(size_t row, size_t column)
{
    auto shape = MakeShapeImpl(MakeShapeImpl(Std::Int<1>{}, row), MakeShapeImpl(Std::Int<1>{}, column));
    auto stride = MakeStrideImpl(MakeStrideImpl(Std::Int<0>{}, Std::Int<1>{}), MakeStrideImpl(Std::Int<0>{}, row));
    return MakeLayoutImpl(shape, stride);
}

template <typename T, size_t row, size_t column>
using DNShapeFormat = Shape<Shape<Std::Int<1>, Std::Int<row>>, Shape<Std::Int<1>, Std::Int<column>>>;

template <typename T, size_t row, size_t column>
using DNStrideFormat = Stride<Stride<Std::Int<0>, Std::Int<1>>, Stride<Std::Int<0>, Std::Int<row>>>;

// ZN
template <typename T>
__aicore__ inline decltype(auto) MakeLayoutForZN(size_t row, size_t  column)
{
    auto shape = MakeShapeImpl(
        MakeShapeImpl(Std::Int<C0_SIZE / sizeof(T)>{}, row / (C0_SIZE / sizeof(T))),
        MakeShapeImpl(Std::Int<FRACTAL_FIXED>{}, column / FRACTAL_FIXED)
    );
    auto stride = MakeStrideImpl(
        MakeStrideImpl(Std::Int<1>{}, C0_SIZE / sizeof(T) * column),
        MakeStrideImpl(Std::Int<C0_SIZE / sizeof(T)>{}, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>{})
    );
    return MakeLayoutImpl(shape, stride);
}

template <typename T, size_t  row, size_t  column>
using ZNShapeFormat = Shape<Shape<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<row / (C0_SIZE / sizeof(T))>>,
    Shape<Std::Int<FRACTAL_FIXED>, Std::Int<column / FRACTAL_FIXED>>>;
template <typename T, size_t  row, size_t  column>
using ZNStrideFormat = Stride<Stride<Std::Int<1>, Std::Int<C0_SIZE / sizeof(T) * column>>,
    Stride<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>>>;

template <typename T>
__aicore__ inline decltype(auto) MakeLayoutForZZ(size_t row, size_t column) {
    auto shape = MakeShapeImpl(MakeShapeImpl(Std::Int<FRACTAL_FIXED>{}, row / FRACTAL_FIXED),
        MakeShapeImpl(Std::Int<C0_SIZE / sizeof(T)>{}, column / (C0_SIZE / sizeof(T))));
    auto stride = MakeStrideImpl(MakeStrideImpl(Std::Int<C0_SIZE / sizeof(T)>{}, FRACTAL_FIXED * column),
        MakeStrideImpl(Std::Int<1>{}, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>{}));
    return MakeLayoutImpl(shape, stride);
}

template <typename T, size_t row, size_t column>
using ZZShapeFormat = Shape<Shape<Std::Int<FRACTAL_FIXED>, Std::Int<row / FRACTAL_FIXED>>,
    Shape<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<column / (C0_SIZE / sizeof(T))>>>;
template <typename T, size_t row, size_t column>
using ZZStrideFormat = Stride<Stride<Std::Int<C0_SIZE / sizeof(T)>, Std::Int<FRACTAL_FIXED * column>>,
    Stride<Std::Int<1>, Std::Int<C0_SIZE / sizeof(T) * FRACTAL_FIXED>>>;

}
} // namespace AscendC
#endif
