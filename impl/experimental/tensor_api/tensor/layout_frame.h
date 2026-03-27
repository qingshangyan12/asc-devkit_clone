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
* \file layout_fractal.h
* \brief
*/
#ifndef IMPL_TENSOR_API_TENSOR_LAYOUT_FRAME_H
#define IMPL_TENSOR_API_TENSOR_LAYOUT_FRAME_H

#include "impl/experimental/tensor_api/utils/utils_impl.h"
#include "impl/experimental/tensor_api/tensor/layout_dispatch.h"
#include "impl/experimental/tensor_api/tensor/layout_definition.h"

namespace AscendC {
namespace Te {

struct MAkeNzFrameLayout {
    template <typename TraitType>
    __aicore__ inline static auto Make(size_t row, size_t column) {
        static constexpr int32_t c0Ele = TraitType::C0_size;
        auto shape = GetTuple(t0, t1, t2, t3);
        auto stride = GetTuple(ts...);
        using LayoutT = Layout<decltype(shape), decltype(stride), Pattern>;
        return LayoutT(shape, stride);
    }
};

template <typename T = half, int32_t C0 =32>
struct LayoutTrait {
    using type = T;
    static constexpr int32_t C0_size = C0;;
};
using LayoutFormatSet = Std::tuple<Std::tuple<NzLayoutPattern, MakeNzFrameLayout>>;

template <typename LayoutPattern, typename Tuple = LayoutFormatSet>
struct FindLayoutFromPattern;

template <typename Pattern, typename FrameLayout, typename... Rest>
struct FindLayoutFromPattern<Pattern, Std::tuple<Std::tuple<Pattern, FrameLayout>, Rest...>> {
    using type = FrameLayout;
};

template <typename Pattern, typename NotMatchTuple, typename... Rest>
struct FindLayoutFromPattern<Pattern, Std::tuple<NotMatchTuple, Rest...>> {};

template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args) {
    using GetLayoutMakeFun = typename FindLayoutFromPattern<LayoutPattern>::type;
    return GetLayoutMakeFun::Make<TraitType>(args...);
}

}
}