/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <algorithm>
#include "ins_temp_all_gather_mesh_1D_Z_axis_detour.h"
#include "alg_data_trans_wrapper.h"
#include "template_utils.h"

namespace mc2_ops_hccl {
bool InsTempAllGatherMesh1D1DZAxisDetour::isNew;
InsTempAllGatherMesh1D1DZAxisDetour::InsTempAllGatherMesh1D1DZAxisDetour(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : InsTempAllGatherMesh1D(param, rankId, subCommRanks)
{
    if (param.opConfig.multipleDimensionSplitRatioSource != MultipleDimensionSplitRatioSource::BUILTIN_FORMULA) {
        configSplitRatio_ = param.opConfig.multipleDimensionSplitRatio;
        configSplitRatioSet_ = true;
    }
}
InsTempAllGatherMesh1D1DZAxisDetour::~InsTempAllGatherMesh1D1DZAxisDetour() {}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour][CalcRes] start");
    std::vector<HcclChannelDesc> level0Channels;
    CHK_RET(CalcChannelRequestMesh1DLevel0(comm, param, topoInfo, subCommRanks_, level0Channels));
    std::vector<HcclChannelDesc> level1Channels;
    CHK_RET(CalcChannelRequestMesh1DLevel1(comm, param, topoInfo, subCommRanks_, level1Channels));
    level0ChannelNumPerRank_ = level0Channels.empty() ? 0 : CalcChannelsPerRank(level0Channels);
    level1ChannelNumPerRank_ = level1Channels.empty() ? 0 : CalcChannelsPerRank(level1Channels);
    channelsPerRank_ = level0ChannelNumPerRank_ + level1ChannelNumPerRank_;
    std::vector<HcclChannelDesc> mergedChannels;
    HCCL_INFO("level0Channels[%d]level1Channels[%d]\n", level0Channels.size(), level1Channels.size());
    mergedChannels.insert(mergedChannels.end(), level0Channels.begin(), level0Channels.end());
    mergedChannels.insert(mergedChannels.end(), level1Channels.begin(), level1Channels.end());
    resourceRequest.channels.push_back(mergedChannels);
    HCCL_INFO("mergedChannels[%d]\n", mergedChannels.size());
    channelsSize = mergedChannels.size();

    if (subCommRanks_.size() <= COMM_LEVEL0) {
        return HCCL_E_PARA;
    }
    GetRes(resourceRequest);
    return HCCL_SUCCESS;
}
HcclResult InsTempAllGatherMesh1D1DZAxisDetour::GetRes(AlgResourceRequest& resourceRequest) const
{
    u32 threadNum = templateRankSize_ > 1 ? ((templateRankSize_ - 1) * channelsPerRank_) : 1;
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour][GetRes] threadNum[%u]", threadNum);
    resourceRequest.slaveThreadNum = threadNum - 1;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
    resourceRequest.notifyNumOnMainThread = threadNum - 1;
    return HCCL_SUCCESS;
}

u64 InsTempAllGatherMesh1D1DZAxisDetour::GetThreadNum() const
{
    u32 threadNum = templateRankSize_ > 1 ? ((templateRankSize_ - 1) * channelsPerRank_) : 1;
    HCCL_INFO(
        "[InsTempAllGatherMesh1D1DZAxisDetour][GetThreadNum] templateRankSize_[%u] channelsPerRank_[%u] threadNum[%u]",
        templateRankSize_, channelsPerRank_, threadNum);
    return threadNum;
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::CalcDataSplitByPortGroup(
    const u64 totalDataCount, const u64 dataTypeSize, const std::vector<ChannelInfo>& channels,
    std::vector<u64>& elemCountOut, std::vector<u64>& sizeOut, std::vector<u64>& elemOffset)
{
    HCCL_INFO(
        "[InsTempAllGatherMesh1D1DZAxisDetour][CalcDataSplitByPortGroup] Run Start[%u][%u][%f]\n",
        level0ChannelNumPerRank_, level1ChannelNumPerRank_, level0DataRatio_);
    return CalcDataSplitByPortGroupZAxisDetour(
        totalDataCount, dataTypeSize, channels, elemCountOut, sizeOut, elemOffset, level0ChannelNumPerRank_,
        level1ChannelNumPerRank_, level0DataRatio_);
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::SetchannelsPerRank(
    const std::map<u32, std::vector<ChannelInfo>>& channels)
{
    CHK_PRT_RET(channels.empty(), HCCL_ERROR("[SetchannelsPerRank] channels is empty."), HCCL_E_INTERNAL);
    channelsPerRank_ = CalcChannelsPerRank(channels);
    if (channelsPerRank_ > 1) {
        level0ChannelNumPerRank_ = MESH_CHANNELS_NUM;
        level1ChannelNumPerRank_ = channelsPerRank_ - level0ChannelNumPerRank_;
        level0DataRatio_ = (templateRankSize_ == 2) ? 0.25f : 0.5f;
        if (configSplitRatioSet_) {
            if (configSplitRatio_ >= 0.0 && configSplitRatio_ <= 1.0) {
                level0DataRatio_ = static_cast<float>(configSplitRatio_);
            } else {
                HCCL_WARNING(
                    "[InsTempAllGatherMesh1D1DZAxisDetour][SetchannelsPerRank] configured ratio[%f] is out of "
                    "range[0, 1], use built-in ratio[%.2f].",
                    configSplitRatio_, level0DataRatio_);
            }
        }
    }
    HCCL_INFO(
        "[InsTempAllGatherMesh1D1DZAxisDetour][SetchannelsPerRank], channelsPerRank_[%u], "
        "level0ChannelNumPerRank_[%u], level1ChannelNumPerRank_[%u], level0DataRatio_[%.2f]",
        channelsPerRank_, level0ChannelNumPerRank_, level1ChannelNumPerRank_, level0DataRatio_);
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::RunAllGatherMeshOnChannel(
    const ThreadHandle& thread, const ChannelInfo& linkRemote, const u32 connectedRank, const u32 connectedAlgRank,
    const u32 channelIdx)
{
    void* remoteCclBuffAddr = linkRemote.remoteCclMem.addr;
    std::vector<DataSlice> rxSrcSlicesAll;
    std::vector<DataSlice> rxDstSlicesAll;

    for (u32 rpt = 0; rpt < tempAlgParams_.repeatNum; ++rpt) {
        const u64 outBaseOff = tempAlgParams_.buffInfo.outBuffBaseOff + rpt * tempAlgParams_.outputRepeatStride;
        const u64 scratchRepeatStride = tempAlgParams_.sliceSize * templateRankSize_;
        u64 scratchBase = tempAlgParams_.buffInfo.hcclBuffBaseOff + rpt * scratchRepeatStride;
        if (tempAlgParams_.buffInfo.inBuffType == BufferType::HCCL_BUFFER) {
            scratchBase = tempAlgParams_.buffInfo.hcclBuffBaseOff + rpt * tempAlgParams_.inputRepeatStride;
        }
        const u64 rxOutOffset =
            tempAlgParams_.outputSliceStride * connectedAlgRank + outBaseOff + elemOffset_[channelIdx];
        const u64 rxScratchOffset =
            scratchBase + tempAlgParams_.inputSliceStride * connectedAlgRank + elemOffset_[channelIdx];
        void* rxSrcPtr = (!enableRemoteMemAccess_) ? remoteCclBuffAddr : linkRemote.remoteOutputGraphMode.addr;
        rxSrcSlicesAll.emplace_back(rxSrcPtr, rxScratchOffset, sizeOut_[channelIdx], elemCountOut_[channelIdx]);
        rxDstSlicesAll.emplace_back(
            tempAlgParams_.buffInfo.outputPtr, rxOutOffset, sizeOut_[channelIdx], elemCountOut_[channelIdx]);
        HCCL_DEBUG(
            "[InsTempAllGatherMesh1D1DZAxisDetour][RunAllGatherMesh] rankId [%d] connectedRank [%d] rpt [%d] "
            "channelIdx [%u] rxSrcOffset[%llu] rxDstOffset[%llu] sliceSize[%llu] count[%llu].",
            myRank_, connectedRank, rpt, channelIdx, rxScratchOffset, rxOutOffset, sizeOut_[channelIdx],
            elemCountOut_[channelIdx]);
    }

    const std::vector<DataSlice> emptySlices;
    TxRxSlicesList sendRecvSlicesList({emptySlices, emptySlices}, {rxSrcSlicesAll, rxDstSlicesAll});
    TxRxChannels sendRecvChannels(linkRemote, linkRemote);
    SendRecvInfo sendRecvInfo(sendRecvChannels, sendRecvSlicesList);
    CHK_PRT_RET(
        SendRecvBatchRead(sendRecvInfo, thread),
        HCCL_ERROR("[InsTempAllGatherMesh1D1DZAxisDetour] RunAllGatherMesh read failed"), HcclResult::HCCL_E_INTERNAL);
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::LocalDataCopy(const std::vector<ThreadHandle>& threads)
{
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour] LocalDataCopy.");
    if (threads.empty()) {
        return HcclResult::HCCL_E_INTERNAL;
    }

    u32 myAlgRank;
    CHK_RET(GetAlgRank(myRank_, subCommRanks_[0], myAlgRank));
    const u32 dataTypeSize = DATATYPE_SIZE_TABLE[dataType_];
    u64 sliceSize = tempAlgParams_.sliceSize;
    if (tempAlgParams_.tailSize != 0 && myAlgRank == templateRankSize_ - 1) {
        sliceSize = tempAlgParams_.tailSize;
    }
    u64 sliceCount = sliceSize / dataTypeSize;
    const u32 subThreadNum = static_cast<u32>(threads.size()) - 1;
    for (u32 rpt = 0; rpt < tempAlgParams_.repeatNum; ++rpt) {
        const u64 inBaseOff = tempAlgParams_.buffInfo.inBuffBaseOff + rpt * tempAlgParams_.inputRepeatStride;
        const u64 outBaseOff = tempAlgParams_.buffInfo.outBuffBaseOff + rpt * tempAlgParams_.outputRepeatStride;
        const u64 inOff = tempAlgParams_.inputSliceStride * myAlgRank + inBaseOff;
        const u64 outOff = tempAlgParams_.outputSliceStride * myAlgRank + outBaseOff;

        DataSlice srcSlice(tempAlgParams_.buffInfo.inputPtr, inOff, sliceSize, sliceCount);
        bool skipOutCopy = (tempAlgParams_.buffInfo.inputPtr == tempAlgParams_.buffInfo.outputPtr && inOff == outOff);
        if (!skipOutCopy) {
            DataSlice dstSlice(tempAlgParams_.buffInfo.outputPtr, outOff, sliceSize, sliceCount);
            const u32 copyThreadIdx = (subThreadNum > 0) ? (1 + rpt % subThreadNum) : 0;
            HCCL_DEBUG(
                "[InsTempAllGatherMesh1D1DZAxisDetour][LocalDataCopy] RankID [%d] AlgRank [%d] srcSlice: "
                "inBaseOff[%llu] inOff[%llu] "
                "sliceSize[%llu] count[%llu].",
                myRank_, myAlgRank, inBaseOff, inOff, sliceSize, sliceCount);
            HCCL_DEBUG(
                "[InsTempAllGatherMesh1D1DZAxisDetour][LocalDataCopy] RankID [%d] AlgRank [%d] dstSlice: "
                "outBaseoff[%llu] "
                "outOff[%llu] sliceSize[%llu] count[%llu] copyThreadIdx[%u].",
                myRank_, myAlgRank, outBaseOff, outOff, sliceSize, sliceCount, copyThreadIdx);
            LocalCopy(threads[copyThreadIdx], srcSlice, dstSlice);
        }
    }
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::KernelRun(
    const OpParam& param, const TemplateDataParams& tempAlgParams, const TemplateResource& templateResource)
{
    enableRemoteMemAccess_ = tempAlgParams.enableRemoteMemAccess;
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour] Run start");
    if (tempAlgParams.sliceSize == 0 && tempAlgParams.tailSize == 0) {
        HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour] Rank [%d], get slicesize zero.", myRank_);
        return HCCL_SUCCESS;
    }
    threadNum_ = templateResource.threads.size();
    tempAlgParams_ = tempAlgParams;
    dataType_ = param.DataDes.dataType;
    HCCL_DEBUG("[InsTempAllGatherMesh1D1DZAxisDetour] Rank [%d], get threadNum_[%d].", myRank_, threadNum_);
    CHK_RET(LocalDataCopy(templateResource.threads));
    if (templateRankSize_ == 1) {
        return HcclResult::HCCL_SUCCESS;
    }
    if (threadNum_ > 1) {
        std::vector<ThreadHandle> subThreads(templateResource.threads.begin() + 1, templateResource.threads.end());
        GetNotifyIdxMainToSub(notifyIdxMainToSub_);
        CHK_RET(PreSyncInterThreads(templateResource.threads[0], subThreads, notifyIdxMainToSub_));
    }

    CHK_RET(RunAllGatherMesh(templateResource.threads, templateResource.channels));

    if (threadNum_ > 1) {
        std::vector<ThreadHandle> subThreads(templateResource.threads.begin() + 1, templateResource.threads.end());
        GetNotifyIdxSubToMain(notifyIdxSubToMain_);
        CHK_RET(PostSyncInterThreads(templateResource.threads[0], subThreads, notifyIdxSubToMain_));
    }
    if (opMode_ == OpMode::OPBASE) {
        CHK_RET(PostLocalCopy(templateResource.threads));
    }
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour] Run End");
    return HcclResult::HCCL_SUCCESS;
}

HcclResult InsTempAllGatherMesh1D1DZAxisDetour::RunAllGatherMesh(
    const std::vector<ThreadHandle>& threads, const std::map<u32, std::vector<ChannelInfo>>& channels)
{
    HCCL_INFO("[InsTempAllGatherMesh1D1DZAxisDetour] RunAllGatherMesh RankIDs[%d].", myRank_);
    u32 myAlgRank = 0;
    CHK_RET(GetAlgRank(myRank_, subCommRanks_[0], myAlgRank));
    const u32 dataTypeSize = DATATYPE_SIZE_TABLE[dataType_];
    u32 queIdx = 0;
    for (u32 rankIdx = 0; rankIdx < subCommRanks_[0].size(); rankIdx++) {
        u32 connectedRank = subCommRanks_[0][rankIdx];
        if (connectedRank == myRank_) {
            continue;
        }
        u32 connectedAlgRank = 0;
        CHK_RET(GetAlgRank(connectedRank, subCommRanks_[0], connectedAlgRank));
        HCCL_INFO(
            "[InsTempAllGatherMesh1D1DZAxisDetour] RunAllGatherMesh RankIDs[%d], connectedRank[%d], "
            "connectedAlgRank[%d].",
            myRank_, connectedRank, connectedAlgRank);
        CHK_PRT_RET(
            channels.count(connectedRank) == 0 || channels.at(connectedRank).empty(),
            HCCL_ERROR(
                "[InsTempAllGatherMesh1D1DZAxisDetour][RankID]=%u connectedRank=%d, channels.size=%zu", myRank_,
                connectedRank, channels.size()),
            HcclResult::HCCL_E_INTERNAL);
        const std::vector<ChannelInfo>& curChannels = channels.at(connectedRank);
        u64 sliceSize = tempAlgParams_.sliceSize;
        if (tempAlgParams_.tailSize != 0 && connectedAlgRank == templateRankSize_ - 1) {
            sliceSize = tempAlgParams_.tailSize;
        }
        u64 sliceCount = sliceSize / dataTypeSize;
        elemCountOut_.clear();
        sizeOut_.clear();
        elemOffset_.clear();
        CHK_RET(CalcDataSplitByPortGroup(sliceCount, dataTypeSize, curChannels, elemCountOut_, sizeOut_, elemOffset_));
        const u32 curChannelNum = std::min(
            std::min(static_cast<u32>(curChannels.size()), channelsPerRank_),
            static_cast<u32>(elemOffset_.size()));
        for (u32 channelIdx = 0; channelIdx < curChannelNum; channelIdx++) {
            CHK_PRT_RET(
                queIdx >= threads.size(),
                HCCL_ERROR(
                    "[InsTempAllGatherMesh1D1DZAxisDetour][RankID]=%u queIdx=%u, threads.size=%zu", myRank_, queIdx,
                    threads.size()),
                HcclResult::HCCL_E_INTERNAL);
            CHK_RET(RunAllGatherMeshOnChannel(
                threads[queIdx], curChannels[channelIdx], connectedRank, connectedAlgRank, channelIdx));
            queIdx++;
        }
    }
    return HcclResult::HCCL_SUCCESS;
}

} // namespace mc2_ops_hccl
