// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "kfc_dispatch.h"
#include <stdexcept>
namespace mc2_ops_hccl {
namespace dsl {
KfcDispatchPlan PlanKfcDispatch(const Plan& plan, const ExecutionRoute& route)
{
    if (route.dispatcher == Dispatcher::DIRECT)
        throw std::invalid_argument("direct route has no KFC dispatch");
    KfcDispatchPlan r;
    r.usesHcclMsgArea = true;
    r.queueCount = 1;
    for (const Stage& s : plan.stages)
        if (s.primitive != Primitive::PLACE_INPUT)
            ++r.requestCount;
    r.buildsCcuMission = route.dispatcher == Dispatcher::KFC_CCU_SERVER;
    return r;
}
} // namespace dsl
} // namespace mc2_ops_hccl
