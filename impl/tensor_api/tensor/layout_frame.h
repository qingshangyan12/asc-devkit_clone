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
    "impl/tensor_api/tensor/layout_frame.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file layout_frame.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_FRAME_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_FRAME_H

#include "impl/tensor_api/tensor/layout_pattern.h"

namespace asc {
namespace te {

using LayoutFormatSet =
    TupleMap<Std::tuple<NZLayoutPtn, MakeNzFrameLayout>, Std::tuple<NDLayoutPtn, MakeNDFrameLayout>,
             Std::tuple<DNLayoutPtn, MakeDNFrameLayout>, Std::tuple<NDExtLayoutPtn, MakeNDExtFrameLayout>,
             Std::tuple<DNExtLayoutPtn, MakeDNExtFrameLayout>, Std::tuple<NNLayoutPtn, MakeNnFrameLayout>,
             Std::tuple<ZZLayoutPtn, MakeZzFrameLayout>, Std::tuple<ZNLayoutPtn, MakeZnFrameLayout>,
             Std::tuple<ScaleANDLayoutPtn, MakeScaleANDFrameLayout>,
             Std::tuple<ScaleADNLayoutPtn, MakeScaleADNFrameLayout>,
             Std::tuple<ScaleBNDLayoutPtn, MakeScaleBNDFrameLayout>,
             Std::tuple<ScaleBDNLayoutPtn, MakeScaleBDNFrameLayout>, Std::tuple<NCHWLayoutPtn, MakeNCHWFrameLayout>,
             Std::tuple<NHWCLayoutPtn, MakeNHWCFrameLayout>, Std::tuple<NC1HWC0LayoutPtn, MakeNC1HWC0FrameLayout>,
             Std::tuple<NCDHWLayoutPtn, MakeNCDHWFrameLayout>, Std::tuple<NDC1HWC0LayoutPtn, MakeNDC1HWC0FrameLayout>>;

template <typename T, typename C0>
struct layout_trait {
    static constexpr auto C0_ELEMENT = C0{};
};

template <typename T = uint16_t, typename C0 = Std::Int<C0_ELEMENT<T>>>
struct layout_trait_default : layout_trait<T, C0> {};

struct layout_trait_scale : layout_trait_default<fp8_e8m0_t, Std::Int<MX_SCALE_K0 / sizeof(fp8_e8m0_t)>> {};

struct layout_trait_fp4 : layout_trait_default<fp4x2_e2m1_t, Std::Int<C0_ELEMENT<fp4x2_e2m1_t>>> {};

template <typename T, typename C0>
using LayoutTrait = layout_trait<T, C0>;

template <typename T = uint16_t, typename C0 = Std::Int<C0_ELEMENT<T>>>
using LayoutTraitDefault = layout_trait_default<T, C0>;

using LayoutTraitScale = layout_trait_scale;
using LayoutTraitFP4 = layout_trait_fp4;

using FormatTraitSet = TupleMap<
    Std::tuple<NZLayoutPtn, LayoutTraitDefault<>>, Std::tuple<NDLayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<DNLayoutPtn, LayoutTrait<Std::ignore_t, _1>>, Std::tuple<NDExtLayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<DNExtLayoutPtn, LayoutTrait<Std::ignore_t, _1>>, Std::tuple<NNLayoutPtn, LayoutTrait<Std::ignore_t, _2>>,
    Std::tuple<ZZLayoutPtn, LayoutTraitDefault<>>, Std::tuple<ZNLayoutPtn, LayoutTraitDefault<>>,
    Std::tuple<ScaleANDLayoutPtn, LayoutTrait<Std::ignore_t, _2>>,
    Std::tuple<ScaleADNLayoutPtn, LayoutTrait<Std::ignore_t, _2>>,
    Std::tuple<ScaleBNDLayoutPtn, LayoutTrait<Std::ignore_t, _2>>,
    Std::tuple<ScaleBDNLayoutPtn, LayoutTrait<Std::ignore_t, _2>>,
    Std::tuple<NCHWLayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<NHWCLayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<NC1HWC0LayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<NCDHWLayoutPtn, LayoutTrait<Std::ignore_t, _1>>,
    Std::tuple<NDC1HWC0LayoutPtn, LayoutTrait<Std::ignore_t, _1>>>;

template <typename T, typename = void>
struct is_frame_layout_trait : Std::false_type {};

template <typename T>
struct is_frame_layout_trait<T, void_t<decltype(T::C0_ELEMENT)>> : Std::true_type {};

template <typename T, typename Enable = void>
using IsFrameLayoutTrait = is_frame_layout_trait<T, Enable>;

template <typename T>
constexpr bool is_frame_layout_trait_v = is_frame_layout_trait<T>::value;

template <typename T>
constexpr bool IsFrameLayoutTraitV = is_frame_layout_trait_v<T>;

template <typename LayoutPattern, typename TraitType>
struct trait_conversion {
private:
    using RawTrait = Std::remove_cvref_t<TraitType>;

    static_assert(Std::is_same_v<RawTrait, Std::ignore_t> || is_integral_constant_v<RawTrait>
                      || is_frame_layout_trait_v<RawTrait> || is_data_type<RawTrait>,
                  "TraitType must be ignore_t, integral constant, frame layout trait, or data type.");

    using FromPattern = typename FormatTraitSet::template Get<LayoutPattern>;
    using FromInt = LayoutTrait<Std::ignore_t, RawTrait>;
    using FromDataType = LayoutTraitDefault<RawTrait>;
    using FromTrait = RawTrait;

    using Converted = typename Std::conditional<
        Std::is_same_v<RawTrait, Std::ignore_t>, FromPattern,
        typename Std::conditional<
            is_integral_constant_v<RawTrait>, FromInt,
            typename Std::conditional<is_frame_layout_trait_v<RawTrait>, FromTrait, FromDataType>::type>::type>::type;

public:
    using type = Converted;
};

template <typename LayoutPattern, typename TraitType>
using TraitConversion = trait_conversion<LayoutPattern, TraitType>;

// ===== Multi-batch frame layout: make_frame_layout<Ptn>(batch0, ..., batch_n, row, col) =====

// SupportsRowColMake: true when the pattern's Make accepts a bare (row, column) pair, i.e. it is one
// of the matrix layouts that also offer the (batch, row, column) form. The conv feature-map patterns
// (NCHW/NHWC/NC1HWC0/NCDHW/NDC1HWC0) take fixed positional dimensions and have no 2-arg Make, so they
// never take the multi-batch path below.
template <typename Maker, typename Trait, typename = void>
struct supports_row_col_make : Std::false_type {};

template <typename Maker, typename Trait>
struct supports_row_col_make<Maker, Trait,
                             void_t<decltype(Maker::template make<Trait>(Std::declval<int>(), Std::declval<int>()))>>
    : Std::true_type {};

template <typename Maker, typename Trait, typename Enable = void>
using SupportsRowColMake = supports_row_col_make<Maker, Trait, Enable>;

template <typename Maker, typename Trait>
constexpr bool supports_row_col_make_v = supports_row_col_make<Maker, Trait>::value;

template <typename Maker, typename Trait>
constexpr bool SupportsRowColMakeV = supports_row_col_make_v<Maker, Trait>;

// BatchStrideAt: stride of batch axis I, given the batch extents tuple and the base layout capacity.
// Batch axes are flat and row-major over the base block, so the stride of an axis is the product of
// all batch extents to its right times the base capacity:
//   stride[n-1] = capacity, stride[n-2] = batch[n-1]*capacity, ...
template <size_t I, size_t BatchNum, typename BatchTuple, typename Capacity>
__aicore__ inline constexpr auto batch_stride_at(const BatchTuple& batches, const Capacity& capacity)
{
    if constexpr (I + 1 >= BatchNum) {
        return capacity;
    } else {
        return get<I + 1>(batches) * batch_stride_at<I + 1, BatchNum, BatchTuple, Capacity>(batches, capacity);
    }
}

// Builds the flat multi-batch layout: all batch axes sit side by side in the outermost tuple and the
// base (row, col) block is appended as the last element, e.g.
//   (batch0, batch1, ((row0, row1), (col0, col1)))
// so rank is batch_num + 1 (not a chain of nested single-batch layouts).
template <typename LayoutPattern, typename Trait, typename BatchTuple, typename BaseLayout, size_t... batch_is>
__aicore__ inline constexpr auto make_flat_batch_layout(const BatchTuple& batches, const BaseLayout& base,
                                                        Std::index_sequence<batch_is...>)
{
    constexpr size_t batch_num = sizeof...(batch_is);
    auto capacity = base.capacity();
    return make_pattern_layout<LayoutPattern, Trait>(
        make_shape(get<batch_is>(batches)..., base.shape()),
        make_stride(batch_stride_at<batch_is, batch_num, BatchTuple, decltype(capacity)>(batches, capacity)...,
                    base.stride()));
}

// Peels the trailing (row, column) off the argument pack, builds the 2D base layout from them, then
// lays the leading arguments out as flat batch axes.
template <typename LayoutPattern, typename Trait, typename Maker, typename ArgsTuple, size_t... batch_is>
__aicore__ inline constexpr auto make_multi_batch_frame_layout_impl(const ArgsTuple& args,
                                                                    Std::index_sequence<batch_is...>)
{
    constexpr size_t arg_num = Std::tuple_size_v<ArgsTuple>;
    auto base = Maker::template make<Trait>(get<arg_num - 2>(args), get<arg_num - 1>(args));
    auto batches = Std::make_tuple(get<batch_is>(args)...);
    return make_flat_batch_layout<LayoutPattern, Trait>(batches, base, Std::index_sequence<batch_is...>{});
}

template <typename LayoutPattern, typename Trait, typename Maker, typename... Args>
__aicore__ inline constexpr auto make_multi_batch_frame_layout(const Args&... args)
{
    return make_multi_batch_frame_layout_impl<LayoutPattern, Trait, Maker>(
        Std::make_tuple(args...), Std::make_index_sequence<sizeof...(Args) - 2>{});
}

template <typename LayoutPattern, typename TraitType = Std::ignore_t, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args)
{
    using Trait = typename TraitConversion<LayoutPattern, TraitType>::type;
    using LayoutMaker = typename LayoutFormatSet::template Get<LayoutPattern>;
    static_assert(!Std::is_same_v<LayoutMaker, Std::ignore_t>, "Unsupported layout pattern.");
    // 2 args = (row, col) and 3 args = (batch, row, col) are handled by the pattern's own Make. Four
    // or more args on a row/col-style pattern means multiple batch axes: (batch0, ..., batch_n, row, col).
    if constexpr (sizeof...(Args) > 3 && supports_row_col_make_v<LayoutMaker, Trait>) {
        return make_multi_batch_frame_layout<LayoutPattern, Trait, LayoutMaker>(args...);
    } else {
        return LayoutMaker::template make<Trait>(args...);
    }
}

template <typename LayoutPattern, size_t C0Element, typename... Args>
__aicore__ inline constexpr decltype(auto) make_frame_layout(const Args&... args)
{
    return make_frame_layout<LayoutPattern, Std::Int<C0Element>>(args...);
}

template <typename LayoutPattern, typename TraitType = LayoutTraitDefault<>>
struct frame_layout_format {
    template <typename... Args>
    __aicore__ inline constexpr decltype(auto) operator()(const Args&... args)
    {
        return make_frame_layout<LayoutPattern, TraitType>(args...);
    }
};

template <typename LayoutPattern, typename TraitType = LayoutTraitDefault<>>
using FrameLayoutFormat = frame_layout_format<LayoutPattern, TraitType>;
} // namespace te
} // namespace asc

// PascalCase compatibility wrappers
namespace asc {
namespace te {
template <size_t I, size_t BatchNum, typename BatchTuple, typename Capacity>
__aicore__ inline constexpr auto BatchStrideAt(const BatchTuple& batches, const Capacity& capacity)
{
    return batch_stride_at<I, BatchNum>(batches, capacity);
}

template <typename LayoutPattern, typename Trait, typename BatchTuple, typename BaseLayout, size_t... batch_is>
__aicore__ inline constexpr auto MakeFlatBatchLayout(const BatchTuple& batches, const BaseLayout& base,
                                                     Std::index_sequence<batch_is...> arg2)
{
    return make_flat_batch_layout<LayoutPattern, Trait, batch_is...>(batches, base, arg2);
}

template <typename LayoutPattern, typename Trait, typename Maker, typename ArgsTuple, size_t... batch_is>
__aicore__ inline constexpr auto MakeMultiBatchFrameLayoutImpl(const ArgsTuple& args,
                                                               Std::index_sequence<batch_is...> arg1)
{
    return make_multi_batch_frame_layout_impl<LayoutPattern, Trait, Maker, batch_is...>(args, arg1);
}

template <typename LayoutPattern, typename Trait, typename Maker, typename... Args>
__aicore__ inline constexpr auto MakeMultiBatchFrameLayout(const Args&... args)
{
    return make_multi_batch_frame_layout<LayoutPattern, Trait, Maker>(args...);
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
} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_FRAME_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
