/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include <vector>
#include <cstdint>
#include <sstream>
#include <iomanip>

namespace {

constexpr uint64_t MB = 1024ULL * 1024ULL;
constexpr uint64_t GB = 1024ULL * MB;

constexpr uint64_t CCU_MS_INTERLEAVE = 8;
constexpr uint64_t CCU_MS_SIZE = 4096;

constexpr uint64_t SetBits(uint16_t end) { return ((uint64_t(1) << (end + 1)) - uint64_t(1)); }

uint64_t GetMaxLoopIterNum() {
    constexpr uint16_t loopNumBitNum = 12;
    return SetBits(loopNumBitNum);
}

uint64_t GetLoopParam(uint64_t loopCtxId, uint64_t gsaOffset, uint64_t loopIterNum) {
    constexpr uint16_t ctxIdBitNum = 8;
    constexpr uint16_t ctxIdShiftBit = 45;
    constexpr uint16_t gsaBitNum = 32;
    constexpr uint16_t gsaShiftBit = 13;
    constexpr uint16_t loopNumBitNum = 13;
    constexpr uint16_t loopNumShiftBit = 0;
    return ((loopCtxId & SetBits(ctxIdBitNum)) << ctxIdShiftBit) |
           ((gsaOffset & SetBits(gsaBitNum)) << gsaShiftBit) |
           ((loopIterNum & SetBits(loopNumBitNum)) << loopNumShiftBit);
}

uint64_t GetParallelParam(uint64_t repeatNum, uint64_t repeatLoopIndex, uint64_t totalLoopNum) {
    constexpr uint16_t repeatBitNum = 7;
    constexpr uint16_t repeatNumShiftBit = 55;
    constexpr uint16_t repeatLoopBitNum = 7;
    constexpr uint16_t repeatLoopShiftBit = 48;
    constexpr uint16_t totalLoopBitNum = 7;
    constexpr uint16_t totalLoopShiftBit = 41;
    return ((repeatNum & SetBits(repeatBitNum)) << repeatNumShiftBit) |
           ((repeatLoopIndex & SetBits(repeatLoopBitNum)) << repeatLoopShiftBit) |
           ((totalLoopNum & SetBits(totalLoopBitNum)) << totalLoopShiftBit);
}

uint64_t GetOffsetParam(uint64_t gsaOffset, uint64_t msOffset, uint64_t ckeOffset) {
    constexpr uint16_t gsaBitNum = 32;
    constexpr uint16_t gsaShiftBit = 21;
    constexpr uint16_t msBitNum = 11;
    constexpr uint16_t msShiftBit = 10;
    constexpr uint16_t ckeBitNum = 10;
    constexpr uint16_t ckeShiftBit = 0;
    return ((gsaOffset & SetBits(gsaBitNum)) << gsaShiftBit) |
           ((msOffset & SetBits(msBitNum)) << msShiftBit) |
           ((ckeOffset & SetBits(ckeBitNum)) << ckeShiftBit);
}

struct LoopGroupConfig {
    uint32_t msInterleave;
    uint32_t loopCount;
    uint64_t memSlice;
};

std::vector<uint64_t> CalGoSize(uint64_t size, const LoopGroupConfig& config) {
    uint64_t loopSize = config.loopCount * config.memSlice;
    uint64_t maxSize = loopSize * (GetMaxLoopIterNum() + 1);

    uint64_t m = size / loopSize;
    uint64_t n = (size - m * loopSize) / config.memSlice;
    uint64_t p = size - m * loopSize - n * config.memSlice;

    if (size == maxSize) {
        m = GetMaxLoopIterNum();
        n = config.loopCount - 1;
        p = config.memSlice;
    }

    uint64_t offset = config.memSlice * config.loopCount * m;
    uint64_t loopIterNum = m;

    uint64_t loopExtendNum = 0;
    uint64_t tailSize = 0;
    uint64_t LoopNumTwo = 2;

    if (n == 0 && p == 0) {
        loopExtendNum = 0;
        tailSize = 0;
    } else if (n != 0 && p == 0) {
        loopExtendNum = GetParallelParam(n - 1, 0, 1);
        tailSize = config.memSlice;
    } else if (n == 0 && p != 0) {
        loopExtendNum = GetParallelParam(0, 0, 1);
        tailSize = p;
    } else {
        loopExtendNum = GetParallelParam(n - 1, 1, LoopNumTwo);
        tailSize = p;
    }

    return {offset, loopIterNum, loopExtendNum, tailSize};
}

std::string Hex(uint64_t v) {
    std::ostringstream oss;
    oss << "0x" << std::setfill('0') << std::setw(16) << std::hex << v;
    return oss.str();
}

std::string Dec(uint64_t v) {
    std::ostringstream oss;
    oss << v;
    return oss.str();
}

struct GoSizeResult {
    uint64_t offset;
    uint64_t loopIterNum;
    uint64_t loopExtendNum;
    uint64_t tailSize;
};

GoSizeResult RunCalGoSize(uint64_t sliceSize) {
    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount    = CCU_MS_INTERLEAVE;
    config.memSlice     = CCU_MS_INTERLEAVE * CCU_MS_SIZE;
    auto go = CalGoSize(sliceSize, config);
    return {go[0], go[1], go[2], go[3]};
}

constexpr uint64_t MEM_SLICE    = CCU_MS_INTERLEAVE * CCU_MS_SIZE;
constexpr uint64_t LOOP_COUNT   = CCU_MS_INTERLEAVE;
constexpr uint64_t MS_INTERLEAVE = CCU_MS_INTERLEAVE;
constexpr uint64_t LOOP_SIZE    = LOOP_COUNT * MEM_SLICE;
constexpr uint64_t MAX_LOOP_ITER = 8191;
constexpr uint64_t MAX_SIZE     = LOOP_SIZE * (MAX_LOOP_ITER + 1);

void PrintResult(const std::string& label, uint64_t sliceSize, const GoSizeResult& go) {
    uint64_t loopParamBase = GetLoopParam(0, MEM_SLICE * LOOP_COUNT, 0);
    uint64_t loopParamFull = GetLoopParam(0, go.offset, go.loopIterNum);
    uint64_t offsetParam   = GetOffsetParam(MEM_SLICE, MS_INTERLEAVE, 1);
    uint64_t sum           = loopParamBase + go.loopIterNum;

    std::cout << "\n=== " << label << " ==="
              << "\n  sliceSize       = " << Dec(sliceSize) << " (" << Hex(sliceSize) << ")"
              << "\n  goSize.offset   = " << Dec(go.offset) << " (" << Hex(go.offset) << ")"
              << "\n  goSize.loopIter = " << Dec(go.loopIterNum) << " (" << Hex(go.loopIterNum) << ")"
              << "\n  goSize.loopExt  = " << Dec(go.loopExtendNum) << " (" << Hex(go.loopExtendNum) << ")"
              << "\n  goSize.tailSize = " << Dec(go.tailSize) << " (" << Hex(go.tailSize) << ")"
              << "\n  loopParamBase   = " << Dec(loopParamBase) << " (" << Hex(loopParamBase) << ")"
              << "\n  loopParamFull   = " << Dec(loopParamFull) << " (" << Hex(loopParamFull) << ")"
              << "\n  base+loopIter   = " << Dec(sum) << " (" << Hex(sum) << ")"
              << "\n  offsetParam     = " << Dec(offsetParam) << " (" << Hex(offsetParam) << ")"
              << std::endl;
}

} // namespace

class CalGoSizeOverflowTest : public ::testing::Test {
protected:
    void TestSize(const std::string& label, uint64_t sliceSize) {
        auto go = RunCalGoSize(sliceSize);
        PrintResult(label, sliceSize, go);

        EXPECT_LE(go.loopIterNum, MAX_LOOP_ITER)
            << "loopIterNum overflow: " << go.loopIterNum << " > " << MAX_LOOP_ITER;

        uint64_t gsaMask = ((uint64_t(1) << 33) - 1);
        EXPECT_LE(go.offset, gsaMask)
            << "offset exceeds 32-bit gsa field: " << Hex(go.offset);

        EXPECT_LE(go.loopIterNum, ((uint64_t(1) << 13) - 1))
            << "loopIterNum exceeds 13-bit field";

        uint64_t loopParamSum = GetLoopParam(0, MEM_SLICE * LOOP_COUNT, 0) + go.loopIterNum;
        uint64_t gsaFromSum = (loopParamSum >> 13) & gsaMask;
        uint64_t gsaExpected = (MEM_SLICE * LOOP_COUNT) & gsaMask;
        EXPECT_EQ(gsaFromSum, gsaExpected)
            << "gsa corrupted after base+loopIter addition: got " << Hex(gsaFromSum)
            << " expected " << Hex(gsaExpected);

        if (go.tailSize > 0) {
            EXPECT_EQ(go.offset + go.tailSize, sliceSize)
                << "offset + tailSize != sliceSize";
        }
    }
};

TEST_F(CalGoSizeOverflowTest, Below512MB) {
    TestSize("256MB", 256 * MB);
}

TEST_F(CalGoSizeOverflowTest, Exactly512MB) {
    TestSize("512MB", 512 * MB);
}

TEST_F(CalGoSizeOverflowTest, Above512MB_Plus1) {
    TestSize("512MB+1", 512 * MB + 1);
}

TEST_F(CalGoSizeOverflowTest, Above512MB_768MB) {
    TestSize("768MB", 768 * MB);
}

TEST_F(CalGoSizeOverflowTest, OneGB) {
    TestSize("1GB", 1 * GB);
}

TEST_F(CalGoSizeOverflowTest, OneHalfGB) {
    TestSize("1.5GB", static_cast<uint64_t>(1.5 * GB));
}

TEST_F(CalGoSizeOverflowTest, NearMax2GB) {
    TestSize("2GB-1", 2 * GB - 1);
}

TEST_F(CalGoSizeOverflowTest, ExactlyMax2GB) {
    TestSize("2GB(max)", MAX_SIZE);
}

TEST_F(CalGoSizeOverflowTest, Boundary512MB_MultiOffset) {
    std::cout << "\n=== Boundary Analysis ==="
              << "\n  loopSize     = " << Dec(LOOP_SIZE) << " (" << Hex(LOOP_SIZE) << ")"
              << "\n  maxLoopIter  = " << Dec(MAX_LOOP_ITER)
              << "\n  maxSize      = " << Dec(MAX_SIZE) << " (" << Hex(MAX_SIZE) << ")"
              << "\n  512MB        = " << Dec(512 * MB) << " (" << Hex(512 * MB) << ")"
              << "\n  512MB / loop = " << Dec((512 * MB) / LOOP_SIZE)
              << "\n  512MB % loop = " << Dec((512 * MB) % LOOP_SIZE)
              << std::endl;

    for (uint64_t sz = 512 * MB - LOOP_SIZE; sz <= 512 * MB + LOOP_SIZE; sz += LOOP_SIZE / 4) {
        auto go = RunCalGoSize(sz);
        PrintResult("boundary " + Dec(sz), sz, go);
    }
}

TEST_F(CalGoSizeOverflowTest, SimulateKernelRunParams) {
    std::cout << "\n=== Simulate CcuTempAlltoAllMesh1D::KernelRun ===" << std::endl;

    struct TestCase {
        std::string name;
        uint64_t count;
        uint64_t dataTypeSize;
        uint32_t rankSize;
        uint32_t myRank;
    };

    std::vector<TestCase> cases = {
        {"2p bf16 256MB",   128 * MB, 2, 2, 0},
        {"2p bf16 512MB",   256 * MB, 2, 2, 0},
        {"2p bf16 512MB+1", 256 * MB + 1, 2, 2, 0},
        {"2p bf16 1GB",     512 * MB, 2, 2, 0},
        {"2p fp32 512MB",   128 * MB, 4, 2, 0},
        {"2p fp32 1GB",     256 * MB, 4, 2, 0},
        {"4p bf16 512MB",   128 * MB, 2, 4, 0},
        {"4p bf16 1GB",     256 * MB, 2, 4, 0},
    };

    for (const auto& tc : cases) {
        uint64_t sliceSize = tc.count * tc.dataTypeSize;
        uint64_t srcStride = tc.count * tc.dataTypeSize;
        uint64_t srcOffset = 0;
        uint64_t dstOffset = tc.myRank * srcStride;
        uint64_t inputAddr = 0x100000000ULL;
        uint64_t outputAddr = 0x200000000ULL;

        auto go = RunCalGoSize(sliceSize);

        std::cout << "\n--- " << tc.name << " ---"
                  << "\n  count=" << Dec(tc.count) << " dtype=" << Dec(tc.dataTypeSize)
                  << " rankSize=" << tc.rankSize << " rank=" << tc.myRank
                  << "\n  sliceSize  = " << Dec(sliceSize) << " (" << Hex(sliceSize) << ")"
                  << "\n  srcStride  = " << Dec(srcStride) << " (" << Hex(srcStride) << ")"
                  << "\n  srcOffset  = " << Dec(srcOffset) << " (" << Hex(srcOffset) << ")"
                  << "\n  dstOffset  = " << Dec(dstOffset) << " (" << Hex(dstOffset) << ")"
                  << "\n  inputAddr  = " << Hex(inputAddr)
                  << "\n  outputAddr = " << Hex(outputAddr)
                  << "\n  goSize[0] (offset)   = " << Dec(go.offset) << " (" << Hex(go.offset) << ")"
                  << "\n  goSize[1] (loopIter) = " << Dec(go.loopIterNum) << " (" << Hex(go.loopIterNum) << ")"
                  << "\n  goSize[2] (loopExt)  = " << Dec(go.loopExtendNum) << " (" << Hex(go.loopExtendNum) << ")"
                  << "\n  goSize[3] (tailSize) = " << Dec(go.tailSize) << " (" << Hex(go.tailSize) << ")"
                  << "\n  taskArgs = {"
                  << Hex(inputAddr) << ", " << Hex(outputAddr) << ", " << "token"
                  << ", " << Hex(sliceSize) << ", " << Hex(srcStride)
                  << ", " << Hex(srcOffset) << ", " << Hex(dstOffset)
                  << ", " << Hex(go.offset) << ", " << Hex(go.loopIterNum)
                  << ", " << Hex(go.loopExtendNum) << ", " << Hex(go.tailSize) << "}"
                  << std::endl;

        EXPECT_LE(go.loopIterNum, MAX_LOOP_ITER)
            << tc.name << ": loopIterNum overflow";
        EXPECT_LE(go.offset, ((uint64_t(1) << 33) - 1))
            << tc.name << ": offset exceeds 32-bit gsa field";

        for (uint32_t r = 0; r < tc.rankSize; r++) {
            if (r != tc.myRank) {
                uint64_t srcAddr = srcOffset;
                for (uint32_t i = 0; i < r; i++) {
                    srcAddr += srcStride;
                }
                uint64_t dstAddr = dstOffset;
                std::cout << "  rank[" << r << "] Write: srcAddr=" << Hex(inputAddr + srcAddr)
                          << " dstAddr=" << Hex(outputAddr + dstAddr)
                          << " len=" << Dec(sliceSize) << std::endl;
            }
        }
    }
}
