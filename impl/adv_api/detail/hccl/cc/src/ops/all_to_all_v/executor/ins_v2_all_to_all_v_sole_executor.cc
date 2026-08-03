/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ins_v2_all_to_all_v_sole_executor.h"
#include "topo_match_1d.h"
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
#include "ccu_temp_kfc_half_all_to_all_v_mesh_1D.h"
#endif
#include <cstddef>
namespace mc2_ops_hccl {

template <typename AlgTopoMatch, typename InsAlgTemplate>
InsV2AllToAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::InsV2AllToAllVSoleExecutor()
{}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllToAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcAlgHierarchyInfo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllToAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    std::shared_ptr<InsAlgTemplate> algTemplate =
        std::make_shared<InsAlgTemplate>(param, topoInfo->userRank, algHierarchyInfo.infos[0]);
    CHK_RET(algTemplate->CalcRes(comm, param, topoInfo, resourceRequest));
    myRank_ = topoInfo->userRank;
    HCCL_DEBUG(
        "[InsV2AllToAllVSoleExecutor][CalcRes] myRank[%u], notifyNumOnMainThread[%u], slaveThreadNum[%u], "
        "channels[%zu]",
        myRank_, resourceRequest.notifyNumOnMainThread, resourceRequest.slaveThreadNum,
        resourceRequest.channels.size());
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllToAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::Orchestrate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO(
        "[InsV2AllToAllVSoleExecutor][Orchestrate] Orchestrate Start, opType %u, algName[%s].",
        static_cast<u32>(param.opType), param.algName);
    myRank_ = resCtx.topoInfo.userRank;

    threads_ = resCtx.threads;
    if (param.engine != CommEngine::COMM_ENGINE_AIV && param.engine != CommEngine::COMM_ENGINE_CCU) {
        CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
    }
    // AllToAllV slices are variable-length: sendType drives the element size, the per-peer counts live in
    // sendCounts/sdispls and are consumed on the CCU side rather than here.
    dataType_ = param.all2AllVDataDes.sendType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataType_];
    dataSize_ = param.inputSize;

    HcclResult ret = OrchestrateLoop(param, resCtx);
    CHK_PRT_RET(
        ret != HCCL_SUCCESS,
        HCCL_ERROR(
            "[InsV2AllToAllVSoleExecutor][Orchestrate]errNo[0x%016llx] All to all v executor kernel run failed",
            HCCL_ERROR_CODE(ret)),
        ret);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllToAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::OrchestrateLoop(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[InsV2AllToAllVSoleExecutor][OrchestrateLoop] Start");

    TemplateResource templateAlgRes;
    if (param.engine == COMM_ENGINE_CCU) {
        templateAlgRes.ccuKernels = resCtx.ccuKernels;
    }
    if (param.engine != CommEngine::COMM_ENGINE_AIV && remoteRankToChannelInfo_.size() > 0) {
        templateAlgRes.channels = remoteRankToChannelInfo_[0];
    }
    templateAlgRes.threads = resCtx.threads;
    templateAlgRes.aivCommInfoPtr = resCtx.aivCommInfoPtr;

    TemplateDataParams tempAlgParams;
    tempAlgParams.buffInfo.inputPtr = param.inputPtr;
    tempAlgParams.buffInfo.outputPtr = param.outputPtr;
    tempAlgParams.buffInfo.hcclBuff = resCtx.cclMem;
    tempAlgParams.buffInfo.inBuffType = BufferType::INPUT;
    tempAlgParams.buffInfo.outBuffType = BufferType::OUTPUT;
    tempAlgParams.buffInfo.hcclBuffType = BufferType::HCCL_BUFFER;
    tempAlgParams.buffInfo.inputSize = param.inputSize;
    tempAlgParams.buffInfo.outputSize = param.outputSize;
    tempAlgParams.buffInfo.inBuffBaseOff = 0;
    tempAlgParams.buffInfo.outBuffBaseOff = 0;
    tempAlgParams.buffInfo.hcclBuffBaseOff = 0;
    tempAlgParams.enableRemoteMemAccess = param.opMode == OpMode::OFFLOAD;
    tempAlgParams.repeatNum = 1;
    tempAlgParams.inputRepeatStride = 0;
    tempAlgParams.outputRepeatStride = 0;

    InsAlgTemplate algTemplate(param, resCtx.topoInfo.userRank, resCtx.algHierarchyInfo.infos[0]);
    maxTmpMemSize_ = tempAlgParams.buffInfo.hcclBuff.size;

    // Variable-length slices cannot be split into uniform per-loop chunks the way AllGather does, so the
    // whole operation is orchestrated in one shot; the per-peer sizes are resolved inside the CCU kernel.
    HCCL_INFO(
        "[InsV2AllToAllVSoleExecutor][OrchestrateLoop] myRank[%u], inputSize[%llu], cclSize[%llu], threadSize[%zu]",
        myRank_, param.inputSize, resCtx.cclMem.size, templateAlgRes.threads.size());
    CHK_RET(algTemplate.KernelRun(param, tempAlgParams, templateAlgRes));

    HCCL_INFO("[InsV2AllToAllVSoleExecutor][OrchestrateLoop] End.");
    return HCCL_SUCCESS;
}

#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU

REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_HALF_ALLTOALLV, CcuHalfAll2AllVMesh1D, InsV2AllToAllVSoleExecutor, TopoMatch1D,
    CcuTempKfcHalfAllToAllVMesh1D);

#endif
} // namespace mc2_ops_hccl
