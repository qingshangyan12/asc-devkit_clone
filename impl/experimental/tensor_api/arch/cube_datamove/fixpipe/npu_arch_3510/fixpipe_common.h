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
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/fixpipe_common.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file fixpipe_common.h
 * \brief Common utilities and base class for Fixpipe operations on 3510 architecture
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_COMMON_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_COMMON_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/instruction.h"

namespace AscendC {
namespace Te {

/**
 * \brief Layout parameter structure for Fixpipe operations
 */
struct LayoutParams {
    uint32_t nSize;
    uint32_t mSize;
    uint32_t srcStride;
    uint32_t dstStride;
};

/**
 * \brief Common utilities for Fixpipe operations
 *
 * This class provides common functionality shared across different
 * format conversion strategies, reducing code duplication.
 */
class FixpipeCommon {
public:
    /**
     * \brief Calculate layout parameters from source and destination tensors
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \param dst Destination tensor
     * \param src Source tensor
     * \return LayoutParams structure containing calculated parameters
     */
    template <typename T, typename U>
    __aicore__ inline static LayoutParams CalcLayoutParam(const T& dst, const U& src)
    {
        const auto& dstLayout = dst.Layout();
        const auto& srcLayout = src.Layout();

        LayoutParams params;
        params.nSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::COLUMN, 1>(dstLayout));

        params.mSize =
            Std::min(GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(srcLayout)
                         * GetEleFromLayout<decltype(srcLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(srcLayout),
                     GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 0>(dstLayout)
                         * GetEleFromLayout<decltype(dstLayout), AttrInfo::SHAPE, AttrInfo::ROW, 1>(dstLayout));

        params.srcStride =
            GetEleFromLayout<decltype(srcLayout), AttrInfo::STRIDE, AttrInfo::COLUMN, 1>(srcLayout) / FRACTAL_FIXED;

        return params;
    }

    /**
     * \brief Execute GM (Global Memory) branch of data copy
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param nSize N dimension size
     * \param mSize M dimension size
     * \param srcStride Source stride
     * \param dstStride Destination stride
     * \param reluEn ReLU enable flag
     * \param unitFlag Unit flag for fine-grained parallelism
     * \param isChannelSplit Channel split enable flag
     * \param nz2ndEn NZ to ND conversion enable flag
     * \param nz2dnEn NZ to DN conversion enable flag
     */
    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void ExecuteGmBranch(const T& dst, const U& src, uint32_t nSize, uint32_t mSize,
                                                  uint32_t srcStride, uint32_t dstStride, bool reluEn, uint8_t unitFlag,
                                                  bool isChannelSplit, bool nz2ndEn, bool nz2dnEn)
    {
        uint8_t cacheMode = GetCacheModeFromTensor(dst);
        CopyMatrixCcToGm3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, cacheMode, reluEn,
                                                       unitFlag, isChannelSplit, nz2ndEn, nz2dnEn);
    }

    /**
     * \brief Execute UB (Unified Buffer) branch of data copy
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param nSize N dimension size
     * \param mSize M dimension size
     * \param srcStride Source stride
     * \param dstStride Destination stride
     * \param dualDstCtl Dual destination control mode
     * \param reluEn ReLU enable flag
     * \param unitFlag Unit flag for fine-grained parallelism
     * \param subBlockId Sub-block identifier
     * \param nz2ndEn NZ to ND conversion enable flag
     * \param nz2dnEn NZ to DN conversion enable flag
     */
    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void
    ExecuteUbBranch(const T& dst, const U& src, uint32_t nSize, uint32_t mSize, uint32_t srcStride, uint32_t dstStride,
                    uint8_t dualDstCtl, bool reluEn, uint8_t unitFlag, bool subBlockId, bool nz2ndEn, bool nz2dnEn)
    {
        CopyMatrixCcToUb3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, dualDstCtl, reluEn,
                                                       unitFlag, subBlockId, nz2ndEn, nz2dnEn);
    }

    /**
     * \brief Execute data copy with automatic hardware branch selection
     * \tparam trait Fixpipe trait configuration
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param nSize N dimension size
     * \param mSize M dimension size
     * \param srcStride Source stride
     * \param dstStride Destination stride
     * \param nz2ndEn NZ to ND conversion enable flag
     * \param nz2dnEn NZ to DN conversion enable flag
     * \param dualDstCtlAdjust Adjustment function for dual destination control
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void ExecuteHardwareBranch(const T& dst, const U& src, uint32_t nSize, uint32_t mSize,
                                                        uint32_t srcStride, uint32_t dstStride, bool nz2ndEn,
                                                        bool nz2dnEn,
                                                        uint8_t (*dualDstCtlAdjust)(uint8_t, uint32_t&) = nullptr)
    {
        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = 0; // Default value, can be extended with params

        if constexpr (GetHardPos<T>() == Hardware::GM) {
            bool isChannelSplit = trait.enableChannelSplit;
            ExecuteGmBranch<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, reluEn, unitFlag,
                                            isChannelSplit, nz2ndEn, nz2dnEn);
        } else {
            uint8_t dualDstCtl = trait.dualDstCtl;
            if (dualDstCtlAdjust != nullptr) {
                dualDstCtl = dualDstCtlAdjust(dualDstCtl, dstStride);
            }
            bool subBlockId = false;
            ExecuteUbBranch<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, dualDstCtl, reluEn, unitFlag,
                                            subBlockId, nz2ndEn, nz2dnEn);
        }
    }

    /**
     * \brief Common type checking for L0C to output operations
     * \tparam trait Fixpipe trait configuration
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static constexpr void CheckL0C2OutTypes()
    {
        CheckFormat::CheckL0CNZTemplate<U>();
        if constexpr (GetHardPos<T>() == Hardware::GM) {
            CheckDataTypeFor3510::CheckL0C2GmDataType<quantPre, T, U>();
        } else {
            CheckDataTypeFor3510::CheckL0C2UbDataType<quantPre, T, U>();
        }
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FIXPIPE_COMMON_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
