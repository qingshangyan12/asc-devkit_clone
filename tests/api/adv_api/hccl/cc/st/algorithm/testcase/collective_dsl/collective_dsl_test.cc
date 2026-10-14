// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "all_gather_algorithms.h"
#include "execution_route.h"
#include "kfc_dispatch.h"
#include "logical_resources.h"
#include "native_binding_trace.h"
#include "trace_engine.h"
#include <iostream>
#include <sstream>
#include <stdexcept>
using namespace mc2_ops_hccl::dsl;
namespace {
Dimension Full(uint32_t n)
{
    Dimension d;
    Ranks all;
    for (Rank r = 0; r < n; ++r)
        all.push_back(r);
    d.groups.assign(n, all);
    return d;
}
Dimension Contiguous(uint32_t n, uint32_t size)
{
    Dimension d;
    d.groups.resize(n);
    for (Rank r = 0; r < n; ++r)
        for (Rank x = r / size * size; x < r / size * size + size; ++x)
            d.groups[r].push_back(x);
    return d;
}
Dimension Strided(uint32_t n, uint32_t stride)
{
    Dimension d;
    d.groups.resize(n);
    for (Rank r = 0; r < n; ++r)
        for (Rank x = r % stride; x < n; x += stride)
            d.groups[r].push_back(x);
    return d;
}
void Require(bool ok, const char* message)
{
    if (!ok)
        throw std::runtime_error(message);
}
void CheckPlan(const Plan& plan)
{
    Require(!plan.stages.empty(), "empty plan");
    Require(!plan.completion.empty(), "no completion");
    const LogicalResources logical = Analyze(plan);
    Require(logical.stageCount == plan.stages.size(), "stage count");
    std::ostringstream a, b;
    WriteTrace(plan, a);
    WriteTrace(plan, b);
    std::istringstream ea(a.str()), eb(b.str());
    std::string diff;
    Require(CompareTrace(ea, eb, diff), "trace mismatch");
}
void CheckRoutes(const Plan& mesh, const Plan& nhr)
{
    const ExecutionRoute hostAicpu{Initiator::HOST, Dispatcher::DIRECT, Executor::AICPU};
    const ExecutionRoute aivKfcAicpu{Initiator::AIV, Dispatcher::KFC_AICPU_SERVER, Executor::AICPU};
    const ExecutionRoute aicKfcCcu{Initiator::AIC, Dispatcher::KFC_CCU_SERVER, Executor::CCU};
    const ExecutionRoute aivDirect{Initiator::AIV, Dispatcher::DIRECT, Executor::AIV};
    Require(AnalyzeRoute(mesh, hostAicpu).workerThreads == 3, "AICPU Mesh thread policy");
    Require(PlanKfcDispatch(mesh, aivKfcAicpu).usesHcclMsgArea, "missing HcclMsgArea route");
    Require(PlanKfcDispatch(nhr, aicKfcCcu).buildsCcuMission, "missing CCU mission route");
    Require(AnalyzeRoute(mesh, aivDirect).aivKernels == 1, "missing direct AIV kernel");
    bool rejected = false;
    try {
        (void)AnalyzeRoute(nhr, aivDirect);
    } catch (const std::invalid_argument&) {
        rejected = true;
    }
    Require(rejected, "unsupported AIV NHR must not be faked");
}
void CheckNativeTrace(const Plan& plan)
{
    EnableNativeBindingTrace(true);
    ClearNativeBindingTrace();
    for (const Stage& s : plan.stages)
        if (s.primitive != Primitive::PLACE_INPUT)
            RecordNativeBinding(s, NativeAction::LOWER, "test native primitive");
    std::string difference;
    Require(ComparePlanToNativeTrace(plan, NativeBindingTrace(), difference), "native trace mismatch");
    EnableNativeBindingTrace(false);
}
} // namespace
int main()
{
    const AllGatherArgs a{0, 4, 32};
    const Dimension full = Full(4);
    const Plan mesh = SoleMesh(a, full);
    const Plan nhr = SoleNhr(a, full);
    CheckPlan(mesh);
    CheckPlan(nhr);
    CheckPlan(ConcurrentMeshNhr(a, full, 12));
    CheckNativeTrace(mesh);
    const AllGatherArgs b{0, 8, 32};
    CheckPlan(ParallelMeshNhr(b, Contiguous(8, 4), Strided(8, 4), 12));
    CheckPlan(PipelineMeshNhr(b, Contiguous(8, 4), Strided(8, 4), 8));
    CheckRoutes(mesh, nhr);
    std::cout << "collective_dsl_test: PASS\n";
    return 0;
}
