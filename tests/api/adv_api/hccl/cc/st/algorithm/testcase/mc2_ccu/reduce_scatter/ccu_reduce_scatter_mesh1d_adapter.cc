/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_reduce_scatter_mesh1d_adapter.h"
#include "ccu_reduce_scatter.h"

#include "ccu_primitives_stub.h"
#include "ccu_kernel_alg_base.h"
#include "ccu_kernel_kfc_server.h"
#include "ccu_kernel_kfc_reduce_scatter_mesh1d_mem2mem.h"
#include "alg_param.h"
#include "mc2_type.h"

#include <algorithm>
#include <cstring>

namespace HcclSim {
namespace CcuSt {

namespace {

void* ConvertReduceScatterArg(const void* productArg, uint32_t argNum)
{
    (void)argNum;
    if (productArg == nullptr) {
        return nullptr;
    }
    const auto* src = static_cast<const mc2_ops_hccl::CcuKernelArgKfcServer*>(productArg);
    auto* config = new ReduceScatterKernelConfig();
    config->rankId = src->rankId;
    config->rankSize = static_cast<uint32_t>(src->rankSize);
    config->dataType = src->opParam.DataDes.dataType;
    config->outputDataType = src->opParam.DataDes.outputType;
    if (config->outputDataType == HcclDataType::HCCL_DATA_TYPE_RESERVED) {
        config->outputDataType = config->dataType;
    }
    config->reduceOp = src->opParam.reduceType;
    for (uint32_t i = 0; i < src->channelCount; ++i) {
        config->channels.push_back(src->channels[i]);
    }
    return config;
}

std::vector<ReduceScatterLaunchConfig> g_launchConfigs;

bool g_registered = []() {
    RegisterCaptureFunction(
        "CcuKernelKfcReduceScatterMesh1DMem2Mem", CaptureReduceScatterKernel, ConvertReduceScatterArg,
        [](void* p) { delete static_cast<ReduceScatterKernelConfig*>(p); });

    CcuStReduceScatter::RegisterVariant(
        "CcuKfcReduceScatterMesh1DMem2Mem",
        [](const CcuStScenario& scenario, const std::vector<KernelHandle>& handles) -> ScenarioData {
            return BuildReduceScatterScenario(scenario, handles);
        });
    return true;
}();

} // namespace

Result CaptureReduceScatterKernel(void* kernelArg)
{
    if (kernelArg == nullptr) {
        return Result::PARAM_ERROR;
    }
    const auto& config = *static_cast<const ReduceScatterKernelConfig*>(kernelArg);
    if (config.rankSize < 2 || config.rankId >= config.rankSize ||
        config.channels.size() != config.rankSize - 1) {
        return Result::PARAM_ERROR;
    }

    std::array<AscendC::ccu::Variable, REDUCE_SCATTER_TASK_ARG_COUNT> arguments;
    for (uint32_t index = 0; index < arguments.size(); ++index) {
        if (AscendC::ccu::LoadArg(arguments[index], index) != Result::SUCCESS) {
            return Result::CONTRACT_ERROR;
        }
    }
    CompilerContext::Current().ConfigureProgram(config.rankId, config.rankSize, config.channels);

    const CcuResult result = mc2_ops_hccl::CcuReduceScatterMesh1DMem2MemKernel(
        arguments[0], arguments[1], arguments[2], arguments[3], arguments[4], arguments[5], arguments[6],
        config.channels.data(), static_cast<uint32_t>(config.channels.size()), config.rankSize, config.rankId,
        config.dataType, config.outputDataType, config.reduceOp);
    return result == CCU_SUCCESS ? Result::SUCCESS : Result::CONTRACT_ERROR;
}

std::vector<uint64_t> PrepareReduceScatterTaskArgs(const ReduceScatterLaunchConfig& config)
{
    return {
        config.inputAddress, config.outputAddress, config.token, config.scratchAddress,
        config.currentRankSliceInputOffset, config.tailSize, config.repeatNum};
}

ScenarioData BuildReduceScatterScenario(const CcuStScenario& scenario, const std::vector<KernelHandle>& handles)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(scenario.topoMeta);
    const uint32_t typeSize = DATATYPE_SIZE_TABLE[scenario.dataType];
    const uint64_t perRankBytes = scenario.count * typeSize / rankSize;
    const uint64_t totalInputBytes = perRankBytes * rankSize;

    constexpr uint64_t scratchSize = Hccl::MC2_WORKSPACE_SIZE;
    constexpr uint64_t guard = 128;
    constexpr uint64_t inputGuard = 32;
    constexpr uint64_t outputGuard = 32;

    const uint64_t chunkSize = (scratchSize / rankSize) / 128 * 128;

    ScenarioData data;
    data.memories.resize(rankSize);
    data.launches.resize(rankSize);
    g_launchConfigs.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.reduceOp = scenario.reduceType;

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory& memory = data.memories[rank];
        memory.input.assign(totalInputBytes + inputGuard, 0);
        memory.output.assign(perRankBytes + outputGuard, 0);
        memory.parameter.assign(scratchSize / sizeof(uint64_t), 0);

        for (uint32_t slice = 0; slice < rankSize; ++slice) {
            const uint64_t sliceOffset = inputGuard + static_cast<uint64_t>(slice) * perRankBytes;
            for (uint64_t byte = 0; byte < perRankBytes; ++byte) {
                memory.input[sliceOffset + byte] = static_cast<uint8_t>(
                    (rank + 1) * 10 + (slice + 1) * 3 + static_cast<uint8_t>(byte % 200));
            }
        }

        for (uint32_t src = 0; src < rankSize; ++src) {
            for (uint32_t dst = 0; dst < rankSize; ++dst) {
                data.srcOffsets[src][dst] = inputGuard + static_cast<uint64_t>(dst) * perRankBytes;
                data.dstOffsets[src][dst] = 0;
            }
        }

        ReduceScatterLaunchConfig& cfg = g_launchConfigs[rank];
        cfg.inputAddress = memory.InputAddress() + inputGuard;
        cfg.outputAddress = memory.OutputAddress();
        cfg.token = 0x2000U + rank;
        cfg.scratchAddress = memory.ParameterAddress();
        cfg.currentRankSliceInputOffset = static_cast<uint64_t>(rank) * perRankBytes;
        cfg.tailSize = perRankBytes;
        cfg.repeatNum = UINT64_MAX - 1; // single iteration

        data.launches[rank] = RankLaunch{handles[rank], PrepareReduceScatterTaskArgs(cfg), &memory};
    }
    return data;
}

} // namespace CcuSt
} // namespace HcclSim
