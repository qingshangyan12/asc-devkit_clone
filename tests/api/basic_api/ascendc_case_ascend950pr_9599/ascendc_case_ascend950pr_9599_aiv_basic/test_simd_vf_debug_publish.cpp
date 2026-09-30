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
#include <mockcpp/mockcpp.hpp>
#include <array>
#include <cstring>
#include <vector>
#include "stub_fun.h"
#include "impl/utils/sys_macros_impl.h"
#include "impl/utils/common_types.h"

// Keep CPU stub types/core selection, but include the real device debug implementation.
// float8_e5m2_t/float8_e4m3_t/float8_e8m0_t/float4_e1m2x2_t are aliased by the CPU stub;
// float4_e2m1x2_t is only referenced by uninstantiated traits in the included header.
struct float4_e2m1x2_t;
namespace __cce_scalar {
using ::copy_ubuf_to_ubuf;
}
#pragma push_macro("ASCENDC_CPU_DEBUG")
#undef ASCENDC_CPU_DEBUG
#include "impl/utils/debug/asc_debug_utils.h"
#pragma pop_macro("ASCENDC_CPU_DEBUG")

namespace {
using namespace __asc_aicore;
constexpr uint32_t RING_BYTES = 8192;
constexpr uint32_t BATCH_BYTES = 64;
struct GmBlock {
    DebugBlockHeadInfo head;
    DebugBlockReadInfo reader;
    alignas(8) std::array<uint8_t, RING_BYTES> data;
    DebugBlockWriteInfo writer;
};
static_assert(offsetof(GmBlock, reader) == sizeof(DebugBlockHeadInfo));
static_assert(offsetof(GmBlock, writer) == offsetof(GmBlock, data) + RING_BYTES);

struct Flush {
    const void* address;
    uint64_t writeOffset;
    uint64_t packages;
    uint32_t ubRead;
    uint16_t coreType;
};
GmBlock* activeGm;
BlockVFBufInfo* activeUb;
std::vector<Flush> flushes;
bool appendAtCommit = false;
void RecordFlush(void* address, uint64_t entire, uint64_t destination)
{
    EXPECT_EQ(entire, cache_line_t::ENTIRE_DATA_CACHE);
    EXPECT_EQ(destination, dcci_dst_t::CACHELINE_OUT);
    flushes.push_back(
        {address, activeGm->writer.bufOffset, activeGm->writer.packIdx, activeUb->readLen, activeGm->head.flag});
    if (appendAtCommit && address == &activeGm->writer) {
        appendAtCommit = false;
        const uint32_t header[] = {1, 24};
        std::memcpy(activeUb->buffer + 32, header, sizeof(header));
        activeUb->writeLen = 64;
    }
}

class TestSimdVfDebugPublish : public testing::Test {
protected:
    void SetUp() override
    {
        previousCore = g_coreType;
        previousFifo = g_sysPrintFifoSpace;
        g_coreType = AscendC::AIV;
        gm = {};
        ub = {};
        gm.head.magic = 0xAE86;
        gm.head.length = sizeof(GmBlock);
        gm.head.ringBufLen = RING_BYTES;
        gm.head.ringBufAddr = reinterpret_cast<uint64_t>(gm.data.data());
        gm.head.debugBusAddr = 0x12345678;
        g_sysPrintFifoSpace = reinterpret_cast<uint8_t*>(&gm);
        activeGm = &gm;
        activeUb = &ub;
        flushes.clear();
        appendAtCommit = false;
        // Two complete scalar TLVs. The publisher must preserve the batch count.
        const uint32_t header[2] = {static_cast<uint32_t>(DumpType::DUMP_SCALAR), 24};
        std::memcpy(ub.buffer, header, sizeof(header));
        std::memcpy(ub.buffer + 32, header, sizeof(header));
        ub.buffer[24] = 7;
        ub.buffer[56] = 9;
        ub.writeLen = BATCH_BYTES;
        MOCKER(get_coreid).stubs().will(returnValue(int64_t(0)));
        MOCKER(dcci, void(void*, uint64_t, uint64_t)).stubs().will(invoke(RecordFlush));
    }
    void TearDown() override
    {
        GlobalMockObject::verify();
        g_sysPrintFifoSpace = previousFifo;
        g_coreType = previousCore;
    }
    template <typename T> void Wire(uint32_t offset, T value)
    {
        std::memcpy(ub.buffer + offset, &value, sizeof(value));
    }
    template <typename T> T Output(uint32_t offset)
    {
        T value;
        std::memcpy(&value, gm.data.data() + offset, sizeof(value));
        return value;
    }
    void Reference(const char* fmt, uint32_t length, uint32_t args = 0, uint32_t inlineBytes = 0)
    {
        ub = {};
        ub.writeLen = (48 + 16 * args + inlineBytes + 7) & ~7U;
        Wire<uint32_t>(0, 0x80000001U);
        Wire<uint32_t>(4, ub.writeLen - 8);
        Wire<uint16_t>(8, 1);
        Wire<uint16_t>(10, 1);
        Wire<uint32_t>(12, 19);
        Wire<uint32_t>(16, args);
        Wire<uint32_t>(20, inlineBytes);
        Wire<uint64_t>(24, reinterpret_cast<uint64_t>(fmt));
        Wire<uint32_t>(32, length);
    }
    void Argument(uint32_t index, uint32_t kind, uint32_t length, uint64_t value)
    {
        Wire<uint32_t>(48 + index * 16, kind);
        Wire<uint32_t>(52 + index * 16, length);
        Wire<uint64_t>(56 + index * 16, value);
    }
    void Publish() { asc_vf_debug_publish(&ub, ub.writeLen, ub.readLen); }
    void ExpectNoCommit()
    {
        EXPECT_EQ(gm.writer.bufOffset, 0U);
        EXPECT_EQ(gm.writer.packIdx, 0U);
    }
    GmBlock gm{};
    BlockVFBufInfo ub{};
    uint8_t* previousFifo = nullptr;
    int32_t previousCore = 0;
};

TEST_F(TestSimdVfDebugPublish, SetsAivTypeAndPublishesEachRecordAfterPayloadFlush)
{
    ASSERT_EQ(gm.head.flag, 0U);
    Publish();
    EXPECT_EQ(gm.head.flag, 1U);
    EXPECT_EQ(gm.head.debugBusAddr, 0x12345678U);
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, BATCH_BYTES);
    EXPECT_EQ(gm.writer.bufOffset, BATCH_BYTES);
    EXPECT_EQ(gm.writer.packIdx, 2U);
    EXPECT_EQ(std::memcmp(ub.buffer, gm.data.data(), BATCH_BYTES), 0);
    ASSERT_EQ(flushes.size(), 5U);
    EXPECT_EQ(flushes[0].address, &gm.head);
    for (uint32_t i = 0; i < 2; ++i) {
        const auto& payload = flushes[1 + 2 * i];
        const auto& commit = flushes[2 + 2 * i];
        EXPECT_EQ(payload.address, gm.data.data() + 32 * i);
        EXPECT_EQ(payload.writeOffset, 32U * i);
        EXPECT_EQ(payload.ubRead, 32U * i);
        EXPECT_EQ(commit.address, &gm.writer);
        EXPECT_EQ(commit.writeOffset, 32U * (i + 1));
        EXPECT_EQ(commit.packages, i + 1U);
        EXPECT_EQ(commit.ubRead, 32U * i);
    }
    for (const auto& flush : flushes) {
        EXPECT_EQ(flush.coreType, 1U);
    }
}

TEST_F(TestSimdVfDebugPublish, StandardGetterRetainsAicType)
{
    g_coreType = AscendC::AIC;
    gm.head.flag = 1;
    Publish();
    EXPECT_EQ(gm.head.flag, 0U);
    EXPECT_EQ(gm.writer.packIdx, 2U);
}

TEST_F(TestSimdVfDebugPublish, NullFifoStopsWithoutConsuming)
{
    g_sysPrintFifoSpace = nullptr;
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    EXPECT_EQ(ub.readLen, 0U);
    ExpectNoCommit();
    EXPECT_TRUE(flushes.empty());
}

TEST_F(TestSimdVfDebugPublish, InvalidMagicStopsWithoutConsuming)
{
    gm.head.magic = 0;
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    EXPECT_EQ(ub.readLen, 0U);
    ExpectNoCommit();
    EXPECT_TRUE(flushes.empty());
}

TEST_F(TestSimdVfDebugPublish, MalformedBatchStopsBeforeGmAccess)
{
    const uint32_t invalidLength = BATCH_BYTES;
    std::memcpy(ub.buffer + 4, &invalidLength, sizeof(invalidLength));
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    EXPECT_EQ(ub.readLen, 0U);
    ExpectNoCommit();
    EXPECT_TRUE(flushes.empty());
}

TEST_F(TestSimdVfDebugPublish, PermanentCapacityFailureDoesNotPublish)
{
    gm.head.ringBufLen = 16;
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    EXPECT_EQ(ub.readLen, 0U); // No unpublished record is acknowledged.
    ExpectNoCommit();
}

TEST_F(TestSimdVfDebugPublish, TransientFailureCanRetryTheSameBatch)
{
    MOCKER(check_ringbuf_space).stubs().will(returnValue(false)).then(returnValue(true));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 0U);
    ExpectNoCommit();
    ASSERT_EQ(flushes.size(), 1U);
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, BATCH_BYTES);
    EXPECT_EQ(gm.writer.bufOffset, BATCH_BYTES);
    EXPECT_EQ(gm.writer.packIdx, 2U);
}
TEST_F(TestSimdVfDebugPublish, GmReferenceExpandsScalarsGmStringsAndInlineSnapshot)
{
    const char fmt[] = "%d %s %s";
    const char text[] = "GM";
    Reference(fmt, sizeof(fmt), 3, 4);
    Argument(0, 0, 0, 0xfedcba9876543210ULL);
    Argument(1, 1, sizeof(text), reinterpret_cast<uint64_t>(text));
    Argument(2, 2, 4, 96);
    std::memcpy(ub.buffer + 96, "old", 4);
    gm.data.fill(0xcc);
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 104U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_EQ(gm.writer.bufOffset, 72U);
    EXPECT_EQ(Output<uint32_t>(0), 1U);
    EXPECT_EQ(Output<uint32_t>(4), 64U);
    EXPECT_EQ(Output<uint32_t>(8), 19U);
    EXPECT_EQ(Output<uint64_t>(16), 32U);
    EXPECT_EQ(Output<uint64_t>(24), 0xfedcba9876543210ULL);
    EXPECT_EQ(Output<uint64_t>(32), 33U);
    EXPECT_EQ(Output<uint64_t>(40), 28U);
    EXPECT_EQ(std::memcmp(gm.data.data() + 48, fmt, sizeof(fmt)), 0);
    EXPECT_EQ(std::memcmp(gm.data.data() + 65, text, sizeof(text)), 0);
    EXPECT_EQ(std::memcmp(gm.data.data() + 68, "old", 4), 0);
    EXPECT_EQ(Output<uint32_t>(12), 0U);
    for (uint32_t i = 57; i < 65; ++i) {
        EXPECT_EQ(gm.data[i], 0U);
    }
    ASSERT_EQ(flushes.size(), 3U);
    EXPECT_EQ(flushes[1].address, gm.data.data());
    EXPECT_EQ(flushes[1].writeOffset, 0U);
    EXPECT_EQ(flushes[2].address, &gm.writer);
    EXPECT_EQ(flushes[2].writeOffset, 72U);
    EXPECT_EQ(flushes[2].ubRead, 0U);
}

TEST_F(TestSimdVfDebugPublish, GmReferenceBodyCanExceedUbCapacity)
{
    std::vector<char> fmt(3001, 'x');
    fmt.back() = 0;
    Reference(fmt.data(), fmt.size());
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 48U);
    EXPECT_EQ(gm.writer.bufOffset, 3040U);
    EXPECT_EQ(Output<uint32_t>(0), 1U);
    EXPECT_EQ(std::memcmp(gm.data.data() + 24, fmt.data(), fmt.size()), 0);
}

TEST_F(TestSimdVfDebugPublish, GmReferenceAssertConcatenatesPrefix)
{
    const char fmt[] = "message";
    const char prefix[] = "assert: ";
    Reference(fmt, sizeof(fmt));
    Wire<uint16_t>(10, 4);
    Wire<uint32_t>(36, sizeof(prefix) - 1);
    Wire<uint64_t>(40, reinterpret_cast<uint64_t>(prefix));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(Output<uint32_t>(0), 4U);
    EXPECT_EQ(std::memcmp(gm.data.data() + 24, "assert: message", 16), 0);
}

TEST_F(TestSimdVfDebugPublish, InvalidGmReferenceNeverCommits)
{
    const char fmt[] = "%s";
    const char unterminated[] = {'n', 'o'};
    for (uint32_t invalid = 0; invalid < 18; ++invalid) {
        SCOPED_TRACE(invalid);
        // GM termination is a producer contract. The consumer must not probe an
        // arbitrary GM address merely to validate the trailing NUL.
        if (invalid == 11 || invalid == 12) {
            continue;
        }
        gm.writer = {};
        Reference(fmt, sizeof(fmt), 1, 2);
        Argument(0, 2, 2, 64);
        std::memcpy(ub.buffer + 64, "x", 2);
        switch (invalid) {
            case 0: Wire<uint16_t>(8, 2); break;
            case 1: Wire<uint32_t>(48, 9); break;
            case 2: Wire<uint64_t>(56, 63); break;
            case 3: Wire<uint32_t>(52, 3); break;
            case 4: ub.buffer[65] = 'x'; break;
            case 5: Wire<uint64_t>(24, 0); break;
            case 6: Wire<uint32_t>(32, 0); break;
            case 7: Wire<uint64_t>(24, UINT64_MAX - 1); break;
            case 8: Wire<uint32_t>(16, UINT32_MAX); break;
            case 9: Wire<uint16_t>(10, 2); break;
            case 10: Argument(0, 0, 1, 0); break;
            case 11: Argument(0, 1, 2, reinterpret_cast<uint64_t>(unterminated)); break;
            case 12: Wire<uint64_t>(24, reinterpret_cast<uint64_t>(unterminated));
                     Wire<uint32_t>(32, sizeof(unterminated)); break;
            case 13: Argument(0, 1, 1, 0); break;
            case 14: Argument(0, 1, 2, UINT64_MAX); break;
            case 15: Wire<uint32_t>(36, 1); Wire<uint64_t>(40, 0); break;
            case 16: Wire<uint32_t>(20, UINT32_MAX); break;
            case 17: Wire<uint32_t>(36, UINT32_MAX); break;
        }
        Publish();
        EXPECT_EQ(ub.flag, 1U);
        ExpectNoCommit();
    }
}

TEST_F(TestSimdVfDebugPublish, ExpandedRecordTooLargeStopsWithoutPublishing)
{
    Reference("x", 2);
    Wire<uint32_t>(32, UINT32_MAX);
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    ExpectNoCommit();
}

TEST_F(TestSimdVfDebugPublish, MixedRecordsRetryStartsAtFirstUnpublishedItem)
{
    Reference("x", 2);
    std::memmove(ub.buffer + 32, ub.buffer, 48);
    const uint32_t oldHeader[] = {1, 24};
    std::memset(ub.buffer, 0, 32);
    std::memcpy(ub.buffer, oldHeader, sizeof(oldHeader));
    ub.writeLen = 80;
    MOCKER(check_ringbuf_space).stubs().will(returnValue(true)).then(returnValue(false)).then(returnValue(true));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 32U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_EQ(gm.writer.bufOffset, 32U);
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 80U);
    EXPECT_EQ(gm.writer.packIdx, 2U);
    EXPECT_EQ(gm.writer.bufOffset, 72U);
    EXPECT_EQ(Output<uint32_t>(32), 1U);
    EXPECT_EQ(gm.data[56], 'x');
}
TEST_F(TestSimdVfDebugPublish, MalformedLaterRecordRetainsEarlierCommit)
{
    Wire<uint32_t>(36, UINT32_MAX);
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    EXPECT_EQ(ub.readLen, 32U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_EQ(gm.writer.bufOffset, 32U);
    EXPECT_EQ(std::memcmp(ub.buffer, gm.data.data(), 32), 0);
}

TEST_F(TestSimdVfDebugPublish, EmptyGmStringsAreValid)
{
    const char empty[] = "";
    Reference(empty, 1, 1);
    Argument(0, 1, 1, reinterpret_cast<uint64_t>(empty));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 64U);
    EXPECT_EQ(gm.writer.bufOffset, 48U);
    EXPECT_EQ(Output<uint32_t>(0), 1U);
    EXPECT_EQ(gm.data[32], 0U);
    EXPECT_EQ(Output<uint64_t>(24), 17U);
    EXPECT_EQ(gm.data[41], 0U);
}
TEST_F(TestSimdVfDebugPublish, RetryPreservesSkipPublishedBeforeTemporaryFailure)
{
    Reference("x", 2);
    gm.writer.bufOffset = RING_BYTES - 12;
    gm.reader.bufOffset = 40;
    MOCKER(ringbuf_overflow_wait).stubs().will(returnValue(false)).then(returnValue(true));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 0U);
    EXPECT_EQ(gm.writer.bufOffset, 0U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_EQ(Output<uint32_t>(RING_BYTES - 12), static_cast<uint32_t>(DumpType::DUMP_SKIP));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 48U);
    EXPECT_EQ(gm.writer.bufOffset, 40U);
    EXPECT_EQ(gm.writer.packIdx, 2U);
    EXPECT_EQ(Output<uint32_t>(0), 1U);
}

TEST_F(TestSimdVfDebugPublish, ProducerAppendDuringCommitWaitsForNextSnapshot)
{
    ub.writeLen = 32;
    appendAtCommit = true;
    Publish();
    EXPECT_EQ(ub.writeLen, 64U);
    EXPECT_EQ(ub.readLen, 32U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    Publish();
    EXPECT_EQ(ub.readLen, 64U);
    EXPECT_EQ(gm.writer.packIdx, 2U);
    EXPECT_EQ(gm.writer.bufOffset, 64U);
}
} // namespace

TEST_F(TestSimdVfDebugPublish, CStringReferenceScansFormatAndStringArgument)
{
    const char fmt[] = "%d %s";
    const char text[] = "hello";
    Reference(fmt, 0, 2);
    Wire<uint32_t>(0, 0x80000002U);
    Argument(0, 0, 0, 10);
    Argument(1, 3, 0, reinterpret_cast<uint64_t>(text));
    Publish();
    ASSERT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 80U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_EQ(Output<uint64_t>(24), 10U);
    EXPECT_STREQ(reinterpret_cast<const char*>(gm.data.data() + 40), fmt);
    EXPECT_STREQ(reinterpret_cast<const char*>(gm.data.data() + 32 + Output<uint64_t>(32)), text);
}

TEST_F(TestSimdVfDebugPublish, CStringEmptyAndLongFormats)
{
    for (uint32_t size : {1U, 3001U}) {
        std::vector<char> fmt(size, 'x');
        fmt.back() = 0;
        gm.writer = {};
        Reference(fmt.data(), 0);
        Wire<uint32_t>(0, 0x80000002U);
        Publish();
        ASSERT_EQ(ub.flag, 0U);
        EXPECT_EQ(ub.readLen, 48U);
        EXPECT_EQ(std::memcmp(gm.data.data() + 24, fmt.data(), size), 0);
    }
}

TEST_F(TestSimdVfDebugPublish, CStringScanRespectsCapacityAndIncludesTerminator)
{
    // Use the physical FIFO size so its trailing writer remains at the correct address.
    constexpr uint32_t maxFormat = RING_BYTES - 8 - 24;
    for (uint32_t length : {maxFormat, maxFormat + 1}) {
        std::vector<char> fmt(length, 'x');
        fmt.back() = 0;
        gm.writer = {};
        Reference(fmt.data(), 0);
        Wire<uint32_t>(0, 0x80000002U);
        Publish();
        if (length == maxFormat) {
            EXPECT_EQ(ub.flag, 0U);
            EXPECT_EQ(gm.writer.bufOffset, RING_BYTES);
        } else {
            EXPECT_EQ(ub.flag, 1U);
            EXPECT_EQ(ub.readLen, 0U);
            ExpectNoCommit();
        }
    }
}

TEST_F(TestSimdVfDebugPublish, CStringMissingTerminatorAndNullAreRejected)
{
    std::array<char, RING_BYTES - 8 - 24> text;
    text.fill('x');
    for (const char* fmt : {text.data(), static_cast<char*>(nullptr)}) {
        Reference(fmt, 0);
        Wire<uint32_t>(0, 0x80000002U);
        Publish();
        EXPECT_EQ(ub.flag, 1U);
        EXPECT_EQ(ub.readLen, 0U);
        ExpectNoCommit();
    }
}

TEST_F(TestSimdVfDebugPublish, CStringValidatesAllMetadataBeforeReadingFormat)
{
    Reference(reinterpret_cast<const char*>(1), 0, 1);
    Wire<uint32_t>(0, 0x80000002U);
    Argument(0, 99, 0, 0); // Invalid kind must reject before dereferencing format address 1.
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    ExpectNoCommit();
}

TEST_F(TestSimdVfDebugPublish, CStringCombinedStringsShareOneCapacityBudget)
{
    const char fmt[] = "%s";
    std::array<char, RING_BYTES - 8 - 32 - sizeof(fmt)> text;
    text.fill('x');
    text.back() = 0;
    // Remaining scan budget excludes header, scalar slot and format including NUL.
    Reference(fmt, 0, 1);
    Wire<uint32_t>(0, 0x80000002U);
    Argument(0, 3, 0, reinterpret_cast<uint64_t>(text.data()));
    Publish();
    ASSERT_EQ(ub.flag, 0U);
    EXPECT_EQ(gm.writer.bufOffset, RING_BYTES);
    gm.writer = {};
    text.back() = 'x'; // readable budget, but no terminator within it
    Reference(fmt, 0, 1);
    Wire<uint32_t>(0, 0x80000002U);
    Argument(0, 3, 0, reinterpret_cast<uint64_t>(text.data()));
    Publish();
    EXPECT_EQ(ub.flag, 1U);
    ExpectNoCommit();
}

TEST_F(TestSimdVfDebugPublish, CStringSpaceRetryUsesResolvedRecord)
{
    const char fmt[] = "retry";
    Reference(fmt, 0);
    Wire<uint32_t>(0, 0x80000002U);
    MOCKER(check_ringbuf_space).stubs().will(returnValue(false)).then(returnValue(true));
    Publish();
    EXPECT_EQ(ub.flag, 0U);
    EXPECT_EQ(ub.readLen, 0U);
    ExpectNoCommit();
    auto* rec = reinterpret_cast<__asc_simd_vf::GmPrintRecord*>(ub.buffer);
    EXPECT_EQ(rec->type, 0x80000001U);
    EXPECT_EQ(rec->fmtByteLen, sizeof(fmt));
    Publish();
    EXPECT_EQ(ub.readLen, 48U);
    EXPECT_EQ(gm.writer.packIdx, 1U);
    EXPECT_STREQ(reinterpret_cast<const char*>(gm.data.data() + 24), fmt);
}

TEST_F(TestSimdVfDebugPublish, CStringSupportsSizedFormatAndInlineArgument)
{
    const char fmt[] = "%s %s";
    const char text[] = "";
    Reference(fmt, sizeof(fmt), 2, 3);
    Wire<uint32_t>(0, 0x80000002U);
    Argument(0, 3, 0, reinterpret_cast<uint64_t>(text));
    Argument(1, 2, 3, 80);
    std::memcpy(ub.buffer + 80, "ub", 3);
    Publish();
    ASSERT_EQ(ub.flag, 0U);
    EXPECT_STREQ(reinterpret_cast<const char*>(gm.data.data() + 24 + Output<uint64_t>(24)), "");
    EXPECT_STREQ(reinterpret_cast<const char*>(gm.data.data() + 32 + Output<uint64_t>(32)), "ub");
}
