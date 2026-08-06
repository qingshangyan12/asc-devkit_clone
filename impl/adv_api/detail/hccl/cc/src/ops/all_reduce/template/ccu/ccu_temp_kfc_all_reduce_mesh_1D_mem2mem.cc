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

#include "channel.h"
#include "ccu_assist_pub.h"
#include "ccu_kernel_alg_base.h"
#include "hccl_ccu_res.h"

namespace mc2_ops_hccl {

CcuTempKfcAllReduceMesh1DMem2Mem::CcuTempKfcAllReduceMesh1DMem2Mem(
    const OpParam& param, u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    CHK_SAFETY_FUNC_RET(strcpy_s(
        kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelKfcAllReduceMesh1DMem2Mem"));
    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    (void)param;
    buffInfo_ = templateDataParams.buffInfo;
    const u64 inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const u64 outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    const u64 scratchAddr = PointerToAddr(buffInfo_.hcclBuff.addr) + buffInfo_.hcclBuffBaseOff;

    const u64 dataTypeSize = DATATYPE_SIZE_TABLE[dataType_];
    const u64 sliceCount = RoundUp(templateDataParams.count, templateRankSize_);
    const u64 normalSliceSize = sliceCount * dataTypeSize;
    const u64 sliceOffset = normalSliceSize * mySubCommRank_;
    const u64 processedSize = std::min(sliceOffset, templateDataParams.sliceSize);
    const u64 sliceSize = std::min(normalSliceSize, templateDataParams.sliceSize - processedSize);

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(sliceSize, config);

    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(
        submitInfo, inputAddr, outputAddr, scratchAddr, sliceOffset, sliceSize, goSize[0], goSize[1], goSize[2],
        goSize[3]));
    templateResource.submitInfos.push_back(submitInfo);
    return HCCL_SUCCESS;
}

u64 CcuTempKfcAllReduceMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return templateRankSize_;
}

} // namespace mc2_ops_hccl
