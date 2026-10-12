// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "execution_route.h"
#include <stdexcept>
namespace mc2_ops_hccl {
namespace dsl {
bool IsSupported(const ExecutionRoute& r, Primitive p)
{
    if (p == Primitive::PLACE_INPUT)
        return true;
    if (r.dispatcher == Dispatcher::KFC_AICPU_SERVER)
        return r.executor == Executor::AICPU;
    if (r.dispatcher == Dispatcher::KFC_CCU_SERVER)
        return r.executor == Executor::CCU;
    if (r.dispatcher == Dispatcher::DIRECT && r.executor == Executor::AICPU)
        return r.initiator == Initiator::HOST;
    if (r.dispatcher == Dispatcher::DIRECT && (r.executor == Executor::AIV || r.executor == Executor::AIC))
        return p == Primitive::MESH_ALL_GATHER;
    return false;
}
EngineResources AnalyzeRoute(const Plan& plan, const ExecutionRoute& route)
{
    EngineResources r;
    for (const Stage& stage : plan.stages) {
        if (!IsSupported(route, stage.primitive))
            throw std::invalid_argument("route cannot lower primitive");
        if (stage.primitive == Primitive::PLACE_INPUT)
            continue;
        if (route.dispatcher != Dispatcher::DIRECT) {
            ++r.clientRequestSlots;
            r.serverQueues = 1;
        }
        if (route.executor == Executor::AICPU) {
            r.workerThreads += stage.primitive == Primitive::MESH_ALL_GATHER ?
                                   static_cast<uint32_t>(stage.ranks.size() > 1 ? stage.ranks.size() - 1 : 1) :
                                   1;
        }
        if (route.executor == Executor::CCU)
            ++r.ccuMissions;
        if (route.executor == Executor::AIV || route.executor == Executor::AIC)
            ++r.aivKernels;
    }
    if (r.workerThreads > 1)
        r.notifies = 2 * (r.workerThreads - 1);
    return r;
}
const char* RouteName(const ExecutionRoute& r)
{
    if (r.dispatcher == Dispatcher::KFC_AICPU_SERVER)
        return "aic/aiv -> KFC AICPU server -> HComm";
    if (r.dispatcher == Dispatcher::KFC_CCU_SERVER)
        return "aic/aiv -> KFC CCU server -> CCU mission";
    if (r.executor == Executor::AICPU)
        return "host -> AICPU HComm";
    if (r.executor == Executor::CCU)
        return "host -> CCU mission";
    return "AIC/AIV direct communication kernel";
}
} // namespace dsl
} // namespace mc2_ops_hccl
