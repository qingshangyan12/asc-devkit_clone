/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// AICPU ST 校验库（本目录 = aicpu_dag_checker）：对执行日志做静态正确性校验，
// 原语无关。目录结构与角色映射见 aicpu_scenario/aicpu_fixture.h 头部说明。
#ifndef AICPU_ST_DAG_CHECKER_H
#define AICPU_ST_DAG_CHECKER_H

#include <cstdint>
#include <string>
#include <vector>

#include "task_graph_executor.h"

namespace HcclSim {
namespace AicpuSt {

struct DagConflict {
    std::string kind; // "WAW" / "RAW"(写先读后) / "WAR"(读先写后)；A 为时序在先的访问
    std::string detailA;
    std::string detailB;
};

// DAG 内存冲突检测：
// happens-before = 同队列程序序 + （POST -> 被其满足的 WAIT）同步边
// （WAIT / LOCAL_WAIT_FROM；新增同步原语需同步此处与 checker 实现）；
// 两个来自不同队列、无 happens-before 关系的内存访问，区间重叠且至少一个写 => 冲突。
// 算法按 notify 同步设计时不应有冲突；冲突即同步缺失或 slice 计算错误的信号。
class AicpuDagChecker {
public:
    std::vector<DagConflict> Check(const std::vector<ExecEvent>& log);

private:
    static bool Reachable(const std::vector<std::vector<uint64_t>>& adj, uint64_t from, uint64_t to);
};

} // namespace AicpuSt
} // namespace HcclSim

#endif // AICPU_ST_DAG_CHECKER_H
