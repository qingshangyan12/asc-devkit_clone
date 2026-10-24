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
#include <ctime>
#include <string>
#include <utility>
#include "log.h"
// Runtime opt-in buffered probe. Disabled launches do not read clocks or record/output samples.
// Durations share the AICPU thread's CLOCK_MONOTONIC domain; never subtract
// these timestamps from Host or AICore timestamps.
namespace Mc2PerfProbe {
struct Record {
    const char* metric;
    uint64_t begin;
    uint64_t end;
    uint32_t seq;
    int cache;
    int status;
};
using ClearCallback = int (*)(void*, void*, const std::string&);
struct State {
    bool clockError = false;
    bool paired = false;
    ClearCallback clear = nullptr;
    void* manager = nullptr;
    void* comm = nullptr;
    std::string clearTag;
    Record records[2048];
    uint32_t count = 0;
    uint32_t dropped = 0;
    uint64_t entry = 0;
    uint64_t poll = 0;
    uint64_t parsed = 0;
    uint64_t launch = 0;
    uint32_t seq = 0;
    int cache = -1; // -1 unknown/disabled; 0 confirmed miss; 1 confirmed hit; 2 mixed
};
inline bool& Enabled()
{
    static thread_local bool enabled = false;
    return enabled;
}
inline State& Get()
{
    static thread_local State state;
    return state;
}
inline uint64_t Now()
{
    timespec value{};
    if (clock_gettime(CLOCK_MONOTONIC, &value) != 0) {
        Get().clockError = true;
        return 0;
    }
    return static_cast<uint64_t>(value.tv_sec) * 1000000000ULL + value.tv_nsec;
}
inline void Add(const char* metric, uint64_t begin, uint64_t end, int status = 0)
{
    auto& s = Get();
    if (s.count == 2048) {
        ++s.dropped;
        return;
    }
    s.records[s.count++] = {metric, begin, end, s.seq, s.cache, status};
}
inline void Start()
{
    auto& s = Get();
    s.clockError = false;
    s.clear = nullptr;
    s.clearTag.clear();
    s.count = 0;
    s.dropped = 0;
    s.cache = -1;
    s.seq = 0;
    s.poll = 0;
    s.parsed = 0;
    ++s.launch;
    s.entry = Now();
}
inline void PollBegin()
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.poll = Now();
    Add("init", s.entry, s.poll);
}
inline void Receive(uint32_t seq)
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.seq = seq;
    s.cache = -1;
}
inline void Parsed()
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.parsed = Now();
    Add("msg_parse", s.poll, s.parsed);
}
inline void Cache(bool hit)
{
    if (!Enabled())
        return;
    auto& s = Get();
    const int value = hit ? 1 : 0;
    s.cache = s.cache == -1 || s.cache == value ? value : 2;
}
inline void Submitted(int status)
{
    if (!Enabled())
        return;
    auto& s = Get();
    const auto end = Now();
    Add("sq_expand", s.parsed, end, status);
    s.poll = end; // subsequent messages start from the previous submission boundary
}
inline bool Paired() { return Enabled() && Get().paired; }
inline void DeferClear(void* manager, void* comm, std::string&& tag, ClearCallback callback)
{
    if (!Paired())
        return;
    auto& s = Get();
    s.manager = manager;
    s.comm = comm;
    s.clearTag = std::move(tag);
    s.clear = callback;
}
// Call only after normal dispatcher completion: Client has waited for communication
// before Finalize. Cache destruction is outside all three measured intervals.
inline int Finish()
{
    if (!Paired())
        return 0;
    auto& s = Get();
    const bool cleared = s.clear != nullptr;
    const int status = cleared ? s.clear(s.manager, s.comm, s.clearTag) : 0;
    HCCL_RUN_INFO(
        "[MC2_PERF_CACHE_CONTROL] launch=%llu policy=paired cleared=%d status=%d",
        static_cast<unsigned long long>(s.launch), static_cast<int>(cleared), status);
    s.clear = nullptr;
    s.clearTag.clear();
    return status;
}
inline void Dump()
{
    auto& s = Get();
    for (uint32_t i = 0; i < s.count; ++i) {
        const auto& r = s.records[i];
        HCCL_RUN_INFO(
            "[MC2_PERF] launch=%llu seq=%u metric=%s begin_ns=%llu end_ns=%llu cache=%d status=%d",
            static_cast<unsigned long long>(s.launch), r.seq, r.metric, static_cast<unsigned long long>(r.begin),
            static_cast<unsigned long long>(r.end), r.cache, s.clockError ? -1 : r.status);
    }
    HCCL_RUN_INFO(
        "[MC2_PERF_END] launch=%llu records=%u dropped=%u", static_cast<unsigned long long>(s.launch), s.count,
        s.dropped);
}
struct KernelScope {
    explicit KernelScope(bool enabled, bool paired = false)
    {
        Enabled() = enabled;
        if (enabled) {
            Start();
            Get().paired = paired;
        }
    }
    ~KernelScope()
    {
        if (Enabled()) {
            Dump();
        }
        Enabled() = false;
    }
    KernelScope(const KernelScope&) = delete;
    KernelScope& operator=(const KernelScope&) = delete;
};
} // namespace Mc2PerfProbe
#endif
