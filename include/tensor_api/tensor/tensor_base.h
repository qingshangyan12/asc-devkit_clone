/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "tensor_base.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS)
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_BASE_H
#endif

/*!
 * \file tensor_base.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_TENSOR_TENSOR_BASE_H
#define INCLUDE_TENSOR_API_TENSOR_TENSOR_BASE_H

#include "utils/base/sys_macros.h"
#include "utils/std/tuple.h"

namespace AscendC {
namespace Te {

template <typename EngineType, typename LayoutType>
struct BaseTensor {
    using iterator = typename EngineType::iterator;
    using valueType = typename EngineType::valueType;
    using elementType = typename EngineType::elementType;
    using reference = typename EngineType::reference;

    using engineType = EngineType;
    using layoutType = LayoutType;

    __aicore__ inline BaseTensor();
    __aicore__ inline BaseTensor(const EngineType& engine, const LayoutType& layout);

    static constexpr int rank = LayoutType::rank;

    __aicore__ inline constexpr decltype(auto) Tensor() const;

    __aicore__ inline constexpr decltype(auto) Engine() const;

    __aicore__ inline constexpr decltype(auto) Engine();

    __aicore__ inline constexpr decltype(auto) Layout() const;

    __aicore__ inline constexpr decltype(auto) Data() const;

    __aicore__ inline constexpr decltype(auto) Data();

    __aicore__ inline constexpr decltype(auto) Shape() const;

    __aicore__ inline constexpr decltype(auto) Stride() const;

    __aicore__ inline constexpr auto Size() const;

    __aicore__ inline constexpr auto Capacity() const;

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
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info);

    template <typename Coord, typename Info>
    __aicore__ inline constexpr decltype(auto) Slice(const Coord& coord, const Info& info) const;

private:
    template <typename SliceEngine, typename SliceLayout>
    __aicore__ inline static constexpr decltype(auto) MakeSubTensor(
        const SliceEngine& sliceEngine, const SliceLayout& sliceLayout);

    Std::tuple<layoutType, engineType> rep;
};

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_TENSOR_TENSOR_BASE_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_BASE_H)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC_TENSOR_BASE_H
#endif
