// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_KFC_DISPATCH_H
#define MC2_COLLECTIVE_DSL_KFC_DISPATCH_H
#include "execution_route.h"
namespace mc2_ops_hccl {
namespace dsl {
struct KfcDispatchPlan {
    uint32_t requestCount = 0;
    uint32_t queueCount = 0;
    bool usesHcclMsgArea = false;
    bool buildsCcuMission = false;
};
KfcDispatchPlan PlanKfcDispatch(const Plan& plan, const ExecutionRoute& route);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
