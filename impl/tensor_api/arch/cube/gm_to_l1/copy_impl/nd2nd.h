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

    // Batch case: layout is (B, (M, N)) with strides (sB, (sM, sN)) on both sides. The dst layout
    // is still declared as ND/NDExt, but the data lands on L1 in NZ form, so the nz strides
    // required by the nd2nz instruction must be derived from the row/column extents rather than
    // read from the dst layout directly.
    //   - ndNum is the batch dimension;
    //   - loop1SrcStride / loop4SrcStride are the per-row and per-batch GM strides (bytes);
    //   - dstNzC0Stride = ceil_align(row, FRACTAL_FIXED) (in C0_ELEMENT units), per the NZ frame
    //     layout definition stride[1][1] = c0 * ceil_align(row, 16);
    //   - dstNzMatrixStride = ceil_align(row, FRACTAL_FIXED) * ceil_align(col, C0_ELEMENT)
    //     (in C0_ELEMENT units), i.e. the aligned NZ matrix size per batch.
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void BatchDataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        using type = typename U::elementType;
        auto srcLayout = src.Layout();

        uint16_t ndNum = Get<0>(srcLayout.Shape());
        uint64_t srcBatchStride = Get<0>(srcLayout.Stride());

        // Strip the leading B axis before calling GetElement, otherwise the dim=0/1 split below
        // would read the batch axis as row/column.
        auto srcInner = Te::Get<1>(srcLayout);

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

        if constexpr (IsB4Type<type>) {
            // move fp4 as b8, need to be divided by 2
            dValue = dValue >> 1;
            srcRowStride = srcRowStride >> 1;
            srcBatchStride = srcBatchStride >> 1;
        }

        uint64_t loop1SrcStride = static_cast<uint64_t>(srcRowStride) * sizeof(type);
        uint64_t loop4SrcStride = srcBatchStride * sizeof(type);

        // ND-declared dst lands on L1 in NZ form: derive nz strides from row/col extents.
        uint32_t alignedRow = Std::ceil_align(static_cast<uint32_t>(nValue), FRACTAL_FIXED);
        uint32_t alignedCol = Std::ceil_align(dValue, C0_ELEMENT<type>);
        uint16_t loop2DstStride = 1;
        uint16_t loop3DstStride = static_cast<uint16_t>(alignedRow);
        uint16_t loop4DstStride = static_cast<uint16_t>(
            (alignedRow * alignedCol) / C0_ELEMENT<type>);
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
