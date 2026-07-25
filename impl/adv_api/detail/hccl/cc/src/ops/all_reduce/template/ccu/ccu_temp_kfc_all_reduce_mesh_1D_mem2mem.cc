/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_temp_kfc_all_reduce_mesh_1D_mem2mem.h"

#include "ccu_assist_pub.h"
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_utils.h"
#include "channel.h"
#include "hccl_ccu_res.h"

namespace mc2_ops_hccl {

CcuTempKfcAllReduceMesh1DMem2Mem::CcuTempKfcAllReduceMesh1DMem2Mem(
    const OpParam& param, u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    const std::vector<u32>& ranks = subCommRanks[0];
    templateRankSize_ = ranks.size();
    auto it = std::find(ranks.begin(), ranks.end(), rankId);
    if (it != ranks.end()) {
        mySubCommRank_ = std::distance(ranks.begin(), it);
    }
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy(kernelInfo.kernelFuncName, "CcuKernelAllReduceMesh1DMem2Mem");

    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    CHK_PRT_RET(
        channelDescs.size() + 1 != templateRankSize_,
        HCCL_ERROR(
            "[CcuTempKfcAllReduceMesh1DMem2Mem] channelCount[%zu] does not match rankSize[%u]", channelDescs.size(),
            templateRankSize_),
        HCCL_E_INTERNAL);

    // CcuKernelKfcServer replaces this placeholder and reuses the channels during resource registration.
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    CHK_PRT_RET(
        templateRankSize_ == 0, HCCL_ERROR("[CcuTempKfcAllReduceMesh1DMem2Mem] rankSize is 0"), HCCL_E_INTERNAL);

    const BuffInfo& buffInfo = templateDataParams.buffInfo;
    const uint64_t inputAddr = PointerToAddr(buffInfo.inputPtr) + buffInfo.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo.outputPtr) + buffInfo.outBuffBaseOff;
    const uint64_t scratchAddr = PointerToAddr(buffInfo.hcclBuff.addr) + buffInfo.hcclBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo, token));

    const uint64_t dataTypeSize = DataTypeSizeGet(param.DataDes.dataType);
    CHK_PRT_RET(dataTypeSize == 0, HCCL_ERROR("[CcuTempKfcAllReduceMesh1DMem2Mem] dataTypeSize is 0"), HCCL_E_PARA);
    const uint64_t totalSize = templateDataParams.sliceSize;
    const uint64_t normalSliceCount = (templateDataParams.count + templateRankSize_ - 1) / templateRankSize_;
    const uint64_t normalSliceSize = normalSliceCount * dataTypeSize;
    const uint64_t normalRankCount = normalSliceCount == 0 ? 0 : templateDataParams.count / normalSliceCount;
    const uint64_t lastSliceSize = normalRankCount == templateRankSize_ ?
                                       totalSize - (normalRankCount - 1) * normalSliceSize :
                                       totalSize - normalRankCount * normalSliceSize;
    const uint64_t mySliceSize = mySubCommRank_ < normalRankCount ? normalSliceSize : lastSliceSize;
    const uint64_t sliceOffset = mySubCommRank_ * normalSliceSize;

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_M2M_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE;
    const std::vector<uint64_t> goSize = CalGoSize(mySliceSize, config);

    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(
        submitInfo, inputAddr, outputAddr, token, scratchAddr, sliceOffset, sliceOffset, normalSliceSize, lastSliceSize,
        mySliceSize, sliceOffset, 0, goSize[0], goSize[1], goSize[2], goSize[3]));
    templateResource.submitInfos.push_back(submitInfo);
    return HCCL_SUCCESS;
}

u64 CcuTempKfcAllReduceMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return templateRankSize_;
}

u64 CcuTempKfcAllReduceMesh1DMem2Mem::GetThreadNum() const { return 1; }

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::GetRes(AlgResourceRequest& resourceRequest) const
{
    resourceRequest.slaveThreadNum = 0;
    resourceRequest.notifyNumOnMainThread = 0;
    return HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
