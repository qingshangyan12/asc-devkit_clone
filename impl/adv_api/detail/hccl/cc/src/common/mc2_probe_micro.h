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
namespace Mc2ProbeMicro {
struct Result { uint64_t a, b, c; uint32_t valid; };
inline void Barrier() { asm volatile("" ::: "memory"); }
inline uint64_t Clock() {
    timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) return 0;
    return static_cast<uint64_t>(ts.tv_sec) * 1000000000ULL + ts.tv_nsec;
}
template<int Mode> inline uint64_t Measure(uint32_t& valid) {
    Mc2PerfProbe::Totals totals;
    Barrier();
    const uint64_t begin = Clock();
    Barrier();
    if (Mode != 0) {
        Mc2PerfProbe::Start();
        Mc2PerfProbe::PollBegin();
        Mc2PerfProbe::Receive(0);
        Mc2PerfProbe::Parsed();
        Mc2PerfProbe::Cache(true);
        Mc2PerfProbe::Submitted(0);
        totals = Mc2PerfProbe::Finish(0, Mode == 2);
        // Force computed B/C totals to exist before the ending timestamp.
        asm volatile("" : "+m"(totals) :: "memory");
    }
    Barrier();
    const uint64_t end = Clock();
    Barrier();
    if (Mode != 0) {
        // Observable use is outside the timed interval and identical for B/C.
        static thread_local volatile uint64_t sink;
        sink = totals.init ^ totals.msg ^ totals.sq;
        (void)sink;
        if (Mc2PerfProbe::Get().status != 0) valid = 0;
    }
    if (begin == 0 || end < begin) valid = 0;
    return end >= begin ? end - begin : 0;
}
inline void Triple(uint32_t i, Result& r) {
    r.valid = 1;
    // Rotate all six orders to reduce systematic position bias.
    static constexpr uint32_t orders[6][3] = {{0,1,2},{0,2,1},{1,0,2},{1,2,0},{2,0,1},{2,1,0}};
    for (uint32_t j = 0; j < 3; ++j) {
        switch (orders[i % 6][j]) {
            case 0: r.a = Measure<0>(r.valid); break;
            case 1: r.b = Measure<1>(r.valid); break;
            default: r.c = Measure<2>(r.valid); break;
        }
    }
}
inline void RunOnce() {
    static thread_local bool done = false;
    if (done) return;
    done = true;
    static thread_local Result results[100];
    Result warm;
    for (uint32_t i = 0; i < 20; ++i) Triple(i, warm);
    for (uint32_t i = 0; i < 100; ++i) Triple(i, results[i]);
    for (uint32_t i = 0; i < 100; ++i) {
        const auto& r = results[i];
        HCCL_RUN_INFO("[MC2_PROBE_MICRO] protocol=2 count=100 index=%u a_ns=%llu b_ns=%llu c_ns=%llu valid=%u",
            i, static_cast<unsigned long long>(r.a), static_cast<unsigned long long>(r.b),
            static_cast<unsigned long long>(r.c), r.valid);
    }
}
}
#endif
