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
    "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/scaleb_nn2nn.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file scaleb_nn2nn.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_SCALEB_NN2NN_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_SCALEB_NN2NN_H

#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/instruction.h"

namespace AscendC {
namespace Te {

class CopyGmToCbufScaleBNn2Nn {
public:
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src)
    {
        DataCopyImpl<trait>(dst, src);
    }

    template <const CopyGM2L1Trait& trait, typename T, typename U, typename Coord, typename Shape>
    __aicore__ inline static void Run(
        const T& dst, const U& src, const Coord& coord, const Shape& shape)
    {
        CheckTemplate<trait, T, U>();
        using type = typename U::elementType;
        auto regionShape = MakeSliceShape(coord, src.Layout(), shape);
        auto srcOffset = src.Layout()(coord);
        constexpr auto depth = U::layoutType::depth;
        uint32_t srcShapeColB;
        uint32_t srcShapeColS;
        uint32_t srcShapeRowB;
        uint32_t srcStrideColB;
        uint32_t srcStrideColS;
        uint32_t dstStrideColB;
        if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
            srcShapeColB = Get<1, 1, 1>(regionShape);
            srcShapeColS = Get<1, 1, 0>(regionShape);
            srcShapeRowB = Get<1, 0, 1>(regionShape);
            srcStrideColB = Get<1, 1, 1>(src.Layout().Stride());
            srcStrideColS = Get<1, 1, 0>(src.Layout().Stride());
            dstStrideColB = Get<1, 1, 1>(dst.Layout().Stride());
        } else {
            srcShapeColB = Get<1, 1>(regionShape);
            srcShapeColS = Get<1, 0>(regionShape);
            srcShapeRowB = Get<0, 1>(regionShape);
            srcStrideColB = Get<1, 1>(src.Layout().Stride());
            srcStrideColS = Get<1, 0>(src.Layout().Stride());
            dstStrideColB = Get<1, 1>(dst.Layout().Stride());
        }
        uint32_t blockLen = srcShapeRowB * sizeof(type) * srcShapeColS * srcStrideColS;
        uint64_t srcStride = srcStrideColB * sizeof(type);
        uint32_t dstStride = dstStrideColB * sizeof(type);
        uint32_t batchNum = GetBatchShapeFromShape(regionShape);
        uint64_t srcBatchStride = 0;
        if constexpr (depth == FIVE_DIM_DATA) {
            srcBatchStride = Get<0>(src.Layout().Stride());
        }
        for (uint32_t i = 0; i < batchNum; ++i) {
            if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
                auto dstBatch = MakeSingleBatchSubTensor(dst, i);
                CopyGmToCbufAlignV2Base::DataCopyRegion(dstBatch, src,
                    srcOffset + i * srcBatchStride, srcShapeColB, blockLen, 0, 0,
                    src.Engine().GetCacheMode(), srcStride, dstStride);
            } else {
                CopyGmToCbufAlignV2Base::DataCopyRegion(dst, src, srcOffset, srcShapeColB,
                    blockLen, 0, 0, src.Engine().GetCacheMode(), srcStride, dstStride);
            }
        }
    }

private:
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckLayoutPattern<U, T>();
        CheckDataType::CheckGm2L1ScaleDataType<T, U>();
    }

    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();
        if constexpr (U::layoutType::depth == FIVE_DIM_DATA) {
            auto srcLayout = src.Layout();
            auto dstLayout = dst.Layout();
            EmitCopy(dst, src, RemoveBatchDim(srcLayout), RemoveBatchDim(dstLayout), Get<0>(srcLayout.Shape()),
                Get<0>(srcLayout.Stride()), Get<0>(dstLayout.Stride()));
        } else {
            EmitCopy(dst, src, src.Layout(), dst.Layout(), 1, 0, 0);
        }
    }

    template <typename T, typename U, typename SrcLayout, typename DstLayout>
    __aicore__ inline static void EmitCopy(const T& dst, const U& src, const SrcLayout& srcLayout,
        const DstLayout& dstLayout, uint16_t batchNum, uint64_t srcBatchStride, uint64_t dstBatchStride)
    {
        using type = typename U::elementType;

        auto srcShapeColB = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
        auto srcShapeColS = GetElement<AttrInfo::Shape, AttrInfo::Column, 0>(srcLayout);
        auto srcShapeRowB = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcLayout);
        auto srcStrideColB = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout);
        auto srcStrideColS = GetElement<AttrInfo::Stride, AttrInfo::Column, 0>(srcLayout);

        auto dstStrideColB = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout);

        uint8_t cacheMode = src.Engine().GetCacheMode();

        // lprp mode, dst_stride % C0_SIZE should be 0
        // multi rows copy, dst non-contiguous case
        uint32_t blockCount = srcShapeColB;
        uint32_t blockLen = srcShapeRowB * sizeof(type) * srcShapeColS * srcStrideColS;
        uint64_t srcStride = srcStrideColB * sizeof(type);
        uint32_t dstStride = dstStrideColB * sizeof(type);

        uint8_t leftPaddingCnt = 0;
        uint8_t rightPaddingCnt = 0;
        for (uint16_t batchIndex = 0; batchIndex < batchNum; ++batchIndex) {
            CopyGmToCbufAlignV2Base::CopyGmToCbufAlignV2(
                (__cbuf__ half*)((dst.Data() + batchIndex * dstBatchStride).Get()),
                (__gm__ half*)((src.Data() + batchIndex * srcBatchStride).Get()), blockCount, blockLen,
                leftPaddingCnt, rightPaddingCnt, cacheMode, srcStride, dstStride);
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
