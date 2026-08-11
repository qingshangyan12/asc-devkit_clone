/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#warning                                                                                                               \
    "impl/tensor_api/tensor/layout_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file layout_impl.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_IMPL_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_IMPL_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/tensor/layout_method.h"
#include "impl/tensor_api/tensor/coord_index.h"
#include "impl/tensor_api/tensor/layout_fractal.h"
#include "impl/tensor_api/tensor/layout_frame.h"
#include "impl/tensor_api/tensor/layout_infer.h"

namespace asc {
namespace te {

struct zero_coord_type {};

inline constexpr zero_coord_type zero_coord{};

struct min_op {
    template <typename T, typename U>
    __aicore__ inline constexpr auto operator()(const T& src, const U& dst) const
    {
        return Std::min(src, dst);
    }
};

struct diff_op {
    template <typename T, typename U>
    __aicore__ inline constexpr auto operator()(const T& shape, const U& coord) const
    {
        return shape - coord;
    }
};

template <typename T>
__aicore__ inline constexpr auto make_zero_coord_like(const T& value);

template <typename T, size_t... Is>
__aicore__ inline constexpr auto make_zero_coord_like_impl(const T& value, Std::index_sequence<Is...>)
{
    return make_coord(make_zero_coord_like(Std::get<Is>(value))...);
}

template <typename T>
__aicore__ inline constexpr auto make_zero_coord_like(const T& value)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<T>>) {
        return make_zero_coord_like_impl(
            value, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<T>>>{});
    } else {
        return _0{};
    }
}

template <typename layout_type, typename SliceShape>
__aicore__ inline constexpr decltype(auto) resolve_copy_coord(
    const layout_type& layout, const SliceShape& slice_shape, const zero_coord_type&)
{
    using origin_shape = Std::remove_cvref_t<decltype(layout.shape())>;
    using slice_shape_type = Std::remove_cvref_t<SliceShape>;
    constexpr auto origin_shape_depth = nesting_depth_v<origin_shape>;
    constexpr auto slice_shape_depth = nesting_depth_v<slice_shape_type>;
    constexpr bool is_same_shape = origin_shape_depth == slice_shape_depth &&
        Std::tuple_size_v<origin_shape> == Std::tuple_size_v<slice_shape_type>;
    constexpr size_t batch_num = Std::tuple_size_v<origin_shape> - 1;
    constexpr bool is_flat_batch = Std::tuple_size_v<origin_shape> == Std::tuple_size_v<slice_shape_type> &&
        Std::tuple_size_v<origin_shape> >= TWO_DIM_DATA && origin_shape_depth == batch_num + FOUR_DIM_DATA &&
        slice_shape_depth == batch_num + TWO_DIM_DATA;

    if constexpr (is_same_shape) {
        return make_zero_coord_like(layout.shape());
    } else if constexpr (origin_shape_depth == FOUR_DIM_DATA && slice_shape_depth == TWO_DIM_DATA) {
        return make_coord(_0{}, _0{});
    } else if constexpr (is_flat_batch) {
        return make_zero_coord_like(slice_shape);
    } else {
        return make_zero_coord_like(layout.shape());
    }
}

template <typename layout_type, typename SliceShape, typename coord_type,
    Std::enable_if_t<!Std::is_same_v<Std::remove_cvref_t<coord_type>, zero_coord_type>, int> = 0>
__aicore__ inline constexpr const coord_type& resolve_copy_coord(
    const layout_type&, const SliceShape&, const coord_type& coord)
{
    return coord;
}

template <typename coord_param_type, typename layout_type>
__aicore__ inline decltype(auto) make_coord_layout(const coord_param_type& coord, const layout_type& layout)
{
    using shape_type = Std::remove_cvref_t<decltype(layout.shape())>;
    using coord_type = Std::remove_cvref_t<coord_param_type>;
    static_assert(is_layout_v<layout_type> && Std::is_tuple_v<coord_type>, "layout_type must be layout_type");
    static_assert(nesting_depth_v<shape_type> == nesting_depth_v<coord_type> &&
        Std::tuple_size_v<shape_type> == Std::tuple_size_v<coord_type>,
        "shape and coord must have same tuple structure");
    auto coord_shape = transform_tuple_apply(layout.shape(), coord, diff_op{});
    using trait_type = get_layout_trait<layout_type>;
    using pattern_type = get_layout_pattern<layout_type>;
    return make_pattern_layout<pattern_type, trait_type>(coord_shape, layout.stride());
}

template <typename layout_type, typename shape_type>
__aicore__ inline decltype(auto) make_slice_pattern_layout(const layout_type& layout, const shape_type& shape)
{
    using trait_type = get_layout_trait<layout_type>;
    using pattern_type = get_layout_pattern<layout_type>;
    return make_pattern_layout<pattern_type, trait_type>(shape, layout.stride());
}

template <typename coord_type, typename layout_type, typename SliceShape>
__aicore__ inline decltype(auto) make_same_shape_slice_shape(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape)
{
    auto coord_layout = make_coord_layout(coord, layout);
    return transform_tuple_apply(coord_layout.shape(), slice_shape, min_op{});
}

template <typename coord_type, typename layout_type, typename SliceShape>
__aicore__ inline decltype(auto) make_four_dim_slice_shape(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape)
{
    static_assert(nesting_depth_v<SliceShape> == TWO_DIM_DATA,
        "SliceShape must be Two Dim when layout is Four Dim");
    auto inner_row = get<0, 0>(layout.shape());
    auto inner_col = get<1, 0>(layout.shape());

    auto src_row = inner_row * get<0, 1>(layout.shape()) - get<0>(coord);
    auto src_col = inner_col * get<1, 1>(layout.shape()) - get<1>(coord);

    auto real_row = Std::min(src_row, get<0>(slice_shape));
    auto real_col = Std::min(src_col, get<1>(slice_shape));
    return make_fractal_shape(make_shape(real_row, real_col), make_shape(inner_row, inner_col));
}

// slice for the flat multi-batch layouts produced by make_frame_layout(batch0, ..., batch_n, row, col):
//   layout shape (batch0, ..., batch_n-1, ((row0, row1), (col0, col1)))   -- batch_num + 1 elements
//   slice shape  (batch0, ..., batch_n-1, (x, y))                        -- logical row/col for the block
// Every batch axis is clamped elementwise like the same-shape path, and the trailing logical (x, y) is
// refractalized against the layout's inner row/col. The batch_num == 1 case covers the classic
// five-dimensional layout and three-dimensional slice shape.
template <typename coord_type, typename layout_type, typename SliceShape, size_t... batch_is>
__aicore__ inline decltype(auto) make_flat_batch_slice_shape_impl(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape, Std::index_sequence<batch_is...>)
{
    constexpr size_t block_idx = sizeof...(batch_is); // last element: the fractal block / logical (x, y)
    auto inner_row = get<block_idx, 0, 0>(layout.shape());
    auto inner_col = get<block_idx, 1, 0>(layout.shape());

    auto real_row = Std::min(
        inner_row * get<block_idx, 0, 1>(layout.shape()) - get<block_idx, 0>(coord), get<block_idx, 0>(slice_shape));
    auto real_col = Std::min(
        inner_col * get<block_idx, 1, 1>(layout.shape()) - get<block_idx, 1>(coord), get<block_idx, 1>(slice_shape));
    auto fractal_shape = make_fractal_shape(make_shape(real_row, real_col), make_shape(inner_row, inner_col));

    return make_shape(
        Std::min(get<batch_is>(layout.shape()) - get<batch_is>(coord), get<batch_is>(slice_shape))...,
        fractal_shape);
}

template <typename coord_type, typename layout_type, typename SliceShape>
__aicore__ inline decltype(auto) make_flat_batch_slice_shape(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape)
{
    constexpr size_t batch_num = Std::tuple_size_v<Std::remove_cvref_t<decltype(layout.shape())>> - 1;
    return make_flat_batch_slice_shape_impl(
        coord, layout, slice_shape, Std::make_index_sequence<batch_num>{});
}

template <typename coord_type, typename layout_type, typename SliceShape, Std::enable_if_t<!is_layout_v<SliceShape>, int> = 0>
__aicore__ inline decltype(auto) make_slice_shape(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape)
{
    static_assert(is_layout_v<layout_type>, "layout_type must be layout_type");
    static_assert(Std::is_tuple_v<Std::remove_cvref_t<SliceShape>>, "SliceShape must be a tuple");
    using origin_shape = Std::remove_cvref_t<decltype(layout.shape())>;
    using slice_shape_type = Std::remove_cvref_t<SliceShape>;
    constexpr auto origin_shape_depth = nesting_depth_v<origin_shape>;
    constexpr auto slice_shape_depth = nesting_depth_v<slice_shape_type>;
    constexpr bool is_same_shape = origin_shape_depth == slice_shape_depth &&
        Std::tuple_size_v<origin_shape> == Std::tuple_size_v<slice_shape_type>;
    // Flat multi-batch: same arity on both sides, batch axes are scalars and the last element is the
    // fractal block on the layout side vs a logical (x, y) pair on the slice side. Depth is then
    // batch_num + 4 against batch_num + 2. batch_num == 1 covers the classic five-dim/three-dim case.
    constexpr size_t batch_num = Std::tuple_size_v<origin_shape> - 1;
    constexpr bool is_flat_batch = Std::tuple_size_v<origin_shape> == Std::tuple_size_v<slice_shape_type> &&
        Std::tuple_size_v<origin_shape> >= TWO_DIM_DATA &&
        origin_shape_depth == batch_num + FOUR_DIM_DATA && slice_shape_depth == batch_num + TWO_DIM_DATA;

    if constexpr (is_same_shape) {
        return make_same_shape_slice_shape(coord, layout, slice_shape);
    } else if constexpr (origin_shape_depth == FOUR_DIM_DATA && slice_shape_depth == TWO_DIM_DATA) {
        return make_four_dim_slice_shape(coord, layout, slice_shape);
    } else if constexpr (is_flat_batch) {
        return make_flat_batch_slice_shape(coord, layout, slice_shape);
    } else {
        static_assert(is_same_shape || (origin_shape_depth == FOUR_DIM_DATA && slice_shape_depth == TWO_DIM_DATA) ||
            is_flat_batch,
            "SliceShape must be same structure as layout_type shape, or logical Two Dim shape for Four Dim layout_type, "
            "or (batch0, ..., batch_n, logical Two Dim shape) for a flat multi-batch layout_type.");
    }
}

template <typename coord_type, typename layout_type, typename SliceShape, Std::enable_if_t<!is_layout_v<SliceShape>, int> = 0>
__aicore__ inline decltype(auto) make_slice_layout(
    const coord_type& coord, const layout_type& layout, const SliceShape& slice_shape)
{
    return make_slice_pattern_layout(layout, make_slice_shape(coord, layout, slice_shape));
}

template <typename coord_type, typename SrcLayoutType, typename DstLayoutType, Std::enable_if_t<is_layout_v<DstLayoutType>, int> = 0>
__aicore__ inline decltype(auto) make_slice_layout(const coord_type& coord, const SrcLayoutType& src_layout, const DstLayoutType& dst_layout)
{
    static_assert(is_layout_v<SrcLayoutType>, "SrcLayoutType must be layout_type");
    static_assert(SrcLayoutType::rank == DstLayoutType::rank,
        "src_layout_type rank must be equal to dst_layout_type rank");

    auto slice_layout = make_coord_layout(coord, src_layout);
    auto slice_shape = transform_tuple_apply(slice_layout.shape(), dst_layout.shape(), min_op{});
    using trait_type = get_layout_trait<SrcLayoutType>;
    using pattern_type = get_layout_pattern<SrcLayoutType>;
    return make_pattern_layout<pattern_type, trait_type>(slice_shape, src_layout.stride());
}

template <typename layout_type, typename = Std::enable_if_t<is_layout_v<layout_type>>>
__aicore__ inline constexpr decltype(auto) remove_batch_dim(const layout_type& layout)
{
    constexpr auto layout_depth = layout_type::depth;
    static_assert(layout_depth == THREE_DIM_DATA || layout_depth == FIVE_DIM_DATA,
        "remove_batch_dim only supports Three Dim or Five Dim layout_type.");
    using trait_type = get_layout_trait<layout_type>;
    using pattern_type = get_layout_pattern<layout_type>;
    return make_pattern_layout<pattern_type, trait_type>(get<1>(layout.shape()), get<1>(layout.stride()));
}

// ===== squeeze =====

// is_in_pack: true if I is in the pack.
template <size_t I, size_t... Dims>
struct is_in_pack : Std::false_type {};
template <size_t I, size_t Head, size_t... Tail>
struct is_in_pack<I, Head, Tail...> : Std::conditional_t<Head == I, Std::true_type, is_in_pack<I, Tail...>> {};
template <size_t I, size_t... Dims>
constexpr bool is_in_pack_v = is_in_pack<I, Dims...>::value;

// is_squeeze_mark: pattern position marked _1.
template <typename T>
struct is_squeeze_mark : Std::is_same<Std::remove_cvref_t<T>, Std::Int<1>> {};
template <typename T>
constexpr bool is_squeeze_mark_v = is_squeeze_mark<Std::remove_cvref_t<T>>::value;

// squeezable_size: true if axis size is Int<1> or a runtime int (trust caller).
template <typename T, bool IsConst = is_integral_constant_v<Std::remove_cvref_t<T>>>
struct squeezable_size : Std::true_type {};
template <typename T>
struct squeezable_size<T, true> : Std::bool_constant<(Std::remove_cvref_t<T>::value == 1)> {};
template <typename T>
constexpr bool squeezable_size_v = squeezable_size<Std::remove_cvref_t<T>>::value;

// dims_mask: keep mask. Drop axis I iff I in SqueezeDims && squeezable_size.
template <typename shape, typename DimsSeq, typename IdxSeq>
struct dims_mask_impl;
template <typename shape, size_t... Dims, size_t... Is>
struct dims_mask_impl<shape, Std::index_sequence<Dims...>, Std::index_sequence<Is...>> {
    using type = Std::IntegerSequence<
        bool, (!(is_in_pack_v<Is, Dims...> && squeezable_size_v<typename Std::tuple_element<Is, shape>::type>))...>;
};
template <typename shape, size_t... Dims>
using dims_mask = typename dims_mask_impl<
    shape, Std::index_sequence<Dims...>, Std::make_index_sequence<Std::tuple_size_v<shape>>>::type;

// pattern_mask: keep mask. Drop axis I iff pattern[I] is _1 && squeezable_size.
template <typename shape, typename pattern, typename IdxSeq>
struct pattern_mask_impl;
template <typename shape, typename pattern, size_t... Is>
struct pattern_mask_impl<shape, pattern, Std::index_sequence<Is...>> {
    using type = Std::IntegerSequence<
        bool, (!(is_squeeze_mark_v<typename Std::tuple_element<Is, pattern>::type> &&
                 squeezable_size_v<typename Std::tuple_element<Is, shape>::type>))...>;
};
template <typename shape, typename pattern>
using pattern_mask = typename pattern_mask_impl<shape, pattern, Std::make_index_sequence<Std::tuple_size_v<shape>>>::type;

// Promote single nested tuple after squeeze; preserve pattern/trait.
template <typename pattern, typename trait, typename ShapeT, typename StrideT>
__aicore__ inline constexpr auto make_squeezed_layout(const ShapeT& shape, const StrideT& stride)
{
    if constexpr (
        Std::tuple_size_v<ShapeT> == 1 &&
        Std::is_tuple_v<Std::remove_cvref_t<typename Std::tuple_element<0, ShapeT>::type>>) {
        return make_pattern_layout<pattern, trait>(get<0>(shape), get<0>(stride));
    } else {
        return make_pattern_layout<pattern, trait>(shape, stride);
    }
}

// squeeze mode 1: drop selected axes by index (if squeezable). Preserves pattern/trait.
template <
    size_t... SqueezeDims, typename layout_type,
    typename = Std::enable_if_t<is_layout_v<layout_type> && (sizeof...(SqueezeDims) > 0)>>
__aicore__ inline constexpr auto squeeze_layout(const layout_type& layout)
{
    using shape = Std::remove_cvref_t<decltype(layout.shape())>;
    using mask = dims_mask<shape, SqueezeDims...>;
    using keep_seq = typename keep_index_seq_from_seq<mask>::type;
    using pattern = get_layout_pattern<layout_type>;
    using trait = get_layout_trait<layout_type>;
    return make_squeezed_layout<pattern, trait>(
        select_by_seq<keep_seq>(layout.shape()), select_by_seq<keep_seq>(layout.stride()));
}

// ---- Recursive pattern squeeze (mode 2) ----

template <typename S, typename D, typename P>
__aicore__ inline constexpr auto squeeze_rec_tuple_level(const S& s, const D& d, const P& p);

// Recurse into tuple pattern nodes; keep leaves as-is.
template <typename SNode, typename DNode, typename PNode>
__aicore__ inline constexpr auto squeeze_rec_node(const SNode& s, const DNode& d, const PNode& p)
{
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<PNode>>) {
        return squeeze_rec_tuple_level(s, d, p);
    } else {
        return Std::make_tuple(s, d);
    }
}

// Unwrap single-element tuple pair one layer.
template <typename S, typename D>
__aicore__ inline constexpr auto squeeze_unwrap(const S& s, const D& d)
{
    if constexpr (Std::tuple_size_v<Std::remove_cvref_t<S>> == 1) {
        return Std::make_tuple(get<0>(s), get<0>(d));
    } else {
        return Std::make_tuple(s, d);
    }
}

template <typename S, typename D, typename P, size_t... Is>
__aicore__ inline constexpr auto squeeze_rec_tuple_level_impl(
    const S& s, const D& d, const P& p, Std::index_sequence<Is...>)
{
    auto t_shape = Std::make_tuple(get<0>(squeeze_rec_node(get<Is>(s), get<Is>(d), get<Is>(p)))...);
    auto t_stride = Std::make_tuple(get<1>(squeeze_rec_node(get<Is>(s), get<Is>(d), get<Is>(p)))...);
    using mask = pattern_mask<Std::remove_cvref_t<S>, Std::remove_cvref_t<P>>;
    using keep_seq = typename keep_index_seq_from_seq<mask>::type;
    return squeeze_unwrap(select_by_seq<keep_seq>(t_shape), select_by_seq<keep_seq>(t_stride));
}

template <typename S, typename D, typename P>
__aicore__ inline constexpr auto squeeze_rec_tuple_level(const S& s, const D& d, const P& p)
{
    return squeeze_rec_tuple_level_impl(s, d, p, Std::make_index_sequence<Std::tuple_size_v<Std::remove_cvref_t<S>>>{});
}

// squeeze mode 2: recursively drop _1-marked positions (if squeezable). Preserves pattern/trait.
template <
    typename pattern_type, typename layout_type,
    typename = Std::enable_if_t<is_layout_v<layout_type> && Std::is_tuple_v<Std::remove_cvref_t<pattern_type>>>>
__aicore__ inline constexpr auto squeeze_layout(const layout_type& layout, const pattern_type& pattern)
{
    using shape = Std::remove_cvref_t<decltype(layout.shape())>;
    static_assert(is_same_structure_v<pattern_type, shape>, "squeeze pattern must be isomorphic to the layout shape.");
    using layout_pattern_type = get_layout_pattern<layout_type>;
    using layout_trait_type = get_layout_trait<layout_type>;
    auto pr = squeeze_rec_tuple_level(layout.shape(), layout.stride(), pattern);
    auto out_shape = get<0>(pr);
    auto out_stride = get<1>(pr);
    if constexpr (Std::is_tuple_v<Std::remove_cvref_t<decltype(out_shape)>>) {
        return make_pattern_layout<layout_pattern_type, layout_trait_type>(out_shape, out_stride);
    } else {
        return make_pattern_layout<layout_pattern_type, layout_trait_type>(
            Std::make_tuple(out_shape), Std::make_tuple(out_stride));
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
