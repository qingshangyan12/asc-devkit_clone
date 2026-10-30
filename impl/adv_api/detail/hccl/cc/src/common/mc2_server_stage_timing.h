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
// MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
#include <cstdio>
// MC2_CACHE_DETAIL_DIAG END

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
// MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
// Diagnostic schema 1: fixed order shared with the benchmark parser. Durations are
// inclusive; child phases must not be added to their parents. No per-call logging.
inline bool& DetailEnabled()
{
    static thread_local bool enabled = false;
    return enabled;
}
enum DetailPhase : uint32_t {
    DFX_REGISTER,
    CCORE_WAIT,
    KERNEL_DISPATCH,
    CCORE_RECORD,
    BATCH_START,
    BATCH_END,
    CACHE_POLICY,
    CACHE_LOOKUP,
    CAPTURE_CREATE,
    CAPTURE_BEGIN,
    MISS_EXECUTE,
    CAPTURE_END,
    CACHE_PUBLISH,
    HIT_REPLAY,
    VAR_RESTORE,
    EXECUTOR_CREATE,
    ALGORITHM_EXECUTE,
    BATCH_FLUSH,
    BATCH_FINISH,
    DETAIL_PHASE_COUNT
};
// Native 64-bit integer arithmetic; no allocation, atomics or floating-point
// work in the measured path. Accumulate directly into the 30-launch batch.
struct DetailBatch {
    uint64_t ns[DETAIL_PHASE_COUNT];
    uint64_t calls[DETAIL_PHASE_COUNT];
};
inline DetailBatch& DetailTotals()
{
    static thread_local DetailBatch totals{};
    return totals;
}
// Backend schema 3: four execution contexts, 56 uint64_t words per context.
constexpr uint32_t BACKEND_WORDS = 56;
constexpr uint32_t BACKEND_ZONES = 4;
enum BackendZone : uint32_t { EXECUTE, FLUSH, SEAL, REPLAY };
struct BackendBatch {
    uint64_t words[BACKEND_WORDS * BACKEND_ZONES];
    uint64_t bindings, unavailable;
};
inline BackendBatch& BackendTotals()
{
    static thread_local BackendBatch batch{};
    return batch;
}
// Sparse hex serialization is performed only after the message loop exits.
// Bounded buffer covers all 224 index:value pairs even at UINT64_MAX.
inline bool FormatBackend(char* text, size_t capacity)
{
    size_t used = 0;
    bool first = true;
    for (uint32_t i = 0; i < BACKEND_WORDS * BACKEND_ZONES; ++i) {
        if (BackendTotals().words[i] == 0) continue;
        const int count = snprintf(text + used, capacity - used, "%s%x:%llx", first ? "" : ",", i,
            static_cast<unsigned long long>(BackendTotals().words[i]));
        if (count < 0 || static_cast<size_t>(count) >= capacity - used) return false;
        used += static_cast<size_t>(count);
        first = false;
    }
    if (first) {
        if (capacity < 2) return false;
        text[0] = '-'; text[1] = '\0';
    }
    return true;
}
class DetailScope {
public:
    explicit DetailScope(DetailPhase phase) : phase_(phase), start_(DetailEnabled() ? Now() : 0) {}
    ~DetailScope() { Stop(); }
    DetailScope(const DetailScope&) = delete;
    DetailScope& operator=(const DetailScope&) = delete;
    void Stop()
    {
        if (start_ == 0)
            return;
        const uint64_t end = Now();
        if (end >= start_) {
            auto& totals = DetailTotals();
            totals.ns[phase_] += end - start_;
            ++totals.calls[phase_];
        } else {
            Get().status = STATUS_INVALID_TIMESTAMPS;
        }
        start_ = 0;
    }
private:
    DetailPhase phase_;
    uint64_t start_;
};
// MC2_CACHE_DETAIL_DIAG END
// Descending order; equal values represent distinct samples and must be retained.
struct Top3 {
    uint64_t first, second, third;
};
inline void UpdateTop3(Top3& top, uint64_t value)
{
    if (value <= top.third)
        return;
    if (value > top.first) {
        top.third = top.second;
        top.second = top.first;
        top.first = value;
    } else if (value > top.second) {
        top.third = top.second;
        top.second = value;
    } else {
        top.third = value;
    }
}
struct Batch {
    uint64_t init, msg, sq, first, last;
    Top3 initTop, msgTop, sqTop;
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
inline void Start(bool enabled, uintptr_t context = 0, bool detailEnabled = false)
{
    const bool modeChanged = DetailEnabled() != detailEnabled;
    DetailEnabled() = enabled && detailEnabled;
    Enabled() = enabled;
    if (!enabled) {
        Totals() = {};
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        DetailTotals() = {};
        BackendTotals() = {};
        // MC2_CACHE_DETAIL_DIAG END
        Context() = 0;
        return;
    }
    if (Context() != context || modeChanged) {
        Totals() = {};
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        DetailTotals() = {};
        BackendTotals() = {};
        // MC2_CACHE_DETAIL_DIAG END
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
    // Aggregate after all stage timestamps; no sorting or per-sample logging.
    UpdateTop3(b.initTop, init);
    UpdateTop3(b.msgTop, msg);
    UpdateTop3(b.sqTop, sq);
    if (s.status)
        b.status = s.status;
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    auto& detail = DetailTotals();
    // MC2_CACHE_DETAIL_DIAG END
    if (++b.count != SAMPLES_PER_BATCH)
        return;
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    if (!DetailEnabled()) {
        HCCL_RUN_INFO("[MC2_SERVER_PERF] schema=%u first=%llu last=%llu count=%u init_mean_ns=%.6f "
            "msg_parse_mean_ns=%.6f sq_expand_mean_ns=%.6f cache=%d status=%d init_top3_ns=%llu,%llu,%llu msg_parse_top3_ns=%llu,%llu,%llu sq_expand_top3_ns=%llu,%llu,%llu",
            LOG_SCHEMA_VERSION, static_cast<unsigned long long>(b.first), static_cast<unsigned long long>(b.last),
            b.count, static_cast<double>(b.init) / SAMPLES_PER_BATCH,
            static_cast<double>(b.msg) / SAMPLES_PER_BATCH, static_cast<double>(b.sq) / SAMPLES_PER_BATCH,
            b.cache, b.status,
        static_cast<unsigned long long>(b.initTop.first), static_cast<unsigned long long>(b.initTop.second), static_cast<unsigned long long>(b.initTop.third), static_cast<unsigned long long>(b.msgTop.first), static_cast<unsigned long long>(b.msgTop.second), static_cast<unsigned long long>(b.msgTop.third), static_cast<unsigned long long>(b.sqTop.first), static_cast<unsigned long long>(b.sqTop.second), static_cast<unsigned long long>(b.sqTop.third));
        b = {};
        return;
    }
    char backendText[6144];
    const bool backendValid = FormatBackend(backendText, sizeof(backendText));
    if (!backendValid) { backendText[0] = '-'; backendText[1] = '\0'; }
    // MC2_CACHE_DETAIL_DIAG END

    HCCL_RUN_INFO(
        "[MC2_SERVER_PERF] schema=%u first=%llu last=%llu count=%u init_mean_ns=%.6f msg_parse_mean_ns=%.6f "
        "sq_expand_mean_ns=%.6f cache=%d status=%d init_top3_ns=%llu,%llu,%llu msg_parse_top3_ns=%llu,%llu,%llu sq_expand_top3_ns=%llu,%llu,%llu"
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        " detail_schema=2 detail_ns=%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu "
        "detail_calls=%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu,%llu "
        "backend_schema=3 backend_bindings=%llu backend_unavailable=%llu backend_valid=%u backend=%s"
        // MC2_CACHE_DETAIL_DIAG END
        ,
        LOG_SCHEMA_VERSION, static_cast<unsigned long long>(b.first), static_cast<unsigned long long>(b.last), b.count,
        static_cast<double>(b.init) / SAMPLES_PER_BATCH, static_cast<double>(b.msg) / SAMPLES_PER_BATCH,
        static_cast<double>(b.sq) / SAMPLES_PER_BATCH, b.cache, b.status,
        static_cast<unsigned long long>(b.initTop.first), static_cast<unsigned long long>(b.initTop.second), static_cast<unsigned long long>(b.initTop.third), static_cast<unsigned long long>(b.msgTop.first), static_cast<unsigned long long>(b.msgTop.second), static_cast<unsigned long long>(b.msgTop.third), static_cast<unsigned long long>(b.sqTop.first), static_cast<unsigned long long>(b.sqTop.second), static_cast<unsigned long long>(b.sqTop.third)
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        ,
        static_cast<unsigned long long>(detail.ns[DFX_REGISTER]), static_cast<unsigned long long>(detail.ns[CCORE_WAIT]),
        static_cast<unsigned long long>(detail.ns[KERNEL_DISPATCH]), static_cast<unsigned long long>(detail.ns[CCORE_RECORD]),
        static_cast<unsigned long long>(detail.ns[BATCH_START]), static_cast<unsigned long long>(detail.ns[BATCH_END]),
        static_cast<unsigned long long>(detail.ns[CACHE_POLICY]), static_cast<unsigned long long>(detail.ns[CACHE_LOOKUP]),
        static_cast<unsigned long long>(detail.ns[CAPTURE_CREATE]), static_cast<unsigned long long>(detail.ns[CAPTURE_BEGIN]),
        static_cast<unsigned long long>(detail.ns[MISS_EXECUTE]), static_cast<unsigned long long>(detail.ns[CAPTURE_END]),
        static_cast<unsigned long long>(detail.ns[CACHE_PUBLISH]), static_cast<unsigned long long>(detail.ns[HIT_REPLAY]),
        static_cast<unsigned long long>(detail.ns[VAR_RESTORE]), static_cast<unsigned long long>(detail.ns[EXECUTOR_CREATE]),
        static_cast<unsigned long long>(detail.ns[ALGORITHM_EXECUTE]),
        static_cast<unsigned long long>(detail.ns[BATCH_FLUSH]), static_cast<unsigned long long>(detail.ns[BATCH_FINISH]),
        static_cast<unsigned long long>(detail.calls[DFX_REGISTER]), static_cast<unsigned long long>(detail.calls[CCORE_WAIT]),
        static_cast<unsigned long long>(detail.calls[KERNEL_DISPATCH]), static_cast<unsigned long long>(detail.calls[CCORE_RECORD]),
        static_cast<unsigned long long>(detail.calls[BATCH_START]), static_cast<unsigned long long>(detail.calls[BATCH_END]),
        static_cast<unsigned long long>(detail.calls[CACHE_POLICY]), static_cast<unsigned long long>(detail.calls[CACHE_LOOKUP]),
        static_cast<unsigned long long>(detail.calls[CAPTURE_CREATE]), static_cast<unsigned long long>(detail.calls[CAPTURE_BEGIN]),
        static_cast<unsigned long long>(detail.calls[MISS_EXECUTE]), static_cast<unsigned long long>(detail.calls[CAPTURE_END]),
        static_cast<unsigned long long>(detail.calls[CACHE_PUBLISH]), static_cast<unsigned long long>(detail.calls[HIT_REPLAY]),
        static_cast<unsigned long long>(detail.calls[VAR_RESTORE]), static_cast<unsigned long long>(detail.calls[EXECUTOR_CREATE]),
        static_cast<unsigned long long>(detail.calls[ALGORITHM_EXECUTE]),
        static_cast<unsigned long long>(detail.calls[BATCH_FLUSH]), static_cast<unsigned long long>(detail.calls[BATCH_FINISH]),
        static_cast<unsigned long long>(BackendTotals().bindings),
        static_cast<unsigned long long>(BackendTotals().unavailable), static_cast<unsigned>(backendValid), backendText
        // MC2_CACHE_DETAIL_DIAG END
    );
    b = {};
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    detail = {};
    BackendTotals() = {};
    // MC2_CACHE_DETAIL_DIAG END
}
} // namespace Mc2ServerStageTiming
#endif
