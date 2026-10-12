// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherSoleNhr(Program& graph, Rank p, const AllGather& a, const Groups& groups, Op direction)
{
    Queue q = graph.queue(p, 0);
    placeInput(q, p, a);
    // 一条 NHR 路径逐轮扩大已持有的数据；每轮 recv 完成后才允许下一轮转发。
    return q.submit(nhr(p, groups.at(p),
        nhrSlices(initialOwners(graph.rankCount()), a, 0, a.input.bytes, direction)));
}
} }
