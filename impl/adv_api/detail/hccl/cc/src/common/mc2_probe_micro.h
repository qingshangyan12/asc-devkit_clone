/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_PROBE_MICRO_H
#define MC2_PROBE_MICRO_H
#include "mc2_perf_probe.h"
#include <initializer_list>
namespace Mc2ProbeMicro {
struct Result { uint64_t a, b; uint32_t valid; };
inline void Barrier() { asm volatile("" ::: "memory"); }
inline uint64_t Clock() {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0;
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}
template<bool Probe> inline uint64_t Measure(uint32_t& valid) {
    Barrier();
    const uint64_t begin = Clock();
    Barrier();
    if (Probe) {
        Mc2PerfProbe::Start();
        Mc2PerfProbe::PollBegin();
        Mc2PerfProbe::Receive(0);
        Mc2PerfProbe::Parsed();
        Mc2PerfProbe::Cache(true);
        Mc2PerfProbe::Submitted(0);
        Mc2PerfProbe::Finish(0);
    }
    Barrier();
    const uint64_t end = Clock();
    Barrier();
    if (begin == 0 || end < begin) valid = 0;
    if (Probe && Mc2PerfProbe::Get().status != 0) valid = 0;
    return end >= begin ? end - begin : 0;
}
inline void Pair(uint32_t i, Result& r) {
    r.valid = 1;
    if (i % 2 == 0) { r.a = Measure<false>(r.valid); r.b = Measure<true>(r.valid); }
    else { r.b = Measure<true>(r.valid); r.a = Measure<false>(r.valid); }
}
inline void RunOnce() {
    static thread_local bool done = false;
    if (done) return;
    done = true;
    // Test-only storage. Allocation/initialization and result output are untimed.
    static thread_local Result results[1000];
    for (const uint32_t count : {100U, 1000U}) {
        Result warm;
        for (uint32_t i = 0; i < 20; ++i) Pair(i, warm);
        for (uint32_t i = 0; i < count; ++i) Pair(i, results[i]);
        for (uint32_t i = 0; i < count; ++i) {
            const auto& r = results[i];
            HCCL_RUN_INFO("[MC2_PROBE_MICRO] protocol=1 count=%u index=%u a_ns=%llu b_ns=%llu valid=%u",
                count, i, static_cast<unsigned long long>(r.a), static_cast<unsigned long long>(r.b), r.valid);
        }
    }
}
}
#endif
