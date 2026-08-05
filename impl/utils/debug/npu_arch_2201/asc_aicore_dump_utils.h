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
#ifndef IMPL_UTILS_DEBUG_NPU_ARCH_2201_ASC_AICORE_DUMP_UTILS_H
#define IMPL_UTILS_DEBUG_NPU_ARCH_2201_ASC_AICORE_DUMP_UTILS_H

#include "impl/utils/debug/asc_debug_utils.h"
#include "impl/utils/debug/asc_aicore_dump_common.h"
namespace __asc_aicore {

__aicore__ inline void get_matrix_copy_param(
    uint32_t dumpSize, uint16_t& n, uint16_t& m, uint16_t& dstStrideDstD, uint16_t& srcStride)
{
    const L0cDumpCopyParams copyParams = get_l0c_dump_copy_params(dumpSize);
    n = copyParams.nSize;
    m = copyParams.mSize;
    srcStride = copyParams.mSize;
    dstStrideDstD = ASC_L0C_DUMP_BLOCK_BYTES / ASC_ONE_DATABLOCK_SIZE;
}

template <typename T>
__aicore__ inline uint32_t mem_copy_cbuf_to_gm_impl(__gm__ T* dst, __cc__ T* src, const uint32_t& len)
{
#if defined(__DAV_CUBE__)
    uint16_t nSize, mSize, dstStrideDstD, srcStride;
    get_matrix_copy_param(len, nSize, mSize, dstStrideDstD, srcStride);
    const uint32_t dstAlignOffset = get_l0c_dump_gm_align_offset(reinterpret_cast<uint64_t>(dst));
    __gm__ float* alignedDst = reinterpret_cast<__gm__ float*>(reinterpret_cast<uint64_t>(dst) + dstAlignOffset);
    copy_matrix_cc_to_gm(
        alignedDst, (__cc__ float*)src, 0, nSize, mSize, dstStrideDstD, srcStride, 0, QuantMode_t::NoQuant,
        static_cast<uint8_t>(false), false, false);
    sync_all();
    asc_entire_dcci(reinterpret_cast<__gm__ uint64_t*>(alignedDst));
    if (dstAlignOffset == 0U) {
        return 0;
    }
    // dst is below alignedDst, so a forward copy safely compacts the overlapping staging range.
    return mem_copy_gm_to_gm(
        reinterpret_cast<__gm__ uint8_t*>(dst), reinterpret_cast<__gm__ const uint8_t*>(alignedDst), len);
#else
    return 1;
#endif
}

template <typename T>
__aicore__ inline uint32_t mem_copy_l1buf_to_gm_impl(__gm__ T* dst, __cbuf__ T* src, const uint32_t& len)
{
#if defined(__DAV_CUBE__)
    copy_cbuf_to_gm(reinterpret_cast<__gm__ void*>(dst), reinterpret_cast<__cbuf__ void*>(src), 0, 1, len, 0, 0);
    return 0;
#else
    return 1;
#endif
}

template <typename T>
__aicore__ inline uint32_t mem_copy_ub_to_gm_impl(__gm__ T* dst, __ubuf__ T* src, const uint32_t& len)
{
#if defined(__DAV_VEC__)
    copy_ubuf_to_gm(reinterpret_cast<__gm__ void*>(dst), reinterpret_cast<__ubuf__ void*>(src), 0, 1, len, 0, 0);
    return 0;
#else
    return 1;
#endif
}

template <typename T>
__aicore__ inline uint32_t mem_copy_abuf_to_gm_impl(__gm__ T* dst, __ca__ T* src, const uint32_t alignDumpBytes)
{
    return 1;
}

template <typename T>
__aicore__ inline uint32_t mem_copy_bbuf_to_gm_impl(__gm__ T* dst, __cb__ T* src, const uint32_t alignDumpBytes)
{
    return 1;
}

template <typename T>
__aicore__ inline uint32_t mem_copy_biasbuf_to_gm_impl(__gm__ T* dst, __biasbuf__ T* src, const uint32_t alignDumpBytes)
{
    return 1;
}

template <typename T>
__aicore__ inline uint32_t mem_copy_fbuf_to_gm_impl(__gm__ T* dst, __fbuf__ T* src, const uint32_t alignDumpBytes)
{
    return 1;
}

} // namespace __asc_aicore

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__
#endif

#endif // IMPL_UTILS_DEBUG_NPU_ARCH_2002_ASC_AICORE_PRINTF_UTILS_H
