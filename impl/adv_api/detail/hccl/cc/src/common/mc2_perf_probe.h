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
#include "log.h"
namespace Mc2PerfProbe {
struct Sample {
    uint64_t entry, poll, parsed, submitted;
    uint32_t messages, valid;
    int32_t cache, status;
};
static_assert(sizeof(Sample) == 48, "comparison sample layout");
inline Sample& Get() { static thread_local Sample sample; return sample; }
inline uint64_t& Launch() { static thread_local uint64_t launch; return launch; }
inline uint64_t Now() {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) { Get().status = -1; return 0; }
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}
inline void Start() {
    auto& s = Get();
    s.status = 0;
    s.entry = Now();
    s.messages = 0; s.valid = 1; s.cache = -1;
    ++Launch();
}
inline void PollBegin() { auto& s = Get(); s.poll = Now(); s.valid |= 2; }
inline void Receive(uint32_t) {
    auto& s = Get();
    if (++s.messages != 1) s.status = -2;
}
inline void Parsed() { auto& s = Get(); s.parsed = Now(); s.valid |= 4; }
inline void Cache(bool hit) {
    auto& s = Get(); const int value = hit ? 1 : 0;
    s.cache = s.cache == -1 || s.cache == value ? value : 2;
}
inline void Submitted(int status) {
    auto& s = Get(); s.submitted = Now(); s.valid |= 8;
    if (status) s.status = status;
}
struct Totals { uint64_t init, msg, sq; };
// Shared non-inlined body keeps B/C calculation and validation identical.
__attribute__((noinline)) inline Totals Finish(uint32_t status, bool emitLog) {
    auto& s = Get();
    if (status || s.valid != 15 || s.messages != 1) s.status = -3;
    uint64_t init = 0, msg = 0, sq = 0;
    if (s.valid == 15) {
        if (s.entry == 0 || s.poll < s.entry || s.parsed < s.poll || s.submitted < s.parsed) s.status = -4;
        else { init = s.poll-s.entry; msg = s.parsed-s.poll; sq = s.submitted-s.parsed; }
    }
    if (emitLog) HCCL_RUN_INFO("[MC2_PERF] launch=%llu messages=%u valid=%u init_ns=%llu msg_parse_ns=%llu sq_expand_ns=%llu cache=%d status=%d",
        static_cast<unsigned long long>(Launch()), s.messages, s.valid,
        static_cast<unsigned long long>(init), static_cast<unsigned long long>(msg),
        static_cast<unsigned long long>(sq), s.cache, s.status);
    return {init, msg, sq};
}
}
#endif
