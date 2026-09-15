/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
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
#include "ccu_temp_kfc_all_reduce_mesh_1D_mem2mem.h"
#include "kernel/ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"

namespace mc2_ops_hccl {

CcuTempKfcAllReduceMesh1DMem2Mem::CcuTempKfcAllReduceMesh1DMem2Mem(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcAllReduceMesh1DMem2Mem::~CcuTempKfcAllReduceMesh1DMem2Mem() {}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelAllReduceMesh1DMem2Mem");
    kernelInfo.kernelFunc = reinterpret_cast<void*>(CcuKfcAllReduceMesh1DMem2MemKernel);

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, channelDescs, CommTopo::COMM_TOPO_1DMESH));
        for (const auto& channel : channelDescs) {
            if (channel.channelProtocol != COMM_PROTOCOL_UBC_CTP) {
                HCCL_ERROR("[CcuTempKfcAllReduceMesh1DMem2Mem][CalcRes] channelProtocol: %u", channel.channelProtocol);
                return HCCL_E_INTERNAL;
            }
        }
    }

    auto kernelArg = std::make_shared<CcuKernelArgAllReduceKfc>();
    kernelArg->rankSize = subCommRanks_[0].size();
    kernelArg->rankId = mySubCommRank_;
    kernelArg->opParam = param;
    kernelArg->subCommRanks = subCommRanks_;
    kernelInfo.setKernelArg(kernelArg);
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    (void)param;
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    const uint64_t scratchAddr = PointerToAddr(buffInfo_.hcclBuff.addr) + buffInfo_.hcclBuffBaseOff;

    SaveSubmitInfo(
        inputAddr, outputAddr, scratchAddr, buffInfo_.inBuffBaseOff, buffInfo_.outBuffBaseOff, buffInfo_.hcclBuffBaseOff,
        templateResource);

    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch(
    const OpParam& param, const TemplateFastLaunchCtx& tempFastLaunchCtx)
{
    (void)param;
    if (tempFastLaunchCtx.ccuKernelSubmitInfos.empty()) {
        HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch] ccu kernel num is 0, just success.");
        return HCCL_SUCCESS;
    }
    HCCL_DEBUG("[CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch] start");

    uint64_t* args = const_cast<uint64_t*>(tempFastLaunchCtx.ccuKernelSubmitInfos[0].cachedArgs);
    constexpr u32 inputIdx = 0;
    constexpr u32 outputIdx = 1;
    constexpr u32 scratchIdx = 2;
    constexpr u32 inputOffsetIdx = 3;
    constexpr u32 outputOffsetIdx = 4;
    constexpr u32 scratchOffsetIdx = 5;

    args[inputIdx] = PointerToAddr(tempFastLaunchCtx.buffInfo.inputPtr) + args[inputOffsetIdx];
    args[outputIdx] = PointerToAddr(tempFastLaunchCtx.buffInfo.outputPtr) + args[outputOffsetIdx];
    args[scratchIdx] = PointerToAddr(tempFastLaunchCtx.buffInfo.hcclBuff.addr) + args[scratchOffsetIdx];

    void* taskArgs = reinterpret_cast<void*>(args);
    CcuResult launchRet = HcommCcuKernelLaunch(
        tempFastLaunchCtx.threads[0], tempFastLaunchCtx.ccuKernelSubmitInfos[0].kernelHandle, taskArgs, 6);
    if (launchRet != CCU_SUCCESS) {
        HCCL_ERROR("[CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch] kernel launch failed, ccuRet -> %d", launchRet);
        return ConvertCcuToHccl(launchRet);
    }

    HCCL_DEBUG("[CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch] end");
    return HcclResult::HCCL_SUCCESS;
}

void CcuTempKfcAllReduceMesh1DMem2Mem::SaveSubmitInfo(
    const uint64_t inputAddr, const uint64_t outputAddr, const uint64_t scratchAddr, const uint64_t inputOffset,
    const uint64_t outputOffset, const uint64_t scratchOffset, TemplateResource& templateResource) const
{
    CcuKernelSubmitInfo submitInfo;
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    (void)FillCachedArgs(
        submitInfo, inputAddr, outputAddr, scratchAddr, inputOffset, outputOffset, scratchOffset);
    templateResource.submitInfos.push_back(submitInfo);
}

u64 CcuTempKfcAllReduceMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    // AllReduce partitions scratch into rankSize slices for the reduce-scatter step.
    (void)inBuffType;
    (void)outBuffType;
    return templateRankSize_;
}

} // namespace mc2_ops_hccl
