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

CcuTempKfcHalfAllToAllVMesh1D::CcuTempKfcHalfAllToAllVMesh1D(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcHalfAllToAllVMesh1D::~CcuTempKfcHalfAllToAllVMesh1D() {}

HcclResult CcuTempKfcHalfAllToAllVMesh1D::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelKfcHalfAll2AllVMesh1D");

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, channelDescs, CommTopo::COMM_TOPO_1DMESH));
    }

    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcHalfAllToAllVMesh1D::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t sendSizesAddr = PointerToAddr(param.all2AllVDataDes.sendCounts);
    const uint64_t sendOffsetsAddr = PointerToAddr(param.all2AllVDataDes.sdispls);
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    const uint64_t recvOffset = mySubCommRank_ * templateDataParams.sliceSize;

    uint64_t sliceSize = templateDataParams.sliceSize;
    if (sliceSize == 0) {
        return HcclResult::HCCL_SUCCESS;
    }

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(sliceSize, config);

    HCCL_INFO(
        "[CcuTempKfcHalfAllToAllVMesh1D][KernelRun] inputAddr[0x%llx], sendSizesAddr[0x%llx], "
        "sendOffsetsAddr[0x%llx], recvOffset[0x%llx], token[0x%llx], sliceSize[%llu], "
        "goSize[0x%llx, 0x%llx, 0x%llx, 0x%llx]",
        static_cast<unsigned long long>(inputAddr), static_cast<unsigned long long>(sendSizesAddr),
        static_cast<unsigned long long>(sendOffsetsAddr), static_cast<unsigned long long>(recvOffset),
        static_cast<unsigned long long>(token), static_cast<unsigned long long>(sliceSize),
        static_cast<unsigned long long>(goSize[0]), static_cast<unsigned long long>(goSize[1]),
        static_cast<unsigned long long>(goSize[2]), static_cast<unsigned long long>(goSize[3]));

    // Mission 0 — 搬运前半数据 (sendSizeA/sendOffsetA)
    CcuKernelSubmitInfo submitInfo0{};
    submitInfo0.kernelHandle = templateResource.ccuKernels[0];
    submitInfo0.cachedArgs[0] = 0ULL;
    FillCachedArgsPayload(
        submitInfo0, inputAddr, sendSizesAddr, sendOffsetsAddr, recvOffset,
        goSize[0], goSize[1], goSize[2], goSize[3]);
    templateResource.submitInfos.push_back(submitInfo0);

    // Mission 1 — 搬运后半数据 (sendSizeB/sendOffsetB)
    CcuKernelSubmitInfo submitInfo1{};
    submitInfo1.kernelHandle = templateResource.ccuKernels[0];
    submitInfo1.cachedArgs[0] = 1ULL;
    FillCachedArgsPayload(
        submitInfo1, inputAddr, sendSizesAddr, sendOffsetsAddr, recvOffset,
        goSize[0], goSize[1], goSize[2], goSize[3]);
    templateResource.submitInfos.push_back(submitInfo1);

    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcHalfAllToAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
