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
#warning                                                                                                               \
    "impl/tensor_api/tensor/layout_infer.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file layout_infer.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_INFER_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_INFER_H

#include "impl/tensor_api/tensor/layout_frame.h"

namespace AscendC {
namespace Te {

template <typename T, size_t Value>
struct IsIntValue : Std::false_type {};

template <size_t T, size_t Value>
struct IsIntValue<Std::Int<T>, Value> : Std::bool_constant<T == Value> {};

template <typename T, typename U, typename V>
struct IsIntProduct : Std::false_type {};

template <size_t T, size_t U, size_t V>
struct IsIntProduct<Std::Int<T>, Std::Int<U>, Std::Int<V>> : Std::bool_constant<T * U == V> {};

template <typename RowStride, typename ColumnStride>
struct InferFlatLayoutPattern {
    using type = Std::conditional_t<
        Std::is_same_v<Std::remove_cvref_t<ColumnStride>, _1>,
        NDLayoutPtn,
        Std::conditional_t<
            Std::is_same_v<Std::remove_cvref_t<RowStride>, _1>,
            DNLayoutPtn,
            Std::ignore_t>>;
};

template <typename ShapeRows, typename ShapeColumns, typename StrideRows, typename StrideColumns>
struct InferNestedLayoutPattern {
    using type = Std::ignore_t;
};

template <typename ShapeRow0, typename ShapeRow1, typename ShapeColumn0, typename ShapeColumn1,
    typename StrideRow0, typename StrideRow1, typename StrideColumn0, typename StrideColumn1>
struct InferNestedLayoutPattern<
    Shape<ShapeRow0, ShapeRow1>,
    Shape<ShapeColumn0, ShapeColumn1>,
    Stride<StrideRow0, StrideRow1>,
    Stride<StrideColumn0, StrideColumn1>> {
private:
    using RawShapeRow0 = Std::remove_cvref_t<ShapeRow0>;
    using RawShapeColumn0 = Std::remove_cvref_t<ShapeColumn0>;
    using RawStrideRow0 = Std::remove_cvref_t<StrideRow0>;
    using RawStrideRow1 = Std::remove_cvref_t<StrideRow1>;
    using RawStrideColumn0 = Std::remove_cvref_t<StrideColumn0>;
    using RawStrideColumn1 = Std::remove_cvref_t<StrideColumn1>;

    static constexpr bool isNDExt =
        Std::is_same_v<RawShapeRow0, _1> &&
        Std::is_same_v<RawShapeColumn0, _1> &&
        Std::is_same_v<RawStrideRow0, _0> &&
        Std::is_same_v<RawStrideColumn0, _0> &&
        Std::is_same_v<RawStrideColumn1, _1>;
    static constexpr bool isDNExt =
        Std::is_same_v<RawShapeRow0, _1> &&
        Std::is_same_v<RawShapeColumn0, _1> &&
        Std::is_same_v<RawStrideRow0, _0> &&
        Std::is_same_v<RawStrideRow1, _1> &&
        Std::is_same_v<RawStrideColumn0, _0>;
    static constexpr bool isNN =
        Std::is_same_v<RawShapeRow0, _2> &&
        Std::is_same_v<RawShapeColumn0, _16> &&
        Std::is_same_v<RawStrideRow0, _1> &&
        Std::is_same_v<RawStrideRow1, _32> &&
        Std::is_same_v<RawStrideColumn0, _2>;
    static constexpr bool isNZ =
        IsIntValue<RawShapeRow0, FRACTAL_FIXED>::value &&
        Std::is_same_v<RawStrideRow0, RawShapeColumn0> &&
        IsIntProduct<RawShapeRow0, RawShapeColumn0, RawStrideRow1>::value &&
        Std::is_same_v<RawStrideColumn0, _1>;
    static constexpr bool isZZ =
        IsIntValue<RawShapeRow0, FRACTAL_FIXED>::value &&
        Std::is_same_v<RawStrideRow0, RawShapeColumn0> &&
        Std::is_same_v<RawStrideColumn0, _1> &&
        IsIntProduct<RawShapeRow0, RawShapeColumn0, RawStrideColumn1>::value;
    static constexpr bool isZN =
        IsIntValue<RawShapeColumn0, FRACTAL_FIXED>::value &&
        Std::is_same_v<RawStrideRow0, _1> &&
        Std::is_same_v<RawStrideColumn0, RawShapeRow0> &&
        IsIntProduct<RawShapeRow0, RawShapeColumn0, RawStrideColumn1>::value;
    static constexpr bool isScaleADN =
        Std::is_same_v<RawShapeRow0, _1> &&
        Std::is_same_v<RawShapeColumn0, _2> &&
        Std::is_same_v<RawStrideRow0, _0> &&
        Std::is_same_v<RawStrideRow1, _2> &&
        Std::is_same_v<RawStrideColumn0, _1>;
    static constexpr bool isScaleBND =
        Std::is_same_v<RawShapeRow0, _2> &&
        Std::is_same_v<RawShapeColumn0, _1> &&
        Std::is_same_v<RawStrideRow0, _1> &&
        Std::is_same_v<RawStrideColumn0, _0> &&
        Std::is_same_v<RawStrideColumn1, _2>;

public:
    using type = Std::conditional_t<isNDExt, NDExtLayoutPtn,
        Std::conditional_t<isDNExt, DNExtLayoutPtn,
        Std::conditional_t<isNN, NNLayoutPtn,
        Std::conditional_t<isNZ, NZLayoutPtn,
        Std::conditional_t<isZZ, ZZLayoutPtn,
        Std::conditional_t<isZN, ZNLayoutPtn,
        Std::conditional_t<isScaleADN, ScaleADNLayoutPtn,
        Std::conditional_t<isScaleBND, ScaleBNDLayoutPtn, Std::ignore_t>>>>>>>>;
};

template <typename Row, typename Column, typename RowStride, typename ColumnStride,
    bool RowIsTuple = Std::is_tuple_v<Std::remove_cvref_t<Row>>,
    bool ColumnIsTuple = Std::is_tuple_v<Std::remove_cvref_t<Column>>,
    bool RowStrideIsTuple = Std::is_tuple_v<Std::remove_cvref_t<RowStride>>,
    bool ColumnStrideIsTuple = Std::is_tuple_v<Std::remove_cvref_t<ColumnStride>>>
struct InferTwoDimLayoutPattern {
    using type = Std::ignore_t;
};

template <typename Row, typename Column, typename RowStride, typename ColumnStride>
struct InferTwoDimLayoutPattern<Row, Column, RowStride, ColumnStride, false, false, false, false>
    : InferFlatLayoutPattern<RowStride, ColumnStride> {};

template <typename Row, typename Column, typename RowStride, typename ColumnStride>
struct InferTwoDimLayoutPattern<Row, Column, RowStride, ColumnStride, true, true, true, true>
    : InferNestedLayoutPattern<Row, Column, RowStride, ColumnStride> {};

template <typename Batch, typename MatrixShape, typename BatchStride, typename MatrixStride>
struct InferTwoDimLayoutPattern<Batch, MatrixShape, BatchStride, MatrixStride, false, true, false, true>
    : InferTwoDimLayoutPattern<
        typename Std::tuple_element<0, MatrixShape>::type,
        typename Std::tuple_element<1, MatrixShape>::type,
        typename Std::tuple_element<0, MatrixStride>::type,
        typename Std::tuple_element<1, MatrixStride>::type> {};

template <typename ShapeType, typename StrideType>
struct InferLayoutPattern {
    using type = Std::ignore_t;
};

template <typename Row, typename Column, typename RowStride, typename ColumnStride>
struct InferLayoutPattern<Shape<Row, Column>, Stride<RowStride, ColumnStride>>
    : InferTwoDimLayoutPattern<Row, Column, RowStride, ColumnStride> {};

template <typename Pattern, typename C0 = _1>
struct BuildInferredLayoutTrait {
    using type = Std::conditional_t<Std::is_same_v<Pattern, Std::ignore_t>,
        Std::ignore_t, LayoutTrait<Std::ignore_t, C0>>;
};

template <typename ShapeType, typename StrideType>
struct InferLayoutTrait {
    using type = Std::ignore_t;
};

template <typename Row, typename Column, typename RowStride, typename ColumnStride>
struct InferLayoutTrait<Shape<Row, Column>, Stride<RowStride, ColumnStride>> {
private:
    using PatternType = typename InferLayoutPattern<Shape<Row, Column>, Stride<RowStride, ColumnStride>>::type;

public:
    using type = typename BuildInferredLayoutTrait<PatternType>::type;
};

template <typename ShapeRow0, typename ShapeRow1, typename ShapeColumn0, typename ShapeColumn1,
    typename StrideRow0, typename StrideRow1, typename StrideColumn0, typename StrideColumn1>
struct InferLayoutTrait<
    Shape<Shape<ShapeRow0, ShapeRow1>, Shape<ShapeColumn0, ShapeColumn1>>,
    Stride<Stride<StrideRow0, StrideRow1>, Stride<StrideColumn0, StrideColumn1>>> {
private:
    using LayoutShape = Shape<Shape<ShapeRow0, ShapeRow1>, Shape<ShapeColumn0, ShapeColumn1>>;
    using LayoutStride = Stride<Stride<StrideRow0, StrideRow1>, Stride<StrideColumn0, StrideColumn1>>;
    using PatternType = typename InferLayoutPattern<LayoutShape, LayoutStride>::type;
    using C0Type = Std::conditional_t<Std::is_same_v<PatternType, NZLayoutPtn> ||
            Std::is_same_v<PatternType, ZZLayoutPtn>,
        ShapeColumn0,
        Std::conditional_t<Std::is_same_v<PatternType, ZNLayoutPtn> ||
                Std::is_same_v<PatternType, NNLayoutPtn> ||
                Std::is_same_v<PatternType, ScaleBNDLayoutPtn>,
            ShapeRow0,
            Std::conditional_t<Std::is_same_v<PatternType, ScaleADNLayoutPtn>, ShapeColumn0, _1>>>;

public:
    using type = typename BuildInferredLayoutTrait<PatternType, C0Type>::type;
};

template <typename Batch, typename ShapeRow0, typename ShapeRow1, typename ShapeColumn0, typename ShapeColumn1,
    typename BatchStride, typename StrideRow0, typename StrideRow1, typename StrideColumn0, typename StrideColumn1>
struct InferLayoutTrait<
    Shape<Batch, Shape<Shape<ShapeRow0, ShapeRow1>, Shape<ShapeColumn0, ShapeColumn1>>>,
    Stride<BatchStride, Stride<Stride<StrideRow0, StrideRow1>, Stride<StrideColumn0, StrideColumn1>>>>
    : InferLayoutTrait<
        Shape<Shape<ShapeRow0, ShapeRow1>, Shape<ShapeColumn0, ShapeColumn1>>,
        Stride<Stride<StrideRow0, StrideRow1>, Stride<StrideColumn0, StrideColumn1>>> {};

template <typename T, typename U>
struct GetLayoutInfo<Layout<T, U, Std::ignore_t>> {
    using type = Std::tuple<typename InferLayoutPattern<T, U>::type, typename InferLayoutTrait<T, U>::type>;
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_INFER_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
