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
#define TENSOR_API_DEBUG_CHECK_OWNS_INTERNAL_HEADER_ACCESS
#endif

/*!
 * \file npu_debug_check.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H
#define IMPL_TENSOR_API_UTILS_NPU_DEBUG_CHECK_H

#include "impl/tensor_api/utils/npu_debug_assert.h"

#ifdef ASCENDC_DEBUG
namespace AscendC {
namespace Te {

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_same_format_tag, const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord,
    const info_t& info, const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    using source_format_type = debug_tuple_format_t<source_shape_t>;
    if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_1>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld)", "(%lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld)", "(%lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_3>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_5>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_6>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT("(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else {
        TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT(
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    }
}

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_flat_source_tag, const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord,
    const info_t& info, const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    using coord_format_type = debug_tuple_format_t<coord_t>;
    if constexpr (Std::is_same_v<coord_format_type, debug_tuple_nested_2x2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "((%lld, %lld), (%lld, %lld))", "((%lld, %lld), (%lld, %lld))",
            "(%lld, (%lld, %lld), (%lld, %lld))");
    } else if constexpr (Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "(%lld, (%lld, %lld))", "(%lld, (%lld, %lld))",
            "(%lld, %lld, (%lld, %lld))");
    } else {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld)", "(%lld, %lld, %lld)", "(%lld, ((%lld, %lld), (%lld, %lld)))",
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    }
}

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_nested_source_tag, const tensor_api_assert_context& context, const layout_t& layout,
    const coord_t& coord, const info_t& info, const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    using coord_format_type = debug_tuple_format_t<coord_t>;
    if constexpr (Std::is_same_v<coord_format_type, debug_tuple_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))", "(%lld, %lld)", "(%lld, %lld)",
            "(%lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<coord_format_type, debug_tuple_leading_scalar_flat_2>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld))",
            "(%lld, (%lld, %lld))", "(%lld, %lld, (%lld, %lld))");
    } else {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "((%lld, %lld), (%lld, %lld))", "(%lld, (%lld, %lld), (%lld, %lld))",
            "(%lld, ((%lld, %lld), (%lld, %lld)))", "(%lld, ((%lld, %lld), (%lld, %lld)))",
            "(%lld, %lld, ((%lld, %lld), (%lld, %lld)))");
    }
}

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_rank_expanded_tag, const tensor_api_assert_context& context, const layout_t& layout,
    const coord_t& coord, const info_t& info, const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<layout_t>;
    using slice_view_type = debug_slice_info_view<info_t>;
    using source_format_type = debug_tuple_format_t<source_shape_t>;
    if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_3>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld)");
    } else if constexpr (Std::is_same_v<source_format_type, debug_tuple_flat_4>) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)");
    } else {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS(
            "(%lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld, %lld)", "(%lld, %lld, %lld, %lld, %lld, %lld)",
            "(%lld, %lld, %lld, %lld, %lld, %lld, %lld)");
    }
}

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_unsupported_format_tag, const tensor_api_assert_context& context, const layout_t& layout,
    const coord_t& coord, const info_t& info, const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    report_slice_unsupported<kind>(context, layout, coord, info, source_shape, slice_shape);
}

template <
    slice_error_kind kind, typename layout_t, typename coord_t, typename info_t, typename source_shape_t,
    typename slice_shape_t, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_impl(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info,
    const source_shape_t& source_shape, const slice_shape_t& slice_shape,
    Std::index_sequence<source_indices...> source_seq, Std::index_sequence<coord_indices...> coord_seq,
    Std::index_sequence<slice_indices...> slice_seq)
{
    using category_type = debug_slice_format_category_t<
        debug_tuple_format_t<source_shape_t>, debug_tuple_format_t<coord_t>, debug_tuple_format_t<slice_shape_t>>;
    report_slice_error_by_format<kind>(
        category_type{}, context, layout, coord, info, source_shape, slice_shape, source_seq, coord_seq, slice_seq);
}

template <slice_error_kind kind, typename layout_t, typename coord_t, typename info_t>
__aicore__ inline void report_slice_error(
    const tensor_api_assert_context& context, const layout_t& layout, const coord_t& coord, const info_t& info)
{
    auto source_shape = debug_layout_view<layout_t>::shape(layout);
    auto slice_shape = debug_slice_info_view<info_t>::shape(info);
    report_slice_error_impl<kind>(
        context, layout, coord, info, source_shape, slice_shape,
        Std::make_index_sequence<NestingDepthV<decltype(source_shape)>>{},
        Std::make_index_sequence<NestingDepthV<coord_t>>{},
        Std::make_index_sequence<NestingDepthV<decltype(slice_shape)>>{});
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT
#undef TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS
#undef TENSOR_API_DETAIL_SLICE_FAIL

#define TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(ctx, value, name, api) \
    TENSOR_API_DEBUG_ASSERT_AT(                                      \
        ctx, (is_stride_valid((value).Stride())),                    \
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error<tensor_layout_error_kind::STRIDE>, (value), name, api))

template <typename layout_t>
__aicore__ inline constexpr void debug_check_layout(
    const tensor_api_assert_context& context, const layout_t& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, tensor_name, api_name));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, layout, tensor_name, api_name);
}

template <typename layout_t>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const layout_t& layout)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, "result", "MakeTensor"));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, layout, "result", "MakeTensor");
}

template <typename shape_t, typename stride_t>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const shape_t& shape, const stride_t& stride)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(shape)),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, MakeLayout(shape, stride), "result", "MakeTensor"));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, MakeLayout(shape, stride), "result", "MakeTensor");
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
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, layout, "source", "Slice");
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_slice_shape_valid(info)),
        TENSOR_API_REPORT_INTERNAL(report_slice_error<slice_error_kind::SLICE_SHAPE>, layout, coord, info));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_coord_in_shape(coord, layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_slice_error<slice_error_kind::COORD>, layout, coord, info));
}

#undef TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE

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

#if defined(TENSOR_API_DEBUG_CHECK_OWNS_INTERNAL_HEADER_ACCESS)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef TENSOR_API_DEBUG_CHECK_OWNS_INTERNAL_HEADER_ACCESS
#endif
