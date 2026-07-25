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
#include "alg_data_trans_wrapper.h"
#include "kernel/ccu_kernel_all_to_all_v_mesh1d.h"
#include "ccu_temp_all_to_all_v_mesh_1D.h"
#include "ccu_launch_dl.h"

namespace mc2_ops_hccl {

CcuTempAlltoAllVMesh1D::CcuTempAlltoAllVMesh1D(const OpParam& param, const u32 rankId,
                                       const std::vector<std::vector<u32>> &subCommRanks)
: CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    tempRankSize_ = subCommRanks[0].size();
    auto it = std::find(subCommRanks[0].begin(), subCommRanks[0].end(), rankId);
    if (it != subCommRanks[0].end()) {
        mySubCommRank_ = std::distance(subCommRanks[0].begin(), it);
    }
}

CcuTempAlltoAllVMesh1D::~CcuTempAlltoAllVMesh1D()
{
}

HcclResult CcuTempAlltoAllVMesh1D::CalcRes(HcclComm comm, const OpParam& param,
                                           const TopoInfoWithNetLayerDetails* topoInfo,
                                           AlgResourceRequest& resourceRequest)
{
    // 不需要从流
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.slaveThreadNum = 0;
    // kernel 数量
    resourceRequest.ccuKernelNum.push_back(1);
    HCCL_DEBUG("[CcuTempAlltoAllVMesh1D::CalcRes] notifyNumOnMainThread[%u] slaveThreadNum[%u]",
               resourceRequest.notifyNumOnMainThread, resourceRequest.slaveThreadNum);

    CcuKernelInfo kernelInfo;
    strcpy(kernelInfo.kernelFuncName, "CcuKernelAlltoAllVMesh1D");
    kernelInfo.kernelFunc = reinterpret_cast<void *>(CcuAlltoAllVMesh1DKernel);

    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));

    auto kernelArg = std::make_shared<CcuKernelArgAlltoAllVMesh1D>();
    kernelArg->rankSize = subCommRanks_[0].size();
    kernelArg->rankId = mySubCommRank_;
    kernelArg->loadFromMem = param.isMc2;
    kernelArg->opParam = param;
    kernelArg->subCommRanks = subCommRanks_;
    kernelInfo.setKernelArg(kernelArg);
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);

    HCCL_DEBUG("[CcuTempAlltoAllVMesh1D::CalcRes] channelDescs.size()=%llu, dimsize=%llu, "
               "ccuKernelInfos.size()=%llu",
               channelDescs.size(), subCommRanks_[0].size(), resourceRequest.ccuKernelInfos.size());

    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempAlltoAllVMesh1D::KernelRun(const OpParam& param,
                                             const TemplateDataParams& templateDataParams,
                                             TemplateResource& templateResource)
{
    HCCL_INFO("[CcuTempAlltoAllVMesh1D] KernelRun");
    buffInfo_ = templateDataParams.buffInfo;

    uint64_t sliceSize = templateDataParams.sliceSize;

    // 单 P 场景直通（与定长模板一致）
    if (tempRankSize_ == 1) {
        DataSlice usrInSlice = DataSlice(buffInfo_.inputPtr, buffInfo_.inBuffBaseOff, sliceSize);
        DataSlice usrOutSlice = DataSlice(buffInfo_.outputPtr, buffInfo_.outBuffBaseOff, sliceSize);
        LocalCopy(templateResource.threads[0], usrInSlice, usrOutSlice);
        HCCL_INFO("[CcuTempAlltoAllVMesh1D::KernelRun] rankSize==1, LocalCopy done.");
        return HcclResult::HCCL_SUCCESS;
    }

    HCCL_DEBUG("[CcuTempAlltoAllVMesh1D::KernelRun] end");
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempAlltoAllVMesh1D::FastLaunch(const OpParam& param, const TemplateFastLaunchCtx& tempFastLaunchCtx)
{
    if (tempFastLaunchCtx.ccuKernelSubmitInfos.size() == 0) {
        HCCL_INFO("[CcuTempAlltoAllVMesh1D::FastLaunch] ccu kernel num is 0, just success.");
        return HCCL_SUCCESS;
    }
    HCCL_INFO("[CcuTempAlltoAllVMesh1D::FastLaunch] MC2 path, params prepared by device ApiClient, nothing to relaunch.");
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempAlltoAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return tempRankSize_;
}

} // namespace mc2_ops_hccl
