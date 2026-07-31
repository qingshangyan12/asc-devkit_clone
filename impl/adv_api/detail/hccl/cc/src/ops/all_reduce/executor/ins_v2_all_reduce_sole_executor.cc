/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ins_v2_all_reduce_sole_executor.h"
#include "topo_match_1d.h"
#include "ins_temp_all_reduce_mesh_1D_one_shot.h"

namespace mc2_ops_hccl {

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllReduceSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcAlgHierarchyInfo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllReduceSoleExecutor<AlgTopoMatch, InsAlgTemplate>::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    std::shared_ptr<InsAlgTemplate> algTemplate =
        std::make_shared<InsAlgTemplate>(param, topoInfo->userRank, algHierarchyInfo.infos[0]);
    CHK_RET(algTemplate->CalcRes(comm, param, topoInfo, resourceRequest));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllReduceSoleExecutor<AlgTopoMatch, InsAlgTemplate>::Orchestrate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    dataCount_ = param.DataDes.count;
    dataType_ = param.DataDes.dataType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[param.DataDes.dataType];
    CHK_PRT_RET(
        dataTypeSize_ == 0 || dataCount_ > UINT64_MAX / dataTypeSize_,
        HCCL_ERROR("[InsV2AllReduceSoleExecutor] invalid count[%llu] or dataType[%d]", dataCount_, dataType_),
        HCCL_E_PARA);
    dataSize_ = dataCount_ * dataTypeSize_;

    if (param.engine != CommEngine::COMM_ENGINE_AIV) {
        CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
    }

    return OrchestrateLoop(param, resCtx);
}

template <typename AlgTopoMatch, typename InsAlgTemplate>
HcclResult InsV2AllReduceSoleExecutor<AlgTopoMatch, InsAlgTemplate>::OrchestrateLoop(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    TemplateResource templateResource;
    templateResource.ccuKernels = resCtx.ccuKernels;
    templateResource.threads = resCtx.threads;
    templateResource.aivCommInfoPtr = resCtx.aivCommInfoPtr;
    if (param.engine != CommEngine::COMM_ENGINE_AIV && !remoteRankToChannelInfo_.empty()) {
        templateResource.channels = remoteRankToChannelInfo_[0];
    }

    TemplateDataParams dataParams;
    dataParams.buffInfo.inputPtr = param.inputPtr;
    dataParams.buffInfo.outputPtr = param.outputPtr;
    dataParams.buffInfo.hcclBuff = resCtx.cclMem;
    dataParams.buffInfo.inBuffType = BufferType::INPUT;
    dataParams.buffInfo.outBuffType = BufferType::OUTPUT;
    dataParams.buffInfo.hcclBuffType = BufferType::HCCL_BUFFER;
    dataParams.buffInfo.inputSize = param.inputSize;
    dataParams.buffInfo.outputSize = param.outputSize;
    dataParams.enableRemoteMemAccess = param.opMode == OpMode::OFFLOAD;
    dataParams.repeatNum = 1;

    std::shared_ptr<InsAlgTemplate> algTemplate =
        std::make_shared<InsAlgTemplate>(param, resCtx.topoInfo.userRank, resCtx.algHierarchyInfo.infos[0]);
    const u64 scratchMultiplier =
        algTemplate->CalcScratchMultiple(dataParams.buffInfo.inBuffType, dataParams.buffInfo.outBuffType);
    const u64 scratchBound = scratchMultiplier == 0 ?
                                 UB_MAX_DATA_SIZE :
                                 resCtx.cclMem.size / scratchMultiplier / HCCL_MIN_SLICE_ALIGN * HCCL_MIN_SLICE_ALIGN;
    const u64 maxDataSizePerLoop = std::min<u64>(UB_MAX_DATA_SIZE, scratchBound);
    const u64 maxCountPerLoop = maxDataSizePerLoop / dataTypeSize_;
    CHK_PRT_RET(maxCountPerLoop == 0, HCCL_ERROR("[InsV2AllReduceSoleExecutor] maxCountPerLoop is 0"), HCCL_E_INTERNAL);

    const u64 loopTimes = dataCount_ / maxCountPerLoop + static_cast<u64>(dataCount_ % maxCountPerLoop != 0);
    u64 processedCount = 0;
    for (u64 loop = 0; loop < loopTimes; ++loop) {
        const u64 currentCount = loop + 1 == loopTimes ? dataCount_ - processedCount : maxCountPerLoop;
        dataParams.count = currentCount;
        dataParams.sliceSize = currentCount * dataTypeSize_;
        dataParams.tailSize = dataParams.sliceSize;
        dataParams.inputSliceStride = 0;
        dataParams.outputSliceStride = 0;
        dataParams.inputRepeatStride = 0;
        dataParams.outputRepeatStride = 0;
        dataParams.buffInfo.inBuffBaseOff = processedCount * dataTypeSize_;
        dataParams.buffInfo.outBuffBaseOff = processedCount * dataTypeSize_;
        dataParams.buffInfo.hcclBuffBaseOff = 0;
        CHK_RET(algTemplate->KernelRun(param, dataParams, templateResource));
        processedCount += currentCount;
    }
    return HCCL_SUCCESS;
}

REGISTER_EXEC_V2(
    HcclCMDType::HCCL_CMD_ALLREDUCE, InsAllReduceMesh1DOneShot, InsV2AllReduceSoleExecutor, TopoMatch1D,
    InsTempAllReduceMesh1DOneShot);

} // namespace mc2_ops_hccl
