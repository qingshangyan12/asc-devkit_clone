/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_reduce_mesh1d_adapter.h"
#include "ccu_all_reduce.h"

#include "ccu_kernel_kfc_all_reduce_mesh1d_mem2mem.h"
#include "ccu_primitives_stub.h"
#include "ccu_kernel_alg_base.h"
#include "alg_param.h"

#include <cmath>
#include <cstring>

namespace HcclSim {
namespace CcuSt {

namespace {

void* ConvertAllReduceArg(const void* productArg, uint32_t argNum)
{
    if (productArg == nullptr) {
        return nullptr;
    }
    const auto* base = static_cast<const mc2_ops_hccl::CcuKernelArgBase*>(productArg);
    auto* config = new AllReduceKernelConfig();
    const auto* raw = static_cast<const uint8_t*>(productArg);
    std::memcpy(&config->rankId, raw + sizeof(mc2_ops_hccl::CcuKernelArgBase), sizeof(uint32_t));
    uint64_t rankSize = 0;
    std::memcpy(&rankSize, raw + sizeof(mc2_ops_hccl::CcuKernelArgBase) + sizeof(uint32_t), sizeof(uint64_t));
    config->rankSize = static_cast<uint32_t>(rankSize);
    config->dataType = HCCL_DATA_TYPE_FP16;
    config->outputType = HCCL_DATA_TYPE_FP16;
    config->reduceOp = HCCL_REDUCE_SUM;
    for (uint32_t i = 0; i < base->channelCount; ++i) {
        config->channels.push_back(base->channels[i]);
    }
    return config;
}

uint32_t DataTypeSize(HcclDataType dt)
{
    switch (dt) {
        case HCCL_DATA_TYPE_INT8:
            return 1;
        case HCCL_DATA_TYPE_FP16:
        case HCCL_DATA_TYPE_BFP16:
            return 2;
        case HCCL_DATA_TYPE_INT32:
        case HCCL_DATA_TYPE_FP32:
            return 4;
        default:
            return 0;
    }
}

void WriteElement(uint8_t* buf, uint64_t byteOffset, uint64_t value, HcclDataType dt)
{
    const uint32_t elemSize = DataTypeSize(dt);
    if (elemSize == 0) {
        buf[byteOffset] = static_cast<uint8_t>(value);
        return;
    }
    std::memcpy(buf + byteOffset, &value, elemSize);
}

bool g_registered = []() {
    RegisterCaptureFunction("CcuKernelKfcServer", CaptureAllReduceMesh1DKernel, ConvertAllReduceArg, [](void* p) {
        delete static_cast<AllReduceKernelConfig*>(p);
    });

    CcuStAllReduce::RegisterVariant(
        "CcuSchedAllReduceSoleMesh",
        [](const CcuStScenario& scenario, const std::vector<KernelHandle>& handles) -> ScenarioData {
            return BuildAllReduceMesh1DScenario(scenario, handles);
        });
    return true;
}();

} // namespace

Result CaptureAllReduceMesh1DKernel(void* kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto& config = *static_cast<AllReduceKernelConfig*>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize || config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    std::array<AscendC::ccu::Variable, ALL_REDUCE_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);

    const CcuResult result = mc2_ops_hccl::CcuKfcAllReduceMesh1DMem2MemKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6], arguments[7],
        arguments[8], arguments[9], arguments[10], arguments[11], arguments[12], arguments[13], arguments[14],
        arguments[15], arguments[16], arguments[17], arguments[18], arguments[19], arguments[20], arguments[21],
        arguments[22], arguments[23], arguments[24], arguments[25], config.channels.data(),
        static_cast<uint32_t>(config.channels.size()), config.rankSize, config.rankId, config.dataType,
        config.outputType, config.reduceOp);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

std::vector<uint64_t> PrepareAllReduceTaskArgs(const AllReduceLaunchConfig& config)
{
    return {
        config.inputAddress,
        config.outputAddress,
        config.token,
        config.scratchAddress,
        config.currentRankSliceInputOffset,
        config.currentRankSliceOutputOffset,
        config.normalSliceSize,
        config.lastSliceSize,
        config.mySliceSize,
        config.sliceOffset,
        config.isInputOutputEqual,
        config.goSize[0],
        config.goSize[1],
        config.goSize[2],
        config.goSize[3],
        config.chunkSize,
        config.tailSize,
        config.chunkLoopNum,
        config.fullGoSize[0],
        config.fullGoSize[1],
        config.fullGoSize[2],
        config.fullGoSize[3],
        config.tailGoSize[0],
        config.tailGoSize[1],
        config.tailGoSize[2],
        config.tailGoSize[3],
    };
}

ScenarioData BuildAllReduceMesh1DScenario(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(scenario.topoMeta);
    const uint32_t typeSize = DataTypeSize(scenario.dataType);
    const uint64_t totalDataSize = scenario.count * typeSize;

    const uint64_t normalSliceCount = (scenario.count + rankSize - 1) / rankSize;
    const uint64_t normalSliceSize = normalSliceCount * typeSize;
    const uint64_t normalRankCount = (normalSliceCount == 0) ? 0 : scenario.count / normalSliceCount;
    const uint64_t lastSliceSize = (normalRankCount == rankSize) ?
                                       totalDataSize - (normalRankCount - 1) * normalSliceSize :
                                       totalDataSize - normalRankCount * normalSliceSize;

    const uint64_t mySliceSize =
        (scenario.count < rankSize) ?
            ((scenario.count <= 1) ? typeSize : scenario.count * typeSize) :
            ((0 < normalRankCount && normalRankCount < rankSize) ? lastSliceSize : normalSliceSize);

    const uint64_t chunkSize = (16 * 1024 * 1024) / rankSize / 128 * 128;
    const uint64_t fullChunkCount = (mySliceSize == 0) ? 0 : (mySliceSize - 1) / chunkSize;
    const uint64_t tailSize = mySliceSize - fullChunkCount * chunkSize;
    const uint64_t chunkCount = (mySliceSize == 0) ? 0 : fullChunkCount + 1;

    constexpr uint64_t scratchSize = 16 * 1024 * 1024;
    constexpr uint64_t sourceBaseOffset = 7;
    constexpr uint64_t destinationBaseOffset = 11;
    constexpr uint64_t guard = 64;

    mc2_ops_hccl::LoopGroupConfig goConfig{};
    goConfig.msInterleave = mc2_ops_hccl::CCU_MS_INTERLEAVE;
    goConfig.loopCount = mc2_ops_hccl::CCU_MS_DEFAULT_LOOP_COUNT;
    goConfig.memSlice = mc2_ops_hccl::CCU_MS_SIZE;

    mc2_ops_hccl::LoopGroupConfig chunkGoConfig{};
    chunkGoConfig.msInterleave = mc2_ops_hccl::CCU_MS_INTERLEAVE;
    chunkGoConfig.loopCount = 16;
    chunkGoConfig.memSlice = mc2_ops_hccl::CCU_MS_SIZE;

    const auto goSizeVec = mc2_ops_hccl::CalGoSize(mySliceSize, goConfig);
    const auto fullGoSizeVec =
        (chunkSize > 0) ? mc2_ops_hccl::CalGoSize(chunkSize, chunkGoConfig) : mc2_ops_hccl::CalGoSize(0, chunkGoConfig);
    const auto tailGoSizeVec = mc2_ops_hccl::CalGoSize(tailSize, chunkGoConfig);

    ScenarioData data;
    data.memories.resize(rankSize);
    data.launches.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.reduceOp = scenario.reduceType;

    const uint64_t inputSize = sourceBaseOffset + totalDataSize + guard;
    const uint64_t outputSize = destinationBaseOffset + totalDataSize + guard;
    const uint64_t scratchOffset = 0;

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory& memory = data.memories[rank];
        memory.input.assign(inputSize, 0);
        memory.output.assign(outputSize, 0);
        memory.parameter.assign(scratchSize, 0);
        memory.inPlace = scenario.inPlace;

        const uint64_t rankSliceSize = (rank < normalRankCount) ? normalSliceSize : lastSliceSize;
        const uint64_t inputSliceOffset = rank * normalSliceSize;

        for (uint64_t byte = 0; byte < rankSliceSize; ++byte) {
            memory.input[sourceBaseOffset + inputSliceOffset + byte] =
                static_cast<uint8_t>((17U + rank * 41U + byte) & 0xFF);
        }

        AllReduceLaunchConfig config;
        config.inputAddress = memory.InputAddress() + sourceBaseOffset;
        config.outputAddress = memory.OutputAddress() + destinationBaseOffset;
        config.token = 0x1000U + rank;
        config.scratchAddress = memory.ParameterAddress() + scratchOffset;
        config.currentRankSliceInputOffset = 0;
        config.currentRankSliceOutputOffset = 0;
        config.normalSliceSize = normalSliceSize;
        config.lastSliceSize = lastSliceSize;
        config.mySliceSize = mySliceSize;
        config.sliceOffset = rank * normalSliceSize;
        config.isInputOutputEqual = 0;
        config.goSize = {goSizeVec[0], goSizeVec[1], goSizeVec[2], goSizeVec[3]};
        config.chunkSize = chunkSize;
        config.tailSize = tailSize;
        config.chunkLoopNum = UINT64_MAX - chunkCount;
        config.fullGoSize = {fullGoSizeVec[0], fullGoSizeVec[1], fullGoSizeVec[2], fullGoSizeVec[3]};
        config.tailGoSize = {tailGoSizeVec[0], tailGoSizeVec[1], tailGoSizeVec[2], tailGoSizeVec[3]};
        config.rankId = rank;

        data.srcOffsets[rank][0] = sourceBaseOffset + inputSliceOffset;
        data.dstOffsets[rank][0] = destinationBaseOffset + inputSliceOffset;

        data.launches[rank] = RankLaunch{handles[rank], PrepareAllReduceTaskArgs(config), &memory};
    }

    return data;
}

} // namespace CcuSt
} // namespace HcclSim
