/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_THREAD_CONFIG_COMPAT_H
#define MC2_THREAD_CONFIG_COMPAT_H

#include <cstddef>
#include <cstdint>
#include "hccl_res.h"

namespace mc2_thread_config {
// Private mirror of HCOMM ThreadConfig v1; keep public copied headers unchanged.
struct CommAbiHeader {
    uint32_t version;
    uint32_t magicWord;
    uint32_t size;
    uint32_t reserved;
};
struct ThreadConfig {
    CommAbiHeader header;
    uint16_t notifyNumPerThread;
    uint8_t reserved[14];
};
enum ThreadType { THREAD_TYPE_INVALID = -1, THREAD_TYPE_TS = 0 };
constexpr size_t MC2_PROFILING_RESERVED_INDEX = 13U;
static_assert(sizeof(ThreadConfig) == 32 && sizeof(CommAbiHeader) == 16, "ThreadConfig v1 ABI changed");
static_assert(alignof(ThreadConfig) == 4 && sizeof(ThreadType) == 4, "ThreadConfig ABI alignment changed");
static_assert(offsetof(ThreadConfig, notifyNumPerThread) == 16, "ThreadConfig notify offset changed");
static_assert(
    offsetof(ThreadConfig, reserved) + MC2_PROFILING_RESERVED_INDEX == 31, "MC2 profiling byte offset changed");

inline ThreadConfig MakeConfig(uint16_t notifyNum)
{
    ThreadConfig config{};
    config.header = {1U, 0x0f0f0f2fU, sizeof(ThreadConfig), 0U};
    config.notifyNumPerThread = notifyNum;
    // Paired with HCOMM PR5028: only value 1 opts into deferred MC2 profiling.
    config.reserved[MC2_PROFILING_RESERVED_INDEX] = 1U;
    return config;
}
} // namespace mc2_thread_config

HcclResult Mc2ThreadAcquireWithConfig(
    HcclComm comm, CommEngine engine, uint32_t threadNum, uint32_t notifyNum, ThreadHandle* threads);

#endif
