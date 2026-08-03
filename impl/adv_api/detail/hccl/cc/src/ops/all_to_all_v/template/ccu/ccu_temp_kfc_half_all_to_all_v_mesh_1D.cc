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
#include "ccu_kernel_alg_base.h"
#include "ccu_temp_kfc_half_all_to_all_v_mesh_1D.h"

namespace mc2_ops_hccl {

// Source-side ordering caps the supported rank count; kept aligned with the selector and sub-kernel guards.
constexpr u32 A2AV_MIN_SUPPORTED_RANK_SIZE = 2;
constexpr u32 A2AV_MAX_SUPPORTED_RANK_SIZE = 8;

CcuTempKfcHalfAllToAllVMesh1D::CcuTempKfcHalfAllToAllVMesh1D(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcHalfAllToAllVMesh1D::~CcuTempKfcHalfAllToAllVMesh1D() {}

HcclResult CcuTempKfcHalfAllToAllVMesh1D::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_PRT_RET(
        subCommRanks_.size() != 1 || templateRankSize_ < A2AV_MIN_SUPPORTED_RANK_SIZE ||
            templateRankSize_ > A2AV_MAX_SUPPORTED_RANK_SIZE,
        HCCL_ERROR(
            "[CcuTempKfcHalfAllToAllVMesh1D][CalcRes] Only 2-8 ranks in one 1D mesh are supported, "
            "subCommRanks[%zu], tempRankSize[%u].",
            subCommRanks_.size(), templateRankSize_),
        HCCL_E_NOT_SUPPORT);

    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelKfcHalfAllToAllVMesh1D");

    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    CHK_PRT_RET(
        channelDescs.size() != templateRankSize_ - 1,
        HCCL_ERROR(
            "[CcuTempKfcHalfAllToAllVMesh1D][CalcRes] Channel count[%zu] != rankSize-1[%u].", channelDescs.size(),
            templateRankSize_ - 1),
        HCCL_E_INTERNAL);

    // CcuKernelKfcServer replaces this placeholder before registration and reuses these Mesh1D channels.
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcHalfAllToAllVMesh1D::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    // Per-round parameters (sendSizes/sendOffsets/remoteWinOffset) are written by CcuPrepareForAllToAllVWrite
    // on the AI core side and consumed by KFC server through the HBM parameter block.
    (void)param;
    (void)templateDataParams;
    (void)templateResource;
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcHalfAllToAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    // Peers write straight into the CCL buffer, so no scratch partitioning is required.
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
