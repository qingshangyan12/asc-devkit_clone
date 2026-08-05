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
 * \file asc_aicore_dump_common.h
 * \brief
 */
#ifndef IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_COMMON_H
#define IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_COMMON_H

#include <cstdint>
#include "impl/utils/common_types.h"

namespace __asc_aicore {
constexpr uint16_t ASC_L0C_DUMP_BLOCK_SIDE = 16U;
constexpr uint32_t ASC_L0C_DUMP_BLOCK_ELEMENTS = ASC_L0C_DUMP_BLOCK_SIDE * ASC_L0C_DUMP_BLOCK_SIDE;
constexpr uint32_t ASC_L0C_DUMP_BLOCK_BYTES = ASC_L0C_DUMP_BLOCK_ELEMENTS * sizeof(float);
constexpr uint32_t ASC_L0C_DUMP_GM_ALIGN_BYTES = 32U;

struct L0cDumpCopyParams {
    uint16_t blockCount;
    uint16_t nSize;
    uint16_t mSize;
    uint32_t physicalBytes;
};

__aicore__ constexpr inline uint32_t get_l0c_dump_block_count(const uint32_t dumpBytes)
{
    return dumpBytes == 0U ? 0U : 1U + (dumpBytes - 1U) / ASC_L0C_DUMP_BLOCK_BYTES;
}

__aicore__ constexpr inline L0cDumpCopyParams get_l0c_dump_copy_params(const uint32_t dumpBytes)
{
    const uint16_t blockCount = static_cast<uint16_t>(get_l0c_dump_block_count(dumpBytes));
    return L0cDumpCopyParams{
        blockCount, static_cast<uint16_t>(blockCount * ASC_L0C_DUMP_BLOCK_SIDE), ASC_L0C_DUMP_BLOCK_SIDE,
        blockCount * ASC_L0C_DUMP_BLOCK_BYTES};
}

__aicore__ constexpr inline uint32_t get_l0c_dump_max_copy_bytes(const uint32_t dumpBytes)
{
    const L0cDumpCopyParams params = get_l0c_dump_copy_params(dumpBytes);
    return params.blockCount == 0U ? 0U : params.physicalBytes + ASC_L0C_DUMP_GM_ALIGN_BYTES - 1U;
}

__aicore__ constexpr inline uint32_t get_l0c_dump_reservation_bytes(const uint32_t dumpBytes)
{
    const uint32_t maxCopyBytes = get_l0c_dump_max_copy_bytes(dumpBytes);
    return maxCopyBytes > dumpBytes ? maxCopyBytes : dumpBytes;
}

__aicore__ constexpr inline uint32_t get_l0c_dump_gm_align_offset(const uint64_t dstAddr)
{
    return static_cast<uint32_t>(
        (ASC_L0C_DUMP_GM_ALIGN_BYTES - dstAddr % ASC_L0C_DUMP_GM_ALIGN_BYTES) % ASC_L0C_DUMP_GM_ALIGN_BYTES);
}
} // namespace __asc_aicore

#endif // IMPL_UTILS_DEBUG_ASC_AICORE_DUMP_COMMON_H
