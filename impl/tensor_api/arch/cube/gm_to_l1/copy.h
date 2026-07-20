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
#warning \
    "impl/tensor_api/arch/cube/gm_to_l1/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H

#include "impl/tensor_api/arch/utils/arch_utils.h"
#include "impl/tensor_api/arch/cube/gm_to_l1/routing.h"
#include "kernel_npu_debug.h"

namespace AscendC {
namespace Te {

constexpr CopyGM2L1Trait DEFAULT_COPY_GM_TO_L1_TRAIT;

struct CopyGM2L1TraitDefault {
    using TraitType = CopyGM2L1Trait;
    static constexpr const TraitType value = DEFAULT_COPY_GM_TO_L1_TRAIT;
};

struct CopyGM2L1 {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            DataCopyImpl<traits, Args...>(args...);
        }
    }

private:
    template <const CopyGM2L1Trait& trait = DEFAULT_COPY_GM_TO_L1_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        using DstPos = GetMemLocation<T>;
        using SrcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<DstPos, Location::L1>, "CopyGM2L1 requires destination on L1");
        static_assert(Std::is_same_v<SrcPos, Location::GM>, "CopyGM2L1 requires source on GM");
        using DstLayout = typename T::layoutType;
        using SrcLayout = typename U::layoutType;
        using DstLayoutPtn = GetLayoutPattern<DstLayout>;
        using SrcLayoutPtn = GetLayoutPattern<SrcLayout>;
        if constexpr (DstLayout::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Shape()) > 0 && Get<1>(dst.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyGM2L1, dst row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Stride()) > 0 && Get<1>(dst.Layout().Stride()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyGM2L1, all dst strides should be greater than 0."));
        } else if constexpr (DstLayout::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Shape()) > 0 && Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyGM2L1, dst batch, row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Stride()) > 0 && Get<1, 0>(dst.Layout().Stride()) > 0 &&
                Get<1, 1>(dst.Layout().Stride()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyGM2L1, all dst strides should be greater than 0."));
        } else if constexpr (DstLayout::depth == FOUR_DIM_DATA) {
            using DstStride00 = Std::remove_cvref_t<decltype(Get<0, 0>(dst.Layout().Stride()))>;
            using DstStride01 = Std::remove_cvref_t<decltype(Get<0, 1>(dst.Layout().Stride()))>;
            using DstStride10 = Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>;
            using DstStride11 = Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>;
            ASCENDC_DEBUG_ASSERT((Get<0, 0>(dst.Layout().Shape()) > 0 &&
                Get<0, 1>(dst.Layout().Shape()) > 0 && Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyGM2L1, all dst row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT(((Std::is_same_v<DstStride00, _0> || Get<0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride01, _0> || Get<0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride10, _0> || Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride11, _0> || Get<1, 1>(dst.Layout().Stride()) > 0)),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyGM2L1, all runtime dst strides should be greater than 0."));
        } else if constexpr (DstLayout::depth == FIVE_DIM_DATA) {
            using DstStride100 = Std::remove_cvref_t<decltype(Get<1, 0, 0>(dst.Layout().Stride()))>;
            using DstStride101 = Std::remove_cvref_t<decltype(Get<1, 0, 1>(dst.Layout().Stride()))>;
            using DstStride110 = Std::remove_cvref_t<decltype(Get<1, 1, 0>(dst.Layout().Stride()))>;
            using DstStride111 = Std::remove_cvref_t<decltype(Get<1, 1, 1>(dst.Layout().Stride()))>;
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(dst.Layout().Shape()) > 0 && Get<1, 0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(dst.Layout().Shape()) > 0 && Get<1, 1, 1>(dst.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyGM2L1, dst batch and all dst row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Stride()) > 0 &&
                (Std::is_same_v<DstStride100, _0> || Get<1, 0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride101, _0> || Get<1, 0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride110, _0> || Get<1, 1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<DstStride111, _0> || Get<1, 1, 1>(dst.Layout().Stride()) > 0)),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyGM2L1, all runtime dst strides should be greater than 0."));
        }
        if constexpr (SrcLayout::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Shape()) > 0 && Get<1>(src.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyGM2L1, source row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Stride()) > 0 && Get<1>(src.Layout().Stride()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyGM2L1, all source strides should be greater than 0."));
        } else if constexpr (SrcLayout::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Shape()) > 0 && Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyGM2L1, source batch, row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Stride()) > 0 && Get<1, 0>(src.Layout().Stride()) > 0 &&
                Get<1, 1>(src.Layout().Stride()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyGM2L1, all source strides should be greater than 0."));
        } else if constexpr (SrcLayout::depth == FOUR_DIM_DATA) {
            using SrcStride00 = Std::remove_cvref_t<decltype(Get<0, 0>(src.Layout().Stride()))>;
            using SrcStride01 = Std::remove_cvref_t<decltype(Get<0, 1>(src.Layout().Stride()))>;
            using SrcStride10 = Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>;
            using SrcStride11 = Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>;
            ASCENDC_DEBUG_ASSERT((Get<0, 0>(src.Layout().Shape()) > 0 &&
                Get<0, 1>(src.Layout().Shape()) > 0 && Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyGM2L1, all source row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT(((Std::is_same_v<SrcStride00, _0> || Get<0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride01, _0> || Get<0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride10, _0> || Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride11, _0> || Get<1, 1>(src.Layout().Stride()) > 0)),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyGM2L1, all runtime source strides should be greater than 0."));
        } else if constexpr (SrcLayout::depth == FIVE_DIM_DATA) {
            using SrcStride100 = Std::remove_cvref_t<decltype(Get<1, 0, 0>(src.Layout().Stride()))>;
            using SrcStride101 = Std::remove_cvref_t<decltype(Get<1, 0, 1>(src.Layout().Stride()))>;
            using SrcStride110 = Std::remove_cvref_t<decltype(Get<1, 1, 0>(src.Layout().Stride()))>;
            using SrcStride111 = Std::remove_cvref_t<decltype(Get<1, 1, 1>(src.Layout().Stride()))>;
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(src.Layout().Shape()) > 0 && Get<1, 0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(src.Layout().Shape()) > 0 && Get<1, 1, 1>(src.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyGM2L1, source batch and all source row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Stride()) > 0 &&
                (Std::is_same_v<SrcStride100, _0> || Get<1, 0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride101, _0> || Get<1, 0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride110, _0> || Get<1, 1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<SrcStride111, _0> || Get<1, 1, 1>(src.Layout().Stride()) > 0)),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyGM2L1, all runtime source strides should be greater than 0."));
        }
        using CopyGM2L1Impl = typename CopyGM2L1Routing<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyGM2L1Impl::template Run<trait, T, U>(dst, src);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
