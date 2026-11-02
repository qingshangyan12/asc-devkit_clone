#!/usr/bin/env python3
# ----------------------------------------------------------------------------------------------------------
# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.
# ----------------------------------------------------------------------------------------------------------
"""Local C++14 log-contract test; does not validate target CANN or NPU timing."""

from pathlib import Path
import shutil
import subprocess
import tempfile

repo = Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as t:
    root = Path(t)
    shutil.copyfile(
        repo / "impl/adv_api/detail/hccl/cc/src/common/mc2_server_stage_timing.h", root / "mc2_server_stage_timing.h"
    )
    (root / "log.h").write_text("""#pragma once
#include <cstdio>
#include <string>
#include <vector>
inline std::vector<std::string>& Logs() { static std::vector<std::string> v; return v; }
#define HCCL_RUN_INFO(...) do { char text[1024]; std::snprintf(text,sizeof(text),__VA_ARGS__); Logs().push_back(text); } while(0)
""")
    (root / "test.cc").write_text("""#include "mc2_server_stage_timing.h"
#include <cassert>
#include <algorithm>
#include <limits>
#include <random>
int main() {
    using namespace Mc2ServerStageTiming;
    // Check order, ties, zeros and the full uint64_t range against a sorted reference.
    std::mt19937_64 random(1);
    for (int trial=0; trial<100; ++trial) {
        Top3 top{};
        std::vector<uint64_t> values(3, 0);
        for (int i=0; i<30; ++i) {
            uint64_t value=trial==0 ? 0 : trial==1 ? 7 : trial==2 ?
                std::numeric_limits<uint64_t>::max() : random();
            UpdateTop3(top, value);
            values.push_back(value);
            std::sort(values.rbegin(), values.rend());
            assert(top.first==values[0] && top.second==values[1] && top.third==values[2]);
        }
    }
    Start(false); PollBegin(); Receive(0); Parsed(); Cache(false); Submitted(0); Finish(0);
    assert(Logs().empty());
    for (int i=0; i<30; ++i) {
        Start(true, 1);
        auto& s=Get(); s.entry=100; s.poll=200; s.parsed=500; s.submitted=900;
        s.messages=1; s.valid=15; Cache(false); Finish(0);
        assert(Logs().size()==(i==29 ? 1U : 0U));
    }
    assert(Logs()[0].find("count=30 init_mean_ns=100.000000 msg_parse_mean_ns=300.000000 sq_expand_mean_ns=400.000000 cache=0 status=0")!=std::string::npos);
    assert(Logs()[0].find("init_top3_ns=100,100,100 msg_parse_top3_ns=300,300,300 sq_expand_top3_ns=400,400,400")!=std::string::npos);
    assert(Totals().initTop.first==0 && Totals().msgTop.first==0 && Totals().sqTop.first==0);
    for (int i=0; i<30; ++i) { Start(true, 1); Finish(1); }
    assert(Logs().size()==2 && Logs()[1].find("status=-3")!=std::string::npos);
    Totals().initTop.first=123;
    Start(true, 1); Finish(1); Start(false); assert(Totals().initTop.first==0); assert(Totals().count==0);
    Start(true, 1); Finish(1); Totals().sqTop.first=456;
    Start(true, 2); assert(Totals().count==0 && Totals().sqTop.first==0);
    for (uint64_t i=1; i<=30; ++i) {
        Start(true, 2);
        auto& s=Get(); s.entry=100; s.poll=100+i; s.parsed=s.poll+2*i; s.submitted=s.parsed+3*i;
        s.messages=1; s.valid=VALID_ALL; Cache(true); Finish(0);
    }
    assert(Logs().size()==3);
    assert(Logs()[2].find("init_mean_ns=15.500000 msg_parse_mean_ns=31.000000 sq_expand_mean_ns=46.500000 cache=1 status=0")!=std::string::npos);
    assert(Logs()[2].find("init_top3_ns=30,29,28 msg_parse_top3_ns=60,58,56 sq_expand_top3_ns=90,87,84")!=std::string::npos);
}
""")
    subprocess.run(
        ["c++", "-std=c++14", "-O2", "-Wall", "-Wextra", "-Werror", str(root / "test.cc"), "-o", str(root / "test")],
        check=True,
    )
    subprocess.run([str(root / "test")], check=True)
print("Server timing mock contract passed; no target build/NPU validation.")
