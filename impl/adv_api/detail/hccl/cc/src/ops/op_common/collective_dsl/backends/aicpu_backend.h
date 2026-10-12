// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.

#ifndef MC2_COLLECTIVE_DSL_AICPU_BACKEND_H
#define MC2_COLLECTIVE_DSL_AICPU_BACKEND_H

#include "collective_plan.h"
#include "resource_plan.h"
#include "alg_param.h"
#include "template_utils.h"

namespace mc2_ops_hccl {
namespace dsl {

// 第一版 AICPU backend 使用一个 HComm thread 顺序执行 Plan。它先保证所有组合都能真实 lowering；
// 后续并行 backend 再把不同 queue 映射到多 thread，并为跨 thread 依赖申请 notify。
struct AicpuMemory {
    Rank rank = 0;
    uint64_t outputSlotBytes = 0;
    void* input = nullptr;
    void* output = nullptr;
    std::vector<void*> remoteInput;
    std::vector<void*> remoteOutput;
};

HcclResult CalcAicpuResources(
    const Plan& plan, const std::vector<HcclChannelDesc>& channelDescs, AlgResourceRequest& request);

HcclResult RunAicpu(const Plan& plan, const AicpuMemory& memory, const TemplateResource& resource);

} // namespace dsl
} // namespace mc2_ops_hccl

#endif
