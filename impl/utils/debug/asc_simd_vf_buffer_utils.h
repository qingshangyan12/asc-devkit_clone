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
 * \file asc_simd_vf_buffer_utils.h
 * \brief SIMD VF debug buffer boundary helpers.
 */
#ifndef IMPL_UTILS_DEBUG_ASC_SIMD_VF_BUFFER_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_SIMD_VF_BUFFER_UTILS_H

#include "impl/utils/debug/asc_debug_types.h"

#if defined(ASCENDC_CPU_DEBUG) && (ASCENDC_CPU_DEBUG == 1)
namespace __asc_simd_vf {
inline bool has_debug_overflow(const BlockVFBufInfo* blockInfo)
{
    return (blockInfo->flag & ASCENDC_SIMD_VF_DEBUG_FLAG_OVERFLOW) != 0;
}

inline void init_debug_buffer(BlockVFBufInfo* blockInfo, uint16_t blockIdx)
{
    blockInfo->magic = ASCENDC_SIMD_VF_MAGIC_NUMBER;
    blockInfo->length = ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE;
    blockInfo->writeLen = 0;
    blockInfo->pidx = 0;
    blockInfo->flag = 0;
    blockInfo->resv1 = 0;
    blockInfo->blockIdx = blockIdx;
    blockInfo->resv2 = 0;
}

inline void mark_debug_overflow(BlockVFBufInfo* blockInfo) { blockInfo->flag |= ASCENDC_SIMD_VF_DEBUG_FLAG_OVERFLOW; }

inline bool reserve_debug_tlv(BlockVFBufInfo* blockInfo, uint32_t tlvLen)
{
    if (has_debug_overflow(blockInfo)) {
        return false;
    }

    const uint32_t capacity = blockInfo->length;
    if (capacity > ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE || blockInfo->writeLen > capacity ||
        tlvLen > capacity - blockInfo->writeLen) {
        mark_debug_overflow(blockInfo);
        return false;
    }
    return true;
}
} // namespace __asc_simd_vf
#else
#include "impl/utils/sys_macros.h"

namespace __asc_simd_vf {
__simd_callee__ inline bool has_debug_overflow(__ubuf__ const BlockVFBufInfo* blockInfo)
{
    return (blockInfo->flag & ASCENDC_SIMD_VF_DEBUG_FLAG_OVERFLOW) != 0;
}

__simd_callee__ inline void init_debug_buffer(__ubuf__ BlockVFBufInfo* blockInfo, uint16_t blockIdx)
{
    blockInfo->magic = ASCENDC_SIMD_VF_MAGIC_NUMBER;
    blockInfo->length = ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE;
    blockInfo->writeLen = 0;
    blockInfo->pidx = 0;
    blockInfo->flag = 0;
    blockInfo->resv1 = 0;
    blockInfo->blockIdx = blockIdx;
    blockInfo->resv2 = 0;
}

__simd_callee__ inline void mark_debug_overflow(__ubuf__ BlockVFBufInfo* blockInfo)
{
    blockInfo->flag |= ASCENDC_SIMD_VF_DEBUG_FLAG_OVERFLOW;
}

__simd_callee__ inline bool reserve_debug_tlv(__ubuf__ BlockVFBufInfo* blockInfo, uint32_t tlvLen)
{
    if (has_debug_overflow(blockInfo)) {
        return false;
    }

    const uint32_t capacity = blockInfo->length;
    if (capacity > ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE || blockInfo->writeLen > capacity ||
        tlvLen > capacity - blockInfo->writeLen) {
        mark_debug_overflow(blockInfo);
        return false;
    }
    return true;
}
} // namespace __asc_simd_vf
#endif

namespace __asc_simd_vf {
constexpr inline uint32_t merge_debug_status(uint32_t gmStatus, uint8_t ubFlag)
{
    if ((ubFlag & ASCENDC_SIMD_VF_DEBUG_FLAG_OVERFLOW) != 0) {
        gmStatus |= ASCENDC_SIMD_VF_DEBUG_GM_STATUS_OVERFLOW;
    }
    return gmStatus;
}
} // namespace __asc_simd_vf

#endif // IMPL_UTILS_DEBUG_ASC_SIMD_VF_BUFFER_UTILS_H
