/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// AICPU ST 执行引擎库（本目录 = task_graph_executor + aicpu_float_convert）：
// 任务图真执行（TaskStub 轮询调度/notify 同步/死锁定位/搬运与归约），
// 原语无关——任何集合通信算法投递的 TaskStub 均可执行。
// 目录结构与角色映射见 aicpu_scenario/aicpu_fixture.h 头部说明。
#ifndef AICPU_ST_TASK_GRAPH_EXECUTOR_H
#define AICPU_ST_TASK_GRAPH_EXECUTOR_H

#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "sim_common.h"
#include "sim_task.h"
#include "sim_task_queue.h"

namespace HcclSim {
namespace AicpuSt {

enum class ExecResult { SUCCESS, DEADLOCK };

// 单次内存访问（供 DAG 冲突检测）
struct MemAccess {
    RankId rank;
    BufferType bufferType;
    uint64_t offset;
    uint64_t size;
    bool isWrite;
};

// 执行事件：一个 TaskStub 执行一次的记录
struct ExecEvent {
    RankId rank = 0; // 队列所属 rank
    uint32_t streamId = 0;
    uint32_t taskIndex = 0;
    TaskTypeStub type;
    std::vector<MemAccess> accesses;
    uint32_t notifyId = 0; // POST/WAIT 系任务有效；0 并非哨兵（真实 notifyId 可为 0），判别必须按 event.type
    uint64_t seq = 0;            // 全局执行序号（从 1 开始）
    uint64_t satisfiedBySeq = 0; // 仅 WAIT 系事件有效：满足它的 POST 的 seq
};

// 任务图执行引擎：轮询调度所有 (rank, stream) 队列，
// 同队列 FIFO 保序，WAIT 系任务等待对应 notifyId 置位（消费后自动清零）。
// 全部排空返回 SUCCESS；全部队头阻塞返回 DEADLOCK（report 给出每队列卡点）。
class TaskGraphExecutor {
public:
    ExecResult Execute(AllRankTaskQueues& queues, std::string& deadlockReport);

    const std::vector<ExecEvent>& ExecutionLog() const { return executionLog_; }

private:
    struct QueueCtx {
        RankId rank;
        uint32_t streamId;
        std::vector<std::shared_ptr<TaskStub>>* tasks;
        size_t next;
    };

    bool CanExecute(const TaskStub* task) const;
    void ExecuteOne(QueueCtx& ctx, TaskStub* task);
    std::string BuildDeadlockReport(const std::vector<QueueCtx>& ctxs) const;

    std::vector<ExecEvent> executionLog_;
    // notify 配对不变量：算法层按 1:1 使用 notifyId（一次 POST 恰好满足一次 WAIT，设备 notify 语义）。
    // 违反 1:1（如连续双 POST）时由下方置位吸收语义兜底——最老 POST→WAIT 边保留（HB sound），
    // 此类误用仍应由算法层排查。
    // 置位吸收语义：notify 是标志位非计数器（设备 record 置位/wait 清零）。置位期间的后续 POST 被吸收
    // （不覆盖已记录 seq）——WAIT 的 satisfiedBySeq 指向"置位期间最老"的 POST：跨队列 double-post 时
    // 最新 POST→WAIT 不蕴含最老 POST→WAIT（跨队列无先后边），取最老是 HB sound 方向。
    std::map<uint32_t, bool> notifyState_;     // notifyId -> posted
    std::map<uint32_t, uint64_t> lastPostSeq_; // notifyId -> 置位期间最老 POST 的 seq（见上方不变量）
    uint64_t nextSeq_ = 1;
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_TASK_GRAPH_EXECUTOR_H
