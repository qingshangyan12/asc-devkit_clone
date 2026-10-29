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
#include "ccu_temp_kfc_all_gather_nhr_1D_multi_jetty_mem2mem.h"
#include "kernel/ccu_kernel_kfc_server.h"
#include "ccu_temp_kfc_server.h"
#include "ccu_launch_dl.h"

namespace mc2_ops_hccl {
// 从源 CcuKernelInfo 继承属性到 KfcServer 的 CcuKernelInfo
HcclResult InheritKfcServerKernelArg(
    const CcuKernelInfo& srcKernel, CcuKernelInfo& dstKernel, const OpParam& param, KfcServerRole role,
    uint32_t missionIndex)
{
    dstKernel = srcKernel;
    CHK_PTR_NULL(srcKernel.kernelArg);
    const auto* srcArg = static_cast<const CcuKernelArgBase*>(srcKernel.kernelArg);
    auto kfcArg = std::make_shared<CcuKernelArgKfcServer>();
    kfcArg->opType = param.opType;
    kfcArg->role = role;
    kfcArg->missionIndex = missionIndex;
    kfcArg->opParam = param;
    kfcArg->algSubType = srcArg->algSubType;
    kfcArg->algArg = srcArg->algArg;
    if (param.algName != nullptr && std::strcmp(param.algName, KFC_PARALLEL_ALL_GATHER_ALG_NAME) == 0) {
        CHK_PRT_RET(
            kfcArg->algSubType != KFC_PARALLEL_ALL_GATHER_SUB_TYPE || !kfcArg->algArg,
            HCCL_ERROR("[ParallelAllGather] missing algorithm arguments"), HCCL_E_PARA);
    }
    CHK_PTR_NULL(srcKernel.kernelFunc);
    dstKernel.setSubKernelFromKernel();
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
    if (param.opType != HcclCMDType::HCCL_CMD_ALLGATHER && param.opType != HcclCMDType::HCCL_CMD_REDUCE_SCATTER &&
        param.opType != HcclCMDType::HCCL_CMD_ALLTOALL && param.opType != HcclCMDType::HCCL_CMD_ALLTOALLV &&
        param.opType != HcclCMDType::HCCL_CMD_ALLREDUCE) {
        HCCL_ERROR("[CcuTempKfcServer::CalcRes] unsupported opType[%u]", static_cast<uint32_t>(param.opType));
        return HCCL_E_NOT_SUPPORT;
    }
    // 不需要从流
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.notifyNumPerThread.clear();
    HCCL_DEBUG(
        "[CcuTempKfcServer::CalcRes] notifyNumOnMainThread[%u] slaveThreadNum[%u]",
        resourceRequest.notifyNumOnMainThread, resourceRequest.slaveThreadNum);

    const uint32_t missionNum = GetKfcServerMissionNum(param.algName);
    // SoleNHR 动态 mission 特例（RS 2Die / AG SoleNHR D4）：mission 数 = dieNum（模板 CalcRes 按
    // 通道探测塞入的源 kernel 数），静态名单不可知（单 die=1 / 双 die=2），按尾部同名源 kernel 计数修正
    const bool isSoleNhr2Die =
        param.algName != nullptr && std::strcmp(param.algName, KFC_RS_SOLE_NHR_2DIE_ALG_NAME) == 0;
    const bool isAgSoleNhrDynDie =
        param.algName != nullptr && std::strcmp(param.algName, KFC_AG_SOLE_NHR_ALG_NAME) == 0;
    uint32_t soleNhr2DieMissionNum = missionNum;
    if (isSoleNhr2Die || isAgSoleNhrDynDie) {
        const char* dynMissionKernelName =
            isSoleNhr2Die ? "CcuKernelKfcReduceScatterNHR1D2DieMem2Mem" : "CcuKernelKfcAllGatherNHR1DMem2Mem";
        uint32_t trailingCount = 0;
        for (auto it = resourceRequest.ccuKernelInfos.rbegin(); it != resourceRequest.ccuKernelInfos.rend(); ++it) {
            if (std::strcmp(it->kernelFuncName, dynMissionKernelName) != 0) {
                break;
            }
            ++trailingCount;
        }
        CHK_PRT_RET(
            trailingCount == 0U,
            HCCL_ERROR(
                "[CcuTempKfcServer::CalcRes] SoleNHR dyn-mission expects at least one source kernel, got [%zu]",
                resourceRequest.ccuKernelInfos.size()),
            HCCL_E_PARA);
        soleNhr2DieMissionNum = std::min(trailingCount, KFC_MAX_MISSION_NUM);
    }
    const uint32_t effectiveMissionNum = (isSoleNhr2Die || isAgSoleNhrDynDie) ? soleNhr2DieMissionNum : missionNum;
    CHK_PRT_RET(
        resourceRequest.ccuKernelInfos.size() < effectiveMissionNum,
        HCCL_ERROR(
            "[CcuTempKfcServer::CalcRes] kernel count[%zu] is less than mission count[%u]",
            resourceRequest.ccuKernelInfos.size(), effectiveMissionNum),
        HCCL_E_PARA);
    resourceRequest.slaveThreadNum = effectiveMissionNum - 1U;
    const size_t firstKernelIndex = resourceRequest.ccuKernelInfos.size() - effectiveMissionNum;

    for (uint32_t missionIndex = 0; missionIndex < effectiveMissionNum; ++missionIndex) {
        const size_t kernelIndex = firstKernelIndex + missionIndex;
        const CcuKernelInfo& srcKernel = resourceRequest.ccuKernelInfos[kernelIndex];
        const std::string sourceName(srcKernel.kernelFuncName);
        const bool isAllGather =
            sourceName == "CcuKernelAllGatherMesh1DMem2Mem" && param.opType == HcclCMDType::HCCL_CMD_ALLGATHER;
        const bool isAllGatherKfc =
            sourceName == "CcuKernelKfcAllGatherMesh1DMem2Mem" && param.opType == HcclCMDType::HCCL_CMD_ALLGATHER;
        const bool isAllGatherNhr =
            sourceName == "CcuKernelAllGatherNHR1DMultiJettyMem2Mem" && param.opType == HcclCMDType::HCCL_CMD_ALLGATHER;
        const bool isAllGatherSoleNhr =
            sourceName == "CcuKernelKfcAllGatherNHR1DMem2Mem" && param.opType == HcclCMDType::HCCL_CMD_ALLGATHER;
        const bool isReduceScatter = sourceName == "CcuKernelKfcReduceScatterMesh1DMem2Mem" &&
                                     param.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
        const bool isReduceScatterPeerOnly = sourceName == KFC_REDUCE_SCATTER_PEER_ONLY_KERNEL_NAME &&
                                             param.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
        const bool isReduceScatterNhr = sourceName == "CcuKernelReduceScatterNHR1DMultiJettyMem2Mem" &&
                                        param.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
        const bool isReduceScatterNhr2Die = sourceName == "CcuKernelKfcReduceScatterNHR1D2DieMem2Mem" &&
                                            param.opType == HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
        const bool isAlltoAll =
            sourceName == "CcuKernelAlltoAllMesh1D" && param.opType == HcclCMDType::HCCL_CMD_ALLTOALL;
        const bool isAllToAllV =
            sourceName == "CcuKernelAlltoAllVMesh1D" && param.opType == HcclCMDType::HCCL_CMD_ALLTOALLV;
        const bool isAlltoAllKfcMultiJetty =
            sourceName == "CcuKernelKfcAllToAllMesh1DMultiJetty" && param.opType == HcclCMDType::HCCL_CMD_ALLTOALL;
        const bool isAlltoAllMultiJetty =
            sourceName == "CcuAllToAllMesh1DMultiJettyKernel" && param.opType == HcclCMDType::HCCL_CMD_ALLTOALL;
        const bool isAllReduce =
            sourceName == "CcuKernelAllReduceMesh1DMem2Mem" && param.opType == HcclCMDType::HCCL_CMD_ALLREDUCE;
        const bool roleMatches = missionNum == 1U ||
                                 (missionIndex == 0U && (isAllGather || isAllGatherKfc || isReduceScatter)) ||
                                 (missionIndex == 1U && (isAllGatherNhr || isReduceScatterNhr)) ||
                                 (missionNum == KFC_MAX_MISSION_NUM && isAlltoAllKfcMultiJetty) ||
                                 isAlltoAllMultiJetty || (isSoleNhr2Die && isReduceScatterNhr2Die);
        if ((!isAllGather && !isAllGatherKfc && !isAllGatherNhr && !isReduceScatter && !isReduceScatterPeerOnly &&
             !isReduceScatterNhr && !isReduceScatterNhr2Die && !isAlltoAll && !isAlltoAllKfcMultiJetty &&
             !isAllReduce && !isAllToAllV && !isAlltoAllMultiJetty && !isAllGatherSoleNhr) ||
            !roleMatches) {
            HCCL_ERROR(
                "[CcuTempKfcServer::CalcRes] unsupported or misordered source kernel[%s] at mission[%u]",
                srcKernel.kernelFuncName, missionIndex);
            return HCCL_E_NOT_SUPPORT;
        }
        HCCL_INFO(
            "[CcuTempKfcServer::CalcRes] mission[%u] inheriting kernelargs from[%s]", missionIndex,
            srcKernel.kernelFuncName);
        CcuKernelInfo dstKernel{};
        CHK_RET(InheritKfcServerKernelArg(
            srcKernel, dstKernel, param, GetKfcServerRole(param.algName, missionIndex), missionIndex));
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
