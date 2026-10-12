// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_RESOURCE_PLAN_H
#define MC2_COLLECTIVE_DSL_RESOURCE_PLAN_H

#include "collective_plan.h"

#include <vector>

namespace mc2_ops_hccl {
namespace dsl {

struct ChannelNeed {
    Rank peer = 0;
    uint32_t path = 0;
};

struct ResourcePlan {
    std::vector<ChannelNeed> channels;
    uint32_t queueCount = 0;
    uint32_t eventCount = 0;
};

ResourcePlan CalcResources(const Plan& plan);

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
