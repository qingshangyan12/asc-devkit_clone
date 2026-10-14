// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "collective_plan.h"
#include <stdexcept>
namespace mc2_ops_hccl {
namespace dsl {
Builder::Builder(const std::string& name, Rank rank, uint32_t rankSize, uint64_t bytesPerRank)
{
    if (rank >= rankSize || rankSize == 0 || bytesPerRank == 0)
        throw std::invalid_argument("invalid plan arguments");
    plan_.name = name;
    plan_.rank = rank;
    plan_.rankSize = rankSize;
    plan_.bytesPerRank = bytesPerRank;
}
StageId Builder::PlaceInput() { return Add(0, Primitive::PLACE_INPUT, {plan_.rank}, {}, {0, plan_.bytesPerRank}, {}); }
StageId Builder::Add(
    Queue queue, Primitive primitive, const Ranks& ranks, const std::vector<Ranks>& ownersByRank, Interval bytes,
    const std::vector<StageId>& dependsOn)
{
    if (bytes.begin > bytes.end || bytes.end > plan_.bytesPerRank)
        throw std::invalid_argument("invalid interval");
    Stage stage;
    stage.id = nextStage_++;
    stage.queue = queue;
    stage.primitive = primitive;
    stage.ranks = ranks;
    stage.ownersByRank = ownersByRank;
    stage.bytes = bytes;
    stage.dependsOn = dependsOn;
    plan_.stages.push_back(stage);
    return stage.id;
}
Plan Builder::Finish(const std::vector<StageId>& completion)
{
    plan_.completion = completion;
    return plan_;
}
} // namespace dsl
} // namespace mc2_ops_hccl
