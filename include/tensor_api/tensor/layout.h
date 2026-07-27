/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "layout.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file layout.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#include "impl/tensor_api/tensor/layout_impl.h"

namespace AscendC {
namespace Te {

// layout construction
/**
 * @brief Construct a layout with the specified layout pattern and trait.
 * @param shape : Shape of the layout.
 * @param stride : Stride of the layout.
 */
template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakePatternLayout(const ShapeType& shape, const StrideType& stride);

/**
 * @brief Construct a shape from the specified dimensions.
 * @param t : Dimensions or nested shapes used to construct the shape.
 */
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t);

/**
 * @brief Construct a stride from the specified stride values.
 * @param t : Stride values or nested strides used to construct the stride.
 */
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t);

/**
 * @brief Construct a tile from the specified elements.
 * @param t : Elements used to construct the tile.
 */
template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...> MakeTile(const Ts&... t);

/**
 * @brief Construct a coordinate from the specified coordinate values.
 * @param t : Coordinate values used to construct the coordinate.
 */
template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t);

/**
 * @brief Construct a layout from a shape and a stride.
 * @param shape : Shape of the layout.
 * @param stride : Stride of the layout.
 */
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);

/**
 * @brief Construct a contiguous layout from a shape and an automatically generated stride.
 * @param shape : Shape of the layout.
 */
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

/**
 * @brief Convert a logical coordinate to its linear index.
 * @param coord : Logical coordinate to convert.
 * @param shape : Shape associated with the coordinate.
 * @param stride : Stride used to calculate the linear index.
 */
template <typename T, typename Shape, typename Stride>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Shape& shape, const Stride& stride);

/**
 * @brief Get an element from a tuple by index.
 * @param t : Tuple from which to get the element.
 */
template <size_t I, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Get an element from a nested tuple by an index path.
 * @param t : Nested tuple from which to get the element.
 */
template <size_t I0, size_t I1, size_t... Is, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Get the input tuple without selecting an element.
 * @param t : Tuple to return.
 */
template <typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Calculate the codomain size of a layout or one of its nested sublayouts.
 * @param layout : Layout whose codomain size is calculated.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout);

/**
 * @brief Get the rank of a layout or one of its nested sublayouts.
 * @param layout : Layout whose rank is queried.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Rank(const LayoutType& layout);

/**
 * @brief Select top-level dimensions from a layout and construct a sublayout.
 * @param layout : Layout from which to select dimensions.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Select(const LayoutType& layout);

/**
 * @brief Get a nested sublayout by an index path.
 * @param layout : Layout from which to get the sublayout.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Get(const LayoutType& layout);

/**
 * @brief Calculate the number of elements in a layout or one of its nested sublayouts.
 * @param layout : Layout whose element count is calculated.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Size(const LayoutType& layout);

/**
 * @brief Calculate the storage capacity required by a layout or one of its nested sublayouts.
 * @param layout : Layout whose storage capacity is calculated.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout);

/**
 * @brief Create a tensor slice at the specified coordinate.
 * @param tensor : Tensor to slice.
 * @param coord : Coordinate that identifies the slice origin.
 * @param info : Slice information that describes the retained dimensions.
 */
template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const Coord& coord, const Info& info);

/**
 * @brief Construct a predefined frame layout with a layout trait.
 * @param args : Dimensions used to construct the frame layout.
 */
template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args);

/**
 * @brief Construct a predefined frame layout with an explicit C0 element count.
 * @param args : Dimensions used to construct the frame layout.
 */
template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args);

/**
 * @brief Remove the specified unit-sized dimensions from a layout or tensor.
 * @param x : Layout or tensor to squeeze.
 */
template <size_t... SqueezeDims, typename T, typename Enable>
__aicore__ inline constexpr auto Squeeze(const T& x);

/**
 * @brief Remove unit-sized dimensions selected by an isomorphic pattern.
 * @param x : Layout or tensor to squeeze.
 * @param pattern : Pattern whose marked positions identify dimensions to remove.
 */
template <typename Pattern, typename T, typename Enable>
__aicore__ inline constexpr auto Squeeze(const T& x, const Pattern& pattern);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
