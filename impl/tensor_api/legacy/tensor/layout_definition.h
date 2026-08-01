/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_DEFINITION_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_DEFINITION_H

namespace asc {
namespace te {

template <typename... Shapes>
using Shape = shape_type<Shapes...>;

template <typename... Strides>
using Stride = stride_type<Strides...>;

template <typename... Layouts>
using Tile = tile_type<Layouts...>;

template <typename... Coords>
using Coord = coord_type<Coords...>;

template <typename T, typename U, typename Info = Std::ignore_t>
using Layout = layout_type<T, U, Info>;

template <typename T>
using GetLayoutInfo = get_layout_info<T>;

template <typename T>
using GetLayoutInfoT = get_layout_info_t<T>;

template <typename T>
using GetPattern = get_pattern<T>;

template <typename T>
using GetTraitType = get_trait_type<T>;

template <typename T>
using GetLayoutTrait = get_layout_trait<T>;

template <typename T>
using GetLayoutPattern = get_layout_pattern<T>;

template <typename T>
using IsLayout = is_layout<T>;

template <typename T>
constexpr bool IsLayoutV = is_layout_v<T>;

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_DEFINITION_H
