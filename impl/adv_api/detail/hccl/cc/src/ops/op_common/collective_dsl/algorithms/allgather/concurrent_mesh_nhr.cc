// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherConcurrent(Program& graph, Rank p, const AllGather& a,
                          const Groups& full, uint64_t split, Op direction)
{
    if (split > a.input.bytes) throw std::invalid_argument("split exceeds input");
    Queue local = graph.queue(p, 0), Q0 = graph.queue(p, 1), Q1 = graph.queue(p, 2);
    Event ready = placeInput(local, p, a);
    Q0.enqueueWait(ready); Q1.enqueueWait(ready);
    Groups own = initialOwners(graph.rankCount());
    // 两路都使用全通信域，处理不重叠的输入区间；不需要第二维交换。
    Event m = Q0.submit(mesh(p, full.at(p), meshSlices(p, full.at(p), own, a, 0, split, direction)));
    Event n = Q1.submit(nhr(p, full.at(p), nhrSlices(own, a, split, a.input.bytes - split, direction)));
    local.enqueueWait(m); local.enqueueWait(n);
    return local.join();
}
} }
