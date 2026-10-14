// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "ccu_all_gather_binding.h"
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
#include "ccu_temp_kfc_all_gather_mesh_1D_mem2mem.h"
#include "ccu_temp_kfc_all_gather_nhr_1d_mem2mem.h"
#endif
namespace mc2_ops_hccl {
namespace dsl {
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
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
    return native.KernelRun(op, data, resource);
}
} // namespace
#endif
HcclResult BindCcuAllGather(const Stage& s, const AscendPlanContext& c, AlgResourceRequest& r)
{
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
    return s.primitive == Primitive::MESH_ALL_GATHER ? Bind<CcuTempKfcAllGatherMesh1DMem2Mem>(c, r) :
                                                       Bind<CcuTempKfcAllGatherNHR1DMem2Mem>(c, r);
#else
    (void)s;
    (void)c;
    (void)r;
    return HCCL_E_NOT_SUPPORT;
#endif
}
HcclResult LowerCcuAllGather(
    const Stage& s, Rank rank, const OpParam& op, const std::vector<std::vector<u32>>& groups,
    const TemplateDataParams& data, TemplateResource& resource)
{
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
    return s.primitive == Primitive::MESH_ALL_GATHER ?
               Lower<CcuTempKfcAllGatherMesh1DMem2Mem>(rank, op, groups, data, resource) :
               Lower<CcuTempKfcAllGatherNHR1DMem2Mem>(rank, op, groups, data, resource);
#else
    (void)s;
    (void)rank;
    (void)op;
    (void)groups;
    (void)data;
    (void)resource;
    return HCCL_E_NOT_SUPPORT;
#endif
}
} // namespace dsl
} // namespace mc2_ops_hccl
