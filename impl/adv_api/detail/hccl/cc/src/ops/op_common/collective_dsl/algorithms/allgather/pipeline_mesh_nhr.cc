// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherPipeline(Program& graph, Rank p, const AllGather& a,
                        const Groups& meshGroups, const Groups& nhrGroups, uint64_t chunkBytes, Op direction)
{
    if (!chunkBytes) throw std::invalid_argument("zero pipeline chunk");
    Queue local = graph.queue(p, 0), Q0 = graph.queue(p, 1), Q1 = graph.queue(p, 2);
    Event ready = placeInput(local, p, a);
    Q0.enqueueWait(ready); Q1.enqueueWait(ready);
    Groups own = initialOwners(graph.rankCount());
    for (uint64_t begin = 0; begin < a.input.bytes;) {
        uint64_t bytes = std::min(chunkBytes, a.input.bytes - begin);
        Event m = Q0.submit(mesh(p, meshGroups.at(p),
            meshSlices(p, meshGroups.at(p), own, a, begin, bytes, direction)));
        // 仅向 Q1 提交依赖，不在 Host 等待。Q0(chunk c+1) 可与 Q1(chunk c) 重叠。
        Q1.enqueueWait(m);
        Event n = Q1.submit(nhr(p, nhrGroups.at(p), nhrSlices(meshGroups, a, begin, bytes, direction)));
        local.enqueueWait(n);
        begin += bytes; // 使用实际尾块长度，不会 begin+chunkBytes 溢出
    }
    return local.join(); // 所有 chunk 的两个维度完成后，整个 AllGather 才完成
}
} }
