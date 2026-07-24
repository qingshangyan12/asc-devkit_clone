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
    "impl/tensor_api/arch/cube/l1_to_ub/copy_impl/data_copy.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file data_copy.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H
#define IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H

#include "impl/tensor_api/utils/utils_impl.h"
#include "impl/tensor_api/arch/cube/l1_to_ub/copy_impl/instruction.h"

namespace AscendC {
namespace Te {

class CopyL12UBND {
public:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src)
    {
        Execute<trait>(dst, src);
    }

private:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        ASCENDC_DEBUG_ASSERT((GetTotalRowShape(srcLayout) <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockCount in CopyL12UB ND path, blockCount should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((GetTotalColumnShape(srcLayout) <= 65535U * C0_ELEMENT<srcType>),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockLen in CopyL12UB ND path, blockLen should be no greater than 65535."));
        uint16_t blockCount = GetTotalRowShape(srcLayout);
        uint32_t blockLen = Std::ceil_division(GetTotalColumnShape(srcLayout), C0_ELEMENT<srcType>);

        uint32_t srcRowStride;
        uint32_t dstRowStride;
        if constexpr (IsSatisfiedPtnFormatV<U, NDLayoutPtn>) {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout)) >=
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout)) -
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<srcType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check srcStride in CopyL12UB ND path, source gap should be no greater than 65535."));
            srcRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row>(srcLayout);
        } else {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout)) >=
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout)) -
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<srcType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check srcStride in CopyL12UB ND path, source gap should be no greater than 65535."));
            srcRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(srcLayout);
        }
        if constexpr (IsSatisfiedPtnFormatV<T, NDLayoutPtn>) {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout)) >=
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout)) -
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<dstType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dstStride in CopyL12UB ND path, destination gap should be no greater than 65535."));
            dstRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row>(dstLayout);
        } else {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout)) >=
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout)) -
                static_cast<uint64_t>(GetTotalColumnShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<dstType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dstStride in CopyL12UB ND path, destination gap should be no greater than 65535."));
            dstRowStride = GetElement<AttrInfo::Stride, AttrInfo::Row, 1>(dstLayout);
        }
        ASCENDC_DEBUG_ASSERT((srcRowStride >= GetTotalColumnShape(srcLayout)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src stride in CopyL12UB ND path, source row stride should be no less than source column shape."));
        ASCENDC_DEBUG_ASSERT((dstRowStride >= GetTotalColumnShape(srcLayout)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst stride in CopyL12UB ND path, destination row stride should be no less than source column shape."));
        int64_t srcStride = Std::ceil_division(srcRowStride - GetTotalColumnShape(srcLayout), C0_ELEMENT<srcType>);
        int64_t dstStride = Std::ceil_division(dstRowStride - GetTotalColumnShape(srcLayout), C0_ELEMENT<dstType>);

        CopyCbufToUbufInstr::DataCopy(dst, src, blockCount, blockLen, srcStride, dstStride);
    }
};

class CopyL12UBDN {
public:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src)
    {
        Execute<trait>(dst, src);
    }

private:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        ASCENDC_DEBUG_ASSERT((GetTotalColumnShape(srcLayout) <= 4095), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockCount in CopyL12UB DN path, blockCount should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((GetTotalRowShape(srcLayout) <= 65535U * C0_ELEMENT<srcType>),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockLen in CopyL12UB DN path, blockLen should be no greater than 65535."));
        uint16_t blockCount = GetTotalColumnShape(srcLayout);
        uint32_t blockLen = Std::ceil_division(GetTotalRowShape(srcLayout), C0_ELEMENT<srcType>);

        uint32_t srcColumnStride;
        uint32_t dstColumnStride;
        if constexpr (IsSatisfiedPtnFormatV<U, DNLayoutPtn>) {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column>(srcLayout)) >=
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column>(srcLayout)) -
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<srcType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check srcStride in CopyL12UB DN path, source gap should be no greater than 65535."));
            srcColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column>(srcLayout);
        } else {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout)) >=
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout)) -
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<srcType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check srcStride in CopyL12UB DN path, source gap should be no greater than 65535."));
            srcColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout);
        }
        if constexpr (IsSatisfiedPtnFormatV<T, DNLayoutPtn>) {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column>(dstLayout)) >=
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column>(dstLayout)) -
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<dstType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dstStride in CopyL12UB DN path, destination gap should be no greater than 65535."));
            dstColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column>(dstLayout);
        } else {
            ASCENDC_DEBUG_ASSERT((
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout)) >=
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) &&
                static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout)) -
                static_cast<uint64_t>(GetTotalRowShape(srcLayout)) <=
                65535ULL * C0_ELEMENT<dstType>), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
                "Failed to check dstStride in CopyL12UB DN path, destination gap should be no greater than 65535."));
            dstColumnStride = GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout);
        }
        ASCENDC_DEBUG_ASSERT((srcColumnStride >= GetTotalRowShape(srcLayout)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src stride in CopyL12UB DN path, source column stride should be no less than source row shape."));
        ASCENDC_DEBUG_ASSERT((dstColumnStride >= GetTotalRowShape(srcLayout)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst stride in CopyL12UB DN path, destination column stride should be no less than source row shape."));
        int64_t srcStride = Std::ceil_division(srcColumnStride - GetTotalRowShape(srcLayout), C0_ELEMENT<srcType>);
        int64_t dstStride = Std::ceil_division(dstColumnStride - GetTotalRowShape(srcLayout), C0_ELEMENT<dstType>);

        CopyCbufToUbufInstr::DataCopy(dst, src, blockCount, blockLen, srcStride, dstStride);
    }
};

class CopyL12UBNZ {
public:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src)
    {
        Execute<trait>(dst, src);
    }

private:
    template <const CopyL12UBTrait& trait, typename T, typename U>
    __aicore__ inline static void Execute(const T& dst, const U& src)
    {
        using srcType = typename U::elementType;
        using dstType = typename T::elementType;

        auto dstLayout = dst.Layout();
        auto srcLayout = src.Layout();

        ASCENDC_DEBUG_ASSERT((GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout) <= 4095),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockCount in CopyL12UB NZ path, blockCount should be no greater than 4095."));
        ASCENDC_DEBUG_ASSERT((GetTotalRowShape(srcLayout) <= 65535), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check blockLen in CopyL12UB NZ path, blockLen should be no greater than 65535."));
        uint16_t blockCount = GetElement<AttrInfo::Shape, AttrInfo::Column, 1>(srcLayout);
        uint32_t blockLen = GetTotalRowShape(srcLayout);
        ASCENDC_DEBUG_ASSERT((GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) % C0_ELEMENT<srcType> == 0 &&
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) / C0_ELEMENT<srcType> >= blockLen),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check src stride in CopyL12UB NZ path, source column stride should be divisible by C0 and no less than blockLen in C0 units."));
        ASCENDC_DEBUG_ASSERT((GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) % C0_ELEMENT<dstType> == 0 &&
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) / C0_ELEMENT<dstType> >= blockLen),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dst stride in CopyL12UB NZ path, destination column stride should be divisible by C0 and no less than blockLen in C0 units."));
        ASCENDC_DEBUG_ASSERT((
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) % C0_ELEMENT<srcType> == 0 &&
            static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) /
            C0_ELEMENT<srcType>) >= static_cast<uint64_t>(blockLen) &&
            static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) /
            C0_ELEMENT<srcType>) - static_cast<uint64_t>(blockLen) <= 65535), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check srcStride in CopyL12UB NZ path, source gap should be no greater than 65535."));
        ASCENDC_DEBUG_ASSERT((
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) % C0_ELEMENT<dstType> == 0 &&
            static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) /
            C0_ELEMENT<dstType>) >= static_cast<uint64_t>(blockLen) &&
            static_cast<uint64_t>(GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) /
            C0_ELEMENT<dstType>) - static_cast<uint64_t>(blockLen) <= 65535), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dstStride in CopyL12UB NZ path, destination gap should be no greater than 65535."));
        int64_t srcStride =
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(srcLayout) / C0_ELEMENT<srcType> - blockLen;
        int64_t dstStride =
            GetElement<AttrInfo::Stride, AttrInfo::Column, 1>(dstLayout) / C0_ELEMENT<dstType> - blockLen;

        CopyCbufToUbufInstr::DataCopy(dst, src, blockCount, blockLen, srcStride, dstStride);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L1_TO_UB_COPY_IMPL_DATA_COPY_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
