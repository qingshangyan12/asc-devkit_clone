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
#warning \
    "impl/tensor_api/utils/npu_debug_check.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file npu_debug_check.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H
#define IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H

#include "impl/basic_api/kernel_log.h"
#include "impl/tensor_api/tensor/layout_pattern.h"

namespace AscendC {
namespace Te {

template <typename Shape>
__aicore__ inline constexpr bool IsShapeValid(const Shape& shape);

template <typename Shape, size_t... Is>
__aicore__ inline constexpr bool IsShapeValidImpl(const Shape& shape, Std::index_sequence<Is...>)
{
    return (IsShapeValid(Std::get<Is>(shape)) && ...);
}

template <typename Shape>
__aicore__ inline constexpr bool IsShapeValid(const Shape& shape)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<Shape>>) {
        return IsShapeValidImpl(
            shape, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<Shape>>>{});
    } else {
        return shape > 0;
    }
}

template <typename Coord, typename Shape>
__aicore__ inline constexpr bool IsCoordInShape(const Coord& coord, const Shape& shape);

template <typename Coord, typename Shape, size_t... Is>
__aicore__ inline constexpr bool IsCoordInShapeImpl(
    const Coord& coord, const Shape& shape, Std::index_sequence<Is...>)
{
    return (IsCoordInShape(Std::get<Is>(coord), Std::get<Is>(shape)) && ...);
}

template <typename Coord, typename Shape>
__aicore__ inline constexpr bool IsCoordInShape(const Coord& coord, const Shape& shape)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<Coord>>) {
        static_assert(Std::is_tuple_v<Std::remove_cvref_t<Shape>>,
            "The coord and shape must describe compatible dimensions.");
        if constexpr (Std::is_tuple_v<Std::remove_cvref_t<Shape>>) {
            static_assert(Std::tuple_size_v<Std::remove_cvref_t<Coord>> ==
                              Std::tuple_size_v<Std::remove_cvref_t<Shape>>,
                "The coord and shape must have the same rank.");
            if constexpr (Std::tuple_size_v<Std::remove_cvref_t<Coord>> ==
                          Std::tuple_size_v<Std::remove_cvref_t<Shape>>) {
                return IsCoordInShapeImpl(coord, shape,
                    Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<Coord>>>{});
            }
        }
        return false;
    } else if constexpr (Std::is_tuple_v<Std::remove_cvref_t<Shape>>) {
        return coord >= 0 && coord < Product{}(shape);
    } else {
        return coord >= 0 && coord < shape;
    }
}

template <typename Pattern, size_t... Path>
struct IsStaticZeroStrideAllowedAt : Std::false_type {};

template <typename Pattern>
struct IsStaticZeroStrideAllowedAt<Pattern, 0, 0>
    : Std::bool_constant<IsOneOfAttrV<Pattern,
          NDExtLayoutPtn, DNExtLayoutPtn, ScaleANDLayoutPtn,
          ScaleADNLayoutPtn, ScaleBDNLayoutPtn>> {};

template <typename Pattern>
struct IsStaticZeroStrideAllowedAt<Pattern, 1, 0>
    : Std::bool_constant<IsOneOfAttrV<Pattern,
          NDExtLayoutPtn, DNExtLayoutPtn, ScaleANDLayoutPtn,
          ScaleBNDLayoutPtn, ScaleBDNLayoutPtn>> {};

template <typename Pattern>
struct IsStaticZeroStrideAllowedAt<Pattern, 1, 0, 0>
    : IsStaticZeroStrideAllowedAt<Pattern, 0, 0> {};

template <typename Pattern>
struct IsStaticZeroStrideAllowedAt<Pattern, 1, 1, 0>
    : IsStaticZeroStrideAllowedAt<Pattern, 1, 0> {};

template <typename Pattern, typename Shape, typename Stride, size_t... Path>
__aicore__ inline constexpr bool IsStrideValid(
    const Shape& shape, const Stride& stride, Std::index_sequence<Path...>);

template <typename Pattern, typename Shape, typename Stride,
          size_t... Path, size_t... Is>
__aicore__ inline constexpr bool IsStrideValidImpl(
    const Shape& shape, const Stride& stride,
    Std::index_sequence<Path...>, Std::index_sequence<Is...>)
{
    return (IsStrideValid<Pattern>(
        Std::get<Is>(shape), Std::get<Is>(stride),
        Std::index_sequence<Path..., Is>{}) && ...);
}

template <typename Pattern, typename Shape, typename Stride, size_t... Path>
__aicore__ inline constexpr bool IsStrideValid(
    const Shape& shape, const Stride& stride, Std::index_sequence<Path...>)
{
    if constexpr (!IsSameStructureV<Shape, Stride>) {
        static_assert(IsSameStructureV<Shape, Stride>,
            "The shape and stride must describe the same dimensions.");
        return false;
    } else if constexpr (Std::is_tuple_v<Std::remove_cvref_t<Shape>>) {
        return IsStrideValidImpl<Pattern>(shape, stride,
            Std::index_sequence<Path...>{},
            Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<Shape>>>{});
    } else if constexpr (Std::is_constant<0, Std::remove_cvref_t<Stride>>::value) {
        return shape == 1 && IsStaticZeroStrideAllowedAt<Pattern, Path...>::value;
    } else {
        return stride > 0;
    }
}

template <typename LayoutType>
__aicore__ inline constexpr bool IsLayoutStrideValid(const LayoutType& layout)
{
    static_assert(IsLayoutV<LayoutType>, "The checked value must be a tensor layout.");
    return IsStrideValid<GetLayoutPattern<LayoutType>>(
        layout.Shape(), layout.Stride(), Std::index_sequence<>{});
}

template <typename LayoutType, typename Coord, typename Info>
__aicore__ inline void CheckSlice(const LayoutType& layout, const Coord& coord, const Info& info)
{
    ASCENDC_DEBUG_ASSERT((IsShapeValid(layout.Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check source tensor shape in Slice, all source tensor shape sizes must be greater than 0."));
    ASCENDC_DEBUG_ASSERT((IsLayoutStrideValid(layout)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check source tensor stride in Slice, all source tensor strides must be greater than 0; a zero "
        "stride is allowed only when the tensor format defines it and the corresponding shape size is 1."));
    if constexpr (IsLayoutV<Info>) {
        ASCENDC_DEBUG_ASSERT((IsShapeValid(info.Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check slice shape in Slice, all slice shape sizes must be greater than 0."));
    } else {
        ASCENDC_DEBUG_ASSERT((IsShapeValid(info)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check slice shape in Slice, all slice shape sizes must be greater than 0."));
    }
    ASCENDC_DEBUG_ASSERT((IsCoordInShape(coord, layout.Shape())), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
        "Failed to check coord in Slice, coord must be non-negative and within the source tensor shape range."));
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
