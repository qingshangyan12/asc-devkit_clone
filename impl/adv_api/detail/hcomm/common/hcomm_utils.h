/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file hcomm_util.h
 * \brief
 */
#ifndef IMPL_HCOMM_UTIL_H
#define IMPL_HCOMM_UTIL_H

namespace AscendC {

__aicore__ inline void cacheWriteThrough(__gm__ uint8_t* sourceAddr, uint64_t length)
{
    __gm__ uint8_t* start = (__gm__ uint8_t*)((uint64_t)sourceAddr / CACHE_LINE_SIZE * CACHE_LINE_SIZE);
    __gm__ uint8_t* end = (__gm__ uint8_t*)(((uint64_t)sourceAddr + length) / CACHE_LINE_SIZE * CACHE_LINE_SIZE);
    GlobalTensor<uint8_t> global;
    global.SetGlobalBuffer(start);
    for (uint32_t i = 0; i < end - start; i += CACHE_LINE_SIZE) {
        DataCacheCleanAndInvalid<uint8_t, CacheLine::SINGLE_CACHE_LINE, DcciDst::CACHELINE_OUT>(global[i]);
    }
}
} // namespace AscendC

#endif
