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
    "impl/tensor_api/arch/cube/l1_to_fb/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_H

#include "impl/tensor_api/arch/utils/arch_utils.h"
#include "impl/tensor_api/arch/cube/l1_to_fb/routing.h"

namespace AscendC {
namespace Te {

constexpr CopyL12FBTrait DEFAULT_COPY_L1_FB_TRAIT;

struct CopyL12FBTraitDefault {
    using TraitType = CopyL12FBTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_FB_TRAIT;
};

struct CopyL12FB {
public:
    template <typename Tp, const Tp& traits, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            DataCopyImpl<traits, Args...>(args...);
        }
    }

private:
    template <const CopyL12FBTrait& trait = DEFAULT_COPY_L1_FB_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        using DstPos = GetMemLocation<T>;
        using SrcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<DstPos, Location::FIXBUF>, "CopyL12FB requires destination on FIXBUF");
        static_assert(Std::is_same_v<SrcPos, Location::L1>, "CopyL12FB requires source on L1");
        using DstLayout = typename T::layoutType;
        using SrcLayout = typename U::layoutType;
        using DstLayoutPtn = GetLayoutPattern<DstLayout>;
        using SrcLayoutPtn = GetLayoutPattern<SrcLayout>;
        if constexpr (DstLayout::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Shape()) > 0 && Get<1>(dst.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL12FB, dst row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(dst.Layout().Stride()) > 0 && Get<1>(dst.Layout().Stride()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL12FB, all dst strides should be greater than 0."));
        } else if constexpr (DstLayout::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0, 0>(dst.Layout().Shape()) > 0 &&
                Get<0, 1>(dst.Layout().Shape()) > 0 && Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL12FB, all dst row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT(((Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(dst.Layout().Stride()))>,
                _0> || Get<0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL12FB, all runtime dst strides should be greater than 0."));
        }
        if constexpr (SrcLayout::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Shape()) > 0 && Get<1>(src.Layout().Shape()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL12FB, source row and column shape should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((Get<0>(src.Layout().Stride()) > 0 && Get<1>(src.Layout().Stride()) > 0),
                KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL12FB, all source strides should be greater than 0."));
        } else if constexpr (SrcLayout::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((Get<0, 0>(src.Layout().Shape()) > 0 &&
                Get<0, 1>(src.Layout().Shape()) > 0 && Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL12FB, all source row and column shapes should be greater than 0."));
            ASCENDC_DEBUG_ASSERT(((Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(src.Layout().Stride()))>,
                _0> || Get<0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(src.Layout().Stride()))>, _0> ||
                Get<0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL12FB, all runtime source strides should be greater than 0."));
        }
        using CopyL12FBImpl = typename CopyL12FBRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL12FBImpl::template Run<trait, T, U>(dst, src);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
