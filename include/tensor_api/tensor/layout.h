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
 * @param[in] shape : Shape of the layout.
 * @param[in] stride : Stride of the layout.
 * @return A layout containing the specified pattern, trait, shape, and stride.
 * @note Shape and stride must be tuple types, and the pattern and trait must be compatible.
 */
template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakePatternLayout(const ShapeType& shape, const StrideType& stride);

/**
 * @brief Construct a shape from the specified dimensions.
 * @param[in] t : Dimensions or nested shapes used to construct the shape.
 * @return A shape containing the specified dimensions.
 * @note At least one dimension or nested shape is required; scalar dimensions must be positive integral values.
 */
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t);

/**
 * @brief Construct a stride from the specified stride values.
 * @param[in] t : Stride values or nested strides used to construct the stride.
 * @return A stride containing the specified values.
 * @note At least one stride value or nested stride is required; scalar stride values must be nonnegative integral
 * values.
 */
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t);

/**
 * @brief Construct a tile from the specified elements.
 * @param[in] t : Elements used to construct the tile.
 * @return A tile containing the specified elements.
 */
template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...> MakeTile(const Ts&... t);

/**
 * @brief Construct a coordinate from the specified coordinate values.
 * @param[in] t : Coordinate values used to construct the coordinate.
 * @return A coordinate containing the specified values.
 * @note Coordinate values must be nonnegative and match the rank of the corresponding shape.
 */
template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t);

/**
 * @brief Construct a layout from a shape and a stride.
 * @param[in] shape : Shape of the layout.
 * @param[in] stride : Stride of the layout.
 * @return A layout containing the specified shape and stride.
 * @note Shape and stride must be tuple types with compatible ranks and nesting depths.
 */
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);

/**
 * @brief Construct a contiguous layout from a shape and an automatically generated stride.
 * @param[in] shape : Shape of the layout.
 * @return A layout containing the specified shape and an inferred stride.
 * @note Flat shapes use compact row-major strides; nested shapes use the corresponding nested stride inference.
 */
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

/**
 * @brief Convert a logical coordinate to its linear index.
 * @param[in] coord : Logical coordinate to convert.
 * @param[in] shape : Shape associated with the coordinate.
 * @param[in] stride : Stride used to calculate the linear index.
 * @return The linear index corresponding to the logical coordinate.
 * @note Tuple coordinates, shapes, and strides must have matching ranks.
 */
template <typename T, typename Shape, typename Stride>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Shape& shape, const Stride& stride);

/**
 * @brief Get an element from a tuple by index.
 * @param[in] t : Tuple from which to get the element.
 * @return The tuple element at the specified index.
 * @note The index must be within the tuple rank.
 */
template <size_t I, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Get an element from a nested tuple by an index path.
 * @param[in] t : Nested tuple from which to get the element.
 * @return The tuple element selected by the specified index path.
 * @note Every index in the path must be valid for the corresponding nesting level.
 */
template <size_t I0, size_t I1, size_t... Is, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Get the input tuple without selecting an element.
 * @param[in] t : Tuple to return.
 * @return The input tuple.
 */
template <typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/**
 * @brief Calculate the codomain size of a layout or one of its nested sublayouts.
 * @param[in] layout : Layout whose codomain size is calculated.
 * @return The number of elements in the layout codomain.
 * @note The optional index path must be within the layout rank.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout);

/**
 * @brief Get the rank of a layout or one of its nested sublayouts.
 * @param[in] layout : Layout whose rank is queried.
 * @return The rank of the layout or selected sublayout.
 * @note The optional index path must be within the layout rank.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Rank(const LayoutType& layout);

/**
 * @brief Select top-level dimensions from a layout and construct a sublayout.
 * @param[in] layout : Layout from which to select dimensions.
 * @return A layout containing the selected top-level dimensions.
 * @note Every selected dimension index must be within the layout rank.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Select(const LayoutType& layout);

/**
 * @brief Get a nested sublayout by an index path.
 * @param[in] layout : Layout from which to get the sublayout.
 * @return The sublayout selected by the specified index path.
 * @note Every index in the path must be valid for the corresponding nesting level.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Get(const LayoutType& layout);

/**
 * @brief Calculate the number of elements in a layout or one of its nested sublayouts.
 * @param[in] layout : Layout whose element count is calculated.
 * @return The number of valid elements in the layout or selected sublayout.
 * @note The optional index path must be within the layout rank.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Size(const LayoutType& layout);

/**
 * @brief Calculate the storage capacity required by a layout or one of its nested sublayouts.
 * @param[in] layout : Layout whose storage capacity is calculated.
 * @return The storage capacity of the layout or selected sublayout.
 * @note The optional index path must be within the layout rank.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout);

/**
 * @brief Create a tensor slice at the specified coordinate.
 * @param[in] tensor : Tensor to slice.
 * @param[in] coord : Coordinate that identifies the slice origin.
 * @param[in] info : Slice information that describes the retained dimensions.
 * @return A subtensor that starts at the specified coordinate.
 * @note The coordinate structure must match the source layout, and info must be a compatible shape or layout.
 */
template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const Coord& coord, const Info& info);

/**
 * @brief Construct a predefined frame layout with a layout trait.
 * @param[in] args : Dimensions used to construct the frame layout.
 * @return A frame layout containing the specified pattern and trait.
 * @note The layout pattern, trait, and argument list must form a supported combination.
 */
template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args);

/**
 * @brief Construct a predefined frame layout with an explicit C0 element count.
 * @param[in] args : Dimensions used to construct the frame layout.
 * @return A frame layout containing the specified pattern and C0 element count.
 * @note The layout pattern, C0 element count, and argument list must form a supported combination.
 */
template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args);

/**
 * @brief Remove the specified unit-sized dimensions from a layout or tensor.
 * @param[in] x : Layout or tensor to squeeze.
 * @return A layout or tensor with the selected unit-sized dimensions removed.
 * @note Selected dimensions whose compile-time extent is not 1 are retained.
 */
template <size_t... SqueezeDims, typename T, typename Enable>
__aicore__ inline constexpr auto Squeeze(const T& x);

/**
 * @brief Remove unit-sized dimensions selected by an isomorphic pattern.
 * @param[in] x : Layout or tensor to squeeze.
 * @param[in] pattern : Pattern whose marked positions identify dimensions to remove.
 * @return A layout or tensor with the marked unit-sized dimensions removed.
 * @note The pattern must be isomorphic to the input layout; marked dimensions not equal to 1 are retained.
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
