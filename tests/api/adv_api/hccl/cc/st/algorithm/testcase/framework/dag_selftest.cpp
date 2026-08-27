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
#include "ccu_dag_types.h"
#include "ccu_st_types.h"

#include <gtest/gtest.h>

using namespace HcclSim::CcuSt;

namespace {

ExecutionLogEntry MakeEntry(uint32_t rank, size_t opIdx, OpCode code,
    uint32_t srcRank, uint64_t srcAddr, uint64_t srcSize,
    uint32_t dstRank, uint64_t dstAddr, uint64_t dstSize)
{
    ExecutionLogEntry e;
    e.rank = rank;
    e.operationIndex = opIdx;
    e.code = code;
    e.source = {srcRank, srcAddr, srcSize, false};
    e.destination = {dstRank, dstAddr, dstSize, true};
    return e;
}

ExecutionLog MakeLog(const std::vector<std::vector<ExecutionLogEntry>>& perRank)
{
    ExecutionLog log;
    log.rankEntries = perRank;
    log.programs.resize(perRank.size(), nullptr);
    return log;
}

} // namespace

// ===== L4: 无冲突场景 =====

TEST(CcuStFrameworkL4, NoConflictSequential)
{
    // Same rank, sequential ops on different addresses — connected by sequential edge
    auto log = MakeLog({{
        MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100),
        MakeEntry(0, 1, OpCode::WRITE, 0, 0x1100, 100, 1, 0x2100, 100),
    }});
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL4, NoConflictDiffRankDiffAddr)
{
    // Different ranks writing to different addresses — no conflict
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100)},
        {MakeEntry(1, 0, OpCode::WRITE, 1, 0x3000, 100, 0, 0x4000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL4, NoConflictReadOnly)
{
    // Concurrent reads of the same address — no conflict (read is not write)
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::READ, 1, 0x1000, 100, 0, 0x2000, 100)},
        {MakeEntry(1, 0, OpCode::READ, 1, 0x1000, 100, 1, 0x3000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL4, NoConflictConnectedByEdge)
{
    // Two writes to same address but connected by notify edge — no conflict
    // Rank 0 writes, then notifies rank 1, rank 1 waits then writes
    // We simulate this by having sequential entries + notify connection
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100)},
        {MakeEntry(0, 1, OpCode::NOTIFY_RECORD, 0, 0, 0, 0, 0, 0)},
        {MakeEntry(1, 0, OpCode::NOTIFY_WAIT, 0, 0, 0, 0, 0, 0)},
        {MakeEntry(1, 1, OpCode::WRITE, 1, 0x3000, 100, 0, 0x2000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ===== L4: 冲突检测 =====

TEST(CcuStFrameworkL4, ConflictWriteWrite)
{
    // Two ranks write to the same destination without dependency
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100)},
        {MakeEntry(1, 0, OpCode::WRITE, 1, 0x3000, 100, 1, 0x2000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL4, ConflictWriteRead)
{
    // Rank 0 writes to dst, rank 1 reads from same dst (as source) — conflict
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100)},
        {MakeEntry(1, 0, OpCode::WRITE, 1, 0x2000, 100, 1, 0x3000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL4, ConflictReduceWrite)
{
    // Rank 0 reduces (writes) to dst, rank 1 writes to same dst
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::LOCAL_REDUCE, 0, 0x1000, 100, 0, 0x2000, 100)},
        {MakeEntry(1, 0, OpCode::WRITE, 1, 0x3000, 100, 0, 0x2000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_FALSE(result.Ok());
}

// ===== L4: 空日志 =====

TEST(CcuStFrameworkL4, EmptyLog)
{
    ExecutionLog log;
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok());
}

TEST(CcuStFrameworkL4, SingleEntry)
{
    auto log = MakeLog({
        {MakeEntry(0, 0, OpCode::WRITE, 0, 0x1000, 100, 1, 0x2000, 100)},
    });
    auto result = CcuDagChecker().Check(log);
    EXPECT_TRUE(result.Ok());
}
