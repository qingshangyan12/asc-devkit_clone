/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "mc2_aicpu_task_cache.h"

#include <algorithm>
#include <atomic>
#include <cstring>
#include <iomanip>
#include <limits>
#include <sstream>

#include "alg_env_config.h"
#include "hcomm_primitives_dl.h"
#include "log.h"
#include "mc2_server_stage_timing.h"
#include <memory>
#include <vector>

namespace mc2_ops_hccl {
namespace {

constexpr uint64_t CACHE_ADDR_COUNT = 2U;
constexpr char CACHE_TAG_PREFIX[] = "mc2tc:v1";

class Digest128Builder {
public:
    explicit Digest128Builder(const char* domain) { AddString(domain, std::strlen(domain)); }

    void AddU8(uint8_t value) { AddBytes(&value, sizeof(value)); }

    void AddU32(uint32_t value)
    {
        uint8_t bytes[sizeof(value)];
        for (size_t i = 0; i < sizeof(value); ++i) {
            bytes[i] = static_cast<uint8_t>((value >> (i * 8U)) & 0xffU);
        }
        AddBytes(bytes, sizeof(bytes));
    }

    void AddU64(uint64_t value)
    {
        uint8_t bytes[sizeof(value)];
        for (size_t i = 0; i < sizeof(value); ++i) {
            bytes[i] = static_cast<uint8_t>((value >> (i * 8U)) & 0xffU);
        }
        AddBytes(bytes, sizeof(bytes));
    }

    void AddBool(bool value) { AddU8(value ? 1U : 0U); }

    void AddDouble(double value)
    {
        static_assert(sizeof(value) == sizeof(uint64_t), "unexpected double size");
        // The cache is process-local on little-endian A5, so hashing the object bytes preserves the existing tag.
        AddBytes(&value, sizeof(value));
    }

    void AddPointer(const void* value) { AddU64(static_cast<uint64_t>(reinterpret_cast<uintptr_t>(value))); }

    void AddString(const char* value, size_t maxLen)
    {
        const size_t len = value == nullptr ? 0U : strnlen(value, maxLen);
        AddU64(static_cast<uint64_t>(len));
        if (len > 0U) {
            AddBytes(value, len);
        }
    }

    std::string Finish() const
    {
        const uint64_t first = Avalanche(first_ ^ length_);
        const uint64_t second = Avalanche(second_ ^ (length_ * 0x9e3779b97f4a7c15ULL));
        std::ostringstream stream;
        stream << std::hex << std::setfill('0') << std::setw(16) << first << std::setw(16) << second;
        return stream.str();
    }

private:
    static uint64_t RotateLeft(uint64_t value, uint32_t count) { return (value << count) | (value >> (64U - count)); }

    static uint64_t Avalanche(uint64_t value)
    {
        value ^= value >> 30U;
        value *= 0xbf58476d1ce4e5b9ULL;
        value ^= value >> 27U;
        value *= 0x94d049bb133111ebULL;
        return value ^ (value >> 31U);
    }

    void AddBytes(const void* data, size_t size)
    {
        const auto* bytes = static_cast<const uint8_t*>(data);
        for (size_t i = 0; i < size; ++i) {
            first_ ^= static_cast<uint64_t>(bytes[i]);
            first_ *= 0x100000001b3ULL;
            first_ = RotateLeft(first_, 13U);

            second_ += static_cast<uint64_t>(bytes[i]) + 0x9e3779b97f4a7c15ULL;
            second_ ^= second_ >> 29U;
            second_ *= 0x165667919e3779f9ULL;
            second_ = RotateLeft(second_, 17U);
        }
        length_ += static_cast<uint64_t>(size);
    }

    uint64_t first_ = 0xcbf29ce484222325ULL;
    uint64_t second_ = 0x6a09e667f3bcc909ULL;
    uint64_t length_ = 0U;
};

bool SafeMul(uint64_t lhs, uint64_t rhs, uint64_t& result)
{
    if (lhs != 0U && rhs > std::numeric_limits<uint64_t>::max() / lhs) {
        return false;
    }
    result = lhs * rhs;
    return true;
}

bool SafeAdd(uint64_t lhs, uint64_t rhs, uint64_t& result)
{
    if (rhs > std::numeric_limits<uint64_t>::max() - lhs) {
        return false;
    }
    result = lhs + rhs;
    return true;
}

uint64_t GetDataTypeSize(HcclDataType dataType)
{
    const uint32_t type = static_cast<uint32_t>(dataType);
    return type < static_cast<uint32_t>(HCCL_DATA_TYPE_RESERVED) ? DATATYPE_SIZE_TABLE[type] : 0U;
}

bool GetStridedSpan(uint64_t count, uint64_t strideCount, uint32_t rankSize, uint64_t elementSize, uint64_t& span)
{
    if (rankSize == 0U) {
        return false;
    }

    const uint64_t effectiveStrideCount = strideCount == 0U ? count : strideCount;
    uint64_t lastRankOffset = 0U;
    uint64_t totalCount = 0U;
    return SafeMul(effectiveStrideCount, rankSize - 1U, lastRankOffset) && SafeAdd(lastRankOffset, count, totalCount) &&
           SafeMul(totalCount, elementSize, span);
}

bool IsSupportedOp(HcclCMDType opType)
{
    return opType == HCCL_CMD_ALLGATHER || opType == HCCL_CMD_REDUCE_SCATTER || opType == HCCL_CMD_ALLREDUCE ||
           opType == HCCL_CMD_ALLTOALL || opType == HCCL_CMD_ALLTOALLV;
}

bool RequiresAicpuReduction(const OpParam& param)
{
    if (param.opType != HCCL_CMD_ALLREDUCE && param.opType != HCCL_CMD_REDUCE_SCATTER) {
        return false;
    }
    const HcclDataType dataType = param.DataDes.dataType;
    return dataType == HCCL_DATA_TYPE_INT64 || dataType == HCCL_DATA_TYPE_UINT64 || dataType == HCCL_DATA_TYPE_FP64 ||
           param.reduceType == HCCL_REDUCE_PROD;
}

bool IsSupportedProtocol(CommProtocol protocol)
{
    return protocol == CommProtocol::COMM_PROTOCOL_UBC_CTP || protocol == CommProtocol::COMM_PROTOCOL_UBC_TP;
}

void SetBypass(Mc2AicpuTaskCachePlan& plan, const char* reason)
{
    plan.enabled = false;
    HCCL_DEBUG("[MC2_TASK_CACHE][Bypass] reason[%s].", reason);
}

template <typename Builder>
bool GetAllToAllVSpans(
    const OpParam& param, uint32_t rankSize, uint64_t& inputSpan, uint64_t& outputSpan, Builder& shape)
{
    const auto* sendCounts = static_cast<const uint64_t*>(param.all2AllVDataDes.sendCounts);
    const auto* recvCounts = static_cast<const uint64_t*>(param.all2AllVDataDes.recvCounts);
    const auto* sendDispls = static_cast<const uint64_t*>(param.all2AllVDataDes.sdispls);
    const auto* recvDispls = static_cast<const uint64_t*>(param.all2AllVDataDes.rdispls);
    if (sendCounts == nullptr || recvCounts == nullptr || sendDispls == nullptr || recvDispls == nullptr) {
        return false;
    }

    const uint64_t sendTypeSize = GetDataTypeSize(param.all2AllVDataDes.sendType);
    const uint64_t recvTypeSize = GetDataTypeSize(param.all2AllVDataDes.recvType);
    if (sendTypeSize == 0U || recvTypeSize == 0U) {
        return false;
    }

    shape.AddU32(static_cast<uint32_t>(param.all2AllVDataDes.sendType));
    shape.AddU32(static_cast<uint32_t>(param.all2AllVDataDes.recvType));
    shape.AddU32(rankSize);
    for (uint32_t rank = 0U; rank < rankSize; ++rank) {
        uint64_t sendCountBytes = 0U;
        uint64_t recvCountBytes = 0U;
        uint64_t sendDisplBytes = 0U;
        uint64_t recvDisplBytes = 0U;
        uint64_t sendEnd = 0U;
        uint64_t recvEnd = 0U;
        if (!SafeMul(sendCounts[rank], sendTypeSize, sendCountBytes) ||
            !SafeMul(recvCounts[rank], recvTypeSize, recvCountBytes) ||
            !SafeMul(sendDispls[rank], sendTypeSize, sendDisplBytes) ||
            !SafeMul(recvDispls[rank], recvTypeSize, recvDisplBytes) ||
            !SafeAdd(sendDisplBytes, sendCountBytes, sendEnd) || !SafeAdd(recvDisplBytes, recvCountBytes, recvEnd)) {
            return false;
        }
        shape.AddU64(sendCountBytes);
        shape.AddU64(recvCountBytes);
        shape.AddU64(sendDisplBytes);
        shape.AddU64(recvDisplBytes);
        inputSpan = std::max(inputSpan, sendEnd);
        outputSpan = std::max(outputSpan, recvEnd);
    }
    return true;
}

template <typename Builder>
bool GetRegularSpans(const OpParam& param, uint32_t rankSize, uint64_t& inputSpan, uint64_t& outputSpan, Builder& shape)
{
    const uint64_t dataTypeSize = GetDataTypeSize(param.DataDes.dataType);
    const HcclDataType outputType =
        param.DataDes.outputType == HCCL_DATA_TYPE_RESERVED ? param.DataDes.dataType : param.DataDes.outputType;
    const uint64_t outputTypeSize = GetDataTypeSize(outputType);
    if (dataTypeSize == 0U || outputTypeSize == 0U) {
        return false;
    }

    shape.AddU64(param.DataDes.count);
    shape.AddU32(static_cast<uint32_t>(param.DataDes.dataType));
    shape.AddU32(static_cast<uint32_t>(outputType));
    shape.AddU64(param.DataDes.strideCount);

    uint64_t countBytes = 0U;
    uint64_t outputCountBytes = 0U;
    if (!SafeMul(param.DataDes.count, dataTypeSize, countBytes) ||
        !SafeMul(param.DataDes.count, outputTypeSize, outputCountBytes)) {
        return false;
    }
    if (param.opType == HCCL_CMD_ALLGATHER) {
        if (!GetStridedSpan(param.DataDes.count, param.DataDes.strideCount, rankSize, outputTypeSize, outputSpan)) {
            return false;
        }
        inputSpan = countBytes;
    } else if (param.opType == HCCL_CMD_REDUCE_SCATTER) {
        if (!GetStridedSpan(param.DataDes.count, param.DataDes.strideCount, rankSize, dataTypeSize, inputSpan)) {
            return false;
        }
        outputSpan = outputCountBytes;
    } else {
        inputSpan = countBytes;
        outputSpan = outputCountBytes;
    }
    return true;
}

bool IsOverlapping(const Mc2AicpuTaskCachePlan& plan)
{
    if (plan.sizes[0] == 0U || plan.sizes[1] == 0U) {
        return false;
    }
    const uint64_t input = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(plan.addrs[0]));
    const uint64_t output = static_cast<uint64_t>(reinterpret_cast<uintptr_t>(plan.addrs[1]));
    uint64_t inputEnd = 0U;
    uint64_t outputEnd = 0U;
    if (!SafeAdd(input, plan.sizes[0], inputEnd) || !SafeAdd(output, plan.sizes[1], outputEnd)) {
        return true;
    }
    return input < outputEnd && output < inputEnd;
}

template <typename Builder>
void AddAlgType(Builder& digest, const AlgType& algType)
{
    digest.AddU32(static_cast<uint32_t>(algType.algoLevel0));
    digest.AddU32(static_cast<uint32_t>(algType.algoLevel1));
    digest.AddU32(static_cast<uint32_t>(algType.algoLevel2));
}

void AddHcclMem(Digest128Builder& digest, const HcclMem& mem)
{
    digest.AddPointer(mem.addr);
    digest.AddU64(mem.size);
}

void AddByteArray(Digest128Builder& digest, const uint8_t* data, size_t size)
{
    digest.AddU64(static_cast<uint64_t>(size));
    for (size_t idx = 0U; idx < size; ++idx) {
        digest.AddU8(data[idx]);
    }
}

void AddEndpointDesc(Digest128Builder& digest, const EndpointDesc& endpoint)
{
    digest.AddU32(static_cast<uint32_t>(endpoint.protocol));
    digest.AddU32(static_cast<uint32_t>(endpoint.commAddr.type));
    AddByteArray(digest, endpoint.commAddr.raws, sizeof(endpoint.commAddr.raws));
    digest.AddU32(static_cast<uint32_t>(endpoint.loc.locType));
    AddByteArray(digest, endpoint.loc.raws, sizeof(endpoint.loc.raws));
    AddByteArray(digest, endpoint.raws, sizeof(endpoint.raws));
}

void AddPhysicalLevel(Digest128Builder& digest, const PhysicalLevelInfo& level)
{
    digest.AddU64(static_cast<uint64_t>(level.localRanks.size()));
    for (uint32_t rank : level.localRanks) {
        digest.AddU32(rank);
    }
    digest.AddU32(static_cast<uint32_t>(level.view));
    digest.AddU64(static_cast<uint64_t>(level.instSizeListByLayer.size()));
    for (uint32_t size : level.instSizeListByLayer) {
        digest.AddU32(size);
    }
    digest.AddU32(level.ref.netLayer);
    digest.AddU32(level.ref.topoInstId);
    digest.AddBool(level.hasTopoInst);
    digest.AddU32(static_cast<uint32_t>(level.topoType));
    digest.AddU32(static_cast<uint32_t>(level.locType));
    digest.AddU64(static_cast<uint64_t>(level.protocols.size()));
    for (CommProtocol protocol : level.protocols) {
        digest.AddU32(static_cast<uint32_t>(protocol));
    }
    digest.AddU64(static_cast<uint64_t>(level.portNums.size()));
    for (uint32_t portNum : level.portNums) {
        digest.AddU32(portNum);
    }
    digest.AddU64(static_cast<uint64_t>(level.endpoints.size()));
    for (const EndpointDesc& endpoint : level.endpoints) {
        AddEndpointDesc(digest, endpoint);
    }
}

template <typename Builder>
void AddShapeFields(const OpParam& param, uint64_t inputSpan, uint64_t outputSpan, Builder& shape)
{
    shape.AddU32(static_cast<uint32_t>(param.opType));
    shape.AddString(param.algName, OP_ALG_LENGTH);
    AddAlgType(shape, param.algType);
    shape.AddU32(static_cast<uint32_t>(param.engine));
    shape.AddU32(static_cast<uint32_t>(param.opMode));
    shape.AddU32(static_cast<uint32_t>(param.reduceType));
    shape.AddU32(param.root);
    shape.AddU32(param.userRank);
    shape.AddU32(param.sendRecvRemoteRank);
    shape.AddBool(param.enableDetour);
    shape.AddU32(static_cast<uint32_t>(param.deviceType));
    shape.AddBool(param.isZeroCopy);
    shape.AddBool(param.supportSymmetricMemory);
    shape.AddU64(param.inputOffset);
    shape.AddU64(param.outputOffset);
    shape.AddU32(static_cast<uint32_t>(param.commOpExpansionMode));
    shape.AddU32(static_cast<uint32_t>(param.opExecuteConfig));
    shape.AddU32(param.numBlocksLimit);
    shape.AddBool(param.isAivClearEnable);
    shape.AddU32(param.opConfig.execTimeout);
    shape.AddDouble(param.opConfig.multipleDimensionSplitRatio);
    shape.AddU32(static_cast<uint32_t>(param.opConfig.multipleDimensionSplitRatioSource));
    shape.AddU64(param.inputSize);
    shape.AddU64(param.outputSize);
    shape.AddU64(param.ctxSize);
    shape.AddU64(param.varMemSize);
    shape.AddU64(inputSpan);
    shape.AddU64(outputSpan);
}

bool AddResourceFields(const OpParam& param, const AlgResourceCtxSerializable& resCtx, Digest128Builder& resource)
{
    resource.AddPointer(param.hcclComm);
    resource.AddString(param.commName, COMM_INDENTIFIER_MAX_LENGTH);
    resource.AddString(param.algTag, ALG_TAG_LENGTH);
    resource.AddU32(resCtx.topoInfo.userRank);
    resource.AddU32(resCtx.topoInfo.userRankSize);
    resource.AddPointer(param.inputSymWindow);
    resource.AddPointer(param.outputSymWindow);
    AddHcclMem(resource, resCtx.cclMem);
    AddAlgType(resource, resCtx.algType);
    resource.AddU32(resCtx.notifyNumOnMainThread);
    resource.AddU32(resCtx.slaveThreadNum);
    resource.AddU32(resCtx.waitTimeout);
    resource.AddU32(resCtx.fullTimeout);
    resource.AddU64(static_cast<uint64_t>(resCtx.notifyNumPerThread.size()));
    for (uint32_t notifyNum : resCtx.notifyNumPerThread) {
        resource.AddU32(notifyNum);
    }
    resource.AddU64(static_cast<uint64_t>(resCtx.threads.size()));
    for (ThreadHandle thread : resCtx.threads) {
        resource.AddU64(static_cast<uint64_t>(thread));
    }
    resource.AddU64(static_cast<uint64_t>(resCtx.unfoldThread));
    resource.AddDouble(resCtx.dieSplitRatio);
    resource.AddBool(resCtx.isHcommBatchTransferOnThreadSupported);
    resource.AddBool(resCtx.isHcclThreadAcquireWithConfigSupported);
    resource.AddPointer(resCtx.aivCommInfoPtr);
    resource.AddPointer(resCtx.commInfoPtr);
    resource.AddPointer(resCtx.npu2DpuShmemPtr);
    resource.AddPointer(resCtx.dpu2NpuShmemPtr);
    resource.AddU64(static_cast<uint64_t>(param.opThread));
    AddHcclMem(resource, param.hcclBuff);

    const auto& topo = resCtx.topoInfo;
    resource.AddU32(topo.userRank);
    resource.AddU32(topo.userRankSize);
    resource.AddU32(topo.serverIdx);
    resource.AddU32(topo.superPodIdx);
    resource.AddU32(static_cast<uint32_t>(topo.deviceType));
    resource.AddU32(topo.deviceNumPerModule);
    resource.AddU32(topo.serverNumPerSuperPod);
    resource.AddU32(topo.serverNum);
    resource.AddU32(topo.moduleNum);
    resource.AddU32(topo.superPodNum);
    resource.AddU32(topo.moduleIdx);
    resource.AddBool(topo.isDiffDeviceModule);
    resource.AddBool(topo.multiModuleDiffDeviceNumMode);
    resource.AddBool(topo.multiSuperPodDiffServerNumMode);
    resource.AddBool(topo.isHCCSSWNumEqualToTwiceSIONum);
    resource.AddU64(static_cast<uint64_t>(topo.mainThread));
    resource.AddU32(topo.notifyNumOnMainThread);
    resource.AddU32(topo.topoLevelNums);
    resource.AddU32(static_cast<uint32_t>(topo.level0Topo));
    resource.AddBool(topo.Level0Nhr);
    resource.AddBool(topo.Level1Nhr);
    resource.AddBool(topo.Level1Hd);
    resource.AddBool(topo.is2DieFullMesh);
    resource.AddBool(topo.level0PcieMix);
    resource.AddBool(topo.level0BigClosRange);
    resource.AddBool(topo.topLevelUboe);
    resource.AddBool(topo.level2UbRtp);
    resource.AddBool(topo.hostDpuOnly);
    resource.AddBool(topo.level0Symmetric);
    resource.AddBool(topo.level1Symmetric);
    resource.AddU32(topo.topoInstDetailsOfLayerSize);
    resource.AddBool(topo.isPod);
    resource.AddU32(static_cast<uint32_t>(topo.level0MeshType));

    resource.AddU32(topo.netLayerDetails.netLayerNum);
    resource.AddU64(static_cast<uint64_t>(topo.netLayerDetails.netLayers.size()));
    for (uint32_t value : topo.netLayerDetails.netLayers) {
        resource.AddU32(value);
    }
    resource.AddU64(static_cast<uint64_t>(topo.netLayerDetails.netInstNumOfLayer.size()));
    for (uint32_t value : topo.netLayerDetails.netInstNumOfLayer) {
        resource.AddU32(value);
    }
    resource.AddU64(static_cast<uint64_t>(topo.netLayerDetails.instSizeListOfLayer.size()));
    for (const auto& values : topo.netLayerDetails.instSizeListOfLayer) {
        resource.AddU64(static_cast<uint64_t>(values.size()));
        for (uint32_t value : values) {
            resource.AddU32(value);
        }
    }
    resource.AddU64(static_cast<uint64_t>(topo.netLayerDetails.localNetInsSizeOfLayer.size()));
    for (uint32_t value : topo.netLayerDetails.localNetInsSizeOfLayer) {
        resource.AddU32(value);
    }

    resource.AddU64(static_cast<uint64_t>(topo.topoInstDetailsOfLayer.size()));
    for (const auto& detail : topo.topoInstDetailsOfLayer) {
        resource.AddU32(detail.topoInstNum);
        resource.AddU64(static_cast<uint64_t>(detail.sizeOfTopo.size()));
        for (uint32_t value : detail.sizeOfTopo) {
            resource.AddU32(value);
        }
        resource.AddU64(static_cast<uint64_t>(detail.typeOfTopo.size()));
        for (CommTopo value : detail.typeOfTopo) {
            resource.AddU32(static_cast<uint32_t>(value));
        }
        resource.AddU64(static_cast<uint64_t>(detail.ranksInTopo.size()));
        for (const auto& ranks : detail.ranksInTopo) {
            resource.AddU64(static_cast<uint64_t>(ranks.size()));
            for (uint32_t rank : ranks) {
                resource.AddU32(rank);
            }
        }
        resource.AddU64(static_cast<uint64_t>(detail.rankNumForTopoType.size()));
        for (const auto& rankNum : detail.rankNumForTopoType) {
            resource.AddU32(static_cast<uint32_t>(rankNum.first));
            resource.AddU64(static_cast<uint64_t>(rankNum.second.size()));
            for (uint32_t value : rankNum.second) {
                resource.AddU32(value);
            }
        }
    }
    resource.AddU64(static_cast<uint64_t>(topo.physicalLevels.size()));
    for (const PhysicalLevelInfo& level : topo.physicalLevels) {
        AddPhysicalLevel(resource, level);
    }

    resource.AddU64(static_cast<uint64_t>(resCtx.algHierarchyInfo.infos.size()));
    for (const auto& level : resCtx.algHierarchyInfo.infos) {
        resource.AddU64(static_cast<uint64_t>(level.size()));
        for (const auto& instance : level) {
            resource.AddU64(static_cast<uint64_t>(instance.size()));
            for (uint32_t rank : instance) {
                resource.AddU32(rank);
            }
        }
    }
    resource.AddU64(static_cast<uint64_t>(resCtx.algHierarchyInfo.physicalIdxForAlgoLevels.size()));
    for (const auto& level : resCtx.algHierarchyInfo.physicalIdxForAlgoLevels) {
        resource.AddU64(static_cast<uint64_t>(level.size()));
        for (PhysicalLevelIndex index : level) {
            resource.AddU32(static_cast<uint32_t>(index));
        }
    }

    bool hasValidChannel = false;
    resource.AddU64(static_cast<uint64_t>(resCtx.channels.size()));
    for (const auto& level : resCtx.channels) {
        resource.AddU64(static_cast<uint64_t>(level.size()));
        for (const auto& channel : level) {
            resource.AddBool(channel.isValid);
            if (!channel.isValid) {
                continue;
            }
            hasValidChannel = true;
            if (!IsSupportedProtocol(channel.protocol)) {
                return false;
            }
            resource.AddU32(channel.remoteRank);
            resource.AddU32(static_cast<uint32_t>(channel.protocol));
            resource.AddU32(static_cast<uint32_t>(channel.locationType));
            resource.AddU32(channel.notifyNum);
            resource.AddU32(channel.portGroupSize);
            resource.AddU32(channel.dieId);
            resource.AddU64(static_cast<uint64_t>(channel.handle));
            AddHcclMem(resource, channel.remoteCclMem);
            AddHcclMem(resource, channel.remoteInputGraphMode);
            AddHcclMem(resource, channel.remoteOutputGraphMode);
            AddHcclMem(resource, channel.remoteInput);
            AddHcclMem(resource, channel.remoteOutput);
        }
    }
    resource.AddU64(static_cast<uint64_t>(resCtx.kfcServerArgs.size()));
    for (uint64_t arg : resCtx.kfcServerArgs) {
        resource.AddU64(arg);
    }
    resource.AddU32(resCtx.kfcServerArgSize);
    return hasValidChannel;
}

} // namespace

HcclResult BuildMc2AicpuTaskCachePlan(
    const OpParam& param, const AlgResourceCtxSerializable& resCtx, Mc2AicpuTaskCachePlan& plan)
{
    plan = Mc2AicpuTaskCachePlan{};
    if (GetExternalInputHcclAicpuCacheEnable() == 0U) {
        SetBypass(plan, "AICPU_CacheDisable");
        return HCCL_SUCCESS;
    }
    if (!Mc2HcommIsSupportAicpuTsTaskCache()) {
        SetBypass(plan, "hcomm task cache symbols incomplete");
        return HCCL_SUCCESS;
    }
    if (!IsSupportedOp(param.opType) ||
        (param.engine != CommEngine::COMM_ENGINE_AICPU_TS && param.engine != CommEngine::COMM_ENGINE_CCU)) {
        SetBypass(plan, "unsupported op or engine");
        return HCCL_SUCCESS;
    }
    if (param.opMode == OpMode::OFFLOAD) {
        SetBypass(plan, "offload mode is not replayable");
        return HCCL_SUCCESS;
    }
    if (RequiresAicpuReduction(param)) {
        SetBypass(plan, "AICPU reduction work is not represented by cached tasks");
        return HCCL_SUCCESS;
    }
    if (param.supportSymmetricMemory) {
        SetBypass(plan, "symmetric-memory peer addresses are not replayable");
        return HCCL_SUCCESS;
    }
    if (param.hcclComm == nullptr || param.inputPtr == nullptr || param.outputPtr == nullptr ||
        resCtx.topoInfo.userRankSize <= 1U || resCtx.threads.empty() || param.algName[0] == '\0') {
        SetBypass(plan, "resource is incomplete");
        return HCCL_SUCCESS;
    }

    Digest128Builder shape("mc2-task-cache-shape-v1");
    uint64_t inputSpan = 0U;
    uint64_t outputSpan = 0U;
    bool spansValid = false;
    if (param.opType == HCCL_CMD_ALLTOALL || param.opType == HCCL_CMD_ALLTOALLV) {
        // The MC2 open adapter restores both operations into the AllToAllV four-array representation.
        spansValid = GetAllToAllVSpans(param, resCtx.topoInfo.userRankSize, inputSpan, outputSpan, shape);
    } else {
        spansValid = GetRegularSpans(param, resCtx.topoInfo.userRankSize, inputSpan, outputSpan, shape);
    }
    if (!spansValid || (inputSpan == 0U && outputSpan == 0U)) {
        SetBypass(plan, "invalid or empty input/output span");
        return HCCL_SUCCESS;
    }

    AddShapeFields(param, inputSpan, outputSpan, shape);
    Digest128Builder resource("mc2-task-cache-resource-v1");
    if (!AddResourceFields(param, resCtx, resource)) {
        SetBypass(plan, "unsupported or empty channel resource");
        return HCCL_SUCCESS;
    }

    plan.comm = param.hcclComm;
    plan.addrs[0] = param.inputPtr;
    plan.addrs[1] = param.outputPtr;
    plan.sizes[0] = inputSpan;
    plan.sizes[1] = outputSpan;
    if (IsOverlapping(plan)) {
        SetBypass(plan, "input and output overlap");
        return HCCL_SUCCESS;
    }

    plan.tag = std::string(CACHE_TAG_PREFIX) + ":" + std::to_string(static_cast<uint32_t>(param.opType)) + ":" +
               shape.Finish() + ":" + resource.Finish();
    plan.enabled = true;
    HCCL_INFO(
        "[MC2_TASK_CACHE][Plan] tag[%s], input[%p,%llu], output[%p,%llu].", plan.tag.c_str(), plan.addrs[0],
        static_cast<unsigned long long>(plan.sizes[0]), plan.addrs[1], static_cast<unsigned long long>(plan.sizes[1]));
    return HCCL_SUCCESS;
}

namespace {
struct ShapeWords {
    std::vector<uint64_t> words;
    void AddU8(uint8_t v) { words.push_back(v); }
    void AddU32(uint32_t v) { words.push_back(v); }
    void AddU64(uint64_t v) { words.push_back(v); }
    void AddBool(bool v) { words.push_back(v ? 1U : 0U); }
    void AddDouble(double v)
    {
        uint64_t bits;
        std::memcpy(&bits, &v, sizeof(bits));
        words.push_back(bits);
    }
    void AddString(const char* v, size_t maxLen)
    {
        const size_t n = v == nullptr ? 0U : strnlen(v, maxLen);
        words.push_back(n);
        for (size_t i = 0; i < n; ++i) {
            words.push_back(static_cast<unsigned char>(v[i]));
        }
    }
};
struct ShapeHash {
    size_t operator()(const std::vector<uint64_t>& key) const
    {
        uint64_t h = 0x9e3779b97f4a7c15ULL;
        for (auto v : key) {
            h ^= v + 0x9e3779b97f4a7c15ULL + (h << 6U) + (h >> 2U);
        }
        return static_cast<size_t>(h);
    }
};
std::atomic<uint64_t> replayBytes{0};
constexpr uint64_t TOTAL_REPLAY_BUDGET = 64U * 1024U * 1024U;
struct ReplayImage {
    void* handle = nullptr;
    uint64_t bytes = 0;
    std::vector<uint64_t> key;
    bool accounted = false;
    ~ReplayImage()
    {
        if (accounted) {
            replayBytes.fetch_sub(bytes);
        }
        if (handle != nullptr) {
            (void)Mc2HcommReplayV1(4, &handle, nullptr, nullptr, 0, nullptr);
        }
    }
};
std::mutex domainRegistryMutex;
thread_local Mc2ReplayScope* currentReplayScope = nullptr;
} // namespace

class Mc2ReplayDomain {
public:
    std::mutex mutex;
    void* comm = nullptr;
    std::shared_ptr<const AlgResourceCtxSerializable> resources;
    std::unordered_map<std::vector<uint64_t>, std::unique_ptr<ReplayImage>, ShapeHash> entries;
    ReplayImage* recent = nullptr;
    ShapeWords shape;
    uint64_t bytes = 0;
    bool supported = false;
    void Clear()
    {
        recent = nullptr;
        entries.clear();
        bytes = 0;
    }
};
namespace {
std::unordered_map<const AlgResourceCtxSerializable*, std::shared_ptr<Mc2ReplayDomain>> replayDomains;
constexpr uint64_t DOMAIN_BUDGET = 10U * 1024U * 1024U;
constexpr size_t MAX_DOMAINS = 256U;
void ClearReplayDomains(void* comm)
{
    std::lock_guard<std::mutex> registryLock(domainRegistryMutex);
    for (auto it = replayDomains.begin(); it != replayDomains.end();) {
        if (it->second->comm != comm) {
            ++it;
            continue;
        }
        {
            std::lock_guard<std::mutex> domainLock(it->second->mutex);
            it->second->Clear();
        }
        it = replayDomains.erase(it);
    }
}
} // namespace

std::shared_ptr<Mc2ReplayDomain> BindMc2ReplayDomain(
    const OpParam& param, std::shared_ptr<const AlgResourceCtxSerializable> resources)
{
    if (!resources || !Mc2HcommHasReplayV1()) {
        return {};
    }
    std::lock_guard<std::mutex> registryLock(domainRegistryMutex);
    const auto found = replayDomains.find(resources.get());
    if (found != replayDomains.end()) {
        return found->second;
    }
    // Only unused domains may be removed. An active scope owns another reference.
    if (replayDomains.size() >= MAX_DOMAINS) {
        for (auto it = replayDomains.begin(); it != replayDomains.end();) {
            if (it->second.use_count() == 1) {
                it = replayDomains.erase(it);
                break;
            }
            ++it;
        }
        if (replayDomains.size() >= MAX_DOMAINS) {
            return {};
        }
    }
    auto domain = std::make_shared<Mc2ReplayDomain>();
    domain->comm = param.hcclComm;
    domain->resources = resources;
    Digest128Builder resource("mc2-bound-resource-v1");
    domain->supported = AddResourceFields(param, *resources, resource);
    domain->shape.words.reserve(1024);
    replayDomains.emplace(resources.get(), domain);
    return domain;
}

Mc2ReplayScope::Mc2ReplayScope(std::shared_ptr<Mc2ReplayDomain> domain)
    : domain_(std::move(domain)), previous_(currentReplayScope)
{
    if (domain_) {
        lock_ = std::unique_lock<std::mutex>(domain_->mutex);
    }
    currentReplayScope = this;
}
Mc2ReplayScope::~Mc2ReplayScope() { currentReplayScope = previous_; }
bool Mc2ReplayScope::Active() { return currentReplayScope != nullptr && currentReplayScope->domain_ != nullptr; }

HcclResult RunMc2BoundReplay(
    const OpParam& param, const AlgResourceCtxSerializable& resources, const std::function<HcclResult()>& execute)
{
    auto& scope = *currentReplayScope;
    auto& domain = *scope.domain_;
    {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        Mc2ServerStageTiming::DetailScope timing(Mc2ServerStageTiming::BATCH_START);
        // MC2_CACHE_DETAIL_DIAG END
        CHK_RET(static_cast<HcclResult>(HcommBatchModeStart(param.algTag)));
    }
    auto finish = [&](HcclResult result) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        Mc2ServerStageTiming::DetailScope timing(Mc2ServerStageTiming::BATCH_END);
        // MC2_CACHE_DETAIL_DIAG END
        const auto end = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
        return result == HCCL_SUCCESS ? end : result;
    };
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope policyTiming(Mc2ServerStageTiming::CACHE_POLICY);
    // MC2_CACHE_DETAIL_DIAG END
    if (!domain.supported || GetExternalInputHcclAicpuCacheEnable() == 0U || !IsSupportedOp(param.opType) ||
        param.opMode == OpMode::OFFLOAD || RequiresAicpuReduction(param) || param.supportSymmetricMemory ||
        param.hcclComm != domain.comm || param.inputPtr == nullptr || param.outputPtr == nullptr ||
        resources.topoInfo.userRankSize <= 1U || resources.threads.empty() ||
        (param.engine != CommEngine::COMM_ENGINE_AICPU_TS && param.engine != CommEngine::COMM_ENGINE_CCU)) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        return finish(execute());
    }
    Mc2AicpuTaskCachePlan addressPlan;
    addressPlan.addrs = {{param.inputPtr, param.outputPtr}};
    ReplayImage* image = nullptr;
    const bool regular = param.opType != HCCL_CMD_ALLTOALL && param.opType != HCCL_CMD_ALLTOALLV;
    const bool reusableRepeat = regular || param.opType == HCCL_CMD_ALLTOALL;
    if (reusableRepeat && scope.repeatReady_) {
        addressPlan.sizes = scope.repeatSizes_;
        image = static_cast<ReplayImage*>(scope.repeatEntry_);
    } else {
        domain.shape.words.clear();
        uint64_t inputSpan = 0, outputSpan = 0;
        const bool valid =
            regular ? GetRegularSpans(param, resources.topoInfo.userRankSize, inputSpan, outputSpan, domain.shape) :
                      GetAllToAllVSpans(param, resources.topoInfo.userRankSize, inputSpan, outputSpan, domain.shape);
        if (!valid || (inputSpan == 0 && outputSpan == 0)) {
            // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
            policyTiming.Stop();
            // MC2_CACHE_DETAIL_DIAG END
            return finish(execute());
        }
        addressPlan.sizes = {{inputSpan, outputSpan}};
        AddShapeFields(param, inputSpan, outputSpan, domain.shape);
        // OpParam fields are not owned by the immutable resource object.
        domain.shape.AddString(param.algTag, ALG_TAG_LENGTH);
        domain.shape.AddString(param.commName, COMM_INDENTIFIER_MAX_LENGTH);
        domain.shape.AddU64(static_cast<uint64_t>(param.opThread));
        domain.shape.AddU64(reinterpret_cast<uintptr_t>(param.hcclBuff.addr));
        domain.shape.AddU64(param.hcclBuff.size);
        domain.shape.AddU64(reinterpret_cast<uintptr_t>(param.inputSymWindow));
        domain.shape.AddU64(reinterpret_cast<uintptr_t>(param.outputSymWindow));
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        Mc2ServerStageTiming::DetailScope lookupTiming(Mc2ServerStageTiming::CACHE_LOOKUP);
        // MC2_CACHE_DETAIL_DIAG END
        if (domain.recent && domain.recent->key == domain.shape.words) {
            image = domain.recent;
        } else {
            const auto found = domain.entries.find(domain.shape.words);
            if (found != domain.entries.end()) {
                image = found->second.get();
            }
        }
    }
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    policyTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    if (IsOverlapping(addressPlan)) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        return finish(execute());
    }
    if (image) {
        Mc2ServerStageTiming::Cache(true);
        scope.repeatReady_ = reusableRepeat;
        scope.repeatSizes_ = addressPlan.sizes;
        scope.repeatEntry_ = image;
        domain.recent = image;
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        Mc2ServerStageTiming::DetailScope replayTiming(Mc2ServerStageTiming::HIT_REPLAY);
        // MC2_CACHE_DETAIL_DIAG END
        const auto replayResult = static_cast<HcclResult>(
            Mc2HcommReplayV1(3, &image->handle, addressPlan.addrs.data(), addressPlan.sizes.data(), 2, nullptr));
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        replayTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        const auto result = finish(replayResult);
        if (result != HCCL_SUCCESS) {
            scope.repeatReady_ = false;
            domain.Clear();
        }
        return result;
    }
    if (domain.bytes >= DOMAIN_BUDGET || replayBytes.load() >= TOTAL_REPLAY_BUDGET) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        return finish(execute());
    }
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope createTiming(Mc2ServerStageTiming::CAPTURE_CREATE);
    // MC2_CACHE_DETAIL_DIAG END
    std::unique_ptr<ReplayImage> captured(new ReplayImage);
    auto ret = static_cast<HcclResult>(Mc2HcommReplayV1(0, &captured->handle, nullptr, nullptr, 0, nullptr));
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    createTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    if (ret != HCCL_SUCCESS) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        return finish(execute());
    }
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope beginTiming(Mc2ServerStageTiming::CAPTURE_BEGIN);
    // MC2_CACHE_DETAIL_DIAG END
    ret = static_cast<HcclResult>(
        Mc2HcommReplayV1(1, &captured->handle, addressPlan.addrs.data(), addressPlan.sizes.data(), 2, nullptr));
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    beginTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    if (ret != HCCL_SUCCESS) {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        policyTiming.Stop();
        // MC2_CACHE_DETAIL_DIAG END
        return finish(execute());
    }
    Mc2ServerStageTiming::Cache(false);
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope executeTiming(Mc2ServerStageTiming::MISS_EXECUTE);
    // MC2_CACHE_DETAIL_DIAG END
    ret = execute();
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    executeTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope flushTiming(Mc2ServerStageTiming::BATCH_END);
    // MC2_CACHE_DETAIL_DIAG END
    const auto flush = static_cast<HcclResult>(HcommBatchModeEnd(param.algTag));
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    flushTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    if (ret != HCCL_SUCCESS || flush != HCCL_SUCCESS) {
        return ret != HCCL_SUCCESS ? ret : flush;
    }
    {
        // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
        Mc2ServerStageTiming::DetailScope timing(Mc2ServerStageTiming::BATCH_START);
        // MC2_CACHE_DETAIL_DIAG END
        CHK_RET(static_cast<HcclResult>(HcommBatchModeStart(param.algTag)));
    }
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope endTiming(Mc2ServerStageTiming::CAPTURE_END);
    // MC2_CACHE_DETAIL_DIAG END
    ret = static_cast<HcclResult>(Mc2HcommReplayV1(2, &captured->handle, nullptr, nullptr, 0, &captured->bytes));
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    endTiming.Stop();
    // MC2_CACHE_DETAIL_DIAG END
    ret = finish(ret);
    if (ret != HCCL_SUCCESS) {
        return ret;
    }
    // MC2_CACHE_DETAIL_DIAG BEGIN: temporary diagnostic; remove this marked block.
    Mc2ServerStageTiming::DetailScope publishTiming(Mc2ServerStageTiming::CACHE_PUBLISH);
    // MC2_CACHE_DETAIL_DIAG END
    captured->bytes += domain.shape.words.size() * sizeof(uint64_t) * 2U + sizeof(ReplayImage);
    if (captured->bytes > DOMAIN_BUDGET - domain.bytes) {
        return HCCL_SUCCESS;
    }
    uint64_t total = replayBytes.load();
    do {
        if (captured->bytes > TOTAL_REPLAY_BUDGET - total) {
            return HCCL_SUCCESS;
        }
    } while (!replayBytes.compare_exchange_weak(total, total + captured->bytes));
    captured->accounted = true;
    captured->key = domain.shape.words;
    domain.bytes += captured->bytes;
    image = captured.get();
    auto tableKey = captured->key;
    domain.entries.emplace(std::move(tableKey), std::move(captured));
    domain.recent = image;
    scope.repeatReady_ = reusableRepeat;
    scope.repeatEntry_ = image;
    scope.repeatSizes_ = addressPlan.sizes;
    return HCCL_SUCCESS;
}

HcclResult Mc2AicpuTaskCacheLookup(const Mc2AicpuTaskCachePlan& plan, bool& isHit)
{
    isHit = false;
    return static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheLookup(plan.tag.c_str(), &isHit));
}

HcclResult Mc2AicpuTaskCacheStart(const Mc2AicpuTaskCachePlan& plan)
{
    return static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheStart(
        plan.tag.c_str(), const_cast<void**>(plan.addrs.data()), const_cast<uint64_t*>(plan.sizes.data()),
        CACHE_ADDR_COUNT));
}

HcclResult Mc2AicpuTaskCacheEnd(const Mc2AicpuTaskCachePlan& plan)
{
    return static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheEnd(plan.tag.c_str()));
}

HcclResult Mc2AicpuTaskCacheExecute(const Mc2AicpuTaskCachePlan& plan)
{
    return static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheExecute(
        plan.tag.c_str(), const_cast<void**>(plan.addrs.data()), const_cast<uint64_t*>(plan.sizes.data()),
        CACHE_ADDR_COUNT));
}

HcclResult Mc2AicpuTaskCacheClear(const Mc2AicpuTaskCachePlan& plan)
{
    return static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheClear(plan.tag.c_str()));
}

Mc2AicpuTaskCacheManager& Mc2AicpuTaskCacheManager::Instance()
{
    static Mc2AicpuTaskCacheManager manager;
    return manager;
}

Mc2AicpuTaskCacheManager::Mc2AicpuTaskCacheManager(size_t maxTagCount, size_t maxTagBytes)
    : maxTagCount_(maxTagCount), maxTagBytes_(maxTagBytes)
{}

Mc2TaskCacheReserveResult Mc2AicpuTaskCacheManager::TryReserve(void* comm, const std::string& tag)
{
    if (comm == nullptr || tag.empty()) {
        return Mc2TaskCacheReserveResult::FULL;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    auto& tags = commTags_[comm];
    if (tags.find(tag) != tags.end()) {
        return Mc2TaskCacheReserveResult::EXISTING;
    }
    const bool countFull = trackedTagCount_ >= maxTagCount_;
    const bool bytesFull = tag.size() > maxTagBytes_ - std::min(trackedTagBytes_, maxTagBytes_);
    if (countFull || bytesFull) {
        if (!capacityWarningReported_) {
            HCCL_INFO(
                "[MC2_TASK_CACHE][Capacity] bypass new tags, trackedCount[%zu/%zu], trackedBytes[%zu/%zu].",
                trackedTagCount_, maxTagCount_, trackedTagBytes_, maxTagBytes_);
            capacityWarningReported_ = true;
        } else {
            HCCL_DEBUG(
                "[MC2_TASK_CACHE][Capacity] bypass tag[%s], trackedCount[%zu/%zu], trackedBytes[%zu/%zu].", tag.c_str(),
                trackedTagCount_, maxTagCount_, trackedTagBytes_, maxTagBytes_);
        }
        if (tags.empty()) {
            commTags_.erase(comm);
        }
        return Mc2TaskCacheReserveResult::FULL;
    }
    tags.insert(tag);
    ++trackedTagCount_;
    trackedTagBytes_ += tag.size();
    return Mc2TaskCacheReserveResult::RESERVED;
}

void Mc2AicpuTaskCacheManager::Unregister(void* comm, const std::string& tag)
{
    if (comm == nullptr || tag.empty()) {
        return;
    }
    std::lock_guard<std::mutex> lock(mutex_);
    const auto commIter = commTags_.find(comm);
    if (commIter == commTags_.end()) {
        return;
    }
    const auto tagIter = commIter->second.find(tag);
    if (tagIter == commIter->second.end()) {
        return;
    }
    trackedTagBytes_ -= tagIter->size();
    --trackedTagCount_;
    commIter->second.erase(tagIter);
    if (commIter->second.empty()) {
        commTags_.erase(commIter);
    }
    capacityWarningReported_ = false;
}

HcclResult Mc2AicpuTaskCacheManager::Evict(void* comm)
{
    ClearReplayDomains(comm);
    std::unordered_set<std::string> tags;
    {
        std::lock_guard<std::mutex> lock(mutex_);
        const auto iter = commTags_.find(comm);
        if (iter == commTags_.end()) {
            return HCCL_SUCCESS;
        }
        tags = iter->second;
    }

    HcclResult result = HCCL_SUCCESS;
    for (const std::string& tag : tags) {
        const HcclResult ret = static_cast<HcclResult>(Mc2HcommAicpuTsTaskCacheClear(tag.c_str()));
        if (ret != HCCL_SUCCESS) {
            HCCL_ERROR("[MC2_TASK_CACHE][Evict] comm[%p], tag[%s], ret[%d].", comm, tag.c_str(), ret);
            result = ret;
            continue;
        }
        Unregister(comm, tag);
    }
    return result;
}

} // namespace mc2_ops_hccl

extern "C" __attribute__((visibility("default"))) uint32_t Mc2AicpuTaskCacheClearKernel(void** comm)
{
    if (comm == nullptr || *comm == nullptr) {
        HCCL_ERROR("[MC2_TASK_CACHE][EvictKernel] invalid comm argument.");
        return static_cast<uint32_t>(HCCL_E_PARA);
    }
    return static_cast<uint32_t>(mc2_ops_hccl::Mc2AicpuTaskCacheManager::Instance().Evict(*comm));
}
