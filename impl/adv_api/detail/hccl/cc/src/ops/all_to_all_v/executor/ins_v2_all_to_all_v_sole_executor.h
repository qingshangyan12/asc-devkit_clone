/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * 本程序是自由软件，您可以根据 CANN 开源软件许可证 2.0 版的条款和条件对其进行再分发和/或修改。
 * 详情请参阅许可证。如不符合许可证要求，不得使用本软件。
 */
#ifndef HCCLV2_INS_V2_ALL_TO_ALL_V_SOLE_EXECUTOR_H
#define HCCLV2_INS_V2_ALL_TO_ALL_V_SOLE_EXECUTOR_H

#include "executor_common_ops.h"
#include "topo_match_1d.h"
#include "topo_match_base.h"

namespace mc2_ops_hccl {

/*
 * AlltoAllV Sole 执行器（模板类）
 *
 * 对应 hccl 仓的 InsAlltoAllVSoleExecutor（hccl）/ InsV2AllGatherSoleExecutor（asc-devkit all_gather）
 *
 * 模板参数：
 *   - AlgTopoMatch: 拓扑匹配类（如 TopoMatch1D）
 *   - InsAlgTemplate: 算法模板类（如 CcuTempHalfAllToAllVMesh1D）
 *
 * 职责：
 *   1. CalcAlgHierarchyInfo() — 拓扑匹配
 *   2. CalcRes() — 调用 Template::CalcRes 计算资源
 *   3. Orchestrate() — 编排执行，调用 Template::KernelRun
 */
template <typename AlgTopoMatch, typename InsAlgTemplate>
class InsV2AlltoAllVSoleExecutor : public InsCollAlgBase {
public:
    explicit InsV2AlltoAllVSoleExecutor();
    ~InsV2AlltoAllVSoleExecutor() override = default;

    HcclResult Orchestrate(const OpParam& param, const AlgResourceCtxSerializable& resCtx) override;

    /* 资源计算 */
    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest) override;

    HcclResult CalcAlgHierarchyInfo(
        HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo) override;

protected:
    /* 算法编排循环 */
    HcclResult OrchestrateLoop(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    std::vector<std::map<u32, std::vector<ChannelInfo>>> remoteRankToChannelInfo_;
    std::vector<ThreadHandle> threads_;
};

} // namespace mc2_ops_hccl

#endif // HCCLV2_INS_V2_ALL_TO_ALL_V_SOLE_EXECUTOR_H
