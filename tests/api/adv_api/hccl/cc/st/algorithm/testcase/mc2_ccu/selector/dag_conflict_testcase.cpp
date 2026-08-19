/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_dag_checker.h"
#include "ccu_kernel_proxy.h"

#include <gtest/gtest.h>

using namespace HcclSim::CcuSt;

namespace {

ExecutionLogEntry MakeEntry(uint32_t rank, size_t opIndex, OpCode code,
                            uint32_t srcOwner, uint64_t srcAddr, uint64_t srcSize,
                            uint32_t dstOwner, uint64_t dstAddr, uint64_t dstSize)
{
    ExecutionLogEntry entry;
    entry.rank = rank;
    entry.operationIndex = opIndex;
    entry.code = code;
    entry.source = {srcOwner, srcAddr, srcSize, false};
    entry.destination = {dstOwner, dstAddr, dstSize, true};
    return entry;
}

Operation MakeNotifyRecord(ChannelHandle channel, uint32_t notifyIndex)
{
    Operation op;
    op.code = OpCode::NOTIFY_RECORD;
    op.channel = channel;
    op.index = notifyIndex;
    return op;
}

Operation MakeNotifyWait(ChannelHandle channel, uint32_t notifyIndex)
{
    Operation op;
    op.code = OpCode::NOTIFY_WAIT;
    op.channel = channel;
    op.index = notifyIndex;
    return op;
}

} // namespace

// ---------------------------------------------------------------------------
// Positive: empty log never conflicts.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, EmptyLogPassesDagCheck)
{
    ExecutionLog log;
    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ---------------------------------------------------------------------------
// Positive: two ranks write to their own memory (different owners) — no
// overlap, no conflict even without synchronization.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, NonOverlappingWritesPassDagCheck)
{
    ExecutionLog log;
    log.rankEntries.resize(2);
    log.programs.resize(2, nullptr);

    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    log.rankEntries[1].push_back(
        MakeEntry(1, 0, OpCode::WRITE, 1, 3000, 50, 1, 4000, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ---------------------------------------------------------------------------
// Positive: two ranks write to the same owner but at adjacent (touching, not
// overlapping) address ranges — no conflict.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, AdjacentNonOverlappingWritesPassDagCheck)
{
    ExecutionLog log;
    log.rankEntries.resize(2);
    log.programs.resize(2, nullptr);

    // [2000, 2050) and [2050, 2100) touch but do not overlap.
    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    log.rankEntries[1].push_back(
        MakeEntry(1, 0, OpCode::WRITE, 1, 3000, 50, 0, 2050, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ---------------------------------------------------------------------------
// Positive: two ranks write to the SAME overlapping region, but a
// NOTIFY_RECORD -> NOTIFY_WAIT synchronization edge orders them. The DAG
// checker must respect the happens-before edge and report no conflict.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, ProperlySynchronizedOverlappingWritesPassDagCheck)
{
    Program program0;
    program0.operations.push_back({}); // placeholder for WRITE at index 0
    program0.operations[0].code = OpCode::WRITE;
    program0.operations.push_back(
        MakeNotifyRecord(MakeChannel(0, 1), 1)); // NOTIFY_RECORD at index 1

    Program program1;
    program1.operations.push_back(
        MakeNotifyWait(MakeChannel(1, 0), 1)); // NOTIFY_WAIT at index 0
    program1.operations.push_back({}); // placeholder for WRITE at index 1
    program1.operations[1].code = OpCode::WRITE;

    ExecutionLog log;
    log.programs = {&program0, &program1};
    log.rankEntries.resize(2);

    // Both ranks write to owner 0 at overlapping [2000,2050) / [2020,2070).
    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    log.rankEntries[1].push_back(
        MakeEntry(1, 1, OpCode::WRITE, 1, 3000, 50, 0, 2020, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ---------------------------------------------------------------------------
// Negative: two ranks write to the SAME overlapping region with no
// synchronization at all. The DAG checker must detect the conflict.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, DetectsUnsynchronizedOverlappingWrites)
{
    ExecutionLog log;
    log.rankEntries.resize(2);
    log.programs.resize(2, nullptr);

    // [2000,2050) and [2020,2070) overlap on owner 0.
    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    log.rankEntries[1].push_back(
        MakeEntry(1, 0, OpCode::WRITE, 1, 3000, 50, 0, 2020, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_FALSE(result.Ok());
    EXPECT_EQ(result.result, Result::STATE_ERROR);
}

// ---------------------------------------------------------------------------
// Negative: rank 0 writes while rank 1 reads the same overlapping region
// (write-read hazard) without synchronization. The DAG checker must detect
// the conflict.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, DetectsUnsynchronizedWriteReadConflict)
{
    ExecutionLog log;
    log.rankEntries.resize(2);
    log.programs.resize(2, nullptr);

    // Rank 0 writes to owner 0 [2000,2050); rank 1 reads from owner 0
    // [2020,2070) — destination vs source overlap.
    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    // source (read) overlaps rank 0's destination (write).
    log.rankEntries[1].push_back(
        MakeEntry(1, 0, OpCode::WRITE, 0, 2020, 50, 1, 8000, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_FALSE(result.Ok());
    EXPECT_EQ(result.result, Result::STATE_ERROR);
}

// ---------------------------------------------------------------------------
// Negative: same overlapping writes but with programs present yet NO matching
// sync edges — the conflict must still be detected, proving the checker does
// not falsely assume synchronization.
// ---------------------------------------------------------------------------
TEST(CcuStDagTest, OverlappingWritesWithUnmatchedSyncStillConflicts)
{
    Program program0;
    program0.operations.push_back({}); // WRITE at index 0
    program0.operations[0].code = OpCode::WRITE;

    Program program1;
    program1.operations.push_back({}); // WRITE at index 0
    program1.operations[0].code = OpCode::WRITE;

    ExecutionLog log;
    log.programs = {&program0, &program1};
    log.rankEntries.resize(2);

    // Overlapping writes on owner 0, no NOTIFY edges at all.
    log.rankEntries[0].push_back(
        MakeEntry(0, 0, OpCode::WRITE, 0, 1000, 50, 0, 2000, 50));
    log.rankEntries[1].push_back(
        MakeEntry(1, 0, OpCode::WRITE, 1, 3000, 50, 0, 2020, 50));

    CcuDagChecker checker;
    CheckResult result = checker.Check(log);
    EXPECT_FALSE(result.Ok());
    EXPECT_EQ(result.result, Result::STATE_ERROR);
}
