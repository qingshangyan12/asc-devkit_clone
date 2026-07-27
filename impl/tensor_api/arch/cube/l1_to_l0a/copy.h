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
    "impl/tensor_api/arch/cube/l1_to_l0a/copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H

#include "impl/tensor_api/arch/cube/l1_to_l0a/routing.h"

namespace AscendC {
namespace Te {

constexpr CopyL12L0ATrait DEFAULT_COPY_L1_TO_L0A_TRAIT;

struct CopyL12L0ATraitDefault {
    using TraitType = CopyL12L0ATrait;
    static constexpr const TraitType value = DEFAULT_COPY_L1_TO_L0A_TRAIT;
};

using CopyL12L0AModeSet = TupleMap<
    Std::tuple<Std::tuple<_1, _0>, CopyMode::NORMAL>, Std::tuple<Std::tuple<_1, _1>, CopyMode::NORMAL>,
    Std::tuple<Std::tuple<_0, _0>, CopyMode::TRANS>, Std::tuple<Std::tuple<_0, _1>, CopyMode::TRANS_B8B4>>;

struct CopyL12L0ABase {
public:
    template <
        const CopyL12L0ATrait& trait = DEFAULT_COPY_L1_TO_L0A_TRAIT, typename T, typename U, typename PadT = int16_t>
    __aicore__ inline static void LoadDataImpl(
        const T& dst, const U& src, const Img2ColParams<PadT>& params = DEFAULT_IMG2COL_PARAMS)
    {
        using dstPos = GetMemLocation<T>;
        using srcPos = GetMemLocation<U>;
        static_assert(
            Std::is_same_v<dstPos, Location::L0A>, "When Copy tensor from L1 to L0A, dst tensor must be from L0A.");
        static_assert(
            Std::is_same_v<srcPos, Location::L1>, "When Copy tensor from L1 to L0A, src tensor must be from L1.");
        using DstLayout = typename T::layoutType;
        using SrcLayout = typename U::layoutType;
        using DstPattern = GetLayoutPattern<DstLayout>;
        using SrcPattern = GetLayoutPattern<SrcLayout>;
        TENSOR_API_DEBUG_ASSERT(
            (is_shape_valid(dst.Layout().Shape())),
            TENSOR_API_REPORT_INTERNAL(
                report_tensor_layout_error<tensor_layout_error_kind::SHAPE>, dst.Layout(), "dst", "CopyL12L0A"));
        TENSOR_API_DEBUG_ASSERT(
            (is_layout_stride_valid(dst.Layout())),
            TENSOR_API_REPORT_INTERNAL(
                report_tensor_layout_error<tensor_layout_error_kind::STRIDE>, dst.Layout(), "dst", "CopyL12L0A"));
        TENSOR_API_DEBUG_ASSERT(
            (is_shape_valid(src.Layout().Shape())),
            TENSOR_API_REPORT_INTERNAL(
                report_tensor_layout_error<tensor_layout_error_kind::SHAPE>, src.Layout(), "src", "CopyL12L0A"));
        TENSOR_API_DEBUG_ASSERT(
            (is_layout_stride_valid(src.Layout())),
            TENSOR_API_REPORT_INTERNAL(
                report_tensor_layout_error<tensor_layout_error_kind::STRIDE>, src.Layout(), "src", "CopyL12L0A"));
        if constexpr (Std::is_same_v<SrcPattern, NC1HWC0LayoutPtn>) {
            // Img2Col: NZ<-NC1HWC0 for conv feature maps; params drives filter/stride/pad.
            using CopyL12L0AImpl =
                typename CopyL12L0ARouting<CURRENT_ARCH_VERSION, DstPattern, SrcPattern, CopyMode::NORMAL>::type;
            CopyL12L0AImpl::template Run<trait, T, U, PadT>(dst, src, params);
        } else {
            // Common load (NZ<-NZ / NZ<-ZN): params unused.
            if constexpr (DstLayout::depth == FIVE_DIM_DATA && SrcLayout::depth == FIVE_DIM_DATA) {
                TENSOR_API_DEBUG_ASSERT(
                    (Get<0>(src.Layout().Shape()) == Get<0>(dst.Layout().Shape())),
                    TENSOR_API_LOG_INTERNAL(
                        "Failed to check src and dst batch shape sizes in CopyL12L0A, src batch shape size must equal "
                        "dst batch shape size; current srcBatchShape is %lld, dstBatchShape is %lld.",
                        static_cast<long long>(Get<0>(src.Layout().Shape())),
                        static_cast<long long>(Get<0>(dst.Layout().Shape()))));
                TENSOR_API_DEBUG_ASSERT(
                    (Get<0>(src.Layout().Stride()) == RemoveBatchDim(src.Layout()).Capacity() &&
                     Get<0>(dst.Layout().Stride()) == RemoveBatchDim(dst.Layout()).Capacity()),
                    TENSOR_API_LOG_INTERNAL(
                        "Failed to check src and dst batch strides in CopyL12L0A, src batch stride must equal the src "
                        "matrix capacity, and dst batch stride must equal the dst matrix capacity; current "
                        "srcBatchStride is %lld, srcMatrixCapacity is %lld, dstBatchStride is %lld, "
                        "dstMatrixCapacity is %lld.",
                        static_cast<long long>(Get<0>(src.Layout().Stride())),
                        static_cast<long long>(RemoveBatchDim(src.Layout()).Capacity()),
                        static_cast<long long>(Get<0>(dst.Layout().Stride())),
                        static_cast<long long>(RemoveBatchDim(dst.Layout()).Capacity())));
            }
            constexpr auto isB8B4Type = sizeof(typename T::elementType) == 1;
            constexpr auto noTrans = Std::is_same_v<DstPattern, SrcPattern>;
            using CopyL12L0AMode =
                typename CopyL12L0AModeSet::template Get<Std::tuple<Std::Int<noTrans>, Std::Int<isB8B4Type>>>;
            static_assert(!Std::is_same_v<CopyL12L0AMode, Std::ignore_t>, "Unsupported CopyL12L0AMode.");
            using CopyL12L0AImpl =
                typename CopyL12L0ARouting<CURRENT_ARCH_VERSION, DstPattern, SrcPattern, CopyL12L0AMode>::type;
            CopyL12L0AImpl::template Run<trait, T, U>(dst, src);
        }
    }
};

struct CopyL12L0A : public CopyL12L0ABase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            LoadDataImpl<trait>(args...);
        }
    }
};

struct CopyL12L0AWith : public CopyL12L0ABase {
public:
    template <typename Tp, const Tp& trait, typename... Args>
    __aicore__ inline static void Copy(const Args&... args)
    {
        if ASCEND_IS_AIC {
            LoadDataImpl<trait>(args...);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_L0A_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
