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
    "impl/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/format_policies.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file format_policies.h
 * \brief Format conversion policy templates for Fixpipe operations
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FORMAT_POLICIES_H
#define IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FORMAT_POLICIES_H

#include "impl/experimental/tensor_api/arch/cube_datamove/fixpipe/npu_arch_3510/instruction.h"

namespace AscendC {
namespace Te {

/**
 * \brief NZ format conversion policy template
 * \tparam DstFormat Destination format (NZ, ND, DN)
 */
template <Format3510 DstFormat>
struct NzFormatPolicy;

/**
 * \brief NZ to NZ format conversion policy
 */
template <>
struct NzFormatPolicy<Format3510::NZ> {
    static constexpr AttrInfo strideAttr = AttrInfo::COLUMN;
    static constexpr bool nz2ndEn = false;
    static constexpr bool nz2dnEn = false;

    template <const FixpipeTrait& trait, typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckFixpipeNZTemplate<T, trait.enableChannelSplit>();
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t&)
    {
        return dualDstCtl;
    }
};

/**
 * \brief NZ to ND format conversion policy
 */
template <>
struct NzFormatPolicy<Format3510::ND> {
    static constexpr AttrInfo strideAttr = AttrInfo::ROW;
    static constexpr bool nz2ndEn = true;
    static constexpr bool nz2dnEn = false;

    template <const FixpipeTrait& trait, typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckNDTemplate<T>();
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t& dstStride)
    {
        if (dualDstCtl == DUAL_DST_SPLIT_N) {
            dstStride = dstStride >> 1;
        }
        return dualDstCtl;
    }
};

/**
 * \brief NZ to DN format conversion policy
 */
template <>
struct NzFormatPolicy<Format3510::DN> {
    static constexpr AttrInfo strideAttr = AttrInfo::COLUMN;
    static constexpr bool nz2ndEn = false;
    static constexpr bool nz2dnEn = true;

    template <const FixpipeTrait& trait, typename T>
    __aicore__ inline static constexpr void CheckTemplate()
    {
        CheckFormat::CheckDNTemplate<T>();
    }

    __aicore__ inline static uint8_t AdjustDualDstCtl(uint8_t dualDstCtl, uint32_t&)
    {
        return dualDstCtl;
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_DATAMOVE_FIXPIPE_NPU_ARCH_3510_FORMAT_POLICIES_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
