// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherSoleMesh(Program& graph, Rank p, const AllGather& a, const Groups& groups, Op direction)
{
    Queue q = graph.queue(p, 0);
    placeInput(q, p, a);
    // 一个 Mesh 覆盖全通信域，所有 peer 并行交换自己的 input 槽位。
    return q.submit(mesh(p, groups.at(p),
        meshSlices(p, groups.at(p), initialOwners(graph.rankCount()), a, 0, a.input.bytes, direction)));
}
} }
