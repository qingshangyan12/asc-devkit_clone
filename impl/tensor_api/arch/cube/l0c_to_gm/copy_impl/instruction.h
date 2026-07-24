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
    "impl/tensor_api/arch/cube/l0c_to_gm/copy_impl/instruction.h is an internal header file and must not be used directly. Functions or variables defined in this file maybe removed in the future. Please use "#include "tensor_api/tensor.h"" and use public functions or variables defined in interface headers files."
#define ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#define UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif

/*!
 * \file instruction.h
 * \brief
 */
#ifndef IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_INSTRUCTION_H
#define IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_INSTRUCTION_H

#include "impl/tensor_api/arch/utils/arch_utils.h"

namespace AscendC {
namespace Te {

class CopyMatrixCcToGmInstr {
public:
    template <QuantMode_t quantPre, typename T, typename U, typename... Params>
    __aicore__ inline static void DataCopy(const T& dst, const U& src, const Params&... params)
    {
        CopyMatrixCcToGm<quantPre>(dst.Data().Get(), src.Data().Get(), params...);
    }

private:
    template <QuantMode_t quantPre, typename T, typename U>
    __aicore__ inline static void CopyMatrixCcToGm(
        __gm__ T* dst, __cc__ U* src, uint32_t nSize, uint32_t mSize, uint32_t srcStride, uint32_t dstStride,
        uint8_t cacheMode, bool reluEn, uint8_t unitFlag, bool isChannelSplit, bool nz2ndEn, bool nz2dnEn)
    {
        if ASCEND_IS_AIV {
            return;
        }
        ASCENDC_DEBUG_ASSERT((!isChannelSplit || (Std::is_same_v<T, float> && Std::is_same_v<U, float>)),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check isChannelSplit in CopyL0C2GM instruction, isChannelSplit requires both dst and src type to be float."));
        ASCENDC_DEBUG_ASSERT((!isChannelSplit || (!nz2ndEn && !nz2dnEn)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check isChannelSplit in CopyL0C2GM instruction, isChannelSplit can only be used on NZ output path."));
        ASCENDC_DEBUG_ASSERT((!isChannelSplit || (nSize >= 1 && nSize <= 4095 && nSize % 8 == 0)),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check nSize in CopyL0C2GM instruction, nSize should be in range [1, 4095] and divisible by 8 when isChannelSplit is true."));
        ASCENDC_DEBUG_ASSERT((isChannelSplit || !nz2ndEn || (nSize >= 1 && nSize <= 4095)),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check nSize in CopyL0C2GM NZ2ND instruction, nSize should be in range [1, 4095]."));
        ASCENDC_DEBUG_ASSERT((isChannelSplit || !nz2dnEn || (mSize >= 1 && mSize <= 32767)),
            KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check mSize in CopyL0C2GM NZ2DN instruction, mSize should be in range [1, 32767]."));
        ASCENDC_DEBUG_ASSERT((isChannelSplit || nz2ndEn || nz2dnEn ||
            (nSize >= 1 && nSize <= 4095 && nSize % 16 == 0)), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check nSize in CopyL0C2GM NZ instruction, nSize should be in range [1, 4095] and divisible by 16."));
        ASCENDC_DEBUG_ASSERT((mSize >= 1 && mSize <= 65535), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check mSize in CopyL0C2GM instruction, mSize should be in range [1, 65535]."));
        ASCENDC_DEBUG_ASSERT((dstStride > 0), KERNEL_LOG_INTERNAL(KERNEL_ERROR,
            "Failed to check dstStride in CopyL0C2GM instruction, dstStride should be greater than 0."));

        asc_copy_l0c2gm(
            dst, src, static_cast<uint16_t>(nSize), static_cast<uint16_t>(mSize), dstStride,
            static_cast<uint16_t>(srcStride), cacheMode, 0, unitFlag, static_cast<uint64_t>(quantPre),
            static_cast<uint8_t>(reluEn), isChannelSplit, nz2ndEn, static_cast<uint64_t>(QuantMode_post::NoConv), 0,
            false, 0, false, false, false, nz2dnEn);
    }
};

} // namespace Te
} // namespace AscendC

#endif // IMPL_TENSOR_API_ARCH_CUBE_L0C_TO_GM_COPY_IMPL_INSTRUCTION_H

#if defined(UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC)
#undef ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS
#undef UNDEF_ASCENDC_TENSOR_API_INCLUDE_COMPILER_INTERNAL_HEADERS_ASCENDC
#endif
