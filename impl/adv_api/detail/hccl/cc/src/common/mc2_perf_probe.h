/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_PERF_PROBE_H
#define MC2_PERF_PROBE_H

#include <cstdint>
#ifdef MC2_PERF_ACCEPTANCE
#include <ctime>
#include "log.h"
#endif

// Test-only buffered probe. Default build has no clocks, storage or output.
// Durations share the AICPU thread's CLOCK_MONOTONIC domain; never subtract
// these timestamps from Host or AICore timestamps.
namespace Mc2PerfProbe {
#ifdef MC2_PERF_ACCEPTANCE
struct Record {
    const char* metric;
    uint64_t begin;
    uint64_t end;
    uint32_t seq;
    int cache;
    int status;
};
struct State {
    Record records[2048];
    uint32_t count = 0;
    uint32_t dropped = 0;
    uint64_t entry = 0;
    uint64_t receive = 0;
    uint64_t launch = 0;
    uint32_t seq = 0;
    int cache = -1; // -1 unknown/disabled; 0 confirmed miss; 1 confirmed hit; 2 mixed
    bool firstCondition = true;
};
inline State& Get() { static thread_local State state; return state; }
inline uint64_t Now()
{
    timespec value{};
    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) return 0;
    return static_cast<uint64_t>(value.tv_sec) * 1000000000ULL + value.tv_nsec;
}
inline void Add(const char* metric, uint64_t begin, uint64_t end, int status = 0)
{
    auto& s = Get();
    if (s.count == 2048) { ++s.dropped; return; }
    s.records[s.count++] = {metric, begin, end, s.seq, s.cache, status};
}
inline void Start()
{
    auto& s = Get();
    s.count = 0; s.dropped = 0; s.cache = -1; s.firstCondition = true;
    s.seq = 0; s.receive = 0; ++s.launch; s.entry = Now();
}
inline void Ready() { Add("server_ready", Get().entry, Now()); }
inline void Receive(uint32_t seq)
{
    auto& s = Get(); s.receive = Now(); s.seq = seq; s.cache = -1;
}
inline void Cache(bool hit)
{
    auto& s = Get();
    const int value = hit ? 1 : 0;
    s.cache = s.cache == -1 || s.cache == value ? value : 2;
}
inline void Condition(int status)
{
    auto& s = Get(); const auto end = Now();
    if (s.firstCondition) {
        Add("server_entry_to_condition", s.entry, end, status);
        s.firstCondition = false;
    }
    Add("server_receive_to_condition", s.receive, end, status);
}
inline void Submitted(int status) { Add("server_receive_to_submit", Get().receive, Now(), status); }
inline void Dump()
{
    auto& s = Get();
    for (uint32_t i = 0; i < s.count; ++i) {
        const auto& r = s.records[i];
        HCCL_RUN_INFO("[MC2_PERF] launch=%llu seq=%u metric=%s begin_ns=%llu end_ns=%llu cache=%d status=%d",
            static_cast<unsigned long long>(s.launch), r.seq, r.metric,
            static_cast<unsigned long long>(r.begin), static_cast<unsigned long long>(r.end), r.cache, r.status);
    }
    HCCL_RUN_INFO("[MC2_PERF_END] launch=%llu records=%u dropped=%u",
        static_cast<unsigned long long>(s.launch), s.count, s.dropped);
}
#else
inline void Start() {}
inline void Ready() {}
inline void Receive(uint32_t) {}
inline void Cache(bool) {}
inline void Condition(int) {}
inline void Submitted(int) {}
inline void Dump() {}
#endif
struct KernelScope {
    KernelScope() { Start(); }
    ~KernelScope() { Dump(); }
};
} // namespace Mc2PerfProbe
#endif
