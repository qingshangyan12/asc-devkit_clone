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

#include <cstring>

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
    EXPECT_EQ(memcmp(out, in, 4), 0);
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

// 跨队列门控（数据因果版）：被门控的读取所取数据，由 rank1 在 POST 之后的轮次才写入。
// 轮询序 ctxs=[r0主流, r1主流]（队列 map 按 (pod,server,phy) 升序，同一 sweep 内 r0 先于 r1 推进队头）。
// 三轮推演（门控正确，CanExecute 拦 WAIT）：
//   round1: r0 的 WAIT 因 0x40 未置位而阻塞（r1 的 POST 尚未执行）；r1 的 POST 置位 0x40。
//   round2: r0 的 WAIT 消费信号放行（satisfiedBySeq=POST 的 seq）；r1 的 Write 将新值 0x22
//           写入 r1 自己的 INPUT(src) 块（Write 在 rank1 队列中位于 POST 之后）。
//   round3: r0 的 Read 拉取 r1 的 INPUT → r0 的 OUTPUT=0x22——读发生在写之后，读到新值。
// 门控失效（CanExecute 恒 true）：
//   round1: r0 的 WAIT 立即执行（satisfiedBySeq=0）；r1 的 POST 置位。
//   round2: r0 的 Read 先于 r1 的 Write 执行（r0 轮询在前）→ 读到旧值 0x11 → 数据断言 FAIL
//           （且 WAIT.satisfiedBySeq==0 亦 FAIL）——门控回归可被数据面观测。
// 注：src 块在 rank1 名下，跨 rank 读取须用 TaskStubRead（LocalCopy 双端均按入队 rank 解析，
// 读不到远端块）；rank1 的 Write 为自写：local 端是自己的 OUTPUT 暂存块（预填新值 0x22），
// remote 端是自己的 src(INPUT) 块。
TEST_F(TaskGraphExecutorTest, PostThenWaitSynchronizes)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    // rank0 的读取源 = rank1 将在 POST 之后写入的 INPUT 块
    const uint64_t srcAddr = npu1.AllocMemory(BufferType::INPUT, 4);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    // rank1 的 Write 暂存块（新值来源）
    const uint64_t stagingAddr = npu1.AllocMemory(BufferType::OUTPUT, 4);
    u8* srcBase = Base(1, BufferType::INPUT, 4);    // 旧值预填
    u8* stageBase = Base(1, BufferType::OUTPUT, 4); // 新值暂存
    for (uint64_t i = 0; i < 4; ++i) {
        srcBase[i] = 0x11;
        stageBase[i] = 0x22;
    }

    DataSlice srcSlice; // rank1 的 INPUT 块
    ASSERT_EQ(npu1.GetSlice(srcAddr, 4, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice; // rank0 的 OUTPUT 块
    ASSERT_EQ(npu0.GetSlice(dstAddr, 4, dstSlice), HcclResult::HCCL_SUCCESS);
    DataSlice stagingSlice; // rank1 的 OUTPUT 暂存块
    ASSERT_EQ(npu1.GetSlice(stagingAddr, 4, stagingSlice), HcclResult::HCCL_SUCCESS);

    const uint32_t notifyId = 0x40;
    // rank1：先发信号，下一轮才把新值写进自己的 INPUT(src)
    Append(1, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));
    Append(1, 0, std::make_shared<TaskStubWrite>(1, LinkInfo(LinkProtoStub::SDMA), stagingSlice, srcSlice));
    // rank0：等信号，放行后才拉取 rank1 的 INPUT
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));
    Append(0, 0, std::make_shared<TaskStubRead>(1, LinkInfo(LinkProtoStub::SDMA), dstSlice, srcSlice));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;

    const auto& log = executor.ExecutionLog();
    const ExecEvent* wait = nullptr;
    for (const auto& e : log) {
        if (e.type == TaskTypeStub::LOCAL_WAIT_FROM && e.rank == 0 && e.notifyId == notifyId) {
            wait = &e;
        }
    }
    ASSERT_NE(wait, nullptr);
    EXPECT_NE(wait->satisfiedBySeq, 0U); // 门控正确：WAIT 由真实 POST 满足（非空跑）
    // 门控因果：被门控的读必须观察到 POST 之后写入的新值（失效路径读到 0x11）
    const u8* out = Base(0, BufferType::OUTPUT, 4);
    for (uint64_t i = 0; i < 4; ++i) {
        EXPECT_EQ(out[i], 0x22U) << "gated read must observe the post-POST write (byte " << i << ")";
    }
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

// 同 id 多轮 POST/WAIT（ping-pong，AllGather ring 的实际同步模式）：
// 每轮 WAIT 必须由"最近一次 POST"满足（satisfiedBySeq 精确指向），且消费后清零。
TEST_F(TaskGraphExecutorTest, PingPongReuseNotifyIdWithExactSatisfiedBySeq)
{
    const uint32_t notifyId = 0x50;
    // 第一轮：rank0 POST → rank1 WAIT；第二轮：rank1 POST → rank0 WAIT
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));
    Append(1, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;

    const auto& log = executor.ExecutionLog();
    ASSERT_EQ(log.size(), 4U);
    // 按执行序找事件：POST/WAIT/POST/WAIT（轮询每个 sweep 内 rank0 与 rank1 各推进队头，第一轮 POST1→WAIT1 完成后第二轮
    // POST2→WAIT2）
    const ExecEvent* post1 = nullptr;
    const ExecEvent* wait1 = nullptr;
    const ExecEvent* post2 = nullptr;
    const ExecEvent* wait2 = nullptr;
    for (const auto& e : log) {
        if (e.type == TaskTypeStub::LOCAL_POST_TO && e.notifyId == notifyId) {
            if (post1 == nullptr) {
                post1 = &e;
            } else {
                post2 = &e;
            }
        }
        if (e.type == TaskTypeStub::LOCAL_WAIT_FROM && e.notifyId == notifyId) {
            if (wait1 == nullptr) {
                wait1 = &e;
            } else {
                wait2 = &e;
            }
        }
    }
    ASSERT_NE(post1, nullptr);
    ASSERT_NE(wait1, nullptr);
    ASSERT_NE(post2, nullptr);
    ASSERT_NE(wait2, nullptr);
    EXPECT_EQ(wait1->satisfiedBySeq, post1->seq); // 精确指向第一轮 POST
    EXPECT_EQ(wait2->satisfiedBySeq, post2->seq); // 精确指向第二轮 POST（非第一轮残留）
}

// 一次性信号：POST→WAIT 消费后，同 id 第二次 WAIT 无重 POST 必须死锁。
TEST_F(TaskGraphExecutorTest, SecondWaitAfterConsumedPostDeadlocks)
{
    const uint32_t notifyId = 0x51;
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId));
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId)); // 消费后再等 → 死锁

    TaskGraphExecutor executor;
    std::string report;
    EXPECT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::DEADLOCK);
    EXPECT_NE(report.find("deadlock detected"), std::string::npos);
}

TEST_F(TaskGraphExecutorTest, LocalReduceInt32Sum)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t srcAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 8);
    auto* src = reinterpret_cast<int32_t*>(Base(0, BufferType::INPUT, 8));
    auto* dst = reinterpret_cast<int32_t*>(Base(0, BufferType::OUTPUT, 8));
    src[0] = 1;
    src[1] = 2;
    dst[0] = 10;
    dst[1] = 20;

    DataSlice srcSlice;
    ASSERT_EQ(npu0.GetSlice(srcAddr, 8, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice;
    ASSERT_EQ(npu0.GetSlice(dstAddr, 8, dstSlice), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubLocalReduce>(
            srcSlice, dstSlice, HcclDataType::HCCL_DATA_TYPE_INT32, HcclReduceOp::HCCL_REDUCE_SUM));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(dst[0], 11);
    EXPECT_EQ(dst[1], 22);
}

TEST_F(TaskGraphExecutorTest, WriteReduceFp16Sum)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::OUTPUT, 4);
    // FP16 位模式：1.0=0x3C00, 2.0=0x4000, 3.0=0x4200
    auto* inU16 = reinterpret_cast<uint16_t*>(Base(0, BufferType::INPUT, 4));
    auto* rmtU16 = reinterpret_cast<uint16_t*>(Base(1, BufferType::OUTPUT, 4));
    inU16[0] = 0x3C00;
    inU16[1] = 0x4000;
    rmtU16[0] = 0x4000;
    rmtU16[1] = 0x3C00;

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(inAddr, 4, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 4, remote), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubWriteReduce>(
            1, LinkInfo(LinkProtoStub::SDMA), local, remote, HcclDataType::HCCL_DATA_TYPE_FP16,
            HcclReduceOp::HCCL_REDUCE_SUM));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(rmtU16[0], 0x4200); // 2.0 + 1.0 = 3.0
    EXPECT_EQ(rmtU16[1], 0x4200);
}

TEST_F(TaskGraphExecutorTest, ReadReduceBfp16Max)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t locAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::CCL, 4);
    // BFP16 = FP32 高 16 位：1.0=0x3F80, 2.0=0x4000
    auto* locU16 = reinterpret_cast<uint16_t*>(Base(0, BufferType::OUTPUT, 4));
    auto* rmtU16 = reinterpret_cast<uint16_t*>(Base(1, BufferType::CCL, 4));
    rmtU16[0] = 0x3F80;
    rmtU16[1] = 0x4000;
    locU16[0] = 0x4000; // local 2.0 > remote 1.0：MAX 取 local，可区分 OVERRIDE 误实现
    locU16[1] = 0x3F80; // local 1.0 < remote 2.0：MAX 取 remote，可区分空执行

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(locAddr, 4, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 4, remote), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubReadReduce>(
            1, LinkInfo(LinkProtoStub::SDMA), local, remote, HcclDataType::HCCL_DATA_TYPE_BFP16,
            HcclReduceOp::HCCL_REDUCE_MAX));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(locU16[0], 0x4000);
    EXPECT_EQ(locU16[1], 0x4000);
}

// 非法 reduce op 必须 THROW（fail-fast，不静默降级为拷贝）
TEST_F(TaskGraphExecutorTest, LocalReduceInvalidOpThrows)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t srcAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    DataSlice srcSlice;
    ASSERT_EQ(npu0.GetSlice(srcAddr, 4, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice;
    ASSERT_EQ(npu0.GetSlice(dstAddr, 4, dstSlice), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubLocalReduce>(
            srcSlice, dstSlice, HcclDataType::HCCL_DATA_TYPE_INT8, static_cast<HcclReduceOp>(0xFF))); // 非法 op

    TaskGraphExecutor executor;
    std::string report;
    // THROW 在 Execute 的调度路径中抛出，未被捕获将终止进程——本用例验证其抛出。
    // 抛出后执行器状态已部分推进，用例只断言抛出，不检查后续状态。
    EXPECT_ANY_THROW((void)executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report));
}

// INT32 大值精确：2^24+1 在 float 不可精确表示（float 路径会回写 16777216）
TEST_F(TaskGraphExecutorTest, LocalReduceInt32Beyond24BitExact)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t srcAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    auto* src = reinterpret_cast<int32_t*>(Base(0, BufferType::INPUT, 4));
    auto* dst = reinterpret_cast<int32_t*>(Base(0, BufferType::OUTPUT, 4));
    src[0] = 16777217; // 2^24 + 1
    dst[0] = 0;

    DataSlice srcSlice;
    ASSERT_EQ(npu0.GetSlice(srcAddr, 4, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice;
    ASSERT_EQ(npu0.GetSlice(dstAddr, 4, dstSlice), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubLocalReduce>(
            srcSlice, dstSlice, HcclDataType::HCCL_DATA_TYPE_INT32, HcclReduceOp::HCCL_REDUCE_SUM));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(dst[0], 16777217); // int64 路径精确；float 路径会得 16777216
}

// 置位吸收：double-post 的 WAIT 由最老 POST 满足（跨队列时最新 POST 不蕴含最老，HB sound 取最老）。
// 轮询序 ctxs=[rank0主流, rank0从流, rank1主流]：两 POST 须同 sweep 先后执行（第二个落在置位期间被吸收），
// WAIT 放 rank1 主流 POST 之后（下一 sweep 消费）——否则 rank0 从流的 WAIT 会在 rank1 POST 前消费信号，用例失效。
TEST_F(TaskGraphExecutorTest, DoublePostAbsorbedKeepsOldestSeq)
{
    const uint32_t notifyId = 0x60;
    Append(0, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));   // 先执行（轮询序 rank0 主流在前）
    Append(1, 0, std::make_shared<TaskStubLocalPostTo>(notifyId));   // 同 sweep 置位期间 → 被吸收
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(notifyId)); // 下一 sweep 消费

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    const auto& log = executor.ExecutionLog();
    const ExecEvent* firstPost = nullptr;
    const ExecEvent* secondPost = nullptr;
    const ExecEvent* wait = nullptr;
    for (const auto& e : log) {
        if (e.type == TaskTypeStub::LOCAL_POST_TO && e.notifyId == notifyId) {
            if (firstPost == nullptr) {
                firstPost = &e;
            } else {
                secondPost = &e;
            }
        }
        if (e.type == TaskTypeStub::LOCAL_WAIT_FROM && e.notifyId == notifyId) {
            wait = &e;
        }
    }
    ASSERT_NE(firstPost, nullptr);
    ASSERT_NE(secondPost, nullptr);
    ASSERT_NE(wait, nullptr);
    ASSERT_LT(firstPost->seq, secondPost->seq); // 轮询序：rank0 主流在前
    // 不变量：satisfiedBySeq == 两 POST 中 seq 较小者——最老（首个）POST，非被吸收的第二个；
    // 覆盖式（吸收前）实现会记录 secondPost->seq，本断言可检出该回归
    EXPECT_EQ(wait->satisfiedBySeq, firstPost->seq);
}

// PROD + 负数 + INT8 双向饱和（int64 精确路径）
TEST_F(TaskGraphExecutorTest, LocalReduceInt8ProdNegativeAndSaturation)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t srcAddr = npu0.AllocMemory(BufferType::INPUT, 4);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 4);
    auto* src = reinterpret_cast<int8_t*>(Base(0, BufferType::INPUT, 4));
    auto* dst = reinterpret_cast<int8_t*>(Base(0, BufferType::OUTPUT, 4));
    src[0] = -3;
    src[1] = 20;
    src[2] = -20;
    src[3] = 5;
    dst[0] = 1;
    dst[1] = 10;
    dst[2] = 10;
    dst[3] = -1;

    DataSlice srcSlice;
    ASSERT_EQ(npu0.GetSlice(srcAddr, 4, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice;
    ASSERT_EQ(npu0.GetSlice(dstAddr, 4, dstSlice), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubLocalReduce>(
            srcSlice, dstSlice, HcclDataType::HCCL_DATA_TYPE_INT8, HcclReduceOp::HCCL_REDUCE_PROD));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(dst[0], -3);   // 1 × (-3)
    EXPECT_EQ(dst[1], 127);  // 10 × 20 = 200 → 饱和上界
    EXPECT_EQ(dst[2], -128); // 10 × (-20) = -200 → 饱和下界
    EXPECT_EQ(dst[3], -5);   // (-1) × 5
}

// MIN + INT32 负数
TEST_F(TaskGraphExecutorTest, LocalReduceInt32MinNegative)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    const uint64_t srcAddr = npu0.AllocMemory(BufferType::INPUT, 8);
    const uint64_t dstAddr = npu0.AllocMemory(BufferType::OUTPUT, 8);
    auto* src = reinterpret_cast<int32_t*>(Base(0, BufferType::INPUT, 8));
    auto* dst = reinterpret_cast<int32_t*>(Base(0, BufferType::OUTPUT, 8));
    src[0] = -100;
    src[1] = 7;
    dst[0] = 50;
    dst[1] = -7;

    DataSlice srcSlice;
    ASSERT_EQ(npu0.GetSlice(srcAddr, 8, srcSlice), HcclResult::HCCL_SUCCESS);
    DataSlice dstSlice;
    ASSERT_EQ(npu0.GetSlice(dstAddr, 8, dstSlice), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubLocalReduce>(
            srcSlice, dstSlice, HcclDataType::HCCL_DATA_TYPE_INT32, HcclReduceOp::HCCL_REDUCE_MIN));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(dst[0], -100); // min(50, -100)
    EXPECT_EQ(dst[1], -7);   // min(-7, 7)
}

// PROD + FP16 负数（float 路径 + 跨 rank WriteReduce）：-1.5 × 2.0 = -3.0
TEST_F(TaskGraphExecutorTest, WriteReduceFp16ProdNegative)
{
    SimNpu& npu0 = SimWorld::Global()->GetSimNpuByRankId(0);
    SimNpu& npu1 = SimWorld::Global()->GetSimNpuByRankId(1);
    const uint64_t inAddr = npu0.AllocMemory(BufferType::INPUT, 2);
    const uint64_t rmtAddr = npu1.AllocMemory(BufferType::OUTPUT, 2);
    auto* inU16 = reinterpret_cast<uint16_t*>(Base(0, BufferType::INPUT, 2));
    auto* rmtU16 = reinterpret_cast<uint16_t*>(Base(1, BufferType::OUTPUT, 2));
    inU16[0] = 0xBE00;  // -1.5
    rmtU16[0] = 0x4000; // 2.0

    DataSlice local;
    ASSERT_EQ(npu0.GetSlice(inAddr, 2, local), HcclResult::HCCL_SUCCESS);
    DataSlice remote;
    ASSERT_EQ(npu1.GetSlice(rmtAddr, 2, remote), HcclResult::HCCL_SUCCESS);
    Append(
        0, 0,
        std::make_shared<TaskStubWriteReduce>(
            1, LinkInfo(LinkProtoStub::SDMA), local, remote, HcclDataType::HCCL_DATA_TYPE_FP16,
            HcclReduceOp::HCCL_REDUCE_PROD));

    TaskGraphExecutor executor;
    std::string report;
    ASSERT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::SUCCESS) << report;
    EXPECT_EQ(rmtU16[0], 0xC200); // 2.0 × (-1.5) = -3.0
}

// 多队列死锁：报告必须列出每个阻塞队列的卡点（诊断完整性——BuildDeadlockReport 遍历全部队列）
TEST_F(TaskGraphExecutorTest, MultiQueueDeadlockReportsAllBlockedQueues)
{
    const uint32_t idA = 0x71;
    const uint32_t idB = 0x72;
    const uint32_t idC = 0x73;
    Append(0, 0, std::make_shared<TaskStubLocalWaitFrom>(idA));
    Append(1, 0, std::make_shared<TaskStubLocalWaitFrom>(idB));
    Append(1, 1, std::make_shared<TaskStubLocalWaitFrom>(idC)); // rank1 从流也阻塞

    TaskGraphExecutor executor;
    std::string report;
    EXPECT_EQ(executor.Execute(SimTaskQueue::Global()->GetAllRankTaskQueues(), report), ExecResult::DEADLOCK);
    EXPECT_NE(report.find("rank 0 stream 0"), std::string::npos);
    EXPECT_NE(report.find("rank 1 stream 0"), std::string::npos);
    EXPECT_NE(report.find("rank 1 stream 1"), std::string::npos);
    // 三条 pending=0（从未 POST）
    size_t pos = 0;
    uint32_t pendingCount = 0;
    while ((pos = report.find("pending=0", pos)) != std::string::npos) {
        ++pendingCount;
        pos += sizeof("pending=0") - 1;
    }
    EXPECT_EQ(pendingCount, 3U);
}
