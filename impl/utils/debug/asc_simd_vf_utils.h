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
 * \file asc_simd_vf_utils.h
 * \brief SIMD VF debug helpers.
 */
#ifndef IMPL_UTILS_DEBUG_ASC_SIMD_VF_UTILS_H
#define IMPL_UTILS_DEBUG_ASC_SIMD_VF_UTILS_H

#include "impl/utils/sys_macros.h"
#include "impl/utils/debug/asc_debug_types.h"

namespace __asc_simd_vf {
__simd_callee__ inline void init_debug_buffer(__ubuf__ BlockVFBufInfo* blockInfo, uint16_t blockIdx)
{
    blockInfo->magic = ASCENDC_SIMD_VF_MAGIC_NUMBER;
    blockInfo->length = ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE;
    blockInfo->writeLen = 0;
    blockInfo->pidx = 0;
    blockInfo->flag = 0;
    blockInfo->finish = 0;
    blockInfo->blockIdx = blockIdx;
    blockInfo->assert = 0;
    blockInfo->resv2 = 0;
    blockInfo->readLen = 0;
}

#if !(defined(ASCENDC_DUMP) && ASCENDC_DUMP == 0) && !defined(ASCENDC_CPU_DEBUG) && defined(__NPU_ARCH__) && \
    __NPU_ARCH__ == 3510 && !defined(__ASC_DISABLE_RESERVED_UBUF__)
__simd_callee__ __ubuf__ inline BlockVFBufInfo* get_printf_ubuf_addr(uint64_t addr, uint16_t blockIdx);

__no_simd_vf_fusion__ __simd_vf__ static inline void asc_finish_flag()
{
    __ubuf__ BlockVFBufInfo* blockInfo = get_printf_ubuf_addr(0, 0);
    blockInfo->finish = 1;
}
#endif

__simd_callee__ inline bool reserve_debug_tlv(__ubuf__ BlockVFBufInfo* blockInfo, uint32_t tlvLen)
{
    if (blockInfo->flag != 0) {
        return false;
    }

    const uint32_t capacity = blockInfo->length;
    if (capacity > ASCENDC_SIMD_VF_PRINTF_UBUF_MAX_SIZE || blockInfo->writeLen > capacity ||
        tlvLen > capacity - blockInfo->writeLen) {
        blockInfo->flag = 1;
        return false;
    }
    return true;
}
} // namespace __asc_simd_vf

#endif // IMPL_UTILS_DEBUG_ASC_SIMD_VF_UTILS_H
