# Copyright (c) 2026 Huawei Technologies Co., Ltd.
# This program is free software, you can redistribute it and/or modify it under the terms and conditions of
# CANN Open Software License Agreement Version 2.0 (the "License").
# Please refer to the License for details. You may not use this file except in compliance with the License.
# THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
# INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
# See LICENSE in the root of the software repository for the full text of the License.

"""Exercise the actual 2201 dump sender with host models of copy instructions.

Run: python3 tests/api/basic_api/dump_super_tensor/test_2201_sender.py
Requires a C++17 compiler; does not replace device compilation or board tests.
"""

import os
from pathlib import Path
import subprocess
import tempfile

ROOT = Path(__file__).resolve().parents[4]


def section(text, start, end):
    begin = text.index(start)
    return text[begin : text.index(end, begin)]


def test_2201_sender():
    debug = ROOT / "impl/utils/debug"
    types = section((debug / "asc_debug_types.h").read_text(), "enum class DumpType", "struct TimeStampTlv")
    copies = section(
        (debug / "npu_arch_2201/asc_aicore_dump_utils.h").read_text(),
        "namespace __asc_aicore {",
        "#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_ASC_AICORE_DUMP_UTILS__)",
    )
    sender = section(
        (debug / "asc_aicore_dump_impl.h").read_text(),
        "template <AscendC::Hardware hardware, typename T, typename U>\n__aicore__ inline void set_dump_tlv_info(",
        "__aicore__ inline void asc_dump_shape_impl(",
    )
    code = PRELUDE + types + STUBS + copies + "namespace __asc_aicore {\n" + sender + "}\n" + MAIN
    failures = []
    with tempfile.TemporaryDirectory(prefix="dump-2201-") as directory:
        cpp = Path(directory) / "sender.cpp"
        cpp.write_text(code)
        for core, modes in (("CUBE", ("l0c", "l1", "tail", "super", "other")), ("VEC", ("other", "super"))):
            binary = Path(directory) / core
            subprocess.run(
                [
                    os.environ.get("CXX", "g++"),
                    "-std=c++17",
                    "-O1",
                    "-g",
                    "-Wall",
                    "-Wextra",
                    "-Werror",
                    "-Wno-unused-parameter",
                    "-fsanitize=address,undefined",
                    "-fno-omit-frame-pointer",
                    "-fno-pie",
                    "-no-pie",
                    f"-D__DAV_{core}__",
                    str(cpp),
                    "-o",
                    str(binary),
                ],
                check=True,
            )
            for mode in modes:
                result = subprocess.run(
                    [str(binary), mode], check=False, env={**os.environ, "ASAN_OPTIONS": "detect_leaks=0"}
                )
                if result.returncode:
                    failures.append(f"{core}/{mode}")
    assert not failures, f"2201 sender regressions: {', '.join(failures)}"


PRELUDE = r"""
#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstring>
#include <iostream>
#include <string>
#include <vector>
#define __aicore__
#define __gm__
#define __ubuf__
#define __cbuf__
#define __cc__
#define __ca__
#define __cb__
#define __biasbuf__
#define __fbuf__
#define __NPU_ARCH__ 2201
namespace AscendC { enum class Hardware {GM, UB, L1, L0A, L0B, L0C, BIAS, FIXBUF}; }
enum class QuantMode_t {NoQuant};
namespace __asc_aicore {
constexpr uint32_t ASC_ONE_DATABLOCK_SIZE = 32, K_MAX_SHAPE_DIM = 8;
"""

STUBS = r"""
DebugBlockHeadInfo block;
DebugBlockWriteInfo writer;
std::vector<uint8_t> ring, collected;
const uint8_t* sourceBegin;
size_t sourceBytes, reserved, copyCalls;
uint32_t align_up(uint32_t n, uint32_t a) {return (n + a - 1) / a * a;}
uint32_t div_ceil(uint32_t n, uint32_t a) {return (n + a - 1) / a;}
void sync_all() {}
void asc_entire_dcci(uint64_t*) {}
uint32_t asc_debug_get_block_idx() {return 0;}
template<class T> uint32_t get_dump_datatype() {return sizeof(T);}
DebugBlockHeadInfo* get_block_info() {return &block;}
DebugBlockWriteInfo* get_block_write_info(DebugBlockHeadInfo*) {return &writer;}
uint8_t* get_ringbuf_tlv_addr(DebugBlockHeadInfo*) {return ring.data() + writer.bufOffset;}
bool check_ringbuf_space(DebugBlockHeadInfo*, uint32_t n) {
    assert(n <= block.ringBufLen);
    if (writer.bufOffset + n > block.ringBufLen) writer.bufOffset = 0;
    reserved = n;
    std::fill(ring.begin() + writer.bufOffset, ring.end(), 0xA5);
    return true;
}
void update_write_info(DebugBlockWriteInfo*, uint32_t n) {
    assert(n == reserved);
    for (size_t i = writer.bufOffset + n; i < ring.size(); ++i) assert(ring[i] == 0xA5);
    collected.insert(collected.end(), ring.data() + writer.bufOffset, ring.data() + writer.bufOffset + n);
    writer.bufOffset += n;
    ++writer.packIdx;
}
void copy_bytes(void* dst, const void* src, size_t bytes) {
    auto* d = static_cast<uint8_t*>(dst);
    auto* s = static_cast<const uint8_t*>(src);
    assert(s >= sourceBegin && s + bytes <= sourceBegin + sourceBytes);
    assert(d >= ring.data() + writer.bufOffset && d + bytes <= ring.data() + writer.bufOffset + reserved);
    std::memcpy(d, s, bytes);
    ++copyCalls;
}
uint32_t mem_copy_gm_to_gm(uint8_t* d, const uint8_t* s, uint32_t n) {copy_bytes(d, s, n); return 0;}
void copy_cbuf_to_gm(void* d, void* s, int sid, int bursts, uint32_t len, int srcGap, int dstGap) {
    assert(sid == 0 && bursts == 1 && srcGap == 0 && dstGap == 0 && len > 0 && len <= 65535);
    copy_bytes(d, s, len * 32);
}
void copy_ubuf_to_gm(void* d, void* s, int sid, int bursts, uint32_t len, int srcGap, int dstGap) {
    copy_cbuf_to_gm(d, s, sid, bursts, len, srcGap, dstGap);
}
void copy_matrix_cc_to_gm(float* d, float* s, int sid, uint16_t n, uint16_t m, uint16_t dstStride,
                          uint16_t srcStride, int flag, QuantMode_t quant, uint8_t relu, bool split, bool nz2nd) {
    assert(sid == 0 && flag == 0 && quant == QuantMode_t::NoQuant && !relu && !split && !nz2nd);
    assert(n > 0 && n <= 4095 && n % 16 == 0 && m == 16 && srcStride == 16 && dstStride == 32);
    copy_bytes(d, s, size_t(n) * m * sizeof(float));
}
} // namespace __asc_aicore
"""

MAIN = r"""
using namespace __asc_aicore;
using AscendC::Hardware;
void reset(uint32_t capacity, const void* src, size_t bytes) {
    block.ringBufLen = capacity;
    writer = {};
    reserved = copyCalls = 0;
    ring.assign(capacity + 2048, 0xA5);
    collected.clear();
    sourceBegin = static_cast<const uint8_t*>(src);
    sourceBytes = bytes;
}
template<Hardware hw, class T> void ordinary(uint32_t count) {
    constexpr uint32_t alignment = hw == Hardware::L0C ? 1024 : 32;
    const uint32_t bytes = count * sizeof(T), padded = align_up(bytes, alignment);
    std::vector<T> src(padded / sizeof(T));
    for (size_t i = 0; i < src.size(); ++i) src[i] = static_cast<T>(i + 1);
    reset(sizeof(DumpTensorTlv) + padded, src.data(), padded);
    uint32_t shape[] = {count};
    asc_dump_impl<hw, T>(src.data(), 17, count, shape, 1);
    assert(writer.packIdx == 1 && copyCalls == 1);
    assert(writer.bufOffset == sizeof(DumpTensorTlv) + padded);
    auto* h = reinterpret_cast<const DumpTensorTlv*>(collected.data());
    assert(h->dumpSize == bytes && h->desc == 17 && h->dim == 1 && h->shape[0] == count);
    assert(h->position == static_cast<uint16_t>(hw) && h->length + 8 == collected.size());
    assert(std::memcmp(collected.data() + sizeof(*h), src.data(), bytes) == 0);
    // A following record must not overlap the copy's padding.
    for (size_t i = writer.bufOffset; i < ring.size(); ++i) assert(ring[i] == 0xA5);
}
template<Hardware hw> void super_tensor() {
    constexpr uint32_t count = 8193;
    std::vector<uint32_t> src(align_up(count * 4, 1024) / 4);
    for (size_t i = 0; i < src.size(); ++i) src[i] = i * 13 + 7;
    reset(4096, src.data(), src.size() * 4);
    asc_dump_impl<hw, uint32_t>(src.data(), 17, count, nullptr, 0);
    assert(writer.packIdx > 1);
    size_t cursor = 0, offset = 0;
    while (cursor < collected.size()) {
        size_t head, size, span;
        if (offset == 0) {
            DumpSuperTensorTlv h; std::memcpy(&h, collected.data() + cursor, sizeof(h));
            assert(h.tensorLength == count * 4 && h.tensorOffset == 0);
            head = sizeof(h); size = h.dumpSize; span = h.length + 8;
        } else {
            DumpSuperTensorBodyTlv h; std::memcpy(&h, collected.data() + cursor, sizeof(h));
            assert(h.tensorLength == count * 4 && h.tensorOffset == offset);
            head = sizeof(h); size = h.dumpSize; span = h.length + 8;
        }
        assert(head + size <= span && size > 0);
        assert(std::memcmp(collected.data() + cursor + head, sourceBegin + offset, size) == 0);
        cursor += span; offset += size;
    }
    assert(offset == count * 4);
}
int main(int argc, char** argv) {
    assert(argc == 2);
    const std::string mode = argv[1];
#if defined(__DAV_CUBE__)
    if (mode == "l0c") for (unsigned count : {256u, 512u, 1024u, 32768u}) {
        ordinary<Hardware::L0C, float>(count); ordinary<Hardware::L0C, int32_t>(count);
    }
    if (mode == "l1") for (unsigned count : {1u, 31u, 32u, 33u, 1024u, 65536u}) {
        ordinary<Hardware::L1, uint8_t>(count); ordinary<Hardware::L1, uint64_t>(count);
    }
    if (mode == "tail") {
        for (unsigned count : {1u, 255u, 257u, 1023u, 1025u}) ordinary<Hardware::L0C, float>(count);
        std::vector<float> src(256, 1);
        reset(512, src.data(), 1024);
        asc_dump_impl<Hardware::L0C, float>(src.data(), 0, 1, nullptr, 0);
        assert(writer.packIdx == 0 && copyCalls == 0);
    }
    if (mode == "super") {super_tensor<Hardware::L0C>(); super_tensor<Hardware::L1>();}
#else
    if (mode == "super") super_tensor<Hardware::UB>();
#endif
    if (mode == "other") {
        for (unsigned count : {1u, 31u, 32u, 33u, 1025u}) {
            ordinary<Hardware::GM, uint8_t>(count);
#if defined(__DAV_VEC__)
            ordinary<Hardware::UB, uint16_t>(count); ordinary<Hardware::UB, uint64_t>(count);
#endif
        }
        std::vector<float> src(256, 1);
        reset(4096, src.data(), 1024);
        asc_dump_impl<Hardware::GM, float>(src.data(), 0, 0, nullptr, 0);
        assert(copyCalls == 0 && writer.packIdx == 0);
        assert(mem_copy_abuf_to_gm_impl(src.data(), src.data(), 1024) == 1);
        assert(mem_copy_bbuf_to_gm_impl(src.data(), src.data(), 1024) == 1);
        assert(mem_copy_biasbuf_to_gm_impl(src.data(), src.data(), 1024) == 1);
        assert(mem_copy_fbuf_to_gm_impl(src.data(), src.data(), 1024) == 1);
    }
    std::cout << "2201 sender " << mode << " passed\n";
}
"""

if __name__ == "__main__":
    test_2201_sender()
