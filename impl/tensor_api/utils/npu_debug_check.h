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

#ifdef ASCENDC_DEBUG
#include "impl/tensor_api/tensor/layout_pattern.h"

namespace AscendC {
namespace Te {

constexpr auto debug_mmad_dim_min = 0;
constexpr auto debug_mmad_dim_max = 4095;
constexpr auto debug_gemv_m_size = 1;
constexpr auto debug_unit_flag_default = 0;
constexpr auto debug_unit_flag_keep = 2;
constexpr auto debug_unit_flag_reset = 3;
constexpr auto debug_block_count_min = 1;
constexpr auto debug_block_count_max = 4095;
constexpr auto debug_block_len_min = 1;
constexpr auto debug_block_len_max = 65535;
constexpr auto debug_gm_ub_block_len_max = 2097151;
constexpr auto debug_copy_nop_value = 0;
constexpr auto debug_single_block_count = 1;
constexpr auto debug_data_block_bytes = 32;
constexpr auto debug_batch_count_max = 4095;
constexpr auto debug_fixpipe_src_stride_max = 65535;
constexpr auto debug_fixpipe_m_min = 1;
constexpr auto debug_fixpipe_m_nd_max = 8192;
constexpr auto debug_fixpipe_m_max = 65535;
constexpr auto debug_fixpipe_split_n_multiple = 8;
constexpr auto debug_fixpipe_n_multiple = 16;
constexpr auto debug_l0_step_max = 255;
constexpr auto debug_l0_b4_m_step_multiple = 4;
constexpr auto debug_l0_b8_m_step_multiple = 2;
constexpr auto debug_l0_transpose_data_bytes = 4;
constexpr auto debug_l0_transpose_k_step_multiple = 2;

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

struct debug_tuple_unsupported {};
struct debug_tuple_flat_1 {};
struct debug_tuple_flat_2 {};
struct debug_tuple_flat_3 {};
struct debug_tuple_flat_4 {};
struct debug_tuple_flat_5 {};
struct debug_tuple_flat_6 {};
struct debug_tuple_nested_2x2 {};
struct debug_tuple_leading_scalar_flat_2 {};
struct debug_tuple_leading_scalar_nested_2x2 {};

template <size_t size>
struct debug_flat_tuple_format {
    using type = debug_tuple_unsupported;
};

#define TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(size) \
    template <>                                          \
    struct debug_flat_tuple_format<size> {               \
        using type = debug_tuple_flat_##size;            \
    }

TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(1);
TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(2);
TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(3);
TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(4);
TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(5);
TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT(6);

#undef TENSOR_API_DETAIL_DEFINE_FLAT_TUPLE_FORMAT

template <typename... values_t>
constexpr bool are_debug_scalar_values_v = (... && !Std::is_tuple_v<Std::remove_cvref_t<values_t>>);

template <typename value_t>
struct debug_tuple_format {
    using type = debug_tuple_unsupported;
};

template <typename... values_t>
struct debug_tuple_format<Std::tuple<values_t...>> {
    using type = Std::conditional_t<
        are_debug_scalar_values_v<values_t...>, typename debug_flat_tuple_format<sizeof...(values_t)>::type,
        debug_tuple_unsupported>;
};

template <typename value_00_t, typename value_01_t, typename value_10_t, typename value_11_t>
struct debug_tuple_format<Std::tuple<Std::tuple<value_00_t, value_01_t>, Std::tuple<value_10_t, value_11_t>>> {
    using type = Std::conditional_t<
        are_debug_scalar_values_v<value_00_t, value_01_t, value_10_t, value_11_t>, debug_tuple_nested_2x2,
        debug_tuple_unsupported>;
};

template <typename leading_t, typename value_0_t, typename value_1_t>
struct debug_tuple_format<Std::tuple<leading_t, Std::tuple<value_0_t, value_1_t>>> {
    using type = Std::conditional_t<
        are_debug_scalar_values_v<leading_t, value_0_t, value_1_t>, debug_tuple_leading_scalar_flat_2,
        debug_tuple_unsupported>;
};

template <typename leading_t, typename value_00_t, typename value_01_t, typename value_10_t, typename value_11_t>
struct debug_tuple_format<
    Std::tuple<leading_t, Std::tuple<Std::tuple<value_00_t, value_01_t>, Std::tuple<value_10_t, value_11_t>>>> {
    using type = Std::conditional_t<
        are_debug_scalar_values_v<leading_t, value_00_t, value_01_t, value_10_t, value_11_t>,
        debug_tuple_leading_scalar_nested_2x2, debug_tuple_unsupported>;
};

template <typename value_t>
using debug_tuple_format_t = typename debug_tuple_format<Std::remove_cvref_t<value_t>>::type;

template <typename shape_t>
struct is_debug_ext_shape : Std::false_type {};

template <typename value_00_t, typename value_01_t, typename value_10_t, typename value_11_t>
struct is_debug_ext_shape<Std::tuple<Std::tuple<value_00_t, value_01_t>, Std::tuple<value_10_t, value_11_t>>>
    : Std::bool_constant<
          are_debug_scalar_values_v<value_00_t, value_01_t, value_10_t, value_11_t> &&
          Std::is_constant<1, Std::remove_cvref_t<value_00_t>>::value &&
          Std::is_constant<1, Std::remove_cvref_t<value_10_t>>::value> {};

template <typename shape_t>
constexpr bool is_debug_ext_shape_v = is_debug_ext_shape<Std::remove_cvref_t<shape_t>>::value;

template <typename shape_t>
struct is_debug_batch_ext_shape : Std::false_type {};

template <typename batch_t, typename value_00_t, typename value_01_t, typename value_10_t, typename value_11_t>
struct is_debug_batch_ext_shape<
    Std::tuple<batch_t, Std::tuple<Std::tuple<value_00_t, value_01_t>, Std::tuple<value_10_t, value_11_t>>>>
    : Std::bool_constant<
          are_debug_scalar_values_v<batch_t, value_00_t, value_01_t, value_10_t, value_11_t> &&
          Std::is_constant<1, Std::remove_cvref_t<value_00_t>>::value &&
          Std::is_constant<1, Std::remove_cvref_t<value_10_t>>::value> {};

template <typename shape_t>
constexpr bool is_debug_batch_ext_shape_v = is_debug_batch_ext_shape<Std::remove_cvref_t<shape_t>>::value;

template <typename pattern_t>
__aicore__ inline constexpr auto get_debug_layout_pattern_name()
{
    if constexpr (Std::is_same_v<pattern_t, ZNLayoutPtn>) {
        return "ZNLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, ZZLayoutPtn>) {
        return "ZZLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NNLayoutPtn>) {
        return "NNLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NZLayoutPtn>) {
        return "NZLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NDLayoutPtn>) {
        return "NDLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, DNLayoutPtn>) {
        return "DNLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NDExtLayoutPtn>) {
        return "NDExtLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, DNExtLayoutPtn>) {
        return "DNExtLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, ScaleANDLayoutPtn>) {
        return "ScaleANDLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, ScaleADNLayoutPtn>) {
        return "ScaleADNLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, ScaleBNDLayoutPtn>) {
        return "ScaleBNDLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, ScaleBDNLayoutPtn>) {
        return "ScaleBDNLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NCHWLayoutPtn>) {
        return "NCHWLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NHWCLayoutPtn>) {
        return "NHWCLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NC1HWC0LayoutPtn>) {
        return "NC1HWC0LayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NCDHWLayoutPtn>) {
        return "NCDHWLayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, NDC1HWC0LayoutPtn>) {
        return "NDC1HWC0LayoutPtn";
    } else if constexpr (Std::is_same_v<pattern_t, Std::ignore_t>) {
        return "Unspecified";
    } else {
        return "Unknown";
    }
}

template <typename layout_t>
struct debug_layout_view {
    using layout_type = Std::remove_cvref_t<layout_t>;
    using pattern_type = GetLayoutPattern<layout_type>;
    using shape_type = Std::remove_cvref_t<decltype(Std::declval<const layout_type&>().Shape())>;
    using shape_format_type = debug_tuple_format_t<shape_type>;

    static constexpr bool is_flat_matrix_pattern = IsOneOfAttrV<pattern_type, NDLayoutPtn, DNLayoutPtn>;
    static constexpr bool is_ext_matrix_pattern =
        IsOneOfAttrV<pattern_type, NDExtLayoutPtn, DNExtLayoutPtn, ScaleANDLayoutPtn, ScaleBDNLayoutPtn>;
    static constexpr bool is_fractal_matrix_pattern = IsOneOfAttrV<
        pattern_type, NZLayoutPtn, ZNLayoutPtn, ZZLayoutPtn, NNLayoutPtn, ScaleADNLayoutPtn, ScaleBNDLayoutPtn>;
    static constexpr bool is_conv_4d_pattern = IsOneOfAttrV<pattern_type, NCHWLayoutPtn, NHWCLayoutPtn>;
    static constexpr bool is_conv_5d_pattern = IsOneOfAttrV<pattern_type, NC1HWC0LayoutPtn, NCDHWLayoutPtn>;
    static constexpr bool is_conv_6d_pattern = Std::is_same_v<pattern_type, NDC1HWC0LayoutPtn>;

    static constexpr bool has_batch =
        (is_flat_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_flat_2>) ||
        (is_ext_matrix_pattern && is_debug_batch_ext_shape_v<shape_type>) ||
        (is_fractal_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_nested_2x2>) ||
        (is_conv_4d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_4>) ||
        (is_conv_5d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_5>) ||
        (is_conv_6d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_6>);

    __aicore__ inline static constexpr auto pattern_name() { return get_debug_layout_pattern_name<pattern_type>(); }

    __aicore__ inline static constexpr decltype(auto) batch(const layout_type& layout)
    {
        return Get<0>(layout.Shape());
    }

    __aicore__ inline static constexpr auto shape(const layout_type& layout)
    {
        if constexpr (is_flat_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_flat_2>) {
            return Get<1>(layout.Shape());
        } else if constexpr (is_ext_matrix_pattern && is_debug_batch_ext_shape_v<shape_type>) {
            return MakeShape(Get<1, 0, 1>(layout.Shape()), Get<1, 1, 1>(layout.Shape()));
        } else if constexpr (is_ext_matrix_pattern && is_debug_ext_shape_v<shape_type>) {
            return MakeShape(Get<0, 1>(layout.Shape()), Get<1, 1>(layout.Shape()));
        } else if constexpr (
            is_fractal_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_nested_2x2>) {
            return Get<1>(layout.Shape());
        } else if constexpr (is_conv_4d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_4>) {
            return MakeShape(Get<1>(layout.Shape()), Get<2>(layout.Shape()), Get<3>(layout.Shape()));
        } else if constexpr (is_conv_5d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_5>) {
            return MakeShape(
                Get<1>(layout.Shape()), Get<2>(layout.Shape()), Get<3>(layout.Shape()), Get<4>(layout.Shape()));
        } else if constexpr (is_conv_6d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_6>) {
            return MakeShape(
                Get<1>(layout.Shape()), Get<2>(layout.Shape()), Get<3>(layout.Shape()), Get<4>(layout.Shape()),
                Get<5>(layout.Shape()));
        } else {
            return layout.Shape();
        }
    }
};

template <typename info_t, bool is_layout = IsLayoutV<info_t>>
struct debug_slice_info_view {
    static constexpr bool has_layout = false;
    static constexpr bool has_batch = false;

    __aicore__ inline static constexpr decltype(auto) shape(const info_t& info) { return info; }
};

template <typename info_t>
struct debug_slice_info_view<info_t, true> {
    using layout_view_type = debug_layout_view<info_t>;

    static constexpr bool has_layout = true;
    static constexpr bool has_batch = layout_view_type::has_batch;

    __aicore__ inline static constexpr auto pattern_name() { return layout_view_type::pattern_name(); }

    __aicore__ inline static constexpr decltype(auto) batch(const info_t& info)
    {
        return layout_view_type::batch(info);
    }

    __aicore__ inline static constexpr auto shape(const info_t& info) { return layout_view_type::shape(info); }
};

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

#define TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(shape_format, batch_shape_format)                 \
    do {                                                                                                     \
        if constexpr (view_type::has_batch) {                                                                \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                   \
                context,                                                                                     \
                "Failed to check %s tensor shape in %s, current %sLayoutPtn=%s, %sShape=" batch_shape_format \
                "; all %s tensor shape sizes must be greater than 0.",                                       \
                tensor_name, api_name, tensor_name, view_type::pattern_name(), tensor_name,                  \
                static_cast<long long>(view_type::batch(layout)),                                            \
                static_cast<long long>(get_debug_tuple_leaf<indices>(shape))..., tensor_name);               \
        } else {                                                                                             \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                   \
                context,                                                                                     \
                "Failed to check %s tensor shape in %s, current %sLayoutPtn=%s, %sShape=" shape_format       \
                "; all %s tensor shape sizes must be greater than 0.",                                       \
                tensor_name, api_name, tensor_name, view_type::pattern_name(), tensor_name,                  \
                static_cast<long long>(get_debug_tuple_leaf<indices>(shape))..., tensor_name);               \
        }                                                                                                    \
    } while (0)

template <typename layout_t, typename shape_t, size_t... indices>
__aicore__ inline void report_tensor_layout_error_impl(
    const tensor_api_assert_context& context, const layout_t& layout, const shape_t& shape,
    __gm__ const char* tensor_name, __gm__ const char* api_name, Std::index_sequence<indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using format_type = debug_tuple_format_t<shape_t>;
    if constexpr (Std::is_same_v<format_type, debug_tuple_flat_1>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("(%lld)", "(%lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("(%lld, %lld)", "(%lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_flat_3>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(
            "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_flat_5>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_flat_6>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else if constexpr (Std::is_same_v<format_type, debug_tuple_leading_scalar_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    } else if constexpr (view_type::has_batch) {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check %s tensor shape in %s, current %sLayoutPtn=%s, "
            "%sShape=(%lld, <unsupported tuple structure>), %sShapeLeafCount=%u; all %s tensor shape sizes must be "
            "greater than 0.",
            tensor_name, api_name, tensor_name, view_type::pattern_name(), tensor_name,
            static_cast<long long>(view_type::batch(layout)), tensor_name,
            static_cast<unsigned int>(NestingDepthV<shape_t> + 1), tensor_name);
    } else {
        TENSOR_API_DETAIL_ASSERT_FAIL(
            context,
            "Failed to check %s tensor shape in %s, current %sLayoutPtn=%s, "
            "%sShape=<unsupported tuple structure>, %sShapeLeafCount=%u; all %s tensor shape sizes must be greater "
            "than 0.",
            tensor_name, api_name, tensor_name, view_type::pattern_name(), tensor_name, tensor_name,
            static_cast<unsigned int>(NestingDepthV<shape_t>), tensor_name);
    }
}

#undef TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT

template <typename layout_t>
__aicore__ inline void report_tensor_layout_error(
    const tensor_api_assert_context& context, const layout_t& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name)
{
    using view_type = debug_layout_view<layout_t>;
    auto shape = view_type::shape(layout);
    report_tensor_layout_error_impl(
        context, layout, shape, tensor_name, api_name, Std::make_index_sequence<NestingDepthV<decltype(shape)>>{});
}

#define TENSOR_API_DETAIL_COPY_SIZE_FAIL(fields_format, ...)                         \
    TENSOR_API_DETAIL_ASSERT_FAIL(                                                   \
        context,                                                                     \
        "Failed to check copy data size in %s, " fields_format                       \
        "; the amount of data to copy must not exceed the destination tensor size.", \
        api_name, __VA_ARGS__)

#define TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                         \
    src_shape_format, src_batch_shape_format, dst_shape_format, dst_batch_shape_format)                          \
    do {                                                                                                         \
        if constexpr (src_view_type::has_batch && dst_view_type::has_batch) {                                    \
            TENSOR_API_DETAIL_COPY_SIZE_FAIL(                                                                    \
                "srcLayoutPtn=%s, srcShape=" src_batch_shape_format                                              \
                ", copyDataSize=%lld, dstLayoutPtn=%s, dstShape=" dst_batch_shape_format ", dstTensorSize=%lld", \
                src_view_type::pattern_name(), static_cast<long long>(src_view_type::batch(src.Layout())),       \
                static_cast<long long>(get_debug_tuple_leaf<src_indices>(src_shape))...,                         \
                static_cast<long long>(src.Size()), dst_view_type::pattern_name(),                               \
                static_cast<long long>(dst_view_type::batch(dst.Layout())),                                      \
                static_cast<long long>(get_debug_tuple_leaf<dst_indices>(dst_shape))...,                         \
                static_cast<long long>(dst.Size()));                                                             \
        } else if constexpr (src_view_type::has_batch) {                                                         \
            TENSOR_API_DETAIL_COPY_SIZE_FAIL(                                                                    \
                "srcLayoutPtn=%s, srcShape=" src_batch_shape_format                                              \
                ", copyDataSize=%lld, dstLayoutPtn=%s, dstShape=" dst_shape_format ", dstTensorSize=%lld",       \
                src_view_type::pattern_name(), static_cast<long long>(src_view_type::batch(src.Layout())),       \
                static_cast<long long>(get_debug_tuple_leaf<src_indices>(src_shape))...,                         \
                static_cast<long long>(src.Size()), dst_view_type::pattern_name(),                               \
                static_cast<long long>(get_debug_tuple_leaf<dst_indices>(dst_shape))...,                         \
                static_cast<long long>(dst.Size()));                                                             \
        } else if constexpr (dst_view_type::has_batch) {                                                         \
            TENSOR_API_DETAIL_COPY_SIZE_FAIL(                                                                    \
                "srcLayoutPtn=%s, srcShape=" src_shape_format                                                    \
                ", copyDataSize=%lld, dstLayoutPtn=%s, dstShape=" dst_batch_shape_format ", dstTensorSize=%lld", \
                src_view_type::pattern_name(),                                                                   \
                static_cast<long long>(get_debug_tuple_leaf<src_indices>(src_shape))...,                         \
                static_cast<long long>(src.Size()), dst_view_type::pattern_name(),                               \
                static_cast<long long>(dst_view_type::batch(dst.Layout())),                                      \
                static_cast<long long>(get_debug_tuple_leaf<dst_indices>(dst_shape))...,                         \
                static_cast<long long>(dst.Size()));                                                             \
        } else {                                                                                                 \
            TENSOR_API_DETAIL_COPY_SIZE_FAIL(                                                                    \
                "srcLayoutPtn=%s, srcShape=" src_shape_format                                                    \
                ", copyDataSize=%lld, dstLayoutPtn=%s, dstShape=" dst_shape_format ", dstTensorSize=%lld",       \
                src_view_type::pattern_name(),                                                                   \
                static_cast<long long>(get_debug_tuple_leaf<src_indices>(src_shape))...,                         \
                static_cast<long long>(src.Size()), dst_view_type::pattern_name(),                               \
                static_cast<long long>(get_debug_tuple_leaf<dst_indices>(dst_shape))...,                         \
                static_cast<long long>(dst.Size()));                                                             \
        }                                                                                                        \
    } while (0)

#define TENSOR_API_DETAIL_DISPATCH_COPY_DST(src_shape_format, src_batch_shape_format)                            \
    do {                                                                                                         \
        if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_1>) {                                     \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld)", "(%lld, %lld)");                             \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_2>) {                              \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, %lld)", "(%lld, %lld, %lld)");                 \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_3>) {                              \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)");     \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_4>) {                              \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, %lld, %lld, %lld)",                            \
                "(%lld, %lld, %lld, %lld, %lld)");                                                               \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_5>) {                              \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, %lld, %lld, %lld, %lld)",                      \
                "(%lld, %lld, %lld, %lld, %lld, %lld)");                                                         \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_flat_6>) {                              \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, %lld, %lld, %lld, %lld, %lld)",                \
                "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");                                                   \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_nested_2x2>) {                          \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "((%lld, %lld), (%lld, %lld))",                        \
                "(%lld, (%lld, %lld), (%lld, %lld))");                                                           \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_leading_scalar_flat_2>) {               \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))"); \
        } else if constexpr (Std::is_same_v<dst_format_type, debug_tuple_leading_scalar_nested_2x2>) {           \
            TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS(                                                     \
                src_shape_format, src_batch_shape_format, "(%lld, ((%lld, %lld), (%lld, %lld)))",                \
                "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");                                                   \
        } else {                                                                                                 \
            report_copy_size_unsupported(context, src, dst, api_name);                                           \
        }                                                                                                        \
    } while (0)

template <typename src_tensor_t, typename dst_tensor_t>
__aicore__ inline void report_copy_size_unsupported(
    const tensor_api_assert_context& context, const src_tensor_t& src, const dst_tensor_t& dst,
    __gm__ const char* api_name)
{
    using src_view_type = debug_layout_view<decltype(src.Layout())>;
    using dst_view_type = debug_layout_view<decltype(dst.Layout())>;
    if constexpr (src_view_type::has_batch && dst_view_type::has_batch) {
        TENSOR_API_DETAIL_COPY_SIZE_FAIL(
            "srcLayoutPtn=%s, srcShape=(%lld, <unsupported tuple structure>), copyDataSize=%lld, "
            "dstLayoutPtn=%s, dstShape=(%lld, <unsupported tuple structure>), dstTensorSize=%lld",
            src_view_type::pattern_name(), static_cast<long long>(src_view_type::batch(src.Layout())),
            static_cast<long long>(src.Size()), dst_view_type::pattern_name(),
            static_cast<long long>(dst_view_type::batch(dst.Layout())), static_cast<long long>(dst.Size()));
    } else if constexpr (src_view_type::has_batch) {
        TENSOR_API_DETAIL_COPY_SIZE_FAIL(
            "srcLayoutPtn=%s, srcShape=(%lld, <unsupported tuple structure>), copyDataSize=%lld, "
            "dstLayoutPtn=%s, dstShape=<unsupported tuple structure>, dstTensorSize=%lld",
            src_view_type::pattern_name(), static_cast<long long>(src_view_type::batch(src.Layout())),
            static_cast<long long>(src.Size()), dst_view_type::pattern_name(), static_cast<long long>(dst.Size()));
    } else if constexpr (dst_view_type::has_batch) {
        TENSOR_API_DETAIL_COPY_SIZE_FAIL(
            "srcLayoutPtn=%s, srcShape=<unsupported tuple structure>, copyDataSize=%lld, "
            "dstLayoutPtn=%s, dstShape=(%lld, <unsupported tuple structure>), dstTensorSize=%lld",
            src_view_type::pattern_name(), static_cast<long long>(src.Size()), dst_view_type::pattern_name(),
            static_cast<long long>(dst_view_type::batch(dst.Layout())), static_cast<long long>(dst.Size()));
    } else {
        TENSOR_API_DETAIL_COPY_SIZE_FAIL(
            "srcLayoutPtn=%s, srcShape=<unsupported tuple structure>, copyDataSize=%lld, "
            "dstLayoutPtn=%s, dstShape=<unsupported tuple structure>, dstTensorSize=%lld",
            src_view_type::pattern_name(), static_cast<long long>(src.Size()), dst_view_type::pattern_name(),
            static_cast<long long>(dst.Size()));
    }
}

template <
    typename src_tensor_t, typename dst_tensor_t, typename src_shape_t, typename dst_shape_t, size_t... src_indices,
    size_t... dst_indices>
__aicore__ inline void report_copy_size_error_impl(
    const tensor_api_assert_context& context, const src_tensor_t& src, const dst_tensor_t& dst,
    const src_shape_t& src_shape, const dst_shape_t& dst_shape, __gm__ const char* api_name,
    Std::index_sequence<src_indices...>, Std::index_sequence<dst_indices...>)
{
    using src_view_type = debug_layout_view<decltype(src.Layout())>;
    using dst_view_type = debug_layout_view<decltype(dst.Layout())>;
    using src_format_type = debug_tuple_format_t<src_shape_t>;
    using dst_format_type = debug_tuple_format_t<dst_shape_t>;
    if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_1>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld)", "(%lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld, %lld)", "(%lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_3>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_5>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_flat_6>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST(
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST("(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else if constexpr (Std::is_same_v<src_format_type, debug_tuple_leading_scalar_nested_2x2>) {
        TENSOR_API_DETAIL_DISPATCH_COPY_DST(
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    } else {
        report_copy_size_unsupported(context, src, dst, api_name);
    }
}

#undef TENSOR_API_DETAIL_DISPATCH_COPY_DST
#undef TENSOR_API_DETAIL_REPORT_COPY_SIZE_WITH_FORMATS

template <typename src_tensor_t, typename dst_tensor_t>
__aicore__ inline void report_copy_size_error(
    const tensor_api_assert_context& context, const src_tensor_t& src, const dst_tensor_t& dst,
    __gm__ const char* api_name)
{
    using src_view_type = debug_layout_view<decltype(src.Layout())>;
    using dst_view_type = debug_layout_view<decltype(dst.Layout())>;
    auto src_shape = src_view_type::shape(src.Layout());
    auto dst_shape = dst_view_type::shape(dst.Layout());
    report_copy_size_error_impl(
        context, src, dst, src_shape, dst_shape, api_name,
        Std::make_index_sequence<NestingDepthV<decltype(src_shape)>>{},
        Std::make_index_sequence<NestingDepthV<decltype(dst_shape)>>{});
}

#undef TENSOR_API_DETAIL_COPY_SIZE_FAIL

enum class slice_error_kind { SOURCE_SHAPE, SLICE_SHAPE, COORD };

#define TENSOR_API_DETAIL_SLICE_FAIL(fields_format, ...)                                                       \
    do {                                                                                                       \
        if constexpr (kind == slice_error_kind::SOURCE_SHAPE) {                                                \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                     \
                context,                                                                                       \
                "Failed to check source tensor shape in Slice, " fields_format                                 \
                "; sourceShape must be greater than 0.",                                                       \
                __VA_ARGS__);                                                                                  \
        } else if constexpr (kind == slice_error_kind::SLICE_SHAPE) {                                          \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                     \
                context,                                                                                       \
                "Failed to check slice shape in Slice, " fields_format "; sliceShape must be greater than 0.", \
                __VA_ARGS__);                                                                                  \
        } else {                                                                                               \
            static_assert(kind == slice_error_kind::COORD, "Unsupported Slice error kind.");                   \
            TENSOR_API_DETAIL_ASSERT_FAIL(                                                                     \
                context,                                                                                       \
                "Failed to check coord in Slice, " fields_format                                               \
                "; coord must be non-negative and within sourceShape.",                                        \
                __VA_ARGS__);                                                                                  \
        }                                                                                                      \
    } while (0)

#define TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(                                                            \
    source_shape_format, source_batch_shape_format, coord_format, slice_shape_format, slice_batch_shape_format) \
    do {                                                                                                        \
        if constexpr (view_type::has_batch) {                                                                   \
            if constexpr (slice_view_type::has_layout && slice_view_type::has_batch) {                          \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_batch_shape_format ", coord=" coord_format        \
                    ", sliceLayoutPtn=%s, sliceShape=" slice_batch_shape_format,                                \
                    view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),                \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    slice_view_type::pattern_name(), static_cast<long long>(slice_view_type::batch(info)),      \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            } else if constexpr (slice_view_type::has_layout) {                                                 \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_batch_shape_format ", coord=" coord_format        \
                    ", sliceLayoutPtn=%s, sliceShape=" slice_shape_format,                                      \
                    view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),                \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    slice_view_type::pattern_name(),                                                            \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            } else {                                                                                            \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_batch_shape_format ", coord=" coord_format        \
                    ", sliceShape=" slice_shape_format,                                                         \
                    view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),                \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            }                                                                                                   \
        } else {                                                                                                \
            if constexpr (slice_view_type::has_layout && slice_view_type::has_batch) {                          \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_shape_format ", coord=" coord_format              \
                    ", sliceLayoutPtn=%s, sliceShape=" slice_batch_shape_format,                                \
                    view_type::pattern_name(),                                                                  \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    slice_view_type::pattern_name(), static_cast<long long>(slice_view_type::batch(info)),      \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            } else if constexpr (slice_view_type::has_layout) {                                                 \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_shape_format ", coord=" coord_format              \
                    ", sliceLayoutPtn=%s, sliceShape=" slice_shape_format,                                      \
                    view_type::pattern_name(),                                                                  \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    slice_view_type::pattern_name(),                                                            \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            } else {                                                                                            \
                TENSOR_API_DETAIL_SLICE_FAIL(                                                                   \
                    "sourceLayoutPtn=%s, sourceShape=" source_shape_format ", coord=" coord_format              \
                    ", sliceShape=" slice_shape_format,                                                         \
                    view_type::pattern_name(),                                                                  \
                    static_cast<long long>(get_debug_tuple_leaf<source_indices>(source_shape))...,              \
                    static_cast<long long>(get_debug_tuple_leaf<coord_indices>(coord))...,                      \
                    static_cast<long long>(get_debug_tuple_leaf<slice_indices>(slice_shape))...);               \
            }                                                                                                   \
        }                                                                                                       \
    } while (0)

#define TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(tuple_format, batch_tuple_format) \
    TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(                                     \
        tuple_format, batch_tuple_format, tuple_format, tuple_format, batch_tuple_format)

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_impl(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info,
    const source_shape_t& source_shape, const slice_shape_t& slice_shape, Std::index_sequence<source_indices...>,
    Std::index_sequence<coord_indices...>, Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    using source_format_type = debug_tuple_format_t<source_shape_t>;
    using coord_format_type = debug_tuple_format_t<coord_t>;
    using slice_format_type = debug_tuple_format_t<slice_shape_t>;

    if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_1> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_1> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_1>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld)", "(%lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld)", "(%lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_3> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_3> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_3>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_4> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_4> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_5> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_5> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_5>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_6> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_6> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_6>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<coord_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<slice_format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_leading_scalar_flat_2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_flat_2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_leading_scalar_nested_2x2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_nested_2x2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))", "(%lld, %lld)", "(%lld, %lld)",
            "(%lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<coord_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<slice_format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "((%lld, %lld), (%lld, %lld))", "((%lld, %lld), (%lld, %lld))",
            "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_flat_2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "(%lld, (%lld, %lld))", "(%lld, (%lld, %lld))",
            "(%lld, %lld, (%lld, %lld))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_flat_2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld))",
            "(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_nested_2x2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_nested_2x2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))",
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, ((%lld, %lld), (%lld, %lld)))",
            "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_2> &&
        Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_nested_2x2> &&
        Std::is_same_v<slice_format_type, debug_tuple_leading_scalar_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "(%lld, ((%lld, %lld), (%lld, %lld)))",
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_3> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_4> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_4> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_5> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_5>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (
        Std::is_same_v<source_format_type, debug_tuple_flat_5> &&
        Std::is_same_v<coord_format_type, debug_tuple_flat_6> &&
        Std::is_same_v<slice_format_type, debug_tuple_flat_6>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (view_type::has_batch && slice_view_type::has_layout && slice_view_type::has_batch) {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=(%lld, <unsupported tuple combination>), "
            "sourceShapeLeafCount=%u, coordLeafCount=%u, sliceLayoutPtn=%s, "
            "sliceShape=(%lld, <unsupported tuple combination>), sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),
            static_cast<unsigned int>(NestingDepthV<source_shape_t> + 1),
            static_cast<unsigned int>(NestingDepthV<coord_t>), slice_view_type::pattern_name(),
            static_cast<long long>(slice_view_type::batch(info)),
            static_cast<unsigned int>(NestingDepthV<slice_shape_t> + 1));
    } else if constexpr (view_type::has_batch && slice_view_type::has_layout) {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=(%lld, <unsupported tuple combination>), "
            "sourceShapeLeafCount=%u, coordLeafCount=%u, sliceLayoutPtn=%s, sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),
            static_cast<unsigned int>(NestingDepthV<source_shape_t> + 1),
            static_cast<unsigned int>(NestingDepthV<coord_t>), slice_view_type::pattern_name(),
            static_cast<unsigned int>(NestingDepthV<slice_shape_t>));
    } else if constexpr (view_type::has_batch) {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=(%lld, <unsupported tuple combination>), "
            "sourceShapeLeafCount=%u, coordLeafCount=%u, sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<long long>(view_type::batch(layout)),
            static_cast<unsigned int>(NestingDepthV<source_shape_t> + 1),
            static_cast<unsigned int>(NestingDepthV<coord_t>), static_cast<unsigned int>(NestingDepthV<slice_shape_t>));
    } else if constexpr (slice_view_type::has_layout && slice_view_type::has_batch) {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=<unsupported tuple combination>, sourceShapeLeafCount=%u, "
            "coordLeafCount=%u, sliceLayoutPtn=%s, sliceShape=(%lld, <unsupported tuple combination>), "
            "sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<unsigned int>(NestingDepthV<source_shape_t>),
            static_cast<unsigned int>(NestingDepthV<coord_t>), slice_view_type::pattern_name(),
            static_cast<long long>(slice_view_type::batch(info)),
            static_cast<unsigned int>(NestingDepthV<slice_shape_t> + 1));
    } else if constexpr (slice_view_type::has_layout) {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=<unsupported tuple combination>, sourceShapeLeafCount=%u, "
            "coordLeafCount=%u, sliceLayoutPtn=%s, sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<unsigned int>(NestingDepthV<source_shape_t>),
            static_cast<unsigned int>(NestingDepthV<coord_t>), slice_view_type::pattern_name(),
            static_cast<unsigned int>(NestingDepthV<slice_shape_t>));
    } else {
        TENSOR_API_DETAIL_SLICE_FAIL(
            "sourceLayoutPtn=%s, sourceShape=<unsupported tuple combination>, sourceShapeLeafCount=%u, "
            "coordLeafCount=%u, sliceShapeLeafCount=%u",
            view_type::pattern_name(), static_cast<unsigned int>(NestingDepthV<source_shape_t>),
            static_cast<unsigned int>(NestingDepthV<coord_t>), static_cast<unsigned int>(NestingDepthV<slice_shape_t>));
    }
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT
#undef TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS
#undef TENSOR_API_DETAIL_SLICE_FAIL

template <slice_error_kind kind, typename layout_t, typename coord_t, typename info_t>
__aicore__ inline void report_slice_error(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    auto source_shape = view_type::shape(layout);
    auto slice_shape = slice_view_type::shape(info);
    report_slice_error_impl<kind>(
        context, layout, coord, info, source_shape, slice_shape,
        Std::make_index_sequence<NestingDepthV<decltype(source_shape)>>{},
        Std::make_index_sequence<NestingDepthV<coord_t>>{},
        Std::make_index_sequence<NestingDepthV<decltype(slice_shape)>>{});
}

template <typename info_t>
__aicore__ inline constexpr bool is_slice_shape_valid(const info_t& info)
{
    if constexpr (IsLayoutV<info_t>) {
        return is_shape_valid(info.Shape());
    } else {
        return is_shape_valid(info);
    }
}

template <typename layout_t>
__aicore__ inline constexpr void debug_check_layout(
    const tensor_api_assert_context& context, const layout_t& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, tensor_name, api_name));
}

template <typename layout_t>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const layout_t& layout)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, "result", "MakeTensor"));
}

template <typename shape_t, typename stride_t>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const shape_t& shape, const stride_t& stride)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(shape)),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, MakeLayout(shape, stride), "result", "MakeTensor"));
}

template <typename src_tensor_t, typename dst_tensor_t>
__aicore__ inline constexpr void debug_check_copy_size(
    const tensor_api_assert_context& context, const src_tensor_t& src, const dst_tensor_t& dst,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (dst.Size() >= src.Size()), TENSOR_API_REPORT_INTERNAL(report_copy_size_error, src, dst, api_name));
}

template <typename layout_t, typename coord_t, typename info_t>
__aicore__ inline constexpr void debug_check_slice_args(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_slice_error<slice_error_kind::SOURCE_SHAPE>, layout, coord, info));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_slice_shape_valid(info)),
        TENSOR_API_REPORT_INTERNAL(report_slice_error<slice_error_kind::SLICE_SHAPE>, layout, coord, info));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_coord_in_shape(coord, layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_slice_error<slice_error_kind::COORD>, layout, coord, info));
}

template <typename params_t>
__aicore__ inline constexpr void debug_check_mmad_params(
    const tensor_api_assert_context& context, const params_t& params, bool disable_gemv, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.m <= debug_mmad_dim_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check m in %s, m must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(debug_mmad_dim_min), static_cast<unsigned int>(debug_mmad_dim_max),
            static_cast<unsigned int>(params.m)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.n <= debug_mmad_dim_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check n in %s, n must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(debug_mmad_dim_min), static_cast<unsigned int>(debug_mmad_dim_max),
            static_cast<unsigned int>(params.n)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.k <= debug_mmad_dim_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check k in %s, k must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(debug_mmad_dim_min), static_cast<unsigned int>(debug_mmad_dim_max),
            static_cast<unsigned int>(params.k)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (disable_gemv || params.m == debug_gemv_m_size),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check m in %s, m must be %u when GEMV is enabled; current value is %u.", api_name,
            static_cast<unsigned int>(debug_gemv_m_size), static_cast<unsigned int>(params.m)));
}

template <typename value_t>
__aicore__ inline constexpr void debug_check_unit_flag(
    const tensor_api_assert_context& context, const value_t& unit_flag, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (unit_flag == debug_unit_flag_default || unit_flag == debug_unit_flag_keep ||
         unit_flag == debug_unit_flag_reset),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check unitFlag in %s, unitFlag must be %u, %u, or %u; current value is %u.", api_name,
            static_cast<unsigned int>(debug_unit_flag_default), static_cast<unsigned int>(debug_unit_flag_keep),
            static_cast<unsigned int>(debug_unit_flag_reset), static_cast<unsigned int>(unit_flag)));
}

template <typename count_t>
__aicore__ inline constexpr void debug_check_block_count(
    const tensor_api_assert_context& context, const count_t& block_count, __gm__ const char* value_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_count >= debug_block_count_min && block_count <= debug_block_count_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be in range [%u, %u]; current value is %lld.", value_name, api_name,
            value_name, static_cast<unsigned int>(debug_block_count_min),
            static_cast<unsigned int>(debug_block_count_max), static_cast<long long>(block_count)));
}

template <typename length_t, typename max_length_t>
__aicore__ inline constexpr void debug_check_block_len(
    const tensor_api_assert_context& context, const length_t& block_len, const max_length_t& max_block_len,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_len >= debug_block_len_min && block_len <= max_block_len),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockLen in %s, blockLen must be in range [%u, %llu]; current value is %llu.", api_name,
            static_cast<unsigned int>(debug_block_len_min), static_cast<unsigned long long>(max_block_len),
            static_cast<unsigned long long>(block_len)));
}

template <typename value_t, typename max_value_t>
__aicore__ inline constexpr void debug_check_block_limit(
    const tensor_api_assert_context& context, const value_t& value, const max_value_t& max_value,
    __gm__ const char* value_name, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (value <= max_value),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be in range [%u, %llu]; current value is %llu.", value_name, api_name,
            value_name, static_cast<unsigned int>(debug_copy_nop_value), static_cast<unsigned long long>(max_value),
            static_cast<unsigned long long>(value)));
}

template <typename stride_t, typename length_t, typename count_t>
__aicore__ inline constexpr void debug_check_gm2ub_stride(
    const tensor_api_assert_context& context, const stride_t& dst_stride, const length_t& block_len,
    const count_t& block_count, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (block_count == debug_copy_nop_value || block_len == debug_copy_nop_value ||
         (block_count == debug_single_block_count && dst_stride == debug_copy_nop_value) ||
         dst_stride == static_cast<stride_t>(block_len) ||
         (dst_stride > debug_copy_nop_value && dst_stride % debug_data_block_bytes == 0)),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check dstStride in %s, for an active copy dstStride must equal blockLen, be a positive "
            "multiple of %u bytes, or be %u when blockCount is %u; current dstStride is %lld, blockLen is %llu, "
            "blockCount is %llu.",
            api_name, static_cast<unsigned int>(debug_data_block_bytes),
            static_cast<unsigned int>(debug_copy_nop_value), static_cast<unsigned int>(debug_single_block_count),
            static_cast<long long>(dst_stride), static_cast<unsigned long long>(block_len),
            static_cast<unsigned long long>(block_count)));
}

template <typename stride_t, typename length_t, typename count_t>
__aicore__ inline constexpr void debug_check_ub2gm_stride(
    const tensor_api_assert_context& context, const stride_t& src_stride, const length_t& block_len,
    const count_t& block_count, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (block_count == debug_copy_nop_value || block_len == debug_copy_nop_value ||
         (src_stride > debug_copy_nop_value &&
          (src_stride == static_cast<stride_t>(block_len) || src_stride % debug_data_block_bytes == 0))),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check srcStride in %s, for an active copy srcStride must equal blockLen or be a positive "
            "multiple of %u bytes; current srcStride is %lld, blockLen is %llu, blockCount is %llu.",
            api_name, static_cast<unsigned int>(debug_data_block_bytes), static_cast<long long>(src_stride),
            static_cast<unsigned long long>(block_len), static_cast<unsigned long long>(block_count)));
}

template <typename batch_t>
__aicore__ inline constexpr void debug_check_batch_count(
    const tensor_api_assert_context& context, const batch_t& batch, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (batch <= debug_batch_count_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src batch shape size in %s, src batch shape size must be at most %u; current value is "
            "%lld.",
            api_name, static_cast<unsigned int>(debug_batch_count_max), static_cast<long long>(batch)));
}

template <typename src_batch_t, typename dst_batch_t>
__aicore__ inline constexpr void debug_check_batch_match(
    const tensor_api_assert_context& context, const src_batch_t& src_batch, const dst_batch_t& dst_batch,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_batch == dst_batch),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src and dst batch shape sizes in %s, src batch shape size must equal dst batch shape "
            "size; current srcBatchShape is %lld, dstBatchShape is %lld.",
            api_name, static_cast<long long>(src_batch), static_cast<long long>(dst_batch)));
}

template <typename src_column_bytes_t, typename dst_column_bytes_t>
__aicore__ inline constexpr void debug_check_l12bt_column_bytes(
    const tensor_api_assert_context& context, const src_column_bytes_t& src_column_bytes,
    const dst_column_bytes_t& dst_column_bytes, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_column_bytes % debug_data_block_bytes == 0 && dst_column_bytes % debug_data_block_bytes == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src and dst column byte sizes in %s, src and dst column byte sizes must be %u-byte "
            "aligned; current srcColumnBytes is %llu, dstColumnBytes is %llu.",
            api_name, static_cast<unsigned int>(debug_data_block_bytes),
            static_cast<unsigned long long>(src_column_bytes), static_cast<unsigned long long>(dst_column_bytes)));
}

template <typename src_stride_t, typename dst_stride_t>
__aicore__ inline constexpr void debug_check_fixpipe_stride(
    const tensor_api_assert_context& context, const src_stride_t& src_stride, const dst_stride_t& dst_stride,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (dst_stride > debug_copy_nop_value),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check dstStride in %s, dstStride must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(debug_copy_nop_value), static_cast<unsigned int>(dst_stride)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_stride <= debug_fixpipe_src_stride_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check srcStride in %s, srcStride must be at most %u; current value is %u.", api_name,
            static_cast<unsigned int>(debug_fixpipe_src_stride_max), static_cast<unsigned int>(src_stride)));
}

template <typename size_type>
__aicore__ inline constexpr void debug_check_fixpipe_m(
    const tensor_api_assert_context& context, const size_type& m_size, bool nz2nd_en, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (m_size >= debug_fixpipe_m_min &&
         ((!nz2nd_en && m_size <= debug_fixpipe_m_max) || (nz2nd_en && m_size <= debug_fixpipe_m_nd_max))),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check mSize in %s, mSize must be in range [%u, %u] when the dst format is ND and [%u, %u] "
            "otherwise; current mSize is %u, nz2ndEn is %u.",
            api_name, static_cast<unsigned int>(debug_fixpipe_m_min), static_cast<unsigned int>(debug_fixpipe_m_nd_max),
            static_cast<unsigned int>(debug_fixpipe_m_min), static_cast<unsigned int>(debug_fixpipe_m_max),
            static_cast<unsigned int>(m_size), static_cast<unsigned int>(nz2nd_en)));
}

template <typename size_type>
__aicore__ inline constexpr void debug_check_fixpipe_n(
    const tensor_api_assert_context& context, const size_type& n_size, bool enable_channel_split, bool nz2nd_en,
    bool nz2dn_en, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (nz2nd_en || nz2dn_en ||
         (enable_channel_split ? n_size % debug_fixpipe_split_n_multiple == 0 :
                                 n_size % debug_fixpipe_n_multiple == 0)),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check nSize in %s NZ instruction, nSize must be divisible by %u when enableChannelSplit is "
            "enabled or by %u otherwise; current nSize is %u, enableChannelSplit is %u, nz2ndEn is %u, nz2dnEn is "
            "%u.",
            api_name, static_cast<unsigned int>(debug_fixpipe_split_n_multiple),
            static_cast<unsigned int>(debug_fixpipe_n_multiple), static_cast<unsigned int>(n_size),
            static_cast<unsigned int>(enable_channel_split), static_cast<unsigned int>(nz2nd_en),
            static_cast<unsigned int>(nz2dn_en)));
}

template <typename count_t, typename length_t>
__aicore__ inline constexpr void debug_check_copy_blocks(
    const tensor_api_assert_context& context, const count_t& block_count, const length_t& block_len,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_count > debug_copy_nop_value),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockCount in %s, blockCount must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(debug_copy_nop_value), static_cast<unsigned int>(block_count)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_len > debug_copy_nop_value),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockLen in %s, blockLen must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(debug_copy_nop_value), static_cast<unsigned int>(block_len)));
}

template <typename data_size_t, typename step_t>
__aicore__ inline constexpr void debug_check_l0_transpose(
    const tensor_api_assert_context& context, bool transpose, const data_size_t& data_size, const step_t& k_step,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (!transpose || data_size != debug_l0_transpose_data_bytes || k_step % debug_l0_transpose_k_step_multiple == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check kStep in %s transpose instruction, kStep must be divisible by %u for %u-byte data; "
            "current value is %u.",
            api_name, static_cast<unsigned int>(debug_l0_transpose_k_step_multiple),
            static_cast<unsigned int>(debug_l0_transpose_data_bytes), static_cast<unsigned int>(k_step)));
}

template <typename step_t>
__aicore__ inline constexpr void debug_check_l0_step(
    const tensor_api_assert_context& context, const step_t& step, __gm__ const char* step_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (step <= debug_l0_step_max),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be at most %u; current value is %lld.", step_name, api_name, step_name,
            static_cast<unsigned int>(debug_l0_step_max), static_cast<long long>(step)));
}

template <typename step_t>
__aicore__ inline constexpr void debug_check_l0_m_step(
    const tensor_api_assert_context& context, const step_t& m_step, bool is_b4, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (m_step % (is_b4 ? debug_l0_b4_m_step_multiple : debug_l0_b8_m_step_multiple) == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check mStep in %s, mStep must be divisible by %u for b4 or by %u for b8; current value is "
            "%lld.",
            api_name, static_cast<unsigned int>(debug_l0_b4_m_step_multiple),
            static_cast<unsigned int>(debug_l0_b8_m_step_multiple), static_cast<long long>(m_step)));
}

template <typename src_stride_t, typename src_capacity_t, typename dst_stride_t, typename dst_capacity_t>
__aicore__ inline constexpr void debug_check_l0_batch_stride(
    const tensor_api_assert_context& context, const src_stride_t& src_stride, const src_capacity_t& src_capacity,
    const dst_stride_t& dst_stride, const dst_capacity_t& dst_capacity, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_stride == src_capacity && dst_stride == dst_capacity),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src and dst batch strides in %s, each batch stride must equal its matrix capacity; "
            "current srcBatchStride is %lld, srcMatrixCapacity is %lld, dstBatchStride is %lld, dstMatrixCapacity is "
            "%lld.",
            api_name, static_cast<long long>(src_stride), static_cast<long long>(src_capacity),
            static_cast<long long>(dst_stride), static_cast<long long>(dst_capacity)));
}

} // namespace Te
} // namespace AscendC
#endif // ASCENDC_DEBUG

#endif // IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
