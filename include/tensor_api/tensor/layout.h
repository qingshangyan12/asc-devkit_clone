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

/**
 * @file layout.h
 * @brief Layout utilities for tensor shape and stride.
 * 
 * @details
 * Factory functions for creating Shape, Stride, Layout, and Coord objects.
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_LAYOUT_H
#define INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#include "impl/tensor_api/tensor/layout_impl.h"

namespace AscendC {
namespace Te {

/**
 * @brief Create Shape from dimensions.
 * 
 * @details
 * Constructs Shape object describing data logical shape.
 * Supports hierarchical Shape with nested sub-structures.
 * 
 * @tparam Ts Dimension types
 * @param[in] t Dimension values or sub-structures
 * @return Shape<Ts...> object
 * 
 * @example
 * // Create normal Shape
 * auto shape = MakeShape(10, 20, 30);
 * auto dim0 = Std::get<0>(shape); // dim0 = 10
 * 
 * // Create hierarchical Shape
 * auto fractalShape = MakeShape(MakeShape(16, 8), MakeShape(32, 4));
 * auto innerRow = Std::get<0>(Std::get<0>(fractalShape)); // innerRow = 16
 */
template <typename... Ts>
__aicore__ inline constexpr Shape<Ts...> MakeShape(const Ts&... t);

/**
 * @brief Create Stride from values.
 * 
 * @details
 * Constructs Stride object defining memory intervals between adjacent elements.
 * 
 * @tparam Ts Stride value types
 * @param[in] t Stride values
 * @return Stride<Ts...> object
 * 
 * @example
 * auto stride = MakeStride(1, 100, 200);
 */
template <typename... Ts>
__aicore__ inline constexpr Stride<Ts...> MakeStride(const Ts&... t);

/**
 * @brief Create Layout from shape and stride.
 * 
 * @details
 * Packages Shape and Stride into Layout data structure.
 * 
 * @tparam T Shape type (must be Std::tuple)
 * @tparam U Stride type (must be Std::tuple)
 * @param[in] shape Shape defining logical data shape
 * @param[in] stride Stride defining memory intervals
 * @return Layout object
 * 
 * @example
 * auto shape = MakeShape(10, 20, 30);
 * auto stride = MakeStride(1, 100, 200);
 * auto layout = MakeLayout(shape, stride);
 * int value = Std::get<0>(layout.Shape()); // value = 10
 */
template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride);

/**
 * @brief Create Layout from shape with automatic stride.
 * 
 * @param[in] shape Tensor shape
 * @return Layout object with compact stride
 */
template <typename T>
__aicore__ inline constexpr auto MakeLayout(const T& shape);

/**
 * @brief Create Tile pattern.
 * 
 * @tparam Ts Tile dimension types
 * @param[in] t Tile dimensions
 * @return Tile<Ts...> object
 */
template <typename... Ts>
__aicore__ inline constexpr Tile<Ts...> MakeTile(const Ts&... t);

/**
 * @brief Create coordinate from indices.
 * 
 * @tparam Ts Index types
 * @param[in] t Coordinate indices
 * @return Coord<Ts...> object
 * 
 * @example
 * auto coord = MakeCoord(5, 10);
 */
template <typename... Ts>
__aicore__ inline constexpr Coord<Ts...> MakeCoord(const Ts&... t);

/**
 * @brief Convert coordinate to linear index.
 * 
 * @tparam T Coordinate type
 * @tparam U Shape type
 * @tparam V Stride type
 * @param[in] coord Tensor coordinate
 * @param[in] shape Tensor shape
 * @param[in] stride Tensor stride
 * @return Linear index value
 */
template <typename T, typename U, typename V>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const V& stride);

/**
 * @brief Slice tensor at coordinate with info.
 * 
 * @tparam Tensor Tensor type
 * @tparam Coord Coordinate type
 * @tparam Info Slice info type
 * @param[in] tensor Tensor to slice
 * @param[in] coord Slice coordinate
 * @param[in] info Slice information
 * @return Sliced tensor view
 * 
 * @example
 * auto bigTensor = MakeTensor(MakeL1memPtr<half>(data), MakeNzLayout<half>(64, 64));
 * auto smallTensor = MakeTensor(MakeL1memPtr<half>(data), MakeNzLayout<half>(32, 32));
 * auto tileTensor = bigTensor(MakeCoord(16, 16), smallTensor);
 */
template <typename Tensor, typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const Coord& coord, const Info& info);

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_LAYOUT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif