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

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/tensor/tuple_impl.h"

namespace AscendC {
namespace Te {

template <typename T>
struct nesting_depth {
    static constexpr size_t value = 1;
};

template <>
struct nesting_depth<Std::tuple<>> {
    static constexpr size_t value = 0;
};

template <typename... Args>
struct nesting_depth<Std::tuple<Args...>> {
    static constexpr size_t value = (nesting_depth<Args>::value + ...);
};

template <typename T>
constexpr size_t nesting_depth_v = nesting_depth<T>::value;

template <size_t Dim, typename T, typename U>
struct IsStaticLayout {
private:
    template<typename T1>
    struct include_dynamic_type : Std::true_type {};

    template<size_t v>
    struct include_dynamic_type<Std::Int<v>> : Std::false_type {};

    template <typename... Args>
    struct include_dynamic_type<Std::tuple<Args...>> : Std::bool_constant<(include_dynamic_type<Args>::value || ...)> {};

    __aicore__ inline static constexpr auto TestStaticLayout()
    {
        if constexpr (nesting_depth_v<T> == Dim && 
            !(include_dynamic_type<T>::value || include_dynamic_type<U>::value)) {
            return true;
        }
        return false;
    }
public:
    static constexpr bool value = TestStaticLayout();
};

template<typename T, typename U>
struct StaticLayoutSize {
private:
    __aicore__ inline static constexpr auto GetFourDimStaticLayoutSize() 
    {
        using rowShapeType = typename Std::tuple_element<0, T>::type;
        using colShapeType = typename Std::tuple_element<1, T>::type;
        using rowStrideType = typename Std::tuple_element<0, U>::type;
        using colStrideType = typename Std::tuple_element<1, U>::type;

        using outterRowNumType = typename Std::tuple_element<1, rowShapeType>::type;
        using outterRowStrideType = typename Std::tuple_element<1, rowStrideType>::type;
        using outterColNumType = typename Std::tuple_element<1, colShapeType>::type;
        using outterColStrideType = typename Std::tuple_element<1, colStrideType>::type;

        return (outterRowNumType {} * outterRowStrideType {}) > (outterColNumType {} * outterColStrideType {}) ? 
            (outterRowNumType {} * outterRowStrideType {}) : (outterColNumType {} * outterColStrideType {});
    }

    __aicore__ inline static constexpr auto GetTwoDimStaticLayoutSize() 
    {
        using rowNumType = typename Std::tuple_element<0, T>::type;
        using colNumType = typename Std::tuple_element<1, T>::type;
        using rowStrideType = typename Std::tuple_element<0, U>::type;
        using colStrideType = typename Std::tuple_element<1, U>::type;

        return (rowNumType {} * rowStrideType {}) > (colNumType {} * colStrideType {}) ? 
            (rowNumType {} * rowStrideType {}) : (colNumType {} * colStrideType {});
    }

    __aicore__ inline static constexpr auto GetStaticLayoutSize() {
        if constexpr (IsStaticLayout<FOUR_DIM_DATA, T, U>::value) {
            return GetFourDimStaticLayoutSize();
        } else if constexpr (IsStaticLayout<TWO_DIM_DATA, T, U>::value) {
            return GetTwoDimStaticLayoutSize();
        } else {
            return Std::Int<0>{};
        }
    }
public:
    static constexpr size_t size = GetStaticLayoutSize();
};

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

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_STRUCT_H
