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
    "impl/tensor_api/utils/npu_debug_utils.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define TENSOR_API_DEBUG_UTILS_OWNS_INTERNAL_HEADER_ACCESS
#endif

/*!
 * \file npu_debug_utils.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_UTILS_NPU_DEBUG_UTILS_H
#define IMPL_TENSOR_API_UTILS_NPU_DEBUG_UTILS_H

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

template <typename stride_t>
__aicore__ inline constexpr bool is_stride_valid(const stride_t& stride);

template <typename stride_t, size_t... indices>
__aicore__ inline constexpr bool is_stride_valid_impl(const stride_t& stride, Std::index_sequence<indices...>)
{
    return (is_stride_valid(Std::get<indices>(stride)) && ...);
}

template <typename stride_t>
__aicore__ inline constexpr bool is_stride_valid(const stride_t& stride)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<stride_t>>) {
        return is_stride_valid_impl(
            stride, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<stride_t>>>{});
    } else {
        return stride >= 0;
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

template <typename info_t>
__aicore__ inline constexpr bool is_slice_shape_valid(const info_t& info)
{
    if constexpr (IsLayoutV<info_t>) {
        return is_shape_valid(info.Shape());
    } else {
        return is_shape_valid(info);
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

struct debug_slice_same_format_tag {};
struct debug_slice_flat_source_tag {};
struct debug_slice_nested_source_tag {};
struct debug_slice_rank_expanded_tag {};
struct debug_slice_unsupported_format_tag {};

enum class slice_error_kind { SOURCE_SHAPE, SLICE_SHAPE, COORD };
enum class tensor_layout_error_kind { SHAPE, STRIDE };

template <typename format_t>
constexpr bool is_debug_slice_same_format_v = IsOneOfAttrV<
    format_t, debug_tuple_flat_1, debug_tuple_flat_2, debug_tuple_flat_3, debug_tuple_flat_4, debug_tuple_flat_5,
    debug_tuple_flat_6, debug_tuple_nested_2x2, debug_tuple_leading_scalar_flat_2,
    debug_tuple_leading_scalar_nested_2x2>;

template <typename source_format_t, typename coord_format_t, typename slice_format_t>
struct debug_slice_format_category {
    static constexpr bool is_same_format = Std::is_same_v<source_format_t, coord_format_t> &&
                                           Std::is_same_v<coord_format_t, slice_format_t> &&
                                           is_debug_slice_same_format_v<source_format_t>;
    static constexpr bool is_flat_source =
        Std::is_same_v<source_format_t, debug_tuple_flat_2> && Std::is_same_v<coord_format_t, slice_format_t> &&
        IsOneOfAttrV<
            coord_format_t, debug_tuple_nested_2x2, debug_tuple_leading_scalar_flat_2,
            debug_tuple_leading_scalar_nested_2x2>;
    static constexpr bool is_nested_source = Std::is_same_v<source_format_t, debug_tuple_nested_2x2> &&
                                             Std::is_same_v<coord_format_t, slice_format_t> &&
                                             IsOneOfAttrV<
                                                 coord_format_t, debug_tuple_flat_2, debug_tuple_leading_scalar_flat_2,
                                                 debug_tuple_leading_scalar_nested_2x2>;
    static constexpr bool is_rank_expanded =
        (Std::is_same_v<source_format_t, debug_tuple_flat_3> && Std::is_same_v<coord_format_t, debug_tuple_flat_4> &&
         Std::is_same_v<slice_format_t, debug_tuple_flat_4>) ||
        (Std::is_same_v<source_format_t, debug_tuple_flat_4> && Std::is_same_v<coord_format_t, debug_tuple_flat_5> &&
         Std::is_same_v<slice_format_t, debug_tuple_flat_5>) ||
        (Std::is_same_v<source_format_t, debug_tuple_flat_5> && Std::is_same_v<coord_format_t, debug_tuple_flat_6> &&
         Std::is_same_v<slice_format_t, debug_tuple_flat_6>);

    using type = Std::conditional_t<
        is_same_format, debug_slice_same_format_tag,
        Std::conditional_t<
            is_flat_source, debug_slice_flat_source_tag,
            Std::conditional_t<
                is_nested_source, debug_slice_nested_source_tag,
                Std::conditional_t<
                    is_rank_expanded, debug_slice_rank_expanded_tag, debug_slice_unsupported_format_tag>>>>;
};

template <typename source_format_t, typename coord_format_t, typename slice_format_t>
using debug_slice_format_category_t =
    typename debug_slice_format_category<source_format_t, coord_format_t, slice_format_t>::type;

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

    __aicore__ inline static constexpr decltype(auto) batch_stride(const layout_type& layout)
    {
        return Get<0>(layout.Stride());
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

    __aicore__ inline static constexpr auto stride(const layout_type& layout)
    {
        if constexpr (is_flat_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_flat_2>) {
            return Get<1>(layout.Stride());
        } else if constexpr (is_ext_matrix_pattern && is_debug_batch_ext_shape_v<shape_type>) {
            return MakeStride(Get<1, 0, 1>(layout.Stride()), Get<1, 1, 1>(layout.Stride()));
        } else if constexpr (is_ext_matrix_pattern && is_debug_ext_shape_v<shape_type>) {
            return MakeStride(Get<0, 1>(layout.Stride()), Get<1, 1>(layout.Stride()));
        } else if constexpr (
            is_fractal_matrix_pattern && Std::is_same_v<shape_format_type, debug_tuple_leading_scalar_nested_2x2>) {
            return Get<1>(layout.Stride());
        } else if constexpr (is_conv_4d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_4>) {
            return MakeStride(Get<1>(layout.Stride()), Get<2>(layout.Stride()), Get<3>(layout.Stride()));
        } else if constexpr (is_conv_5d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_5>) {
            return MakeStride(
                Get<1>(layout.Stride()), Get<2>(layout.Stride()), Get<3>(layout.Stride()), Get<4>(layout.Stride()));
        } else if constexpr (is_conv_6d_pattern && Std::is_same_v<shape_format_type, debug_tuple_flat_6>) {
            return MakeStride(
                Get<1>(layout.Stride()), Get<2>(layout.Stride()), Get<3>(layout.Stride()), Get<4>(layout.Stride()),
                Get<5>(layout.Stride()));
        } else {
            return layout.Stride();
        }
    }
};

template <tensor_layout_error_kind kind>
struct tensor_layout_error_traits;

template <>
struct tensor_layout_error_traits<tensor_layout_error_kind::SHAPE> {
    __aicore__ inline static constexpr auto value_name() { return "shape"; }
    __aicore__ inline static constexpr auto field_name() { return "Shape"; }
    __aicore__ inline static constexpr auto value_description() { return "shape sizes"; }
    __aicore__ inline static constexpr auto requirement() { return "greater than 0"; }

    template <typename layout_t>
    __aicore__ inline static constexpr decltype(auto) batch(const layout_t& layout)
    {
        return debug_layout_view<layout_t>::batch(layout);
    }

    template <typename layout_t>
    __aicore__ inline static constexpr auto value(const layout_t& layout)
    {
        return debug_layout_view<layout_t>::shape(layout);
    }
};

template <>
struct tensor_layout_error_traits<tensor_layout_error_kind::STRIDE> {
    __aicore__ inline static constexpr auto value_name() { return "stride"; }
    __aicore__ inline static constexpr auto field_name() { return "Stride"; }
    __aicore__ inline static constexpr auto value_description() { return "stride values"; }
    __aicore__ inline static constexpr auto requirement() { return "non-negative"; }

    template <typename layout_t>
    __aicore__ inline static constexpr decltype(auto) batch(const layout_t& layout)
    {
        return debug_layout_view<layout_t>::batch_stride(layout);
    }

    template <typename layout_t>
    __aicore__ inline static constexpr auto value(const layout_t& layout)
    {
        return debug_layout_view<layout_t>::stride(layout);
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

} // namespace Te
} // namespace AscendC
#endif // ASCENDC_DEBUG

#endif // IMPL_TENSOR_API_UTILS_NPU_DEBUG_UTILS_H

#if defined(TENSOR_API_DEBUG_UTILS_OWNS_INTERNAL_HEADER_ACCESS)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef TENSOR_API_DEBUG_UTILS_OWNS_INTERNAL_HEADER_ACCESS
#endif
