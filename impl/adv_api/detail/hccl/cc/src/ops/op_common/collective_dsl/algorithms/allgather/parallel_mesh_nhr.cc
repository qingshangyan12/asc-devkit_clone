// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherParallel(Program& graph, Rank p, const AllGather& a,
                        const Groups& meshGroups, const Groups& nhrGroups, uint64_t split, Op direction)
{
    if (split > a.input.bytes) throw std::invalid_argument("split exceeds input");
    Queue local = graph.queue(p, 0), Q0 = graph.queue(p, 1), Q1 = graph.queue(p, 2);
    Event ready = placeInput(local, p, a);
    Q0.enqueueWait(ready); Q1.enqueueWait(ready);
    Groups own = initialOwners(graph.rankCount());
    // 第一个区间：[0,split)，Mesh→NHR；第二个区间：[split,sendBytes)，NHR→Mesh。
    // 每条 queue 内有顺序，两条 queue 之间不加无必要的阶段 barrier。
    Q0.submit(mesh(p, meshGroups.at(p), meshSlices(p, meshGroups.at(p), own, a, 0, split, direction)));
    Event n = Q0.submit(nhr(p, nhrGroups.at(p), nhrSlices(meshGroups, a, 0, split, direction)));
    Q1.submit(nhr(p, nhrGroups.at(p), nhrSlices(own, a, split, a.input.bytes - split, direction)));
    Event m = Q1.submit(mesh(p, meshGroups.at(p),
        meshSlices(p, meshGroups.at(p), nhrGroups, a, split, a.input.bytes - split, direction)));
    local.enqueueWait(m); local.enqueueWait(n);
    return local.join();
}
} }
