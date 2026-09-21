# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Host-only regression for the production MC2 loader, without CANN libraries.

Run with python3 tests/tools/test_mc2_thread_config_loader.py.
Compile the loader section verbatim and the real private config header; mock
only the dynamic loader, logging and legacy resource API. Not a package/NPU test.
"""

import os
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest


ROOT = Path(__file__).resolve().parents[2]
SOURCE = ROOT / "impl/adv_api/detail/hccl/cc/src/common/hcomm_dlsym"

TYPES = r"""
#pragma once
#include <cstdint>
using HcclResult = int;
using HcclComm = void*;
using ThreadHandle = uint64_t;
enum CommEngine { COMM_ENGINE_CPU = 0, COMM_ENGINE_AICPU = 1, COMM_ENGINE_AICPU_TS = 3 };
constexpr int HCCL_E_PARA = 4;
HcclResult HcclThreadAcquire(HcclComm, CommEngine, uint32_t, uint32_t, ThreadHandle*);
"""

HARNESS = r"""
#include <atomic>
#include <cassert>
#include <cstring>
#include <dlfcn.h>
#include <thread>
#include <vector>
#include "mc2_thread_config_compat.h"
std::atomic<int> opens{0}, resolves{0}, closes{0}, warnings{0}, infos{0}, calls{0}, legacy{0};
int mode = 0;
int target(HcclComm, CommEngine engine, uint32_t count, mc2_thread_config::ThreadType type,
           const mc2_thread_config::ThreadConfig* configs, ThreadHandle*)
{
    assert(engine == COMM_ENGINE_CPU || engine == COMM_ENGINE_AICPU);
    assert(type == mc2_thread_config::THREAD_TYPE_TS);
    for (uint32_t i = 0; i < count; ++i) {
        assert(configs[i].header.version == 1 && configs[i].header.size == 32);
        assert(configs[i].header.magicWord == 0x0f0f0f2fU);
        assert(configs[i].notifyNumPerThread == 7);
        for (int j = 0; j < 14; ++j) assert(configs[i].reserved[j] == (j == 13 ? 1 : 0));
    }
    ++calls;
    return 23;
}
void* mockOpen(const char* name, int flags)
{
    assert(std::strcmp(name, "libhcomm.so") == 0 && flags == RTLD_NOW);
    ++opens;
    return mode == 1 ? nullptr : reinterpret_cast<void*>(1);
}
void* mockSymbol(void* handle, const char* name)
{
    assert(handle == reinterpret_cast<void*>(1));
    assert(std::strcmp(name, "HcclThreadAcquireWithConfig") == 0);
    ++resolves;
    return mode == 2 ? nullptr : reinterpret_cast<void*>(&target);
}
const char* mockError() { return nullptr; }
int mockClose(void*) { ++closes; return 0; }
int HcclThreadAcquire(HcclComm, CommEngine engine, uint32_t, uint32_t, ThreadHandle*)
{
    assert(engine == COMM_ENGINE_CPU || engine == COMM_ENGINE_AICPU_TS);
    ++legacy;
    return 29;
}
// An unrelated compat initializer must never be needed by this loader.
extern "C" void HcommDlInit() { assert(false); }
void mockLog(const char*, ...) {}
#define dlopen mockOpen
#define dlsym mockSymbol
#define dlerror mockError
#define dlclose mockClose
#define HCCL_WARNING(...) (++warnings, mockLog(__VA_ARGS__))
#define HCCL_INFO(...) (++infos, mockLog(__VA_ARGS__))
#define HCCL_ERROR(...) mockLog(__VA_ARGS__)
"""

MAIN = r"""
int main(int argc, char** argv)
{
    assert(argc == 2);
    mode = argv[1][0] - '0';
    assert(Mc2ThreadAcquireWithConfig(nullptr, COMM_ENGINE_AICPU, 1, 65536, nullptr) == HCCL_E_PARA);
    assert(opens == 0);
    std::vector<std::thread> workers;
    for (int i = 0; i < 32; ++i) workers.emplace_back([i]() {
        auto engine = i % 2 ? COMM_ENGINE_CPU : COMM_ENGINE_AICPU;
        assert(Mc2ThreadAcquireWithConfig(nullptr, engine, 3, 7, nullptr) == (mode ? 29 : 23));
    });
    for (auto& worker : workers) worker.join();
    assert(Mc2ThreadAcquireWithConfig(nullptr, COMM_ENGINE_AICPU, 3, 7, nullptr) == (mode ? 29 : 23));
    assert(opens == 1 && resolves == (mode == 1 ? 0 : 1));
    assert(closes == (mode == 2 ? 1 : 0));
    assert(warnings == (mode ? 1 : 0) && infos == (mode ? 0 : 1));
    assert(calls == (mode ? 0 : 33) && legacy == (mode ? 33 : 0));
}
"""


class Mc2ThreadConfigLoaderTest(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        compiler = shutil.which(os.environ.get("CXX", "c++"))
        if compiler is None:
            raise unittest.SkipTest("C++ compiler unavailable")
        cls.temp = tempfile.TemporaryDirectory(prefix="mc2-loader-test-")
        cls.addClassCleanup(cls.temp.cleanup)
        work = Path(cls.temp.name)
        (work / "hccl_res.h").write_text(TYPES)
        (work / "mc2_thread_config_compat.h").write_text(
            (SOURCE / "mc2_thread_config_compat.h").read_text()
        )
        source = (SOURCE / "hccl_res_dl.cc").read_text()
        loader = source[: source.index("DEFINE_WEAK_FUNC(")]
        loader = loader.replace('#include "hccl_res_dl.h"', "")
        (work / "test.cc").write_text(HARNESS + loader + MAIN)
        cls.binary = work / "test"
        subprocess.run(
            [
                compiler,
                "-std=c++11",
                "-pthread",
                "-Wall",
                "-Wextra",
                "-Werror",
                "-I",
                str(work),
                str(work / "test.cc"),
                "-o",
                str(cls.binary),
            ],
            check=True,
        )

    def test_first_use_concurrency_and_markers(self):
        subprocess.run([str(self.binary), "0"], check=True)

    def test_missing_library_falls_back_once(self):
        subprocess.run([str(self.binary), "1"], check=True)

    def test_missing_symbol_falls_back_once(self):
        subprocess.run([str(self.binary), "2"], check=True)


if __name__ == "__main__":
    unittest.main()
