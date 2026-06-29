/**
* Copyright (c) 2026 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file asc_aicore_dump_utils.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_5102_ASC_AICORE_DUMP_UTILS_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_5102_ASC_AICORE_DUMP_UTILS_H

#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/sys_macros.h"
#include "kernel_macros.h"

namespace __asc_aicore {
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_OFFSET = 0x078;
constexpr uint32_t ASC_DEBUG_BUS_CTRL_OFFSET = 0x100;
constexpr uint32_t ASC_DEBUG_BUS_ADDR_LOW_OFFSET = 0x108;
constexpr uint32_t ASC_DEBUG_BUS_MODEL_BASE_OFFSET = 0x10c;
constexpr uint32_t ASC_DEBUG_BUS_DATA_OFFSET = 0x110;
constexpr uint32_t ASC_DEBUG_BUS_BUSY_OFFSET = 0x130;
constexpr uint32_t ASC_DEBUG_BUS_UNLOCK_VALUE = 0x19101920;
constexpr uint32_t ASC_DEBUG_BUS_READ_MASK = 0x2;
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_WORDS = 8;
constexpr uint32_t ASC_DEBUG_BUS_CHUNK_BYTES = ASC_DEBUG_BUS_CHUNK_WORDS * sizeof(uint32_t);

template<typename T>
__aicore__ inline uint32_t mem_copy_cbuf_to_gm_impl(__gm__ T* dst, __cc__ T* src, const uint32_t& dumpSize)
{
    return 1;
}


template<typename T>
__aicore__ inline uint32_t mem_copy_l1buf_to_gm_impl(
    __gm__ T* dst, __cbuf__ T* src, const uint32_t alignDumpBytes) { return 1; }

template<typename T>
__aicore__ inline uint32_t mem_copy_ub_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
    constexpr uint8_t byte_32_align = 32;
    constexpr uint32_t blockCount = 1;
    uint32_t blockLen = len;
    constexpr uint32_t dstStride = 0;
    constexpr uint32_t srcStride = 0;

    uint32_t unitOfBytes = byte_32_align;
    uint32_t burstLen = blockLen * unitOfBytes;
    uint32_t srcStride1 = srcStride * byte_32_align + burstLen;
    srcStride1 = div_ceil(srcStride1, byte_32_align) * byte_32_align;
    uint64_t dstStride1 = dstStride * unitOfBytes + burstLen;
    copy_ubuf_to_gm_align_v2((__gm__ void*)dst, (__ubuf__ void*)src, 0, blockCount, burstLen, 0, dstStride1, srcStride1);
    return 0;
}

template<typename T>
__aicore__ inline uint32_t mem_copy_abuf_to_gm_impl(
    __gm__ T* dst, __ca__ T* src, const uint32_t alignDumpBytes) { return 1; }

template<typename T>
__aicore__ inline uint32_t mem_copy_bbuf_to_gm_impl(
    __gm__ T* dst, __cb__ T* src, const uint32_t alignDumpBytes) { return 1; }

} // namespace __asc_aicore

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_5102_ASC_AICORE_DUMP_UTILS_H