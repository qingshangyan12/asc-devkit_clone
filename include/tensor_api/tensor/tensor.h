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
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif

/*!
 * \file tensor.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_TENSOR_H
#define INCLUDE_TENSOR_API_TENSOR_TENSOR_H

namespace asc {
namespace te {

enum class cache_mode : uint8_t { normal = 0, disable = 4, last = 5, persistent = 6 };

} // namespace te
} // namespace asc

#include "tensor_api/tensor/layout_interface.h"
#include "tensor_api/tensor/pointer.h"
#include "impl/tensor_api/tensor/tuple_impl.h"

namespace asc {
namespace te {

template <typename EngineT, typename LayoutT>
struct local_tensor;

template <typename EngineT, typename LayoutT>
struct base_tensor {
    using iterator = typename EngineT::iterator;
    using value_type = typename EngineT::value_type;
    using element_type = typename EngineT::element_type;
    using elementType = element_type;
    using reference = typename EngineT::reference;
    using engine_type = EngineT;
    using layout_type = LayoutT;

    static constexpr int rank = LayoutT::rank_size;

    __aicore__ inline base_tensor();
    __aicore__ inline base_tensor(const EngineT& engine, const LayoutT& layout);

    __aicore__ inline constexpr decltype(auto) tensor() const;
    __aicore__ inline constexpr decltype(auto) engine() const;
    __aicore__ inline constexpr decltype(auto) engine();
    __aicore__ inline constexpr decltype(auto) layout() const;
    __aicore__ inline constexpr decltype(auto) data() const;
    __aicore__ inline constexpr decltype(auto) data();
    __aicore__ inline constexpr decltype(auto) shape() const;
    __aicore__ inline constexpr decltype(auto) stride() const;
    __aicore__ inline constexpr auto size() const;
    __aicore__ inline constexpr auto capacity() const;

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord);

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator[](const Coord& coord) const;

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord);

    template <typename Coord>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord& coord) const;

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(const Coord0& c0, const Coord1& c1, const Coords&... cs);

    template <typename Coord0, typename Coord1, typename... Coords>
    __aicore__ inline constexpr decltype(auto) operator()(
        const Coord0& c0, const Coord1& c1, const Coords&... cs) const;

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info);

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) slice(const Coord& coord, const Info& info) const;

/**
 * @ingroup simd_api.tensor_api.deprecated_interface
 * @override_id Tensor__z
 * @archs 3510
 * @brief **该接口已废弃，请使用[base_tensor::tensor](../tensor/tensor.md#tensor)替代。
 */
    __aicore__ inline constexpr decltype(auto) Tensor() const;
    __aicore__ inline constexpr decltype(auto) Engine() const;
    __aicore__ inline constexpr decltype(auto) Engine();
/**
 * @ingroup simd_api.tensor_api.deprecated_interface
 * @override_id Layout__z
 * @archs 3510
 * @brief **该接口已废弃，请使用[base_tensor::layout](../tensor/tensor.md#layout)替代。
 */
    __aicore__ inline constexpr decltype(auto) Layout() const;
    __aicore__ inline constexpr decltype(auto) Data() const;
    __aicore__ inline constexpr decltype(auto) Data();
    __aicore__ inline constexpr decltype(auto) Shape() const;
    __aicore__ inline constexpr decltype(auto) Stride() const;
    __aicore__ inline constexpr auto Size() const;
    __aicore__ inline constexpr auto Capacity() const;

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info);

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const;

private:
    template <typename SliceEngine, typename SliceLayout>
    __aicore__ inline static constexpr decltype(auto) make_sub_tensor(
        const SliceEngine& slice_engine, const SliceLayout& slice_layout);

    Std::tuple<layout_type, engine_type> rep;
};

template <typename EngineT, typename LayoutT>
struct global_tensor : public base_tensor<EngineT, LayoutT> {
    using tensor_api_base = base_tensor<EngineT, LayoutT>;
    using layoutType = typename tensor_api_base::layout_type;
    using tensor_api_base::tensor_api_base;

    __aicore__ inline global_tensor();
/**
 * @ingroup simd_api.tensor_api.tensor
 * @override_id get_cache_mode__z
 * @archs 3510
 * @brief view_engine是Tensor API中的视图引擎，负责持有一个指针对象Iterator，通过`begin()`返回这段视图的起始位置。在`local_tensor`中，view_engine和layout一起组成一个Tensor的视图。
 */
    __aicore__ inline constexpr cache_mode get_cache_mode() const;
    __aicore__ inline constexpr void set_l2_cache_hint(cache_mode mode);
/**
 * @ingroup simd_api.tensor_api.deprecated_interface
 * @override_id SetL2CacheHint__cachemode
 * @archs 3510
 * @brief **该接口已废弃，请使用[global_tensor::set_l2_cache_hint](../tensor/tensor.md#set_l2_cache_hint)替代。
 * @param [in] mode L2 Cache模式。旧接口使用`AscendC::Te::CacheMode`，替代接口使用`cache_mode`。
 */
    __aicore__ inline constexpr void SetL2CacheHint(AscendC::Te::CacheMode mode);
};

} // namespace te
} // namespace asc

#include "impl/tensor_api/tensor/tensor_impl.h"
#include "tensor_api/tensor/local_tensor.h"

// tensor construction
namespace asc {
namespace te {

template <typename Iterator, typename... Args>
/**
 * @ingroup simd_api.tensor_api.tensor
 * @override_id make_tensor__RCpIterator_QRCpArgs
 * @archs 3510
 * @brief make_tensor负责将内存指针（由[make_mem_ptr](make_mem_ptr.md)生成）与布局对象（由[make_frame_layout](../layout/make_frame_layout.md)/[make_layout](../layout/make_layout.md)生成）绑定，生成带有完整类型信息的Tensor对象。
 * @param [in] iter 迭代器，要求传入view_engine对象，用于创建Tensor。返回类型由该迭代器绑定的内存位置决定。
 */
__aicore__ inline constexpr auto make_tensor(const Iterator& iter, const Args&... args);

template <size_t... SqueezeDims, typename Input, typename Enable>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id squeeze__RCpInput
 * @archs 3510
 * @brief squeeze用于删除layout的shape中大小为1的指定维度，并同步删除stride中的对应维度。输入为Tensor时，接口更新Tensor的layout，Tensor的Engine、存储位置和数据起始地址保持不变。
 * @param [in] x 待处理的layout或Tensor对象。
 */
__aicore__ inline constexpr auto squeeze(const Input& x);

template <typename Pattern, typename Input, typename Enable>
/**
 * @ingroup simd_api.tensor_api.layout
 * @override_id squeeze__RCpInput_RCpPattern
 * @archs 3510
 * @brief squeeze用于删除layout的shape中大小为1的指定维度，并同步删除stride中的对应维度。输入为Tensor时，接口更新Tensor的layout，Tensor的Engine、存储位置和数据起始地址保持不变。
 * @param [in] x 待处理的layout或Tensor对象。
 * @param [in] pattern 维度选择Pattern。`_1`表示删除对应的大小为1的维度，`_`表示保留对应维度。Pattern可由`make_coord`构造。
 */
__aicore__ inline constexpr auto squeeze(const Input& x, const Pattern& pattern);

} // namespace te
} // namespace asc

#endif // INCLUDE_TENSOR_API_TENSOR_TENSOR_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_API_H
#endif
