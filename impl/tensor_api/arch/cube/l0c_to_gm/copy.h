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
    "impl/tensor_api/arch/cube/l0c_to_gm/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/atom/copy_traits_impl.h"
#include "impl/tensor_api/arch/cube/utils/l0c2out_utils.h"
#include "impl/tensor_api/arch/cube/l0c_to_gm/routing.h"

namespace AscendC {
namespace Te {

constexpr CopyL0C2GMTrait DEFAULT_COPY_L0C2GM_TRAIT = CopyL0C2GMTrait{};
struct CopyL0C2GMTraitDefault {
    using TraitType = CopyL0C2GMTrait;
    static constexpr const TraitType value = DEFAULT_COPY_L0C2GM_TRAIT;
};

struct CopyL0C2GMBase {
public:
    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(
        const T& dst, const U& src, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");

        if constexpr (T::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(dst.Layout().Stride()))>, _0> ||
                Get<1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(dst.Layout().Shape()) > 0 &&
                Get<0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        }
        if constexpr (U::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(src.Layout().Stride()))>, _0> ||
                Get<1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(src.Layout().Shape()) > 0 &&
                Get<0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(src.Layout().Stride()))>, _0> ||
                Get<0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(src.Layout().Stride()))>, _0> ||
                Get<0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        }
        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, params);
    }

    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<Std::is_same_v<S, uint64_t>, void>::type DataCopyImpl(
        const T& dst, const U& src, const S& quant, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");

        if constexpr (T::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(dst.Layout().Stride()))>, _0> ||
                Get<1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(dst.Layout().Shape()) > 0 &&
                Get<0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        }
        if constexpr (U::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(src.Layout().Stride()))>, _0> ||
                Get<1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(src.Layout().Shape()) > 0 &&
                Get<0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(src.Layout().Stride()))>, _0> ||
                Get<0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(src.Layout().Stride()))>, _0> ||
                Get<0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        }
        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, quant, params);
    }

    template <const CopyL0C2GMTrait& trait = DEFAULT_COPY_L0C2GM_TRAIT, typename T, typename U, typename S>
    __aicore__ inline static typename Std::enable_if<IsAttrTensorV<S>, void>::type DataCopyImpl(
        const T& dst, const U& src, const S& quant, const FixpipeParams& params = DEFAULT_FIXPIPE_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(Std::is_same_v<dstPos, Location::GM>, "When Copy tensor from L0C to GM, dst tensor must on GM");
        static_assert(Std::is_same_v<srcPos, Location::L0C>, "When Copy tensor from L0C to GM, src tensor must on L0C");

        if constexpr (T::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(dst.Layout().Stride()))>, _0> ||
                Get<1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(dst.Layout().Shape()) > 0 &&
                Get<0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        } else if constexpr (T::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(dst.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(dst.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst shape in CopyL0C2GM, all dst shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(dst.Layout().Stride()))>, _0> ||
                Get<0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(dst.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(dst.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(dst.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dst stride in CopyL0C2GM, all runtime dst stride leaves should be greater than 0."));
        }
        if constexpr (U::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(src.Layout().Stride()))>, _0> ||
                Get<1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(src.Layout().Shape()) > 0 &&
                Get<0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(src.Layout().Stride()))>, _0> ||
                Get<0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(src.Layout().Stride()))>, _0> ||
                Get<0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        } else if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(src.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(src.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src shape in CopyL0C2GM, all src shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(src.Layout().Stride()))>, _0> ||
                Get<0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(src.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(src.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(src.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check src stride in CopyL0C2GM, all runtime src stride leaves should be greater than 0."));
        }
        if constexpr (S::layoutType::depth == TWO_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(quant.Layout().Shape()) > 0 &&
                Get<1>(quant.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant shape in CopyL0C2GM, all quant shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(quant.Layout().Stride()))>, _0> ||
                Get<0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1>(quant.Layout().Stride()))>, _0> ||
                Get<1>(quant.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant stride in CopyL0C2GM, all runtime quant stride leaves should be greater than 0."));
        } else if constexpr (S::layoutType::depth == THREE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(quant.Layout().Shape()) > 0 &&
                Get<1, 0>(quant.Layout().Shape()) > 0 &&
                Get<1, 1>(quant.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant shape in CopyL0C2GM, all quant shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(quant.Layout().Stride()))>, _0> ||
                Get<0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(quant.Layout().Stride()))>, _0> ||
                Get<1, 0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(quant.Layout().Stride()))>, _0> ||
                Get<1, 1>(quant.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant stride in CopyL0C2GM, all runtime quant stride leaves should be greater than 0."));
        } else if constexpr (S::layoutType::depth == FOUR_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0, 0>(quant.Layout().Shape()) > 0 &&
                Get<0, 1>(quant.Layout().Shape()) > 0 &&
                Get<1, 0>(quant.Layout().Shape()) > 0 &&
                Get<1, 1>(quant.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant shape in CopyL0C2GM, all quant shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 0>(quant.Layout().Stride()))>, _0> ||
                Get<0, 0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0, 1>(quant.Layout().Stride()))>, _0> ||
                Get<0, 1>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0>(quant.Layout().Stride()))>, _0> ||
                Get<1, 0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1>(quant.Layout().Stride()))>, _0> ||
                Get<1, 1>(quant.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant stride in CopyL0C2GM, all runtime quant stride leaves should be greater than 0."));
        } else if constexpr (S::layoutType::depth == FIVE_DIM_DATA) {
            ASCENDC_DEBUG_ASSERT((
                Get<0>(quant.Layout().Shape()) > 0 &&
                Get<1, 0, 0>(quant.Layout().Shape()) > 0 &&
                Get<1, 0, 1>(quant.Layout().Shape()) > 0 &&
                Get<1, 1, 0>(quant.Layout().Shape()) > 0 &&
                Get<1, 1, 1>(quant.Layout().Shape()) > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant shape in CopyL0C2GM, all quant shape leaves should be greater than 0."));
            ASCENDC_DEBUG_ASSERT((
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<0>(quant.Layout().Stride()))>, _0> ||
                Get<0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 0>(quant.Layout().Stride()))>, _0> ||
                Get<1, 0, 0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 0, 1>(quant.Layout().Stride()))>, _0> ||
                Get<1, 0, 1>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 0>(quant.Layout().Stride()))>, _0> ||
                Get<1, 1, 0>(quant.Layout().Stride()) > 0) &&
                (Std::is_same_v<Std::remove_cvref_t<decltype(Get<1, 1, 1>(quant.Layout().Stride()))>, _0> ||
                Get<1, 1, 1>(quant.Layout().Stride()) > 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check quant stride in CopyL0C2GM, all runtime quant stride leaves should be greater than 0."));
        }
        using DstLayoutPtn = GetLayoutPattern<typename T::layoutType>;
        using SrcLayoutPtn = GetLayoutPattern<typename U::layoutType>;

        using CopyL0C2GMImpl = typename CopyL0C2GMRouting<CURRENT_ARCH_VERSION, DstLayoutPtn, SrcLayoutPtn>::type;
        CopyL0C2GMImpl::template Run<trait>(dst, src, quant, params);
    }
};

struct CopyL0C2GM : public CopyL0C2GMBase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        DataCopyImpl<trait>(args...);
    }
};

struct CopyL0C2GMWith : public CopyL0C2GMBase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIV {
            return;
        }
        DataCopyImpl<trait>(args...);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
