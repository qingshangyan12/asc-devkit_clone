/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "task_graph_executor.h"

#include <cstring>

#include "aicpu_float_convert.h"
#include "exception_util.h"
#include "sim_npu.h"
#include "sim_world.h"

namespace HcclSim {
namespace AicpuSt {
namespace {

// 解析 slice 到可读写指针。注意：GetBufferBase 可能触发后备内存扩容（vector 重新分配），
// 会使同块先前取得的裸指针失效——因此每次使用都重新解析，禁止缓存跨 GetBufferBase 调用的指针。
u8* ResolveSlicePtr(RankId rank, const DataSlice& slice)
{
    SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(rank);
    u8* base = npu.GetBufferBase(slice.GetType(), slice.GetOffset() + slice.GetSize());
    return base + slice.GetOffset();
}

// 先对两端各完成一次后备扩容（防同块扩容使先取的指针悬空），再解析出双端指针。
// 所有搬运/归约任务必须经此取指针，禁止直接分别调 ResolveSlicePtr。
void ResolveEndpoints(
    RankId srcRank, const DataSlice& srcSlice, RankId dstRank, const DataSlice& dstSlice, const u8*& srcPtr,
    u8*& dstPtr)
{
    SimNpu& srcNpu = SimWorld::Global()->GetSimNpuByRankId(srcRank);
    (void)srcNpu.GetBufferBase(srcSlice.GetType(), srcSlice.GetOffset() + srcSlice.GetSize());
    SimNpu& dstNpu = SimWorld::Global()->GetSimNpuByRankId(dstRank);
    (void)dstNpu.GetBufferBase(dstSlice.GetType(), dstSlice.GetOffset() + dstSlice.GetSize());
    srcPtr = ResolveSlicePtr(srcRank, srcSlice);
    dstPtr = ResolveSlicePtr(dstRank, dstSlice);
    if (srcSlice.GetSize() != dstSlice.GetSize()) {
        THROW<InvalidParamsException>(
            "[TaskGraphExecutor] slice size mismatch: src[%llu] dst[%llu]", srcSlice.GetSize(), dstSlice.GetSize());
    }
}

// WAIT 系任务的统一取 notifyId 入口（TaskStubWait / TaskStubLocalWaitFrom）
uint32_t GetWaitNotifyId(const TaskStub* task)
{
    if (task->GetType() != TaskTypeStub::WAIT && task->GetType() != TaskTypeStub::LOCAL_WAIT_FROM) {
        THROW<InvalidParamsException>(
            "[TaskGraphExecutor] GetWaitNotifyId on non-wait task[%s]", task->GetType().Describe().c_str());
    }
    return (task->GetType() == TaskTypeStub::WAIT) ? static_cast<const TaskStubWait*>(task)->GetTopicId() :
                                                     static_cast<const TaskStubLocalWaitFrom*>(task)->GetTopicId();
}

// 逐元素归约 src 进 dst。端点解析必须经 ResolveEndpoints（先两端扩容，防同块扩容悬空；
// 内含 size 一致性 THROW 防护）——这是 Task 2 质量审查确立的强制契约。
// 精度边界（spec §12.2）：归约经 float 中间精度——INT32 输入 |值|>2^24 引入舍入误差；
// INT8/INT32 存储饱和（StoreElem）；FP16 按 RNE、BFP16 按截断（aicpu_float_convert.h）。
void ReduceInto(
    RankId dstRank, const DataSlice& dstSlice, RankId srcRank, const DataSlice& srcSlice, HcclDataType dt,
    HcclReduceOp op)
{
    const uint64_t elem = ElemSize(dt);
    if (elem == 0 || dstSlice.GetSize() % elem != 0) {
        THROW<InvalidParamsException>(
            "[TaskGraphExecutor] reduce invalid elem size[%llu] dataType[%u]", elem, static_cast<uint32_t>(dt));
    }
    const u8* srcPtr = nullptr;
    u8* dstPtr = nullptr;
    ResolveEndpoints(srcRank, srcSlice, dstRank, dstSlice, srcPtr, dstPtr);
    const uint64_t count = dstSlice.GetSize() / elem;
    for (uint64_t i = 0; i < count; ++i) {
        const float acc = ApplyOp(LoadElem(dstPtr + i * elem, dt), LoadElem(srcPtr + i * elem, dt), op);
        StoreElem(dstPtr + i * elem, dt, acc);
    }
}

MemAccess MakeAccess(RankId rank, const DataSlice& slice, bool isWrite)
{
    MemAccess access;
    access.rank = rank;
    access.bufferType = slice.GetType();
    access.offset = slice.GetOffset();
    access.size = slice.GetSize();
    access.isWrite = isWrite;
    return access;
}

// rank 与 NpuPos 的反查：CommDomain 按 topoMeta 向量顺序编号 rank，
// 不能依赖 AllRankTaskQueues 的 map 迭代序（按 key 值排序）。
std::map<std::tuple<uint32_t, uint32_t, uint32_t>, RankId> BuildPosToRank()
{
    std::map<std::tuple<uint32_t, uint32_t, uint32_t>, RankId> posToRank;
    const TopoMeta topoMeta = SimWorld::Global()->GetTopoMetaInfo();
    RankId rank = 0;
    uint32_t pod = 0;
    for (const auto& superPod : topoMeta) {
        uint32_t ser = 0;
        for (const auto& server : superPod) {
            for (uint32_t phyId : server) {
                posToRank[std::make_tuple(pod, ser, phyId)] = rank;
                ++rank;
            }
            ++ser;
        }
        ++pod;
    }
    return posToRank;
}

} // namespace

bool TaskGraphExecutor::CanExecute(const TaskStub* task) const
{
    if (task->GetType() == TaskTypeStub::WAIT || task->GetType() == TaskTypeStub::LOCAL_WAIT_FROM) {
        const uint32_t notifyId = GetWaitNotifyId(task);
        auto it = notifyState_.find(notifyId);
        return it != notifyState_.end() && it->second;
    }
    return true;
}

void TaskGraphExecutor::ExecuteOne(QueueCtx& ctx, TaskStub* task)
{
    ExecEvent event;
    event.rank = ctx.rank;
    event.streamId = ctx.streamId;
    event.taskIndex = static_cast<uint32_t>(ctx.next);
    event.type = task->GetType();
    event.seq = nextSeq_;

    switch (task->GetType()) {
        case TaskTypeStub::LOCAL_COPY: {
            auto* t = static_cast<TaskStubLocalCopy*>(task);
            const u8* srcPtr = nullptr;
            u8* dstPtr = nullptr;
            ResolveEndpoints(ctx.rank, t->GetSrcSlice(), ctx.rank, t->GetDstSlice(), srcPtr, dstPtr);
            memmove(dstPtr, srcPtr, t->GetSrcSlice().GetSize());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetSrcSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetDstSlice(), true));
            break;
        }
        case TaskTypeStub::WRITE: {
            auto* t = static_cast<TaskStubWrite*>(task);
            const RankId remote = t->GetRemoteRank();
            const u8* srcPtr = nullptr;
            u8* dstPtr = nullptr;
            ResolveEndpoints(ctx.rank, t->GetLocalSlice(), remote, t->GetRemoteSlice(), srcPtr, dstPtr);
            memmove(dstPtr, srcPtr, t->GetLocalSlice().GetSize());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), false));
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), true));
            break;
        }
        case TaskTypeStub::READ: {
            auto* t = static_cast<TaskStubRead*>(task);
            const RankId remote = t->GetRemoteRank();
            const u8* srcPtr = nullptr;
            u8* dstPtr = nullptr;
            ResolveEndpoints(remote, t->GetRemoteSlice(), ctx.rank, t->GetLocalSlice(), srcPtr, dstPtr);
            memmove(dstPtr, srcPtr, t->GetLocalSlice().GetSize());
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), true));
            break;
        }
        case TaskTypeStub::POST: {
            auto* t = static_cast<TaskStubPost*>(task);
            event.notifyId = t->GetTopicId();
            notifyState_[t->GetTopicId()] = true;
            lastPostSeq_[t->GetTopicId()] = event.seq;
            break;
        }
        case TaskTypeStub::LOCAL_POST_TO: {
            auto* t = static_cast<TaskStubLocalPostTo*>(task);
            event.notifyId = t->GetTopicId();
            notifyState_[t->GetTopicId()] = true;
            lastPostSeq_[t->GetTopicId()] = event.seq;
            break;
        }
        case TaskTypeStub::WAIT: {
            auto* t = static_cast<TaskStubWait*>(task);
            event.notifyId = t->GetTopicId();
            event.satisfiedBySeq = lastPostSeq_[t->GetTopicId()];
            notifyState_[t->GetTopicId()] = false; // 一次性信号，消费即清零
            lastPostSeq_.erase(t->GetTopicId());
            break;
        }
        case TaskTypeStub::LOCAL_WAIT_FROM: {
            auto* t = static_cast<TaskStubLocalWaitFrom*>(task);
            event.notifyId = t->GetTopicId();
            event.satisfiedBySeq = lastPostSeq_[t->GetTopicId()];
            notifyState_[t->GetTopicId()] = false;
            lastPostSeq_.erase(t->GetTopicId());
            break;
        }
        case TaskTypeStub::LOCAL_REDUCE: {
            auto* t = static_cast<TaskStubLocalReduce*>(task);
            ReduceInto(ctx.rank, t->GetDstSlice(), ctx.rank, t->GetSrcSlice(), t->GetDataType(), t->GetReduceOp());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetSrcSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetDstSlice(), true));
            break;
        }
        case TaskTypeStub::WRITE_REDUCE: {
            auto* t = static_cast<TaskStubWriteReduce*>(task);
            const RankId remote = t->GetRemoteRank();
            ReduceInto(remote, t->GetRemoteSlice(), ctx.rank, t->GetLocalSlice(), t->GetDataType(), t->GetReduceOp());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), false));
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), true));
            break;
        }
        case TaskTypeStub::READ_REDUCE: {
            auto* t = static_cast<TaskStubReadReduce*>(task);
            const RankId remote = t->GetRemoteRank();
            ReduceInto(ctx.rank, t->GetLocalSlice(), remote, t->GetRemoteSlice(), t->GetDataType(), t->GetReduceOp());
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), true));
            break;
        }
        default:
            THROW<InvalidParamsException>(
                "[TaskGraphExecutor] unimplemented task type[%s]", task->GetType().Describe().c_str());
    }

    nextSeq_++;
    executionLog_.push_back(event);
}

std::string TaskGraphExecutor::BuildDeadlockReport(const std::vector<QueueCtx>& ctxs) const
{
    std::string report = "deadlock detected:";
    for (const auto& ctx : ctxs) {
        if (ctx.next >= ctx.tasks->size()) {
            continue;
        }
        const TaskStub* task = (*ctx.tasks)[ctx.next].get();
        // 类型名（如 TaskTypeStub::LOCAL_WAIT_FROM）供脚本化死锁解析，Describe() 供人读
        std::string detail = task->GetType().Describe() + " " + task->Describe();
        if (task->GetType() == TaskTypeStub::WAIT || task->GetType() == TaskTypeStub::LOCAL_WAIT_FROM) {
            const uint32_t notifyId = GetWaitNotifyId(task);
            auto it = notifyState_.find(notifyId);
            const bool pending = (it != notifyState_.end() && it->second);
            detail += " pending=" + std::to_string(pending ? 1 : 0);
        }
        report += "\n  rank " + std::to_string(ctx.rank) + " stream " + std::to_string(ctx.streamId) +
                  " blocked at task[" + std::to_string(ctx.next) + "]: " + detail;
    }
    return report;
}

ExecResult TaskGraphExecutor::Execute(AllRankTaskQueues& queues, std::string& deadlockReport)
{
    executionLog_.clear();
    notifyState_.clear();
    lastPostSeq_.clear();
    nextSeq_ = 1;

    const auto posToRank = BuildPosToRank();
    std::vector<QueueCtx> ctxs;
    for (auto& podEntry : queues) {
        for (auto& serEntry : podEntry.second) {
            for (auto& phyEntry : serEntry.second) {
                const auto key = std::make_tuple(podEntry.first, serEntry.first, phyEntry.first);
                const RankId rank = posToRank.at(key);
                SingleTaskQueue& streams = phyEntry.second;
                for (uint32_t streamId = 0; streamId < streams.size(); ++streamId) {
                    if (!streams[streamId].empty()) {
                        ctxs.push_back(QueueCtx{rank, streamId, &streams[streamId], 0});
                    }
                }
            }
        }
    }

    while (true) {
        bool allDrained = true;
        for (const auto& ctx : ctxs) {
            if (ctx.next < ctx.tasks->size()) {
                allDrained = false;
                break;
            }
        }
        if (allDrained) {
            return ExecResult::SUCCESS;
        }

        bool progressed = false;
        for (auto& ctx : ctxs) {
            if (ctx.next >= ctx.tasks->size()) {
                continue;
            }
            TaskStub* task = (*ctx.tasks)[ctx.next].get();
            if (!CanExecute(task)) {
                continue;
            }
            ExecuteOne(ctx, task);
            ctx.next++;
            progressed = true;
        }
        if (!progressed) {
            deadlockReport = BuildDeadlockReport(ctxs);
            return ExecResult::DEADLOCK;
        }
    }
}

} // namespace AicpuSt
} // namespace HcclSim
