/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ins_v2_all_to_all_concurrent_executor.h"
#include "channel.h"
#include "hccl_alloc_ctx_res.h"

#include "ins_temp_all_to_all_v_mesh_1D.h"
#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
#include "ccu_temp_kfc_all_to_all_mesh1d_multi_jetty_mem2mem.h"
#endif

namespace mc2_ops_hccl {
using namespace std;
constexpr uint32_t CONCURRENT_NUM = 2;
constexpr uint32_t CONST_0 = 0;
constexpr uint32_t CONST_1 = 1;
constexpr uint32_t MIN_SUBGROUP_NUM = 2;
constexpr u32 MESH_BW = 100;
constexpr u32 CLOS_BW = 113;

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::InsV2AllToAllConcurrentExecutor()
{}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::InitCommInfo(
    const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    myRank_ = topoInfo->userRank;
    rankSize_ = topoInfo->userRankSize;
    dataType_ = param.all2AllVDataDes.sendType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataType_];
    algHierarchyInfo_ = algHierarchyInfo;
    HCCL_INFO(
        "[InsV2AllToAllConcurrentExecutor][InitCommInfo] myRank = %u, rankSize = %u, "
        "dataType = %u, dataTypeSize = %u",
        myRank_, rankSize_, dataType_, dataTypeSize_);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::CalcAlgHierarchyInfo(
    HcclComm comm, TopoInfoWithNetLayerDetails* topoInfo, AlgHierarchyInfoForAllLevel& algHierarchyInfo)
{
    myRank_ = topoInfo->userRank;
    rankSize_ = topoInfo->userRankSize;
    AlgTopoMatch topoMatch;
    CHK_RET(topoMatch.MatchTopo(comm, topoInfo, algHierarchyInfo));
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::CalcRes(
    HcclComm comm, const OpParam& param, const TopoInfoWithNetLayerDetails* topoInfo,
    const AlgHierarchyInfoForAllLevel& algHierarchyInfo, AlgResourceRequest& resourceRequest)
{
    CHK_RET(InitCommInfo(param, topoInfo, algHierarchyInfo));
    CHK_PRT_RET(
        algHierarchyInfo.infos.empty() || algHierarchyInfo.infos[0].size() < MIN_SUBGROUP_NUM,
        HCCL_ERROR("[%s] algHierarchyInfo.infos[0] is invalid (empty or size < 2).", __func__), HCCL_E_PARA);

    std::vector<std::vector<u32>> subCommRanks0 = {algHierarchyInfo.infos[0][0]};
    std::vector<std::vector<u32>> subCommRanks1 = {algHierarchyInfo.infos[0][1]};

    std::shared_ptr<InsAlgTemplate0> temp0Alg
        = std::make_shared<InsAlgTemplate0>(param, topoInfo->userRank, subCommRanks0);
    std::shared_ptr<InsAlgTemplate1> temp1Alg
        = std::make_shared<InsAlgTemplate1>(param, topoInfo->userRank, subCommRanks1);

    AlgResourceRequest resReq0, resReq1;
    CHK_RET(temp0Alg->CalcRes(comm, param, topoInfo, resReq0));
    CHK_RET(temp1Alg->CalcRes(comm, param, topoInfo, resReq1));

    resourceRequest.slaveThreadNum = resReq0.slaveThreadNum + resReq1.slaveThreadNum + 1;
    resourceRequest.notifyNumOnMainThread = resReq0.notifyNumOnMainThread + 1;
    resourceRequest.notifyNumPerThread.reserve(
        resReq0.notifyNumPerThread.size() + resReq1.notifyNumPerThread.size() + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), resReq0.notifyNumPerThread.begin(), resReq0.notifyNumPerThread.end());
    resourceRequest.notifyNumPerThread.emplace_back(resReq1.notifyNumOnMainThread + 1);
    resourceRequest.notifyNumPerThread.insert(
        resourceRequest.notifyNumPerThread.end(), resReq1.notifyNumPerThread.begin(), resReq1.notifyNumPerThread.end());

    if (param.engine == CommEngine::COMM_ENGINE_CCU) {
        std::vector<HcclChannelDesc> channelDescs0, channelDescs1;
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks0, channelDescs0, CommTopo::COMM_TOPO_1DMESH));
        CHK_RET(CalcChannelRequestMeshClosMultiJetty(comm, param, topoInfo, subCommRanks1, channelDescs1, false, false));
        resReq0.ccuKernelInfos[0].channels = channelDescs0;
        resReq1.ccuKernelInfos[0].channels = channelDescs1;
        resourceRequest.ccuKernelNum.push_back(resReq0.ccuKernelNum[0]);
        resourceRequest.ccuKernelNum.push_back(resReq1.ccuKernelNum[0]);
        resourceRequest.ccuKernelInfos.insert(
            resourceRequest.ccuKernelInfos.end(), resReq0.ccuKernelInfos.begin(), resReq0.ccuKernelInfos.end());
        resourceRequest.ccuKernelInfos.insert(
            resourceRequest.ccuKernelInfos.end(), resReq1.ccuKernelInfos.begin(), resReq1.ccuKernelInfos.end());
    } else {
        std::vector<HcclChannelDesc> channelDescs0, channelDescs1;
        CHK_RET(CalcChannelRequestMesh1DWithPriorityTopo(
            comm, param, topoInfo, subCommRanks0, channelDescs0, CommTopo::COMM_TOPO_1DMESH));
        CHK_RET(CalcChannelRequestMeshClosMultiJetty(comm, param, topoInfo, subCommRanks1, channelDescs1, false, false));
        resourceRequest.channels.resize(1);
        resourceRequest.channels[0].insert(
            resourceRequest.channels[0].end(), channelDescs0.begin(), channelDescs0.end());
        resourceRequest.channels[0].insert(
            resourceRequest.channels[0].end(), channelDescs1.begin(), channelDescs1.end());
    }
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::FillTemplateResource(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, TemplateResource& templateAlgRes, uint32_t index)
{
    if (param.engine != CommEngine::COMM_ENGINE_CCU && param.engine != CommEngine::COMM_ENGINE_AICPU &&
        param.engine != CommEngine::COMM_ENGINE_AICPU_TS) {
        HCCL_ERROR("[%s] unsupported engine.", __func__);
        return HCCL_E_PARA;
    }
    if (param.engine == CommEngine::COMM_ENGINE_AICPU || param.engine == CommEngine::COMM_ENGINE_AICPU_TS) {
        templateAlgRes.threads
            = {resCtx.threads.begin() + index * rankSize_, resCtx.threads.begin() + (index + 1) * rankSize_};
        templateAlgRes.channels = remoteRankToChannelInfo_[index];
    } else {
        CHK_PRT_RET(
            resCtx.threads.size() < MIN_SUBGROUP_NUM || resCtx.ccuKernels.size() < MIN_SUBGROUP_NUM,
            HCCL_ERROR(
                "[%s] resCtx resource not enough. threads.size[%zu], ccuKernels.size[%zu].", __func__,
                resCtx.threads.size(), resCtx.ccuKernels.size()),
            HCCL_E_INTERNAL);
        templateAlgRes.threads = {resCtx.threads[index]};
        templateAlgRes.ccuKernels = {resCtx.ccuKernels[index]};
        templateAlgRes.aivCommInfoPtr = resCtx.aivCommInfoPtr;
    }
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::InitTemplateDataParams(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, TemplateDataParams& tempAlgParams) const
{
    tempAlgParams.buffInfo.inputPtr = param.inputPtr;
    tempAlgParams.buffInfo.outputPtr = param.outputPtr;
    tempAlgParams.buffInfo.inputSize = param.inputSize;
    tempAlgParams.buffInfo.outputSize = param.outputSize;
    tempAlgParams.buffInfo.hcclBuff = resCtx.cclMem;
    tempAlgParams.buffInfo.inBuffType = BufferType::INPUT;
    tempAlgParams.buffInfo.outBuffType = BufferType::OUTPUT;
    tempAlgParams.buffInfo.hcclBuffType = BufferType::HCCL_BUFFER;
    tempAlgParams.sendCounts.resize(rankSize_, 0);
    tempAlgParams.recvCounts.resize(rankSize_, 0);
    tempAlgParams.sdispls.resize(rankSize_, 0);
    tempAlgParams.rdispls.resize(rankSize_, 0);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::RestoreSendRecvData(
    const OpParam& param)
{
    const u64* data = reinterpret_cast<const u64*>(param.varData);
    for (u64 i = 0; i < ALL_TO_ALL_V_VECTOR_NUM * rankSize_; i++) {
        u64 val = i / rankSize_;
        switch (val) {
            case CONST_0:
                sendCounts_.push_back(data[i]);
                break;
            case CONST_1:
                recvCounts_.push_back(data[i]);
                break;
            default:
                break;
        }
    }
    sdispls_.resize(rankSize_, 0);
    rdispls_.resize(rankSize_, 0);
    ConvertAlltoAllParam(recvCounts_[myRank_], rankSize_, sdispls_, rdispls_);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::SplitSendRecvData(
    const OpParam& param, std::vector<SendRecvData>& splitData)
{
    splitData.resize(CONCURRENT_NUM);
    for (u32 i = 0; i < CONCURRENT_NUM; i++) {
        splitData[i].sendCounts.resize(rankSize_);
        splitData[i].recvCounts.resize(rankSize_);
        splitData[i].sdispls.resize(rankSize_);
        splitData[i].rdispls.resize(rankSize_);
    }

    uint32_t factorMesh = MESH_BW;
    uint32_t factorClos = CLOS_BW;
    if (param.engine == CommEngine::COMM_ENGINE_AICPU_TS) {
        factorMesh = 10;
        factorClos = 12;
    }
    uint32_t factor = factorMesh + factorClos;
    for (u64 i = 0; i < rankSize_; i++) {
        uint64_t sendQuotient = sendCounts_[i] / factor;
        uint64_t recvQuotient = recvCounts_[i] / factor;
        uint64_t sendCountsMesh = sendQuotient * factorMesh;
        uint64_t recvCountsMesh = recvQuotient * factorMesh;
        splitData[0].sendCounts[i] = sendCountsMesh;
        splitData[1].sendCounts[i] = sendCounts_[i] - sendCountsMesh;
        splitData[0].recvCounts[i] = recvCountsMesh;
        splitData[1].recvCounts[i] = recvCounts_[i] - recvCountsMesh;
        splitData[0].sdispls[i] = sdispls_[i];
        splitData[1].sdispls[i] = sdispls_[i] + sendCountsMesh;
        splitData[0].rdispls[i] = rdispls_[i];
        splitData[1].rdispls[i] = rdispls_[i] + recvCountsMesh;
    }
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::GetMaxSendRecvDataCount(
    u64& maxSendRecvDataCount, const SendRecvData& splitData) const
{
    u64 max = 0;
    for (u64 i = 0; i < rankSize_; i++) {
        max = std::max(max, splitData.sendCounts[i]);
        max = std::max(max, splitData.recvCounts[i]);
    }
    maxSendRecvDataCount = max;
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
void InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::GenTemplateAlgParams(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, const u64 dataOffset, const u64 dataCountPerLoop,
    TemplateDataParams& tempAlgParams) const
{
    (void)param;
    (void)resCtx;
    tempAlgParams.count = dataCountPerLoop;
    tempAlgParams.sliceSize = dataCountPerLoop * dataTypeSize_;
    tempAlgParams.tailSize = tempAlgParams.sliceSize;
    tempAlgParams.buffInfo.inBuffBaseOff = dataOffset;
    tempAlgParams.buffInfo.outBuffBaseOff = dataOffset;
    tempAlgParams.inputSliceStride = 0;
    tempAlgParams.outputSliceStride = dataCountPerLoop * dataTypeSize_;
    tempAlgParams.repeatNum = 1;
    tempAlgParams.inputRepeatStride = 0;
    tempAlgParams.outputRepeatStride = 0;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::CalcMaxDataCountPerLoop(
    const OpParam& param, const std::vector<u64> scratchMulti, std::vector<u64>& maxDataCountPerLoop) const
{
    u64 transportBoundDataSize = UB_MAX_DATA_SIZE;
    u64 scratchMultiSum = 0;
    for (const auto& x : scratchMulti) {
        scratchMultiSum += x;
    }
    for (u32 i = 0; i < scratchMulti.size(); i++) {
        u64 maxDataSizePerLoop;
        if ((scratchMulti[i] != 0) && (scratchMultiSum != 0)) {
            u64 scratchBoundDataSize = maxTmpMemSize_ / scratchMultiSum / HCCL_MIN_SLICE_ALIGN * HCCL_MIN_SLICE_ALIGN;
            maxDataSizePerLoop = std::min(transportBoundDataSize, scratchBoundDataSize);
        } else {
            maxDataSizePerLoop = transportBoundDataSize;
        }
        if (param.engine == CommEngine::COMM_ENGINE_AICPU || param.engine == CommEngine::COMM_ENGINE_AICPU_TS) {
            maxDataCountPerLoop[i] = maxDataSizePerLoop / dataTypeSize_ / rankSize_;
        } else {
            maxDataCountPerLoop[i] = maxDataSizePerLoop / dataTypeSize_;
        }
        CHK_PRT_RET(
            maxDataCountPerLoop[i] == 0,
            HCCL_ERROR("[InsV2AllToAllConcurrentExecutor] maxDataCountPerLoop[%u] is 0.", i),
            HCCL_E_INTERNAL);
    }
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::SetTemplateDataParams(
    TemplateDataParams& tempAlgParams, const SendRecvData& splitData, u32 loop, u64 currDataCount,
    u64 processedDataCount, u64 maxDataCountPerLoop) const
{
    (void)loop;
    tempAlgParams.count = currDataCount;
    tempAlgParams.buffInfo.inBuffBaseOff = (splitData.sdispls[0] + processedDataCount) * dataTypeSize_;
    tempAlgParams.buffInfo.outBuffBaseOff = (splitData.rdispls[0] + processedDataCount) * dataTypeSize_;
    tempAlgParams.sliceSize = currDataCount * dataTypeSize_;
    tempAlgParams.tailSize = tempAlgParams.sliceSize;
    tempAlgParams.inputSliceStride = maxDataCountPerLoop * dataTypeSize_;
    tempAlgParams.outputSliceStride = maxDataCountPerLoop * dataTypeSize_;

    for (u64 i = 0; i < rankSize_; i++) {
        if (splitData.sendCounts[i] > processedDataCount) {
            tempAlgParams.sendCounts[i] = std::min(currDataCount, splitData.sendCounts[i] - processedDataCount);
            tempAlgParams.sdispls[i] = splitData.sdispls[i] + processedDataCount;
        } else {
            tempAlgParams.sendCounts[i] = 0;
            tempAlgParams.sdispls[i] = splitData.sdispls[i] + splitData.sendCounts[i];
        }
        if (splitData.recvCounts[i] > processedDataCount) {
            tempAlgParams.recvCounts[i] = std::min(currDataCount, splitData.recvCounts[i] - processedDataCount);
            tempAlgParams.rdispls[i] = splitData.rdispls[i] + processedDataCount;
        } else {
            tempAlgParams.recvCounts[i] = 0;
            tempAlgParams.rdispls[i] = splitData.rdispls[i] + splitData.recvCounts[i];
        }
    }
    tempAlgParams.repeatNum = 1;
    tempAlgParams.inputRepeatStride = 0;
    tempAlgParams.outputRepeatStride = 0;
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::Orchestrate(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[InsV2AllToAllConcurrentExecutor][Orchestrate] Start");
    maxTmpMemSize_ = resCtx.cclMem.size;
    threads_ = resCtx.threads;
    if (param.engine == CommEngine::COMM_ENGINE_AICPU || param.engine == CommEngine::COMM_ENGINE_AICPU_TS) {
        if (resCtx.topoInfo.level0Topo == Level0Shape::MESH_1D_CLOS && !resCtx.topoInfo.level0PcieMix) {
            CHK_PRT_RET(
                resCtx.channels.size() != CONST_1,
                HCCL_ERROR("[%s] resCtx.channels.size[%zu] is not 1.", __func__, resCtx.channels.size()),
                HCCL_E_PARA);
            remoteRankToChannelInfo_.resize(CONCURRENT_NUM);
            size_t sizePerTemplate = resCtx.channels[0].size() / CONCURRENT_NUM;
            for (size_t i = 0; i < resCtx.channels[0].size(); i++) {
                auto& channel = resCtx.channels[0][i];
                u32 idx = (i < sizePerTemplate) ? CONST_0 : CONST_1;
                remoteRankToChannelInfo_[idx][channel.remoteRank].push_back(channel);
            }
        } else {
            CHK_RET(RestoreChannelMap(resCtx, remoteRankToChannelInfo_));
        }
    }

    dataType_ = param.all2AllVDataDes.sendType;
    dataTypeSize_ = DATATYPE_SIZE_TABLE[dataType_];
    rankSize_ = resCtx.topoInfo.userRankSize;
    myRank_ = resCtx.topoInfo.userRank;
    algHierarchyInfo_ = resCtx.algHierarchyInfo;
    HcclResult ret = OrchestrateLoop(param, resCtx);
    CHK_PRT_RET(
        ret != HCCL_SUCCESS,
        HCCL_ERROR("[%s] AllToAll executor kernel run failed, errNo[0x%016llx]", __func__, HCCL_ERROR_CODE(ret)),
        ret);
    return HCCL_SUCCESS;
}

template <typename AlgTopoMatch, typename InsAlgTemplate0, typename InsAlgTemplate1>
HcclResult InsV2AllToAllConcurrentExecutor<AlgTopoMatch, InsAlgTemplate0, InsAlgTemplate1>::OrchestrateLoop(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx)
{
    HCCL_INFO("[InsV2AllToAllConcurrentExecutor][OrchestrateLoop] Start");
    CHK_PRT_RET(
        param.varMemSize != ALL_TO_ALL_V_VECTOR_NUM * rankSize_ * sizeof(u64),
        HCCL_ERROR("[%s] param.varMemSize[%llu] is invalid.", __func__, param.varMemSize),
        HCCL_E_PARA);

    CHK_PRT_RET(
        algHierarchyInfo_.infos.empty() || algHierarchyInfo_.infos[0].size() < MIN_SUBGROUP_NUM,
        HCCL_ERROR("[%s] algHierarchyInfo.infos[0] is invalid.", __func__), HCCL_E_PARA);
    std::vector<std::vector<u32>> subCommRanks0 = {algHierarchyInfo_.infos[0][0]};
    std::vector<std::vector<u32>> subCommRanks1 = {algHierarchyInfo_.infos[0][1]};

    TemplateResource templateAlgRes0, templateAlgRes1;
    CHK_RET(FillTemplateResource(param, resCtx, templateAlgRes0, 0));
    CHK_RET(FillTemplateResource(param, resCtx, templateAlgRes1, 1));

    std::vector<SendRecvData> splitData;
    CHK_RET(RestoreSendRecvData(param));
    CHK_RET(SplitSendRecvData(param, splitData));

    u64 maxSendOrRecvDataCount0, maxSendOrRecvDataCount1;
    GetMaxSendRecvDataCount(maxSendOrRecvDataCount0, splitData[0]);
    GetMaxSendRecvDataCount(maxSendOrRecvDataCount1, splitData[1]);

    std::shared_ptr<InsAlgTemplate0> algTemplate0
        = std::make_shared<InsAlgTemplate0>(param, resCtx.topoInfo.userRank, subCommRanks0);
    std::shared_ptr<InsAlgTemplate1> algTemplate1
        = std::make_shared<InsAlgTemplate1>(param, resCtx.topoInfo.userRank, subCommRanks1);

    TemplateDataParams tempAlgParams0, tempAlgParams1;
    InitTemplateDataParams(param, resCtx, tempAlgParams0);
    InitTemplateDataParams(param, resCtx, tempAlgParams1);

    std::vector<u64> scratchMulti;
    scratchMulti.push_back(
        algTemplate0->CalcScratchMultiple(tempAlgParams0.buffInfo.inBuffType, tempAlgParams0.buffInfo.outBuffType));
    scratchMulti.push_back(
        algTemplate1->CalcScratchMultiple(tempAlgParams1.buffInfo.inBuffType, tempAlgParams1.buffInfo.outBuffType));
    std::vector<u64> maxDataCountPerLoop(CONCURRENT_NUM, 1);
    CHK_RET(CalcMaxDataCountPerLoop(param, scratchMulti, maxDataCountPerLoop));

    tempAlgParams0.buffInfo.hcclBuffBaseOff = 0;
    tempAlgParams1.buffInfo.hcclBuffBaseOff = scratchMulti[0] * maxDataCountPerLoop[0] * dataTypeSize_;

    u64 loopTimes0 = (maxSendOrRecvDataCount0 + maxDataCountPerLoop[0] - 1) / maxDataCountPerLoop[0];
    u64 loopTimes1 = (maxSendOrRecvDataCount1 + maxDataCountPerLoop[1] - 1) / maxDataCountPerLoop[1];

    std::vector<u64> processedDataCount = {0, 0};
    u32 notifyPreSyncIdx = resCtx.slaveThreadNum - resCtx.notifyNumOnMainThread;
    u32 notifyPostSyncIdx = resCtx.notifyNumOnMainThread - 1;
    std::vector<u32> notifyIdxesMainToSub = {notifyPreSyncIdx};
    std::vector<u32> notifyIdxesSubToMain = {notifyPostSyncIdx};

    CHK_RET(PreSyncInterThreads(templateAlgRes0.threads[0], {templateAlgRes1.threads[0]}, notifyIdxesMainToSub));
    u64 loop = 0;
    while (loop < loopTimes0 || loop < loopTimes1) {
        if (loop < loopTimes0) {
            u64 currDataCount = (loop == loopTimes0 - 1) ? (maxSendOrRecvDataCount0 - processedDataCount[0]) :
                                                           (maxDataCountPerLoop[0]);
            SetTemplateDataParams(
                tempAlgParams0, splitData[0], loop, currDataCount, processedDataCount[0], maxDataCountPerLoop[0]);
            CHK_RET(algTemplate0->KernelRun(param, tempAlgParams0, templateAlgRes0));
            processedDataCount[0] += currDataCount;
        }
        if (loop < loopTimes1) {
            u64 currDataCount = (loop == loopTimes1 - 1) ? (maxSendOrRecvDataCount1 - processedDataCount[1]) :
                                                           (maxDataCountPerLoop[1]);
            SetTemplateDataParams(
                tempAlgParams1, splitData[1], loop, currDataCount, processedDataCount[1], maxDataCountPerLoop[1]);
            CHK_RET(algTemplate1->KernelRun(param, tempAlgParams1, templateAlgRes1));
            processedDataCount[1] += currDataCount;
        }
        loop++;
    }
    CHK_RET(PostSyncInterThreads(templateAlgRes0.threads[0], {templateAlgRes1.threads[0]}, notifyIdxesSubToMain));
    HCCL_INFO("[InsV2AllToAllConcurrentExecutor][OrchestrateLoop] End.");
    return HCCL_SUCCESS;
}

REGISTER_EXECUTOR_BY_TWO_TEMPS(
    HcclCMDType::HCCL_CMD_ALLTOALL, AicpuAllToAllSoleMeshConcurrent, InsV2AllToAllConcurrentExecutor, TopoMatchUBX,
    InsTempAlltoAllVMesh1D, InsTempAlltoAllVMesh1D);

#if !defined(AICPU_COMPILE) && MC2_CLIENT_ENABLE_CCU
REGISTER_EXECUTOR_BY_TWO_TEMPS(
    HcclCMDType::HCCL_CMD_ALLTOALL, CcuSchedAllToAllSoleMeshConcurrent, InsV2AllToAllConcurrentExecutor, TopoMatchUBX,
    CcuTempKfcAllToAllMesh1DMultiJettyMem2Mem, CcuTempKfcAllToAllMesh1DMultiJettyMem2Mem);
#endif

} // namespace mc2_ops_hccl
