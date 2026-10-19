/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "aicpu_dag_checker.h"
#include "sim_common.h"
#include "sim_npu.h"
#include "sim_task_queue.h"
#include "sim_world.h"
#include "task_graph_executor.h"

using namespace HcclSim;
using namespace HcclSim::AicpuSt;

namespace {
class AicpuDagCheckerTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        SimWorld::Global()->Deinit();
        SimWorld::Global()->Init(TopoMeta{{{{0, 1}}}}, DevType::DEV_TYPE_950);
        SimTaskQueue::Global()->Clear();
        streamCache_.clear(); // 世界重建后旧流指针失效，随 SetUp 一并清空
    }

    void TearDown() override { SimWorld::Global()->Deinit(); }

    // 在 rank 的流上投递任务（与 TaskGraphExecutorTest 相同约定：主流单次分配语义，
    // 流按 (rank, streamIdx) 只分配一次并复用）
    void Append(RankId rank, uint32_t streamIdx, std::shared_ptr<TaskStub> task)
    {
        auto key = std::make_pair(rank, streamIdx);
        auto it = streamCache_.find(key);
        if (it == streamCache_.end()) {
            SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(rank);
            void* stream = (streamIdx == 0) ? npu.AllocMainStream() : npu.AllocSlaveStream();
            ASSERT_NE(stream, nullptr);
            it = streamCache_.emplace(key, reinterpret_cast<SimStream*>(stream)).first;
        }
        SimTaskQueue::Global()->AppendTask(SimWorld::Global()->GetNpuPosByRankId(rank), it->second, task);
    }

    // 构造指向 rank0 OUTPUT 块 [0, size) 区间的 slice 对
    void MakeOutSlices(uint64_t size, DataSlice& a, DataSlice& b)
    {
        SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
        const uint64_t addr = npu0.AllocMemory(BufferType::OUTPUT, size);
        ASSERT_EQ(npu0.GetSlice(addr, size, a), HcclResult::HCCL_SUCCESS);
        ASSERT_EQ(npu0.GetSlice(addr, size, b), HcclResult::HCCL_SUCCESS);
    }

    // 构造 rank 自身 INPUT 块 [0, size) 的 slice（跨 rank 任务的本地读源，独立组不与 OUTPUT 写重叠）
    void MakeInSlice(RankId rank, uint64_t size, DataSlice& slice)
    {
        SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(rank);
        const uint64_t addr = npu.AllocMemory(BufferType::INPUT, size);
        ASSERT_EQ(npu.GetSlice(addr, size, slice), HcclResult::HCCL_SUCCESS);
    }

private:
    std::map<std::pair<RankId, uint32_t>, SimStream*> streamCache_;
};
} // namespace

// 同步序列：rank0 写 + POST → rank1 WAIT → rank1 跨 rank 写同一区间。
// 读写有 happens-before（程序序 + POST->WAIT 同步边），无冲突。
TEST_F(AicpuDagCheckerTest, SyncedAccessesHaveNoConflict)
{
    DataSlice a;
    DataSlice b;
    MakeOutSlices(8, a, b);
    DataSlice src0;
    DataSlice src1;
    MakeInSlice(0, 8, src0);
    MakeInSlice(1, 8, src1);
    const uint32_t notifyId = 0x30;
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src0, a)); // rank0 写 rank0 OUTPUT[0,8)
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));
    // rank1 同步后跨 rank 写 rank0 同一区间（WRITE 的远端访问按 slice 属主 rank 归档）
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), src1, b));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    EXPECT_TRUE(checker.Check(executor.ExecutionLog()).empty());
}

// 无同步的跨队列 WAW：rank0 本地写与 rank1 跨 rank 写并发落在 rank0 OUTPUT 同一区间 → 报 WAW。
// 读源取各自 INPUT（独立组），保证两事件间唯一重叠访问是两条 WRITE。
TEST_F(AicpuDagCheckerTest, UnsynchronizedWawIsConflict)
{
    DataSlice a;
    DataSlice b;
    MakeOutSlices(8, a, b);
    DataSlice src0;
    DataSlice src1;
    MakeInSlice(0, 8, src0);
    MakeInSlice(1, 8, src1);
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src0, a)); // rank0 写 rank0 OUTPUT[0,8)
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), src1, b)); // 无同步写同一区间

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    const auto conflicts = checker.Check(executor.ExecutionLog());
    ASSERT_EQ(conflicts.size(), 1U);
    EXPECT_EQ(conflicts[0].kind, "WAW");
    EXPECT_NE(conflicts[0].detailA.find("WRITE"), std::string::npos);
}

// 部分重叠：[0,8) 与 [4,12) 交叠 [4,8)，仍报 1 条 WAW（区间判定含部分交叠，非仅全包含）。
TEST_F(AicpuDagCheckerTest, PartialOverlapWawIsConflict)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t outAddr = npu0.AllocMemory(BufferType::OUTPUT, 12);
    DataSlice a;
    ASSERT_EQ(npu0.GetSlice(outAddr, 8, a), HcclResult::HCCL_SUCCESS);
    DataSlice b;
    ASSERT_EQ(npu0.GetSlice(outAddr + 4, 8, b), HcclResult::HCCL_SUCCESS); // [4,12) 与 [0,8) 部分交叠
    DataSlice src0;
    DataSlice src1;
    MakeInSlice(0, 8, src0);
    MakeInSlice(1, 8, src1);
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src0, a)); // rank0 写 rank0 OUTPUT[0,8)
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), src1, b)); // 写 [4,12)

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    const auto conflicts = checker.Check(executor.ExecutionLog());
    ASSERT_EQ(conflicts.size(), 1U);
    EXPECT_EQ(conflicts[0].kind, "WAW");
}

// 无同步的跨队列读写：rank1 READ（读 rank0 INPUT）与 rank0 本地写 INPUT 并发 → 报冲突。
TEST_F(AicpuDagCheckerTest, UnsynchronizedReadWriteIsConflict)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::OUTPUT, 8);
    DataSlice local;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 8, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, remote), HcclResult::HCCL_SUCCESS);

    // rank1 READ：读 rank0 INPUT[0,8)（remote 读）
    Append(1, 0, std::make_shared<TaskStubRead>(0, LinkInfo(LinkProtoStub::SDMA), local, remote));
    // rank0 无同步地写同一 INPUT 区间
    DataSlice selfA;
    DataSlice selfB;
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, selfA), HcclResult::HCCL_SUCCESS);
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, selfB), HcclResult::HCCL_SUCCESS);
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(selfA, selfB));
    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    const auto conflicts = checker.Check(executor.ExecutionLog());
    ASSERT_EQ(conflicts.size(), 1U);
    // 时序推导：执行器按 (pod,server,phy) 有序 map 轮询，rank0 队列先于 rank1；首个 sweep 中
    // rank0 的 LocalCopy（seq1）先执行、rank1 的 READ（seq2）后执行。INPUT 组唯一跨队列重叠对是
    // (LocalCopy dst 写@seq1, READ remote 读@seq2)——写先读后 => RAW。
    EXPECT_EQ(conflicts[0].kind, "RAW");
}

// RAR（双读）不报：两个队列无同步并发读同一区间，双读无写不构成冲突。
// 两个 READ 的写落点分别在各自 rank 的 OUTPUT 组，不与 INPUT 组双读交叉。
TEST_F(AicpuDagCheckerTest, ReadAfterReadNeverConflicts)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t out1 = npu1.AllocMemory(BufferType::OUTPUT, 8);
    const uint64_t out0 = npu0.AllocMemory(BufferType::OUTPUT, 8);
    DataSlice local1; // rank1 读结果的落点（rank1 OUTPUT）
    ASSERT_EQ(npu1.GetSlice(out1, 8, local1), HcclResult::HCCL_SUCCESS);
    DataSlice remote; // rank0 INPUT[0,8)
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, remote), HcclResult::HCCL_SUCCESS);
    DataSlice dst2; // rank0 读结果的落点（rank0 OUTPUT）
    ASSERT_EQ(npu0.GetSlice(out0, 8, dst2), HcclResult::HCCL_SUCCESS);
    DataSlice src2; // rank0 也读自己的 INPUT[0,8)（remote=0 自读）
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, src2), HcclResult::HCCL_SUCCESS);
    // rank1 与 rank0 各自读 rank0 的 INPUT[0,8)（无同步，双读）
    Append(1, 0, std::make_shared<TaskStubRead>(0, LinkInfo(LinkProtoStub::SDMA), local1, remote));
    Append(0, 0, std::make_shared<TaskStubRead>(0, LinkInfo(LinkProtoStub::SDMA), dst2, src2));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_TRUE(AicpuDagChecker().Check(executor.ExecutionLog()).empty()); // RAR 不报
}

// 同队列内读写：程序序保证，不报。
TEST_F(AicpuDagCheckerTest, SameQueueNeverConflicts)
{
    DataSlice a;
    DataSlice b;
    MakeOutSlices(8, a, b);
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(a, b));
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(b, a));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    EXPECT_TRUE(checker.Check(executor.ExecutionLog()).empty());
}

// 无同步的跨队列 WAR（读先写后）：rank0 先执行 READ（远端读打在 (r0, INPUT)），
// rank1 后执行 WRITE（远端写同一区间）→ a=读 时序在先 → kind 必须是 WAR
TEST_F(AicpuDagCheckerTest, UnsynchronizedWarIsConflict)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t out0 = npu0.AllocMemory(BufferType::OUTPUT, 8);
    const uint64_t out1 = npu1.AllocMemory(BufferType::OUTPUT, 8);
    DataSlice remote;
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, remote), HcclResult::HCCL_SUCCESS);
    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(out0, 8, local), HcclResult::HCCL_SUCCESS);
    DataSlice wLocal;
    ASSERT_EQ(npu1.GetSlice(out1, 8, wLocal), HcclResult::HCCL_SUCCESS);
    DataSlice wRemote;
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, wRemote), HcclResult::HCCL_SUCCESS);

    // rank0（轮询序在前）：READ 自读——远端读打在 (r0, INPUT)
    Append(0, 0, std::make_shared<TaskStubRead>(0, LinkInfo(LinkProtoStub::SDMA), local, remote));
    // rank1（后执行）：WRITE 远端写同一 (r0, INPUT)
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), wLocal, wRemote));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    AicpuDagChecker checker;
    const auto conflicts = checker.Check(executor.ExecutionLog());
    ASSERT_EQ(conflicts.size(), 1U);
    EXPECT_EQ(conflicts[0].kind, "WAR"); // 读先写后
    EXPECT_NE(conflicts[0].detailA.find("READ"), std::string::npos);
}

// 三队列两跳传递 HB：writeA(qA) --PO--> POST1 --sync--> WAIT1(qB) --PO--> writeB --PO--> POST2
//                  --sync--> WAIT2(qC) --PO--> writeC。writeA 与 writeC 仅靠传递性建立先后，
// 若 Reachable 退化为单跳邻接，writeA vs writeC 会被误报 WAW。
TEST_F(AicpuDagCheckerTest, TransitiveHappensBeforeAcrossThreeQueues)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t out0 = npu0.AllocMemory(BufferType::OUTPUT, 16); // [0,8)=争用区 [8,16)=源
    const uint64_t out1 = npu1.AllocMemory(BufferType::OUTPUT, 8);
    DataSlice srcA;
    ASSERT_EQ(npu0.GetSlice(out0 + 8, 8, srcA), HcclResult::HCCL_SUCCESS);
    DataSlice targetA;
    ASSERT_EQ(npu0.GetSlice(out0, 8, targetA), HcclResult::HCCL_SUCCESS);
    DataSlice bSrc;
    ASSERT_EQ(npu1.GetSlice(out1, 8, bSrc), HcclResult::HCCL_SUCCESS);
    DataSlice bDst;
    ASSERT_EQ(npu0.GetSlice(out0, 8, bDst), HcclResult::HCCL_SUCCESS);

    const uint32_t id1 = 0x80;
    const uint32_t id2 = 0x81;
    // qA = rank0 主流：写争用区 → POST id1
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(srcA, targetA));
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(id1));
    // qB = rank1 主流：等 id1 → 跨 rank 写同一争用区 → POST id2
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(id1));
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), bSrc, bDst));
    Append(1, 0, std::make_shared<TaskStubLocalPostTo>(id2));
    // qC = rank0 从流：等 id2 → 再写争用区
    Append(0, 1, std::make_shared<TaskStubLocalWaitFrom>(id2));
    Append(0, 1, std::make_shared<TaskStubLocalCopy>(srcA, targetA));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    // 传递 HB 覆盖 writeA/writeB/writeC 三对跨队列写——零冲突
    EXPECT_TRUE(AicpuDagChecker().Check(executor.ExecutionLog()).empty());
}

// 零大小访问豁免：size=0 的 slice 与重叠区间不构成冲突（守卫回归——无守卫时 [4,4) 会误判入 [0,8)）
TEST_F(AicpuDagCheckerTest, ZeroSizeAccessIsExemptFromConflict)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t out0 = npu0.AllocMemory(BufferType::OUTPUT, 8);
    const uint64_t out1 = npu1.AllocMemory(BufferType::OUTPUT, 8);
    DataSlice zero;
    ASSERT_EQ(npu0.GetSlice(out0 + 4, 0, zero), HcclResult::HCCL_SUCCESS); // size=0 @offset 4
    DataSlice wLocal;
    ASSERT_EQ(npu1.GetSlice(out1, 8, wLocal), HcclResult::HCCL_SUCCESS);
    DataSlice wRemote;
    ASSERT_EQ(npu0.GetSlice(out0, 8, wRemote), HcclResult::HCCL_SUCCESS);

    Append(0, 0, std::make_shared<TaskStubLocalCopy>(zero, zero));                                    // 零大小搬运
    Append(1, 0, std::make_shared<TaskStubWrite>(0, LinkInfo(LinkProtoStub::SDMA), wLocal, wRemote)); // 整段写

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_TRUE(AicpuDagChecker().Check(executor.ExecutionLog()).empty());
}
