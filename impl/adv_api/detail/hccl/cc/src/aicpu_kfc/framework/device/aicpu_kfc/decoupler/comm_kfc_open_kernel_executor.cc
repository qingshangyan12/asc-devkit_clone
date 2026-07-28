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
#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#include "log.h"
#include "alg_param.h"
#include "alg_data_trans_wrapper.h"
#include "aicpu_sqe_cache.h"
#include "coll_alg_v2_exec_registry.h"
#include "cann_dispatch_bridge.h"
#include "hcomm_primitives.h"
#include "sal_pub.h"

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

bool IsMc2BatchModeOp(HcclCMDType opType)
{
    return opType == HCCL_CMD_ALLREDUCE || opType == HCCL_CMD_ALLGATHER || opType == HCCL_CMD_REDUCE_SCATTER ||
           opType == HCCL_CMD_ALLTOALLV || opType == HCCL_CMD_ALLTOALL;
}

constexpr u64 PHASE_ONE_MAX_TRANSFER_BYTES = 256ULL * 1024ULL * 1024ULL;

u64 ElapsedNs(u64 begin, u64 end)
{
    return end >= begin ? end - begin : 0U;
}

bool SafeMul(u64 lhs, u64 rhs, u64& result)
{
    if (lhs != 0U && rhs > std::numeric_limits<u64>::max() / lhs) {
        return false;
    }
    result = lhs * rhs;
    return true;
}

bool SafeSpan(u64 displacement, u64 count, u64& span)
{
    if (displacement > std::numeric_limits<u64>::max() - count) {
        return false;
    }
    span = displacement + count;
    return true;
}

void HashValue(u64 value, u64& hash)
{
    constexpr u64 FNV_PRIME = 1099511628211ULL;
    hash ^= value;
    hash *= FNV_PRIME;
}

u64 BuildResourceId(const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
{
    u64 hash = 1469598103934665603ULL;
    HashValue(resCtx.topoInfo.userRank, hash);
    HashValue(resCtx.topoInfo.userRankSize, hash);
    HashValue(reinterpret_cast<u64>(resCtx.cclMem.addr), hash);
    HashValue(resCtx.cclMem.size, hash);
    HashValue(resCtx.unfoldThread, hash);
    for (ThreadHandle thread : resCtx.threads) {
        HashValue(thread, hash);
    }
    for (const auto& level : resCtx.channels) {
        for (const auto& channel : level) {
            HashValue(channel.handle, hash);
            HashValue(channel.remoteRank, hash);
            HashValue(reinterpret_cast<u64>(channel.remoteCclMem.addr), hash);
            HashValue(channel.remoteCclMem.size, hash);
        }
    }
    return hash;
}

u32 BuildModeFlags(const mc2_ops_hccl::OpParam& param)
{
    u64 hash = 2166136261U;
    HashValue(static_cast<u32>(param.engine), hash);
    HashValue(static_cast<u32>(param.opMode), hash);
    HashValue(static_cast<u32>(param.reduceType), hash);
    if (param.opType == HCCL_CMD_ALLTOALL || param.opType == HCCL_CMD_ALLTOALLV) {
        HashValue(static_cast<u32>(param.all2AllVDataDes.sendType), hash);
        HashValue(static_cast<u32>(param.all2AllVDataDes.recvType), hash);
    } else {
        HashValue(static_cast<u32>(param.DataDes.dataType), hash);
        HashValue(static_cast<u32>(param.DataDes.outputType), hash);
    }
    HashValue(param.inputPtr == param.outputPtr, hash);
    HashValue(param.supportSymmetricMemory, hash);
    return static_cast<u32>(hash ^ (hash >> 32U));
}

struct SqeCacheBindingStorage {
    HcommAicpuSqeCacheParam param{};
    std::vector<HcommAicpuSqeCacheRange> ranges;
    std::vector<uint64_t> sendCounts;
    std::vector<uint64_t> recvCounts;
    std::vector<uint64_t> sendDispls;
    std::vector<uint64_t> recvDispls;
};

void AddRange(SqeCacheBindingStorage& storage, u64 addr, u64 size, u32 kind, u32 index)
{
    if (addr == 0U || size == 0U) {
        return;
    }
    storage.ranges.push_back(HcommAicpuSqeCacheRange{addr, size, kind, index});
}

bool BuildAllToAllVShape(const mc2_ops_hccl::OpParam& param,
                         const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
                         SqeCacheBindingStorage& storage, u64& inputSpan, u64& outputSpan, u64& maxTransfer,
                         const char*& reason)
{
    const u32 rankSize = resCtx.topoInfo.userRankSize;
    if (rankSize == 0U || param.all2AllVDataDes.sendCounts == nullptr ||
        param.all2AllVDataDes.recvCounts == nullptr || param.all2AllVDataDes.sdispls == nullptr ||
        param.all2AllVDataDes.rdispls == nullptr) {
        reason = "invalid_alltoall_vectors";
        return false;
    }
    const u64 sendTypeSize = GetDataTypeSize(param.all2AllVDataDes.sendType);
    const u64 recvTypeSize = GetDataTypeSize(param.all2AllVDataDes.recvType);
    if (sendTypeSize == 0U || recvTypeSize == 0U) {
        reason = "invalid_data_type";
        return false;
    }
    const auto* sendCounts = reinterpret_cast<const u64*>(param.all2AllVDataDes.sendCounts);
    const auto* recvCounts = reinterpret_cast<const u64*>(param.all2AllVDataDes.recvCounts);
    const auto* sendDispls = reinterpret_cast<const u64*>(param.all2AllVDataDes.sdispls);
    const auto* recvDispls = reinterpret_cast<const u64*>(param.all2AllVDataDes.rdispls);
    storage.sendCounts.resize(rankSize);
    storage.recvCounts.resize(rankSize);
    storage.sendDispls.resize(rankSize);
    storage.recvDispls.resize(rankSize);
    for (u32 peer = 0; peer < rankSize; ++peer) {
        u64 sendCountBytes = 0;
        u64 recvCountBytes = 0;
        u64 sendDisplBytes = 0;
        u64 recvDisplBytes = 0;
        if (!SafeMul(sendCounts[peer], sendTypeSize, sendCountBytes) ||
            !SafeMul(recvCounts[peer], recvTypeSize, recvCountBytes) ||
            !SafeMul(sendDispls[peer], sendTypeSize, sendDisplBytes) ||
            !SafeMul(recvDispls[peer], recvTypeSize, recvDisplBytes)) {
            reason = "size_overflow";
            return false;
        }
        storage.sendCounts[peer] = sendCountBytes;
        storage.recvCounts[peer] = recvCountBytes;
        storage.sendDispls[peer] = sendDisplBytes;
        storage.recvDispls[peer] = recvDisplBytes;
        u64 sendEnd = 0;
        u64 recvEnd = 0;
        if (!SafeSpan(storage.sendDispls[peer], storage.sendCounts[peer], sendEnd) ||
            !SafeSpan(storage.recvDispls[peer], storage.recvCounts[peer], recvEnd)) {
            reason = "size_overflow";
            return false;
        }
        inputSpan = std::max(inputSpan, sendEnd);
        outputSpan = std::max(outputSpan, recvEnd);
        maxTransfer = std::max(maxTransfer, std::max(sendCountBytes, recvCountBytes));
    }
    if (maxTransfer > PHASE_ONE_MAX_TRANSFER_BYTES) {
        reason = "phase1_transfer_limit";
        return false;
    }
    return true;
}

bool BuildSqeCacheBinding(const mc2_ops_hccl::OpParam& param,
                          const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx,
                          SqeCacheBindingStorage& storage, const char*& reason)
{
    reason = "none";
    if (!IsMc2BatchModeOp(param.opType)) {
        reason = "unsupported_op";
        return false;
    }
    if (param.engine != CommEngine::COMM_ENGINE_AICPU) {
        reason = "non_aicpu_engine";
        return false;
    }
    if (param.algName[0] == '\0') {
        reason = "empty_alg_name";
        return false;
    }
    if (resCtx.topoInfo.userRankSize == 0U) {
        reason = "invalid_rank_size";
        return false;
    }
    const bool isAllToAll = param.opType == HCCL_CMD_ALLTOALL || param.opType == HCCL_CMD_ALLTOALLV;
    u64 dataBytes = 0;
    u64 inputSpan = 0;
    u64 outputSpan = 0;
    u64 inputUnit = 0;
    u64 outputUnit = 0;
    u64 maxTransfer = 0;
    if (isAllToAll) {
        if (!BuildAllToAllVShape(param, resCtx, storage, inputSpan, outputSpan, maxTransfer, reason)) {
            return false;
        }
        dataBytes = maxTransfer;
        inputUnit = maxTransfer;
        outputUnit = maxTransfer;
    } else {
        const u64 typeSize = GetDataTypeSize(param.DataDes.dataType);
        if (typeSize == 0U) {
            reason = "invalid_data_type";
            return false;
        }
        if (!SafeMul(param.DataDes.count, typeSize, dataBytes)) {
            reason = "size_overflow";
            return false;
        }
        if (dataBytes == 0U) {
            reason = "zero_data";
            return false;
        }
        if (dataBytes > PHASE_ONE_MAX_TRANSFER_BYTES) {
            reason = "phase1_transfer_limit";
            return false;
        }
        u64 strideBytes = dataBytes;
        if (param.DataDes.strideCount != 0U && !SafeMul(param.DataDes.strideCount, typeSize, strideBytes)) {
            reason = "size_overflow";
            return false;
        }
        const u64 rankTail = resCtx.topoInfo.userRankSize - 1U;
        u64 rankOffset = 0;
        if (!SafeMul(rankTail, strideBytes, rankOffset)) {
            reason = "size_overflow";
            return false;
        }
        if (param.opType == HCCL_CMD_ALLGATHER) {
            if (!SafeSpan(rankOffset, dataBytes, outputSpan)) {
                reason = "size_overflow";
                return false;
            }
            inputSpan = dataBytes;
            inputUnit = dataBytes;
            outputUnit = strideBytes;
        } else if (param.opType == HCCL_CMD_REDUCE_SCATTER) {
            if (!SafeSpan(rankOffset, dataBytes, inputSpan)) {
                reason = "size_overflow";
                return false;
            }
            outputSpan = dataBytes;
            inputUnit = strideBytes;
            outputUnit = dataBytes;
        } else {
            inputSpan = dataBytes;
            outputSpan = dataBytes;
            inputUnit = dataBytes;
            outputUnit = dataBytes;
        }
        maxTransfer = dataBytes;
    }

    u64 scratchRequirement = 0;
    if (!SafeMul(maxTransfer, resCtx.topoInfo.userRankSize, scratchRequirement)) {
        reason = "size_overflow";
        return false;
    }
    if (scratchRequirement > resCtx.cclMem.size) {
        reason = "insufficient_ccl_memory";
        return false;
    }

    AddRange(storage, reinterpret_cast<u64>(param.inputPtr), inputSpan, HCOMM_AICPU_CACHE_RANGE_INPUT, 0U);
    AddRange(storage, reinterpret_cast<u64>(param.outputPtr), outputSpan, HCOMM_AICPU_CACHE_RANGE_OUTPUT, 0U);
    AddRange(storage, reinterpret_cast<u64>(resCtx.cclMem.addr), resCtx.cclMem.size,
             HCOMM_AICPU_CACHE_RANGE_CCL, 0U);
    u32 channelIndex = 0U;
    for (const auto& level : resCtx.channels) {
        for (const auto& channel : level) {
            const u32 rangeIndex = channel.remoteRank == INVALID_VALUE_RANKID ? channelIndex : channel.remoteRank;
            AddRange(storage, reinterpret_cast<u64>(channel.remoteCclMem.addr), channel.remoteCclMem.size,
                     HCOMM_AICPU_CACHE_RANGE_REMOTE_CCL, rangeIndex);
            AddRange(storage, reinterpret_cast<u64>(channel.remoteInputGraphMode.addr),
                     channel.remoteInputGraphMode.size, HCOMM_AICPU_CACHE_RANGE_REMOTE_INPUT, rangeIndex);
            AddRange(storage, reinterpret_cast<u64>(channel.remoteOutputGraphMode.addr),
                     channel.remoteOutputGraphMode.size, HCOMM_AICPU_CACHE_RANGE_REMOTE_OUTPUT, rangeIndex);
            ++channelIndex;
        }
    }

    storage.param.algName = param.algName;
    storage.param.opType = static_cast<u32>(param.opType);
    storage.param.modeFlags = BuildModeFlags(param);
    storage.param.resourceId = BuildResourceId(resCtx);
    storage.param.dataBytes = dataBytes;
    storage.param.inputUnitBytes = inputUnit;
    storage.param.outputUnitBytes = outputUnit;
    storage.param.notifyWaitTimeout = resCtx.waitTimeout;
    storage.param.rankSize = resCtx.topoInfo.userRankSize;
    storage.param.ranges = storage.ranges.data();
    storage.param.rangeCount = storage.ranges.size();
    if (isAllToAll) {
        storage.param.sendCountsBytes = storage.sendCounts.data();
        storage.param.recvCountsBytes = storage.recvCounts.data();
        storage.param.sendDisplsBytes = storage.sendDispls.data();
        storage.param.recvDisplsBytes = storage.recvDispls.data();
    }
    return true;
}
template <typename Func>
HcclResult RunWithMc2BatchMode(const mc2_ops_hccl::OpParam& param, Func func)
{
    if (!IsMc2BatchModeOp(param.opType)) {
        return func();
    }

    HCCL_DEBUG(
        "[MC2_BATCH][Start] opType[%u], algName[%s], algTag[%s].", static_cast<u32>(param.opType), param.algName,
        param.algTag);
    HcclResult startRet = static_cast<HcclResult>(HcommBatchModeStart(param.algTag));
    CHK_PRT_RET(
        startRet != HCCL_SUCCESS,
        HCCL_ERROR(
            "[MC2_BATCH][Start] failed, ret[%d], opType[%u], algName[%s], algTag[%s].", static_cast<int>(startRet),
            static_cast<u32>(param.opType), param.algName, param.algTag),
        startRet);

    HcclResult execRet = func();
    HcclResult endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
    HCCL_DEBUG(
        "[MC2_BATCH][End] opType[%u], algName[%s], algTag[%s], execRet[%d], endRet[%d].",
        static_cast<u32>(param.opType), param.algName, param.algTag, static_cast<int>(execRet),
        static_cast<int>(endRet));

    if (execRet != HCCL_SUCCESS) {
        if (endRet != HCCL_SUCCESS) {
            HCCL_ERROR(
                "[MC2_BATCH][Cleanup] executor failed, execRet[%d], and batch end failed, endRet[%d], "
                "opType[%u], algName[%s], algTag[%s].",
                static_cast<int>(execRet), static_cast<int>(endRet), static_cast<u32>(param.opType), param.algName,
                param.algTag);
        }
        return execRet;
    }
    CHK_PRT_RET(
        endRet != HCCL_SUCCESS,
        HCCL_ERROR(
            "[MC2_BATCH][End] failed, ret[%d], opType[%u], algName[%s], algTag[%s].", static_cast<int>(endRet),
            static_cast<u32>(param.opType), param.algName, param.algTag),
        endRet);
    return HCCL_SUCCESS;
}

template <typename Func>
HcclResult RunWithAicpuSqeCache(const mc2_ops_hccl::OpParam& param,
                                const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx, Func func)
{
    const u64 totalBegin = GetCurAicpuTimestamp();
    SqeCacheBindingStorage storage;
    const char* bypassReason = "unknown";
    const bool cacheEligible = BuildSqeCacheBinding(param, resCtx, storage, bypassReason);
    const u64 bindingEnd = GetCurAicpuTimestamp();
    if (!cacheEligible) {
        const HcclResult ret = RunWithMc2BatchMode(param, func);
        const u64 totalEnd = GetCurAicpuTimestamp();
        HCCL_RUN_INFO(
            "[AICPU_SQE_CACHE_DIAG][Upper] path[bypass] reason[%s] algName[%s] opType[%u] ret[%d] "
            "bindingNs[%llu] normalExpandAndLaunchNs[%llu] totalNs[%llu]",
            bypassReason, param.algName, static_cast<u32>(param.opType), static_cast<int>(ret),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
            static_cast<unsigned long long>(ElapsedNs(bindingEnd, totalEnd)),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, totalEnd)));
        return ret;
    }

    const u64 batchStartBegin = GetCurAicpuTimestamp();
    HcclResult startRet = static_cast<HcclResult>(HcommBatchModeStart(param.algTag));
    const u64 batchStartEnd = GetCurAicpuTimestamp();
    if (startRet != HCCL_SUCCESS) {
        HCCL_ERROR("[AicpuSqeCache] batch start failed, algName[%s], ret[%d]", param.algName, startRet);
        HCCL_RUN_INFO(
            "[AICPU_SQE_CACHE_DIAG][Upper] path[error] stage[batch_start] algName[%s] opType[%u] ret[%d] "
            "bindingNs[%llu] batchStartNs[%llu] totalNs[%llu]",
            param.algName, static_cast<u32>(param.opType), static_cast<int>(startRet),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
            static_cast<unsigned long long>(ElapsedNs(batchStartBegin, batchStartEnd)),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, batchStartEnd)));
        return startRet;
    }

    u32 replayed = 0U;
    const u64 replayBegin = GetCurAicpuTimestamp();
    HcclResult replayRet = static_cast<HcclResult>(HcommAicpuSqeCacheReplay(&storage.param, &replayed));
    const u64 replayEnd = GetCurAicpuTimestamp();
    if (replayRet != HCCL_SUCCESS) {
        HcommAicpuSqeCacheAbort();
        const u64 cleanupBegin = GetCurAicpuTimestamp();
        (void)HcommBatchModeEnd(param.algTag);
        const u64 cleanupEnd = GetCurAicpuTimestamp();
        HCCL_RUN_INFO(
            "[AICPU_SQE_CACHE_DIAG][Upper] path[error] stage[replay] algName[%s] opType[%u] ret[%d] "
            "bindingNs[%llu] batchStartNs[%llu] replayNs[%llu] cleanupBatchEndNs[%llu] totalNs[%llu]",
            param.algName, static_cast<u32>(param.opType), static_cast<int>(replayRet),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
            static_cast<unsigned long long>(ElapsedNs(batchStartBegin, batchStartEnd)),
            static_cast<unsigned long long>(ElapsedNs(replayBegin, replayEnd)),
            static_cast<unsigned long long>(ElapsedNs(cleanupBegin, cleanupEnd)),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, cleanupEnd)));
        return replayRet;
    }
    if (replayed != 0U) {
        const u64 launchBegin = GetCurAicpuTimestamp();
        HcclResult endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
        const u64 launchEnd = GetCurAicpuTimestamp();
        HCCL_RUN_INFO(
            "[AICPU_SQE_CACHE_DIAG][Upper] path[hit] algName[%s] opType[%u] ret[%d] bindingNs[%llu] "
            "batchStartNs[%llu] replayNs[%llu] batchEndLaunchNs[%llu] totalNs[%llu]",
            param.algName, static_cast<u32>(param.opType), static_cast<int>(endRet),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
            static_cast<unsigned long long>(ElapsedNs(batchStartBegin, batchStartEnd)),
            static_cast<unsigned long long>(ElapsedNs(replayBegin, replayEnd)),
            static_cast<unsigned long long>(ElapsedNs(launchBegin, launchEnd)),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, launchEnd)));
        if (endRet != HCCL_SUCCESS) {
            HCCL_ERROR("[AicpuSqeCache] replay launch failed, algName[%s], ret[%d]", param.algName, endRet);
            return endRet;
        }
        return HCCL_SUCCESS;
    }

    const u64 beginCaptureBegin = GetCurAicpuTimestamp();
    const HcclResult beginRet = static_cast<HcclResult>(HcommAicpuSqeCacheBegin(&storage.param));
    const u64 beginCaptureEnd = GetCurAicpuTimestamp();
    const bool captureStarted = beginRet == HCCL_SUCCESS;
    if (!captureStarted) {
        HCCL_WARNING("[AicpuSqeCache] capture begin failed, continue normal expansion, algName[%s], ret[%d]",
                     param.algName, beginRet);
    }
    const u64 executorBegin = GetCurAicpuTimestamp();
    const HcclResult execRet = func();
    const u64 executorEnd = GetCurAicpuTimestamp();
    const u64 launchBegin = GetCurAicpuTimestamp();
    const HcclResult endRet = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
    const u64 launchEnd = GetCurAicpuTimestamp();
    if (execRet != HCCL_SUCCESS || endRet != HCCL_SUCCESS) {
        HcommAicpuSqeCacheAbort();
        const HcclResult ret = execRet != HCCL_SUCCESS ? execRet : endRet;
        HCCL_RUN_INFO(
            "[AICPU_SQE_CACHE_DIAG][Upper] path[miss_error] algName[%s] opType[%u] ret[%d] captureStarted[%u] "
            "bindingNs[%llu] batchStartNs[%llu] lookupReplayNs[%llu] captureBeginNs[%llu] executorNs[%llu] "
            "batchEndLaunchNs[%llu] totalNs[%llu]",
            param.algName, static_cast<u32>(param.opType), static_cast<int>(ret), captureStarted,
            static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
            static_cast<unsigned long long>(ElapsedNs(batchStartBegin, batchStartEnd)),
            static_cast<unsigned long long>(ElapsedNs(replayBegin, replayEnd)),
            static_cast<unsigned long long>(ElapsedNs(beginCaptureBegin, beginCaptureEnd)),
            static_cast<unsigned long long>(ElapsedNs(executorBegin, executorEnd)),
            static_cast<unsigned long long>(ElapsedNs(launchBegin, launchEnd)),
            static_cast<unsigned long long>(ElapsedNs(totalBegin, launchEnd)));
        return ret;
    }
    u64 commitNs = 0U;
    HcclResult commitRet = HCCL_SUCCESS;
    if (captureStarted) {
        const u64 commitBegin = GetCurAicpuTimestamp();
        commitRet = static_cast<HcclResult>(HcommAicpuSqeCacheCommit());
        const u64 commitEnd = GetCurAicpuTimestamp();
        commitNs = ElapsedNs(commitBegin, commitEnd);
        if (commitRet != HCCL_SUCCESS) {
            HCCL_WARNING("[AicpuSqeCache] publish failed after successful launch, algName[%s], ret[%d]",
                         param.algName, commitRet);
        }
    }
    const u64 totalEnd = GetCurAicpuTimestamp();
    HCCL_RUN_INFO(
        "[AICPU_SQE_CACHE_DIAG][Upper] path[miss_expand] algName[%s] opType[%u] ret[%d] captureStarted[%u] "
        "captureBeginRet[%d] commitRet[%d] bindingNs[%llu] batchStartNs[%llu] lookupReplayNs[%llu] "
        "captureBeginNs[%llu] executorNs[%llu] batchEndLaunchNs[%llu] commitNs[%llu] totalNs[%llu]",
        param.algName, static_cast<u32>(param.opType), static_cast<int>(HCCL_SUCCESS), captureStarted,
        static_cast<int>(beginRet), static_cast<int>(commitRet),
        static_cast<unsigned long long>(ElapsedNs(totalBegin, bindingEnd)),
        static_cast<unsigned long long>(ElapsedNs(batchStartBegin, batchStartEnd)),
        static_cast<unsigned long long>(ElapsedNs(replayBegin, replayEnd)),
        static_cast<unsigned long long>(ElapsedNs(beginCaptureBegin, beginCaptureEnd)),
        static_cast<unsigned long long>(ElapsedNs(executorBegin, executorEnd)),
        static_cast<unsigned long long>(ElapsedNs(launchBegin, launchEnd)), static_cast<unsigned long long>(commitNs),
        static_cast<unsigned long long>(ElapsedNs(totalBegin, totalEnd)));
    return HCCL_SUCCESS;
}
} // namespace

HcclResult LaunchOpenOpParamDataImpl(
    std::vector<uint8_t>& opParam, const mc2_ops_hccl::AlgResourceCtxSerializable& resCtx)
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

    const bool useCannBridge =
        (param->opType == HCCL_CMD_ALLTOALL || param->opType == HCCL_CMD_ALLTOALLV ||
         param->opType == HCCL_CMD_ALLREDUCE);
    HcclResult ret = RunWithAicpuSqeCache(*param, resCtx, [&]() -> HcclResult {
        if (useCannBridge) {
            return mc2_ops_hccl::LaunchViaCann(*param, resCtx);
        }
        auto executor =
            mc2_ops_hccl::CollAlgExecRegistryV2::Instance().GetAlgExec(param->opType, std::string(param->algName));
        CHK_SMART_PTR_NULL(executor);
        CHK_RET(mc2_ops_hccl::InitHcommBatchTransferOnThreadSupported(
            resCtx.isHcommBatchTransferOnThreadSupported));
        return executor->Orchestrate(*param, resCtx);
    });
    CHK_PRT_RET(ret != HCCL_SUCCESS, HCCL_ERROR("orchestrate failed for alg:%s", param->algName), ret);

    return HCCL_SUCCESS;
}
