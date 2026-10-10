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
        const uint32_t notifyId = (task->GetType() == TaskTypeStub::WAIT) ?
                                      static_cast<const TaskStubWait*>(task)->GetTopicId() :
                                      static_cast<const TaskStubLocalWaitFrom*>(task)->GetTopicId();
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
            // 先对两端各完成一次后备扩容（防同块扩容使先取的指针悬空），再取裸指针搬数据
            SimNpu& npu = SimWorld::Global()->GetSimNpuByRankId(ctx.rank);
            (void)npu.GetBufferBase(
                t->GetSrcSlice().GetType(), t->GetSrcSlice().GetOffset() + t->GetSrcSlice().GetSize());
            (void)npu.GetBufferBase(
                t->GetDstSlice().GetType(), t->GetDstSlice().GetOffset() + t->GetDstSlice().GetSize());
            memmove(
                ResolveSlicePtr(ctx.rank, t->GetDstSlice()), ResolveSlicePtr(ctx.rank, t->GetSrcSlice()),
                t->GetSrcSlice().GetSize());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetSrcSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetDstSlice(), true));
            break;
        }
        case TaskTypeStub::WRITE: {
            auto* t = static_cast<TaskStubWrite*>(task);
            const RankId remote = t->GetRemoteRank();
            // 同上：先扩容两端（跨 rank 也可能同块类型）
            SimNpu& localNpu = SimWorld::Global()->GetSimNpuByRankId(ctx.rank);
            (void)localNpu.GetBufferBase(
                t->GetLocalSlice().GetType(), t->GetLocalSlice().GetOffset() + t->GetLocalSlice().GetSize());
            SimNpu& remoteNpu = SimWorld::Global()->GetSimNpuByRankId(remote);
            (void)remoteNpu.GetBufferBase(
                t->GetRemoteSlice().GetType(), t->GetRemoteSlice().GetOffset() + t->GetRemoteSlice().GetSize());
            memmove(
                ResolveSlicePtr(remote, t->GetRemoteSlice()), ResolveSlicePtr(ctx.rank, t->GetLocalSlice()),
                t->GetLocalSlice().GetSize());
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), false));
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), true));
            break;
        }
        case TaskTypeStub::READ: {
            auto* t = static_cast<TaskStubRead*>(task);
            const RankId remote = t->GetRemoteRank();
            SimNpu& localNpu = SimWorld::Global()->GetSimNpuByRankId(ctx.rank);
            (void)localNpu.GetBufferBase(
                t->GetLocalSlice().GetType(), t->GetLocalSlice().GetOffset() + t->GetLocalSlice().GetSize());
            SimNpu& remoteNpu = SimWorld::Global()->GetSimNpuByRankId(remote);
            (void)remoteNpu.GetBufferBase(
                t->GetRemoteSlice().GetType(), t->GetRemoteSlice().GetOffset() + t->GetRemoteSlice().GetSize());
            memmove(
                ResolveSlicePtr(ctx.rank, t->GetLocalSlice()), ResolveSlicePtr(remote, t->GetRemoteSlice()),
                t->GetLocalSlice().GetSize());
            event.accesses.push_back(MakeAccess(remote, t->GetRemoteSlice(), false));
            event.accesses.push_back(MakeAccess(ctx.rank, t->GetLocalSlice(), true));
            break;
        }
        default:
            // POST/WAIT/REDUCE 系在 Task 3/4 接入
            break;
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
        std::string detail = task->Describe();
        if (task->GetType() == TaskTypeStub::WAIT || task->GetType() == TaskTypeStub::LOCAL_WAIT_FROM) {
            const uint32_t notifyId = (task->GetType() == TaskTypeStub::WAIT) ?
                                          static_cast<const TaskStubWait*>(task)->GetTopicId() :
                                          static_cast<const TaskStubLocalWaitFrom*>(task)->GetTopicId();
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
