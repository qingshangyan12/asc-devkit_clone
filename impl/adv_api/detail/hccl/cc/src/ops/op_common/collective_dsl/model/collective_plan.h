// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_PLAN_H
#define MC2_COLLECTIVE_DSL_PLAN_H
#include <cstdint>
#include <string>
#include <vector>
namespace mc2_ops_hccl {
namespace dsl {
using Rank = uint32_t;
using StageId = uint32_t;
using Queue = uint32_t;
using Ranks = std::vector<Rank>;
struct Interval {
    uint64_t begin = 0;
    uint64_t end = 0;
};
enum class Primitive { PLACE_INPUT, MESH_ALL_GATHER, NHR_ALL_GATHER };
struct Stage {
    StageId id = 0;
    Queue queue = 0;
    Primitive primitive = Primitive::PLACE_INPUT;
    Ranks ranks;
    std::vector<Ranks> ownersByRank;
    Interval bytes;
    std::vector<StageId> dependsOn;
};
struct Plan {
    std::string name;
    Rank rank = 0;
    uint32_t rankSize = 0;
    uint64_t bytesPerRank = 0;
    std::vector<Stage> stages;
    std::vector<StageId> completion;
};
class Builder {
public:
    Builder(const std::string& name, Rank rank, uint32_t rankSize, uint64_t bytesPerRank);
    StageId PlaceInput();
    StageId Add(
        Queue queue, Primitive primitive, const Ranks& ranks, const std::vector<Ranks>& ownersByRank, Interval bytes,
        const std::vector<StageId>& dependsOn);
    Plan Finish(const std::vector<StageId>& completion);

private:
    Plan plan_;
    StageId nextStage_ = 1;
};
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
