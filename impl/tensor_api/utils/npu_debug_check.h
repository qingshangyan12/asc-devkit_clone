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

#ifdef ASCENDC_DEBUG
template <size_t I, size_t TupleIndex = 0, typename Value>
__aicore__ inline constexpr decltype(auto) GetDebugTupleLeaf(const Value& value)
{
    using ValueType = Std::remove_cvref_t<Value>;
    static_assert(I < NestingDepthV<ValueType>, "The debug tuple leaf index must be in range.");
    if constexpr (Std::is_tuple_v<ValueType>) {
        using ElementType = typename Std::tuple_element<TupleIndex, ValueType>::type;
        if constexpr (I < NestingDepthV<ElementType>) {
            return GetDebugTupleLeaf<I, 0>(Std::get<TupleIndex>(value));
        } else {
            return GetDebugTupleLeaf<I - NestingDepthV<ElementType>, TupleIndex + 1>(value);
        }
    } else {
        static_assert(I == 0, "The debug scalar leaf index must be 0.");
        return value;
    }
}

// A scalar coord addresses one logical dimension represented by the product of its physical shape subtree.
template <size_t I, size_t TupleIndex = 0, typename Coord, typename Shape>
__aicore__ inline constexpr decltype(auto) GetDebugLogicalShapeLeaf(const Coord& coord, const Shape& shape)
{
    using CoordType = Std::remove_cvref_t<Coord>;
    static_assert(I < NestingDepthV<CoordType>, "The debug logical shape leaf index must be in range.");
    if constexpr (Std::is_tuple_v<CoordType>) {
        using ShapeType = Std::remove_cvref_t<Shape>;
        static_assert(Std::is_tuple_v<ShapeType>, "The coord and shape must describe compatible dimensions.");
        static_assert(Std::tuple_size_v<CoordType> == Std::tuple_size_v<ShapeType>,
            "The coord and shape must have the same rank.");
        using CoordElementType = typename Std::tuple_element<TupleIndex, CoordType>::type;
        if constexpr (I < NestingDepthV<CoordElementType>) {
            return GetDebugLogicalShapeLeaf<I, 0>(Std::get<TupleIndex>(coord), Std::get<TupleIndex>(shape));
        } else {
            return GetDebugLogicalShapeLeaf<I - NestingDepthV<CoordElementType>, TupleIndex + 1>(coord, shape);
        }
    } else {
        static_assert(I == 0, "The debug logical scalar leaf index must be 0.");
        return Product{}(shape);
    }
}

enum class TensorLayoutErrorKind {
    SHAPE,
    STRIDE
};

#define TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT(tupleFormat)                                  \
    do {                                                                                                 \
        if constexpr (Kind == TensorLayoutErrorKind::SHAPE) {                                            \
            TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                       \
                "Failed to check %s tensor shape in %s, current %sShape=(" tupleFormat                  \
                "); all %s tensor shape sizes must be greater than 0.",                                  \
                tensorName, apiName, tensorName,                                                         \
                static_cast<long long>(GetDebugTupleLeaf<Is>(layout.Shape()))..., tensorName);           \
        } else {                                                                                         \
            static_assert(Kind == TensorLayoutErrorKind::STRIDE, "Unsupported tensor layout error kind."); \
            TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                       \
                "Failed to check %s tensor stride in %s, current %sShape=(" tupleFormat                 \
                "), %sStride=(" tupleFormat "); all %s tensor strides must be greater than 0; a zero "  \
                "stride is allowed only when "                                                          \
                "the tensor format defines it and the corresponding shape size is 1.",                  \
                tensorName, apiName, tensorName,                                                         \
                static_cast<long long>(GetDebugTupleLeaf<Is>(layout.Shape()))..., tensorName,            \
                static_cast<long long>(GetDebugTupleLeaf<Is>(layout.Stride()))..., tensorName);          \
        }                                                                                                \
    } while (0)

template <TensorLayoutErrorKind Kind, typename LayoutType, size_t... Is>
__aicore__ inline void ReportTensorLayoutErrorImpl(const TensorApiAssertContext& context,
    const LayoutType& layout, __gm__ const char* tensorName, __gm__ const char* apiName,
    Std::index_sequence<Is...>)
{
    // NPU printf needs a literal format. Built-in tensor layouts contain at most five physical dimensions.
    if constexpr (sizeof...(Is) == 1) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld");
    } else if constexpr (sizeof...(Is) == 2) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld");
    } else if constexpr (sizeof...(Is) == 3) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld");
    } else if constexpr (sizeof...(Is) == 4) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(Is) == 5) {
        TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT("%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (Kind == TensorLayoutErrorKind::SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check %s tensor shape in %s, all %s tensor shape sizes must be greater than 0.",
            tensorName, apiName, tensorName);
    } else {
        static_assert(Kind == TensorLayoutErrorKind::STRIDE, "Unsupported tensor layout error kind.");
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check %s tensor stride in %s, all %s tensor strides must be greater than 0; a zero stride "
            "is allowed only when the tensor format defines it and the corresponding shape size is 1.",
            tensorName, apiName, tensorName);
    }
}

#undef TENSOR_API_DETAIL_REPORT_TENSOR_LAYOUT_WITH_FORMAT

template <TensorLayoutErrorKind Kind, typename LayoutType>
__aicore__ inline void ReportTensorLayoutError(const TensorApiAssertContext& context, const LayoutType& layout,
    __gm__ const char* tensorName, __gm__ const char* apiName)
{
    if constexpr (Kind == TensorLayoutErrorKind::SHAPE) {
        ReportTensorLayoutErrorImpl<Kind>(context, layout, tensorName, apiName,
            Std::make_index_sequence<NestingDepthV<decltype(layout.Shape())>>{});
    } else {
        static_assert(Kind == TensorLayoutErrorKind::STRIDE, "Unsupported tensor layout error kind.");
        ReportTensorLayoutErrorImpl<Kind>(context, layout, tensorName, apiName,
            Std::make_index_sequence<NestingDepthV<decltype(layout.Stride())>>{});
    }
}

enum class SliceErrorKind {
    SOURCE_SHAPE,
    SOURCE_STRIDE,
    SLICE_SHAPE,
    COORD
};

#define TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(logicalFormat, strideFormat)                   \
    do {                                                                                                 \
        TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                           \
            "Failed to check source tensor stride in Slice, sourceShape=(" logicalFormat                \
            "), sourceStride=(" strideFormat "), coord=(" logicalFormat "), sliceShape=("             \
            logicalFormat "); sourceStride must be valid.",                                             \
            static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, layout.Shape()))...,       \
            static_cast<long long>(GetDebugTupleLeaf<StrideIs>(layout.Stride()))...,                     \
            static_cast<long long>(GetDebugTupleLeaf<LogicalIs>(coord))...,                              \
            static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, sliceShape))...);           \
    } while (0)

template <typename LayoutType, typename Coord, typename SliceShape,
          size_t... LogicalIs, size_t... StrideIs>
__aicore__ inline void ReportSliceStrideError(const TensorApiAssertContext& context, const LayoutType& layout,
    const Coord& coord, const SliceShape& sliceShape, Std::index_sequence<LogicalIs...>,
    Std::index_sequence<StrideIs...>)
{
    if constexpr (sizeof...(LogicalIs) == 1 && sizeof...(StrideIs) == 1) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld", "%lld");
    } else if constexpr (sizeof...(LogicalIs) == 2 && sizeof...(StrideIs) == 2) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld", "%lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 2 && sizeof...(StrideIs) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld", "%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 3 && sizeof...(StrideIs) == 3) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT("%lld, %lld, %lld", "%lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 3 && sizeof...(StrideIs) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(
            "%lld, %lld, %lld", "%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 4 && sizeof...(StrideIs) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(
            "%lld, %lld, %lld, %lld", "%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 5 && sizeof...(StrideIs) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT(
            "%lld, %lld, %lld, %lld, %lld", "%lld, %lld, %lld, %lld, %lld");
    } else {
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check source tensor stride in Slice, all source tensor strides must be greater than 0; a zero "
            "stride is allowed only when the tensor format defines it and the corresponding shape size is 1.");
    }
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_STRIDE_WITH_FORMAT

#define TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT(tupleFormat)                                          \
    do {                                                                                                 \
        if constexpr (Kind == SliceErrorKind::SOURCE_SHAPE) {                                            \
            TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                       \
                "Failed to check source tensor shape in Slice, sourceShape=(" tupleFormat                \
                "), coord=(" tupleFormat "), sliceShape=(" tupleFormat "); sourceShape must be "       \
                "greater than 0.",                                                                       \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, layout.Shape()))...,   \
                static_cast<long long>(GetDebugTupleLeaf<LogicalIs>(coord))...,                          \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, sliceShape))...);       \
        } else if constexpr (Kind == SliceErrorKind::SLICE_SHAPE) {                                      \
            TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                       \
                "Failed to check slice shape in Slice, sourceShape=(" tupleFormat "), coord=("          \
                tupleFormat "), sliceShape=(" tupleFormat "); sliceShape must be greater than 0.",      \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, layout.Shape()))...,   \
                static_cast<long long>(GetDebugTupleLeaf<LogicalIs>(coord))...,                          \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, sliceShape))...);       \
        } else {                                                                                         \
            static_assert(Kind == SliceErrorKind::COORD, "Unsupported Slice error kind.");               \
            TENSOR_API_DETAIL_ASSERT_FAIL(context,                                                       \
                "Failed to check coord in Slice, sourceShape=(" tupleFormat "), coord=(" tupleFormat    \
                "), sliceShape=(" tupleFormat "); coord must be non-negative and within sourceShape.",  \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, layout.Shape()))...,   \
                static_cast<long long>(GetDebugTupleLeaf<LogicalIs>(coord))...,                          \
                static_cast<long long>(GetDebugLogicalShapeLeaf<LogicalIs>(coord, sliceShape))...);       \
        }                                                                                                \
    } while (0)

template <SliceErrorKind Kind, typename LayoutType, typename Coord, typename SliceShape,
          size_t... LogicalIs, size_t... StrideIs>
__aicore__ inline void ReportSliceErrorImpl(const TensorApiAssertContext& context, const LayoutType& layout,
    const Coord& coord, const SliceShape& sliceShape, Std::index_sequence<LogicalIs...> logicalIndices,
    Std::index_sequence<StrideIs...> strideIndices)
{
    // NPU printf needs a literal format. Built-in tensor layouts contain at most five logical dimensions.
    if constexpr (Kind == SliceErrorKind::SOURCE_STRIDE) {
        ReportSliceStrideError(context, layout, coord, sliceShape, logicalIndices, strideIndices);
    } else if constexpr (sizeof...(LogicalIs) == 1) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld");
    } else if constexpr (sizeof...(LogicalIs) == 2) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 3) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 4) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld, %lld");
    } else if constexpr (sizeof...(LogicalIs) == 5) {
        TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT("%lld, %lld, %lld, %lld, %lld");
    } else if constexpr (Kind == SliceErrorKind::SOURCE_SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check source tensor shape in Slice, all source tensor shape sizes must be greater than 0.");
    } else if constexpr (Kind == SliceErrorKind::SLICE_SHAPE) {
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check slice shape in Slice, all slice shape sizes must be greater than 0.");
    } else {
        static_assert(Kind == SliceErrorKind::COORD, "Unsupported Slice error kind.");
        TENSOR_API_DETAIL_ASSERT_FAIL(context,
            "Failed to check coord in Slice, coord must be non-negative and within the source tensor shape range.");
    }
}

#undef TENSOR_API_DETAIL_REPORT_SLICE_WITH_FORMAT

template <SliceErrorKind Kind, typename LayoutType, typename Coord, typename Info>
__aicore__ inline void ReportSliceError(
    const TensorApiAssertContext& context, const LayoutType& layout, const Coord& coord, const Info& info)
{
    if constexpr (IsLayoutV<Info>) {
        ReportSliceErrorImpl<Kind>(context, layout, coord, info.Shape(),
            Std::make_index_sequence<NestingDepthV<Coord>>{},
            Std::make_index_sequence<NestingDepthV<decltype(layout.Stride())>>{});
    } else {
        ReportSliceErrorImpl<Kind>(context, layout, coord, info,
            Std::make_index_sequence<NestingDepthV<Coord>>{},
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
