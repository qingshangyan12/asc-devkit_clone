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
 * \file make_layout.h
 * \brief
 */
#ifndef EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_LAYOUT_H
#define EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_LAYOUT_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/detail/tensor/make_layout_impl.h"
#include "impl/experimental/tensor_api/detail/tensor/make_coord_impl.h"

// algorithm
namespace AscendC {
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t)
{
    return TensorInternal::MakeShapeImpl<Ts...>(t...);
}

template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t)
{
    return TensorInternal::MakeStrideImpl<Ts...>(t...);
}

template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...>  MakeTile(const Ts&... t)
{
    return TensorInternal::MakeTileImpl<Ts...>(t...);
}

template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t)
{
    return TensorInternal::MakeCoordImpl<Ts...>(t...);
}

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)
{
    return TensorInternal::MakeLayoutImpl(shape, stride);
}

template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape)
{
    return TensorInternal::MakeLayoutImpl(shape);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Rank(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::RankImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetShape(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::GetShapeImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetShape(Layout<Shape, Stride>& layout)
{
    return TensorInternal::GetShapeImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetStride(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::GetStrideImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto GetStride(Layout<Shape, Stride>& layout)
{
    return TensorInternal::GetStrideImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Select(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::SelectImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Size(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::ShapeSizeImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Capicity(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::CapicityImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Coshape(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::CoshapeImpl<Is...>(layout);
}

template <size_t... Is, typename Shape, typename Stride>
__aicore__ inline constexpr auto Cosize(const Layout<Shape, Stride>& layout)
{
    return TensorInternal::CosizeImpl<Is...>(layout);
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Layout<U, S>& layout)
{
    return TensorInternal::Crd2IdxImpl(coord, layout);
}

template <typename T, typename Shape, typename Stride>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Shape& shape, const Stride& stride)
{
    return TensorInternal::Crd2IdxImpl(coord, shape, stride);
}
} // namespace AscendC

#endif // EXPERIMENTAL_TENSOR_API_DETAIL_TENSOR_MAKE_LAYOUT_H
