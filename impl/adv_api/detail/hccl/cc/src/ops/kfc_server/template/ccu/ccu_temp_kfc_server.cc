/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "channel.h"
#include "hccl_ccu_res.h"
#include "ccu_assist_pub.h"
#include "alg_data_trans_wrapper.h"
#include "kernel/ccu_kernel_kfc_server.h"
#include "ccu_temp_kfc_server.h"
#include "ccu_launch_dl.h"

namespace mc2_ops_hccl {
static bool IsKfcSourceKernelMatched(const std::string& sourceName, HcclCMDType opType, KfcServerRole role)
{
    switch (opType) {
        case HcclCMDType::HCCL_CMD_ALLGATHER:
            if (role == KfcServerRole::ALL_GATHER_NHR) {
                return sourceName == "CcuKernelAllGatherNHR1DMultiJettyMem2Mem";
            }
            return (role == KfcServerRole::DEFAULT || role == KfcServerRole::ALL_GATHER_MESH) &&
                   (sourceName == "CcuKernelAllGatherMesh1DMem2Mem" ||
                    sourceName == "CcuKernelKfcAllGatherMesh1DMem2Mem");
        case HcclCMDType::HCCL_CMD_REDUCE_SCATTER:
            return role == KfcServerRole::DEFAULT &&
                    (sourceName == "CcuKernelKfcReduceScatterMesh1DMem2Mem" ||
                    sourceName == KFC_REDUCE_SCATTER_PEER_ONLY_KERNEL_NAME);
        case HcclCMDType::HCCL_CMD_ALLREDUCE:
            return role == KfcServerRole::DEFAULT && sourceName == "CcuKernelAllReduceMesh1DMem2Mem";
        case HcclCMDType::HCCL_CMD_ALLTOALL:
            if (role == KfcServerRole::ALL_TO_ALL_MULTI_JETTY) {
                return sourceName == "CcuKernelKfcAllToAllMesh1DMultiJetty";
            }
            return role == KfcServerRole::DEFAULT && sourceName == "CcuKernelAlltoAllMesh1D";
        case HcclCMDType::HCCL_CMD_ALLTOALLV:
            return role == KfcServerRole::DEFAULT && sourceName == "CcuKernelAlltoAllVMesh1D";
        default:
            return false;
    }
}

static HcclResult InheritKfcServerKernelArg(
    const CcuKernelInfo& srcKernel, CcuKernelInfo& dstKernel, const OpParam& param, KfcServerRole role,
    uint32_t missionIndex)
{
    dstKernel = srcKernel;
    dstKernel.setSubKernelArgFromKernelArg();
    auto kfcArg = std::make_shared<CcuKernelArgKfcServer>();
    kfcArg->opType = param.opType;
    kfcArg->role = role;
    kfcArg->missionIndex = missionIndex;
    dstKernel.setKernelArg(kfcArg);
    CHK_SAFETY_FUNC_RET(strcpy_s(dstKernel.kernelFuncName, sizeof(dstKernel.kernelFuncName), "CcuKernelKfcServer"));
    dstKernel.kernelFunc = reinterpret_cast<void*>(CcuKfcServerKernel);
    return HCCL_SUCCESS;
}

namespace {
uint32_t GetMySubCommRank(const std::vector<std::vector<u32>>& subCommRanks, const u32 rankId)
{
    auto it = std::find(subCommRanks[0].begin(), subCommRanks[0].end(), rankId);
    return (it != subCommRanks[0].end()) ? static_cast<uint32_t>(std::distance(subCommRanks[0].begin(), it)) : 0;
}
} // namespace

CcuTempKfcServer::CcuTempKfcServer(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuAlgTemplateBase(param, rankId, subCommRanks),
      mySubCommRank_(GetMySubCommRank(subCommRanks, rankId)),
      tempRankSize_(static_cast<uint32_t>(subCommRanks[0].size()))
{}

CcuTempKfcServer::~CcuTempKfcServer() {}

HcclResult CcuTempKfcServer::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    HCCL_INFO("[CcuTempKfcServer::CalcRes start]");
    // 不需要从流
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.notifyNumPerThread.clear();
    HCCL_DEBUG(
        "[CcuTempKfcServer::CalcRes] notifyNumOnMainThread[%u] slaveThreadNum[%u]",
        resourceRequest.notifyNumOnMainThread, resourceRequest.slaveThreadNum);

    const uint32_t missionNum = GetKfcServerMissionNum(param.algName);
    CHK_PRT_RET(
        resourceRequest.ccuKernelInfos.size() < missionNum,
        HCCL_ERROR(
            "[CcuTempKfcServer::CalcRes] kernel count[%zu] is less than mission count[%u]",
            resourceRequest.ccuKernelInfos.size(), missionNum),
        HCCL_E_PARA);
    resourceRequest.slaveThreadNum = missionNum - 1U;
    const size_t firstKernelIndex = resourceRequest.ccuKernelInfos.size() - missionNum;

    for (uint32_t missionIndex = 0; missionIndex < missionNum; ++missionIndex) {
        const size_t kernelIndex = firstKernelIndex + missionIndex;
        const CcuKernelInfo& srcKernel = resourceRequest.ccuKernelInfos[kernelIndex];
        const std::string sourceName(srcKernel.kernelFuncName);
        const KfcServerRole role = GetKfcServerRole(param.algName, missionIndex);
        if (!IsKfcSourceKernelMatched(sourceName, param.opType, role)) {
            HCCL_ERROR(
                "[CcuTempKfcServer::CalcRes] unsupported source kernel[%s], opType[%u], role[%u], mission[%u]",
                srcKernel.kernelFuncName, static_cast<uint32_t>(param.opType), static_cast<uint32_t>(role),
                missionIndex);
            return HCCL_E_NOT_SUPPORT;
        }
        HCCL_INFO(
            "[CcuTempKfcServer::CalcRes] mission[%u] inheriting kernelargs from[%s]", missionIndex,
            srcKernel.kernelFuncName);
        CcuKernelInfo dstKernel{};
        CHK_RET(InheritKfcServerKernelArg(srcKernel, dstKernel, param, role, missionIndex));
        resourceRequest.ccuKernelInfos[kernelIndex] = dstKernel;
    }

    HCCL_DEBUG(
        "[CcuTempKfcServer::CalcRes] dimsize=%llu, ccuKernelInfos.size()=%llu", subCommRanks_[0].size(),
        resourceRequest.ccuKernelInfos.size());

    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcServer::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    // 此处仅用于当CCU-KFC作为通信算子时使用
    uint64_t xnAddr = templateDataParams.ccuKfcArgs[0];
    uint64_t ckeAddr = templateDataParams.ccuKfcArgs[1];
    uint64_t workSpace = templateDataParams.ccuKfcArgs[2];
    uint64_t workSpaceSize = templateDataParams.ccuKfcArgs[3];
    uint64_t token = hcomm::CcuRep::GetTokenInfo(workSpace, workSpaceSize);

    // 临时用于参数测试
    uint64_t dieNum = 1;
    uint64_t missionNum = 1;
    uint64_t missionIndex = 0;

    std::unique_ptr<hcomm::CcuTaskArg> taskArg =
        std::make_unique<CcuTaskArgKfcServer>(xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token);
    void* taskArgPtr = static_cast<void*>(taskArg.get());

    HCCL_INFO("[CcuTempKfcServer] KernelRun End.");
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcServer::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
