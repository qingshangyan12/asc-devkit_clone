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
    "impl/tensor_api/arch/cube/l1_to_bt/copy_impl/nd2nd.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nd2nd.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H

#include "impl/tensor_api/arch/cube/l1_to_bt/copy_impl/instruction.h"

namespace AscendC {
namespace Te {

class CopyL12BTND {
public:
    template <const CopyL12BTTrait& trait, typename T, typename U>
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
    template <const CopyL12BTTrait& trait, typename T, typename U>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckLayoutPattern<T, U>();
        CheckDataType::CheckL12BtDataType<T, U>();
    }

    template <const CopyL12BTTrait& trait, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        uint16_t srcCol;
        uint16_t srcRow;
        uint16_t dstCol;
        uint16_t dstRow;
        uint16_t blockCount;
        if constexpr (IsSatisfiedPtnFormatV<U, NDLayoutPtn>) {
            srcCol = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcLayout);
            srcRow = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout);
            blockCount = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcLayout);
        } else {
            srcCol = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
            srcRow = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout);
            blockCount = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcLayout);
        }
        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            dstCol = GetElement<AttrInfo::Shape, AttrInfo::Column>(dstLayout);
            dstRow = GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout);
        } else {
            dstCol = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(dstLayout);
            dstRow = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout);
        }

        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        bool convControl = false;
        if (IsOneOfAttrV<srcType, half> && IsOneOfAttrV<dstType, float>) {
            convControl = true;
        }

        uint16_t blockLen = Std::ceil_division(srcCol, C0_ELEMENT<srcType>);
        if constexpr (IsOneOfAttrV<srcType, float, int32_t>) {
            blockLen = Std::ceil_align(blockLen, 2);
        }

        uint16_t srcStride = (srcRow - srcCol) / C0_ELEMENT<srcType>;
        uint16_t dstStride = Std::ceil_align((dstRow - srcCol) / C0_ELEMENT<dstType>, 2);
        CopyL12BTInstr::DataCopy(dst, src, convControl, blockCount, blockLen, srcStride, dstStride);
    }

    // Batch case: B matrices each laid out as (row, col). The underlying L1->BT instruction has no
    // batch dimension, so each matrix is emitted as one contiguous block: blockCount carries B, and
    // blockLen / src/dst stride are computed from the per-matrix stride taken from Get<0> of src/dst
    // layouts. Assumes each matrix is internally compact (row pitch equals column count); non-compact
    // inner layouts cannot be expressed via a single blockLen and are not supported on this path.
    template <const CopyL12BTTrait& trait, typename T, typename U>
    __aicore__ inline static void BatchDataCopyImpl(const T& dst, const U& src)
    {
        CheckTemplate<trait, T, U>();

        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto srcLayout = src.Layout();
        auto dstLayout = dst.Layout();

        uint16_t batchSize = Get<0>(srcLayout.Shape());
        uint32_t srcMatrixStride = Get<0>(srcLayout.Stride());
        uint32_t dstMatrixStride = Get<0>(dstLayout.Stride());

        // Strip the leading B axis before calling GetElement, otherwise the dim=0/1 split below
        // would read the batch axis as row/column.
        auto srcInner = Te::Get<1>(srcLayout);

        uint32_t srcShapeRows;
        uint32_t srcShapeColumns;
        if constexpr (IsSatisfiedPtnFormatV<U, NDLayoutPtn>) {
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row>(srcInner);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column>(srcInner);
        } else {
            srcShapeRows = GetElement<AttrInfo::Shape, AttrInfo::Row, 1>(srcInner);
            srcShapeColumns = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcInner);
        }

        bool convControl = false;
        if (IsOneOfAttrV<srcType, half> && IsOneOfAttrV<dstType, float>) {
            convControl = true;
        }

        uint16_t blockCount = batchSize;
        uint32_t matrixElems = srcShapeRows * srcShapeColumns;
        uint16_t blockLen = Std::ceil_division(matrixElems, C0_ELEMENT<srcType>);
        if constexpr (IsOneOfAttrV<srcType, float, int32_t>) {
            blockLen = Std::ceil_align(blockLen, 2);
        }

        uint16_t srcStride = (srcMatrixStride - matrixElems) / C0_ELEMENT<srcType>;
        uint16_t dstStride = Std::ceil_align((dstMatrixStride - matrixElems) / C0_ELEMENT<dstType>, 2);
        CopyL12BTInstr::DataCopy(dst, src, convControl, blockCount, blockLen, srcStride, dstStride);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_BT_COPY_IMPL_ND2ND_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
