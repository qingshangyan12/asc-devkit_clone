/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ins_temp_all_to_all_v_mesh_1D.h"

#include <algorithm>
#include <limits>

namespace mc2_ops_hccl {
namespace {

bool MulOverflow(u64 left, u64 right, u64& result)
{
    if (left != 0 && right > std::numeric_limits<u64>::max() / left) {
        return true;
    }
    result = left * right;
    return false;
}

bool AddOverflow(u64 left, u64 right, u64& result)
{
    if (right > std::numeric_limits<u64>::max() - left) {
        return true;
    }
    result = left + right;
    return false;
}

bool CalcByteOffset(u64 displacement, u64 dataTypeSize, u64 extraOffset, u64& result)
{
    u64 displacementBytes = 0;
    if (MulOverflow(displacement, dataTypeSize, displacementBytes) ||
        extraOffset > std::numeric_limits<u64>::max() - displacementBytes) {
        return false;
    }
    result = displacementBytes + extraOffset;
    return true;
}

} // namespace

InsTempAlltoAllVMesh1D::InsTempAlltoAllVMesh1D(
    const OpParam& param, u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : InsAlgTemplateBase(param, rankId, subCommRanks)
{}

InsTempAlltoAllVMesh1D::~InsTempAlltoAllVMesh1D() {}

std::string InsTempAlltoAllVMesh1D::Describe() const
{
    return "Template of AllToAllV Mesh1D with rank size " + std::to_string(templateRankSize_);
}

HcclResult InsTempAlltoAllVMesh1D::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PRT_RET(
        subCommRanks_.size() != 1 || subCommRanks_[0].size() < 2,
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][CalcRes] only one full-mesh domain with at least two ranks is supported."),
        HCCL_E_PARA);
    CHK_PRT_RET(
        std::find(subCommRanks_[0].begin(), subCommRanks_[0].end(), myRank_) == subCommRanks_[0].end(),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][CalcRes] rank[%u] is absent from the communication domain.", myRank_),
        HCCL_E_PARA);

    std::vector<HcclChannelDesc> channels;
    CHK_RET(CalcChannelRequestMesh1D(comm, param, topoInfo, subCommRanks_, channels));
    CHK_PRT_RET(
        channels.empty(), HCCL_ERROR("[InsTempAlltoAllVMesh1D][CalcRes] no channel was requested."), HCCL_E_PARA);
    resourceRequest.channels.push_back(channels);

    channelsPerRank_ = CalcChannelsPerRank(channels);
    CHK_PRT_RET(
        channelsPerRank_ == 0, HCCL_ERROR("[InsTempAlltoAllVMesh1D][CalcRes] channels per rank is zero."), HCCL_E_PARA);
    const u32 concurrentRankNum = std::min(ALLTOALLV_DIRECT_FULLMESH_CONCURRENT_SIZE, templateRankSize_ - 1);
    CHK_PRT_RET(
        channelsPerRank_ > std::numeric_limits<u32>::max() / concurrentRankNum,
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][CalcRes] slave thread number overflows."), HCCL_E_PARA);
    resourceRequest.slaveThreadNum = concurrentRankNum * channelsPerRank_;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, channelsPerRank_);
    resourceRequest.notifyNumOnMainThread = resourceRequest.slaveThreadNum;
    return HCCL_SUCCESS;
}

u64 InsTempAlltoAllVMesh1D::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    if (templateRankSize_ < 2) {
        return 0;
    }
    concurrentSendRecvNum_ = std::min(ALLTOALLV_DIRECT_FULLMESH_CONCURRENT_SIZE, templateRankSize_ - 1);
    return concurrentSendRecvNum_;
}

HcclResult InsTempAlltoAllVMesh1D::ValidateParams(
    const TemplateDataParams& tempAlgParams, const TemplateResource& templateResource) const
{
    CHK_PRT_RET(
        subCommRanks_.size() != 1 || subCommRanks_[0].size() != templateRankSize_ || templateRankSize_ < 2,
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] invalid communication domain."), HCCL_E_PARA);
    for (u32 rank = 0; rank < templateRankSize_; ++rank) {
        CHK_PRT_RET(
            subCommRanks_[0][rank] != rank,
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] non-contiguous rank domain is unsupported."),
            HCCL_E_PARA);
    }
    CHK_PRT_RET(
        myRank_ >= templateRankSize_,
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] rank[%u] is out of range.", myRank_), HCCL_E_PARA);
    CHK_PRT_RET(
        tempAlgParams.sendCounts.size() != templateRankSize_ || tempAlgParams.recvCounts.size() != templateRankSize_ ||
            tempAlgParams.sdispls.size() != templateRankSize_ || tempAlgParams.rdispls.size() != templateRankSize_,
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] AllToAllV vector size mismatch."), HCCL_E_PARA);
    CHK_PRT_RET(
        templateResource.threads.empty(), HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] thread list is empty."),
        HCCL_E_PARA);
    CHK_PRT_RET(
        templateResource.channels.size() != templateRankSize_ - 1,
        HCCL_ERROR(
            "[InsTempAlltoAllVMesh1D][ValidateParams] channel rank count[%zu] is not expected[%u].",
            templateResource.channels.size(), templateRankSize_ - 1),
        HCCL_E_PARA);
    for (u32 rank = 0; rank < templateRankSize_; ++rank) {
        if (rank == myRank_) {
            continue;
        }
        auto channelIt = templateResource.channels.find(rank);
        CHK_PRT_RET(
            channelIt == templateResource.channels.end() || channelIt->second.empty(),
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][ValidateParams] no channel for rank[%u].", rank), HCCL_E_PARA);
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::KernelRun(
    const OpParam& param, const TemplateDataParams& tempAlgParams, const TemplateResource& templateResource)
{
    CHK_RET(ValidateParams(tempAlgParams, templateResource));
    const u32 dataTypeIndex = static_cast<u32>(param.all2AllVDataDes.sendType);
    CHK_PRT_RET(
        dataTypeIndex >= static_cast<u32>(HCCL_DATA_TYPE_RESERVED),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][KernelRun] invalid datatype[%u].", dataTypeIndex), HCCL_E_PARA);
    dataType_ = param.all2AllVDataDes.sendType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataTypeIndex];
    CHK_PRT_RET(
        dataTypeSize_ == 0, HCCL_ERROR("[InsTempAlltoAllVMesh1D][KernelRun] datatype size is zero."), HCCL_E_PARA);

    channelsPerRank_ = CalcChannelsPerRank(templateResource.channels);
    CHK_PRT_RET(
        channelsPerRank_ == 0, HCCL_ERROR("[InsTempAlltoAllVMesh1D][KernelRun] channels per rank is zero."),
        HCCL_E_PARA);
    const u32 concurrentRankNum = std::min(ALLTOALLV_DIRECT_FULLMESH_CONCURRENT_SIZE, templateRankSize_ - 1);
    const u64 expectedThreadNum = 1 + static_cast<u64>(concurrentRankNum) * channelsPerRank_;
    CHK_PRT_RET(
        templateResource.threads.size() != expectedThreadNum,
        HCCL_ERROR(
            "[InsTempAlltoAllVMesh1D][KernelRun] thread count[%zu] is not expected[%llu].",
            templateResource.threads.size(), expectedThreadNum),
        HCCL_E_PARA);

    concurrentSendRecvNum_ = concurrentRankNum;
    threadNum_ = static_cast<u32>(templateResource.threads.size());
    isDmaRead_ = IsPcieProtocol(templateResource.channels);
    CHK_RET(RunAllToAll(templateResource.channels, templateResource.threads, tempAlgParams, myRank_));
    return HCCL_SUCCESS;
}

void InsTempAlltoAllVMesh1D::CalcCommRankSetForOneLoop(
    u32 roundIdx, u32 remainRankSize, std::vector<u32>& commRanks) const
{
    commRanks.clear();
    const u32 pairNumPerRound = (concurrentSendRecvNum_ + 1) / 2;
    const u32 pairSize = remainRankSize < concurrentSendRecvNum_ ? (remainRankSize + 1) / 2 : pairNumPerRound;
    for (u32 distance = roundIdx * pairNumPerRound + 1; distance < roundIdx * pairNumPerRound + pairSize + 1;
         ++distance) {
        const u32 leftRank = (myRank_ + templateRankSize_ - distance) % templateRankSize_;
        const u32 rightRank = (myRank_ + distance) % templateRankSize_;
        commRanks.push_back(leftRank);
        if (leftRank == rightRank) {
            break;
        }
        commRanks.push_back(rightRank);
    }
}

u32 InsTempAlltoAllVMesh1D::CalcCommLoops() const
{
    const u32 remoteRankNum = templateRankSize_ - 1;
    return (remoteRankNum + concurrentSendRecvNum_ - 1) / concurrentSendRecvNum_;
}

void InsTempAlltoAllVMesh1D::CalcCclBuffIdx(u32 remoteRank, u32& myRankCclBuffIdx, u32& remoteCclBuffIdx) const
{
    const u32 pairNum = (concurrentSendRecvNum_ + 1) / 2;
    const u32 gapRight = (templateRankSize_ + remoteRank - myRank_) % templateRankSize_;
    const u32 gapLeft = (templateRankSize_ + myRank_ - remoteRank) % templateRankSize_;
    if (gapLeft < gapRight) {
        myRankCclBuffIdx = pairNum - 1 - ((gapLeft - 1) % pairNum);
        remoteCclBuffIdx = pairNum + ((gapLeft - 1) % pairNum);
    } else if (gapLeft > gapRight) {
        myRankCclBuffIdx = pairNum + ((gapRight - 1) % pairNum);
        remoteCclBuffIdx = pairNum - 1 - ((gapRight - 1) % pairNum);
    } else {
        myRankCclBuffIdx = 0;
        remoteCclBuffIdx = 0;
    }
}

HcclResult InsTempAlltoAllVMesh1D::LocalCopyForMyRank(
    const TemplateDataParams& tempAlgParams, const ThreadHandle& thread, u32 myAlgRank) const
{
    CHK_PRT_RET(
        tempAlgParams.sendCounts[myAlgRank] != tempAlgParams.recvCounts[myAlgRank],
        HCCL_ERROR(
            "[InsTempAlltoAllVMesh1D][LocalCopyForMyRank] self send count[%llu] differs from recv count[%llu].",
            tempAlgParams.sendCounts[myAlgRank], tempAlgParams.recvCounts[myAlgRank]),
        HCCL_E_PARA);
    if (tempAlgParams.sendCounts[myAlgRank] == 0) {
        return HCCL_SUCCESS;
    }

    u64 srcOffset = 0;
    u64 dstOffset = 0;
    u64 dataSize = 0;
    CHK_PRT_RET(
        !CalcByteOffset(tempAlgParams.sdispls[myAlgRank], dataTypeSize_, 0, srcOffset) ||
            !CalcByteOffset(tempAlgParams.rdispls[myAlgRank], dataTypeSize_, 0, dstOffset) ||
            MulOverflow(tempAlgParams.sendCounts[myAlgRank], dataTypeSize_, dataSize),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][LocalCopyForMyRank] offset or size overflows."), HCCL_E_PARA);
    DataSlice srcSlice(tempAlgParams.buffInfo.inputPtr, srcOffset, dataSize, tempAlgParams.sendCounts[myAlgRank]);
    DataSlice dstSlice(tempAlgParams.buffInfo.outputPtr, dstOffset, dataSize, tempAlgParams.recvCounts[myAlgRank]);
    CHK_RET(LocalCopy(thread, srcSlice, dstSlice));
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::RunAllToAll(
    const std::map<u32, std::vector<ChannelInfo>>& channels, const std::vector<ThreadHandle>& threads,
    const TemplateDataParams& tempAlgParams, u32 myAlgRank)
{
    const u32 commLoops = CalcCommLoops();
    u32 remainRankSize = templateRankSize_ - 1;
    std::vector<u32> commRanks;
    std::vector<ThreadHandle> subThreads(threads.begin() + 1, threads.end());

    GetNotifyIdxMainToSub(notifyIdxMainToSub_);
    CHK_RET(PreSyncInterThreads(threads[0], subThreads, notifyIdxMainToSub_));
    for (u32 roundIdx = 0; roundIdx < commLoops && remainRankSize > 0; ++roundIdx) {
        CalcCommRankSetForOneLoop(roundIdx, remainRankSize, commRanks);
        CHK_PRT_RET(
            commRanks.empty() || commRanks.size() > remainRankSize,
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][RunAllToAll] invalid rank set for round[%u].", roundIdx),
            HCCL_E_INTERNAL);
        if (isDmaRead_ && roundIdx == 0) {
            CHK_RET(PreCopyByLoop(commRanks, channels, threads, tempAlgParams));
            GetNotifyIdxSubToMain(notifyIdxSubToMain_);
            CHK_RET(PostSyncInterThreads(threads[0], subThreads, notifyIdxSubToMain_));
            CHK_RET(PreSyncInterThreads(threads[0], subThreads, notifyIdxMainToSub_));
        }
        if (roundIdx == 0) {
            CHK_RET(LocalCopyForMyRank(tempAlgParams, threads[0], myAlgRank));
        }
        CHK_RET(RunSendRecvByLoop(commRanks, tempAlgParams, channels, threads, roundIdx, commLoops));
        remainRankSize -= static_cast<u32>(commRanks.size());
    }
    GetNotifyIdxSubToMain(notifyIdxSubToMain_);
    CHK_RET(PostSyncInterThreads(threads[0], subThreads, notifyIdxSubToMain_));
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::RunSendRecvByLoop(
    const std::vector<u32>& commRanks, const TemplateDataParams& tempAlgParams,
    const std::map<u32, std::vector<ChannelInfo>>& channels, const std::vector<ThreadHandle>& threads, u32 roundIdx,
    u32 commLoops)
{
    for (u32 remoteRank : commRanks) {
        auto channelIt = channels.find(remoteRank);
        CHK_PRT_RET(
            channelIt == channels.end() || channelIt->second.empty(),
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][RunSendRecvByLoop] no channel for rank[%u].", remoteRank),
            HCCL_E_PARA);
        const std::vector<ChannelInfo>& currentChannels = channelIt->second;
        const u32 validChannelNum = std::min(static_cast<u32>(currentChannels.size()), channelsPerRank_);
        CHK_RET(CalcDataSplitByPortGroupCommon(
            tempAlgParams.sendCounts[remoteRank], dataTypeSize_, currentChannels, sendCountsSplit_, sendSizeSplit_,
            sendOffsetSplit_, validChannelNum));
        CHK_RET(CalcDataSplitByPortGroupCommon(
            tempAlgParams.recvCounts[remoteRank], dataTypeSize_, currentChannels, recvCountsSplit_, recvSizeSplit_,
            recvOffsetSplit_, validChannelNum));
        CHK_RET(RunSendRecvByChannel(
            tempAlgParams, roundIdx, validChannelNum, currentChannels, remoteRank, threads, commLoops));
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::PreSyncInterThreadsPerRank(
    const ThreadHandle& mainThread, const std::vector<ThreadHandle>& subThreads) const
{
    std::vector<u32> notifyIndexes(subThreads.size(), 1);
    return PreSyncInterThreads(mainThread, subThreads, notifyIndexes);
}

HcclResult InsTempAlltoAllVMesh1D::PostSyncInterThreadsPerRank(
    const ThreadHandle& mainThread, const std::vector<ThreadHandle>& subThreads) const
{
    std::vector<u32> notifyIndexes;
    for (u32 index = 0; index < subThreads.size(); ++index) {
        notifyIndexes.push_back(index + 1);
    }
    return PostSyncInterThreads(mainThread, subThreads, notifyIndexes);
}

HcclResult InsTempAlltoAllVMesh1D::RunSendRecvByChannel(
    const TemplateDataParams& tempAlgParams, u32 roundIdx, u32 validChannelNum,
    const std::vector<ChannelInfo>& channels, u32 remoteRank, const std::vector<ThreadHandle>& threads,
    u32 commLoops) const
{
    u32 myRankCclBuffIdx = 0;
    u32 remoteCclBuffIdx = 0;
    CalcCclBuffIdx(remoteRank, myRankCclBuffIdx, remoteCclBuffIdx);
    const u64 firstThreadIndex = static_cast<u64>(myRankCclBuffIdx) * channelsPerRank_ + 1;
    CHK_PRT_RET(
        validChannelNum == 0 || firstThreadIndex + validChannelNum > threads.size(),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][RunSendRecvByChannel] thread index is out of range."), HCCL_E_PARA);

    const ThreadHandle& rankMainThread = threads[firstThreadIndex];
    std::vector<ThreadHandle> rankSubThreads;
    if (validChannelNum > 1) {
        rankSubThreads.assign(
            threads.begin() + firstThreadIndex + 1, threads.begin() + firstThreadIndex + validChannelNum);
        if (roundIdx != 0) {
            CHK_RET(PreSyncInterThreadsPerRank(rankMainThread, rankSubThreads));
        }
    }

    for (u32 channelId = 0; channelId < validChannelNum; ++channelId) {
        const u64 threadIndex = firstThreadIndex + channelId;
        if (roundIdx != 0 && isDmaRead_ && sendSizeSplit_[channelId] > 0) {
            CHK_RET(PreCopy(
                tempAlgParams, threads[threadIndex], myRankCclBuffIdx, remoteRank, sendSizeSplit_[channelId],
                sendCountsSplit_[channelId], sendOffsetSplit_[channelId]));
        }

        const ChannelInfo& channel = channels[channelId];
        u64 txSrcOffset = 0;
        u64 txDstOffset = 0;
        u64 rxSrcOffset = 0;
        u64 rxDstOffset = 0;
        u64 txDstBaseOffset = 0;
        u64 rxSrcBaseOffset = 0;
        CHK_PRT_RET(
            AddOverflow(tempAlgParams.buffInfo.hcclBuffBaseOff, sendOffsetSplit_[channelId], txDstBaseOffset) ||
                AddOverflow(tempAlgParams.buffInfo.hcclBuffBaseOff, recvOffsetSplit_[channelId], rxSrcBaseOffset) ||
                !CalcByteOffset(
                    tempAlgParams.sdispls[remoteRank], dataTypeSize_, sendOffsetSplit_[channelId], txSrcOffset) ||
                !CalcByteOffset(remoteCclBuffIdx, tempAlgParams.inputSliceStride, txDstBaseOffset, txDstOffset) ||
                !CalcByteOffset(remoteCclBuffIdx, tempAlgParams.inputSliceStride, rxSrcBaseOffset, rxSrcOffset) ||
                !CalcByteOffset(
                    tempAlgParams.rdispls[remoteRank], dataTypeSize_, recvOffsetSplit_[channelId], rxDstOffset),
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][RunSendRecvByChannel] data offset overflows."), HCCL_E_PARA);

        DataSlice txSrc(
            tempAlgParams.buffInfo.inputPtr, txSrcOffset, sendSizeSplit_[channelId], sendCountsSplit_[channelId]);
        DataSlice txDst(channel.remoteCclMem.addr, txDstOffset, sendSizeSplit_[channelId], sendCountsSplit_[channelId]);
        DataSlice rxSrc(channel.remoteCclMem.addr, rxSrcOffset, recvSizeSplit_[channelId], recvCountsSplit_[channelId]);
        DataSlice rxDst(
            tempAlgParams.buffInfo.outputPtr, rxDstOffset, recvSizeSplit_[channelId], recvCountsSplit_[channelId]);
        DataInfo sendInfo(channel, {{txSrc}, {txDst}}, dataType_);
        DataInfo recvInfo(channel, {{rxSrc}, {rxDst}}, dataType_);
        SendRecvInfo sendRecvInfo({channel, channel}, {{{txSrc}, {txDst}}, {{rxSrc}, {rxDst}}}, dataType_);
        CHK_RET(RunSendRecv(sendRecvInfo, sendInfo, recvInfo, threads[threadIndex], channelId));
        if (!isDmaRead_ && recvSizeSplit_[channelId] > 0) {
            CHK_RET(PostCopy(
                tempAlgParams, threads[threadIndex], myRankCclBuffIdx, remoteRank, recvSizeSplit_[channelId],
                recvCountsSplit_[channelId], recvOffsetSplit_[channelId]));
        }
    }
    if (validChannelNum > 1 && roundIdx + 1 != commLoops) {
        CHK_RET(PostSyncInterThreadsPerRank(rankMainThread, rankSubThreads));
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::RunSendRecv(
    const SendRecvInfo& sendRecvInfo, const DataInfo& sendInfo, const DataInfo& recvInfo, const ThreadHandle& thread,
    u32 channelId) const
{
    const bool hasSend = sendSizeSplit_[channelId] > 0;
    const bool hasRecv = recvSizeSplit_[channelId] > 0;
    if (isDmaRead_) {
        if (hasSend && hasRecv) {
            return SendRecvRead(sendRecvInfo, thread);
        }
        if (hasSend) {
            return SendRead(sendInfo, thread);
        }
        if (hasRecv) {
            return RecvRead(recvInfo, thread);
        }
        return HCCL_SUCCESS;
    }

    if (hasSend && hasRecv) {
        return SendRecvWrite(sendRecvInfo, thread);
    }
    if (hasSend) {
        CHK_RET(SendWrite(sendInfo, thread));
    }
    if (hasRecv) {
        CHK_RET(RecvWrite(recvInfo, thread));
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::PreCopyByLoop(
    const std::vector<u32>& commRanks, const std::map<u32, std::vector<ChannelInfo>>& channels,
    const std::vector<ThreadHandle>& threads, const TemplateDataParams& tempAlgParams)
{
    for (u32 remoteRank : commRanks) {
        u32 myRankCclBuffIdx = 0;
        u32 remoteCclBuffIdx = 0;
        CalcCclBuffIdx(remoteRank, myRankCclBuffIdx, remoteCclBuffIdx);
        (void)remoteCclBuffIdx;
        const u64 firstThreadIndex = static_cast<u64>(myRankCclBuffIdx) * channelsPerRank_ + 1;
        auto channelIt = channels.find(remoteRank);
        CHK_PRT_RET(
            channelIt == channels.end() || channelIt->second.empty(),
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][PreCopyByLoop] no channel for rank[%u].", remoteRank), HCCL_E_PARA);
        const std::vector<ChannelInfo>& currentChannels = channelIt->second;
        const u32 validChannelNum = std::min(static_cast<u32>(currentChannels.size()), channelsPerRank_);
        CHK_PRT_RET(
            firstThreadIndex + validChannelNum > threads.size(),
            HCCL_ERROR("[InsTempAlltoAllVMesh1D][PreCopyByLoop] thread index is out of range."), HCCL_E_PARA);
        CHK_RET(CalcDataSplitByPortGroupCommon(
            tempAlgParams.sendCounts[remoteRank], dataTypeSize_, currentChannels, sendCountsSplit_, sendSizeSplit_,
            sendOffsetSplit_, validChannelNum));
        for (u32 channelId = 0; channelId < validChannelNum; ++channelId) {
            if (sendSizeSplit_[channelId] > 0) {
                CHK_RET(PreCopy(
                    tempAlgParams, threads[firstThreadIndex + channelId], myRankCclBuffIdx, remoteRank,
                    sendSizeSplit_[channelId], sendCountsSplit_[channelId], sendOffsetSplit_[channelId]));
            }
        }
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAlltoAllVMesh1D::PreCopy(
    const TemplateDataParams& tempAlgParams, const ThreadHandle& thread, u32 myRankCclBuffIdx, u32 remoteRank,
    u64 sendSize, u64 sendCount, u64 sendOffset) const
{
    u64 srcOffset = 0;
    u64 dstOffset = 0;
    u64 dstBaseOffset = 0;
    CHK_PRT_RET(
        AddOverflow(tempAlgParams.buffInfo.hcclBuffBaseOff, sendOffset, dstBaseOffset) ||
            !CalcByteOffset(tempAlgParams.sdispls[remoteRank], dataTypeSize_, sendOffset, srcOffset) ||
            !CalcByteOffset(myRankCclBuffIdx, tempAlgParams.inputSliceStride, dstBaseOffset, dstOffset),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][PreCopy] data offset overflows."), HCCL_E_PARA);
    DataSlice src(tempAlgParams.buffInfo.inputPtr, srcOffset, sendSize, sendCount);
    DataSlice dst(tempAlgParams.buffInfo.hcclBuff.addr, dstOffset, sendSize, sendCount);
    return LocalCopy(thread, src, dst);
}

HcclResult InsTempAlltoAllVMesh1D::PostCopy(
    const TemplateDataParams& tempAlgParams, const ThreadHandle& thread, u32 myRankCclBuffIdx, u32 remoteRank,
    u64 recvSize, u64 recvCount, u64 recvOffset) const
{
    u64 srcOffset = 0;
    u64 dstOffset = 0;
    u64 srcBaseOffset = 0;
    CHK_PRT_RET(
        AddOverflow(tempAlgParams.buffInfo.hcclBuffBaseOff, recvOffset, srcBaseOffset) ||
            !CalcByteOffset(myRankCclBuffIdx, tempAlgParams.inputSliceStride, srcBaseOffset, srcOffset) ||
            !CalcByteOffset(tempAlgParams.rdispls[remoteRank], dataTypeSize_, recvOffset, dstOffset),
        HCCL_ERROR("[InsTempAlltoAllVMesh1D][PostCopy] data offset overflows."), HCCL_E_PARA);
    DataSlice src(tempAlgParams.buffInfo.hcclBuff.addr, srcOffset, recvSize, recvCount);
    DataSlice dst(tempAlgParams.buffInfo.outputPtr, dstOffset, recvSize, recvCount);
    return LocalCopy(thread, src, dst);
}

void InsTempAlltoAllVMesh1D::GetNotifyIdxMainToSub(std::vector<u32>& notifyIdxMainToSub)
{
    notifyIdxMainToSub.assign(threadNum_ > 0 ? threadNum_ - 1 : 0, 0);
}

void InsTempAlltoAllVMesh1D::GetNotifyIdxSubToMain(std::vector<u32>& notifyIdxSubToMain)
{
    notifyIdxSubToMain.clear();
    for (u32 notifyIdx = 0; notifyIdx + 1 < threadNum_; ++notifyIdx) {
        notifyIdxSubToMain.push_back(notifyIdx);
    }
}

} // namespace mc2_ops_hccl
