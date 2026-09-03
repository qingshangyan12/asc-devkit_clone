/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <algorithm>
#include <cmath>
#include "template_utils.h"
namespace mc2_ops_hccl {

HcclResult GetAlgRank(const u32 virtRank, const std::vector<u32>& rankIds, u32& algRank)
{
    std::vector<u32>::const_iterator topoVecIter = std::find(rankIds.begin(), rankIds.end(), virtRank);
    CHK_PRT_RET(
        topoVecIter == rankIds.end(), HCCL_ERROR("[GetAlgRank] Invalid virtual Rank!"), HcclResult::HCCL_E_PARA);
    algRank = distance(rankIds.begin(), topoVecIter);

    return HcclResult::HCCL_SUCCESS;
}

u32 GetNHRStepNum(u32 rankSize)
{
    u32 nSteps = 0;
    for (u32 tmp = rankSize - 1; tmp != 0; tmp >>= 1, nSteps++) {
    }
    HCCL_DEBUG("[NHRBase][GetStepNumInterServer] rankSize[%u] nSteps[%u]", rankSize, nSteps);

    return nSteps;
}

HcclResult CalcDataSplitByPortGroupCommon(
    const u64 totalDataCount, const u64 dataTypeSize, const std::vector<ChannelInfo>& channels,
    std::vector<u64>& elemCountOut, std::vector<u64>& sizeOut, std::vector<u64>& elemOffset, const u32 channelsPerRank)
{
    elemCountOut.clear();
    sizeOut.clear();
    elemOffset.clear();

    std::vector<u32> portGroups;
    u32 totalPorts = 0;
    u32 taskCount =
        (static_cast<u32>(channels.size()) > channelsPerRank) ? channelsPerRank : static_cast<u32>(channels.size());
    for (u32 i = 0; i < taskCount; i++) {
        const auto& ch = channels[i];
        portGroups.push_back(ch.portGroupSize);
        totalPorts += ch.portGroupSize;
        HCCL_INFO(
            "[CalcDataSplitByPortGroup] ch.portGroupSize[%u], totalPorts[%u], channelsPerRank[%u]", ch.portGroupSize,
            totalPorts, channelsPerRank);
    }

    u32 channelSize = portGroups.size();
    u64 accumCount = 0;
    u64 offset = 0;
    for (u32 channelIdx = 0; channelIdx < channelSize; channelIdx++) {
        u64 elemCount = 0;
        if (channelIdx == channelSize - 1) {
            elemCount = totalDataCount - accumCount;
        } else {
            CHK_PRT_RET(
                totalPorts == 0, HCCL_ERROR("[CalcDataSplitByPortGroup] totalPorts [%u] is 0.", totalPorts),
                HcclResult::HCCL_E_INTERNAL);
            elemCount = static_cast<u64>((totalDataCount * portGroups[channelIdx]) / totalPorts);
        }
        elemOffset.push_back(offset);
        elemCountOut.push_back(elemCount);
        u64 elemSize = elemCount * dataTypeSize;
        sizeOut.push_back(elemSize);
        offset += elemSize;
        accumCount += elemCount;
    }

    return HcclResult::HCCL_SUCCESS;
}

HcclResult CalcDataSplitByPortGroupZAxisDetour(
    const u64 totalDataCount, const u64 dataTypeSize, const std::vector<ChannelInfo>& channels,
    std::vector<u64>& elemCountOut, std::vector<u64>& sizeOut, std::vector<u64>& elemOffset,
    const u32 level0ChannelNumPerRank, const u32 level1ChannelNumPerRank, const float level0DataRatio)
{
    elemCountOut.clear();
    sizeOut.clear();
    elemOffset.clear();

    CHK_PRT_RET(
        level0DataRatio < 0.0f || level0DataRatio > 1.0f,
        HCCL_ERROR("[CalcDataSplitByPortGroupZAxisDetour] level0DataRatio[%f] is invalid.", level0DataRatio),
        HcclResult::HCCL_E_PARA);

    u64 level0DataCount = totalDataCount;
    if (level1ChannelNumPerRank != 0) {
        level0DataCount = static_cast<u64>(static_cast<double>(totalDataCount) * level0DataRatio);
        level0DataCount = std::min(level0DataCount, totalDataCount);
    }
    u64 level1DataCount = totalDataCount - level0DataCount;

    std::vector<ChannelInfo> level0Chs(channels.begin(), channels.begin() + level0ChannelNumPerRank);
    std::vector<u64> l0ElemCount;
    std::vector<u64> l0Size;
    std::vector<u64> l0Offset;
    CHK_RET(CalcDataSplitByPortGroupCommon(
        level0DataCount, dataTypeSize, level0Chs, l0ElemCount, l0Size, l0Offset, level0ChannelNumPerRank));

    std::vector<ChannelInfo> level1Chs(channels.begin() + level0ChannelNumPerRank, channels.end());
    std::vector<u64> l1ElemCount;
    std::vector<u64> l1Size;
    std::vector<u64> l1Offset;
    CHK_RET(CalcDataSplitByPortGroupCommon(
        level1DataCount, dataTypeSize, level1Chs, l1ElemCount, l1Size, l1Offset, level1ChannelNumPerRank));

    u64 level0TotalSize = 0;
    for (auto sz : l0Size) {
        level0TotalSize += sz;
    }
    for (auto& off : l1Offset) {
        off += level0TotalSize;
    }

    elemCountOut = l0ElemCount;
    elemCountOut.insert(elemCountOut.end(), l1ElemCount.begin(), l1ElemCount.end());
    sizeOut = l0Size;
    sizeOut.insert(sizeOut.end(), l1Size.begin(), l1Size.end());
    elemOffset = l0Offset;
    elemOffset.insert(elemOffset.end(), l1Offset.begin(), l1Offset.end());

    HCCL_INFO(
        "[CalcDataSplitByPortGroupZAxisDetour] totalDataCount[%llu], level0DataCount[%llu], "
        "level1DataCount[%llu], level0ChannelNumPerRank[%u], level1ChannelNumPerRank[%u], "
        "level0DataRatio[%f], elemCountOut.size[%zu]",
        totalDataCount, level0DataCount, level1DataCount, level0ChannelNumPerRank, level1ChannelNumPerRank,
        level0DataRatio, elemCountOut.size());

    return HcclResult::HCCL_SUCCESS;
}

bool IsAllConnetedWithTopo(const TopoInfoWithNetLayerDetails* topoInfo, const u32 netLayer, const CommTopo topoType)
{
    if (topoInfo == nullptr || topoInfo->netLayerDetails.localNetInsSizeOfLayer.size() <= netLayer ||
        topoInfo->topoInstDetailsOfLayer.size() <= netLayer) {
        return false;
    }
    const u32 localRankSize = topoInfo->netLayerDetails.localNetInsSizeOfLayer[netLayer];
    const auto& rankNumForTopoType = topoInfo->topoInstDetailsOfLayer[netLayer].rankNumForTopoType;
    const auto it = rankNumForTopoType.find(topoType);
    if (it == rankNumForTopoType.end()) {
        return false;
    }
    for (const auto topoRankNum : it->second) {
        if (topoRankNum == localRankSize) {
            return true;
        }
    }
    return false;
}

namespace {
bool GetPortGroupSize(const std::map<u32, std::vector<ChannelInfo>>& channels, uint64_t& portGroupSize)
{
    portGroupSize = 0;
    for (const auto& entry : channels) {
        if (entry.second.empty()) {
            continue;
        }
        for (const auto& channel : entry.second) {
            portGroupSize += channel.portGroupSize;
        }
        return portGroupSize != 0;
    }
    return false;
}

double NormalizeSplitFallback(double fallbackRatio)
{
    return std::isfinite(fallbackRatio) ? std::max(0.0, std::min(fallbackRatio, 1.0)) : 0.5;
}

double QuantizeSplitRatio(double ratio)
{
    constexpr double RATIO_STEP = 1.0 / 8.0;
    const double ratioIndex = std::max(1.0, std::min(std::round(ratio / RATIO_STEP), 7.0));
    return ratioIndex * RATIO_STEP;
}
} // namespace

double CalcParallelDataSplitRatio(
    uint64_t intraRankSize, uint64_t interRankSize, const std::map<u32, std::vector<ChannelInfo>>& intraChannels,
    const std::map<u32, std::vector<ChannelInfo>>& interChannels, ParallelDataSplitType splitType,
    double fallbackRatio)
{
    const double fallback = NormalizeSplitFallback(fallbackRatio);
    uint64_t intraPortGroupSize = 0;
    uint64_t interPortGroupSize = 0;
    if (intraRankSize == 0 || interRankSize == 0 ||
        !GetPortGroupSize(intraChannels, intraPortGroupSize) ||
        !GetPortGroupSize(interChannels, interPortGroupSize)) {
        return fallback;
    }

    // A POD inter group has two links on different dies but half the effective bandwidth per link.
    bool isPod = false;
    for (const auto& entry : interChannels) {
        if (entry.second.empty()) {
            continue;
        }
        isPod = entry.second.size() == 2 && entry.second[0].dieId != INVALID_VALUE_RANKID &&
                entry.second[1].dieId != INVALID_VALUE_RANKID && entry.second[0].dieId != entry.second[1].dieId;
        break;
    }

    intraPortGroupSize *= intraRankSize - 1;
    const double effectiveInterPortGroupSize =
        static_cast<double>(interPortGroupSize) / (isPod ? 2.0 : 1.0);
    if (intraPortGroupSize == 0 || effectiveInterPortGroupSize <= 0.0 || !std::isfinite(effectiveInterPortGroupSize)) {
        return fallback;
    }

    double meshTime = 0.0;
    double closTime = 0.0;
    switch (splitType) {
        case ParallelDataSplitType::ALL_GATHER:
            meshTime = static_cast<double>(intraRankSize) * (interRankSize - 1) / effectiveInterPortGroupSize;
            closTime = static_cast<double>(interRankSize) * (intraRankSize - 1) /
                       static_cast<double>(intraPortGroupSize);
            break;
        case ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE:
        case ParallelDataSplitType::SCATTER:
            return fallback;
        default:
            return fallback;
    }

    const double denominator = meshTime + closTime;
    if (denominator <= 0.0 || !std::isfinite(denominator)) {
        return fallback;
    }
    return QuantizeSplitRatio(closTime / denominator);
}
} // namespace mc2_ops_hccl
