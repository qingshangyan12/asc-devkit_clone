/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <ctime>

// KFCKernel ABI: each args[] entry holds the argument value (the GM pointer).
extern "C" __attribute__((visibility("default"))) uint32_t CcuTimestampKernel(void* args[])
{
    if (args == nullptr || args[0] == nullptr) {
        return 1;
    }
#if defined(__aarch64__)
    auto* slot = static_cast<volatile uint64_t*>(args[0]);
    // Host uses aclrtMalloc, NOT aclrtMallocCached, for the device mapping.
    // volatile prevents store elimination; the barrier completes publication.
    // This occupies an AICPU worker until the dedicated stream is aborted.
    while (true) {
        timespec now{};
        if (clock_gettime(CLOCK_MONOTONIC_RAW, &now) != 0) {
            return 1;
        }
        *slot = static_cast<uint64_t>(now.tv_sec) * 1000000000ULL + static_cast<uint64_t>(now.tv_nsec);
        asm volatile("dsb st" ::: "memory");
    }
#else
    return 1; // Never silently publish a fake host clock in a device build.
#endif
}
