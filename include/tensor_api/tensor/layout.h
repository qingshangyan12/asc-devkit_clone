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
 * \brief Provides fundamental abstractions for tensor memory organization.
 * 
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_H 
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_H 

#include "impl/tensor_api/tensor/layout_impl.h"
namespace AscendC {
namespace Te {

// layout construction
/*
 * @brief Creates a Shape from dimension sizes.
 * @brief Shape defines tensor dimensions (e.g., 128x64 for 2D tensor).
 * @param [in] t Dimension sizes
 * @return Shape object
 * @example
 * // Example: Create 2D shape 128x64
 * auto shape = MakeShape(128, 64);
 */
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t);

/*
 * @brief Creates a Stride from stride values.
 * @brief Stride defines memory offset between consecutive elements in each dimension.
 * @param [in] t Stride values for each dimension
 * @return Stride object
 * @example
 * // Example: Create stride for row-major layout (128, 1)
 * auto stride = MakeStride(64, 1);
 */
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t);

/*
 * @brief Creates a Tile for block operations.
 * @brief Tile defines block dimensions for tiled tensor processing.
 * @param [in] t Tile dimensions
 * @return Tile object
 */
template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...>  MakeTile(const Ts&... t);

/*
 * @brief Creates a Coord from coordinate values.
 * @brief Coord represents a position in tensor dimensions (e.g., row=1, col=2).
 * @param [in] t Coordinate values for each dimension
 * @return Coord object
 * @example
 * // Example: Create coordinate (row=1, col=2)
 * auto coord = MakeCoord(1, 2);
 */
template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t);

/*
 * @brief Creates a Layout from shape and stride.
 * @brief Layout combines Shape and Stride to define tensor memory organization.
 * @brief Maps coordinates to memory offsets: offset = coord[i] * stride[i].
 * @param [in] shape Tensor shape dimensions
 * @param [in] stride Memory stride for each dimension
 * @return Layout object
 * @example
 * // Example: Create row-major layout for 128x64 tensor
 * auto layout = MakeLayout(MakeShape(128, 64), MakeStride(64, 1));
 */
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);

/*
 * @brief Creates a Layout from shape with default stride.
 * @brief Automatically generates row-major stride (last dimension stride=1).
 * @param [in] shape Tensor shape dimensions
 * @return Layout object with row-major stride
 * @example
 * // Example: Create layout with auto stride
 * auto layout = MakeLayout(MakeShape(128, 64));  // Stride=(64,1) auto-generated
 */
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

/*
 * @brief Converts coordinate to linear index using shape and stride.
 * @brief Computes memory offset from multi-dimensional coordinate.
 * @brief Formula: index = sum(coord[i] * stride[i]).
 * @param [in] coord Coordinate in tensor dimensions
 * @param [in] shape Tensor shape
 * @param [in] stride Memory stride
 * @return Linear index (memory offset)
 * @example
 * // Example: Convert coordinate (1, 2) to index
 * auto idx = Crd2Idx(MakeCoord(1, 2), MakeShape(128, 64), MakeStride(64, 1));  // idx=66
 */
template <typename T, typename Shape, typename Stride> 
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const Shape& shape, const Stride& stride);

/*
 * @brief Gets element at index I from tuple.
 * @brief Accesses single element from Shape, Stride, or Coord tuple.
 * @param [in] t Tuple object (Shape, Stride, Coord)
 * @return Element at index I
 * @example
 * // Example: Get first dimension from shape
 * auto dim0 = Get<0>(shape);  // Returns 128 for MakeShape(128, 64)
 */
template <size_t I, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Gets elements at multiple indices from tuple.
 * @brief Accesses multiple elements from Shape, Stride, or Coord.
 * @param [in] t Tuple object
 * @return Sub-tuple with elements at specified indices
 */
template <size_t I0, size_t I1, size_t... Is, typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Gets all elements from tuple.
 * @brief Returns entire tuple contents.
 * @param [in] t Tuple object
 * @return Tuple elements
 */
template <typename Tuple, typename Enable>
__aicore__ inline constexpr auto Get(Tuple&& t);

/*
 * @brief Computes cosize of layout dimensions.
 * @brief Returns product of dimension sizes (total elements).
 * @param [in] layout Layout object
 * @return Cosize (total number of elements)
 * @example
 * // Example: Get total elements in 128x64 tensor
 * auto size = Cosize(layout);  // Returns 8192
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout);

/*
 * @brief Returns rank (number of dimensions) of layout.
 * @brief Gets dimensionality of tensor layout.
 * @param [in] layout Layout object
 * @return Rank (number of dimensions)
 * @example
 * // Example: Get rank of 2D layout
 * auto rank = Rank(layout);  // Returns 2 for 128x64 tensor
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Rank(const LayoutType& layout);

/*
 * @brief Selects subset of layout dimensions.
 * @brief Creates new layout from selected dimensions.
 * @param [in] layout Layout object
 * @return Sub-layout with selected dimensions
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Select(const LayoutType& layout);

/*
 * @brief Gets dimension values from layout.
 * @brief Returns shape values for specified dimensions.
 * @param [in] layout Layout object
 * @return Dimension sizes
 * @example
 * // Example: Get dimension 0 size
 * auto size0 = Get<0>(layout);  // Returns 128
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Get(const LayoutType& layout);

/*
 * @brief Computes size of layout dimensions.
 * @brief Returns product of specified dimension sizes.
 * @param [in] layout Layout object
 * @return Size (product of dimension sizes)
 * @example
 * // Example: Get size of first dimension
 * auto size = Size<0>(layout);  // Returns 128
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Size(const LayoutType& layout);

/*
 * @brief Computes capacity of layout (maximum elements).
 * @brief Returns maximum number of elements that can be stored.
 * @param [in] layout Layout object
 * @return Capacity (maximum elements)
 */
template <size_t... Is, typename LayoutType, typename Enable>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout);

/*
 * @brief Creates a sliced tensor view from original tensor.
 * @brief Extracts sub-tensor specified by coordinate range.
 * @brief Provides view into tensor without copying data.
 * @param [in] tensor Original tensor
 * @param [in] coord Coordinate defining slice range
 * @param [in] info Slice information (dimensions to slice)
 * @return Sliced tensor view
 * @example
 * // Example: Slice first 32 rows from 128x64 tensor
 * auto sliced = Slice(tensor, MakeCoord(0, 0), MakeTile(32, 64));
 */
template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const Coord& coord, const Info& info);

/*
 * @brief Creates a frame layout for tiled processing.
 * @brief Frame layout defines block structure for tiled tensor operations.
 * @param [in] args Frame layout parameters (tile sizes, etc.)
 * @return Frame layout object
 */
template <typename LayoutPattern, typename TraitType,
    Std::enable_if_t<!IsIntegralConstantV<TraitType>, int> Enable, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args);

/*
 * @brief Creates a frame layout with integral constant parameters.
 * @brief Compile-time frame layout for tiled operations.
 * @param [in] args Integral constant tile parameters
 * @return Frame layout object
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
