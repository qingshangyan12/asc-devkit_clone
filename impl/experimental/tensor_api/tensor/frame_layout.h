/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
* \file frame_layout.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_FRAME_LAYOUT_H
#define IMPL_TENSOR_API_TENSOR_FRAME_LAYOUT_H

#include "impl/experimental/tensor_api/tensor/layout_method.h"

namespace AscendC {
namespace Te {

struct MakeNzFrameLayout {
    template <typename TraitType, typename T, typename U>
    __aicore__ inline static auto Make(T row, U column) {
        using S = typename TraitType::type;
        constexpr auto c0Ele = TraitType::C0_element;
        auto shape = MakeShape(MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               MakeShape(Std::Int<c0Ele>{}, Std::ceil_division(column, c0Ele)));
        auto stride = MakeStride(MakeStride(Std::Int<c0Ele>{}, Std::Int<c0Ele * FRACTAL_FIXED>{}),
                                 MakeStride(Std::Int<1>{}, c0Ele * Std::ceil_align(row, FRACTAL_FIXED)));
        using LayoutT = Layout<decltype(shape), decltype(stride), NzLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

struct MakeNDFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        using T = typename TraitType::type;
        auto shape = MakeShape(row, column);
        auto stride = MakeStride(column, Std::Int<1>{});
        using LayoutT = Layout<decltype(shape), decltype(stride), NDLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

struct MakeZnFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        using T = typename TraitType::type;
        constexpr auto c0Ele = TraitType::C0_element;
        auto shape = MakeShape(MakeShape(Std::Int<c0Ele>{},  Std::ceil_division(row, c0Ele)),
                               MakeShape(Std::Int<FRACTAL_FIXED>{},  Std::ceil_division(column, FRACTAL_FIXED)));
        auto stride = MakeStride(MakeStride(Std::Int<1>{},  c0Ele * Std::ceil_align(column, FRACTAL_FIXED)),
                                 MakeStride(Std::Int<c0Ele>{},  Std::Int<c0Ele * FRACTAL_FIXED>{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), ZnLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

struct MakeDNFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        using T = typename TraitType::type;
        constexpr auto c0Ele = TraitType::C0_element;
        auto shape = MakeShape(row, column);
        auto stride = MakeStride(Std::Int<1>{}, row);
        using LayoutT = Layout<decltype(shape), decltype(stride), DNLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

struct MakeZzFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        using T = typename TraitType::type;
        constexpr auto c0Ele = TraitType::C0_element;
        auto shape = MakeShape(MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               MakeShape(Std::Int<c0Ele>{}, Std::ceil_division(column, c0Ele)));
        auto stride = MakeStride(MakeStride(Std::Int<c0Ele>{}, FRACTAL_FIXED * Std::ceil_align(column, c0Ele)),
                                 MakeStride(Std::Int<1>{}, Std::Int<c0Ele * FRACTAL_FIXED>{}));
        using LayoutT = Layout<decltype(shape), decltype(stride), ZzLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

struct MakeNnFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        using T = typename TraitType::type;
        constexpr auto c0Ele = TraitType::C0_element;
        auto shape = MakeShape(MakeShape(Std::Int<FRACTAL_FIXED>{}, Std::ceil_division(row, FRACTAL_FIXED)),
                               MakeShape(Std::Int<c0Ele>{}, Std::ceil_division(column, c0Ele)));
        auto stride = MakeStride(MakeStride(Std::Int<c0Ele>{}, Std::Int<c0Ele * FRACTAL_FIXED>{}),
                                 MakeStride(Std::Int<1>{}, c0Ele * Std::ceil_align(row, FRACTAL_FIXED)));
        using LayoutT = Layout<decltype(shape), decltype(stride), NnLayoutPattern>;
        return LayoutT(shape, stride);
    }
};

using LayoutFormatSet = TupleMap<
    Std::tuple<NzLayoutPattern, MakeNzFrameLayout>,
    Std::tuple<NDLayoutPattern, MakeNDFrameLayout>,
    Std::tuple<DNLayoutPattern, MakeDNFrameLayout>,
    Std::tuple<NnLayoutPattern, MakeNnFrameLayout>,
    Std::tuple<ZzLayoutPattern, MakeZzFrameLayout>,
    Std::tuple<ZnLayoutPattern, MakeZnFrameLayout>>;

template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args) {
    using GetLayoutMakeFun = typename LayoutFormatSet::template Get<LayoutPattern>;
    static_assert(!Std::is_same_v<GetLayoutMakeFun, EmptyValue>, "Unsupported layout pattern.");
    return GetLayoutMakeFun::template Make<TraitType>(args...);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_LAYOUT_FRACTAL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
