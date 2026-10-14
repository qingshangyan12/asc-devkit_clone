// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "slices.h"
#include <algorithm>
#include <stdexcept>
namespace asc { namespace ir {
Event allGatherSequence(Program& graph, Rank p, const AllGather& a,
                        const Groups& meshGroups, const Groups& nhrGroups, Op direction)
{
    Queue q = graph.queue(p, 0);
    placeInput(q, p, a);
    q.submit(mesh(p, meshGroups.at(p),
        meshSlices(p, meshGroups.at(p), initialOwners(graph.rankCount()), a, 0, a.input.bytes, direction)));
    // 同 queue 自动依赖前一级完成。NHR 的每个 block 包含 meshGroups[block] 的全部 owner，
    // 而不是又只发送最初的一个 input。
    return q.submit(nhr(p, nhrGroups.at(p), nhrSlices(meshGroups, a, 0, a.input.bytes, direction)));
}
} }
