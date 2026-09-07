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
#include <limits>
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
constexpr double POD_PORT_GROUP_DIVISOR = 2.0;
constexpr u32 POD_CHANNEL_NUM = 2;
constexpr double FLOAT_COMPARE_EPSILON = 1e-6;

bool IsDoubleEqual(const double lhs, const double rhs)
{
    return std::abs(lhs - rhs) < FLOAT_COMPARE_EPSILON;
}

bool IsPodInterChannelGroup(const std::map<u32, std::vector<ChannelInfo>>& channels)
{
    for (const auto& entry : channels) {
        const auto& channelGroup = entry.second;
        if (channelGroup.empty()) {
            continue;
        }
        if (channelGroup.size() != POD_CHANNEL_NUM) {
            return false;
        }
        return channelGroup[0].dieId != INVALID_VALUE_RANKID && channelGroup[1].dieId != INVALID_VALUE_RANKID &&
               channelGroup[0].dieId != channelGroup[1].dieId;
    }
    return false;
}

double NormalizeParallelFallbackRatio(const double fallbackRatio)
{
    return std::isfinite(fallbackRatio) ? std::max(0.0, std::min(fallbackRatio, 1.0)) : 0.5;
}
} // namespace

bool GetPortGroupSize(const std::map<u32, std::vector<ChannelInfo>>& channels, uint64_t& portGroupSize)
{
    portGroupSize = 0;
    for (const auto& entry : channels) {
        if (!entry.second.empty()) {
            for (const auto& channel : entry.second) {
                portGroupSize += channel.portGroupSize;
            }
            return portGroupSize != 0;
        }
    }
    return false;
}

double CalcParallelDataSplitRatio(
    const uint64_t intraRankSize, const uint64_t interRankSize,
    const std::map<u32, std::vector<ChannelInfo>>& intraChannels,
    const std::map<u32, std::vector<ChannelInfo>>& interChannels, const ParallelDataSplitType splitType,
    const double fallbackRatio)
{
    const double validFallback = NormalizeParallelFallbackRatio(fallbackRatio);
    uint64_t intraPortGroupSize = 0;
    uint64_t interPortGroupSize = 0;
    if (intraRankSize == 0 || interRankSize == 0 ||
        !GetPortGroupSize(intraChannels, intraPortGroupSize) ||
        !GetPortGroupSize(interChannels, interPortGroupSize)) {
        return validFallback;
    }
    if (intraRankSize - 1 > std::numeric_limits<uint64_t>::max() / intraPortGroupSize) {
        return validFallback;
    }

    intraPortGroupSize *= intraRankSize - 1;
    const bool isPod = IsPodInterChannelGroup(interChannels);
    const double effectiveInterPortGroupSize =
        static_cast<double>(interPortGroupSize) / (isPod ? POD_PORT_GROUP_DIVISOR : 1.0);
    if (intraPortGroupSize == 0 || IsDoubleEqual(effectiveInterPortGroupSize, 0.0) ||
        !std::isfinite(effectiveInterPortGroupSize)) {
        return validFallback;
    }

    double mesh = 0.0;
    double clos = 0.0;
    switch (splitType) {
        case ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE:
            mesh = 21.0 * static_cast<double>(intraRankSize - 1) /
                   (20.0 * static_cast<double>(intraRankSize) * intraPortGroupSize);
            clos = static_cast<double>(interRankSize - 1) /
                   (static_cast<double>(interRankSize) * effectiveInterPortGroupSize);
            break;
        case ParallelDataSplitType::SCATTER:
            mesh = static_cast<double>(intraRankSize - 1) /
                   (static_cast<double>(intraRankSize) * intraPortGroupSize);
            clos = static_cast<double>(interRankSize - 1) /
                   (static_cast<double>(interRankSize) * effectiveInterPortGroupSize);
            break;
        case ParallelDataSplitType::ALL_GATHER:
            mesh = static_cast<double>(intraRankSize) * static_cast<double>(interRankSize - 1) /
                   effectiveInterPortGroupSize;
            clos = static_cast<double>(interRankSize) * static_cast<double>(intraRankSize - 1) /
                   static_cast<double>(intraPortGroupSize);
            break;
        default:
            return validFallback;
    }

    const double denominator = mesh + clos;
    if (IsDoubleEqual(denominator, 0.0) || !std::isfinite(denominator)) {
        return validFallback;
    }
    const double ratio = clos / denominator;
    if (!std::isfinite(ratio) || ratio < 0.0 || ratio > 1.0) {
        return validFallback;
    }
    constexpr double ratioStep = 1.0 / 8.0;
    const double ratioIndex = std::max(1.0, std::min(std::round(ratio / ratioStep), 7.0));
    return ratioIndex * ratioStep;
}
} // namespace mc2_ops_hccl
