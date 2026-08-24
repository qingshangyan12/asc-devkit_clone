/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCLV2_INS_V2_ALL_TO_ALL_CONCURRENT_EXECUTOR_H
#define HCCLV2_INS_V2_ALL_TO_ALL_CONCURRENT_EXECUTOR_H

#include "executor_common_ops.h"
#include "topo_match_ubx.h"

namespace mc2_ops_hccl {

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
class InsV2AllToAllConcurrentExecutor : public InsCollAlgBase {
public:
    explicit InsV2AllToAllConcurrentExecutor();
    ~InsV2AllToAllConcurrentExecutor() override = default;

    HcclResult Orchestrate(const OpParam& param, const AlgResourceCtxSerializable& resCtx) override;
    HcclResult CalcAlgHierarchyInfo(
        HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo) override;
    HcclResult CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest) override;

private:
    struct SendRecvData {
        std::vector<u64> sendCounts;
        std::vector<u64> recvCounts;
        std::vector<u64> sdispls;
        std::vector<u64> rdispls;
    };

    HcclResult OrchestrateLoop(const OpParam& param, const AlgResourceCtxSerializable& resCtx);
    HcclResult InitCommInfo(
        const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        const AlgHierarchyInfoForAllLevel& algHierarchyInfo);
    HcclResult FillTemplateResource(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, TemplateResource& templateAlgRes,
        uint32_t index);
    HcclResult InitTemplateDataParams(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, TemplateDataParams& tempAlgParams) const;
    HcclResult RestoreSendRecvData(const OpParam& param);
    HcclResult SplitSendRecvData(const OpParam& param, std::vector<SendRecvData>& splitData);
    HcclResult GetMaxSendRecvDataCount(u64& maxSendRecvDataCount, const SendRecvData& splitData) const;
    HcclResult CalcMaxDataCountPerLoop(
        const OpParam& param, const std::vector<u64> scratchMulti, std::vector<u64>& maxDataCountPerLoop) const;
    HcclResult SetTemplateDataParams(
        TemplateDataParams& tempAlgParams, const SendRecvData& splitData, u32 loop, u64 currDataCount,
        u64 processedDataCount, u64 maxDataCountPerLoop) const;
    void GenTemplateAlgParams(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, const u64 dataOffset, const u64 dataCountPerLoop,
        TemplateDataParams& tempAlgParams) const;

    AlgHierarchyInfoForAllLevel algHierarchyInfo_;
    std::vector<std::map<u32, std::vector<ChannelInfo>>> remoteRankToChannelInfo_;
    std::vector<ThreadHandle> threads_;

    std::vector<u64> sendCounts_;
    std::vector<u64> recvCounts_;
    std::vector<u64> sdispls_;
    std::vector<u64> rdispls_;
};

} // namespace mc2_ops_hccl

#endif
