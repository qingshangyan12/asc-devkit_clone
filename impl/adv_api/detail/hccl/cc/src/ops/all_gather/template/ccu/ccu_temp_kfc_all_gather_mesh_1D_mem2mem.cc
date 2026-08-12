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
#include "ccu_temp_kfc_all_gather_mesh_1D_mem2mem.h"

namespace mc2_ops_hccl {

CcuTempKfcAllGatherMesh1DMem2Mem::CcuTempKfcAllGatherMesh1DMem2Mem(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{}

CcuTempKfcAllGatherMesh1DMem2Mem::~CcuTempKfcAllGatherMesh1DMem2Mem() {}

HcclResult CcuTempKfcAllGatherMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    CHK_SAFETY_FUNC_RET(
        strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelAllGatherMesh1DMem2Mem"));

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, channelDescs, CommTopo::COMM_TOPO_1DMESH));
        for (const auto& channel : channelDescs) {
            if (channel.channelProtocol != COMM_PROTOCOL_UBC_CTP) {
                HCCL_ERROR("[CcuTempKfcAllGatherMesh1DMem2Mem][CalcRes] channelProtocol: %u", channel.channelProtocol);
                return HCCL_E_INTERNAL;
            }
        }
    }

    // CcuKernelKfcServer replaces this placeholder before registration and reuses these channels.
    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllGatherMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    buffInfo_ = templateDataParams.buffInfo;

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));

    const uint64_t outputOffset = templateDataParams.outputSliceStride * mySubCommRank_;
    uint64_t sliceSize = templateDataParams.sliceSize;
    uint64_t lastSliceSize = templateDataParams.tailSize;
    if (templateDataParams.tailSize != 0 && mySubCommRank_ == templateRankSize_ - 1) {
        sliceSize = templateDataParams.tailSize;
        lastSliceSize = templateDataParams.tailSize;
    }
    if (sliceSize == 0) {
        return HcclResult::HCCL_SUCCESS;
    }

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_MS_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE * LOCAL_COPY_MS_PER_LOOP;
    const auto goSize = CalGoSize(sliceSize, config);

    // 3 阶段 unroll 高性能版 DoRepeatAllGather 所需参数（与 KFC server 分发保持一致）
    const uint64_t currentRankSliceInputOffset = templateDataParams.inputSliceStride * mySubCommRank_;
    const uint64_t tmpRepeatNum = UINT64_MAX - templateDataParams.repeatNum;
    const uint64_t inputRepeatStride = templateDataParams.inputRepeatStride;
    const uint64_t outputRepeatStride = templateDataParams.outputRepeatStride;
    const uint64_t isInputOutputEqual = (inputAddr == outputAddr) ? 1 : 0;

    HCCL_INFO("[CcuTempKfcAllGather][KernelRun] rank[%u] rankSize[%u] inputAddr[0x%llx] outputAddr[0x%llx] "
        "inBuffBaseOff[%llu] outBuffBaseOff[%llu] outputSliceStride[%llu] mySubCommRank[%u] "
        "outputOffset[%llu] sliceSize[%llu] tailSize[%llu] lastSliceSize[%llu] "
        "inputSliceStride[%llu] currentRankSliceInputOffset[%llu] "
        "repeatNum[%llu] tmpRepeatNum[0x%llx] inputRepeatStride[%llu] outputRepeatStride[%llu] "
        "isInputOutputEqual[%llu] goSize[%llu,%llu,%llu,%llu]",
        mySubCommRank_, templateRankSize_, inputAddr, outputAddr,
        buffInfo_.inBuffBaseOff, buffInfo_.outBuffBaseOff,
        templateDataParams.outputSliceStride, mySubCommRank_,
        outputOffset, sliceSize, templateDataParams.tailSize, lastSliceSize,
        templateDataParams.inputSliceStride, currentRankSliceInputOffset,
        templateDataParams.repeatNum, tmpRepeatNum, inputRepeatStride, outputRepeatStride,
        isInputOutputEqual, goSize[0], goSize[1], goSize[2], goSize[3]);

    // KFC server 的 param[0] 是控制字（非 INVALID_U64 表示有效），param[1..14] 是实际参数。
    // token 由 KFC server 从 ctx.token 读取，不写入 param。参数顺序需与 DispatchKfcSubKernel 一致。
    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(submitInfo, 1ULL, inputAddr, outputAddr, outputOffset, sliceSize, goSize[0], goSize[1],
        goSize[2], goSize[3], currentRankSliceInputOffset, tmpRepeatNum, inputRepeatStride, outputRepeatStride,
        lastSliceSize, isInputOutputEqual));
    templateResource.submitInfos.push_back(submitInfo);
    return HcclResult::HCCL_SUCCESS;
}

u64 CcuTempKfcAllGatherMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
