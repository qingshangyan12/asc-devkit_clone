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
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/strategies/nz_conversion.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file nz_conversion.h
 * \brief Unified NZ format conversion strategy for Fixpipe operations
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_NZ_CONVERSION_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_NZ_CONVERSION_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/fixpipe_common.h"
#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/format_policies.h"

namespace AscendC {
namespace Te {

/**
 * \brief Unified NZ format conversion strategy
 * \tparam DstFormat Destination format (NZ, ND, DN)
 */
template <Format3510 DstFormat>
class NzConversionStrategy {
    using Policy = NzFormatPolicy<DstFormat>;

public:
    /**
     * \brief Execute NZ format conversion
     * \tparam trait Fixpipe trait configuration
     * \tparam quantPre Quantization mode
     * \tparam T Destination tensor type
     * \tparam U Source tensor type
     * \param dst Destination tensor
     * \param src Source tensor
     * \param params Fixpipe parameters
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void Execute(const T& dst, const U& src, const FixpipeParams& params)
    {
        Policy::template CheckTemplate<trait, T>();
        FixpipeCommon::CheckL0C2OutTypes<trait, quantPre, T, U>();

        auto layoutParams = FixpipeCommon::CalcLayoutParam(dst, src);
        layoutParams.dstStride = CalcDstStride(dst);

        ExecuteDataCopy<trait, quantPre, T, U>(dst, src, layoutParams, params);
    }

private:
    /**
     * \brief Calculate destination stride based on format policy
     */
    template <typename T>
    __aicore__ inline static uint32_t CalcDstStride(const T& dst)
    {
        const auto& dstLayout = dst.Layout();
        return GetEleFromLayout<decltype(dstLayout), AttrInfo::STRIDE, Policy::strideAttr, 1>(dstLayout);
    }

    /**
     * \brief Execute data copy with automatic hardware branch selection
     */
    template <const FixpipeTrait& trait, QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void ExecuteDataCopy(const T& dst, const U& src, const LayoutParams& layoutParams,
                                                  const FixpipeParams& params)
    {
        uint32_t nSize = layoutParams.nSize;
        uint32_t mSize = layoutParams.mSize;
        uint32_t srcStride = layoutParams.srcStride;
        uint32_t dstStride = layoutParams.dstStride;

        bool reluEn = trait.enableRelu;
        uint8_t unitFlag = params.unitFlag;

        if constexpr (GetHardPos<T>() == Hardware::GM) {
            uint8_t cacheMode = GetCacheModeFromTensor(dst);
            bool isChannelSplit = trait.enableChannelSplit;
            CopyMatrixCcToGm3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, cacheMode,
                                                           reluEn, unitFlag, isChannelSplit, Policy::nz2ndEn,
                                                           Policy::nz2dnEn);
        } else {
            uint8_t dualDstCtl = Policy::AdjustDualDstCtl(trait.dualDstCtl, dstStride);
            bool subBlockId = false;
            CopyMatrixCcToUb3510::DataCopy<quantPre, T, U>(dst, src, nSize, mSize, srcStride, dstStride, dualDstCtl,
                                                           reluEn, unitFlag, subBlockId, Policy::nz2ndEn,
                                                           Policy::nz2dnEn);
        }
    }
};

/**
 * \brief Type aliases for backward compatibility
 */
using NzToNzStrategy = NzConversionStrategy<Format3510::NZ>;
using NzToNdStrategy = NzConversionStrategy<Format3510::ND>;
using NzToDnStrategy = NzConversionStrategy<Format3510::DN>;

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_STRATEGIES_NZ_CONVERSION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
