/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ins_temp_all_gather_nhr.h"
#include "alg_data_trans_wrapper.h"
#include "template_utils.h"
#include <algorithm>

namespace mc2_ops_hccl {

constexpr u32 CHANNEL_DOUBLE_FACTOR = 2;

InsTempAllGatherNHR::InsTempAllGatherNHR(
    const OpParam& param, const u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : InsAlgTemplateBase(param, rankId, subCommRanks)
{}
InsTempAllGatherNHR::~InsTempAllGatherNHR() {}

HcclResult InsTempAllGatherNHR::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    std::vector<HcclChannelDesc> channels;
    CHK_RET(CalcChannelRequestNhr(comm, param, topoInfo, subCommRanks_, channels));
    resourceRequest.channels.push_back(channels);
    channelsPerRank_ = CalcChannelsPerRank(channels);
    maxChannelsPerRank_ = channelsPerRank_;
    return GetRes(resourceRequest);
}

HcclResult InsTempAllGatherNHR::GetRes(AlgResourceRequest& resourceRequest) const
{
    const u32 threadNum = GetThreadNum();
    resourceRequest.slaveThreadNum = threadNum - 1;
    resourceRequest.notifyNumPerThread.assign(threadNum - 1, 2);
    resourceRequest.notifyNumOnMainThread = threadNum - 1;
    return HCCL_SUCCESS;
}

u64 InsTempAllGatherNHR::GetThreadNum() const { return maxChannelsPerRank_ * CHANNEL_DOUBLE_FACTOR; }

u64 InsTempAllGatherNHR::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return templateRankSize_;
}

HcclResult InsTempAllGatherNHR::PrepareDataSplitForMultiChannel(const TemplateResource& resource)
{
    const u32 typeSize = DATATYPE_SIZE_TABLE[dataType_];
    const u64 totalCount = tempAlgParams_.sliceSize / typeSize;
    if (resource.channels.empty() || resource.channels.begin()->second.empty()) {
        dataSplit_.assign(1, tempAlgParams_.sliceSize);
        dataOffset_.assign(1, 0);
        if (tempAlgParams_.tailSize) {
            dataSplitTail_.assign(1, tempAlgParams_.tailSize);
            dataOffsetTail_.assign(1, 0);
        }
        return HCCL_SUCCESS;
    }
    std::vector<u64> elemCountOut;
    CHK_RET(CalcDataSplitByPortGroupCommon(
        totalCount, typeSize, resource.channels.begin()->second, elemCountOut, dataSplit_, dataOffset_,
        channelsPerRank_));
    if (tempAlgParams_.tailSize) {
        CHK_RET(CalcDataSplitByPortGroupCommon(
            tempAlgParams_.tailSize / typeSize, typeSize, resource.channels.begin()->second, elemCountOut,
            dataSplitTail_, dataOffsetTail_, channelsPerRank_));
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::KernelRun(
    const OpParam& param, const TemplateDataParams& params, const TemplateResource& resource)
{
    if (params.sliceSize == 0 && params.tailSize == 0) return HCCL_SUCCESS;
    threadNum_ = GetThreadNum();
    CHK_PRT_RET(
        resource.threads.size() < threadNum_,
        HCCL_ERROR("[InsTempAllGatherNHR] thread resource is not enough."), HCCL_E_INTERNAL);
    tempAlgParams_ = params;
    dataType_ = param.DataDes.dataType;
    enableRemoteMemAccess_ = params.enableRemoteMemAccess;
    isDmaRead_ = IsPcieProtocol(resource.channels);
    CHK_RET(PrepareDataSplitForMultiChannel(resource));
    readLastStepToOutput_ = CanReadLastStepToOutput();
    skipOwnSliceCopy_ = readLastStepToOutput_ && CanSkipOwnSliceCopy();
    if (threadNum_ > 1) {
        std::vector<ThreadHandle> slaves(resource.threads.begin() + 1, resource.threads.begin() + threadNum_);
        GetNotifyIdxMainToSub(notifyIdxMainToSub_);
        CHK_RET(PreSyncInterThreads(resource.threads[0], slaves, notifyIdxMainToSub_));
    }
    for (u32 channel = 0; channel < channelsPerRank_; ++channel) {
        bool postCopy = false;
        CHK_RET(LocalDataCopy(resource.threads, channel));
        if (templateRankSize_ > 1) CHK_RET(RunAllGatherNHR(resource.threads, resource.channels, channel, postCopy));
        if (!postCopy) CHK_RET(PostLocalCopy(resource.threads[channel], channel));
    }
    if (threadNum_ > 1) {
        std::vector<ThreadHandle> slaves(resource.threads.begin() + 1, resource.threads.begin() + threadNum_);
        GetNotifyIdxSubToMain(notifyIdxSubToMain_);
        CHK_RET(PostSyncInterThreads(resource.threads[0], slaves, notifyIdxSubToMain_));
    }
    return HCCL_SUCCESS;
}

bool InsTempAllGatherNHR::CanReadLastStepToOutput() const
{
    return !isDmaRead_ && !enableRemoteMemAccess_ && tempAlgParams_.buffInfo.outBuffType == BufferType::OUTPUT &&
           tempAlgParams_.buffInfo.outputPtr != tempAlgParams_.buffInfo.hcclBuff.addr;
}
bool InsTempAllGatherNHR::CanSkipOwnSliceCopy() const
{
    return tempAlgParams_.buffInfo.inBuffType == BufferType::OUTPUT &&
           tempAlgParams_.buffInfo.outBuffType == BufferType::OUTPUT &&
           tempAlgParams_.buffInfo.inputPtr == tempAlgParams_.buffInfo.outputPtr &&
           tempAlgParams_.buffInfo.inBuffBaseOff == tempAlgParams_.buffInfo.outBuffBaseOff &&
           tempAlgParams_.inputSliceStride == tempAlgParams_.outputSliceStride &&
           tempAlgParams_.inputRepeatStride == tempAlgParams_.outputRepeatStride;
}
bool InsTempAllGatherNHR::IsLastStepReadSlice(u32 rank) const
{
    return std::find(lastStepReadSliceIdxs_.begin(), lastStepReadSliceIdxs_.end(), rank) != lastStepReadSliceIdxs_.end();
}

InsTempAllGatherNHR::SliceCalcInfo InsTempAllGatherNHR::CalcSliceInfo(
    const AicpuNHRStepInfo& step, u32 rpt, u32 i, u32 channel) const
{
    SliceCalcInfo info{step.txSliceIdxs[i], step.rxSliceIdxs[i], 0, 0, 0, 0, 0, 0, 0};
    info.txPartialOffset = (info.txIdx == templateRankSize_ - 1 && tempAlgParams_.tailSize) ? dataOffsetTail_[channel] : dataOffset_[channel];
    info.rxPartialOffset = (info.rxIdx == templateRankSize_ - 1 && tempAlgParams_.tailSize) ? dataOffsetTail_[channel] : dataOffset_[channel];
    info.scratchBase = tempAlgParams_.buffInfo.hcclBuffBaseOff + rpt * tempAlgParams_.sliceSize * templateRankSize_;
    info.txScratchOff = info.scratchBase + tempAlgParams_.sliceSize * info.txIdx + info.txPartialOffset;
    info.rxScratchOff = info.scratchBase + tempAlgParams_.sliceSize * info.rxIdx + info.rxPartialOffset;
    info.txSliceSize = (info.txIdx == templateRankSize_ - 1 && tempAlgParams_.tailSize) ? dataSplitTail_[channel] : dataSplit_[channel];
    info.rxSliceSize = (info.rxIdx == templateRankSize_ - 1 && tempAlgParams_.tailSize) ? dataSplitTail_[channel] : dataSplit_[channel];
    return info;
}

HcclResult InsTempAllGatherNHR::BuildStepSlices(
    const ChannelInfo& send, const ChannelInfo& recv, const AicpuNHRStepInfo& step, const u32& channel,
    StepBuildMode mode, std::vector<DataSlice>& txSrc, std::vector<DataSlice>& txDst,
    std::vector<DataSlice>& rxSrc, std::vector<DataSlice>& rxDst)
{
    const u32 typeSize = DATATYPE_SIZE_TABLE[dataType_];
    for (u32 rpt = 0; rpt < tempAlgParams_.repeatNum; ++rpt) {
        for (u32 i = 0; i < step.nSlices; ++i) {
            auto info = CalcSliceInfo(step, rpt, i, channel);
            if (mode == StepBuildMode::LAST_STEP_READ_TO_OUTPUT) {
                u64 out = tempAlgParams_.buffInfo.outBuffBaseOff + rpt * tempAlgParams_.outputRepeatStride + tempAlgParams_.outputSliceStride * info.rxIdx + info.rxPartialOffset;
                rxSrc.emplace_back(recv.remoteCclMem.addr, info.rxScratchOff, info.rxSliceSize, info.rxSliceSize / typeSize);
                rxDst.emplace_back(tempAlgParams_.buffInfo.outputPtr, out, info.rxSliceSize, info.rxSliceSize / typeSize);
                if (rpt == 0) lastStepReadSliceIdxs_.push_back(info.rxIdx);
            } else {
                txSrc.emplace_back(tempAlgParams_.buffInfo.hcclBuff.addr, info.txScratchOff, info.txSliceSize, info.txSliceSize / typeSize);
                txDst.emplace_back(send.remoteCclMem.addr, info.txScratchOff, info.txSliceSize, info.txSliceSize / typeSize);
                rxSrc.emplace_back(recv.remoteCclMem.addr, info.rxScratchOff, info.rxSliceSize, info.rxSliceSize / typeSize);
                rxDst.emplace_back(tempAlgParams_.buffInfo.hcclBuff.addr, info.rxScratchOff, info.rxSliceSize, info.rxSliceSize / typeSize);
            }
        }
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::RunLastStepReadToOutput(
    const std::vector<ThreadHandle>& threads, const ChannelInfo& send, const ChannelInfo& recv,
    const AicpuNHRStepInfo& step, const u32& channel, u32, bool& postCopy)
{
    std::vector<DataSlice> txSrc, txDst, rxSrc, rxDst;
    CHK_RET(BuildStepSlices(send, recv, step, channel, StepBuildMode::LAST_STEP_READ_TO_OUTPUT, txSrc, txDst, rxSrc, rxDst));
    if (rxSrc.empty()) return HCCL_SUCCESS;
    u32 postThread = channelsPerRank_ + channel;
    CHK_PRT_RET(postThread >= threads.size(), HCCL_ERROR("[InsTempAllGatherNHR] post copy thread is out of range."), HCCL_E_INTERNAL);
    CHK_RET(PreSyncInterThreads(threads[channel], {threads[postThread]}, {1}));
    CHK_RET(PostLocalCopy(threads[postThread], channel));
    postCopy = true;
    TxRxChannels pair(send, recv);
    TxRxSlicesList slices({{}, {}}, {rxSrc, rxDst});
    CHK_RET(SendRecvBatchRead(SendRecvInfo(pair, slices), threads[channel]));
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::RunStepNHR(
    const std::vector<ThreadHandle>& threads, const std::map<u32, std::vector<ChannelInfo>>& channels,
    const u32& channel, u32 step, u32 nSteps, bool& postCopy)
{
    AicpuNHRStepInfo info;
    CHK_RET(GetStepInfo(step, nSteps, info));
    u32 recvKey = GetRankFromMap(info.fromRank), sendKey = GetRankFromMap(info.toRank);
    CHK_PRT_RET(channels.count(recvKey) == 0 || channels.count(sendKey) == 0 || channel >= channels.at(recvKey).size() || channel >= channels.at(sendKey).size(), HCCL_ERROR("[InsTempAllGatherNHR] invalid channel access."), HCCL_E_INTERNAL);
    const auto& recv = channels.at(recvKey)[channel];
    const auto& send = channels.at(sendKey)[channel];
    if (readLastStepToOutput_ && step == nSteps - 1 && info.nSlices > 1) return RunLastStepReadToOutput(threads, send, recv, info, channel, step, postCopy);
    std::vector<DataSlice> txSrc, txDst, rxSrc, rxDst;
    CHK_RET(BuildStepSlices(send, recv, info, channel, StepBuildMode::NORMAL, txSrc, txDst, rxSrc, rxDst));
    TxRxChannels pair(send, recv);
    SendRecvInfo sendRecv(pair, TxRxSlicesList({txSrc, txDst}, {rxSrc, rxDst}), dataType_);
    CHK_PRT_RET(isDmaRead_ ? SendRecvRead(sendRecv, threads[channel]) : SendRecvBatchWrite(sendRecv, threads[channel]), HCCL_ERROR("[InsTempAllGatherNHR] sendrecv failed."), HCCL_E_INTERNAL);
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::RunAllGatherNHR(
    const std::vector<ThreadHandle>& threads, const std::map<u32, std::vector<ChannelInfo>>& channels,
    const u32& channel, bool& postCopy)
{
    const u32 steps = GetNHRStepNum(templateRankSize_);
    lastStepReadSliceIdxs_.clear();
    for (u32 step = 0; step < steps; ++step) CHK_RET(RunStepNHR(threads, channels, channel, step, steps, postCopy));
    return HCCL_SUCCESS;
}

u32 InsTempAllGatherNHR::GetRankFromMap(const u32 rank) const { return subCommRanks_[0].at(rank); }
HcclResult InsTempAllGatherNHR::GetStepInfo(u32 step, u32 steps, AicpuNHRStepInfo& info)
{
    u32 rank = 0;
    CHK_RET(GetAlgRank(myRank_, subCommRanks_[0], rank));
    u32 delta = 1 << (steps - 1 - step);
    info.step = step;
    info.myRank = rank;
    info.fromRank = (rank + templateRankSize_ - delta) % templateRankSize_;
    info.toRank = (rank + delta) % templateRankSize_;
    info.nSlices = (templateRankSize_ - 1 + delta) / (1 << (steps - step));
    u32 sliceDelta = 1 << (steps - step);
    u32 tx = rank, rx = (rank + templateRankSize_ - delta) % templateRankSize_;
    info.txSliceIdxs.clear();
    info.rxSliceIdxs.clear();
    for (u32 i = 0; i < info.nSlices; ++i) {
        info.txSliceIdxs.push_back(tx);
        info.rxSliceIdxs.push_back(rx);
        tx = (tx + templateRankSize_ - sliceDelta) % templateRankSize_;
        rx = (rx + templateRankSize_ - sliceDelta) % templateRankSize_;
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::LocalDataCopy(const std::vector<ThreadHandle>& threads, const u32& channel)
{
    u32 rank = 0;
    CHK_RET(GetAlgRank(myRank_, subCommRanks_[0], rank));
    u64 size = tempAlgParams_.sliceSize, offset = dataOffset_[channel];
    if (tempAlgParams_.tailSize && rank == templateRankSize_ - 1) { size = tempAlgParams_.tailSize; offset = dataOffsetTail_[channel]; }
    const u32 typeSize = DATATYPE_SIZE_TABLE[dataType_];
    for (u64 rpt = 0; rpt < tempAlgParams_.repeatNum; ++rpt) {
        u64 in = tempAlgParams_.buffInfo.inBuffBaseOff + rpt * tempAlgParams_.inputRepeatStride + tempAlgParams_.inputSliceStride * rank + offset;
        u64 scratch = tempAlgParams_.buffInfo.hcclBuffBaseOff + rpt * tempAlgParams_.sliceSize * templateRankSize_ + tempAlgParams_.sliceSize * rank + offset;
        if (tempAlgParams_.buffInfo.inputPtr == tempAlgParams_.buffInfo.hcclBuff.addr && in == scratch) continue;
        CHK_RET(LocalCopy(threads[channel], DataSlice(tempAlgParams_.buffInfo.inputPtr, in, size, size / typeSize), DataSlice(tempAlgParams_.buffInfo.hcclBuff.addr, scratch, size, size / typeSize)));
    }
    return HCCL_SUCCESS;
}

HcclResult InsTempAllGatherNHR::PostLocalCopy(const ThreadHandle& thread, const u32& channel)
{
    if (tempAlgParams_.buffInfo.outputPtr == tempAlgParams_.buffInfo.hcclBuff.addr) return HCCL_SUCCESS;
    const u32 typeSize = DATATYPE_SIZE_TABLE[dataType_];
    for (u32 rpt = 0; rpt < tempAlgParams_.repeatNum; ++rpt) {
        u64 base = tempAlgParams_.buffInfo.hcclBuffBaseOff + rpt * tempAlgParams_.sliceSize * templateRankSize_;
        u64 outBase = tempAlgParams_.buffInfo.outBuffBaseOff + rpt * tempAlgParams_.outputRepeatStride;
        for (u32 rank = 0; rank < templateRankSize_; ++rank) {
            u32 myAlgRank = 0;
            CHK_RET(GetAlgRank(myRank_, subCommRanks_[0], myAlgRank));
            if (readLastStepToOutput_ && skipOwnSliceCopy_ && rank == myAlgRank) continue;
            if (readLastStepToOutput_ && IsLastStepReadSlice(rank)) continue;
            u64 size = tempAlgParams_.sliceSize, offset = dataOffset_[channel];
            if (tempAlgParams_.tailSize && rank == templateRankSize_ - 1) { size = tempAlgParams_.tailSize; offset = dataOffsetTail_[channel]; }
            CHK_RET(LocalCopy(thread, DataSlice(tempAlgParams_.buffInfo.hcclBuff.addr, base + tempAlgParams_.sliceSize * rank + offset, size, size / typeSize), DataSlice(tempAlgParams_.buffInfo.outputPtr, outBase + tempAlgParams_.outputSliceStride * rank + offset, size, size / typeSize)));
        }
    }
    return HCCL_SUCCESS;
}

void InsTempAllGatherNHR::GetNotifyIdxMainToSub(std::vector<u32>& indexes)
{
    indexes.assign(GetThreadNum() - 1, 0);
}
void InsTempAllGatherNHR::GetNotifyIdxSubToMain(std::vector<u32>& indexes)
{
    indexes.clear();
    for (u32 i = 0; i < GetThreadNum() - 1; ++i) indexes.push_back(i);
}

} // namespace mc2_ops_hccl
