/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "aicpu_dag_checker.h"

#include <algorithm>
#include <map>
#include <queue>
#include <set>

namespace HcclSim {
namespace AicpuSt {
namespace {

std::string AccessToString(const ExecEvent& event, const MemAccess& access)
{
    return "rank " + std::to_string(access.rank) + " (exec rank " + std::to_string(event.rank) + ") stream " +
           std::to_string(event.streamId) + " task[" + std::to_string(event.taskIndex) + "] seq " +
           std::to_string(event.seq) + " " + (access.isWrite ? "WRITE" : "READ") + " [" +
           std::to_string(access.offset) + ", " + std::to_string(access.offset + access.size) + ")";
}

} // namespace

bool AicpuDagChecker::Reachable(const std::vector<std::vector<uint64_t>>& adj, uint64_t from, uint64_t to)
{
    if (from == to) {
        return true;
    }
    std::set<uint64_t> visited;
    std::queue<uint64_t> work;
    work.push(from);
    visited.insert(from);
    while (!work.empty()) {
        const uint64_t cur = work.front();
        work.pop();
        for (uint64_t next : adj[cur]) {
            if (next == to) {
                return true;
            }
            if (visited.insert(next).second) {
                work.push(next);
            }
        }
    }
    return false;
}

std::vector<DagConflict> AicpuDagChecker::Check(const std::vector<ExecEvent>& log)
{
    // 1) 建节点索引（seq -> 下标），邻接表：程序序（同队列按 seq 升序相邻建边）+ 同步边（POST seq -> WAIT seq）
    std::map<uint64_t, size_t> seqToIdx;
    for (size_t i = 0; i < log.size(); ++i) {
        seqToIdx[log[i].seq] = i;
    }
    std::vector<std::vector<uint64_t>> adj(log.size());
    {
        // 同队列程序序：按 (rank, streamId) 分组，seq 升序，相邻建边（前驱 -> 后继）
        std::map<std::pair<RankId, uint32_t>, std::vector<const ExecEvent*>> queues;
        for (const auto& event : log) {
            queues[std::make_pair(event.rank, event.streamId)].push_back(&event);
        }
        for (auto& entry : queues) {
            std::sort(entry.second.begin(), entry.second.end(), [](const ExecEvent* a, const ExecEvent* b) {
                return a->seq < b->seq;
            });
            for (size_t i = 1; i < entry.second.size(); ++i) {
                const size_t from = seqToIdx.at(entry.second[i - 1]->seq);
                const size_t to = seqToIdx.at(entry.second[i]->seq);
                adj[from].push_back(to);
            }
        }
    }
    {
        // 同步边：WAIT 事件记录了满足它的 POST 的 seq（satisfiedBySeq）
        for (const auto& event : log) {
            if ((event.type == TaskTypeStub::WAIT || event.type == TaskTypeStub::LOCAL_WAIT_FROM) &&
                event.satisfiedBySeq != 0) {
                auto it = seqToIdx.find(event.satisfiedBySeq);
                if (it != seqToIdx.end()) {
                    adj[it->second].push_back(seqToIdx.at(event.seq));
                }
            }
        }
    }

    // 2) 收集内存访问：按 (rank, bufferType) 分组，组内两两检查区间重叠 + 方向
    struct AccessRef {
        const ExecEvent* event;
        const MemAccess* access;
    };
    std::map<std::pair<RankId, BufferType>, std::vector<AccessRef>> groups;
    for (const auto& event : log) {
        for (const auto& access : event.accesses) {
            groups[std::make_pair(access.rank, access.bufferType)].push_back(AccessRef{&event, &access});
        }
    }

    std::vector<DagConflict> conflicts;
    for (auto& entry : groups) {
        const auto& list = entry.second;
        for (size_t i = 0; i < list.size(); ++i) {
            for (size_t j = i + 1; j < list.size(); ++j) {
                const AccessRef& a = list[i];
                const AccessRef& b = list[j];
                if (a.event->rank == b.event->rank && a.event->streamId == b.event->streamId) {
                    continue; // 同队列：程序序已保证先后
                }
                if (a.access->size == 0 || b.access->size == 0) {
                    continue; // 零长度访问不构成冲突
                }
                const uint64_t aEnd = a.access->offset + a.access->size;
                const uint64_t bEnd = b.access->offset + b.access->size;
                if (a.access->offset >= bEnd || b.access->offset >= aEnd) {
                    continue; // 区间不重叠
                }
                if (!a.access->isWrite && !b.access->isWrite) {
                    continue; // RAR 不报
                }
                const size_t idxA = seqToIdx.at(a.event->seq);
                const size_t idxB = seqToIdx.at(b.event->seq);
                if (Reachable(adj, idxA, idxB) || Reachable(adj, idxB, idxA)) {
                    continue; // 有 happens-before，合法
                }
                DagConflict conflict;
                conflict.kind = (a.access->isWrite && b.access->isWrite) ? "WAW" : (a.access->isWrite ? "RAW" : "WAR");
                conflict.detailA = AccessToString(*a.event, *a.access);
                conflict.detailB = AccessToString(*b.event, *b.access);
                conflicts.push_back(conflict);
            }
        }
    }
    return conflicts;
}

} // namespace AicpuSt
} // namespace HcclSim
