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

#include "sim_common.h"
#include "sim_npu.h"
#include "sim_task_queue.h"
#include "sim_world.h"
#include "task_graph_executor.h"

using namespace HcclSim;
using namespace HcclSim::AicpuSt;

namespace {
class TaskGraphExecutorTest : public ::testing::Test {
protected:
    void SetUp() override
    {
        SimWorld::Global()->Deinit();
        SimWorld::Global()->Init(TopoMeta{{{{0, 1}}}}, DevType::DEV_TYPE_950);
        SimTaskQueue::Global()->Clear();
        streamCache_.clear(); // 世界重建后旧流指针失效，随 SetUp 一并清空
    }

    void TearDown() override { SimWorld::Global()->Deinit(); }

    // 在 rank 的流上投递任务（模拟 Hcomm stub 的 AppendTask 行为：流按 (rank, streamIdx)
    // 只分配一次并复用——主流是单次分配语义，重复 AllocMainStream 返回 nullptr；真实
    // stub 中 SimHcclThread 同样持有一次分配的流句柄，每次下发任务复用同一指针）
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

    u8* Base(RankId rank, BufferType type, uint64_t size)
    {
        return SimWorld::Global()->GetSimNpuByRankId(rank).GetBufferBase(type, size);
    }

private:
    std::map<std::pair<RankId, uint32_t>, SimStream*> streamCache_;
};
} // namespace

TEST_F(TaskGraphExecutorTest, LocalCopyMovesBytes)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t outAddr = npu0.AllocMemory(BufferType::OUTPUT, 8);
    u8* in = Base(0, BufferType::INPUT, 8);
    for (uint64_t i = 0; i < 8; ++i) {
        in[i] = static_cast<u8>(0xA0 + i);
    }

    DataSlice src;
    ASSERT_EQ(npu0.GetSlice(inAddr, 8, src), HcclResult::HCCL_SUCCESS);
    DataSlice dst;
    ASSERT_EQ(npu0.GetSlice(outAddr, 8, dst), HcclResult::HCCL_SUCCESS);
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src, dst));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    u8* out = Base(0, BufferType::OUTPUT, 8);
    EXPECT_EQ(memcmp(out, in, 8), 0);
    const auto& log = executor.ExecutionLog();
    ASSERT_EQ(log.size(), 1U);
    ASSERT_EQ(log[0].accesses.size(), 2U);
    EXPECT_EQ(log[0].rank, 0U);
    EXPECT_EQ(log[0].streamId, 0U);
    EXPECT_EQ(log[0].taskIndex, 0U);
    EXPECT_EQ(log[0].type, TaskTypeStub::LOCAL_COPY);
    EXPECT_FALSE(log[0].accesses[0].isWrite); // src 读
    EXPECT_TRUE(log[0].accesses[1].isWrite);  // dst 写
    EXPECT_EQ(log[0].accesses[1].bufferType, BufferType::OUTPUT);
    EXPECT_EQ(log[0].accesses[1].offset, 0U);
    EXPECT_EQ(log[0].accesses[1].size, 8U);
}

TEST_F(TaskGraphExecutorTest, WriteMovesBytesCrossRank)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::OUTPUT, 4);
    u8* in = Base(0, BufferType::INPUT, 4);
    in[0] = 1;
    in[1] = 2;
    in[2] = 3;
    in[3] = 4;

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(inAddr, 4, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 4, remote), HcclResult::HCCL_SUCCESS);
    Append(0, 0, std::make_shared<TaskStubWrite>(1, LinkInfo(LinkProtoStub::SDMA), local, remote));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    u8* out = Base(1, BufferType::OUTPUT, 4);
    EXPECT_EQ(out[0], 1);
    EXPECT_EQ(out[3], 4);
}

TEST_F(TaskGraphExecutorTest, ReadPullsFromRemoteRank)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t locAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::CCL, 4);
    Base(1, BufferType::CCL, 4)[0] = 0x5A;

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(locAddr, 4, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 4, remote), HcclResult::HCCL_SUCCESS);
    Append(0, 0, std::make_shared<TaskStubRead>(1, LinkInfo(LinkProtoStub::SDMA), local, remote));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(Base(0, BufferType::OUTPUT, 4)[0], 0x5A);
}

TEST_F(TaskGraphExecutorTest, SameQueuePreservesFifoOrder)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t outAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    DataSlice src;
    ASSERT_EQ(npu0.GetSlice(inAddr, 4, src), HcclResult::HCCL_SUCCESS);
    DataSlice dst;
    ASSERT_EQ(npu0.GetSlice(outAddr, 4, dst), HcclResult::HCCL_SUCCESS);

    Base(0, BufferType::INPUT, 4)[0] = 0x11;
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src, dst)); // 先写 0x11
    Base(0, BufferType::INPUT, 4)[0] = 0x22;
    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src, dst)); // 后写 0x22

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(Base(0, BufferType::OUTPUT, 4)[0], 0x22); // FIFO：后写覆盖先写
}

// ===== Task 1 质量审查遗留：SimNpu 后备内存边界语义 =====
TEST_F(TaskGraphExecutorTest, BackingGrowthPreservesData)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t addr = npu0.AllocMemory(BufferType::CCL, 32);
    (void)addr;
    Base(0, BufferType::CCL, 32)[31] = 0xCD;
    Base(0, BufferType::CCL, 64);                      // 扩容到 64
    EXPECT_EQ(Base(0, BufferType::CCL, 64)[31], 0xCD); // 旧数据保持
}

TEST_F(TaskGraphExecutorTest, ReallocSmallerKeepsBackingAndSliceBoundsFollowNewSize)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t addr = npu0.AllocMemory(BufferType::INPUT, 64);
    Base(0, BufferType::INPUT, 64)[63] = 0xEF;
    const uint64_t addrAgain = npu0.AllocMemory(BufferType::INPUT, 32); // 同块重复分配更小 size
    EXPECT_EQ(addrAgain, addr);                                         // startAddr 不变
    EXPECT_EQ(Base(0, BufferType::INPUT, 64)[63], 0xEF);                // 后备不缩
    DataSlice slice;
    EXPECT_NE(npu0.GetSlice(addr + 40, 8, slice), HcclResult::HCCL_SUCCESS); // 越界按新 size 拒绝
}

// 执行期同块扩容：src/dst 同在 rank0 CCL（未预分配，后备从空开始），
// 解析 src 触发扩容后 dst 指针必须仍有效（预扩容防御的真实场景）。
TEST_F(TaskGraphExecutorTest, SameBackingGrowthDuringExecuteKeepsBothEndsValid)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    MemBlock ccl = npu0.GetMemBlock(BufferType::CCL);
    // CCL 块布局声明 200MB，GetSlice 可直接成功，后备内存为空
    DataSlice src;
    ASSERT_EQ(npu0.GetSlice(ccl.startAddr, 4, src), HcclResult::HCCL_SUCCESS);
    DataSlice dst;
    ASSERT_EQ(npu0.GetSlice(ccl.startAddr + 8, 4, dst), HcclResult::HCCL_SUCCESS);
    Base(0, BufferType::CCL, 4)[0] = 0x9E; // 写入 src[0]（首次触碰，后备长到 4）
    Base(0, BufferType::CCL, 4)[1] = 0x9F;

    Append(0, 0, std::make_shared<TaskStubLocalCopy>(src, dst));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(Base(0, BufferType::CCL, 12)[8], 0x9E); // dst 未因扩容悬空
    EXPECT_EQ(Base(0, BufferType::CCL, 12)[9], 0x9F);
}

TEST_F(TaskGraphExecutorTest, PostThenWaitSynchronizes)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    // rank1 先 WAIT 再做搬运；rank0 POST。跨队列同步由 notify 兑现。
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::OUTPUT, 4);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    Base(0, BufferType::INPUT, 4)[0] = 0x77;

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(inAddr, 4, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 4, remote), HcclResult::HCCL_SUCCESS);

    const uint32_t notifyId = 0x40;
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));
    Append(1, 0, std::make_shared<TaskStubWrite>(1, LinkInfo(LinkProtoStub::SDMA), local, remote)); // 占位顺序任务
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    const auto& log = executor.ExecutionLog();
    bool foundWait = false;
    for (const auto& e : log) {
        if (e.type == TaskTypeStub::LOCAL_WAIT_FROM && e.rank == 1 && e.notifyId == notifyId) {
            foundWait = true;
            EXPECT_NE(e.satisfiedBySeq, 0U);
        }
    }
    EXPECT_TRUE(foundWait);
}

TEST_F(TaskGraphExecutorTest, WaitWithoutPostIsDeadlock)
{
    const uint32_t notifyId = 0x99;
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));

    TaskGraphExecutor executor;
    std::string report;
    EXPECT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::DEADLOCK);
    EXPECT_NE(report.find("deadlock detected"), std::string::npos);
    EXPECT_NE(report.find("LOCAL_WAIT_FROM"), std::string::npos);
    EXPECT_NE(report.find("pending=0"), std::string::npos);
}

TEST_F(TaskGraphExecutorTest, ChannelNotifyRecordPostsRemoteWaitConsumes)
{
    // channel 语义：POST 到远端 rank 的 notifyId；本端 WAIT 消费本端 notifyId。
    // notifyId 全局唯一，执行引擎仅按 id 配对。
    const uint32_t rmtNotifyId = 0x100;
    const uint32_t locNotifyId = 0x101;
    Append(
        0, 0,
        std::make_shared<TaskStubPost>(1, LinkInfo(LinkProtoStub::SDMA), rmtNotifyId, NotifyTypeStub::READY, "POST"));
    Append(
        1, 0,
        std::make_shared<TaskStubWait>(0, LinkInfo(LinkProtoStub::SDMA), rmtNotifyId, NotifyTypeStub::READY, "WAIT"));
    Append(1, 1, std::make_shared<TaskStubLocalPostTo>(locNotifyId));
    Append(
        0, 1,
        std::make_shared<TaskStubWait>(1, LinkInfo(LinkProtoStub::SDMA), locNotifyId, NotifyTypeStub::READY, "WAIT"));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(executor.ExecutionLog().size(), 4U);
}
