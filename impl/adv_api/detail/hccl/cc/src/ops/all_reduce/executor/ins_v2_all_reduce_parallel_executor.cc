/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ins_v2_all_reduce_parallel_executor.h"

#include <algorithm>
#include <cmath>

#include "alg_data_trans_wrapper.h"
#include "ins_temp_all_gather_mesh_1D.h"
#include "ins_temp_all_gather_nhr.h"
#include "ins_temp_reduce_scatter_mesh_1D.h"
#include "ins_temp_reduce_scatter_nhr.h"

namespace mc2_ops_hccl {
namespace {
constexpr u32 MAIN_THREAD_NUM = 2;
constexpr u32 INTRA_SLAVE_START_IDX = 3;
constexpr u32 INTER_SLAVE_OFFSET = 4;
} // namespace

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    InsAllReduceParallelExecutor()
{}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GetHierarchyInfo(
        const TopoInfoWithNetLayerDetails& topoInfo, const AlgHierarchyInfoForAllLevel& algHierarchyInfo,
        std::vector<std::vector<u32>>& intraHierarchyInfo, std::vector<std::vector<u32>>& interHierarchyInfo) const
{
    if (algHierarchyInfo.infos.empty()) {
        HCCL_ERROR("[%s] algHierarchyInfo.infos is empty.", __func__);
        return HCCL_E_PARA;
    }
    if (topoInfo.level0Topo == Level0Shape::MESH_1D_CLOS && !topoInfo.level0PcieMix) {
        if (algHierarchyInfo.infos[0].size() < 2 || algHierarchyInfo.infos[0][0].empty()) {
            HCCL_ERROR("[%s] level0 hierarchy does not contain mesh and CLOS ranks.", __func__);
            return HCCL_E_PARA;
        }
        intraHierarchyInfo = {algHierarchyInfo.infos[0][0]};
        const u32 meshSize = static_cast<u32>(algHierarchyInfo.infos[0][0].size());
        std::vector<u32> closRanks;
        for (const u32 rank : algHierarchyInfo.infos[0][1]) {
            if (rank % meshSize == topoInfo.userRank % meshSize) {
                closRanks.push_back(rank);
            }
        }
        interHierarchyInfo = {closRanks};
    } else {
        if (algHierarchyInfo.infos.size() < 2 || algHierarchyInfo.infos[0].empty() ||
            algHierarchyInfo.infos[1].empty()) {
            HCCL_ERROR("[%s] two-level hierarchy is incomplete.", __func__);
            return HCCL_E_PARA;
        }
        intraHierarchyInfo = algHierarchyInfo.infos[0];
        interHierarchyInfo = algHierarchyInfo.infos[1];
    }
    if (intraHierarchyInfo[0].empty() || interHierarchyInfo[0].empty()) {
        HCCL_ERROR("[%s] hierarchy rank list is empty.", __func__);
        return HCCL_E_PARA;
    }
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcAlgHierarchyInfo(
        HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    CHK_PTR_NULL(topoInfo);
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcRes(
        HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
        const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    CHK_PTR_NULL(topoInfo);
    CHK_PRT_RET(
        topoInfo->topoLevelNums != 2 || topoInfo->level0Topo != Level0Shape::MESH_1D,
        HCCL_ERROR("[%s] topology is not supported by AicpuAllReduceParallelMeshNHR.", __func__),
        HCCL_E_NOT_SUPPORT);
    CHK_PRT_RET(
        param.reduceType == HcclReduceOp::HCCL_REDUCE_PROD ||
            param.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_INT64 ||
            param.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_UINT64 ||
            param.DataDes.dataType == HcclDataType::HCCL_DATA_TYPE_FP64,
        HCCL_ERROR("[%s] data type or reduce operation is not supported.", __func__), HCCL_E_NOT_SUPPORT);
    std::vector<std::vector<u32>> intraHierarchyInfo;
    std::vector<std::vector<u32>> interHierarchyInfo;
    CHK_RET(GetHierarchyInfo(*topoInfo, algHierarchyInfo, intraHierarchyInfo, interHierarchyInfo));

    InsAlgTemplate0 tempIntra(param, topoInfo->userRank, intraHierarchyInfo);
    InsAlgTemplate1 tempInter(param, topoInfo->userRank, interHierarchyInfo);
    InsAlgTemplate2 tempIntra1(param, topoInfo->userRank, intraHierarchyInfo);
    InsAlgTemplate3 tempInter1(param, topoInfo->userRank, interHierarchyInfo);

    AlgResourceRequest intraRequest;
    AlgResourceRequest interRequest;
    AlgResourceRequest intraRequest1;
    AlgResourceRequest interRequest1;
    CHK_RET(tempIntra.CalcRes(comm, param, topoInfo, intraRequest));
    CHK_RET(tempInter.CalcRes(comm, param, topoInfo, interRequest));
    CHK_RET(tempIntra1.CalcRes(comm, param, topoInfo, intraRequest1));
    CHK_RET(tempInter1.CalcRes(comm, param, topoInfo, interRequest1));
    CHK_PRT_RET(
        intraRequest.channels.empty() || interRequest.channels.empty(),
        HCCL_ERROR("[%s] intra or inter template has no channels.", __func__), HCCL_E_INTERNAL);

    const u32 intraSlaveNum = std::max(intraRequest.slaveThreadNum, intraRequest1.slaveThreadNum);
    const u32 interSlaveNum = std::max(interRequest.slaveThreadNum, interRequest1.slaveThreadNum);
    resourceRequest.notifyNumOnMainThread = 2;
    resourceRequest.slaveThreadNum = intraSlaveNum + interSlaveNum + 4;
    resourceRequest.notifyNumPerThread.emplace_back(intraRequest.notifyNumOnMainThread + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), intraRequest.notifyNumPerThread.begin(),
        intraRequest.notifyNumPerThread.end());
    resourceRequest.notifyNumPerThread.emplace_back(intraRequest1.notifyNumOnMainThread + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), intraRequest1.notifyNumPerThread.begin(),
        intraRequest1.notifyNumPerThread.end());
    resourceRequest.notifyNumPerThread.emplace_back(interRequest.notifyNumOnMainThread + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), interRequest.notifyNumPerThread.begin(),
        interRequest.notifyNumPerThread.end());
    resourceRequest.notifyNumPerThread.emplace_back(interRequest1.notifyNumOnMainThread + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), interRequest1.notifyNumPerThread.begin(),
        interRequest1.notifyNumPerThread.end());
    resourceRequest.channels.emplace_back(intraRequest.channels[0]);
    resourceRequest.channels.emplace_back(interRequest.channels[0]);
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    PrepareResForTemplate(
        InsAlgTemplate0& tempAlgIntra, InsAlgTemplate1& tempAlgInter, InsAlgTemplate2& tempAlgIntra1,
        InsAlgTemplate3& tempAlgInter1)
{
    AlgResourceRequest intraRequest;
    AlgResourceRequest interRequest;
    AlgResourceRequest intraRequest1;
    AlgResourceRequest interRequest1;
    CHK_RET(tempAlgIntra.GetRes(intraRequest));
    CHK_RET(tempAlgInter.GetRes(interRequest));
    CHK_RET(tempAlgIntra1.GetRes(intraRequest1));
    CHK_RET(tempAlgInter1.GetRes(interRequest1));

    const u32 intraThreadsNum = intraRequest.slaveThreadNum + 1;
    const u32 intraThreadsNum1 = intraRequest1.slaveThreadNum + 1;
    const u32 intraThreadsNumFinal = std::max(intraThreadsNum, intraThreadsNum1);
    const u32 interThreadsNum = interRequest.slaveThreadNum + 1;
    const u32 interThreadsNum1 = interRequest1.slaveThreadNum + 1;
    const u32 interThreadsNumFinal = std::max(interThreadsNum, interThreadsNum1);
    const u32 requiredThreads = std::max(
        intraThreadsNumFinal + INTER_SLAVE_OFFSET + interThreadsNumFinal - 1,
        intraThreadsNumFinal + MAIN_THREAD_NUM + 1);
    CHK_PRT_RET(
        threads_.size() < requiredThreads, HCCL_ERROR("[%s] allocated thread count is insufficient.", __func__),
        HCCL_E_INTERNAL);

    intraThreads_.clear();
    intraThreads_.emplace_back(threads_.at(1));
    for (u32 i = INTRA_SLAVE_START_IDX; i < intraThreadsNum + MAIN_THREAD_NUM; ++i) {
        intraThreads_.emplace_back(threads_.at(i));
    }

    interThreads_.clear();
    interThreads_.emplace_back(threads_.at(intraThreadsNumFinal + MAIN_THREAD_NUM));
    for (u32 i = intraThreadsNumFinal + INTER_SLAVE_OFFSET; i < threads_.size(); ++i) {
        interThreads_.emplace_back(threads_.at(i));
    }
    CHK_PRT_RET(
        intraThreads_.empty() || interThreads_.empty(), HCCL_ERROR("[%s] template main thread is missing.", __func__),
        HCCL_E_INTERNAL);

    mainThread_ = threads_.at(0);
    templateMainThreads_ = {intraThreads_.at(0), interThreads_.at(0)};
    syncNotifyOnTemplates_ = {intraRequest.notifyNumOnMainThread, interRequest.notifyNumOnMainThread};
    syncNotifyOnMain_ = {0, 1};
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    PrepareResForTemplate23(
        InsAlgTemplate0& tempAlgIntra, InsAlgTemplate2& tempAlgIntra1, InsAlgTemplate3& tempAlgInter1)
{
    AlgResourceRequest intraRequest;
    AlgResourceRequest intraRequest1;
    AlgResourceRequest interRequest1;
    CHK_RET(tempAlgIntra.GetRes(intraRequest));
    CHK_RET(tempAlgIntra1.GetRes(intraRequest1));
    CHK_RET(tempAlgInter1.GetRes(interRequest1));

    const u32 intraThreadsNum1 = intraRequest1.slaveThreadNum + 1;
    const u32 intraThreadsNumFinal = std::max(intraRequest.slaveThreadNum + 1, intraThreadsNum1);
    const u32 interStart = intraThreadsNumFinal + INTRA_SLAVE_START_IDX;
    CHK_PRT_RET(
        threads_.size() <= interStart, HCCL_ERROR("[%s] allocated thread count is insufficient.", __func__),
        HCCL_E_INTERNAL);
    intraThreads_.assign(threads_.begin() + MAIN_THREAD_NUM, threads_.begin() + intraThreadsNum1 + MAIN_THREAD_NUM);
    interThreads_.assign(threads_.begin() + interStart, threads_.end());
    CHK_PRT_RET(
        intraThreads_.empty() || interThreads_.empty(), HCCL_ERROR("[%s] template main thread is missing.", __func__),
        HCCL_E_INTERNAL);
    mainThread_ = threads_.at(0);
    templateMainThreads_ = {intraThreads_.at(0), interThreads_.at(0)};
    syncNotifyOnTemplates_ = {intraRequest1.notifyNumOnMainThread, interRequest1.notifyNumOnMainThread};
    syncNotifyOnMain_ = {0, 1};
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GetParallelDataSplit(std::vector<float>& splitDataSize) const
{
    double ratio = multipleDimensionSplitRatio_;
    if (multipleDimensionSplitRatioSource_ == MultipleDimensionSplitRatioSource::BUILTIN_FORMULA) {
        ratio = CalcParallelDataSplitRatio(
            intraLocalRankSize_, interLocalRankSize_, intraLinks_, interLinks_,
            ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, multipleDimensionSplitRatio_);
    }
    ratio = std::max(0.0, std::min(ratio, 1.0));
    splitDataSize.push_back(static_cast<float>(ratio));
    splitDataSize.push_back(static_cast<float>(1.0 - ratio));
    return;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
u64 InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GetRankSize(const std::vector<std::vector<u32>>& hierarchy)
{
    u64 rankSize = 1;
    for (const auto& ranks : hierarchy) {
        rankSize *= ranks.size();
    }
    return rankSize;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    FillDataMap(
        u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap, bool isInter) const
{
    dataMap.clear();
    const u64 sliceSize = sliceCount * dataTypeSize_;
    const u64 rankStridePart1 = RoundDown(sliceSize, part1 * dataTypeSize_) * dataTypeSize_;
    for (u32 i = 0; i < part1; ++i) {
        const u64 curSizePart1 = (i == part1 - 1) ? sliceSize - rankStridePart1 * i : rankStridePart1;
        const u64 rankStridePart2 = RoundDown(curSizePart1, part2 * dataTypeSize_) * dataTypeSize_;
        for (u32 j = 0; j < part2; ++j) {
            const u64 curSizePart2 = (j == part2 - 1) ? curSizePart1 - rankStridePart2 * j : rankStridePart2;
            const u32 rankId = isInter ? CalcRankIdInter(i, j, part2) : CalcRankIdIntra(i, j, part1);
            dataMap[rankId] = {rankStridePart2, curSizePart2};
        }
    }
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    FillOffsetMaps(
        const std::map<u32, std::pair<u64, u64>>& dataMap, u32 part1, u32 part2, std::map<u32, u64>& agMap,
        bool isInter) const
{
    agMap.clear();
    u64 baseOffset = 0;
    for (u32 i = 0; i < part1; ++i) {
        u64 accumulatedSize = 0;
        for (u32 j = 0; j < part2; ++j) {
            const u32 rankId = isInter ? CalcRankIdInter(i, j, part2) : CalcRankIdIntra(i, j, part1);
            const auto it = dataMap.find(rankId);
            if (it == dataMap.end()) {
                continue;
            }
            agMap[rankId] = baseOffset;
            accumulatedSize += (j == part2 - 1) ? it->second.second : it->second.first;
        }
        baseOffset += accumulatedSize;
    }
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcInterDataAllRank(u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap)
{
    FillDataMap(sliceCount, part1, part2, dataMap, true);
    FillOffsetMaps(dataMap, part1, part2, rankBaseOffIntraAGMap_, true);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcIntraDataAllRank(u64 sliceCount, u32 part1, u32 part2, std::map<u32, std::pair<u64, u64>>& dataMap)
{
    FillDataMap(sliceCount, part1, part2, dataMap, false);
    FillOffsetMaps(dataMap, part1, part2, rankBaseOffInterAGMap_, false);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
u32 InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcRankIdInter(u32 i, u32 j, u32 part2)
{
    return i * part2 + j;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
u32 InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    CalcRankIdIntra(u32 i, u32 j, u32 part1)
{
    return j * part1 + i;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GenDataParamsAllRank(u64 sliceCount, u32 localRankSize, TemplateDataParams& dataParams) const
{
    const u64 sliceSize = sliceCount * dataTypeSize_;
    const u64 rankStride = RoundDown(sliceSize, localRankSize * dataTypeSize_) * dataTypeSize_;
    u64 tailSize = 0;
    for (u64 i = 0; i < localRankSize; ++i) {
        tailSize = (i == localRankSize - 1) ? sliceSize - rankStride * i : rankStride;
    }
    dataParams.tailSize = tailSize;
    dataParams.sliceSize = rankStride;
    dataParams.count = rankStride / dataTypeSize_;
    dataParams.inputSliceStride = rankStride;
    dataParams.outputSliceStride = rankStride;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GenDataParamsBufferType(
        BufferType inBuffType, BufferType outBuffType, BufferType hcclBuffType, TemplateDataParams& dataParams) const
{
    dataParams.buffInfo.inBuffType = inBuffType;
    dataParams.buffInfo.outBuffType = outBuffType;
    dataParams.buffInfo.hcclBuffType = hcclBuffType;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GenDataParams(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 sliceCount,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, u32 localRankSize, u64 inputOffset,
        u64 outputOffset, u64 hcclBuffOffset) const
{
    GenDataParamsAllRank(sliceCount, localRankSize, dataParams);
    dataParams.buffInfo.inputPtr = dataParams.buffInfo.inBuffType == BufferType::HCCL_BUFFER ? resCtx.cclMem.addr :
                                                                                                  param.inputPtr;
    dataParams.buffInfo.inputSize = dataParams.buffInfo.inBuffType == BufferType::HCCL_BUFFER ? resCtx.cclMem.size :
                                                                                                   param.inputSize;
    dataParams.buffInfo.outputPtr = dataParams.buffInfo.outBuffType == BufferType::HCCL_BUFFER ? resCtx.cclMem.addr :
                                                                                                    param.outputPtr;
    dataParams.buffInfo.outputSize = dataParams.buffInfo.outBuffType == BufferType::HCCL_BUFFER ? resCtx.cclMem.size :
                                                                                                     param.outputSize;
    dataParams.buffInfo.hcclBuff = resCtx.cclMem;
    dataParams.buffInfo.hcclBuffSize = resCtx.cclMem.size;
    dataParams.buffInfo.hcclBuffBaseOff = scratchOffsetCount * dataTypeSize_ + hcclBuffOffset;
    dataParams.buffInfo.inBuffBaseOff = dataParams.buffInfo.inBuffType == BufferType::HCCL_BUFFER ?
                                             scratchOffsetCount * dataTypeSize_ + inputOffset :
                                             dataOffset;
    dataParams.buffInfo.outBuffBaseOff = dataParams.buffInfo.outBuffType == BufferType::HCCL_BUFFER ?
                                              scratchOffsetCount * dataTypeSize_ + outputOffset :
                                              dataOffset;
    dataParams.repeatNum = 1;
    dataParams.inputRepeatStride = 0;
    dataParams.outputRepeatStride = 0;
    dataParams.enableRemoteMemAccess = param.opMode == OpMode::OFFLOAD;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    PrepareDataParamsInter(u64 dataOffset, u64 currCountPart, u64 scratchOffsetCount)
{
    const u32 myRankIdx = tempVirtRankMapInter_.at(myRank_);
    allRankSliceSize_.clear();
    u64 sendDataSize = 0;
    const u64 rankStride = RoundDown(currCountPart * dataTypeSize_, intraLocalRankSize_ * dataTypeSize_) * dataTypeSize_;
    for (u32 i = 0; i < intraLocalRankSize_; ++i) {
        const u64 curSize = i == intraLocalRankSize_ - 1 ? currCountPart * dataTypeSize_ - rankStride * i : rankStride;
        allRankSliceSize_.push_back(curSize);
        sendDataSize += curSize;
    }
    (void)sendDataSize;
    currCountPart0_ = allRankSliceSize_.at(myRankIdx) / dataTypeSize_;
    dataOffset0Inter_ = dataOffset;
    scratchOffsetCountInterStage1_ = scratchOffsetCount;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
void InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    PrepareDataParamsIntra(u64 dataOffset, u64 currCountPart, u64 scratchOffsetCount)
{
    const u32 myRankIdx = tempVirtRankMapIntra_.at(myRank_);
    allRankSliceSize_.clear();
    u64 sendDataSize = 0;
    const u64 rankStride = RoundDown(currCountPart * dataTypeSize_, interLocalRankSize_ * dataTypeSize_) * dataTypeSize_;
    for (u32 i = 0; i < interLocalRankSize_; ++i) {
        const u64 curSize = i == interLocalRankSize_ - 1 ? currCountPart * dataTypeSize_ - rankStride * i : rankStride;
        allRankSliceSize_.push_back(curSize);
        sendDataSize += curSize;
    }
    (void)sendDataSize;
    currCountPart1_ = allRankSliceSize_.at(myRankIdx) / dataTypeSize_;
    dataOffset0Intra_ = dataOffset;
    scratchOffsetCountIntraStage1_ = scratchOffsetCount;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    PrepareTemplateResource(
        const AlgResourceCtxSerializable& resCtx, TemplateResource& intraResource, TemplateResource& interResource) const
{
    intraResource.channels = intraLinks_;
    intraResource.threads = intraThreads_;
    intraResource.aivCommInfoPtr = resCtx.aivCommInfoPtr;
    interResource.channels = interLinks_;
    interResource.threads = interThreads_;
    interResource.aivCommInfoPtr = resCtx.aivCommInfoPtr;
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    Orchestrate(const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    if (resCtx.algHierarchyInfo.infos.empty()) {
        HCCL_ERROR("[%s] algHierarchyInfo.infos is empty.", __func__);
        return HCCL_E_PARA;
    }
    maxTmpMemSize_ = resCtx.cclMem.size;
    myRank_ = resCtx.topoInfo.userRank;
    threads_ = resCtx.threads;
    if (param.engine != CommEngine::COMM_ENGINE_AIV && param.engine != CommEngine::COMM_ENGINE_CCU) {
        CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
        CHK_PRT_RET(
            remoteRankToChannelInfo_.size() < 2, HCCL_ERROR("[%s] channel map has fewer than two levels.", __func__),
            HCCL_E_INTERNAL);
        intraLinks_ = remoteRankToChannelInfo_[0];
        interLinks_ = remoteRankToChannelInfo_[1];
    }
    dataCount_ = param.DataDes.count;
    dataType_ = param.DataDes.dataType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataType_];
    CHK_PRT_RET(dataTypeSize_ == 0, HCCL_ERROR("[%s] data type size is zero.", __func__), HCCL_E_PARA);
    CHK_PRT_RET(
        dataCount_ > UINT64_MAX / dataTypeSize_, HCCL_ERROR("[%s] data size overflows uint64.", __func__),
        HCCL_E_INTERNAL);
    dataSize_ = dataCount_ * dataTypeSize_;
    multipleDimensionSplitRatio_ = param.opConfig.multipleDimensionSplitRatio;
    multipleDimensionSplitRatioSource_ = param.opConfig.multipleDimensionSplitRatioSource;

    CHK_RET(GetHierarchyInfo(resCtx.topoInfo, resCtx.algHierarchyInfo, temp0HierarchyInfo_, temp1HierarchyInfo_));
    intraLocalRankSize_ = static_cast<u32>(GetRankSize(temp0HierarchyInfo_));
    interLocalRankSize_ = static_cast<u32>(GetRankSize(temp1HierarchyInfo_));
    rankSize_ = intraLocalRankSize_ * interLocalRankSize_;
    CHK_PRT_RET(
        intraLocalRankSize_ == 0 || interLocalRankSize_ == 0,
        HCCL_ERROR("[%s] hierarchy rank size is zero.", __func__), HCCL_E_PARA);

    InsAlgTemplate0 tempIntra(param, myRank_, temp0HierarchyInfo_);
    InsAlgTemplate1 tempInter(param, myRank_, temp1HierarchyInfo_);
    InsAlgTemplate2 tempIntra1(param, myRank_, temp0HierarchyInfo_);
    InsAlgTemplate3 tempInter1(param, myRank_, temp1HierarchyInfo_);
    CHK_RET(PrepareResForTemplate(tempIntra, tempInter, tempIntra1, tempInter1));
    return GenInsQues(param, resCtx, tempIntra, tempInter, tempIntra1, tempInter1);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    GenInsQues(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, InsAlgTemplate0& tempAlgIntra,
        InsAlgTemplate1& tempAlgInter, InsAlgTemplate2& tempAlgIntra1, InsAlgTemplate3& tempAlgInter1)
{
    std::vector<float> splitDataSize;
    GetParallelDataSplit(splitDataSize);
    u32 multipleIntra = static_cast<u32>(tempAlgIntra.CalcScratchMultiple(BufferType::INPUT, BufferType::OUTPUT));
    u32 multipleInter = static_cast<u32>(tempAlgInter.CalcScratchMultiple(BufferType::INPUT, BufferType::OUTPUT));
    if (multipleIntra > 0 || multipleInter > 0) {
        multipleIntra = 2;
        multipleInter = 2;
    }
    const float multiple0 = splitDataSize[0] * multipleIntra + splitDataSize[1] * multipleInter;
    const float multiple1 = splitDataSize[1] * multipleIntra + splitDataSize[0] * multipleInter;
    const float multiple = std::max(multiple0, multiple1);

    const u64 sliceCountUB = std::min(static_cast<u64>(UB_MAX_DATA_SIZE) / dataTypeSize_, dataCount_);
    const float sliceCountPercent = std::max(
        splitDataSize[0] / static_cast<float>(intraLocalRankSize_),
        splitDataSize[1] / static_cast<float>(interLocalRankSize_));
    const u64 sliceCountUB0 = sliceCountPercent > 0 ?
                                  static_cast<u64>(std::floor(static_cast<double>(sliceCountUB) / sliceCountPercent)) :
                                  sliceCountUB;
    u64 sliceCount = sliceCountUB;
    if (multiple > 0 && maxTmpMemSize_ > 0) {
        const u64 scratchCount = maxTmpMemSize_ / dataTypeSize_;
        sliceCount = std::min(static_cast<u64>(std::floor(scratchCount / multiple)), sliceCountUB0);
    }
    if (sliceCount == 0) {
        return dataCount_ == 0 ? HCCL_SUCCESS : HCCL_E_INTERNAL;
    }

    const u64 alignSize = AICPU_ALIGN_SIZE;
    const u64 sliceCountPart0 = static_cast<u64>(std::floor(sliceCount * splitDataSize[0]));
    const u32 loopTimes = static_cast<u32>(dataCount_ / sliceCount + (dataCount_ % sliceCount != 0));
    const u64 scratchOffsetCountIntraStage0 = 0;
    const u64 scratchOffsetCountInterStage0 = sliceCountPart0 * multipleIntra;
    const u64 scratchOffsetCountInterStage1 = 0;
    const u64 scratchOffsetCountIntraStage1 = sliceCountPart0 * multipleInter;

    TemplateResource intraResource;
    TemplateResource interResource;
    u64 processedCount = 0;
    u32 loopIndex = 0;
    while (processedCount < dataCount_) {
        // Stage 3/4 use a different template-main layout; restore the RS layout for each new loop.
        CHK_RET(PrepareResForTemplate(tempAlgIntra, tempAlgInter, tempAlgIntra1, tempAlgInter1));
        CHK_RET(PrepareTemplateResource(resCtx, intraResource, interResource));
        const u64 remainingCount = dataCount_ - processedCount;
        const u32 remainingLoopTimes = loopIndex < loopTimes ? loopTimes - loopIndex : 1;
        u64 currCount = (remainingCount + remainingLoopTimes - 1) / remainingLoopTimes;
        currCount = std::min(currCount, sliceCount);
        u64 currCountPart0 = static_cast<u64>(std::floor(currCount * splitDataSize[0]));
        u64 currCountPart1 = currCount - currCountPart0;
        if (remainingLoopTimes > 1) {
            const u64 alignedCountPart0 = currCountPart0 * dataTypeSize_ / alignSize * alignSize / dataTypeSize_;
            const u64 alignedCountPart1 = currCountPart1 * dataTypeSize_ / alignSize * alignSize / dataTypeSize_;
            if (alignedCountPart0 + alignedCountPart1 > 0) {
                currCountPart0 = alignedCountPart0;
                currCountPart1 = alignedCountPart1;
            }
        }
        CHK_PRT_RET(
            currCountPart0 + currCountPart1 == 0, HCCL_ERROR("[%s] current count is zero.", __func__), HCCL_E_INTERNAL);
        const u64 dataOffset0 = processedCount * dataTypeSize_;
        const u64 dataOffset1 = dataOffset0 + currCountPart0 * dataTypeSize_;

        tempVirtRankMapInter_.clear();
        tempVirtRankMapIntra_.clear();
        for (u32 i = 0; i < temp0HierarchyInfo_[0].size(); ++i) {
            tempVirtRankMapInter_[temp0HierarchyInfo_[0][i]] = i;
        }
        for (u32 i = 0; i < temp1HierarchyInfo_[0].size(); ++i) {
            tempVirtRankMapIntra_[temp1HierarchyInfo_[0][i]] = i;
        }
        CalcIntraDataAllRank(currCountPart0, intraLocalRankSize_, interLocalRankSize_, meshPartDataMap_);
        CalcInterDataAllRank(currCountPart1, interLocalRankSize_, intraLocalRankSize_, nhrPartDataMap_);

        CHK_RET(PreSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnTemplates_));
        TemplateDataParams paramsIntra0;
        TemplateDataParams paramsInter1;
        CHK_RET(RunTemplateIntra0(
            param, resCtx, dataOffset0, currCountPart0, scratchOffsetCountIntraStage0, paramsIntra0, intraResource,
            tempAlgIntra));
        CHK_RET(RunTemplateInter1(
            param, resCtx, dataOffset1, currCountPart1, scratchOffsetCountInterStage0, paramsInter1, interResource,
            tempAlgInter));
        CHK_RET(PostSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnMain_));

        CHK_RET(PreSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnTemplates_));
        TemplateDataParams paramsInter0;
        TemplateDataParams paramsIntra1;
        CHK_RET(RunTemplateInter0(
            param, resCtx, dataOffset0, currCountPart0, scratchOffsetCountInterStage1, paramsInter0, interResource,
            tempAlgInter));
        CHK_RET(RunTemplateIntra1(
            param, resCtx, dataOffset1, currCountPart1, scratchOffsetCountIntraStage1, paramsIntra1, intraResource,
            tempAlgIntra));
        CHK_RET(PostSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnMain_));

        CHK_RET(PrepareResForTemplate23(tempAlgIntra, tempAlgIntra1, tempAlgInter1));
        CHK_RET(PrepareTemplateResource(resCtx, intraResource, interResource));
        CHK_RET(PreSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnTemplates_));
        TemplateDataParams paramsInter01;
        TemplateDataParams paramsIntra11;
        CHK_RET(RunTemplateInter01(
            param, resCtx, dataOffset0, currCountPart0, scratchOffsetCountInterStage1, paramsInter01, interResource,
            tempAlgInter1));
        CHK_RET(RunTemplateIntra11(
            param, resCtx, dataOffset1, currCountPart1, scratchOffsetCountIntraStage1, paramsIntra11, intraResource,
            tempAlgIntra1));
        CHK_RET(PostSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnMain_));

        CHK_RET(PreSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnTemplates_));
        TemplateDataParams paramsIntra01;
        TemplateDataParams paramsInter11;
        CHK_RET(RunTemplateIntra01(
            param, resCtx, dataOffset0, currCountPart0, scratchOffsetCountIntraStage0, paramsIntra01, intraResource,
            tempAlgIntra1));
        CHK_RET(RunTemplateInter11(
            param, resCtx, dataOffset1, currCountPart1, scratchOffsetCountInterStage0, paramsInter11, interResource,
            tempAlgInter1));
        CHK_RET(PostSyncInterThreads(mainThread_, templateMainThreads_, syncNotifyOnMain_));

        processedCount += currCountPart0 + currCountPart1;
        ++loopIndex;
    }
    return HCCL_SUCCESS;
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateIntra0(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate0& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    GenDataParamsBufferType(BufferType::INPUT, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset, currCountPart, scratchOffsetCount, dataParams, intraLocalRankSize_, 0, 0,
        currCountPart * dataTypeSize_);
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateInter1(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate1& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    GenDataParamsBufferType(BufferType::INPUT, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset, currCountPart, scratchOffsetCount, dataParams, interLocalRankSize_, 0, 0, 0);
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateInter0(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate1& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    PrepareDataParamsInter(dataOffset, currCountPart, scratchOffsetCount);
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset0Inter_, currCountPart0_, scratchOffsetCountInterStage1_, dataParams,
        interLocalRankSize_, 0, 0, 0);
    const u64 rankOffset = rankBaseOffInterAGMap_.at(myRank_);
    dataParams.buffInfo.inBuffBaseOff += rankOffset;
    dataParams.buffInfo.hcclBuffBaseOff += rankOffset;
    dataParams.buffInfo.outBuffBaseOff += rankOffset;
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateIntra1(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate0& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    PrepareDataParamsIntra(dataOffset, currCountPart, scratchOffsetCount);
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset0Intra_, currCountPart1_, scratchOffsetCountIntraStage1_, dataParams,
        intraLocalRankSize_, 0, 0, currCountPart * dataTypeSize_);
    const u64 rankOffset = rankBaseOffIntraAGMap_.at(myRank_);
    dataParams.buffInfo.inBuffBaseOff += rankOffset;
    dataParams.buffInfo.hcclBuffBaseOff += rankOffset;
    dataParams.buffInfo.outBuffBaseOff += rankOffset;
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateInter01(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate3& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    PrepareDataParamsInter(dataOffset, currCountPart, scratchOffsetCount);
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset0Inter_, currCountPart0_, scratchOffsetCountInterStage1_, dataParams,
        interLocalRankSize_, 0, 0, 0);
    const u64 rankOffset = rankBaseOffInterAGMap_.at(myRank_);
    dataParams.buffInfo.inBuffBaseOff += rankOffset;
    dataParams.buffInfo.hcclBuffBaseOff += rankOffset;
    dataParams.buffInfo.outBuffBaseOff += rankOffset;
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateIntra11(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate2& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    PrepareDataParamsIntra(dataOffset, currCountPart, scratchOffsetCount);
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset0Intra_, currCountPart1_, scratchOffsetCountIntraStage1_, dataParams,
        intraLocalRankSize_, 0, 0, 0);
    const u64 rankOffset = rankBaseOffIntraAGMap_.at(myRank_);
    dataParams.buffInfo.inBuffBaseOff += rankOffset;
    dataParams.buffInfo.hcclBuffBaseOff += rankOffset;
    dataParams.buffInfo.outBuffBaseOff += rankOffset;
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateIntra01(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate2& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::OUTPUT, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset, currCountPart, scratchOffsetCount, dataParams, intraLocalRankSize_, 0, 0, 0);
    return tempAlg.KernelRun(param, dataParams, resource);
}

template <
    typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1, typename InsAlgTemplate2,
    typename InsAlgTemplate3>
HcclResult InsAllReduceParallelExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1, InsAlgTemplate2, InsAlgTemplate3>::
    RunTemplateInter11(
        const OpParam& param, const AlgResourceCtxSerializable& resCtx, u64 dataOffset, u64 currCountPart,
        u64 scratchOffsetCount, TemplateDataParams& dataParams, TemplateResource& resource, InsAlgTemplate3& tempAlg)
{
    if (currCountPart == 0) {
        return HCCL_SUCCESS;
    }
    GenDataParamsBufferType(BufferType::HCCL_BUFFER, BufferType::OUTPUT, BufferType::HCCL_BUFFER, dataParams);
    GenDataParams(
        param, resCtx, dataOffset, currCountPart, scratchOffsetCount, dataParams, interLocalRankSize_, 0, 0, 0);
    return tempAlg.KernelRun(param, dataParams, resource);
}

REGISTER_EXECUTOR_BY_FOUR_TEMPS(
    HcclCMDType::HCCL_CMD_ALLREDUCE, AicpuAllReduceParallelMeshNHR, InsAllReduceParallelExecutor, TopoMatchMultilevel,
    InsTempReduceScatterMesh1D, InsTempReduceScatterNHR, InsTempAllGatherMesh1D, InsTempAllGatherNHR);

} // namespace mc2_ops_hccl
