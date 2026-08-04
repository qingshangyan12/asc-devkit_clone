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
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_same_format_tag, const tensor_api_assert_context& context, const LayoutType& layout,
    const CoordType& coord, const InfoType& info, const SourceShapeType& source_shape,
    const SliceShapeType& slice_shape, Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<LayoutType>;
    using slice_view_type = debug_slice_info_view<InfoType>;
    using source_format_type = debug_tuple_format_t<SourceShapeType>;
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
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_flat_source_tag, const tensor_api_assert_context& context, const LayoutType& layout,
    const CoordType& coord, const InfoType& info, const SourceShapeType& source_shape,
    const SliceShapeType& slice_shape, Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<LayoutType>;
    using slice_view_type = debug_slice_info_view<InfoType>;
    using coord_format_type = debug_tuple_format_t<CoordType>;
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
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_nested_source_tag, const tensor_api_assert_context& context, const LayoutType& layout,
    const CoordType& coord, const InfoType& info, const SourceShapeType& source_shape,
    const SliceShapeType& slice_shape, Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<LayoutType>;
    using slice_view_type = debug_slice_info_view<InfoType>;
    using coord_format_type = debug_tuple_format_t<CoordType>;
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
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_rank_expanded_tag, const tensor_api_assert_context& context, const LayoutType& layout,
    const CoordType& coord, const InfoType& info, const SourceShapeType& source_shape,
    const SliceShapeType& slice_shape, Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    using view_type = debug_layout_view<LayoutType>;
    using slice_view_type = debug_slice_info_view<InfoType>;
    using source_format_type = debug_tuple_format_t<SourceShapeType>;
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
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_by_format(
    debug_slice_unsupported_format_tag, const tensor_api_assert_context& context, const LayoutType& layout,
    const CoordType& coord, const InfoType& info, const SourceShapeType& source_shape,
    const SliceShapeType& slice_shape, Std::index_sequence<source_indices...>, Std::index_sequence<coord_indices...>,
    Std::index_sequence<slice_indices...>)
{
    report_slice_unsupported<kind>(context, layout, coord, info, source_shape, slice_shape);
}

template <
    slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType, typename SourceShapeType,
    typename SliceShapeType, size_t... source_indices, size_t... coord_indices, size_t... slice_indices>
__aicore__ inline void report_slice_error_impl(
    const tensor_api_assert_context& context, const LayoutType& layout, const CoordType& coord, const InfoType& info,
    const SourceShapeType& source_shape, const SliceShapeType& slice_shape,
    Std::index_sequence<source_indices...> source_seq, Std::index_sequence<coord_indices...> coord_seq,
    Std::index_sequence<slice_indices...> slice_seq)
{
    using category_type = debug_slice_format_category_t<
        debug_tuple_format_t<SourceShapeType>, debug_tuple_format_t<CoordType>, debug_tuple_format_t<SliceShapeType>>;
    report_slice_error_by_format<kind>(
        category_type{}, context, layout, coord, info, source_shape, slice_shape, source_seq, coord_seq, slice_seq);
}

template <slice_error_kind kind, typename LayoutType, typename CoordType, typename InfoType>
__aicore__ inline void report_slice_error(
    const tensor_api_assert_context& context, const LayoutType& layout, const CoordType& coord, const InfoType& info)
{
    auto source_shape = debug_layout_view<LayoutType>::shape(layout);
    auto slice_shape = debug_slice_info_view<InfoType>::shape(info);
    report_slice_error_impl<kind>(
        context, layout, coord, info, source_shape, slice_shape,
        Std::make_index_sequence<NestingDepthV<decltype(source_shape)>>{},
        Std::make_index_sequence<NestingDepthV<CoordType>>{},
        Std::make_index_sequence<NestingDepthV<decltype(slice_shape)>>{});
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_SAME_FORMAT
#undef TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMATS
#undef TENSOR_API_DETAIL_SLICE_FAIL

#define TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(ctx, value, name, api) \
    TENSOR_API_DEBUG_ASSERT_AT(                                      \
        ctx, (is_stride_valid((value).Stride())),                    \
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error<tensor_layout_error_kind::STRIDE>, (value), name, api))

template <typename LayoutType>
__aicore__ inline constexpr void debug_check_layout(
    const tensor_api_assert_context& context, const LayoutType& layout, __gm__ const char* tensor_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, tensor_name, api_name));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, layout, tensor_name, api_name);
}

template <typename LayoutType>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const LayoutType& layout)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(layout.Shape())),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, layout, "result", "MakeTensor"));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, layout, "result", "MakeTensor");
}

template <typename ShapeType, typename StrideType>
__aicore__ inline constexpr void debug_check_make_tensor_shape(
    const tensor_api_assert_context& context, const ShapeType& shape, const StrideType& stride)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (is_shape_valid(shape)),
        TENSOR_API_REPORT_INTERNAL(report_tensor_layout_error, MakeLayout(shape, stride), "result", "MakeTensor"));
    TENSOR_API_DETAIL_CHECK_LAYOUT_STRIDE(context, MakeLayout(shape, stride), "result", "MakeTensor");
}

template <typename SrcTensorType, typename DstTensorType>
__aicore__ inline constexpr void debug_check_copy_size(
    const tensor_api_assert_context& context, const SrcTensorType& src, const DstTensorType& dst,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (dst.Size() >= src.Size()), TENSOR_API_REPORT_INTERNAL(report_copy_size_error, src, dst, api_name));
}

template <typename LayoutType, typename CoordType, typename InfoType>
__aicore__ inline constexpr void debug_check_slice_args(
    const tensor_api_assert_context& context, const LayoutType& layout, const CoordType& coord, const InfoType& info)
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

template <typename ParamsType>
__aicore__ inline constexpr void debug_check_mmad_params(
    const tensor_api_assert_context& context, const ParamsType& params, bool disable_gemv, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.m <= DEBUG_MMAD_DIM_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check m in %s, m must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_MMAD_DIM_MIN), static_cast<unsigned int>(DEBUG_MMAD_DIM_MAX),
            static_cast<unsigned int>(params.m)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.n <= DEBUG_MMAD_DIM_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check n in %s, n must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_MMAD_DIM_MIN), static_cast<unsigned int>(DEBUG_MMAD_DIM_MAX),
            static_cast<unsigned int>(params.n)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (params.k <= DEBUG_MMAD_DIM_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check k in %s, k must be in range [%u, %u]; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_MMAD_DIM_MIN), static_cast<unsigned int>(DEBUG_MMAD_DIM_MAX),
            static_cast<unsigned int>(params.k)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (disable_gemv || params.m == DEBUG_GEMV_M_SIZE),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check m in %s, m must be %u when GEMV is enabled; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_GEMV_M_SIZE), static_cast<unsigned int>(params.m)));
}

template <typename ValueType>
__aicore__ inline constexpr void debug_check_unit_flag(
    const tensor_api_assert_context& context, const ValueType& unit_flag, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (unit_flag == DEBUG_UNIT_FLAG_DEFAULT || unit_flag == DEBUG_UNIT_FLAG_KEEP ||
         unit_flag == DEBUG_UNIT_FLAG_RESET),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check unitFlag in %s, unitFlag must be %u, %u, or %u; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_UNIT_FLAG_DEFAULT), static_cast<unsigned int>(DEBUG_UNIT_FLAG_KEEP),
            static_cast<unsigned int>(DEBUG_UNIT_FLAG_RESET), static_cast<unsigned int>(unit_flag)));
}

template <typename CountType>
__aicore__ inline constexpr void debug_check_block_count(
    const tensor_api_assert_context& context, const CountType& block_count, __gm__ const char* value_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_count >= DEBUG_BLOCK_COUNT_MIN && block_count <= DEBUG_BLOCK_COUNT_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be in range [%u, %u]; current value is %lld.", value_name, api_name,
            value_name, static_cast<unsigned int>(DEBUG_BLOCK_COUNT_MIN),
            static_cast<unsigned int>(DEBUG_BLOCK_COUNT_MAX), static_cast<long long>(block_count)));
}

template <typename LengthType, typename MaxLengthType>
__aicore__ inline constexpr void debug_check_block_len(
    const tensor_api_assert_context& context, const LengthType& block_len, const MaxLengthType& max_block_len,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_len >= DEBUG_BLOCK_LEN_MIN && block_len <= max_block_len),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockLen in %s, blockLen must be in range [%u, %llu]; current value is %llu.", api_name,
            static_cast<unsigned int>(DEBUG_BLOCK_LEN_MIN), static_cast<unsigned long long>(max_block_len),
            static_cast<unsigned long long>(block_len)));
}

template <typename ValueType, typename MaxValueType>
__aicore__ inline constexpr void debug_check_block_limit(
    const tensor_api_assert_context& context, const ValueType& value, const MaxValueType& max_value,
    __gm__ const char* value_name, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (value <= max_value),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be in range [%u, %llu]; current value is %llu.", value_name, api_name,
            value_name, static_cast<unsigned int>(DEBUG_COPY_NOP_VALUE), static_cast<unsigned long long>(max_value),
            static_cast<unsigned long long>(value)));
}

template <typename StrideType, typename LengthType, typename CountType>
__aicore__ inline constexpr void debug_check_gm2ub_stride(
    const tensor_api_assert_context& context, const StrideType& dst_stride, const LengthType& block_len,
    const CountType& block_count, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (block_count == DEBUG_COPY_NOP_VALUE || block_len == DEBUG_COPY_NOP_VALUE ||
         (block_count == DEBUG_SINGLE_BLOCK_COUNT && dst_stride == DEBUG_COPY_NOP_VALUE) ||
         dst_stride == static_cast<StrideType>(block_len) ||
         (dst_stride > DEBUG_COPY_NOP_VALUE && dst_stride % DEBUG_DATA_BLOCK_BYTES == 0)),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check dstStride in %s, for an active copy dstStride must equal blockLen, be a positive "
            "multiple of %u bytes, or be %u when blockCount is %u; current dstStride is %lld, blockLen is %llu, "
            "blockCount is %llu.",
            api_name, static_cast<unsigned int>(DEBUG_DATA_BLOCK_BYTES),
            static_cast<unsigned int>(DEBUG_COPY_NOP_VALUE), static_cast<unsigned int>(DEBUG_SINGLE_BLOCK_COUNT),
            static_cast<long long>(dst_stride), static_cast<unsigned long long>(block_len),
            static_cast<unsigned long long>(block_count)));
}

template <typename StrideType, typename LengthType, typename CountType>
__aicore__ inline constexpr void debug_check_ub2gm_stride(
    const tensor_api_assert_context& context, const StrideType& src_stride, const LengthType& block_len,
    const CountType& block_count, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (block_count == DEBUG_COPY_NOP_VALUE || block_len == DEBUG_COPY_NOP_VALUE ||
         (src_stride > DEBUG_COPY_NOP_VALUE &&
          (src_stride == static_cast<StrideType>(block_len) || src_stride % DEBUG_DATA_BLOCK_BYTES == 0))),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check srcStride in %s, for an active copy srcStride must equal blockLen or be a positive "
            "multiple of %u bytes; current srcStride is %lld, blockLen is %llu, blockCount is %llu.",
            api_name, static_cast<unsigned int>(DEBUG_DATA_BLOCK_BYTES), static_cast<long long>(src_stride),
            static_cast<unsigned long long>(block_len), static_cast<unsigned long long>(block_count)));
}

template <typename BatchType>
__aicore__ inline constexpr void debug_check_batch_count(
    const tensor_api_assert_context& context, const BatchType& batch, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (batch <= DEBUG_BATCH_COUNT_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src batch shape size in %s, src batch shape size must be at most %u; current value is "
            "%lld.",
            api_name, static_cast<unsigned int>(DEBUG_BATCH_COUNT_MAX), static_cast<long long>(batch)));
}

template <typename SrcBatchType, typename DstBatchType>
__aicore__ inline constexpr void debug_check_batch_match(
    const tensor_api_assert_context& context, const SrcBatchType& src_batch, const DstBatchType& dst_batch,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_batch == dst_batch),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src and dst batch shape sizes in %s, src batch shape size must equal dst batch shape "
            "size; current srcBatchShape is %lld, dstBatchShape is %lld.",
            api_name, static_cast<long long>(src_batch), static_cast<long long>(dst_batch)));
}

template <typename SrcColumnBytesType, typename DstColumnBytesType>
__aicore__ inline constexpr void debug_check_l12bt_column_bytes(
    const tensor_api_assert_context& context, const SrcColumnBytesType& src_column_bytes,
    const DstColumnBytesType& dst_column_bytes, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_column_bytes % DEBUG_DATA_BLOCK_BYTES == 0 && dst_column_bytes % DEBUG_DATA_BLOCK_BYTES == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check src and dst column byte sizes in %s, src and dst column byte sizes must be %u-byte "
            "aligned; current srcColumnBytes is %llu, dstColumnBytes is %llu.",
            api_name, static_cast<unsigned int>(DEBUG_DATA_BLOCK_BYTES),
            static_cast<unsigned long long>(src_column_bytes), static_cast<unsigned long long>(dst_column_bytes)));
}

template <typename SrcStrideType, typename DstStrideType>
__aicore__ inline constexpr void debug_check_fixpipe_stride(
    const tensor_api_assert_context& context, const SrcStrideType& src_stride, const DstStrideType& dst_stride,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (dst_stride > DEBUG_COPY_NOP_VALUE),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check dstStride in %s, dstStride must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_COPY_NOP_VALUE), static_cast<unsigned int>(dst_stride)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (src_stride <= DEBUG_FIXPIPE_SRC_STRIDE_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check srcStride in %s, srcStride must be at most %u; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_FIXPIPE_SRC_STRIDE_MAX), static_cast<unsigned int>(src_stride)));
}

template <typename SizeType>
__aicore__ inline constexpr void debug_check_fixpipe_m(
    const tensor_api_assert_context& context, const SizeType& m_size, bool nz2nd_en, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (m_size >= DEBUG_FIXPIPE_M_MIN &&
         ((!nz2nd_en && m_size <= DEBUG_FIXPIPE_M_MAX) || (nz2nd_en && m_size <= DEBUG_FIXPIPE_M_ND_MAX))),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check mSize in %s, mSize must be in range [%u, %u] when the dst format is ND and [%u, %u] "
            "otherwise; current mSize is %u, nz2ndEn is %u.",
            api_name, static_cast<unsigned int>(DEBUG_FIXPIPE_M_MIN), static_cast<unsigned int>(DEBUG_FIXPIPE_M_ND_MAX),
            static_cast<unsigned int>(DEBUG_FIXPIPE_M_MIN), static_cast<unsigned int>(DEBUG_FIXPIPE_M_MAX),
            static_cast<unsigned int>(m_size), static_cast<unsigned int>(nz2nd_en)));
}

template <typename SizeType>
__aicore__ inline constexpr void debug_check_fixpipe_n(
    const tensor_api_assert_context& context, const SizeType& n_size, bool enable_channel_split, bool nz2nd_en,
    bool nz2dn_en, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (nz2nd_en || nz2dn_en ||
         (enable_channel_split ? n_size % DEBUG_FIXPIPE_SPLIT_N_MULTIPLE == 0 :
                                 n_size % DEBUG_FIXPIPE_N_MULTIPLE == 0)),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check nSize in %s NZ instruction, nSize must be divisible by %u when enableChannelSplit is "
            "enabled or by %u otherwise; current nSize is %u, enableChannelSplit is %u, nz2ndEn is %u, nz2dnEn is "
            "%u.",
            api_name, static_cast<unsigned int>(DEBUG_FIXPIPE_SPLIT_N_MULTIPLE),
            static_cast<unsigned int>(DEBUG_FIXPIPE_N_MULTIPLE), static_cast<unsigned int>(n_size),
            static_cast<unsigned int>(enable_channel_split), static_cast<unsigned int>(nz2nd_en),
            static_cast<unsigned int>(nz2dn_en)));
}

template <typename CountType, typename LengthType>
__aicore__ inline constexpr void debug_check_copy_blocks(
    const tensor_api_assert_context& context, const CountType& block_count, const LengthType& block_len,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_count > DEBUG_COPY_NOP_VALUE),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockCount in %s, blockCount must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_COPY_NOP_VALUE), static_cast<unsigned int>(block_count)));
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (block_len > DEBUG_COPY_NOP_VALUE),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check blockLen in %s, blockLen must be greater than %u; current value is %u.", api_name,
            static_cast<unsigned int>(DEBUG_COPY_NOP_VALUE), static_cast<unsigned int>(block_len)));
}

template <typename DataSizeType, typename StepType>
__aicore__ inline constexpr void debug_check_l0_transpose(
    const tensor_api_assert_context& context, bool transpose, const DataSizeType& data_size, const StepType& k_step,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context,
        (!transpose || data_size != DEBUG_L0_TRANSPOSE_DATA_BYTES || k_step % DEBUG_L0_TRANSPOSE_K_STEP_MULTIPLE == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check kStep in %s transpose instruction, kStep must be divisible by %u for %u-byte data; "
            "current value is %u.",
            api_name, static_cast<unsigned int>(DEBUG_L0_TRANSPOSE_K_STEP_MULTIPLE),
            static_cast<unsigned int>(DEBUG_L0_TRANSPOSE_DATA_BYTES), static_cast<unsigned int>(k_step)));
}

template <typename StepType>
__aicore__ inline constexpr void debug_check_l0_step(
    const tensor_api_assert_context& context, const StepType& step, __gm__ const char* step_name,
    __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (step <= DEBUG_L0_STEP_MAX),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check %s in %s, %s must be at most %u; current value is %lld.", step_name, api_name, step_name,
            static_cast<unsigned int>(DEBUG_L0_STEP_MAX), static_cast<long long>(step)));
}

template <typename StepType>
__aicore__ inline constexpr void debug_check_l0_m_step(
    const tensor_api_assert_context& context, const StepType& m_step, bool is_b4, __gm__ const char* api_name)
{
    TENSOR_API_DEBUG_ASSERT_AT(
        context, (m_step % (is_b4 ? DEBUG_L0_B4_M_STEP_MULTIPLE : DEBUG_L0_B8_M_STEP_MULTIPLE) == 0),
        TENSOR_API_LOG_INTERNAL(
            "Failed to check mStep in %s, mStep must be divisible by %u for b4 or by %u for b8; current value is "
            "%lld.",
            api_name, static_cast<unsigned int>(DEBUG_L0_B4_M_STEP_MULTIPLE),
            static_cast<unsigned int>(DEBUG_L0_B8_M_STEP_MULTIPLE), static_cast<long long>(m_step)));
}

template <typename SrcStrideType, typename SrcCapacityType, typename DstStrideType, typename DstCapacityType>
__aicore__ inline constexpr void debug_check_l0_batch_stride(
    const tensor_api_assert_context& context, const SrcStrideType& src_stride, const SrcCapacityType& src_capacity,
    const DstStrideType& dst_stride, const DstCapacityType& dst_capacity, __gm__ const char* api_name)
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
