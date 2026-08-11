/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file layout_interface.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H

#include "tensor_api/tensor/layout_type.h"
#include "impl/tensor_api/tensor/tuple_impl.h"
#include "impl/tensor_api/tensor/layout_definition.h"

namespace asc {
namespace te {

template <typename T>
using get_layout_pattern = typename get_pattern<Std::remove_cvref_t<T>>::type;

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto crd2idx(const T& coord, const U& shape, const S& stride);

template <size_t I, size_t... Is, typename Tuple, typename Enable>
__aicore__ inline constexpr auto get(Tuple&& t);

// layout construction
template <typename T, typename... Ts>
__aicore__ inline constexpr shape<T, Ts...> make_shape(const T& t, const Ts&... ts);

template <typename T, typename... Ts>
__aicore__ inline constexpr stride<T, Ts...> make_stride(const T& t, const Ts&... ts);

template <typename T, typename... Ts>
__aicore__ inline constexpr coord<T, Ts...> make_coord(const T& t, const Ts&... ts);

template <typename T, typename U>
__aicore__ inline constexpr auto make_layout(const T& shape, const U& stride);

template <typename T>
__aicore__ inline constexpr auto make_layout(const T& shape);

template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto make_pattern_layout(const ShapeType& shape, const StrideType& stride);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto cosize(const LayoutType& layout);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto rank(const LayoutType& layout);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto select(const LayoutType& layout);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto get(const LayoutType& layout);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto size(const LayoutType& layout);

template <size_t... Is, typename LayoutType, typename Enable = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto capacity(const LayoutType& layout);

template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) slice(Tensor&& tensor, const Coord& coord, const Info& info);

template <typename LayoutPattern, typename TraitType = Std::ignore_t, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args);

template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args);

} // namespace te
} // namespace asc

#include "impl/tensor_api/tensor/layout_impl.h"

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_INTERFACE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
