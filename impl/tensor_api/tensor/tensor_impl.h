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
    "impl/tensor_api/tensor/tensor_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file tensor_impl.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H
#define IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/utils/npu_debug_assert.h"
#include "impl/tensor_api/utils/npu_debug_check.h"
#include "impl/tensor_api/tensor/layout_impl.h"
#include "impl/tensor_api/tensor/engine_impl.h"
#include "impl/tensor_api/tensor/pointer_pattern.h"

namespace asc {
namespace te {

template <typename LocationType, typename TensorEngine, typename TensorLayout>
struct make_tensor_result;

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline base_tensor<TensorEngine, TensorLayout>::base_tensor()
{}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline base_tensor<TensorEngine, TensorLayout>::base_tensor(const TensorEngine& engine,
                                                                       const TensorLayout& layout) : rep(layout, engine)
{}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::tensor() const
{
    return *this;
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::engine() const
{
    return Std::get<1>(rep);
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::engine()
{
    return Std::get<1>(rep);
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::layout() const
{
    return Std::get<0>(rep);
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::data() const
{
    return engine().begin();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::data()
{
    return engine().begin();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::shape() const
{
    return layout().shape();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::stride() const
{
    return layout().stride();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr auto base_tensor<TensorEngine, TensorLayout>::size() const
{
    return layout().size();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr auto base_tensor<TensorEngine, TensorLayout>::capacity() const
{
    return layout().capacity();
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::operator[](const Coord& coord)
{
    TENSOR_API_DEBUG_CHECK(debug_check_coord, layout(), coord, "tensor operator[]");
    return data()[layout()(coord)];
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::operator[](const Coord& coord) const
{
    TENSOR_API_DEBUG_CHECK(debug_check_coord, layout(), coord, "tensor operator[]");
    return data()[layout()(coord)];
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::operator()(const Coord& coord)
{
    TENSOR_API_DEBUG_CHECK(debug_check_coord, layout(), coord, "tensor operator()");
    auto slice_engine = engine() + layout()(coord);
    auto coord_layout = make_coord_layout(coord, layout());
    return make_sub_tensor(slice_engine, coord_layout);
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::operator()(const Coord& coord) const
{
    TENSOR_API_DEBUG_CHECK(debug_check_coord, layout(), coord, "tensor operator()");
    auto slice_engine = engine() + layout()(coord);
    auto coord_layout = make_coord_layout(coord, layout());
    return make_sub_tensor(slice_engine, coord_layout);
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord0, typename Coord1, typename... Coords>
__aicore__ inline constexpr decltype(auto)
base_tensor<TensorEngine, TensorLayout>::operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs)
{
    return operator()(make_coord(c0, c1, cs...));
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord0, typename Coord1, typename... Coords>
__aicore__ inline constexpr decltype(auto)
base_tensor<TensorEngine, TensorLayout>::operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs) const
{
    return operator()(make_coord(c0, c1, cs...));
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::slice(const Coord& coord,
                                                                                          const Info& info)
{
    TENSOR_API_DEBUG_CHECK(debug_check_slice_args, layout(), coord, info);
    auto slice_engine = engine() + layout()(coord);
    auto coord_layout = make_slice_layout(coord, layout(), info);
    return make_sub_tensor(slice_engine, coord_layout);
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::slice(const Coord& coord,
                                                                                          const Info& info) const
{
    TENSOR_API_DEBUG_CHECK(debug_check_slice_args, layout(), coord, info);
    auto slice_engine = engine() + layout()(coord);
    auto coord_layout = make_slice_layout(coord, layout(), info);
    return make_sub_tensor(slice_engine, coord_layout);
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Tensor() const
{
    return tensor();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Engine() const
{
    return engine();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Engine()
{
    return engine();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Layout() const
{
    return layout();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Data() const
{
    return data();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Data()
{
    return data();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Shape() const
{
    return shape();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Stride() const
{
    return stride();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr auto base_tensor<TensorEngine, TensorLayout>::Size() const
{
    return size();
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr auto base_tensor<TensorEngine, TensorLayout>::Capacity() const
{
    return capacity();
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Slice(const Coord& coord,
                                                                                          const Info& info)
{
    return slice(coord, info);
}

template <typename TensorEngine, typename TensorLayout>
template <typename Coord, typename Info>
__aicore__ inline constexpr decltype(auto) base_tensor<TensorEngine, TensorLayout>::Slice(const Coord& coord,
                                                                                          const Info& info) const
{
    return slice(coord, info);
}

template <typename TensorEngine, typename TensorLayout>
template <typename SliceEngine, typename SliceLayout>
__aicore__ inline constexpr decltype(auto)
base_tensor<TensorEngine, TensorLayout>::make_sub_tensor(const SliceEngine& slice_engine,
                                                         const SliceLayout& slice_layout)
{
    using location = get_mem_location<SliceEngine>;
    using result_tensor = typename make_tensor_result<location, SliceEngine, SliceLayout>::type;
    return result_tensor{slice_engine, slice_layout};
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline global_tensor<TensorEngine, TensorLayout>::global_tensor() = default;

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr cache_mode global_tensor<TensorEngine, TensorLayout>::get_cache_mode() const
{
    return static_cast<cache_mode>(this->engine().get_cache_mode());
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr void global_tensor<TensorEngine, TensorLayout>::set_l2_cache_hint(cache_mode mode)
{
    this->engine().set_cache_mode(mode);
}

template <typename TensorEngine, typename TensorLayout>
__aicore__ inline constexpr void global_tensor<TensorEngine, TensorLayout>::SetL2CacheHint(AscendC::Te::CacheMode mode)
{
    set_l2_cache_hint(normalize_cache_mode(mode));
}

template <typename LocationType, typename TensorEngine, typename TensorLayout>
struct make_tensor_result {
    using type =
        typename Std::conditional<Std::is_same_v<LocationType, location::gm>, global_tensor<TensorEngine, TensorLayout>,
                                  local_tensor<TensorEngine, TensorLayout>>::type;
};

template <typename Tensor>
struct is_attr_tensor : Std::false_type {};

template <typename TensorEngine, typename TensorLayout>
struct is_attr_tensor<global_tensor<TensorEngine, TensorLayout>> : Std::true_type {};

template <typename TensorEngine, typename TensorLayout>
struct is_attr_tensor<local_tensor<TensorEngine, TensorLayout>> : Std::true_type {};

template <typename Tensor>
constexpr bool is_attr_tensor_v = is_attr_tensor<Std::remove_cvref_t<Tensor>>::value;

template <typename Iterator>
struct make_tensor_builder {
    template <typename MemoryIterator, typename... Args>
    __aicore__ inline constexpr auto operator()(const MemoryIterator& iterator, const Args&... args) const
    {
        using engine = view_engine<MemoryIterator>;
        if constexpr (sizeof...(Args) == 1 && (is_layout_v<Args> && ...)) {
            using layout = typename Std::tuple_element<0, Std::tuple<Args...>>::type;
            using location = get_mem_location<engine>;
            using result_tensor = typename make_tensor_result<location, engine, layout>::type;
            return result_tensor{engine{iterator}, args...};
        } else if constexpr (sizeof...(Args) == 2 && (Std::is_tuple_v<Std::remove_cvref_t<Args>> && ...)) {
            using layout = decltype(make_layout(args...));
            using location = get_mem_location<engine>;
            using result_tensor = typename make_tensor_result<location, engine, layout>::type;
            return result_tensor{engine{iterator}, make_layout(args...)};
        } else {
            static_assert(
                sizeof...(Args) != sizeof...(Args),
                "make_tensor expects a hardware memory pointer and data structure like Layout or Shape and Stride");
        }
    }
};

template <typename Iterator, typename... Args>
__aicore__ inline constexpr auto make_tensor(const Iterator& iter, const Args&... args)
{
    static_assert(is_hardware_mem_ptr_v<Iterator>,
                  "make_tensor expects the first argument to be a memory pointer or iterator");
    TENSOR_API_DEBUG_CHECK(debug_check_make_tensor_shape, args...);
    return make_tensor_builder<Iterator>{}(iter, args...);
}

// Construct a single-batch sub-tensor from a batched tensor by offsetting the engine pointer
// and stripping the leading batch axis from the layout. Layout depth 5 -> 4 (e.g. NZ fractal)
// or 3 -> 2 (e.g. ND row/col). Other depths trigger a compile-time error.
template <typename Tensor>
__aicore__ inline constexpr auto make_single_batch_sub_tensor(const Tensor& t, uint32_t batch_idx)
{
    using layout_type = typename Tensor::layout_type;
    static_assert(layout_type::depth == three_dim_data || layout_type::depth == five_dim_data,
                  "make_single_batch_sub_tensor only supports batched layouts of depth 3 or 5.");
    auto layout = t.layout();
    auto batch_stride = get<0>(layout.stride());
    auto sub_engine = t.engine() + batch_stride * batch_idx;
    auto sub_layout = remove_batch_dim(layout);
    using new_engine = Std::remove_cvref_t<decltype(sub_engine)>;
    using new_layout = Std::remove_cvref_t<decltype(sub_layout)>;
    using location = get_mem_location<new_engine>;
    using new_tensor = typename make_tensor_result<location, new_engine, new_layout>::type;
    return new_tensor{sub_engine, sub_layout};
}

// squeeze (mode 1): drop axes (by index) whose compile-time size is 1; non-one axes are kept (no error).
// Accepts either a Layout (returns a new Layout) or a Tensor (returns a new Tensor over the same engine).
template <
    size_t... squeeze_dims, typename Tensor,
    typename = Std::enable_if_t<(is_layout_v<Tensor> || is_attr_tensor_v<Tensor>) && (sizeof...(squeeze_dims) > 0)>>
__aicore__ inline constexpr auto squeeze(const Tensor& x)
{
    if constexpr (is_attr_tensor_v<Tensor>) {
        auto new_layout = squeeze_layout<squeeze_dims...>(x.layout());
        using new_layout_type = Std::remove_cvref_t<decltype(new_layout)>;
        using new_engine = typename Tensor::engine_type;
        using location = get_mem_location<new_engine>;
        using new_tensor = typename make_tensor_result<location, new_engine, new_layout_type>::type;
        return new_tensor{x.engine(), new_layout};
    } else {
        static_assert(is_layout_v<Tensor>, "squeeze input must be a Layout or Tensor");
        return squeeze_layout<squeeze_dims...>(x);
    }
}

// squeeze (mode 2): drop positions marked _1 in an isomorphic pattern tuple, when size is 1.
// Accepts either a Layout or a Tensor.
template <typename Pattern, typename Tensor,
          typename = Std::enable_if_t<(is_layout_v<Tensor> || is_attr_tensor_v<Tensor>)
                                      && Std::is_tuple_v<Std::remove_cvref_t<Pattern>>>>
__aicore__ inline constexpr auto squeeze(const Tensor& x, const Pattern& pattern)
{
    if constexpr (is_attr_tensor_v<Tensor>) {
        auto new_layout = squeeze_layout(x.layout(), pattern);
        using new_layout_type = Std::remove_cvref_t<decltype(new_layout)>;
        using new_engine = typename Tensor::engine_type;
        using location = get_mem_location<new_engine>;
        using new_tensor = typename make_tensor_result<location, new_engine, new_layout_type>::type;
        return new_tensor{x.engine(), new_layout};
    } else {
        static_assert(is_layout_v<Tensor>, "squeeze input must be a Layout or Tensor");
        return squeeze_layout(x, pattern);
    }
}

} // namespace te
} // namespace asc

#endif // IMPL_TENSOR_API_TENSOR_TENSOR_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
