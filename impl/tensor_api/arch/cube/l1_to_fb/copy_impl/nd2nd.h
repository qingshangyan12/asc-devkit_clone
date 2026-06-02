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
    "impl/tensor_api/arch/cube/l1_to_fb/copy_impl/nd2nd.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nd2nd.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_IMPL_ND2ND_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_IMPL_ND2ND_H

#include "impl/tensor_api/arch/cube/l1_to_fb/copy_impl/instruction.h"

namespace AscendC {
namespace Te {

class CopyL12FBND {
public:
    template <const CopyL12FBTrait& trait, typename T, typename U>
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
    template <const CopyL12FBTrait& trait, typename T, typename U>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckDataType::CheckL12FbDataType<T, U>();
        CheckLayoutPattern<U, T>();
    }

    // Emits a DataCopy on the (already batch-stripped) src/dst layouts. matrixCount/srcMatrixStride/
    // dstMatrixStride carry the batch dimension when matrixCount > 1; non-batch path passes 1/0/0.
    // Pattern is read from the original tensor types T/U so the NDLayoutPtn dim=0 vs NDExt dim=1
    // split keeps working on the inner sub-layout.
    template <typename T, typename U, typename SrcLayout, typename DstLayout>
    __aicore__ inline static void EmitCopy(const T& dst, const U& src, const SrcLayout& srcLayout,
                                           const DstLayout& dstLayout, uint16_t matrixCount,
                                           uint32_t srcMatrixStride, uint32_t dstMatrixStride)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        constexpr uint32_t deqTensorAddrAlignValue = 128;
        constexpr uint16_t fbufBurstLenUnit = 64;

        uint16_t srcCol;
        uint16_t srcRow;
        uint16_t dstRow;
        uint16_t srcShapeRow;
        if constexpr (IsSatisfiedPtnFormatV<U, NDLayoutPtn>) {
            srcCol = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcLayout);
            srcRow = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout);
            srcShapeRow = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcLayout);
        } else {
            srcCol = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
            srcRow = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout);
            srcShapeRow = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcLayout);
        }
        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            dstRow = GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout);
        } else {
            dstRow = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout);
        }

        if (matrixCount > 1) {
            // Batch path: each matrix becomes one block; assumes per-matrix internal compactness so
            // a single blockLen can cover the matrix.
            uint16_t blockCount = matrixCount;
            uint16_t blockLen =
                Std::ceil_align(srcShapeRow * srcCol * sizeof(srcType), deqTensorAddrAlignValue) / fbufBurstLenUnit;
            uint16_t srcStride = Std::ceil_division(srcMatrixStride * sizeof(srcType), C0_SIZE<>);
            uint16_t dstStride =
                Std::ceil_align(dstMatrixStride * sizeof(dstType), deqTensorAddrAlignValue) / fbufBurstLenUnit;
            CopyL12FBInstr::DataCopy(dst, src, blockCount, blockLen, srcStride, dstStride);
            return;
        }

        uint16_t blockCount = srcShapeRow;
        uint16_t blockLen = Std::ceil_align(srcCol * sizeof(srcType), deqTensorAddrAlignValue) / fbufBurstLenUnit;
        uint16_t srcStride = Std::ceil_division(srcRow * sizeof(srcType), C0_SIZE<>);
        uint16_t dstStride = Std::ceil_align(dstRow * sizeof(dstType), deqTensorAddrAlignValue) / fbufBurstLenUnit;

        CopyL12FBInstr::DataCopy(dst, src, blockCount, blockLen, srcStride, dstStride);
    }

    template <const CopyL12FBTrait& trait, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();
        EmitCopy(dst, src, src.Layout(), dst.Layout(), 1, 0, 0);
    }

    // Batch case: strip the leading B axis, then reuse the single-matrix path on the sub-layouts.
    // Per-batch src stride (Get<0> of src stride) is the per-matrix L1 pitch; per-batch dst stride
    // (Get<0> of dst stride) is the per-matrix FB pitch.
    template <const CopyL12FBTrait& trait, typename T, typename U>
    __aicore__ inline static void BatchDataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();
        auto srcLayout = src.Layout();
        auto dstLayout = dst.Layout();

        uint16_t matrixCount = Get<0>(srcLayout.Shape());
        uint32_t srcMatrixStride = Get<0>(srcLayout.Stride());
        uint32_t dstMatrixStride = Get<0>(dstLayout.Stride());

        EmitCopy(dst, src, Te::Get<1>(srcLayout), Te::Get<1>(dstLayout), matrixCount, srcMatrixStride,
                 dstMatrixStride);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_FB_COPY_IMPL_ND2ND_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
