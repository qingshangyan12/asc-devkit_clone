// Copyright (c) 2026 Huawei Technologies Co., Ltd.
// Licensed under the CANN Open Software License Agreement Version 2.0.
#ifndef MC2_DSL_ALL_GATHER_SOLE_TEMPLATES_H
#define MC2_DSL_ALL_GATHER_SOLE_TEMPLATES_H
#include "alg_v2_template_base.h"
#include "ccu_alg_template_base.h"
namespace mc2_ops_hccl {
class DslAicpuAllGatherSoleMesh : public InsAlgTemplateBase {
public:
    DslAicpuAllGatherSoleMesh(const OpParam&, u32, const std::vector<std::vector<u32>>&);
    std::string Describe() const override;
    HcclResult CalcRes(HcclComm, const OpParam&, const TopoInfoWithNetLayerDetails*, AlgResourceRequest&) override;
    HcclResult KernelRun(const OpParam&, const TemplateDataParams&, const TemplateResource&) override;
    u64 CalcScratchMultiple(BufferType, BufferType) override;
    void GetNotifyIdxMainToSub(std::vector<u32>&) override;
    void GetNotifyIdxSubToMain(std::vector<u32>&) override;
};
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
class DslCcuAllGatherSoleMesh : public CcuAlgTemplateBase {
public:
    DslCcuAllGatherSoleMesh(const OpParam&, u32, const std::vector<std::vector<u32>>&);
    std::string Describe() const override;
    HcclResult CalcRes(HcclComm, const OpParam&, const TopoInfoWithNetLayerDetails*, AlgResourceRequest&) override;
    HcclResult KernelRun(const OpParam&, const TemplateDataParams&, TemplateResource&) override;
    u64 CalcScratchMultiple(BufferType, BufferType) override;
};
#endif
} // namespace mc2_ops_hccl
#endif
