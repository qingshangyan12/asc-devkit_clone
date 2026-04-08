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

#include "impl/experimental/tensor_api/tensor/frame_layout_pattern.h"

namespace AscendC {
namespace Te {

template <typename T = uint16_t, size_t C0 = 32>
struct LayoutTrait {
    using type = T;
    static constexpr auto C0_SIZE = Std::Int<C0>{};
    static constexpr auto C0_ELEMENT = C0_SIZE / Std::Int<sizeof(T)>{};
};

using LayoutFormatSet = TupleMap<
    Std::tuple<NzLayoutPattern, MakeNzFrameLayout>,
    Std::tuple<L0CLayoutPattern, MakeL0CFrameLayout>,
    Std::tuple<NDLayoutPattern, MakeNDFrameLayout>,
    Std::tuple<DNLayoutPattern, MakeDNFrameLayout>,
    Std::tuple<NnLayoutPattern, MakeNnFrameLayout>,
    Std::tuple<ZzLayoutPattern, MakeZzFrameLayout>,
    Std::tuple<ZnLayoutPattern, MakeZnFrameLayout>,
    Std::tuple<ScaleANDLayoutPattern, MakeScaleANDFrameLayout>,
    Std::tuple<ScaleADNLayoutPattern, MakeScaleADNFrameLayout>,
    Std::tuple<ScaleBNDLayoutPattern, MakeScaleBNDFrameLayout>,
    Std::tuple<ScaleBDNLayoutPattern, MakeScaleBDNFrameLayout>>;

template <typename LayoutPattern, typename TraitType, typename... Args>
__aicore__ inline decltype(auto) MakeFrameLayout(const Args&... args) {
    using GetLayoutMakeFun = typename LayoutFormatSet::template Get<LayoutPattern>;
    static_assert(!Std::is_same_v<GetLayoutMakeFun, EmptyValue>, "Unsupported layout pattern.");
    return GetLayoutMakeFun::template Make<TraitType>(args...);
}

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_TENSOR_FRAME_LAYOUT_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
