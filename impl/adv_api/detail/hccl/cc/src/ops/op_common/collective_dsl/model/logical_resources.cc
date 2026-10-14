// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "logical_resources.h"
#include <algorithm>
#include <set>
namespace mc2_ops_hccl {
namespace dsl {
LogicalResources Analyze(const Plan& plan)
{
    LogicalResources result;
    std::set<Rank> peers;
    for (const Stage& stage : plan.stages) {
        result.queueCount = std::max(result.queueCount, stage.queue + 1);
        ++result.stageCount;
        result.needsMesh |= stage.primitive == Primitive::MESH_ALL_GATHER;
        result.needsNhr |= stage.primitive == Primitive::NHR_ALL_GATHER;
        for (Rank rank : stage.ranks)
            if (rank != plan.rank)
                peers.insert(rank);
    }
    result.peers.assign(peers.begin(), peers.end());
    return result;
}
} // namespace dsl
} // namespace mc2_ops_hccl
