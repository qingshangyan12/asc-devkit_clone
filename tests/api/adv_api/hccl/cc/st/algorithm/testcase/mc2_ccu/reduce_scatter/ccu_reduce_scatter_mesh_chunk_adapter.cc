/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "ccu_reduce_scatter_mesh_chunk_adapter.h"

#include "ccu_reduce_scatter.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_read_reduce.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_ms_accum.h"
#include "ccu_primitives_stub.h"
#include "ccu_temp_kfc_reduce_scatter_mesh_1D_read_reduce.h"
#include "ccu_temp_kfc_reduce_scatter_mesh_1D_ms_accum.h"

#include <algorithm>
#include <cstring>
#include <limits>

namespace HcclSim {
namespace CcuSt {
namespace {

constexpr uint64_t CHUNK_MIN_SLICE_ALIGN = 128;
constexpr uint64_t CHUNK_MAX_ALIGN = 4096;

// 产品kernelArg → ST运行配置：通道按子rank升序重建（跳过自身），与kernel内
// InitResource按peerId递增消费channelIdx的顺序一致。
void FillChunkKernelConfig(
    ReduceScatterChunkKernelConfig& config, uint32_t rankId, uint32_t rankSize, const mc2_ops_hccl::OpParam& opParam)
{
    config.rankId = rankId;
    config.rankSize = rankSize;
    config.dataType = opParam.DataDes.dataType;
    config.outputType = opParam.DataDes.outputType == HcclDataType::HCCL_DATA_TYPE_RESERVED ?
                           opParam.DataDes.dataType :
                           opParam.DataDes.outputType;
    config.reduceType = opParam.reduceType;
    for (uint32_t subRank = 0; subRank < config.rankSize; ++subRank) {
        if (subRank != config.rankId) {
            config.channels.push_back(MakeChannel(config.rankId, subRank));
        }
    }
}

void* ConvertReduceScatterChunkPipelinedArg(const void* productArg, uint32_t)
{
    if (productArg == nullptr) {
        return nullptr;
    }
    const auto* source = static_cast<const mc2_ops_hccl::CcuKernelArgKfcReduceScatterMesh1DReadReduce*>(productArg);
    if (source->subCommRanks.empty()) {
        return nullptr;
    }
    auto* config = new ReduceScatterChunkKernelConfig();
    FillChunkKernelConfig(*config, source->rankId, static_cast<uint32_t>(source->rankSize), source->opParam);
    return config;
}

void* ConvertReduceScatterTwoShotArg(const void* productArg, uint32_t)
{
    if (productArg == nullptr) {
        return nullptr;
    }
    const auto* source = static_cast<const mc2_ops_hccl::CcuKernelArgKfcReduceScatterMesh1DMsAccum*>(productArg);
    if (source->subCommRanks.empty()) {
        return nullptr;
    }
    auto* config = new ReduceScatterChunkKernelConfig();
    FillChunkKernelConfig(*config, source->rankId, static_cast<uint32_t>(source->rankSize), source->opParam);
    return config;
}

bool g_registered = []() {
    RegisterCaptureFunction(
        "CcuKernelKfcReduceScatterMesh1DReadReducePipelined", CaptureCcuKfcReduceScatterChunkPipelinedKernel,
        ConvertReduceScatterChunkPipelinedArg, [](void* pointer) { delete static_cast<ReduceScatterChunkKernelConfig*>(pointer); });
    RegisterCaptureFunction(
        "CcuKernelKfcReduceScatterMesh1DMsAccum", CaptureCcuKfcReduceScatterTwoShotKernel,
        ConvertReduceScatterTwoShotArg, [](void* pointer) { delete static_cast<ReduceScatterChunkKernelConfig*>(pointer); });
    CcuStReduceScatter::RegisterVariant("CcuSchedReduceScatterSoleMeshChunkPipelined", BuildReduceScatterChunkPipelinedScenario);
    CcuStReduceScatter::RegisterVariant("CcuSchedReduceScatterSoleMeshChunkTwoShot", BuildReduceScatterTwoShotScenario);
    return true;
}();

} // namespace

Result CaptureCcuKfcReduceScatterChunkPipelinedKernel(void* kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto& config = *static_cast<ReduceScatterChunkKernelConfig*>(kernelArg);
    if (config.rankSize < 2 || config.rankSize > 8 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }
    std::array<AscendC::ccu::Variable, REDUCE_SCATTER_CHUNK_PIPELINED_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);
    const uint32_t chunkCount = 4U * (config.rankSize - 1U);
    const CcuResult result = mc2_ops_hccl::CcuReduceScatterMesh1DReadReducePipelinedKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7],
        arguments[8], config.channels.data(), static_cast<uint32_t>(config.channels.size()), config.rankSize,
        config.rankId, chunkCount, config.dataType, config.outputType, config.reduceType);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

Result CaptureCcuKfcReduceScatterTwoShotKernel(void* kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto& config = *static_cast<ReduceScatterChunkKernelConfig*>(kernelArg);
    if (config.rankSize < 2 || config.rankSize > 8 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }
    std::array<AscendC::ccu::Variable, REDUCE_SCATTER_TWO_SHOT_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);
    const CcuResult result = mc2_ops_hccl::CcuReduceScatterMesh1DMsAccumKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7],
        config.channels.data(), static_cast<uint32_t>(config.channels.size()), config.rankSize, config.rankId,
        config.dataType, config.outputType, config.reduceType);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

// taskArgs 按kernel形参顺序排列，模拟AIV侧CcuPrepareForReduceScatterChunkPipelinedM2M写入HBM、
// KFC dispatch 逐槽转发的参数（[0]=input [1]=output [2]=token [3]=localInputOffset
// [4]=sliceSize [5]=chunkBase [6]=chunkAlign [7]=chunkExtraUnits [8]=chunkTail）。
std::vector<uint64_t> PrepareReduceScatterChunkPipelinedTaskArgs(const ReduceScatterChunkPipelinedLaunchConfig& config)
{
    return {
        config.inputAddress,
        config.outputAddress,
        config.token,
        config.localInputOffset,
        config.sliceSize,
        config.chunkBaseSize,
        config.chunkAlign,
        config.chunkExtraUnits,
        config.chunkTail};
}

// [0]=input [1]=output [2]=token [3]=localInputOffset [4..7]=goSize
std::vector<uint64_t> PrepareReduceScatterTwoShotTaskArgs(const ReduceScatterTwoShotLaunchConfig& config)
{
    return {
        config.inputAddress,
        config.outputAddress,
        config.token,
        config.localInputOffset,
        config.goSize[0],
        config.goSize[1],
        config.goSize[2],
        config.goSize[3]};
}

// chunk 切分公式与宿主 CcuPrepareForReduceScatterChunkPipelinedM2M 逐行对齐
// （级联对齐 4096→128→dataTypeSize，chunkCount = 4*(rankSize-1)）。
struct ChunkSplitParams {
    uint64_t baseSize{0};
    uint64_t align{0};
    uint64_t extraUnits{0};
    uint64_t tail{0};
};

ChunkSplitParams CalculateChunkSplitParams(uint64_t sliceSize, uint64_t chunkCount, uint64_t dataTypeSize)
{
    ChunkSplitParams params;
    uint64_t align = CHUNK_MAX_ALIGN;
    if (sliceSize / chunkCount < align) {
        align = CHUNK_MIN_SLICE_ALIGN;
    }
    if (sliceSize / chunkCount < align) {
        align = dataTypeSize;
    }
    params.align = align;
    params.baseSize = (sliceSize / chunkCount / align) * align;
    const uint64_t remainder = sliceSize - params.baseSize * chunkCount;
    params.extraUnits = remainder / align;
    params.tail = remainder - params.extraUnits * align;
    return params;
}

// 元素按各数据类型原码编码（小端）：取值 1..5 编为精确可表示的整数（FP16/BFP16 走 normal 段，
// 避开 checker Float16ToFloat 的 subnormal 放大缺陷），8-rank 求和≤40 各类型均精确，
// 归约结果与加法顺序无关——Chunk 系 kernel 的 peer 访问按轮转序，与 checker 的升序不同。
uint64_t EncodeElementValue(uint64_t value, HcclDataType dataType)
{
    switch (dataType) {
        case HCCL_DATA_TYPE_INT8:
            return value & 0xFFU;
        case HCCL_DATA_TYPE_FP16: {
            uint32_t exp = 0;
            uint64_t v = value;
            while ((v >> 1U) != 0U) {
                v >>= 1U;
                ++exp;
            }
            return static_cast<uint64_t>(((exp + 15U) << 10U) | ((value << (10U - exp)) & 0x3FFU));
        }
        case HCCL_DATA_TYPE_BFP16: {
            const float f = static_cast<float>(value);
            uint32_t bits = 0;
            std::memcpy(&bits, &f, sizeof(bits));
            return static_cast<uint64_t>(bits >> 16U);
        }
        case HCCL_DATA_TYPE_FP32: {
            const float f = static_cast<float>(value);
            uint32_t bits = 0;
            std::memcpy(&bits, &f, sizeof(bits));
            return static_cast<uint64_t>(bits);
        }
        default:
            return value;
    }
}

// 填充公共输入内存与 srcOffsets/dstOffsets：Mesh kernel 输出落点恒为 offset 0，
// 区别于 NHR 的 rank*sliceSize。
void FillChunkScenarioMemories(
    ScenarioData& data, const CcuStScenario& scenario, uint32_t rankSize, uint64_t sliceSize, uint64_t typeSize)
{
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.reduceOp = scenario.reduceType;
    constexpr uint64_t guard = 32;
    const uint64_t totalSize = sliceSize * rankSize;
    const uint64_t elemCount = typeSize == 0 ? 0 : sliceSize / typeSize;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory& memory = data.memories[rank];
        memory.input.assign(guard + totalSize, 0x5a);
        memory.output.assign(guard + totalSize, 0xa5);
        // Chunk 系 kernel 不使用 scratch，仅保留最小 parameter 缓冲使地址有效。
        memory.parameter.resize(1, 0);
        for (uint32_t slice = 0; slice < rankSize; ++slice) {
            data.srcOffsets[rank][slice] = guard + slice * sliceSize;
            data.dstOffsets[rank][slice] = guard;
            for (uint64_t elem = 0; elem < elemCount; ++elem) {
                const uint64_t encoded = EncodeElementValue(1U + ((rank * 3U + slice * 5U + elem) % 5U), scenario.dataType);
                const uint64_t elemBase = guard + slice * sliceSize + elem * typeSize;
                for (uint64_t byte = 0; byte < typeSize; ++byte) {
                    memory.input[elemBase + byte] = static_cast<uint8_t>((encoded >> (byte * 8U)) & 0xFFU);
                }
            }
        }
    }
}

ScenarioData BuildReduceScatterChunkPipelinedScenario(
    const CcuStScenario& scenario, const std::vector<KernelHandle>& handles)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(scenario.topoMeta);
    const uint64_t typeSize = DATATYPE_SIZE_TABLE[scenario.dataType];
    const uint64_t sliceSize = scenario.count * typeSize;
    constexpr uint64_t guard = 32;
    const uint64_t chunkCount = 4ULL * (rankSize - 1ULL);
    const ChunkSplitParams chunk = CalculateChunkSplitParams(sliceSize, chunkCount, typeSize);

    ScenarioData data;
    FillChunkScenarioMemories(data, scenario, rankSize, sliceSize, typeSize);
    data.launches.resize(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        ReduceScatterChunkPipelinedLaunchConfig config;
        config.inputAddress = data.memories[rank].InputAddress() + guard;
        config.outputAddress = data.memories[rank].OutputAddress() + guard;
        config.token = 0x1000U + rank;
        config.localInputOffset = rank * sliceSize; // strideCount=0
        config.sliceSize = sliceSize;
        config.chunkBaseSize = chunk.baseSize;
        config.chunkAlign = chunk.align;
        config.chunkExtraUnits = chunk.extraUnits;
        config.chunkTail = chunk.tail;
        data.launches[rank] =
            RankLaunch{handles[rank], PrepareReduceScatterChunkPipelinedTaskArgs(config), &data.memories[rank]};
    }
    return data;
}

ScenarioData BuildReduceScatterTwoShotScenario(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(scenario.topoMeta);
    const uint64_t typeSize = DATATYPE_SIZE_TABLE[scenario.dataType];
    const uint64_t sliceSize = scenario.count * typeSize;
    constexpr uint64_t guard = 32;

    ScenarioData data;
    FillChunkScenarioMemories(data, scenario, rankSize, sliceSize, typeSize);
    data.launches.resize(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        ReduceScatterTwoShotLaunchConfig config;
        config.inputAddress = data.memories[rank].InputAddress() + guard;
        config.outputAddress = data.memories[rank].OutputAddress() + guard;
        config.token = 0x1000U + rank;
        config.localInputOffset = rank * sliceSize; // strideCount=0
        // goSize 口径与宿主 CcuPrepareForReduceScatterTwoShotM2M 一致：CalcGoSize(slice, 16, 4096)
        config.goSize = CalculateReduceScatterMeshGoSize(sliceSize);
        data.launches[rank] = RankLaunch{handles[rank], PrepareReduceScatterTwoShotTaskArgs(config), &data.memories[rank]};
    }
    return data;
}

} // namespace CcuSt
} // namespace HcclSim
