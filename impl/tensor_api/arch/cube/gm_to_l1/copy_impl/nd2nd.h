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
    "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/nd2nd.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nd2nd.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_ND2ND_H
#define IMPL_TENSOR_API_ARCH_CUBE_GM_TO_L1_COPY_IMPL_ND2ND_H

#include "impl/tensor_api/arch/cube/gm_to_l1/copy_impl/instruction.h"

namespace AscendC {
namespace Te {

class CopyGmToCbufAlignV2ND {
public:
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src)
    {
        // Batch layouts carry a leading B axis: (B, (row, col)) -> depth 3,
        // (B, ((1, row), (1, col))) -> depth 5. Non-batch layouts are depth 2/4.
        constexpr auto gmDepth = NestingDepthV<decltype(src.Layout().Shape())>;
        if constexpr (gmDepth == THREE_DIM_DATA || gmDepth == FIVE_DIM_DATA) {
            BatchDataCopyImpl<trait, T, U>(dst, src);
        } else {
            DataCopyImpl<trait, T, U>(dst, src);
        }
    }

private:
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckLayoutPattern<U, T>();
        CheckDataType::CheckGm2L1AlignV2NDDataType<T, U>(); // todo
    }

    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        using type = typename U::elementType;
        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        uint32_t srcShapeRows;
        uint32_t srcShapeColumns;
        uint32_t srcStrideRows;
        uint32_t dstShapeColumns;
        uint32_t dstStrideRows;

        if constexpr(IsSatisfiedPtnFormatV<T, NDLayoutPtn>){
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcLayout);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcLayout);
            srcStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout);
        } else {
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcLayout);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
            srcStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout);
        }

        if constexpr(IsSatisfiedPtnFormatV<T, NDLayoutPtn>){
            dstShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column>(dstLayout);
            dstStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout);
        } else {
            dstShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(dstLayout);
            dstStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout);
        }

        uint8_t cacheMode = src.Engine().GetCacheMode();

        // normal mode, dst_stride % C0_SIZE should be 0
        // compact mode, blockLen equals dstStride
        // multi rows copy, dst non-contiguous case
        uint32_t blockCount = srcShapeRows;
        uint32_t blockLen = srcShapeColumns * sizeof(type);
        uint64_t srcStride = srcStrideRows * sizeof(type);
        uint32_t dstStride = dstStrideRows * sizeof(type);

        if ((srcShapeRows == 1) || (srcShapeColumns == 1)
            || (srcStrideRows == srcShapeColumns && dstStrideRows == dstShapeColumns
                && srcStrideRows == dstStrideRows)) {
            // compact mode, one line
            blockCount = 1;
            // must use srcShape, there is a scenario of small to large, using dstShape will cause src out of bound
            blockLen = srcShapeRows * srcShapeColumns * sizeof(type);
            srcStride = 0;
            dstStride = blockLen;
        }
        if constexpr (IsB4Type<type>) {
            // move fp4 as b8, need to be divided by 2
            blockLen = blockLen >> 1;
            srcStride = srcStride >> 1;
            dstStride = dstStride >> 1;
        }
        CopyGmToCbufAlignV2Base::DataCopy(dst, src, blockCount, blockLen, 0, 0, cacheMode, srcStride, dstStride);
    }

    // Batch case: layout is (B, (M, N)) with strides (sB, (sM, sN)). Strip the leading B axis,
    // then derive the nd2nz instruction parameters from the per-matrix sub-layout:
    //   - ndNum carries the batch dimension;
    //   - loop1SrcStride / loop4SrcStride encode the per-row and per-batch GM strides in bytes;
    //   - loop2 / loop3 / loop4 DstStride encode the NZ destination's N / C0 / per-matrix strides
    //     in C0_ELEMENT units.
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void BatchDataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        using type = typename U::elementType;
        auto srcLayout = src.Layout();
        auto dstLayout = dst.Layout();

        uint16_t ndNum = Get<0>(srcLayout.Shape());
        uint64_t srcBatchStride = Get<0>(srcLayout.Stride());
        uint32_t dstBatchStride = Get<0>(dstLayout.Stride());

        // Strip the leading B axis before calling GetElement, otherwise the dim=0/1 split below
        // would read the batch axis as row/column.
        auto srcInner = Te::Get<1>(srcLayout);
        auto dstInner = Te::Get<1>(dstLayout);

        uint16_t nValue;
        uint32_t dValue;
        uint32_t srcRowStride;
        if constexpr (IsSatisfiedPtnFormatV<U, NDLayoutPtn>) {
            nValue = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcInner);
            dValue = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcInner);
            srcRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcInner);
        } else {
            nValue = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcInner);
            dValue = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcInner);
            srcRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcInner);
        }

        uint32_t dstColumnStride;
        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            dstColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column>(dstInner);
        } else {
            dstColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstInner);
        }

        if constexpr (IsB4Type<type>) {
            // move fp4 as b8, need to be divided by 2
            dValue = dValue >> 1;
            srcRowStride = srcRowStride >> 1;
            srcBatchStride = srcBatchStride >> 1;
        }

        uint64_t loop1SrcStride = static_cast<uint64_t>(srcRowStride) * sizeof(type);
        uint64_t loop4SrcStride = srcBatchStride * sizeof(type);
        uint16_t dstNzC0Stride = dstColumnStride / C0_ELEMENT<type>;
        uint16_t loop2DstStride = 1;
        uint16_t loop3DstStride = dstNzC0Stride;
        uint16_t loop4DstStride = static_cast<uint16_t>(dstBatchStride / C0_ELEMENT<type>);
        uint8_t cacheMode = src.Engine().GetCacheMode();

        CopyGmToCbufMultiNd2nzInstr::DataCopy(dst, src, ndNum, loop2DstStride, loop3DstStride,
                                              loop4DstStride, loop1SrcStride, cacheMode, nValue, dValue,
                                              loop4SrcStride, false);
    }
};
} // namespace Te
} // namespace AscendC

#endif

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
