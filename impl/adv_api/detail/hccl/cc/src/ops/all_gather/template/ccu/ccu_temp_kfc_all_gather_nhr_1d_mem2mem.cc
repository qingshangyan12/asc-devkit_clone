/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// 与 hccl src/ops/all_gather/algorithm/template/ccu/ccu_temp_all_gather_nhr_1D_mem2mem.cc 逐行对照移植。
// hccl -> KFC 适配：
//  1. 双 die 语义完整迁移（对齐 RS SoleNHR 2die KFC 先例）：CalcRes 按通道探测 dieNum（1/2），
//     每 die 一份 kernelInfo（axisId 区分，KFC 下映射为双 mission，由 CcuTempKfcServer::CalcRes 继承调度）；
//     带宽比经 resourceRequest.dieSplitRatio 下发，AIV prepare 按千分比切分 die0/die1 份额。
//  2. 通道走 CalcChannelRequestNhr（hccl 非 MESH_1D_CLOS 路径）；按 (die, peer) 保留首条分组，
//     rank2ChannelIdx 以 sub-rank 为键且对两个 die 组通用（同一 peer 在两组各占同位次）。
//  3. KernelRun 为空壳：每轮参数由 AIV 侧 CcuPrepareForAllGatherSoleNhrM2M 写入 xnData 队列。
//  4. GetStepInfo 的 toRank/fromRank/nSlices/tx/rx 公式与 hccl 逐行一致（sub-rank 口径）。
#include "alg_data_trans_wrapper.h"
#include "channel.h"
#include "ccu_temp_kfc_all_gather_nhr_1d_mem2mem.h"

namespace mc2_ops_hccl {

CcuTempKfcAllGatherNHR1DMem2Mem::CcuTempKfcAllGatherNHR1DMem2Mem(
    const OpParam& param, u32 rankId, const std::vector<std::vector<u32>>& subCommRanks)
    : CcuAlgTemplateBase(param, rankId, subCommRanks)
{
    const auto& ranks = subCommRanks_[0];
    const auto it = std::find(ranks.begin(), ranks.end(), rankId);
    if (it != ranks.end()) {
        mySubCommRank_ = static_cast<uint32_t>(std::distance(ranks.begin(), it));
    }
    tempRankSize_ = static_cast<uint32_t>(ranks.size());
}

HcclResult CcuTempKfcAllGatherNHR1DMem2Mem::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    AlgResourceRequest& resourceRequest)
{
    CHK_RET(GetRes(resourceRequest));

    std::vector<HcclChannelDesc> channelDescs;
    CHK_RET(CalcChannelRequestNhr(comm, param, topoInfo, subCommRanks_, channelDescs));
    CHK_RET(RestoreChannelMap(channelDescs, rankIdToChannelDesc_));

    // 1.与 hccl 一致：从 channelDesc 探测 dieNum（L1 双 die 链路时为 2，每对 peer 两条链路各在一 die）
    uint32_t dieNum = 0;
    uint32_t enableDieId = 0;
    CHK_RET(GetDieInfoFromChannelDescs(comm, rankIdToChannelDesc_, myRank_, dieNum, enableDieId));
    CHK_PRT_RET(
        dieNum < 1U || dieNum > 2U,
        HCCL_ERROR("[CcuTempKfcAllGatherNHR1DMem2Mem::CalcRes] get channelDescs fail, dieNum[%u]", dieNum),
        HCCL_E_INTERNAL);

    // 双 die 时每个 die 一个 kernel 实例（KFC 下映射为双 mission，由 kfc server 模板继承调度）
    resourceRequest.ccuKernelNum.push_back(dieNum);

    // 2.通道按 die 分组：每 (die, peer) 保留首条；rank2ChannelIdx 以 sub-rank 为键且对两组通用
    //   （双 die 下同一 peer 在两组各占同位次，kernel 按下标取本 mission 组的通道，不变式
    //   rank2ChannelIdx.size() == channelsPerDie[d].size() 保持）
    std::map<uint32_t, std::map<uint32_t, HcclChannelDesc>> diePeerChannel;
    for (const auto& channel : channelDescs) {
        uint32_t channelDieId = 0;
        CHK_RET(GetChannelDieId(comm, myRank_, channel, channelDieId));
        // 单 die 时统一归组 0（dieId 可能为 1，如单 die 挂在 die1）；双 die 时 dieId∈{0,1}，
        // 越界值防御性跳过，避免分组越界
        const uint32_t dieGroup = (dieNum == 1U) ? 0U : channelDieId;
        if (dieGroup >= dieNum) {
            continue;
        }
        const uint32_t remoteSubRank = RemoteRankIdToSubRank(channel.remoteRank);
        diePeerChannel[dieGroup].emplace(remoteSubRank, channel);
    }
    std::vector<std::vector<HcclChannelDesc>> channelsPerDie(dieNum);
    std::map<uint32_t, uint32_t> rank2ChannelIdx;
    for (auto& dieEntry : diePeerChannel) {
        uint32_t channelIdx = 0;
        for (auto& peerEntry : dieEntry.second) {
            if (rank2ChannelIdx.count(peerEntry.first) == 0U) {
                rank2ChannelIdx[peerEntry.first] = channelIdx;
            }
            channelsPerDie[dieEntry.first].push_back(peerEntry.second);
            ++channelIdx;
        }
    }
    if (dieNum > 1U) {
        CHK_PRT_RET(
            channelsPerDie[0].empty() || channelsPerDie[1].empty(),
            HCCL_ERROR("[CcuTempKfcAllGatherNHR1DMem2Mem::CalcRes] some die has no channel after grouping"),
            HCCL_E_INTERNAL);
        // 跨框 die0 连 die1 场景：出框端口数多的 die 组放前面，避免建链失败（与 RS 2die 先例一致）
        CHK_RET(ReverseChannelPerDieIfNeed(comm, myRank_, channelsPerDie));
    }

    // 3.双 die 按通道带宽比切分数据（AIV prepare 消费 dieSplitRatio 计算 die0/die1 份额）
    double ratio = 1.0;
    if (dieNum == 2U) {
        uint32_t portNum0 = 0;
        uint32_t portNum1 = 0;
        CHK_RET(GetChannelBwCoeff(comm, myRank_, channelsPerDie[0][0], portNum0));
        CHK_RET(GetChannelBwCoeff(comm, myRank_, channelsPerDie[1][0], portNum1));
        if (portNum0 + portNum1 > 0) {
            ratio = static_cast<double>(portNum0) / (portNum0 + portNum1);
        }
    }
    resourceRequest.dieSplitRatio = ratio;

    std::vector<KfcNhrStepInfo> stepInfoVector;
    CHK_RET(CalcNhrInfo(stepInfoVector));

    // 4.构造 kernelInfo：每 die 一份（axisId 区分实例），channels 为该 die 的链路组
    for (uint32_t kernelIdx = 0; kernelIdx < dieNum; kernelIdx++) {
        auto kernelArg = std::make_shared<CcuKernelArgKfcAllGatherNHR1DMem2Mem>();
        kernelArg->rankSize = tempRankSize_;
        kernelArg->rankId = mySubCommRank_;
        kernelArg->axisId = kernelIdx;
        kernelArg->axisSize = dieNum;
        kernelArg->dieSplitRatioPermille = DieSplitRatioToPermille(ratio);
        kernelArg->opParam = param;
        kernelArg->subCommRanks = subCommRanks_;
        kernelArg->stepInfoVector = stepInfoVector;
        kernelArg->rank2ChannelIdx = rank2ChannelIdx;

        CcuKernelInfo kernelInfo{};
        CHK_SAFETY_FUNC_RET(strcpy_s(
            kernelInfo.kernelFuncName, sizeof(kernelInfo.kernelFuncName), "CcuKernelKfcAllGatherNHR1DMem2Mem"));
        kernelInfo.channels = channelsPerDie[kernelIdx];
        kernelInfo.setKernelArg(kernelArg);
        resourceRequest.ccuKernelInfos.push_back(kernelInfo);
    }

    HCCL_INFO(
        "[CcuTempKfcAllGatherNHR1DMem2Mem::CalcRes] channelDescs.size()=%zu, dimsize=%u, dieNum=%u, "
        "dieSplitRatio=%.3f, ccuKernelInfos(contributed)=%u",
        channelDescs.size(), tempRankSize_, dieNum, ratio, dieNum);
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllGatherNHR1DMem2Mem::CalcNhrInfo(std::vector<KfcNhrStepInfo>& stepInfoVector) const
{
    u32 stepNum = 0;
    for (u32 ranks = tempRankSize_ - 1U; ranks != 0U; ranks >>= 1U) {
        ++stepNum;
    }
    for (u32 step = 0; step < stepNum; ++step) {
        KfcNhrStepInfo stepInfo;
        CHK_RET(GetStepInfo(step, stepNum, stepInfo));
        stepInfoVector.push_back(stepInfo);
    }
    return HCCL_SUCCESS;
}

// 与 hccl CcuTempAllGatherNHR1DMem2Mem::GetStepInfo 逐行对照：
// deltaRank = 1<<(nSteps-1-step)；sendTo = (r+delta)%size；recvFrom = (r+size-delta)%size；
// nSlices = (size-1+deltaRank)/deltaSlice；tx 从 mySubCommRank 起、rx 从 fromRank 起，按 deltaSlice 递减。
HcclResult CcuTempKfcAllGatherNHR1DMem2Mem::GetStepInfo(u32 step, u32 stepNum, KfcNhrStepInfo& stepInfo) const
{
    const u32 deltaRank = 1U << (stepNum - 1U - step);
    const u32 deltaSlice = 1U << (stepNum - step);
    stepInfo.step = step;
    stepInfo.myRank = mySubCommRank_;
    stepInfo.toRank = (mySubCommRank_ + deltaRank) % tempRankSize_;
    stepInfo.fromRank = (mySubCommRank_ + tempRankSize_ - deltaRank) % tempRankSize_;
    stepInfo.nSlices = (tempRankSize_ - 1U + deltaRank) / deltaSlice;
    u32 txSliceIdx = mySubCommRank_;
    u32 rxSliceIdx = stepInfo.fromRank;
    for (u32 i = 0; i < stepInfo.nSlices; ++i) {
        stepInfo.txSliceIdxs.push_back(txSliceIdx);
        stepInfo.rxSliceIdxs.push_back(rxSliceIdx);
        txSliceIdx = (txSliceIdx + tempRankSize_ - deltaSlice) % tempRankSize_;
        rxSliceIdx = (rxSliceIdx + tempRankSize_ - deltaSlice) % tempRankSize_;
    }
    return HCCL_SUCCESS;
}

uint32_t CcuTempKfcAllGatherNHR1DMem2Mem::RemoteRankIdToSubRank(uint32_t remoteRankId) const
{
    const auto& ranks = subCommRanks_[0];
    const auto it = std::find(ranks.begin(), ranks.end(), remoteRankId);
    return it == ranks.end() ? 0U : static_cast<uint32_t>(std::distance(ranks.begin(), it));
}

HcclResult CcuTempKfcAllGatherNHR1DMem2Mem::KernelRun(
    const OpParam& param, const TemplateDataParams& templateDataParams, TemplateResource& templateResource)
{
    // 每轮参数由 CcuPrepareForAllGatherSoleNhrM2M 写入 xnData 队列，由 KFC server 消费。
    (void)param;
    (void)templateDataParams;
    (void)templateResource;
    return HCCL_SUCCESS;
}

HcclResult CcuTempKfcAllGatherNHR1DMem2Mem::GetRes(AlgResourceRequest& resourceRequest) const
{
    resourceRequest.slaveThreadNum = 0;
    resourceRequest.notifyNumOnMainThread = 0;
    resourceRequest.notifyNumPerThread.assign(resourceRequest.slaveThreadNum, 1);
    return HCCL_SUCCESS;
}

u64 CcuTempKfcAllGatherNHR1DMem2Mem::GetThreadNum() const { return 1; }

u64 CcuTempKfcAllGatherNHR1DMem2Mem::CalcScratchMultiple(BufferType inBuffType, BufferType outBuffType)
{
    (void)inBuffType;
    (void)outBuffType;
    return 0;
}

} // namespace mc2_ops_hccl
