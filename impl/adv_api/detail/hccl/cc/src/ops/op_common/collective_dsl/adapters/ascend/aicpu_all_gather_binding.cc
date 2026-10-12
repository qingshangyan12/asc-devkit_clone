// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "aicpu_all_gather_binding.h"
#include "ins_temp_all_gather_mesh_1D.h"
#include "ins_temp_all_gather_nhr.h"
namespace mc2_ops_hccl {
namespace dsl {
namespace {
template <class T>
HcclResult Bind(const AscendPlanContext& c, AlgResourceRequest& r)
{
    T native(c.op, c.topo.userRank, c.subCommRanks);
    return native.CalcRes(c.comm, c.op, &c.topo, r);
}
template <class T>
HcclResult Lower(
    Rank rank, const OpParam& op, const std::vector<std::vector<u32>>& groups, const TemplateDataParams& data,
    TemplateResource& resource)
{
    T native(op, rank, groups);
    if (!resource.channels.empty()) {
        HcclResult ret = native.SetchannelsPerRank(resource.channels);
        if (ret != HCCL_SUCCESS)
            return ret;
    }
    return native.KernelRun(op, data, resource);
}
} // namespace
HcclResult BindAicpuAllGather(const Stage& s, const AscendPlanContext& c, AlgResourceRequest& r)
{
    return s.primitive == Primitive::MESH_ALL_GATHER ? Bind<InsTempAllGatherMesh1D>(c, r) :
                                                       Bind<InsTempAllGatherNHR>(c, r);
}
HcclResult LowerAicpuAllGather(
    const Stage& s, Rank rank, const OpParam& op, const std::vector<std::vector<u32>>& groups,
    const TemplateDataParams& data, TemplateResource& resource)
{
    return s.primitive == Primitive::MESH_ALL_GATHER ? Lower<InsTempAllGatherMesh1D>(rank, op, groups, data, resource) :
                                                       Lower<InsTempAllGatherNHR>(rank, op, groups, data, resource);
}
} // namespace dsl
} // namespace mc2_ops_hccl
