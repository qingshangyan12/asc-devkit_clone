# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# Licensed under the CANN Open Software License Agreement Version 2.0.
# See LICENSE in the repository root.

"""Portable regression for production key serialization; not an NPU/whole-policy test.

Run with python3 test_asccomm_policy_serialization.py. Requires a C++17 compiler.
The harness extracts the production serializer and dynamic-resource writer verbatim.
"""
import os
from pathlib import Path
import re
import subprocess
import tempfile
import unittest


def block(source, prefix):
    start = source.index(prefix)
    opening = source.index("{", start)
    depth = 1
    end = opening + 1
    while depth:
        depth += (source[end] == "{") - (source[end] == "}")
        end += 1
    return source[start:end]


class PolicySerializationTest(unittest.TestCase):
    def test_production_serialization_and_dynamic_fields(self):
        root = next(p for p in Path(__file__).resolve().parents if (p / "impl").is_dir())
        source = (root / "impl/adv_api/detail/hccl/cc/src/ops/op_common/template/aicpu/task_cache/mc2_aicpu_task_cache.cc").read_text()
        original_fields = set(re.findall(r"param\.(\w+)", block(source, "bool AddResourceFields(")))
        dynamic_fields = set(re.findall(r"param\.(\w+)", block(source, "void AddDynamicResourceFields(")))
        self.assertEqual(original_fields, dynamic_fields)
        program = r"""
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iomanip>
#include <sstream>
#include <string>
#include <utility>
struct HcclMem { void* addr = nullptr; uint64_t size = 0; };
struct OpParam {
    void* hcclComm = nullptr;
    char commName[128] = "comm";
    char algTag[128] = "alg";
    void* inputSymWindow = nullptr;
    void* outputSymWindow = nullptr;
    uint64_t opThread = 0;
    HcclMem hcclBuff;
};
constexpr size_t COMM_INDENTIFIER_MAX_LENGTH = 128, ALG_TAG_LENGTH = 128;
"""
        program += block(source, "class Digest128Builder") + ";\n"
        program += block(source, "void AddHcclMem(") + "\n"
        program += block(source, "void AddDynamicResourceFields(") + "\n"
        program += r"""
std::string key(const OpParam& p) {
    Digest128Builder b("test", true);
    AddDynamicResourceFields(p, b);
    return b.TakeExactBytes();
}
int main() {
    Digest128Builder exact("x", true);
    exact.AddU8(0xff); exact.AddU32(0x12345678); exact.AddU64(0x123456789abcdef0ULL);
    const unsigned char expected[] = {1,0,0,0,0,0,0,0,'x',255,0x78,0x56,0x34,0x12,
                                     0xf0,0xde,0xbc,0x9a,0x78,0x56,0x34,0x12};
    assert(exact.TakeExactBytes() == std::string(reinterpret_cast<const char*>(expected), sizeof(expected)));
    // Each OpParam-owned resource component must discriminate cache identity.
    OpParam base;
    for (int i = 0; i < 8; ++i) {
        auto p = base;
        switch (i) {
            case 0: p.hcclComm = reinterpret_cast<void*>(1); break;
            case 1: p.commName[0] = 'C'; break;
            case 2: p.algTag[0] = 'A'; break;
            case 3: p.inputSymWindow = reinterpret_cast<void*>(2); break;
            case 4: p.outputSymWindow = reinterpret_cast<void*>(3); break;
            case 5: p.opThread = 4; break;
            case 6: p.hcclBuff.addr = reinterpret_cast<void*>(5); break;
            case 7: p.hcclBuff.size = 6; break;
        }
        assert(key(p) != key(base));
    }
    assert(key(base) == key(base));
    OpParam a, b; strcpy(a.commName, "ab"); strcpy(a.algTag, "c");
    strcpy(b.commName, "a"); strcpy(b.algTag, "bc");
    assert(key(a) != key(b)); // Length framing prevents concatenation aliases.
    Digest128Builder digest("x");
    digest.AddU8(0xff); digest.AddU32(0x12345678); digest.AddU64(0x123456789abcdef0ULL);
    assert(digest.TakeExactBytes().empty()); // No unused string on legacy path.
    assert(digest.Finish() == "25b761d622dafc07a3b1058caa09a0b9");
}
"""
        with tempfile.TemporaryDirectory() as temp:
            src, exe = Path(temp) / "test.cc", Path(temp) / "test"
            src.write_text(program)
            subprocess.run([os.environ.get("CXX", "c++"), "-std=c++17", "-Wall", "-Wextra", "-Werror", str(src), "-o", str(exe)], check=True)
            subprocess.run([str(exe)], check=True)


if __name__ == "__main__":
    unittest.main()
