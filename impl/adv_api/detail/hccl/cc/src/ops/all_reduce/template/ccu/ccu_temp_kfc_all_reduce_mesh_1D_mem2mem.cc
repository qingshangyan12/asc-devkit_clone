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
#include "ccu_kernel_utils.h"
#include "ccu_temp_kfc_all_reduce_mesh_1D_mem2mem.h"

namespace mc2_ops_hccl {

CcuTempKfcAllReduceMesh1DMem2Mem::CcuTempKfcAllReduceMesh1DMem2Mem(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuTempKfcMesh1DMem2MemBase(param, rankId, subCommRanks)
{
    reduceOp_ = param.reduceType;
    dataType_ = param.DataDes.dataType;
}

CcuTempKfcAllReduceMesh1DMem2Mem::~CcuTempKfcAllReduceMesh1DMem2Mem() {}

uint64_t CcuTempKfcAllReduceMesh1DMem2Mem::RoundUp(uint64_t dividend, uint64_t divisor) const
{
    return dividend / divisor + ((dividend % divisor != 0) ? 1 : 0);
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::CalcSlice(const u64 dataSize, RankSliceInfo& sliceInfoVec)
{
    std::vector<SliceInfo> tmp(subCommRanks_.size());
    sliceInfoVec.resize(templateRankSize_, tmp);

    u64 unitAllignSize = DataTypeSizeGet(dataType_);
    u64 chunkSize = RoundUp(dataSize, (templateRankSize_ * unitAllignSize)) * unitAllignSize;
    HCCL_INFO(
        "[CcuTempKfcAllReduceMesh1DMem2Mem] chunkSize[%llu], dataSize[%llu], templateRankSize_[%u], "
        "unitAllignSize[%llu]",
        chunkSize, dataSize, templateRankSize_, unitAllignSize);
    u64 accumOff = 0;
    for (u32 rankIdx = 0; rankIdx < templateRankSize_; rankIdx++) {
        u64 currChunkSize = ((dataSize - accumOff) > chunkSize) ? chunkSize : (dataSize - accumOff);
        SliceInfo slice = {accumOff, currChunkSize};
        sliceInfoVec[rankIdx][0] = slice;
        accumOff += currChunkSize;
    }

    CHK_PRT_RET(
        (sliceInfoVec[templateRankSize_ - 1][0].offset + sliceInfoVec[templateRankSize_ - 1][0].size != dataSize),
        HCCL_ERROR(
            "[CcuTempKfcAllReduceMesh1DMem2Mem] chunkSize:[%llu], Rank:[%d], SliceInfo calculation error!", chunkSize,
            myRank_),
        HcclResult::HCCL_E_INTERNAL);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes] start, templateRankSize=%u, mySubCommRank=%u",
              templateRankSize_, mySubCommRank_);
    CHK_RET(GetRes(resourceRequest));
    resourceRequest.ccuKernelNum.push_back(1);

    CcuKernelInfo kernelInfo;
    strcpy_s(kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuAllReduceMesh1DMem2Mem");

    std::vector<HcclChannelDesc> channelDescs;
    if (topoInfo->level0Topo != Level0Shape::MESH_1D_CLOS) {
        CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channelDescs));
    } else {
        std::vector<HcclChannelDesc> myChannelDescs;
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks_, myChannelDescs, CommTopo::COMM_TOPO_1DMESH));
        for (const auto& channel : myChannelDescs) {
            if (channel.channelProtocol == COMM_PROTOCOL_UBC_CTP) {
                channelDescs.push_back(channel);
            }
        }
    }
    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes] channelDescs.size=%zu", channelDescs.size());

    kernelInfo.channels = channelDescs;
    resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::CalcRes] done, ccuKernelInfos.size=%zu",
              resourceRequest.ccuKernelInfos.size());
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    (void)param;
    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun] start, sliceSize=%lu, tailSize=%lu",
              templateDataParams.sliceSize, templateDataParams.tailSize);
    buffInfo_ = templateDataParams.buffInfo;

    if (templateDataParams.sliceSize == 0 && templateDataParams.tailSize == 0) {
        HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem] sliceSize is 0, no need to do, just success.");
        return HCCL_SUCCESS;
    }

    RankSliceInfo sliceInfoVec;
    CHK_RET(CalcSlice(templateDataParams.sliceSize, sliceInfoVec));

    const uint64_t inputAddr = PointerToAddr(buffInfo_.inputPtr) + buffInfo_.inBuffBaseOff;
    const uint64_t outputAddr = PointerToAddr(buffInfo_.outputPtr) + buffInfo_.outBuffBaseOff;
    uint64_t token = 0;
    CHK_RET(GetToken(buffInfo_, token));
    const uint64_t scratchAddr = PointerToAddr(buffInfo_.hcclBuff.addr) + buffInfo_.hcclBuffBaseOff;
    const uint64_t isInputOutputEqual = (inputAddr == outputAddr) ? 1 : 0;

    const uint64_t normalSliceSize = sliceInfoVec[0][0].size;
    const uint64_t lastSliceSize = sliceInfoVec[templateRankSize_ - 1][0].size;
    const uint64_t mySliceSize = sliceInfoVec[mySubCommRank_][0].size;
    const uint64_t currentRankSliceInputOffset = 0;
    const uint64_t currentRankSliceOutputOffset = 0;
    const uint64_t sliceOffset = normalSliceSize * mySubCommRank_;

    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun] inputAddr=0x%llx, outputAddr=0x%llx, scratchAddr=0x%llx, "
              "normalSliceSize=%llu, lastSliceSize=%llu, mySliceSize=%llu, sliceOffset=%llu, mySubCommRank=%u",
              inputAddr, outputAddr, scratchAddr, normalSliceSize, lastSliceSize, mySliceSize, sliceOffset, mySubCommRank_);

    LoopGroupConfig config{};
    config.msInterleave = CCU_MS_INTERLEAVE;
    config.loopCount = CCU_M2M_LOCAL_COPY_LOOP_COUNT;
    config.memSlice = CCU_MS_SIZE;
    const std::vector<uint64_t> goSize = (mySubCommRank_ != templateRankSize_ - 1) ?
                                             CalGoSize(normalSliceSize, config) :
                                             CalGoSize(lastSliceSize, config);

    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun] goSize: [0]=%llu, [1]=%llu, [2]=%llu, [3]=%llu",
              goSize[0], goSize[1], goSize[2], goSize[3]);

    CcuKernelSubmitInfo submitInfo{};
    submitInfo.kernelHandle = templateResource.ccuKernels[0];
    CHK_RET(FillCachedArgs(
        submitInfo, inputAddr, outputAddr, token, scratchAddr, currentRankSliceInputOffset,
        currentRankSliceOutputOffset, normalSliceSize, lastSliceSize, mySliceSize, sliceOffset, isInputOutputEqual,
        goSize[0], goSize[1], goSize[2], goSize[3], buffInfo_.inBuffBaseOff, buffInfo_.outBuffBaseOff,
        buffInfo_.hcclBuffBaseOff));
    templateResource.submitInfos.push_back(submitInfo);

    HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::KernelRun] done, submitInfos.size=%zu",
              templateResource.submitInfos.size());
    return HcclResult::HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch(
    const OpParam& param, const TemplateFastLaunchCtx& tempFastLaunchCtx)
{
    (void)param;
    if (tempFastLaunchCtx.ccuKernelSubmitInfos.size() == 0) {
        HCCL_INFO("[CcuTempKfcAllReduceMesh1DMem2Mem::FastLaunch] ccu kernel num is 0, just success.");
        return HCCL_SUCCESS;
    }
    return HCCL_SUCCESS;
}

u64 CcuTempKfcAllReduceMesh1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return templateRankSize_;
}

} // namespace mc2_ops_hccl
