// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "dsl_all_gather_sole_templates.h"
#include "all_gather_algorithms.h"
#include "ascend_plan_binding.h"
#include <algorithm>
namespace mc2_ops_hccl {
namespace {
dsl::Plan MeshPlan(u32 rank, const std::vector<std::vector<u32>>& groups, u64 bytes)
{
    u32 size = rank + 1;
    for (const auto& g : groups)
        for (u32 r : g)
            size = std::max(size, r + 1);
    dsl::Dimension d;
    d.groups.resize(size);
    d.groups[rank] = groups.at(0);
    return dsl::SoleMesh({rank, size, bytes}, d);
}
const dsl::ExecutionRoute AICPU_ROUTE{dsl::Initiator::AIV, dsl::Dispatcher::KFC_AICPU_SERVER, dsl::Executor::AICPU};
const dsl::ExecutionRoute CCU_ROUTE{dsl::Initiator::AIV, dsl::Dispatcher::KFC_CCU_SERVER, dsl::Executor::CCU};
} // namespace
DslAicpuAllGatherSoleMesh::DslAicpuAllGatherSoleMesh(const OpParam& p, u32 r, const std::vector<std::vector<u32>>& g)
    : InsAlgTemplateBase(p, r, g)
{}
std::string DslAicpuAllGatherSoleMesh::Describe() const { return "DSL sole Mesh bound to AICPU HComm"; }
HcclResult DslAicpuAllGatherSoleMesh::CalcRes(
    HcclComm c, const OpParam& p, const TopoInfoWithNetLayerDetails* t, AlgResourceRequest& r)
{
    auto plan = MeshPlan(myRank_, subCommRanks_, p.DataDes.count * DATATYPE_SIZE_TABLE[p.DataDes.dataType]);
    return dsl::BindNativeResources(plan, AICPU_ROUTE, {c, p, *t, subCommRanks_}, r);
}
HcclResult DslAicpuAllGatherSoleMesh::KernelRun(
    const OpParam& p, const TemplateDataParams& d, const TemplateResource& r)
{
    auto plan = MeshPlan(myRank_, subCommRanks_, d.sliceSize);
    TemplateResource bound = r;
    return dsl::LowerNativePlan(plan, AICPU_ROUTE, p, subCommRanks_, d, bound);
}
u64 DslAicpuAllGatherSoleMesh::CalcScratchMultiple(BufferType, BufferType)
{
    return opMode_ == OpMode::OPBASE ? subCommRanks_[0].size() : 0;
}
void DslAicpuAllGatherSoleMesh::GetNotifyIdxMainToSub(std::vector<u32>& v)
{
    v.assign(subCommRanks_[0].size() > 1 ? subCommRanks_[0].size() - 2 : 0, 0);
}
void DslAicpuAllGatherSoleMesh::GetNotifyIdxSubToMain(std::vector<u32>& v)
{
    v.clear();
    for (u32 i = 0; i + 2 < subCommRanks_[0].size(); ++i)
        v.push_back(i);
}
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
DslCcuAllGatherSoleMesh::DslCcuAllGatherSoleMesh(const OpParam& p, u32 r, const std::vector<std::vector<u32>>& g)
    : CcuAlgTemplateBase(p, r, g)
{}
std::string DslCcuAllGatherSoleMesh::Describe() const { return "DSL sole Mesh bound to KFC CCU mission"; }
HcclResult DslCcuAllGatherSoleMesh::CalcRes(
    HcclComm c, const OpParam& p, const TopoInfoWithNetLayerDetails* t, AlgResourceRequest& r)
{
    auto plan = MeshPlan(myRank_, subCommRanks_, p.DataDes.count * DATATYPE_SIZE_TABLE[p.DataDes.dataType]);
    return dsl::BindNativeResources(plan, CCU_ROUTE, {c, p, *t, subCommRanks_}, r);
}
HcclResult DslCcuAllGatherSoleMesh::KernelRun(const OpParam& p, const TemplateDataParams& d, TemplateResource& r)
{
    auto plan = MeshPlan(myRank_, subCommRanks_, d.sliceSize);
    return dsl::LowerNativePlan(plan, CCU_ROUTE, p, subCommRanks_, d, r);
}
u64 DslCcuAllGatherSoleMesh::CalcScratchMultiple(BufferType, BufferType) { return 0; }
#endif
} // namespace mc2_ops_hccl
