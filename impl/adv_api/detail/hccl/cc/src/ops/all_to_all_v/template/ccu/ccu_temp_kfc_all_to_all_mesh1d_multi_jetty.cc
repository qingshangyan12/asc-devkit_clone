/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "alg_data_trans_wrapper.h"
#include "channel.h"
#include "ccu_launch_dl.h"
#include "ccu_log.h"
#include "ccu_temp_kfc_all_to_all_mesh1d_multi_jetty.h"

namespace mc2_ops_hccl {

CcuTempKfcAllToAllMesh1DMultiJetty::CcuTempKfcAllToAllMesh1DMultiJetty(
    const OpParam& param, u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    const auto& ranks = subCommRanks_[0];
    const auto it = std::find(ranks.begin(), ranks.end(), rankId);
    if (it != ranks.end()) {
        mySubCommRank_ = static_cast<uint32_t>(std::distance(ranks.begin(), it));
    }
    tempRankSize_ = static_cast<uint32_t>(ranks.size());
}

HcclResult CcuTempKfcAllToAllMesh1DMultiJetty::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1U);

    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMeshClosMultiJetty(comm, param, topoInfo, subCommRanks_, channelDescs));

    return CalcResByChannelDescsImpl(param, channelDescs, resourceRequest);
}

HcclResult CcuTempKfcAllToAllMesh1DMultiJetty::CalcResByChannelDescsImpl(
    const OpParam& param, const std::vector<HcclChannelDesc>& channelDescs, AlgResourceRequest& resourceRequest) const
{
    auto kernelArg = std::make_shared<CcuKernelArgKfcAllToAllMesh1DMultiJetty>();
    kernelArg->rankSize = tempRankSize_;
    kernelArg->rankId = mySubCommRank_;
    kernelArg->opParam = param;
    kernelArg->subCommRanks = subCommRanks_;

    CcuKernelInfo kernelInfo{};
    CHK_SAFETY_FUNC_RET(
        strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelKfcAllToAllMesh1DMultiJetty"));
    kernelInfo.channels = channelDescs;
    kernelInfo.setKernelArg(kernelArg);
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllToAllMesh1DMultiJetty::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    const uint64_t sliceSize = templateDataParams.sliceSize;
    if (sliceSize == 0) {
        HCCL_INFO("[CcuTempKfcAllToAllMesh1DMultiJetty] sliceSize is 0, skip.");
        return HCCL_SUCCESS;
    }

    const auto& sdispls = templateDataParams.sdispls;
    const uint32_t dataTypeSize = DATATYPE_SIZE_TABLE[param.all2AllVDataDes.sendType];
    const uint64_t srcStride = (sdispls.size() >= 2U) ? (sdispls[1] - sdispls[0]) * dataTypeSize : sliceSize;
    const uint64_t srcOffset = 0U;
    const uint64_t dstOffset = static_cast<uint64_t>(mySubCommRank_) * srcStride;

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(sliceSize, config);

    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(
        submitInfo, 1ULL, inputAddr, outputAddr, sliceSize, srcStride, srcOffset, dstOffset, goSize[0], goSize[1],
        goSize[2], goSize[3]));
    templateResource.submitInfos.push_back(submitInfo);

    HCCL_INFO(
        "[CcuTempKfcAllToAllMesh1DMultiJetty] inputAddr[%llu], outputAddr[%llu], sliceSize[%llu], "
        "srcStride[%llu], srcOffset[%llu], dstOffset[%llu]",
        inputAddr, outputAddr, sliceSize, srcStride, srcOffset, dstOffset);
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllToAllMesh1DMultiJetty::GetRes(AlgResourceRequest& resourceRequest) const
{
    resourceRequest.slaveThreadNum = 0;
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
    return HCCL_SUCCESS;
}

u64 CcuTempKfcAllToAllMesh1DMultiJetty::GetThreadNum() const { return 1; }

u64 CcuTempKfcAllToAllMesh1DMultiJetty::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

#ifndef AICPU_COMPILE
HcclResult CcuTempKfcAllToAllMesh1DMultiJetty::FastLaunch(
    const OpParam& param, const TemplateFastLaunchCtx& tempFastLaunchCtx)
{
    if (tempFastLaunchCtx.ccuKernelSubmitInfos.empty()) {
        HCCL_INFO("[CcuTempKfcAllToAllMesh1DMultiJetty::FastLaunch] ccu kernel num is 0, just success.");
        return HCCL_SUCCESS;
    }
    HCCL_DEBUG("[CcuTempKfcAllToAllMesh1DMultiJetty::FastLaunch] start");
    buffInfo_ = tempFastLaunchCtx.buffInfo;

    const uint64_t* cachedArgs = tempFastLaunchCtx.ccuKernelSubmitInfos[0].cachedArgs;
    uint64_t token = PointerToAddr(buffInfo_.hcclBuff.addr);

    uint64_t taskArgs[11];
    taskArgs[0] = cachedArgs[1];
    taskArgs[1] = cachedArgs[2];
    taskArgs[2] = token;
    taskArgs[3] = cachedArgs[3];
    taskArgs[4] = cachedArgs[4];
    taskArgs[5] = cachedArgs[5];
    taskArgs[6] = cachedArgs[6];
    taskArgs[7] = cachedArgs[7];
    taskArgs[8] = cachedArgs[8];
    taskArgs[9] = cachedArgs[9];
    taskArgs[10] = cachedArgs[10];

    CcuResult launchRet = HcommCcuKernelLaunch(
        tempFastLaunchCtx.threads[0], tempFastLaunchCtx.ccuKernelSubmitInfos[0].kernelHandle, taskArgs, 11);
    if (launchRet != CCU_SUCCESS) {
        HCCL_ERROR("[CcuTempKfcAllToAllMesh1DMultiJetty::FastLaunch] kernel launch failed, ccuRet -> %d", launchRet);
        return ConvertCcuToHccl(launchRet);
    }

    HCCL_DEBUG("[CcuTempKfcAllToAllMesh1DMultiJetty::FastLaunch] end");
    return HCCL_SUCCESS;
}
#endif

} // namespace mc2_ops_hccl
