// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#include "ascend_plan_binding.h"
#include "aicpu_all_gather_binding.h"
#include "ccu_all_gather_binding.h"
#include "native_binding_trace.h"
namespace mc2_ops_hccl {
namespace dsl {
namespace {
const Stage* CommunicationStage(const Plan& plan)
{
    const Stage* result = nullptr;
    for (const Stage& stage : plan.stages)
        if (stage.primitive != Primitive::PLACE_INPUT) {
            if (result != nullptr)
                return nullptr;
            result = &stage;
        }
    return result;
}
} // namespace
HcclResult BindNativeResources(
    const Plan& plan, const ExecutionRoute& route, const AscendPlanContext& context, AlgResourceRequest& request)
{
    const Stage* stage = CommunicationStage(plan);
    if (stage == nullptr)
        return HCCL_E_NOT_SUPPORT;
    if (route.executor == Executor::AICPU) {
        RecordNativeBinding(*stage, NativeAction::BIND_RESOURCES, "InsAlgTemplate");
        return BindAicpuAllGather(*stage, context, request);
    }
    if (route.executor == Executor::CCU) {
        RecordNativeBinding(*stage, NativeAction::BIND_RESOURCES, "CcuAlgTemplate");
        return BindCcuAllGather(*stage, context, request);
    }
    return HCCL_E_NOT_SUPPORT;
}
HcclResult LowerNativePlan(
    const Plan& plan, const ExecutionRoute& route, const OpParam& op, const std::vector<std::vector<u32>>& groups,
    const TemplateDataParams& data, TemplateResource& resource)
{
    const Stage* stage = CommunicationStage(plan);
    if (stage == nullptr)
        return HCCL_E_NOT_SUPPORT;
    if (route.executor == Executor::AICPU) {
        RecordNativeBinding(*stage, NativeAction::LOWER, "HComm primitives");
        return LowerAicpuAllGather(*stage, plan.rank, op, groups, data, resource);
    }
    if (route.executor == Executor::CCU) {
        RecordNativeBinding(*stage, NativeAction::LOWER, "CcuKernelSubmitInfo");
        return LowerCcuAllGather(*stage, plan.rank, op, groups, data, resource);
    }
    return HCCL_E_NOT_SUPPORT;
}
} // namespace dsl
} // namespace mc2_ops_hccl
