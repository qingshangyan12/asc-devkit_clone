/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_H
#define IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_H

namespace asc {
namespace te {

template <size_t I, size_t BatchNum, typename BatchTuple, typename CapacityType>
__aicore__ inline constexpr auto BatchStrideAt(const BatchTuple& batches, const CapacityType& capacity)
{
    return batch_stride_at<I, BatchNum>(batches, capacity);
}

template <typename LayoutPattern, typename Trait, typename BatchTuple, typename BaseLayout, size_t... BatchIs>
__aicore__ inline constexpr auto MakeFlatBatchLayout(const BatchTuple& batches, const BaseLayout& base,
                                                     Std::index_sequence<BatchIs...> index)
{
    return make_flat_batch_layout<LayoutPattern, Trait, BatchIs...>(batches, base, index);
}

template <typename LayoutPattern, typename Trait, typename Maker, typename ArgsTuple, size_t... BatchIs>
__aicore__ inline constexpr auto MakeMultiBatchFrameLayoutImpl(const ArgsTuple& args,
                                                               Std::index_sequence<BatchIs...> index)
{
    return make_multi_batch_frame_layout_impl<LayoutPattern, Trait, Maker, BatchIs...>(args, index);
}

template <typename LayoutPattern, typename Trait, typename Maker, typename... Args>
__aicore__ inline constexpr auto MakeMultiBatchFrameLayout(const Args&... args)
{
    return make_multi_batch_frame_layout<LayoutPattern, Trait, Maker>(args...);
}

template <typename T, typename F, typename G, size_t... I>
__aicore__ inline constexpr auto TupleApply(T&& t, F&& f, G&& g, Std::index_sequence<I...> index)
{
    return tuple_apply<I...>(t, f, g, index);
}

template <typename T0, typename T1, typename F, typename G, size_t... I>
__aicore__ inline constexpr auto TupleApply(T0&& t0, T1&& t1, F&& f, G&& g, Std::index_sequence<I...> index)
{
    return tuple_apply<I...>(t0, t1, f, g, index);
}

template <typename T, typename F, typename G>
__aicore__ inline constexpr auto TransformApply(T&& t, F&& f, G&& g)
{
    return transform_apply(t, f, g);
}

template <typename T0, typename T1, typename F, typename G>
__aicore__ inline constexpr auto TransformApply(T0&& t0, T1&& t1, F&& f, G&& g)
{
    return transform_apply(t0, t1, f, g);
}

template <typename T, typename U, typename F, size_t... Is>
__aicore__ inline constexpr auto TransformTupleApplyImpl(const T& t, const U& u, const F& f,
                                                         Std::index_sequence<Is...> index)
{
    return transform_tuple_apply_impl<Is...>(t, u, f, index);
}

template <typename T, typename U, typename F>
__aicore__ inline constexpr auto TransformTupleApply(const T& t, const U& u, const F& f)
{
    return transform_tuple_apply(t, u, f);
}

template <size_t I, typename Tuple, typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get<I>(tuple);
}

template <size_t I0, size_t I1, size_t... Is, typename Tuple,
          typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get<I0, I1, Is...>(tuple);
}

template <typename Tuple, typename = Std::enable_if_t<Std::is_tuple_v<Std::remove_cvref_t<Tuple>>>>
__aicore__ inline constexpr auto Get(Tuple&& tuple)
{
    return get(tuple);
}

template <size_t... Is, typename Tuple>
__aicore__ inline constexpr auto GetTuple(Tuple&& tuple)
{
    return get_tuple<Is...>(tuple);
}

template <size_t... Is, typename Tuple>
__aicore__ inline constexpr auto GetRank(const Tuple& tuple)
{
    return get_rank<Is...>(tuple);
}

template <size_t... Is, typename Tuple>
__aicore__ inline constexpr auto TupleSize(const Tuple& tuple)
{
    return tuple_size<Is...>(tuple);
}

template <size_t I, typename Tuple>
__aicore__ inline constexpr auto SelectTuple(Tuple&& tuple)
{
    return select_tuple<I>(tuple);
}

template <size_t I0, size_t I1, size_t... Is, typename Tuple>
__aicore__ inline constexpr auto SelectTuple(Tuple&& tuple)
{
    return select_tuple<I0, I1, Is...>(tuple);
}

template <typename Tuple>
__aicore__ inline constexpr auto SelectTuple(Tuple&& tuple)
{
    return select_tuple(tuple);
}

template <size_t Index, size_t I, size_t... Is, typename Tuple>
__aicore__ inline constexpr decltype(auto) GetValue(const Tuple& tuple)
{
    return get_value<Index, I, Is...>(tuple);
}

template <size_t Index, typename Tuple>
__aicore__ inline constexpr decltype(auto) GetValue(const Tuple& tuple)
{
    return get_value<Index>(tuple);
}

template <typename T0, typename... Ts>
__aicore__ inline constexpr auto GetMax(const T0& t0, const Ts&... ts)
{
    return get_max(t0, ts...);
}

template <typename ShapeType, typename StrideType, size_t... Is>
__aicore__ inline constexpr auto GetCapacityImpl(const ShapeType& shape, const StrideType& stride,
                                                 Std::index_sequence<Is...> index)
{
    return get_capacity_impl<Is...>(shape, stride, index);
}

template <typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto GetCapacity(const ShapeType& shape, const StrideType& stride)
{
    return get_capacity(shape, stride);
}

template <typename Tuple, size_t... Is>
__aicore__ inline constexpr auto SelectBySeqImpl(const Tuple& tuple, Std::index_sequence<Is...> index)
{
    return select_by_seq_impl<Is...>(tuple, index);
}

template <typename Seq, typename Tuple>
__aicore__ inline constexpr auto SelectBySeq(const Tuple& tuple)
{
    return select_by_seq<Seq>(tuple);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Shape<T, Ts...> MakeShape(const T& value, const Ts&... values)
{
    return make_shape(value, values...);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Stride<T, Ts...> MakeStride(const T& value, const Ts&... values)
{
    return make_stride(value, values...);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Tile<T, Ts...> MakeTile(const T& value, const Ts&... values)
{
    return make_tile(value, values...);
}

template <typename T, typename... Ts>
__aicore__ inline constexpr Coord<T, Ts...> MakeCoord(const T& value, const Ts&... values)
{
    return make_coord(value, values...);
}

template <typename T, typename U>
__aicore__ inline constexpr auto MakeLayout(const T& shape, const U& stride)
{
    return make_layout(shape, stride);
}

template <typename ShapeType>
__aicore__ inline constexpr auto MakeLayout(const ShapeType& shape)
{
    return make_layout(shape);
}

template <typename Row, typename Col, size_t... Is>
__aicore__ inline constexpr auto BuildStrideRowImpl(const Row& row, const Col& col, Std::index_sequence<Is...> index)
{
    return build_stride_row_impl<Is...>(row, col, index);
}

template <typename Row, typename Col, size_t... Is>
__aicore__ inline constexpr auto BuildStrideColImpl(const Row& row, const Col& col, Std::index_sequence<Is...> index)
{
    return build_stride_col_impl<Is...>(row, col, index);
}

template <typename ShapeType>
__aicore__ inline constexpr auto ComputeStride(const ShapeType& shape)
{
    return compute_stride(shape);
}

template <typename ShapeType, size_t... Is>
__aicore__ inline constexpr auto BuildFlatStrideImpl(const ShapeType& shape, Std::index_sequence<Is...> index)
{
    return build_flat_stride_impl<Is...>(shape, index);
}

template <typename ShapeType>
__aicore__ inline constexpr auto ComputeFlatStride(const ShapeType& shape)
{
    return compute_flat_stride(shape);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetShape(const LayoutType& layout)
{
    return get_shape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetShape(LayoutType& layout)
{
    return get_shape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetStride(const LayoutType& layout)
{
    return get_stride<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto GetStride(LayoutType& layout)
{
    return get_stride<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Coshape(const LayoutType& layout)
{
    return coshape<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Cosize(const LayoutType& layout)
{
    return cosize<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Rank(const LayoutType& layout)
{
    return rank<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Select(const LayoutType& layout)
{
    return select<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Get(const LayoutType& layout)
{
    return get<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Size(const LayoutType& layout)
{
    return size<Is...>(layout);
}

template <size_t... Is, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Capacity(const LayoutType& layout)
{
    return capacity<Is...>(layout);
}

template <typename Tensor, typename CoordType, typename Info>
__aicore__ inline constexpr decltype(auto) Slice(Tensor&& tensor, const CoordType& coord, const Info& info)
{
    return slice(tensor, coord, info);
}

template <typename T, typename U, size_t... Is>
__aicore__ inline decltype(auto) MakeFractalShape(T origin_shape, U inner_shape, Std::index_sequence<Is...> index)
{
    return make_fractal_shape<Is...>(origin_shape, inner_shape, index);
}

template <typename T, typename U>
__aicore__ inline decltype(auto) MakeFractalShape(T origin_shape, U inner_shape)
{
    return make_fractal_shape(origin_shape, inner_shape);
}

template <typename CoordType, typename LayoutType>
__aicore__ inline decltype(auto) MakeCoordLayout(const CoordType& coord, const LayoutType& layout)
{
    return make_coord_layout(coord, layout);
}

template <typename LayoutType, typename ShapeType>
__aicore__ inline decltype(auto) MakeSlicePatternLayout(const LayoutType& layout, const ShapeType& shape)
{
    return make_slice_pattern_layout(layout, shape);
}

template <typename CoordType, typename LayoutType, typename SliceShape>
__aicore__ inline decltype(auto) MakeSameShapeSliceLayout(const CoordType& coord, const LayoutType& layout,
                                                          const SliceShape& slice_shape)
{
    return make_same_shape_slice_layout(coord, layout, slice_shape);
}

template <typename CoordType, typename LayoutType, typename SliceShape>
__aicore__ inline decltype(auto) MakeFourDimSliceLayout(const CoordType& coord, const LayoutType& layout,
                                                        const SliceShape& slice_shape)
{
    return make_four_dim_slice_layout(coord, layout, slice_shape);
}

template <typename CoordType, typename LayoutType, typename SliceShape, size_t... BatchIs>
__aicore__ inline decltype(auto) MakeFlatBatchSliceLayoutImpl(const CoordType& coord, const LayoutType& layout,
                                                              const SliceShape& slice_shape,
                                                              Std::index_sequence<BatchIs...> index)
{
    return make_flat_batch_slice_layout_impl<BatchIs...>(coord, layout, slice_shape, index);
}

template <typename CoordType, typename LayoutType, typename SliceShape>
__aicore__ inline decltype(auto) MakeFlatBatchSliceLayout(const CoordType& coord, const LayoutType& layout,
                                                          const SliceShape& slice_shape)
{
    return make_flat_batch_slice_layout(coord, layout, slice_shape);
}

template <typename CoordType, typename LayoutType, typename SliceShape>
__aicore__ inline decltype(auto) MakeSliceLayout(const CoordType& coord, const LayoutType& layout,
                                                 const SliceShape& slice_shape)
{
    return make_slice_layout(coord, layout, slice_shape);
}

template <typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr decltype(auto) RemoveBatchDim(const LayoutType& layout)
{
    return remove_batch_dim(layout);
}

template <typename Pattern, typename Trait, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakeSqueezedLayout(const ShapeType& shape, const StrideType& stride)
{
    return make_squeezed_layout<Pattern, Trait>(shape, stride);
}

template <size_t... SqueezeDims, typename LayoutType,
          typename = Std::enable_if_t<is_layout_v<LayoutType> && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto SqueezeLayout(const LayoutType& layout)
{
    return squeeze_layout<SqueezeDims...>(layout);
}

template <typename Pattern, typename LayoutType,
          typename = Std::enable_if_t<is_layout_v<LayoutType> && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto SqueezeLayout(const LayoutType& layout, const Pattern& pattern)
{
    return squeeze_layout(layout, pattern);
}

template <typename ShapeNode, typename DimNode, typename PatternNode>
__aicore__ inline constexpr auto SqueezeRecNode(const ShapeNode& shape, const DimNode& dim, const PatternNode& pattern)
{
    return squeeze_rec_node(shape, dim, pattern);
}

template <typename ShapeType, typename DimType>
__aicore__ inline constexpr auto SqueezeUnwrap(const ShapeType& shape, const DimType& dim)
{
    return squeeze_unwrap(shape, dim);
}

template <typename ShapeType, typename DimType, typename Pattern, size_t... Is>
__aicore__ inline constexpr auto SqueezeRecTupleLevelImpl(const ShapeType& shape, const DimType& dim,
                                                          const Pattern& pattern, Std::index_sequence<Is...> index)
{
    return squeeze_rec_tuple_level_impl<Is...>(shape, dim, pattern, index);
}

template <typename ShapeType, typename DimType, typename Pattern>
__aicore__ inline constexpr auto SqueezeRecTupleLevel(const ShapeType& shape, const DimType& dim,
                                                      const Pattern& pattern)
{
    return squeeze_rec_tuple_level(shape, dim, pattern);
}

template <typename T, typename U, typename S, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxTTT(const T& coord, const U& shape, const S& stride,
                                            Std::index_sequence<Is...> index)
{
    return crd2idx_ttt<Is...>(coord, shape, stride, index);
}

template <typename T, typename U, typename S, size_t I0, size_t... Is>
__aicore__ inline constexpr auto Crd2IdxITT(const T& coord, const U& shape, const S& stride,
                                            Std::index_sequence<I0, Is...> index)
{
    return crd2idx_itt<I0, Is...>(coord, shape, stride, index);
}

template <typename T, typename U, typename S>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const U& shape, const S& stride)
{
    return crd2idx(coord, shape, stride);
}

template <typename T, typename LayoutType, typename = Std::enable_if_t<is_layout_v<LayoutType>>>
__aicore__ inline constexpr auto Crd2Idx(const T& coord, const LayoutType& layout)
{
    return crd2idx(coord, layout);
}

template <typename LayoutPattern, typename TraitType = Std::ignore_t, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
{
    return make_frame_layout<LayoutPattern, TraitType>(args...);
}

template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) MakeFrameLayout(const Args&... args)
{
    return make_frame_layout<LayoutPattern, C0Element>(args...);
}

template <typename LayoutPattern, typename TraitType, typename ShapeType, typename StrideType>
__aicore__ inline constexpr auto MakePatternLayout(const ShapeType& shape, const StrideType& stride)
{
    return make_pattern_layout<LayoutPattern, TraitType>(shape, stride);
}

template <typename LayoutPattern, typename TraitType, typename Batch, typename LayoutType>
__aicore__ inline constexpr auto MakeBatchPatternLayout(const Batch& batch, const LayoutType& layout)
{
    return make_batch_pattern_layout<LayoutPattern, TraitType>(batch, layout);
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_LEGACY_TENSOR_LAYOUT_H
