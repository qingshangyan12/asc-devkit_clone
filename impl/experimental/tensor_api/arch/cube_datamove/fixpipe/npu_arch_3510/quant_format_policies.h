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
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/quant_format_policies.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file quant_format_policies.h
 * \brief Format conversion policy templates for quantized Fixpipe operations
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_QUANT_FORMAT_POLICIES_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_QUANT_FORMAT_POLICIES_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/instruction.h"

namespace AscendC {
namespace Te {

/**
 * \brief Quantized NZ format conversion policy template
 * \tparam DstFormat Destination format (NZ, ND, DN)
 */
template <Format3510 DstFormat>
struct QuantNzFormatPolicy;

/**
 * \brief Quantized NZ to NZ format conversion policy
 */
template <>
struct QuantNzFormatPolicy<Format3510::NZ> {
    static constexpr AttrInfo strideAttr = AttrInfo::COLUMN;
    static constexpr bool nz2ndEn = false;
    static constexpr bool nz2dnEn = false;
    static constexpr bool supportChannelSplit = true;

    template <typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckFixpipeNZTemplate<T, false>();
    }

    template <typename V>
    __aicore__ inline static void SetRegisterWithQuant(const V& quant)
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        SetRegister3510::SetRegister(quant, ndNum, dstNDStride, srcNDStride);
    }

    __aicore__ inline static void SetRegisterWithoutQuant()
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        SetRegister3510::SetRegister(ndNum, dstNDStride, srcNDStride);
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t&)
    {
        return dualDstCtl;
    }

    __aicore__ inline static auto MakeVectorDstCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * CBURST_NUM_3510));
    }

    __aicore__ inline static auto MakeVectorSrcCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * CBURST_NUM_3510));
    }
};

/**
 * \brief Quantized NZ to ND format conversion policy
 */
template <>
struct QuantNzFormatPolicy<Format3510::ND> {
    static constexpr AttrInfo strideAttr = AttrInfo::ROW;
    static constexpr bool nz2ndEn = true;
    static constexpr bool nz2dnEn = false;
    static constexpr bool supportChannelSplit = false;

    template <typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckNDTemplate<T>();
    }

    template <typename V>
    __aicore__ inline static void SetRegisterWithQuant(const V& quant)
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        SetRegister3510::SetRegister(quant, ndNum, dstNDStride, srcNDStride);
    }

    __aicore__ inline static void SetRegisterWithoutQuant()
    {
        uint32_t ndNum = 1;
        uint32_t srcNDStride = 0;
        uint32_t dstNDStride = 0;
        SetRegister3510::SetRegister(ndNum, dstNDStride, srcNDStride);
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t& dstStride)
    {
        if (dualDstCtl == DUAL_DST_SPLIT_N) {
            dstStride = dstStride >> 1;
        }
        return dualDstCtl;
    }

    __aicore__ inline static auto MakeVectorDstCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * MAIN_LOOP_N_SIZE_3510));
    }

    __aicore__ inline static auto MakeVectorSrcCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * CBURST_NUM_3510));
    }
};

/**
 * \brief Quantized NZ to DN format conversion policy
 */
template <>
struct QuantNzFormatPolicy<Format3510::DN> {
    static constexpr AttrInfo strideAttr = AttrInfo::COLUMN;
    static constexpr bool nz2ndEn = false;
    static constexpr bool nz2dnEn = true;
    static constexpr bool supportChannelSplit = false;

    template <typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckDNTemplate<T>();
    }

    template <typename V>
    __aicore__ inline static void SetRegisterWithQuant(const V& quant)
    {
        uint32_t dnNum = 1;
        uint32_t srcNZMatrixStride = 0;
        uint32_t dstDNMatrixStride = 0;
        uint32_t srcNZC0Stride = 1;
        SetRegister3510::SetRegister(quant, dnNum, dstDNMatrixStride, srcNZMatrixStride, srcNZC0Stride);
    }

    __aicore__ inline static void SetRegisterWithoutQuant()
    {
        uint32_t dnNum = 1;
        uint32_t srcNZMatrixStride = 0;
        uint32_t dstDNMatrixStride = 0;
        uint32_t srcNZC0Stride = 1;
        SetRegister3510::SetRegister(dnNum, dstDNMatrixStride, srcNZMatrixStride, srcNZC0Stride);
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t&)
    {
        return dualDstCtl;
    }

    __aicore__ inline static auto MakeVectorDstCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * MAIN_LOOP_N_SIZE_3510));
    }

    __aicore__ inline static auto MakeVectorSrcCoord(uint16_t i)
    {
        return MakeCoord(MakeCoord(0, 0), MakeCoord(0, i * CBURST_NUM_3510));
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_QUANT_FORMAT_POLICIES_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
