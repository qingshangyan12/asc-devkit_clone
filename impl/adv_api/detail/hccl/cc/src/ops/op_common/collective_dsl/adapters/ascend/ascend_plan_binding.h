// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_ASCEND_PLAN_BINDING_H
#define MC2_COLLECTIVE_DSL_ASCEND_PLAN_BINDING_H
#include "collective_plan.h"
#include "execution_route.h"
#include "alg_param.h"
#include "template_utils.h"
namespace mc2_ops_hccl {
namespace dsl {
// 旧框架类型只允许越过这个边界。算法和原语层看不到这些类型。
struct AscendPlanContext {
    HcclComm comm;
    const OpParam& op;
    const TopoInfoWithNetLayerDetails& topo;
    const std::vector<std::vector<u32>>& subCommRanks;
};
HcclResult BindNativeResources(
    const Plan& plan, const ExecutionRoute& route, const AscendPlanContext& context, AlgResourceRequest& request);
HcclResult LowerNativePlan(
    const Plan& plan, const ExecutionRoute& route, const OpParam& op, const std::vector<std::vector<u32>>& subCommRanks,
    const TemplateDataParams& data, TemplateResource& resource);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
