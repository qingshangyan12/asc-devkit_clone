// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_RESOURCE_PLAN_H
#define MC2_COLLECTIVE_DSL_RESOURCE_PLAN_H
#include "collective_plan.h"
namespace mc2_ops_hccl {
namespace dsl {
struct LogicalResources {
    Ranks peers;
    uint32_t queueCount = 0;
    uint32_t stageCount = 0;
    bool needsMesh = false;
    bool needsNhr = false;
};
LogicalResources Analyze(const Plan& plan);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
