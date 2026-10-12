// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_ALL_GATHER_PRIMITIVES_H
#define MC2_COLLECTIVE_DSL_ALL_GATHER_PRIMITIVES_H
#include "collective_plan.h"
namespace mc2_ops_hccl {
namespace dsl {
struct Dimension {
    std::vector<Ranks> groups;
    const Ranks& At(Rank rank) const;
};
struct AllGatherArgs {
    Rank rank = 0;
    uint32_t rankSize = 0;
    uint64_t sendBytes = 0;
};
StageId Mesh(
    Builder& plan, Queue queue, const Ranks& ranks, const std::vector<Ranks>& ownersByRank, Interval bytes,
    const std::vector<StageId>& dependsOn);
StageId Nhr(
    Builder& plan, Queue queue, const Ranks& ranks, const std::vector<Ranks>& ownersByRank, Interval bytes,
    const std::vector<StageId>& dependsOn);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
