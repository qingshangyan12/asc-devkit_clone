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
struct Sample {
    uint64_t entry, poll, parsed, submitted;
    uint32_t messages, valid;
    int32_t cache, status;
};
static_assert(sizeof(Sample) == 48, "server timing sample layout");
inline Sample& Get() { static thread_local Sample sample; return sample; }
inline uint64_t& Launch() { static thread_local uint64_t launch; return launch; }
inline uint64_t Now() {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) { Get().status = -1; return 0; }
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}
inline bool& Enabled() { static thread_local bool enabled; return enabled; }
struct Batch {
    uint64_t init, msg, sq, first, last;
    uint32_t count;
    int32_t cache, status;
};
inline Batch& Totals() { static thread_local Batch batch{}; return batch; }
inline uintptr_t& Context() { static thread_local uintptr_t context; return context; }
inline void Start(bool enabled, uintptr_t context = 0) {
    Enabled() = enabled;
    if (!enabled) { Totals() = {}; Context() = 0; return; }
    if (Context() != context) { Totals() = {}; Context() = context; }

    auto& s = Get();
    s.status = 0;
    s.entry = Now();
    s.messages = 0; s.valid = 1; s.cache = -1;
    ++Launch();
}
inline void PollBegin() { if (!Enabled()) return; auto& s = Get(); s.poll = Now(); s.valid |= 2; }
inline void Receive(uint32_t) { if (!Enabled()) return;
    auto& s = Get();
    if (++s.messages != 1) s.status = -2;
}
inline void Parsed() { if (!Enabled()) return; auto& s = Get(); if (s.valid & 4) s.status = -2; s.parsed = Now(); s.valid |= 4; }
inline void Cache(bool hit) { if (!Enabled()) return;
    auto& s = Get(); const int value = hit ? 1 : 0;
    s.cache = s.cache == -1 || s.cache == value ? value : 2;
}
inline void Submitted(int status) { if (!Enabled()) return;
    auto& s = Get(); s.submitted = Now(); s.valid |= 8;
    if (status) s.status = status;
}
inline void Finish(uint32_t status) { if (!Enabled()) return;
    auto& s = Get();
    if (status || s.valid != 15 || s.messages != 1) s.status = -3;
    uint64_t init = 0, msg = 0, sq = 0;
    if (s.valid == 15) {
        if (s.entry == 0 || s.poll < s.entry || s.parsed < s.poll || s.submitted < s.parsed) s.status = -4;
        else { init = s.poll-s.entry; msg = s.parsed-s.poll; sq = s.submitted-s.parsed; }
    }
    auto& b = Totals();
    if (b.count == 0) { b.first = Launch(); b.cache = s.cache; }
    else if (b.cache != s.cache) b.cache = 2;
    b.last = Launch();
    b.init += init; b.msg += msg; b.sq += sq;
    if (s.status) b.status = s.status;
    if (++b.count != 30) return;
    HCCL_RUN_INFO("[MC2_SERVER_PERF] schema=2 first=%llu last=%llu count=30 init_mean_ns=%.6f msg_parse_mean_ns=%.6f sq_expand_mean_ns=%.6f cache=%d status=%d",
        static_cast<unsigned long long>(b.first), static_cast<unsigned long long>(b.last),
        static_cast<double>(b.init)/30, static_cast<double>(b.msg)/30,
        static_cast<double>(b.sq)/30, b.cache, b.status);
    b = {};
}
}
#endif
