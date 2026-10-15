/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cstddef>
#include <limits>
#include <map>
#include <set>
#include "comm_kfc_open_kernel_adapter.h"
#include "ins_v2_all_gather_sole_executor.h"
#include "ins_temp_all_gather_nhr.h"
#include "topo_match_1d.h"
#include <memory>
#include <string>
#include <vector>

#include "log.h"
#include "alg_param.h"
#include "alg_data_trans_wrapper.h"
#include "coll_alg_v2_exec_registry.h"
#include "cann_dispatch_bridge.h"
#include "cann_host_bridge.h"
#include "hcomm_primitives.h"
#include "task_cache/mc2_aicpu_task_cache.h"
#include "task_cache/mc2_aicpu_task_cache_runner.h"

namespace {
mc2_ops_hccl::OpParam* AsOpenOpParam(std::vector<uint8_t>& opParam)
{
    return reinterpret_cast<mc2_ops_hccl::OpParam*>(opParam.data());
}

u64 GetDataTypeSize(HcclDataType dataType)
{
    switch (dataType) {
        case HCCL_DATA_TYPE_INT8:
        case HCCL_DATA_TYPE_UINT8:
        case HCCL_DATA_TYPE_HIF8:
        case HCCL_DATA_TYPE_FP8E5M2:
        case HCCL_DATA_TYPE_FP8E4M3:
        case HCCL_DATA_TYPE_FP8E8M0:
        case HCCL_DATA_TYPE_MXFP8:
            return 1U;
        case HCCL_DATA_TYPE_INT16:
        case HCCL_DATA_TYPE_UINT16:
        case HCCL_DATA_TYPE_FP16:
        case HCCL_DATA_TYPE_BFP16:
            return 2U;
        case HCCL_DATA_TYPE_INT32:
        case HCCL_DATA_TYPE_UINT32:
        case HCCL_DATA_TYPE_FP32:
            return 4U;
        case HCCL_DATA_TYPE_INT64:
        case HCCL_DATA_TYPE_UINT64:
        case HCCL_DATA_TYPE_FP64:
            return 8U;
        case HCCL_DATA_TYPE_INT128:
            return 16U;
        default:
            return 0U;
    }
}

HcclResult RestoreVarDataAlltoAllV(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    u64 rankSize = resCtx.topoInfo.userRankSize;
    CHK_PRT_RET(
        param.varMemSize != mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM * rankSize * sizeof(u64),
        HCCL_ERROR(
            "[RestoreVarDataAlltoAllV] param.varMemSize [%llu] is invalid,"
            " ALL_TO_ALL_V_VECTOR_NUM is [%u], rankSize is [%u], sizeof(u64) is [%u],",
            param.varMemSize, mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM, rankSize, sizeof(u64)),
        HCCL_E_PARA);

    HCCL_INFO(
        "[RestoreVarDataAlltoAllV] param.varMemSize [%llu],"
        " ALL_TO_ALL_V_VECTOR_NUM is [%u], rankSize is [%u], sizeof(u64) is [%u],",
        param.varMemSize, mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM, rankSize, sizeof(u64));
    for (uint32_t i = 0; i < rankSize; i++) {
        HCCL_INFO(
            "param.all2AllVDataDes.sendCounts[%d]:[%llu], param.all2AllVDataDes.recvCounts[%d]:[%llu], "
            "param.all2AllVDataDes.sdispls[%d]:[%llu], param.all2AllVDataDes.rdispls[%d]:[%llu]",
            i, reinterpret_cast<u64*>(param.all2AllVDataDes.sendCounts)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.recvCounts)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.sdispls)[i], i,
            reinterpret_cast<u64*>(param.all2AllVDataDes.rdispls)[i]);
    }
    constexpr u64 SEND_COUNT_IDX = 0;
    constexpr u64 RECV_COUNT_IDX = 1;
    constexpr u64 SEND_DISPL_IDX = 2;
    constexpr u64 RECV_DISPL_IDX = 3;

    u64* data = reinterpret_cast<u64*>(param.varData);
    for (u64 i = 0; i < mc2_ops_hccl::ALL_TO_ALL_V_VECTOR_NUM * rankSize; i++) {
        u64 val = i / rankSize;
        switch (val) {
            case SEND_COUNT_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.sendCounts)[i % rankSize];
                break;
            case RECV_COUNT_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.recvCounts)[i % rankSize];
                break;
            case SEND_DISPL_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.sdispls)[i % rankSize];
                break;
            case RECV_DISPL_IDX:
                data[i] = reinterpret_cast<u64*>(param.all2AllVDataDes.rdispls)[i % rankSize];
                break;
            default:
                break;
        }
    }

    return HCCL_SUCCESS;
}

HcclResult RestoreVarDataIfNeeded(mc2_ops_hccl::OpParam& param, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    HcclResult ret = HCCL_SUCCESS;
    if (param.opType == HCCL_CMD_ALLTOALLV || param.opType == HCCL_CMD_ALLTOALL) {
        ret = RestoreVarDataAlltoAllV(param, resCtx);
    }
    return ret;
}

} // namespace

namespace {
bool DisjointBuffers(const void* a, uint64_t aSize, const void* b, uint64_t bSize)
{
    const auto x = reinterpret_cast<uintptr_t>(a);
    const auto y = reinterpret_cast<uintptr_t>(b);
    if (x == 0U || y == 0U || aSize > UINTPTR_MAX - x || bSize > UINTPTR_MAX - y) {
        return false;
    }
    return x + aSize <= y || y + bSize <= x;
}
} // namespace

bool PrepareOpenBoundaryLayout(
    const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx, mc2_boundary::TurnPolicy& policy)
{
    policy.fallbackReason = "channel_layout";
    policy.communicationThreads = 0U;
    policy.activeThreads = 0U;
    if (resCtx.channels.size() != 1U || resCtx.channels[0].empty()) {
        return false;
    }
    std::map<uint32_t, std::vector<const mc2_ops_hccl::ChannelInfo*>> peers;
    for (const auto& channel : resCtx.channels[0]) {
        peers[channel.remoteRank].push_back(&channel);
    }
    // This helper is only used for the exact SoleNHR whitelist. Non-POD SoleNHR
    // intentionally retains the template's one-channel defaults.
    const uint32_t channels = resCtx.topoInfo.isPod ? peers.begin()->second.size() : 1U;
    if (channels == 0U || channels > 2U) {
        policy.fallbackReason = "unsupported_channel_count";
        return false;
    }
    uint64_t portCount = 0U;
    for (uint32_t c = 0U; c < channels; ++c) {
        portCount += peers.begin()->second[c]->portGroupSize;
    }
    if (portCount > UINT32_MAX) {
        policy.fallbackReason = "channel_partition_overflow";
        return false;
    }
    std::set<uint64_t> handles;
    for (const auto& peer : peers) {
        if (peer.second.size() < channels || (resCtx.topoInfo.isPod && peer.second.size() != channels)) {
            policy.fallbackReason = "uneven_channel_layout";
            return false;
        }
        for (uint32_t c = 0; c < channels; ++c) {
            const auto& channel = *peer.second[c];
            if (!channel.isValid || channel.handle == 0U || !handles.insert(channel.handle).second ||
                channel.protocol != COMM_PROTOCOL_UBC_CTP ||
                channel.portGroupSize != peers.begin()->second[c]->portGroupSize ||
                (channels > 1U && channel.portGroupSize == 0U)) {
                policy.fallbackReason = "channel_protocol_or_partition";
                return false;
            }
        }
    }
    policy.communicationThreads = channels;
    policy.activeThreads = channels * 2U;
    policy.fallbackReason = "thread_layout";
    if (resCtx.threads.size() < policy.activeThreads || resCtx.threads.size() > mc2_boundary::NOTIFY_LIMIT) {
        return false;
    }
    handles.clear();
    for (uint32_t t = 0; t < policy.activeThreads; ++t) {
        if (resCtx.threads[t] == 0U || !handles.insert(resCtx.threads[t]).second) {
            return false;
        }
    }
    policy.fallbackReason = "none";
    return true;
}

void PrepareOpenBoundaryStaticContext(
    const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx, const mc2_boundary::Config& config,
    OpenBoundaryStaticContext& context)
{
    context = OpenBoundaryStaticContext{};
    context.resource = &resCtx;
    context.mode = config.mode;
    context.totalTurns = config.totalTurns;
    if (!PrepareOpenBoundaryLayout(resCtx, context.layout)) {
        return;
    }
    uint32_t base = resCtx.notifyNumOnMainThread;
    for (const auto value : resCtx.notifyNumPerThread) {
        base = std::max(base, value);
    }
    uint32_t required = 0U;
    context.layout.fallbackReason = "notify_budget";
    if (base >= mc2_boundary::NOTIFY_LIMIT - 1U || !mc2_boundary::NotifyBudget(
        base + 1U, resCtx.threads.size(), config.totalTurns, required)) {
        return;
    }
    context.notifyBase = base + 1U;
    context.valid = true;
    context.layout.fallbackReason = "none";
}

bool PrepareOpenBoundaryPolicy(
    const std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
    const mc2_boundary::Config& config, uint32_t turn, mc2_boundary::TurnPolicy& policy)
{
    OpenBoundaryStaticContext context;
    PrepareOpenBoundaryStaticContext(resCtx, config, context);
    return PrepareOpenBoundaryPolicy(opParam, resCtx, config, turn, context, policy);
}

bool PrepareOpenBoundaryPolicy(
    const std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
    const mc2_boundary::Config& config, uint32_t turn, const OpenBoundaryStaticContext& context,
    mc2_boundary::TurnPolicy& policy)
{
    policy = mc2_boundary::TurnPolicy{};
    policy.fallbackReason = "invalid_policy_or_thread_layout";
    if (opParam.size() < sizeof(mc2_ops_hccl::OpParam) || config.mode != 1U ||
        config.totalTurns <= 1U || turn >= config.totalTurns) {
        return false;
    }
    const auto& param = *reinterpret_cast<const mc2_ops_hccl::OpParam*>(opParam.data());
    policy.fallbackReason = "unsupported_algorithm_or_mode";
    if (param.opType != HCCL_CMD_ALLGATHER || param.engine != COMM_ENGINE_AICPU_TS ||
        std::string(param.algName) != "AicpuAllGatherSoleNHR" || param.supportSymmetricMemory ||
        param.opMode != mc2_ops_hccl::OpMode::OPBASE) {
        return false;
    }
    policy.fallbackReason = "stale_static_context";
    if (context.resource != &resCtx || context.mode != config.mode || context.totalTurns != config.totalTurns) {
        return false;
    }
    policy = context.layout;
    if (!context.valid) {
        return false;
    }
    const uint64_t element = GetDataTypeSize(param.DataDes.dataType);
    const uint64_t count = param.DataDes.count;
    const uint64_t stride = param.DataDes.strideCount == 0U ? count : param.DataDes.strideCount;
    const uint64_t ranks = resCtx.topoInfo.userRankSize;
    policy.fallbackReason = count == 0U ? "zero_count" : "invalid_range_or_stride";
    if (element == 0U || count == 0U || stride < count || ranks <= 1U ||
        count > UINT64_MAX / element || stride > (UINT64_MAX / element - count) / (ranks - 1U)) {
        return false;
    }
    const uint64_t inputSize = count * element;
    const uint64_t outputSize = (stride * (ranks - 1U) + count) * element;
    policy.fallbackReason = "buffer_alias_or_invalid_span";
    if (!DisjointBuffers(param.inputPtr, inputSize, param.outputPtr, outputSize) ||
        !DisjointBuffers(param.inputPtr, inputSize, resCtx.cclMem.addr, resCtx.cclMem.size) ||
        !DisjointBuffers(param.outputPtr, outputSize, resCtx.cclMem.addr, resCtx.cclMem.size)) {
        return false;
    }
    // Only post-copy entries are elided. Communication slaves retain their entry
    // waits, so no producer can recycle scratch before the previous full exit.
    policy.enabled = true;
    policy.skipEntry = turn != 0U;
    policy.exitNotify = context.notifyBase + turn * (policy.activeThreads - 1U);
    policy.fallbackReason = "none";
    return true;
}

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
    const mc2_boundary::TurnPolicy& policy);

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    return LaunchOpenOpParamDataImpl(opParam, resCtx, mc2_boundary::TurnPolicy{});
}

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
    const mc2_boundary::TurnPolicy& policy)
{
    auto* param = AsOpenOpParam(opParam);
    CHK_PTR_NULL(param);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][LaunchOpenOpParamDataImpl] file[%s:%d], opParamSize %zu, "
        "sizeof(OpParam) %zu, varMemSizeOffset %zu, varMemSize %llu, opType %u, algName[%s].",
        __FILE__, __LINE__, opParam.size(), sizeof(mc2_ops_hccl::OpParam), offsetof(mc2_ops_hccl::OpParam, varMemSize),
        static_cast<unsigned long long>(param->varMemSize), static_cast<u32>(param->opType), param->algName);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][Launch] opType %u, algName[%s], inputPtr %p, outputPtr %p, resCtx %p, ctxSize %llu, stream %p."
        "userRank %u, userRankSize %u, threadNum %zu, cclMemAddr %p, cclMemSize %llu.",
        static_cast<u32>(param->opType), param->algName, param->inputPtr, param->outputPtr, param->resCtx,
        static_cast<unsigned long long>(param->ctxSize), param->stream, resCtx.topoInfo.userRank,
        resCtx.topoInfo.userRankSize, resCtx.threads.size(), resCtx.cclMem.addr,
        static_cast<unsigned long long>(resCtx.cclMem.size));
    if (param->opType == HCCL_CMD_ALLTOALLV || param->opType == HCCL_CMD_ALLTOALL) {
        HCCL_INFO(
            "[MC2_OPEN_DIAG][Launch][AllToAllV] sendDataType %u, recvDataType %u, sendCounts %p, recvCounts %p, "
            "sdispls %p, rdispls %p.",
            static_cast<u32>(param->all2AllVDataDes.sendType), static_cast<u32>(param->all2AllVDataDes.recvType),
            param->all2AllVDataDes.sendCounts, param->all2AllVDataDes.recvCounts, param->all2AllVDataDes.sdispls,
            param->all2AllVDataDes.rdispls);
    } else {
        HCCL_INFO(
            "[MC2_OPEN_DIAG][Launch][Else] count %llu, dataType %u, outputType %u, strideCount %llu.",
            static_cast<unsigned long long>(param->DataDes.count), static_cast<u32>(param->DataDes.dataType),
            static_cast<u32>(param->DataDes.outputType), static_cast<unsigned long long>(param->DataDes.strideCount));
    }
    const u64 dataTypeSize = GetDataTypeSize(param->DataDes.dataType);
    const u64 inputBytes = param->DataDes.count * dataTypeSize;
    const bool isAllGather = (param->opType == HCCL_CMD_ALLGATHER);
    const u64 rankStrideCount = (param->DataDes.strideCount == 0U) ? param->DataDes.count : param->DataDes.strideCount;
    const u64 outputSpanCount =
        isAllGather ? (rankStrideCount * (resCtx.topoInfo.userRankSize == 0U ? 0U : resCtx.topoInfo.userRankSize - 1U) +
                       param->DataDes.count) :
                      param->DataDes.count;
    const u64 outputBytes = outputSpanCount * dataTypeSize;
    const u64 rankStrideBytes = isAllGather ? rankStrideCount * dataTypeSize : 0U;
    const u64 inputBegin = reinterpret_cast<u64>(param->inputPtr);
    const u64 outputBegin = reinterpret_cast<u64>(param->outputPtr);
    HCCL_INFO(
        "[MC2_OPEN_DIAG][LaunchRange] userRank %u, userRankSize %u, dataTypeSize %llu, inputBytes %llu, "
        "rankStrideCount %llu, rankStrideBytes %llu, outputSpanCount %llu, outputBytes %llu, "
        "inputRange [%#llx, %#llx), outputSpan [%#llx, %#llx), outputSpanRankScaled %u, engine %u.",
        resCtx.topoInfo.userRank, resCtx.topoInfo.userRankSize, static_cast<unsigned long long>(dataTypeSize),
        static_cast<unsigned long long>(inputBytes), static_cast<unsigned long long>(rankStrideCount),
        static_cast<unsigned long long>(rankStrideBytes), static_cast<unsigned long long>(outputSpanCount),
        static_cast<unsigned long long>(outputBytes), static_cast<unsigned long long>(inputBegin),
        static_cast<unsigned long long>(inputBegin + inputBytes), static_cast<unsigned long long>(outputBegin),
        static_cast<unsigned long long>(outputBegin + outputBytes), isAllGather, static_cast<u32>(param->engine));

    CHK_RET(RestoreVarDataIfNeeded(*param, resCtx));

    const bool useCannBridge = (param->opType == HCCL_CMD_ALLTOALLV || param->opType == HCCL_CMD_ALLREDUCE);
    if (useCannBridge) {
        HcclResult cannRet = mc2_ops_hccl::RunWithMc2TaskCache(
            *param, resCtx, [&]() -> HcclResult { return mc2_ops_hccl::LaunchViaCann(*param, resCtx); });
        CHK_PRT_RET(
            cannRet != HCCL_SUCCESS,
            HCCL_ERROR(
                "[CannBridge] dispatch failed (ret=%d), opType=%u algName[%s]", static_cast<int>(cannRet),
                static_cast<u32>(param->opType), param->algName),
            cannRet);
        return HCCL_SUCCESS;
    }

    if (policy.enabled) {
        using NhrExecutor = mc2_ops_hccl::InsV2AllGatherSoleExecutor<
            mc2_ops_hccl::TopoMatch1D, mc2_ops_hccl::InsTempAllGatherNHR>;
        NhrExecutor nhrExecutor;
        CHK_RET(mc2_ops_hccl::InitHcommBatchTransferOnThreadSupported(resCtx.isHcommBatchTransferOnThreadSupported));
        const std::string boundaryTag = ":mb2:" + std::to_string(policy.skipEntry) + ":" +
            std::to_string(policy.exitNotify) + ":" + std::to_string(policy.communicationThreads) + ":" +
            std::to_string(policy.activeThreads);
        return mc2_ops_hccl::RunWithMc2TaskCache(
            *param, resCtx, [&]() { return nhrExecutor.OrchestrateBoundary(*param, resCtx, policy); },
            mc2_ops_hccl::Mc2AicpuTaskCacheManager::Instance(), boundaryTag);
    }
    auto executor =
        mc2_ops_hccl::CollAlgExecRegistryV2::Instance().GetAlgExec(param->opType, std::string(param->algName));
    CHK_SMART_PTR_NULL(executor);

    CHK_RET(mc2_ops_hccl::InitHcommBatchTransferOnThreadSupported(resCtx.isHcommBatchTransferOnThreadSupported));
    HcclResult ret = mc2_ops_hccl::RunWithMc2TaskCache(
        *param, resCtx, [&]() -> HcclResult { return executor->Orchestrate(*param, resCtx); });
    CHK_PRT_RET(ret != HCCL_SUCCESS, HCCL_ERROR("orchestrate failed for alg:%s", param->algName), ret);

    return HCCL_SUCCESS;
}
