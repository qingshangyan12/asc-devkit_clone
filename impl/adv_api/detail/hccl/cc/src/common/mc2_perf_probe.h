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
#include "mc2_hcomm_perf_schema.h"
extern "C" int32_t Mc2HcommPerfBegin(uint32_t, uint32_t);
extern "C" int32_t Mc2HcommPerfEnd(uint32_t, uint64_t*, uint32_t);

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
enum DetailMetric {
    PRE_WAIT,
    WAIT_SUBMIT,
    PRE_ALGORITHM,
    ALGORITHM,
    PRE_RECORD,
    RECORD_SUBMIT,
    CACHE_POLICY,
    CACHE_LOOKUP,
    EXECUTOR,
    CAPTURE_START,
    CAPTURE_END,
    CACHE_EXECUTE,
    BATCH_END,
    DETAIL_COUNT
};
constexpr const char* DETAIL_NAMES[] = {
    "server_prepare_before_wait",
    "server_ccore_wait_submit",
    "server_prepare_before_algorithm",
    "server_algorithm",
    "server_prepare_before_record",
    "server_ccore_record_submit",
    "server_cache_policy",
    "server_cache_lookup",
    "server_executor",
    "server_cache_capture_start",
    "server_cache_capture_end",
    "server_cache_execute",
    "server_batch_end"};
struct State {
    bool clockError = false;
    bool detail = false;
    bool statsStarted = false;
    bool statsEnded = false;
    int statsStatus = -1;
    uint32_t stages = 0;
    uint64_t boundary = 0;
    uint64_t details[DETAIL_COUNT]{};
    uint64_t hcomm[HcommPerf::METRIC_COUNT]{};
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
    s.statsStarted = false;
    s.statsEnded = false;
    s.statsStatus = -1;
    s.stages = 0;
    for (auto& v : s.details)
        v = 0;
    for (auto& v : s.hcomm)
        v = 0;
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
    Add("server_entry_to_poll", s.entry, s.poll);
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
    Add("server_poll_to_parsed", s.poll, s.parsed);
    s.boundary = s.parsed;
    s.statsStatus = Mc2HcommPerfBegin(HcommPerf::ABI_VERSION, s.detail ? 2 : 1);
    s.statsStarted = s.statsStatus == 0;
}
inline void Mark(DetailMetric metric)
{
    if (!Enabled() || !Get().detail)
        return;
    auto& s = Get();
    const auto end = Now();
    if (!s.boundary || end < s.boundary)
        s.clockError = true;
    s.details[metric] += end - s.boundary;
    s.boundary = end;
    ++s.stages;
}
struct DetailScope {
    DetailMetric metric;
    uint64_t start;
    explicit DetailScope(DetailMetric m) : metric(m), start(Enabled() && Get().detail ? Now() : 0) {}
    ~DetailScope()
    {
        if (start) {
            const auto end = Now();
            if (end < start)
                Get().clockError = true;
            else
                Get().details[metric] += end - start;
        }
    }
    DetailScope(const DetailScope&) = delete;
    DetailScope& operator=(const DetailScope&) = delete;
};
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
    Add("server_parsed_to_submit", s.parsed, end, status);
    if (s.detail) {
        s.details[RECORD_SUBMIT] += end - s.boundary;
        ++s.stages;
    }
    if (s.statsStarted)
        s.statsStatus = Mc2HcommPerfEnd(HcommPerf::ABI_VERSION, s.hcomm, HcommPerf::METRIC_COUNT);
    s.statsEnded = true;
    if (status || s.clockError || end < s.boundary || (s.detail && s.stages != 6))
        s.statsStatus = -1;
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
            static_cast<unsigned long long>(r.end), r.cache, r.status);
    }
    HCCL_RUN_INFO(
        "[MC2_PERF_END] launch=%llu records=%u dropped=%u", static_cast<unsigned long long>(s.launch), s.count,
        s.dropped);
    const uint32_t level = s.detail ? 2 : 1;
    const int status = s.statsEnded ? s.statsStatus : -1;
    const uint32_t n = s.detail ? HcommPerf::METRIC_COUNT : HcommPerf::COUNT_METRICS;
    for (uint32_t i = 0; i < n; ++i) {
        const char* unit = i >= HcommPerf::COUNT_METRICS ?
                               "ns" :
                               (i == HcommPerf::RTSQ_COPY_BYTES || i == HcommPerf::CAPTURE_BYTES ? "bytes" : "count");
        HCCL_RUN_INFO(
            "[MC2_PERF_STAT] launch=%llu seq=%u metric=%s value=%llu unit=%s cache=%d level=%u status=%d",
            static_cast<unsigned long long>(s.launch), s.seq, HcommPerf::NAMES[i],
            static_cast<unsigned long long>(s.hcomm[i]), unit, s.cache, level, status);
    }
    if (s.detail)
        for (uint32_t i = 0; i < DETAIL_COUNT; ++i) {
            HCCL_RUN_INFO(
                "[MC2_PERF_STAT] launch=%llu seq=%u metric=%s value=%llu unit=ns cache=%d level=%u status=%d",
                static_cast<unsigned long long>(s.launch), s.seq, DETAIL_NAMES[i],
                static_cast<unsigned long long>(s.details[i]), s.cache, level, status);
        }
    HCCL_RUN_INFO(
        "[MC2_PERF_STAT_END] launch=%llu records=%u level=%u status=%d", static_cast<unsigned long long>(s.launch),
        n + (s.detail ? DETAIL_COUNT : 0), level, status);
}
struct KernelScope {
    explicit KernelScope(bool enabled, bool paired = false, bool detail = false)
    {
        Enabled() = enabled;
        if (enabled) {
            Start();
            Get().paired = paired;
            Get().detail = detail;
        }
    }
    ~KernelScope()
    {
        if (Enabled()) {
            auto& s = Get();
            if (s.statsStarted && !s.statsEnded)
                Mc2HcommPerfEnd(HcommPerf::ABI_VERSION, nullptr, 0);
            Dump();
        }
        Enabled() = false;
    }
    KernelScope(const KernelScope&) = delete;
    KernelScope& operator=(const KernelScope&) = delete;
};
} // namespace Mc2PerfProbe
#endif
