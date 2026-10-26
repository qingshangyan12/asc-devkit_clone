#!/usr/bin/env python3
"""Local C++14 log-contract test; does not validate target CANN or NPU timing."""
from pathlib import Path
import shutil
import subprocess
import tempfile

repo=Path(__file__).resolve().parents[2]
with tempfile.TemporaryDirectory() as t:
    root=Path(t)
    shutil.copyfile(repo/'impl/adv_api/detail/hccl/cc/src/common/mc2_server_stage_timing.h',root/'mc2_server_stage_timing.h')
    (root/'log.h').write_text('''#pragma once
#include <cstdio>
#include <string>
#include <vector>
inline std::vector<std::string>& Logs() { static std::vector<std::string> v; return v; }
#define HCCL_RUN_INFO(...) do { char text[1024]; std::snprintf(text,sizeof(text),__VA_ARGS__); Logs().push_back(text); } while(0)
''')
    (root/'test.cc').write_text('''#include "mc2_server_stage_timing.h"
#include <cassert>
int main() {
    using namespace Mc2ServerStageTiming;
    Start();
    assert(Logs().empty());
    auto& s=Get(); s.entry=100; s.poll=200; s.parsed=500; s.submitted=900;
    s.messages=1; s.valid=15; Cache(false); Finish(0);
    assert(Logs().size()==1);
    assert(Logs()[0].find("init_ns=100 msg_parse_ns=300 sq_expand_ns=400 cache=0 status=0")!=std::string::npos);
    Start(); Finish(1);
    assert(Logs().size()==2 && Logs()[1].find("status=-3")!=std::string::npos);
    Start(); Receive(0); Receive(1); s.valid=15; s.entry=100; s.poll=200; s.parsed=300; s.submitted=400; Finish(0);
    assert(Logs().size()==3 && Logs()[2].find("status=-3")!=std::string::npos);
    Start(); s.messages=1; s.valid=15; s.entry=400; s.poll=200; Finish(0);
    assert(Logs().size()==4 && Logs()[3].find("status=-4")!=std::string::npos);
    Start(); Cache(true); Cache(false); assert(Get().cache==2);
    Start(); Parsed(); Parsed(); assert(Get().status!=0);
}
''')
    subprocess.run(['c++','-std=c++14','-O2','-Wall','-Wextra','-Werror',str(root/'test.cc'),'-o',str(root/'test')],check=True)
    subprocess.run([str(root/'test')],check=True)
print('Server timing mock contract passed; no target build/NPU validation.')
