/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_SERVER_STAGE_TIMING_H
#define MC2_SERVER_STAGE_TIMING_H
#include <cstdint>
#include <ctime>
#include "log.h"
namespace Mc2ServerStageTiming {
constexpr uint32_t LOG_SCHEMA_VERSION = 2;
constexpr uint32_t SAMPLES_PER_BATCH = 30;
constexpr uint32_t EXPECTED_MESSAGES_PER_SAMPLE = 1;
constexpr uint64_t NANOSECONDS_PER_SECOND = 1000000000ULL;
constexpr uint32_t SAMPLE_SIZE_BYTES = 48;

constexpr int32_t STATUS_SUCCESS = 0;
constexpr int32_t STATUS_CLOCK_FAILURE = -1;
constexpr int32_t STATUS_MULTIPLE_MESSAGES = -2;
constexpr int32_t STATUS_INCOMPLETE_SAMPLE = -3;
constexpr int32_t STATUS_INVALID_TIMESTAMPS = -4;

constexpr int32_t CACHE_UNKNOWN = -1;
constexpr int32_t CACHE_MISS = 0;
constexpr int32_t CACHE_HIT = 1;
constexpr int32_t CACHE_MIXED = 2;

constexpr uint32_t VALID_ENTRY = 1U << 0;
constexpr uint32_t VALID_POLL = 1U << 1;
constexpr uint32_t VALID_PARSED = 1U << 2;
constexpr uint32_t VALID_SUBMITTED = 1U << 3;
constexpr uint32_t VALID_ALL = VALID_ENTRY | VALID_POLL | VALID_PARSED | VALID_SUBMITTED;

struct Sample {
    uint64_t entry, poll, parsed, submitted;
    uint32_t messages, valid;
    int32_t cache, status;
};
static_assert(sizeof(Sample) == SAMPLE_SIZE_BYTES, "server timing sample layout");
inline Sample& Get()
{
    static thread_local Sample sample;
    return sample;
}
inline uint64_t& Launch()
{
    static thread_local uint64_t launch;
    return launch;
}
inline uint64_t Now()
{
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        Get().status = STATUS_CLOCK_FAILURE;
        return 0;
    }
    return static_cast<uint64_t>(ts.tv_sec) * NANOSECONDS_PER_SECOND + ts.tv_nsec;
}
inline bool& Enabled()
{
    static thread_local bool enabled;
    return enabled;
}
struct Batch {
    uint64_t init, msg, sq, first, last;
    uint32_t count;
    int32_t cache, status;
};
inline Batch& Totals()
{
    static thread_local Batch batch{};
    return batch;
}
inline uintptr_t& Context()
{
    static thread_local uintptr_t context;
    return context;
}
inline void Start(bool enabled, uintptr_t context = 0)
{
    Enabled() = enabled;
    if (!enabled) {
        Totals() = {};
        Context() = 0;
        return;
    }
    if (Context() != context) {
        Totals() = {};
        Context() = context;
    }

    auto& s = Get();
    s.status = STATUS_SUCCESS;
    s.entry = Now();
    s.messages = 0;
    s.valid = VALID_ENTRY;
    s.cache = CACHE_UNKNOWN;
    ++Launch();
}
inline void PollBegin()
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.poll = Now();
    s.valid |= VALID_POLL;
}
inline void Receive(uint32_t)
{
    if (!Enabled())
        return;
    auto& s = Get();
    if (++s.messages != EXPECTED_MESSAGES_PER_SAMPLE)
        s.status = STATUS_MULTIPLE_MESSAGES;
}
inline void Parsed()
{
    if (!Enabled())
        return;
    auto& s = Get();
    if (s.valid & VALID_PARSED)
        s.status = STATUS_MULTIPLE_MESSAGES;
    s.parsed = Now();
    s.valid |= VALID_PARSED;
}
inline void Cache(bool hit)
{
    if (!Enabled())
        return;
    auto& s = Get();
    const int32_t value = hit ? CACHE_HIT : CACHE_MISS;
    s.cache = s.cache == CACHE_UNKNOWN || s.cache == value ? value : CACHE_MIXED;
}
inline void Submitted(int status)
{
    if (!Enabled())
        return;
    auto& s = Get();
    s.submitted = Now();
    s.valid |= VALID_SUBMITTED;
    if (status)
        s.status = status;
}
inline void Finish(uint32_t status)
{
    if (!Enabled())
        return;
    auto& s = Get();
    if (status || s.valid != VALID_ALL || s.messages != EXPECTED_MESSAGES_PER_SAMPLE)
        s.status = STATUS_INCOMPLETE_SAMPLE;
    uint64_t init = 0, msg = 0, sq = 0;
    if (s.valid == VALID_ALL) {
        if (s.entry == 0 || s.poll < s.entry || s.parsed < s.poll || s.submitted < s.parsed)
            s.status = STATUS_INVALID_TIMESTAMPS;
        else {
            init = s.poll - s.entry;
            msg = s.parsed - s.poll;
            sq = s.submitted - s.parsed;
        }
    }
    auto& b = Totals();
    if (b.count == 0) {
        b.first = Launch();
        b.cache = s.cache;
    } else if (b.cache != s.cache)
        b.cache = CACHE_MIXED;
    b.last = Launch();
    b.init += init;
    b.msg += msg;
    b.sq += sq;
    if (s.status)
        b.status = s.status;
    if (++b.count != SAMPLES_PER_BATCH)
        return;
    HCCL_RUN_INFO(
        "[MC2_SERVER_PERF] schema=%u first=%llu last=%llu count=%u init_mean_ns=%.6f msg_parse_mean_ns=%.6f "
        "sq_expand_mean_ns=%.6f cache=%d status=%d",
        LOG_SCHEMA_VERSION, static_cast<unsigned long long>(b.first), static_cast<unsigned long long>(b.last), b.count,
        static_cast<double>(b.init) / SAMPLES_PER_BATCH, static_cast<double>(b.msg) / SAMPLES_PER_BATCH,
        static_cast<double>(b.sq) / SAMPLES_PER_BATCH, b.cache, b.status);
    b = {};
}
} // namespace Mc2ServerStageTiming
#endif
