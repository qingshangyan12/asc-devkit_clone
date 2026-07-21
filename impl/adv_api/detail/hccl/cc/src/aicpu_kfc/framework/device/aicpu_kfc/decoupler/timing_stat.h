/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_TIMING_STAT_H
#define HCCL_TIMING_STAT_H

#include <ctime>
#include <cstdint>
#include <string>

constexpr uint32_t TIMING_STAT_CAPACITY = 500U;

// 全局统计数据：每行 [start, end]，slot 对应的"函数名+位置"放在 g_timingStatTags 同一下标里。
extern uint64_t g_temp_all_to_all_times[TIMING_STAT_CAPACITY][2];
extern std::string g_timingStatTags[TIMING_STAT_CAPACITY];
extern uint32_t g_timingStatIndex;

inline uint64_t TimingStatNow()
{
    constexpr uint64_t kNsecPerSec = 1000000000ULL;
    struct timespec ts {};
    (void)clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
    return static_cast<uint64_t>(ts.tv_sec) * kNsecPerSec + static_cast<uint64_t>(ts.tv_nsec);
}

// 申请下一个 slot：写入开始时间，记录位置字符串，并返回该 slot 的下标。
// 数组写满后下标越界时直接丢弃，不再覆盖前面的数据。
inline uint32_t TimingStatStart(const char* tag)
{
    uint32_t idx = g_timingStatIndex;
    g_timingStatIndex = idx + 1U;
    if (idx < TIMING_STAT_CAPACITY) {
        g_temp_all_to_all_times[idx][0] = TimingStatNow();
        g_timingStatTags[idx] = (tag == nullptr) ? "" : tag;
    }
    return idx;
}

inline void TimingStatEnd(uint32_t idx)
{
    if (idx < TIMING_STAT_CAPACITY) {
        g_temp_all_to_all_times[idx][1] = TimingStatNow();
    }
}

// 在 timing_stat.cc 中实现：遍历已写入的 slot 全部打印，然后清零。
void TimingStatDumpAndReset();

#define TIMING_STAT_DUMP() TimingStatDumpAndReset()

#endif // HCCL_TIMING_STAT_H
