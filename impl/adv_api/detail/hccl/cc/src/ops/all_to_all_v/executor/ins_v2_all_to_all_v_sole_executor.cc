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

#include <algorithm>
#include <limits>

#include "ins_temp_all_to_all_v_mesh_1D.h"
#include "topo_match_1d.h"

namespace mc2_ops_hccl {
namespace {

bool AddOverflow(u64 left, u64 right, u64& result)
{
    if (right > std::numeric_limits<u64>::max() - left) {
        return true;
    }
    result = left + right;
    return false;
}

bool MulOverflow(u64 left, u64 right, u64& result)
{
    if (left != 0 && right > std::numeric_limits<u64>::max() / left) {
        return true;
    }
    result = left * right;
    return false;
}

} // namespace

template <typename AlgTopoMatch, typename InsAlgTemplate>
InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::InsV2AlltoAllVSoleExecutor()
{}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcAlgHierarchyInfo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PRT_RET(
        topoInfo->userRankSize < 2, HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][CalcRes] rank size is less than 2."),
        HCCL_E_PARA);
    CHK_PRT_RET(
        algHierarchyInfo.infos.empty() || algHierarchyInfo.infos[0].empty(),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][CalcRes] hierarchy info is empty."), HCCL_E_PARA);

    std::shared_ptr<InsAlgTemplate> algTemplate =
        std::make_shared<InsAlgTemplate>(param, topoInfo->userRank, algHierarchyInfo.infos[0]);
    CHK_RET(algTemplate->CalcRes(comm, param, topoInfo, resourceRequest));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::Orchestrate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    rankSize_ = resCtx.topoInfo.userRankSize;
    CHK_PRT_RET(
        rankSize_ < 2, HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] rank size is less than 2."), HCCL_E_PARA);
    CHK_PRT_RET(
        param.all2AllVDataDes.sendType != param.all2AllVDataDes.recvType,
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] send type and recv type differ."), HCCL_E_PARA);

    const u32 dataTypeIndex = static_cast<u32>(param.all2AllVDataDes.sendType);
    CHK_PRT_RET(
        dataTypeIndex >= static_cast<u32>(HCCL_DATA_TYPE_RESERVED),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] invalid datatype[%u].", dataTypeIndex), HCCL_E_PARA);
    dataType_ = param.all2AllVDataDes.sendType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataTypeIndex];
    CHK_PRT_RET(
        dataTypeSize_ == 0, HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] datatype size is zero."),
        HCCL_E_PARA);

    const u64 expectedVarMemSize = ALL_TO_ALL_V_VECTOR_NUM * static_cast<u64>(rankSize_) * sizeof(u64);
    CHK_PRT_RET(
        param.varMemSize != expectedVarMemSize,
        HCCL_ERROR(
            "[InsV2AlltoAllVSoleExecutor][Orchestrate] varMemSize[%llu] is not expected[%llu].", param.varMemSize,
            expectedVarMemSize),
        HCCL_E_PARA);
    CHK_PTR_NULL(param.all2AllVDataDes.sendCounts);
    CHK_PTR_NULL(param.all2AllVDataDes.recvCounts);
    CHK_PTR_NULL(param.all2AllVDataDes.sdispls);
    CHK_PTR_NULL(param.all2AllVDataDes.rdispls);

    CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
    CHK_PRT_RET(
        remoteRankToChannelInfo_.empty(),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][Orchestrate] restored channel map is empty."), HCCL_E_PARA);

    HcclResult ret = OrchestrateLoop(param, resCtx);
    CHK_PRT_RET(
        ret != HCCL_SUCCESS,
        HCCL_ERROR(
            "[InsV2AlltoAllVSoleExecutor][Orchestrate] kernel run failed, error[0x%016llx].", HCCL_ERROR_CODE(ret)),
        ret);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AlltoAllVSoleExecutor<AlgTopoMatch, InsAlgTemplate>::OrchestrateLoop(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    const u64* sendCountsPtr = static_cast<const u64*>(param.all2AllVDataDes.sendCounts);
    const u64* recvCountsPtr = static_cast<const u64*>(param.all2AllVDataDes.recvCounts);
    const u64* sendDisplsPtr = static_cast<const u64*>(param.all2AllVDataDes.sdispls);
    const u64* recvDisplsPtr = static_cast<const u64*>(param.all2AllVDataDes.rdispls);
    std::vector<u64> sendCounts(sendCountsPtr, sendCountsPtr + rankSize_);
    std::vector<u64> recvCounts(recvCountsPtr, recvCountsPtr + rankSize_);
    std::vector<u64> sendDispls(sendDisplsPtr, sendDisplsPtr + rankSize_);
    std::vector<u64> recvDispls(recvDisplsPtr, recvDisplsPtr + rankSize_);

    u64 maxSendOrRecvCount = 0;
    for (u32 rank = 0; rank < rankSize_; ++rank) {
        maxSendOrRecvCount = std::max(maxSendOrRecvCount, sendCounts[rank]);
        maxSendOrRecvCount = std::max(maxSendOrRecvCount, recvCounts[rank]);
    }
    if (maxSendOrRecvCount == 0) {
        HCCL_INFO("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] all counts are zero.");
        return HCCL_SUCCESS;
    }

    CHK_PRT_RET(
        resCtx.algHierarchyInfo.infos.empty() || resCtx.algHierarchyInfo.infos[0].empty(),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] hierarchy info is empty."), HCCL_E_PARA);

    TemplateResource templateResource;
    templateResource.channels = remoteRankToChannelInfo_[0];
    templateResource.threads = resCtx.threads;

    TemplateDataParams templateParams;
    templateParams.buffInfo.inputPtr = param.inputPtr;
    templateParams.buffInfo.outputPtr = param.outputPtr;
    templateParams.buffInfo.inputSize = param.inputSize;
    templateParams.buffInfo.outputSize = param.outputSize;
    templateParams.buffInfo.hcclBuff = resCtx.cclMem;
    templateParams.buffInfo.inBuffType = BufferType::INPUT;
    templateParams.buffInfo.outBuffType = BufferType::OUTPUT;
    templateParams.buffInfo.hcclBuffType = BufferType::HCCL_BUFFER;
    templateParams.repeatNum = 1;
    templateParams.dataType = dataType_;

    InsAlgTemplate algTemplate(param, resCtx.topoInfo.userRank, resCtx.algHierarchyInfo.infos[0]);
    const u64 scratchMultiplier =
        algTemplate.CalcScratchMultiple(templateParams.buffInfo.inBuffType, templateParams.buffInfo.outBuffType);
    u64 maxDataSizePerLoop = UB_MAX_DATA_SIZE;
    if (scratchMultiplier != 0) {
        const u64 scratchBound = resCtx.cclMem.size / scratchMultiplier / HCCL_MIN_SLICE_ALIGN * HCCL_MIN_SLICE_ALIGN;
        maxDataSizePerLoop = std::min(maxDataSizePerLoop, scratchBound);
    }
    const u64 maxCountPerLoop = maxDataSizePerLoop / dataTypeSize_;
    CHK_PRT_RET(
        maxCountPerLoop == 0, HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] max count per loop is zero."),
        HCCL_E_PARA);

    u64 unused = 0;
    CHK_PRT_RET(
        MulOverflow(maxCountPerLoop, dataTypeSize_, templateParams.inputSliceStride),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] input stride overflows."), HCCL_E_PARA);
    CHK_PRT_RET(
        MulOverflow(maxSendOrRecvCount, dataTypeSize_, templateParams.outputSliceStride),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] output stride overflows."), HCCL_E_PARA);
    CHK_PRT_RET(
        MulOverflow(maxSendOrRecvCount, dataTypeSize_, unused),
        HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] data size overflows."), HCCL_E_PARA);

    const u64 loopTimes =
        maxSendOrRecvCount / maxCountPerLoop + static_cast<u64>(maxSendOrRecvCount % maxCountPerLoop != 0);
    u64 processedCount = 0;
    for (u64 loop = 0; loop < loopTimes; ++loop) {
        const u64 currentCount = (loop + 1 == loopTimes) ? maxSendOrRecvCount - processedCount : maxCountPerLoop;
        CHK_PRT_RET(
            MulOverflow(currentCount, dataTypeSize_, templateParams.sliceSize),
            HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] slice size overflows."), HCCL_E_PARA);
        templateParams.tailSize = templateParams.sliceSize;
        templateParams.count = currentCount;
        templateParams.processedDataCount = processedCount;
        templateParams.buffInfo.inBuffBaseOff = 0;
        templateParams.buffInfo.outBuffBaseOff = 0;
        templateParams.buffInfo.hcclBuffBaseOff = 0;
        templateParams.inputRepeatStride = 0;
        templateParams.outputRepeatStride = 0;
        templateParams.sendCounts.assign(rankSize_, 0);
        templateParams.recvCounts.assign(rankSize_, 0);
        templateParams.sdispls.assign(rankSize_, 0);
        templateParams.rdispls.assign(rankSize_, 0);

        for (u32 rank = 0; rank < rankSize_; ++rank) {
            const u64 processedSend = std::min(processedCount, sendCounts[rank]);
            const u64 processedRecv = std::min(processedCount, recvCounts[rank]);
            templateParams.sendCounts[rank] = std::min(currentCount, sendCounts[rank] - processedSend);
            templateParams.recvCounts[rank] = std::min(currentCount, recvCounts[rank] - processedRecv);
            CHK_PRT_RET(
                AddOverflow(sendDispls[rank], processedSend, templateParams.sdispls[rank]),
                HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] send displacement overflows."), HCCL_E_PARA);
            CHK_PRT_RET(
                AddOverflow(recvDispls[rank], processedRecv, templateParams.rdispls[rank]),
                HCCL_ERROR("[InsV2AlltoAllVSoleExecutor][OrchestrateLoop] recv displacement overflows."), HCCL_E_PARA);
        }

        CHK_RET(algTemplate.KernelRun(param, templateParams, templateResource));
        processedCount += currentCount;
    }
    return HCCL_SUCCESS;
}

REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_ALLTOALLV, InsAlltoAllVMesh1D, InsV2AlltoAllVSoleExecutor, TopoMatch1D,
    InsTempAlltoAllVMesh1D);
} // namespace mc2_ops_hccl
