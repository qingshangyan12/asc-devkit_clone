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

CcuTempKfcServer::CcuTempKfcServer(const OpParam& param, const u32 rankId,
                                   const std::vector<std::vector<u32>> &subCommRanks)
: CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    tempRankSize_ = subCommRanks[0].size();
    auto it = std::find(subCommRanks[0].begin(), subCommRanks[0].end(), rankId);
    if (it != subCommRanks[0].end()) {
        mySubCommRank_ = std::distance(subCommRanks[0].begin(), it);
    }
}

CcuTempKfcServer::~CcuTempKfcServer()
{
}

HcclResult CcuTempKfcServer::CalcRes(HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
                                     AlgResourceRequest& resourceRequest)
{
    HCCL_INFO("[CcuTempKfcServer::CalcRes start]");
    // 不需要从流
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.slaveThreadNum = 0;
    // kernel数量
    resourceRequest.ccuKernelNum.push_back(1);
    HCCL_DEBUG("[CcuTempKfcServer::CalcRes] notifyNumOnMainThread[%u] slaveThreadNum[%u]",
               resourceRequest.notifyNumOnMainThread, resourceRequest.slaveThreadNum);

    CcuKernelInfo kernelInfo;
    strcpy(kernelInfo.kernelFuncName, "CcuKernelKfcServer");
    kernelInfo.kernelFunc = reinterpret_cast<void *>(CcuKfcServerKernel);

    // KfcServer 内联录制 AlltoAllMesh1D，需要为其申请远端 channel
    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));

    auto kernelArg = std::make_shared<CcuKernelArgKfcServer>();
    // arg 经 host 旁路在录制期交给 AlltoAllMesh1DFunc 使用，字段须与
    // CcuKernelArgAlltoAllMesh1D 对齐（两结构体布局一致）
    kernelArg->rankSize = subCommRanks_[0].size();
    kernelArg->rankId = mySubCommRank_;
    kernelArg->loadFromMem = param.isMc2;
    kernelArg->opParam = param;
    kernelArg->subCommRanks = subCommRanks_;
    kernelInfo.setKernelArg(kernelArg);
    kernelInfo.channels = channelDescs;

    resourceRequest.ccuKernelInfos.push_back(kernelInfo);

    HCCL_DEBUG("[CcuTempKfcServer::CalcRes] dimsize=%llu, ccuKernelInfos.size()=%llu",
               subCommRanks_[0].size(), resourceRequest.ccuKernelInfos.size());

    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcServer::KernelRun(const OpParam& param,
                                       const TemplateDataParams& templateDataParams,
                                       TemplateResource& templateResource)
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
 
    std::unique_ptr<hcomm::CcuTaskArg> taskArg = std::make_unique<CcuTaskArgKfcServer>(
        xnAddr, ckeAddr, dieNum, missionNum, missionIndex, token);
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

}
