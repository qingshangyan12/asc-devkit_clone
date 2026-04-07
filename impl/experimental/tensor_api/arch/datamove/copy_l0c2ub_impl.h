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
    "impl/tensor_api/arch/datamove/copy_l0c2ub_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file copy_l0c2ub_impl.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_L0C2UB_IMPL_H
#define IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_L0C2UB_IMPL_H

#include "include/experimental/tensor_api/utils/utils.h"
#include "impl/experimental/tensor_api/arch/utils/copy_fixpipe_utils.h"
#include "impl/experimental/tensor_api/arch/datamove/copy_fixpipe_instrs.h"

namespace AscendC {
namespace Te {

class CopyL0C2UBImpl3510 {
public:
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src, const FixpipeParams& params)
    {
        CheckTemplate<trait, quantPre, T, U>();
        SetRegisterImpl<T, U>(dst, src);
        DataCopyImpl<trait, quantPre, T, U>(dst, src, params);
    }

    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void Run(const T& dst, const U& src, uint64_t quant, const FixpipeParams& params)
    {
        CheckTemplate<trait, quantPre, T, U>();
        SetRegisterImpl<T, U>(dst, src, quant);
        DataCopyImpl<trait, quantPre, T, U>(dst, src, params);
    }

private:
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        if constexpr (IsNDFormat<T>::value) {
            CheckFormat::CheckNDTemplate<T>();
        } else if constexpr (IsDNFormat<T>::value) {
            CheckFormat::CheckDNTemplate<T>();
        } else if constexpr (IsNZFormat<T>::value || IsL0cNZFormat<T>::value) {
            CheckFormat::CheckFixpipeNZTemplate<T, trait.enableChannelSplit>();
        }

        CheckFormat::CheckL0CNZTemplate<U>();
        CheckDataTypeFor3510::CheckL0C2UbDataType<quantPre, T, U>();
    }

    template <typename T, typename U>
    __aicore__ inline static void SetRegisterImpl(const T& /*dst*/, const U& /*src*/)
    {
        if constexpr (IsNDFormat<T>::value) {
            constexpr uint32_t ndNum = 1;
            constexpr uint32_t srcNdStride = 0;
            constexpr uint32_t dstNdStride = 0;
            SetRegister3510::SetRegister(ndNum, dstNdStride, srcNdStride);
        } else if constexpr (IsDNFormat<T>::value) {
            // 寄存器 constexpr 常量配置
            constexpr uint32_t dnNum = 1;
            constexpr uint32_t dstDnMatrixStride = 0;
            constexpr uint32_t srcNzMatrixStride = 0;
            constexpr uint32_t srcNzC0Stride = 1;
            SetRegister3510::SetRegister(dnNum, dstDnMatrixStride, srcNzMatrixStride, srcNzC0Stride);
        }
    }

    template <typename T, typename U>
    __aicore__ inline static void SetRegisterImpl(const T& /*dst*/, const U& /*src*/, uint64_t quant)
    {
        if constexpr (IsNDFormat<T>::value) {
            constexpr uint32_t ndNum = 1;
            constexpr uint32_t srcNdStride = 0;
            constexpr uint32_t dstNdStride = 0;
            SetRegister3510::SetRegister(quant, ndNum, dstNdStride, srcNdStride);
        } else if constexpr (IsDNFormat<T>::value) {
            // 寄存器 constexpr 常量配置
            constexpr uint32_t dnNum = 1;
            constexpr uint32_t dstDnMatrixStride = 0;
            constexpr uint32_t srcNzMatrixStride = 0;
            constexpr uint32_t srcNzC0Stride = 1;
            SetRegister3510::SetRegister(quant, dnNum, dstDnMatrixStride, srcNzMatrixStride, srcNzC0Stride);
        }
    }

    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void DataCopyImpl(const T& dst, const U& src, const FixpipeParams& params)
    {
        const auto& dstLayout = dst.Layout();
        const auto& srcLayout = src.Layout();

        uint32_t nSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout));
        uint32_t mSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout));
        uint32_t srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;
        uint32_t dstStride = 0;

        if constexpr (IsNDFormat<T>::value) {
            dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::ROW, 1>(dstLayout);

        } else {
            dstStride = GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(dstLayout);
        }

        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = params.unitFlag;
        bool nz2ndEn = IsNDFormat<T>::value;
        bool nz2dnEn = IsDNFormat<T>::value;

        bool isChannelSplit = trait.enableChannelSplit;

        uint8_t dualDstCtl = trait.dualDstCtl;
        bool subBlockId = false;
        CopyMatrixCcToUb3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, dualDstCtl, reluEn,
                                                       unitFlag, subBlockId, isChannelSplit, nz2ndEn, nz2dnEn);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_DATAMOVE_COPY_L0C2UB_IMPL_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif