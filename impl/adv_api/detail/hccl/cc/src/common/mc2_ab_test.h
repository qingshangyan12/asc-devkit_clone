/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_AB_TEST_H
#define MC2_AB_TEST_H
#include <cstdint>
#include <ctime>
#include "log.h"
namespace Mc2AbTest {
inline uint64_t Now()
{
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0;
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}
inline void Report(uint64_t begin, uint64_t end, uint32_t status)
{
    // Sequence bookkeeping and test log are outside the measured interval.
    static thread_local uint64_t launch = 0;
    ++launch;
    const bool valid = begin != 0 && end >= begin;
    HCCL_RUN_INFO("[MC2_AB_TEST] launch=%llu elapsed_ns=%llu valid=%u status=%u",
        static_cast<unsigned long long>(launch),
        static_cast<unsigned long long>(valid ? end - begin : 0), valid ? 1U : 0U, status);
}
}
#endif
