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

#include "impl/tensor_api/utils/npu_debug_assert.h"
#include "impl/tensor_api/tensor/layout_pattern.h"

namespace AscendC {
namespace Te {

template <typename shape_t>
__aicore__ inline constexpr bool is_shape_valid(const shape_t& shape);

template <typename shape_t, size_t... indices>
__aicore__ inline constexpr bool is_shape_valid_impl(const shape_t& shape, Std::index_sequence<indices...>)
{
    return (is_shape_valid(Std::get<indices>(shape)) && ...);
}

template <typename shape_t>
__aicore__ inline constexpr bool is_shape_valid(const shape_t& shape)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<shape_t>>) {
        return is_shape_valid_impl(shape, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<shape_t>>>{});
    } else {
        return shape > 0;
    }
}

template <typename coord_t, typename shape_t>
__aicore__ inline constexpr bool is_coord_in_shape(const coord_t& coord, const shape_t& shape);

template <typename coord_t, typename shape_t, size_t... indices>
__aicore__ inline constexpr bool is_coord_in_shape_impl(
    const coord_t& coord, const shape_t& shape, Std::index_sequence<indices...>)
{
    return (is_coord_in_shape(Std::get<indices>(coord), Std::get<indices>(shape)) && ...);
}

template <typename coord_t, typename shape_t>
__aicore__ inline constexpr bool is_coord_in_shape(const coord_t& coord, const shape_t& shape)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<coord_t>>) {
        static_assert(
            Std::is_tuple_v<Std::remove_cvref_t<shape_t>>, "The coord and shape must describe compatible dimensions.");
        if constexpr (Std::is_tuple_v<Std::remove_cvref_t<shape_t>>) {
            static_assert(
                Std::tuple_size_v<Std::remove_cvref_t<coord_t>> == Std::tuple_size_v<Std::remove_cvref_t<shape_t>>,
                "The coord and shape must have the same rank.");
            if constexpr (
                Std::tuple_size_v<Std::remove_cvref_t<coord_t>> == Std::tuple_size_v<Std::remove_cvref_t<shape_t>>) {
                return is_coord_in_shape_impl(
                    coord, shape, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<coord_t>>>{});
            }
        }
        return false;
    } else if constexpr (Std::is_tuple_v<Std::remove_cvref_t<shape_t>>) {
        return coord >= 0 && coord < Product{}(shape);
    } else {
        return coord >= 0 && coord < shape;
    }
}

template <typename pattern_t, size_t... path>
struct is_static_zero_stride_allowed_at : Std::false_type {};

template <typename pattern_t>
struct is_static_zero_stride_allowed_at<pattern_t, 0, 0>
    : Std::bool_constant<IsOneOfAttrV<
          pattern_t, NDExtLayoutPtn, DNExtLayoutPtn, ScaleANDLayoutPtn, ScaleADNLayoutPtn, ScaleBDNLayoutPtn>> {};

template <typename pattern_t>
struct is_static_zero_stride_allowed_at<pattern_t, 1, 0>
    : Std::bool_constant<IsOneOfAttrV<
          pattern_t, NDExtLayoutPtn, DNExtLayoutPtn, ScaleANDLayoutPtn, ScaleBNDLayoutPtn, ScaleBDNLayoutPtn>> {};

template <typename pattern_t>
struct is_static_zero_stride_allowed_at<pattern_t, 1, 0, 0> : is_static_zero_stride_allowed_at<pattern_t, 0, 0> {};

template <typename pattern_t>
struct is_static_zero_stride_allowed_at<pattern_t, 1, 1, 0> : is_static_zero_stride_allowed_at<pattern_t, 1, 0> {};

template <typename pattern_t, typename shape_t, typename stride_t, size_t... path>
__aicore__ inline constexpr bool is_stride_valid(
    const shape_t& shape, const stride_t& stride, Std::index_sequence<path...>);

template <typename pattern_t, typename shape_t, typename stride_t, size_t... path, size_t... indices>
__aicore__ inline constexpr bool is_stride_valid_impl(
    const shape_t& shape, const stride_t& stride, Std::index_sequence<path...>, Std::index_sequence<indices...>)
{
    return (
        is_stride_valid<pattern_t>(
            Std::get<indices>(shape), Std::get<indices>(stride), Std::index_sequence<path..., indices>{}) &&
        ...);
}

template <typename pattern_t, typename shape_t, typename stride_t, size_t... path>
__aicore__ inline constexpr bool is_stride_valid(
    const shape_t& shape, const stride_t& stride, Std::index_sequence<path...>)
{
    if constexpr (!IsSameStructureV<shape_t, stride_t>) {
        static_assert(IsSameStructureV<shape_t, stride_t>, "The shape and stride must describe the same dimensions.");
        return false;
    } else if constexpr (Std::is_tuple_v<Std::remove_cvref_t<shape_t>>) {
        return is_stride_valid_impl<pattern_t>(
            shape, stride, Std::index_sequence<path...>{},
            Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<shape_t>>>{});
    } else if constexpr (Std::is_constant<0, Std::remove_cvref_t<stride_t>>::value) {
        return shape == 1 && is_static_zero_stride_allowed_at<pattern_t, path...>::value;
    } else {
        return stride > 0;
    }
}

template <typename layout_t>
__aicore__ inline constexpr bool is_layout_stride_valid(const layout_t& layout)
{
    static_assert(IsLayoutV<layout_t>, "The checked value must be a tensor layout.");
    return is_stride_valid<GetLayoutPattern<layout_t>>(layout.Shape(), layout.Stride(), Std::index_sequence<>{});
}

#ifdef ASCENDC_DEBUG
template <size_t index, size_t tuple_index = 0, typename value_t>
__aicore__ inline constexpr decltype(auto) get_debug_tuple_leaf(const value_t& value)
{
    using value_type = Std::remove_cvref_t<value_t>;
    static_assert(index < NestingDepthV<value_type>, "The debug tuple leaf index must be in range.");
    if constexpr (Std::is_tuple_v<value_type>) {
        using element_type = typename Std::tuple_element<tuple_index, value_type>::type;
        if constexpr (index < NestingDepthV<element_type>) {
            return get_debug_tuple_leaf<index, 0>(Std::get<tuple_index>(value));
        } else {
            return get_debug_tuple_leaf<index - NestingDepthV<element_type>, tuple_index + 1>(value);
        }
    } else {
        static_assert(index == 0, "The debug scalar leaf index must be 0.");
        return value;
    }
}

// A scalar coord addresses one logical dimension represented by the product of its physical shape subtree.
template <size_t index, size_t tuple_index = 0, typename coord_t, typename shape_t>
__aicore__ inline constexpr decltype(auto) get_debug_logical_shape_leaf(const coord_t& coord, const shape_t& shape)
{
    using coord_type = Std::remove_cvref_t<coord_t>;
    static_assert(index < NestingDepthV<coord_type>, "The debug logical shape leaf index must be in range.");
    if constexpr (Std::is_tuple_v<coord_type>) {
        using shape_type = Std::remove_cvref_t<shape_t>;
        static_assert(Std::is_tuple_v<shape_type>, "The coord and shape must describe compatible dimensions.");
        static_assert(
            Std::tuple_size_v<coord_type> == Std::tuple_size_v<shape_type>,
            "The coord and shape must have the same rank.");
        using coord_element_type = typename Std::tuple_element<tuple_index, coord_type>::type;
        if constexpr (index < NestingDepthV<coord_element_type>) {
            return get_debug_logical_shape_leaf<index, 0>(Std::get<tuple_index>(coord), Std::get<tuple_index>(shape));
        } else {
            return get_debug_logical_shape_leaf<index - NestingDepthV<coord_element_type>, tuple_index + 1>(
                coord, shape);
        }
    } else {
        static_assert(index == 0, "The debug logical scalar leaf index must be 0.");
        return Product{}(shape);
    }
}

enum class tensor_layout_error_kind { SHAPE, STRIDE };

#define TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(tuple_format)                                              \
    do {                                                                                                              \
        if constexpr (kind == tensor_layout_error_kind::SHAPE) {                                                      \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                            \
                context,                                                                                              \
                "Failed to check %s tensor shape in %s, current %sShape=(" tuple_format                               \
                "); all %s tensor shape sizes must be greater than 0.",                                               \
                tensor_name, api_name, tensor_name,                                                                   \
                static_cast<long long>(get_debug_tuple_leaf<indices>(layout.Shape()))..., tensor_name);               \
        } else {                                                                                                      \
            static_assert(kind == tensor_layout_error_kind::STRIDE, "Unsupported tensor layout error kind.");         \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                            \
                context,                                                                                              \
                "Failed to check %s tensor stride in %s, current %sShape=(" tuple_format "), %sStride=(" tuple_format \
                "); all %s tensor strides must be greater than 0; a zero "                                            \
                "stride is allowed only when "                                                                        \
                "the tensor format defines it and the corresponding shape size is 1.",                                \
                tensor_name, api_name, tensor_name,                                                                   \
                static_cast<long long>(get_debug_tuple_leaf<indices>(layout.Shape()))..., tensor_name,                \
                static_cast<long long>(get_debug_tuple_leaf<indices>(layout.Stride()))..., tensor_name);              \
        }                                                                                                             \
    } while (0)

template <tensor_layout_error_kind kind, typename layout_t, size_t... indices>
__aicore__ inline void report_tensor_layout_error_impl(
    const tensor_api_assert_context& context, const layout_t& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name, Std::index_sequence<indices...>)
{
    // NPU printf needs a literal format. Built-in tensor layouts contain at most five physical dimensions.
    if constexpr (sizeof...(indices) == 1) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld");
    } else if constexpr (sizeof...(indices) == 2) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld");
    } else if constexpr (sizeof...(indices) == 3) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld");
    } else if constexpr (sizeof...(indices) == 4) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(indices) == 5) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (kind == tensor_layout_error_kind::SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context, "Failed to check %s tensor shape in %s, all %s tensor shape sizes must be greater than 0.",
            tensor_name, api_name, tensor_name);
    } else {
        static_assert(kind == tensor_layout_error_kind::STRIDE, "Unsupported tensor layout error kind.");
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check %s tensor stride in %s, all %s tensor strides must be greater than 0; a zero stride "
            "is allowed only when the tensor format defines it and the corresponding shape size is 1.",
            tensor_name, api_name, tensor_name);
    }
}

#undef TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT

template <tensor_layout_error_kind kind, typename layout_t>
__aicore__ inline void report_tensor_layout_error(
    const tensor_api_assert_context& context, const layout_t& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name)
{
    if constexpr (kind == tensor_layout_error_kind::SHAPE) {
        report_tensor_layout_error_impl<kind>(
            context, layout, tensor_name, api_name,
            Std::make_index_sequence<NestingDepthV<decltype(layout.Shape())>>{});
    } else {
        static_assert(kind == tensor_layout_error_kind::STRIDE, "Unsupported tensor layout error kind.");
        report_tensor_layout_error_impl<kind>(
            context, layout, tensor_name, api_name,
            Std::make_index_sequence<NestingDepthV<decltype(layout.Stride())>>{});
    }
}

enum class slice_error_kind { SOURCE_SHAPE, SOURCE_STRIDE, SLICE_SHAPE, COORD };

#define TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(logical_format, stride_format)                     \
    do {                                                                                                     \
        TENSOR_API_DETAIL_ASSERT_FAIL(                                                                       \
            context,                                                                                         \
            "Failed to check source tensor stride in Slice, sourceShape=(" logical_format                    \
            "), sourceStride=(" stride_format "), coord=(" logical_format "), sliceShape=(" logical_format   \
            "); sourceStride must be valid.",                                                                \
            static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, layout.Shape()))..., \
            static_cast<long long>(get_debug_tuple_leaf<stride_indices>(layout.Stride()))...,                \
            static_cast<long long>(get_debug_tuple_leaf<logical_indices>(coord))...,                         \
            static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, slice_shape))...);   \
    } while (0)

template <
    typename layout_t, typename coord_t, typename slice_shape_t, size_t... logical_indices, size_t... stride_indices>
__aicore__ inline void report_slice_stride_error(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord,
    const slice_shape_t& slice_shape, Std::index_sequence<logical_indices...>, Std::index_sequence<stride_indices...>)
{
    if constexpr (sizeof...(logical_indices) == 1 && sizeof...(stride_indices) == 1) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld", "%lld");
    } else if constexpr (sizeof...(logical_indices) == 2 && sizeof...(stride_indices) == 2) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld", "%lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 2 && sizeof...(stride_indices) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld", "%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 3 && sizeof...(stride_indices) == 3) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld, %lld", "%lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 3 && sizeof...(stride_indices) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld, %lld", "%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 4 && sizeof...(stride_indices) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld, %lld, %lld", "%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 5 && sizeof...(stride_indices) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(
            "%lld, %lld, %lld, %lld, %lld", "%lld, %lld, %lld, %lld, %lld");
    } else {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check source tensor stride in Slice, all source tensor strides must be greater than 0; a zero "
            "stride is allowed only when the tensor format defines it and the corresponding shape size is 1.");
    }
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT

#define TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT(tuple_format)                                                     \
    do {                                                                                                             \
        if constexpr (kind == slice_error_kind::SOURCE_SHAPE) {                                                      \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                           \
                context,                                                                                             \
                "Failed to check source tensor shape in Slice, sourceShape=(" tuple_format "), coord=(" tuple_format \
                "), sliceShape=(" tuple_format "); sourceShape must be "                                             \
                "greater than 0.",                                                                                   \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, layout.Shape()))...,     \
                static_cast<long long>(get_debug_tuple_leaf<logical_indices>(coord))...,                             \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, slice_shape))...);       \
        } else if constexpr (kind == slice_error_kind::SLICE_SHAPE) {                                                \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                           \
                context,                                                                                             \
                "Failed to check slice shape in Slice, sourceShape=(" tuple_format "), coord=(" tuple_format         \
                "), sliceShape=(" tuple_format "); sliceShape must be greater than 0.",                              \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, layout.Shape()))...,     \
                static_cast<long long>(get_debug_tuple_leaf<logical_indices>(coord))...,                             \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, slice_shape))...);       \
        } else {                                                                                                     \
            static_assert(kind == slice_error_kind::COORD, "Unsupported Slice error kind.");                         \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                           \
                context,                                                                                             \
                "Failed to check coord in Slice, sourceShape=(" tuple_format "), coord=(" tuple_format               \
                "), sliceShape=(" tuple_format "); coord must be non-negative and within sourceShape.",              \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, layout.Shape()))...,     \
                static_cast<long long>(get_debug_tuple_leaf<logical_indices>(coord))...,                             \
                static_cast<long long>(get_debug_logical_shape_leaf<logical_indices>(coord, slice_shape))...);       \
        }                                                                                                            \
    } while (0)

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename slice_shape_t, size_t... logical_indices,
    size_t... stride_indices>
__aicore__ inline void report_slice_error_impl(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord,
    const slice_shape_t& slice_shape, Std::index_sequence<logical_indices...> logical_index_sequence,
    Std::index_sequence<stride_indices...> stride_index_sequence)
{
    // NPU printf needs a literal format. Built-in tensor layouts contain at most five logical dimensions.
    if constexpr (kind == slice_error_kind::SOURCE_STRIDE) {
        report_slice_stride_error(context, layout, coord, slice_shape, logical_index_sequence, stride_index_sequence);
    } else if constexpr (sizeof...(logical_indices) == 1) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld");
    } else if constexpr (sizeof...(logical_indices) == 2) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 3) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(logical_indices) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (kind == slice_error_kind::SOURCE_SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check source tensor shape in Slice, all source tensor shape sizes must be greater than 0.");
    } else if constexpr (kind == slice_error_kind::SLICE_SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context, "Failed to check slice shape in Slice, all slice shape sizes must be greater than 0.");
    } else {
        static_assert(kind == slice_error_kind::COORD, "Unsupported Slice error kind.");
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check coord in Slice, coord must be non-negative and within the source tensor shape range.");
    }
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT

template <slice_error_kind kind, typename layout_t, typename coord_t, typename info_t>
__aicore__ inline void report_slice_error(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info)
{
    if constexpr (IsLayoutV<info_t>) {
        report_slice_error_impl<kind>(
            context, layout, coord, info.Shape(), Std::make_index_sequence<NestingDepthV<coord_t>>{},
            Std::make_index_sequence<NestingDepthV<decltype(layout.Stride())>>{});
    } else {
        report_slice_error_impl<kind>(
            context, layout, coord, info, Std::make_index_sequence<NestingDepthV<coord_t>>{},
            Std::make_index_sequence<NestingDepthV<decltype(layout.Stride())>>{});
    }
}
#endif

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
