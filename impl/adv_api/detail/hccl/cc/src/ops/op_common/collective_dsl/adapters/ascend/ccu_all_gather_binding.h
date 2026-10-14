// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_COLLECTIVE_DSL_CCU_ALL_GATHER_BINDING_H
#define MC2_COLLECTIVE_DSL_CCU_ALL_GATHER_BINDING_H
#include "ascend_plan_binding.h"
namespace mc2_ops_hccl {
namespace dsl {
HcclResult BindCcuAllGather(const Stage&, const AscendPlanContext&, AlgResourceRequest&);
HcclResult LowerCcuAllGather(
    const Stage&, Rank, const OpParam&, const std::vector<std::vector<u32>>&, const TemplateDataParams&,
    TemplateResource&);
} // namespace dsl
} // namespace mc2_ops_hccl
#endif
