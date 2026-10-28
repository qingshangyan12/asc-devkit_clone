/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_CCU_PERF_PROBE_H
#define MC2_CCU_PERF_PROBE_H

#include <cstdint>
#include <ctime>
#include <unistd.h>
#include "log.h"

// Host submission timing only: runtime opt-in, thread-local records, dump
// after submission and clear enablement on exit. No KFC execution probes.
namespace Mc2CcuPerfProbe {
enum Metric { CTX_LOAD, PARAM_LOAD, KERNEL_SUBMIT, METRIC_COUNT };
constexpr const char* NAMES[] = {"host_ctx_load", "host_param_load", "host_kernel_submit"};
struct Record {
    const char* metric;
    uint64_t begin;
    uint64_t end;
    int status;
};
struct State {
    bool clockError = false;
    int status = -1;
    uint32_t valid = 0;
    uint32_t count = 0;
    uint32_t dropped = 0;
    uint64_t launch = 0;
    uint64_t entry = 0;
    uint64_t poll = 0;
    uint64_t parsed = 0;
    Record records[METRIC_COUNT]{};
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
    if (end < begin)
        s.clockError = true;
    if (s.count == METRIC_COUNT) {
        ++s.dropped;
        return;
    }
    s.records[s.count++] = {metric, begin, end, status};
}
inline void Start()
{
    auto& s = Get();
    s.clockError = false;
    s.status = -1;
    s.valid = 0;
    s.count = 0;
    s.dropped = 0;
    s.poll = 0;
    s.parsed = 0;
    ++s.launch;
    s.entry = Now();
}
// Host counterpart of entry-to-poll: the resource context is now loaded.
inline void PollBegin()
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.poll = Now();
    Add(NAMES[CTX_LOAD], s.entry, s.poll);
    s.valid |= 1U << CTX_LOAD;
}
// Host parameter-loading boundary: close the preceding interval and start
// submission at one timestamp. This does not observe KFC message processing.
inline void Parsed()
{
    if (!Enabled())
        return;
    auto& s = Get();
    if ((s.valid & (1U << CTX_LOAD)) == 0)
        return;
    s.parsed = Now();
    Add(NAMES[PARAM_LOAD], s.poll, s.parsed);
    s.valid |= 1U << PARAM_LOAD;
}
inline void Submitted(int status)
{
    if (!Enabled())
        return;
    auto& s = Get();
    if ((s.valid & (1U << PARAM_LOAD)) == 0)
        return; // Keep the default failure status if Parsed was skipped.
    Add(NAMES[KERNEL_SUBMIT], s.parsed, Now(), status);
    s.valid |= 1U << KERNEL_SUBMIT;
    s.status = status;
}
inline void Dump()
{
    const auto& s = Get();
    const int status = s.clockError ? -2 : s.status;
    for (uint32_t i = 0; i < s.count; ++i) {
        const auto& r = s.records[i];
        // Fixed compatibility fields for schema 2; no message/cache collection.
        HCCL_RUN_INFO(
            "[MC2_CCU_PERF] schema=2 pid=%ld launch=%llu seq=0 metric=%s begin_ns=%llu end_ns=%llu cache=-1 status=%d",
            static_cast<long>(getpid()), static_cast<unsigned long long>(s.launch), r.metric,
            static_cast<unsigned long long>(r.begin), static_cast<unsigned long long>(r.end),
            status != 0 ? status : r.status);
    }
    HCCL_RUN_INFO(
        "[MC2_CCU_PERF_END] schema=2 pid=%ld launch=%llu records=%u dropped=%u valid=%u status=%d",
        static_cast<long>(getpid()), static_cast<unsigned long long>(s.launch), s.count, s.dropped, s.valid, status);
}
struct KernelScope {
    explicit KernelScope(bool enabled)
    {
        Enabled() = enabled;
        if (enabled)
            Start();
    }
    ~KernelScope()
    {
        if (Enabled())
            Dump();
        Enabled() = false;
    }
    KernelScope(const KernelScope&) = delete;
    KernelScope& operator=(const KernelScope&) = delete;
};
} // namespace Mc2CcuPerfProbe
#endif
