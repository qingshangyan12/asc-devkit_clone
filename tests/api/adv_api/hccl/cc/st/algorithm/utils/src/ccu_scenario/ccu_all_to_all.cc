/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all.h"

#include <algorithm>
#include <cstring>
#include <vector>

namespace HcclSim {
namespace CcuSt {

ScenarioData CcuStAllToAll::BuildScenarioData(const CcuStScenario &scenario,
                                               const std::vector<KernelHandle> &handles)
{
    const uint32_t rankSize = scenario.rankSize;
    const uint64_t sliceSize = scenario.sizes[0][0];
    constexpr uint64_t sourceBaseOffset = 7;
    constexpr uint64_t destinationBaseOffset = 11;
    constexpr uint64_t guard = 32;

    ScenarioData data;
    data.memories.resize(rankSize);
    data.launches.resize(rankSize);
    configs_.resize(rankSize);

    const uint64_t inputSize = sourceBaseOffset + rankSize * sliceSize + guard;
    const uint64_t outputSize = destinationBaseOffset + rankSize * sliceSize + guard;

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory &memory = data.memories[rank];
        memory.input.assign(inputSize, 0x5a);
        memory.output.assign(outputSize, 0xa5);
        AllToAllLaunchConfig &config = configs_[rank];
        config.inputAddress = memory.InputAddress() + sourceBaseOffset;
        config.outputAddress = memory.OutputAddress();
        config.token = 0x1000U + rank;
        config.sliceSize = sliceSize;
        config.srcStride = sliceSize;
        config.srcOffset = sourceBaseOffset;
        constexpr uint64_t MAX_DMA_TRANS_SIZE = 256ULL * 1024 * 1024;
        config.writeLoopCount = UINT64_MAX - (sliceSize / MAX_DMA_TRANS_SIZE);
        config.dstOffset = destinationBaseOffset + static_cast<uint64_t>(rank) * sliceSize;
        config.rankId = rank;
        for (uint32_t dest = 0; dest < rankSize; ++dest) {
            const uint64_t offset = sourceBaseOffset + static_cast<uint64_t>(dest) * sliceSize;
            for (uint64_t byte = 0; byte < sliceSize; ++byte) {
                memory.input[offset + byte] =
                    static_cast<uint8_t>(17U + rank * 41U + dest * 13U + byte);
            }
        }
        data.launches[rank] = RankLaunch{handles[rank],
            PrepareAllToAllTaskArgs(config), &memory};
    }
    return data;
}

CheckResult CcuStAllToAll::DoCheckRegistration(const Program &program, uint32_t rank,
                                                uint32_t rankSize)
{
    AllToAllKernelConfig kernelConfig;
    kernelConfig.rankId = rank;
    kernelConfig.rankSize = rankSize;
    kernelConfig.loadFromMem = false;
    for (uint32_t peer = 0; peer < rankSize; ++peer) {
        if (peer != rank) {
            kernelConfig.channels.push_back(MakeChannel(rank, peer));
        }
    }
    return checker_.CheckRegistration(program, kernelConfig);
}

CheckResult CcuStAllToAll::DoCheckTaskArgs(const Program &program, uint32_t rank,
                                            const ScenarioData &data)
{
    return checker_.CheckTaskArgs(program, data.launches[rank].taskArgs, configs_[rank]);
}

CheckResult CcuStAllToAll::DoCaptureRuntimeExpectation(const ScenarioData &data)
{
    return checker_.CaptureRuntimeExpectation(data.memories, configs_, expectation_);
}

CheckResult CcuStAllToAll::DoCheckRuntime(const ScenarioData &data, RuntimeWorld &world)
{
    return checker_.CheckRuntime(expectation_, data.memories, world);
}

} // namespace CcuSt
} // namespace HcclSim
