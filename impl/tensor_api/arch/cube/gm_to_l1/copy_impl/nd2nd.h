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
        constexpr auto srcDepth = NestingDepthV<decltype(src.Layout().Shape())>;
        if constexpr (srcDepth == THREE_DIM_DATA || srcDepth == FIVE_DIM_DATA) {
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
        CheckDataType::CheckGm2L1AlignV2NDDataType<T, U>();
    }

    // Emits a DataCopy on the (already batch-stripped) src/dst layouts. matrixCount/srcMatrixStride/
    // dstMatrixStride carry the batch dimension when set (and 0/1 default for the non-batch path).
    // The src/dst pattern is read from the original tensor types T/U so the NDLayoutPtn dim=0 vs
    // NDExt dim=1 split keeps working on the inner sub-layout.
    template <typename T, typename U, typename SrcLayout, typename DstLayout>
    __aicore__ inline static void EmitCopy(const T& dst, const U& src, const SrcLayout& srcLayout,
                                           const DstLayout& dstLayout, uint32_t matrixCount,
                                           uint64_t srcMatrixStride, uint32_t dstMatrixStride)
    {
        using type = typename U::elementType;

        uint32_t srcShapeRows;
        uint32_t srcShapeColumns;
        uint32_t srcStrideRows;
        uint32_t dstShapeColumns;
        uint32_t dstStrideRows;

        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcLayout);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcLayout);
            srcStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout);
        } else {
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcLayout);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
            srcStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout);
        }

        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            dstShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column>(dstLayout);
            dstStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout);
        } else {
            dstShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(dstLayout);
            dstStrideRows = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout);
        }

        uint8_t cacheMode = src.Engine().GetCacheMode();

        if (matrixCount > 1) {
            // Batch path: each matrix becomes one block; assumes per-matrix internal compactness so
            // a single blockLen can cover the matrix.
            uint32_t blockCount = matrixCount;
            uint32_t blockLen = srcShapeRows * srcShapeColumns * sizeof(type);
            uint64_t srcStride = srcMatrixStride * sizeof(type);
            uint32_t dstStride = dstMatrixStride * sizeof(type);
            if constexpr (IsB4Type<type>) {
                blockLen = blockLen >> 1;
                srcStride = srcStride >> 1;
                dstStride = dstStride >> 1;
            }
            CopyGmToCbufAlignV2Base::DataCopy(dst, src, blockCount, blockLen, 0, 0, cacheMode, srcStride, dstStride);
            return;
        }

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

    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();
        EmitCopy(dst, src, src.Layout(), dst.Layout(), 1, 0, 0);
    }

    // Batch case: strip the leading B axis, then reuse the single-matrix path on the sub-layouts.
    // Per-batch GM stride (Get<0> of src stride) covers both contiguous (bmk) and non-contiguous
    // (mbk) memory; per-batch L1 stride (Get<0> of dst stride) is the per-matrix dst pitch.
    template <const CopyGM2L1Trait& trait, typename T, typename U>
    __aicore__ inline static void BatchDataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();
        auto srcLayout = src.Layout();
        auto dstLayout = dst.Layout();

        uint32_t matrixCount = Get<0>(srcLayout.Shape());
        uint64_t srcMatrixStride = Get<0>(srcLayout.Stride());
        uint32_t dstMatrixStride = Get<0>(dstLayout.Stride());

        EmitCopy(dst, src, Te::Get<1>(srcLayout), Te::Get<1>(dstLayout), matrixCount, srcMatrixStride,
                 dstMatrixStride);
    }
};
} // namespace Te
} // namespace AscendC

#endif

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
