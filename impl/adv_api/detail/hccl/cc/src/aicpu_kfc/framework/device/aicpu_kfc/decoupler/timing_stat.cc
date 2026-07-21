/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "timing_stat.h"

#include "log.h"

uint64_t g_temp_all_to_all_times[TIMING_STAT_CAPACITY][2] = {{0U, 0U}};
std::string g_timingStatTags[TIMING_STAT_CAPACITY];
uint32_t g_timingStatIndex = 0U;

void TimingStatDumpAndReset()
{
    uint32_t total = g_timingStatIndex;
    if (total > TIMING_STAT_CAPACITY) {
        total = TIMING_STAT_CAPACITY;
    }
    for (uint32_t i = 0U; i < total; ++i) {
        const uint64_t start = g_temp_all_to_all_times[i][0];
        const uint64_t end = g_temp_all_to_all_times[i][1];
        HCCL_RUN_INFO(
            "[TIMING_STAT][%u] %s startTime: %llu, endTime: %llu, useTime: %llu", i, g_timingStatTags[i].c_str(),
            static_cast<unsigned long long>(start), static_cast<unsigned long long>(end),
            static_cast<unsigned long long>(end - start));
        g_timingStatTags[i].clear();
    }
    g_timingStatIndex = 0U;
}
