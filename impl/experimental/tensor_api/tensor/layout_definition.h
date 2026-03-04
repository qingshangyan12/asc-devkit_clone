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
 * \file layout_definition.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_DEFINITION_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_DEFINITION_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/tensor/layout_size.h"
#include "impl/experimental/tensor_api/tensor/tuple_impl.h"

namespace AscendC {
namespace Te {

// struct layout
template <typename... Shapes>
using Shape = Std::tuple<Shapes...>;

template <typename... Strides>
using Stride = Std::tuple<Strides...>;

template <typename... Layouts>
using Tile = Std::tuple<Layouts...>;

template <typename... Coords>
using Coord = Std::tuple<Coords...>;

template <typename T, typename U>
struct Layout : private Std::tuple<T, U>
{
    static constexpr auto size = StaticLayoutSize<T, U>::size;

    __aicore__ inline constexpr Layout(const T& shape  = {}, const U& stride = {})
        : Std::tuple<T, U>(shape, stride)
    {
        static_assert(Std::is_tuple_v<T> && Std::is_tuple_v<U>, "Shape or Stride is not tuple!");
    }

    __aicore__ inline constexpr decltype(auto) Capacity() const
    {
        return GetCapacity(Shape(), Stride());
    }

    __aicore__ inline constexpr decltype(auto) layout()
    {
        return *this;
    }

    __aicore__ inline constexpr decltype(auto) layout() const
    {
        return *this;
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Shape()
    {
        return GetValue<0, I...>(static_cast<Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Shape() const
    {
        return GetValue<0, I...>(static_cast<const Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Stride()
    {
        return GetValue<1, I...>(static_cast<Std::tuple<T, U>&>(*this));
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Stride() const
    {
        return GetValue<1, I...>(static_cast<const Std::tuple<T, U>&>(*this));
    }

    template <typename S>
    __aicore__ inline constexpr auto operator()(const S& coord) const
    {
        return Crd2Idx(coord, *this);
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Rank() const
    {
        static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<U>, "The dimensions of the Shape and Stride are not the same.");
        return GetRank<I...>(Shape());
    }

    template <size_t... I>
    __aicore__ inline constexpr decltype(auto) Size() const
    {
        return TupleSize<I...>(Shape());
    }

    static constexpr auto depth = nesting_depth_v<T>;
    static constexpr auto rank = Std::tuple_size_v<T>;
};

template <typename T>
struct is_layout : Std::false_type {};

template <typename T, typename U>
struct is_layout<Layout<T, U>> : Std::true_type {};

template <typename T>
constexpr bool is_layout_v = is_layout<T>::value;

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride);

template <typename T, typename U, typename S, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxTTT(const T& coord, const U& shape, const S& stride,
    Std::index_sequence<Is...>)
{
    return (... + Crd2Idx(Std::get<Is>(coord), Std::get<Is>(shape), Std::get<Is>(stride)));
}

template <typename T, typename U, typename S, size_t I0, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxITT(const T& coord, const U& shape, const S& stride,
    Std::index_sequence<I0,Is...>)
{
    if constexpr (sizeof...(Is) == 0) {  // Avoid recursion and mod on single/last iter
        return Crd2Idx(coord, Std::get<I0>(shape), Std::get<I0>(stride));
    } else if constexpr (Std::is_constant<0, T>::value) {
        return Crd2Idx(Std::Int<0>{}, Std::get<I0>(shape), Std::get<I0>(stride)) +
            (Std::Int<0>{} + ... + Crd2Idx(Std::Int<0>{}, Std::get<Is>(shape), Std::get<Is>(stride)));
    } else { // General case
        auto prod = Product{}(Std::get<I0>(shape));
        auto div = coord / prod;
        auto mod = coord % prod;
        return Crd2Idx(mod, Std::get<I0>(shape), Std::get<I0>(stride)) +
            Crd2IdxITT(div, shape, stride, Std::index_sequence<Is...>{});
    }
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride)
{
    if constexpr (Std::is_tuple_v<T>) {
        if constexpr (Std::is_tuple_v<U>) { // tuple tuple tuple
            static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<U>, "Shape and Coord Mismatched Ranks");
            static_assert(Std::tuple_size_v<T> == Std::tuple_size_v<S>, "Stride and Coord Mismatched Ranks");
            return Crd2IdxTTT(coord, shape, stride, tuple_sequence<T>{});
        } else { // tuple "int" "int"
            static_assert(sizeof(T) == 0, "Invalid parameters, U is not tuple!");
        }
    } else {
        if constexpr (Std::is_tuple_v<U>) { // "int" tuple tuple
            static_assert(Std::tuple_size_v<U> == Std::tuple_size_v<S>, "Shape and Stride Mismatched Ranks");
            return Crd2IdxITT(coord, shape, stride, tuple_sequence<U>{});
        } else { // "int" "int" "int"
            return coord * stride;
        }
    }
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Layout<U, S>& layout)
{
    return Crd2Idx(coord, layout.Shape(), layout.Stride());
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_DEFINITION_H
