// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "all_gather_primitives.h"
#include <algorithm>
#include <stdexcept>
namespace mc2_ops_hccl {
namespace dsl {
namespace {
void Check(const Ranks& ranks, const std::vector<Ranks>& owners)
{
    if (ranks.empty() || owners.empty())
        throw std::invalid_argument("empty collective group");
    for (Rank rank : ranks)
        if (rank >= owners.size())
            throw std::invalid_argument("group rank has no owner state");
}
} // namespace
const Ranks& Dimension::At(Rank rank) const { return groups.at(rank); }
StageId Mesh(
    Builder& plan, Queue queue, const Ranks& ranks, const std::vector<Ranks>& owners, Interval bytes,
    const std::vector<StageId>& dependencies)
{
    Check(ranks, owners);
    return plan.Add(queue, Primitive::MESH_ALL_GATHER, ranks, owners, bytes, dependencies);
}
StageId Nhr(
    Builder& plan, Queue queue, const Ranks& ranks, const std::vector<Ranks>& owners, Interval bytes,
    const std::vector<StageId>& dependencies)
{
    Check(ranks, owners);
    return plan.Add(queue, Primitive::NHR_ALL_GATHER, ranks, owners, bytes, dependencies);
}
} // namespace dsl
} // namespace mc2_ops_hccl
