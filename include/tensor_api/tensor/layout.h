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
 * \file layout.h
 * \brief Defines Layout types and construction functions.
 * \brief Layout describes tensor shape, stride, and coordinate mapping.
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#include "impl/tensor_api/tensor/layout_impl.h"

namespace AscendC {
namespace Te {

/*
 * @brief Creates a Shape from dimension sizes.
 * @example
 * // Create 2D shape 128x64
 * auto shape = MakeShape(128, 64);
 * // Create 3D shape 16x128x64
 * auto shape = MakeShape(16, 128, 64);
 */
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t);

/*
 * @brief Creates a Stride from stride values.
 * @example
 * // Create row-major stride (64, 1)
 * auto stride = MakeStride(64, 1);
 * // Create column-major stride (1, 128)
 * auto stride = MakeStride(1, 128);
 */
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t);

/*
 * @brief Creates a Tile for block operations.
 * @example
 * // Create tile 16x16 for block processing
 * auto tile = MakeTile(16, 16);
 */
template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...> MakeTile(const Ts&... t);

/*
 * @brief Creates a Coord from coordinate values.
 * @example
 * // Create coordinate (row=1, col=2)
 * auto coord = MakeCoord(1, 2);
 */
template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t);

/*
 * @brief Creates a Layout from shape and stride.
 * @example
 * // Create row-major layout for 128x64 tensor
 * auto layout = MakeLayout(MakeShape(128, 64), MakeStride(64, 1));
 */
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);

/*
 * @brief Creates a Layout from shape with default stride.
 * @example
 * // Create layout with auto stride (row-major)
 * auto layout = MakeLayout(MakeShape(128, 64));  // Stride=(64,1)
 */
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

/*
 * @brief Converts coordinate to linear index.
 * @example
 * // Convert (1, 2) to index with stride (64, 1)
 * auto idx = Crd2Idx(MakeCoord(1, 2), MakeShape(128, 64), MakeStride(64, 1));  // idx=66
 */
template <typename T, typename Shape, typename Stride>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Shape& shape, const Stride& stride);

/*
 * @brief Gets element at index I from tuple.
 * @example
 * // Get first dimension from shape
 * auto dim0 = Get<0>(shape);  // Returns 128
 */
template <size_t I, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Gets elements at multiple indices from tuple.
 * @example
 * // Get first two dimensions
 * auto dims = Get<0, 1>(shape);
 */
template <size_t I0, size_t I1, size_t... Is, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Gets all elements from tuple.
 */
template <typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Computes cosize of layout dimensions.
 * @example
 * // Get total elements
 * auto size = Cosize(layout);  // Returns 8192 for 128x64
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout);

/*
 * @brief Returns rank (number of dimensions) of layout.
 * @example
 * // Get dimensionality
 * auto rank = Rank(layout);  // Returns 2
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Rank(const LayoutType& layout);

/*
 * @brief Selects subset of layout dimensions.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Select(const LayoutType& layout);

/*
 * @brief Gets dimension values from layout.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Get(const LayoutType& layout);

/*
 * @brief Computes size of layout dimensions.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Size(const LayoutType& layout);

/*
 * @brief Computes capacity of layout.
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout);

/*
 * @brief Creates a sliced tensor view from original tensor.
 */
template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const Coord& coord, const Info& info);

/*
 * @brief Creates a frame layout for tiled processing.
 */
template <typename LayoutPattern, typename TraitType,
    Std::enable_if_t<!IsIntegralConstantV<TraitType>, int> Enable, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args);

/*
 * @brief Creates a frame layout with integral constant parameters.
 */
template <typename LayoutPattern, typename IntType,
    Std::enable_if_t<IsIntegralConstantV<IntType>, int> Enable, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif