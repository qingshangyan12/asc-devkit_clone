/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file be in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_st_all_to_all_v.h"
#include "ccu_kernel_proxy.h"
#include "ccu_sim_world.h"
#include "ccu_all_to_all_v_semantics_checker.h"

#include <algorithm>

namespace HcclSim {
namespace CcuSt {

namespace {

struct ScenarioData {
    std::vector<RankMemory> memories;
    std::vector<AllToAllVLaunchConfig> configs;
    std::vector<RankLaunch> launches;
};

ScenarioData BuildScenarioData(const CcuStScenario &scenario,
                                const std::vector<KernelHandle> &handles)
{
    const uint32_t rankSize = scenario.rankSize;
    constexpr uint64_t sourceBaseOffset = 7;
    constexpr uint64_t destinationBaseOffset = 11;
    constexpr uint64_t gap = 5;
    constexpr uint64_t guard = 32;

    ScenarioData data;
    data.memories.resize(rankSize);
    data.configs.resize(rankSize);
    data.launches.resize(rankSize);

    std::vector<std::vector<uint64_t>> sendOffsets(rankSize, std::vector<uint64_t>(rankSize));
    std::vector<std::vector<uint64_t>> recvOffsets(rankSize, std::vector<uint64_t>(rankSize));
    std::vector<uint64_t> inputSizes(rankSize, sourceBaseOffset + guard);
    std::vector<uint64_t> outputSizes(rankSize, destinationBaseOffset + guard);

    for (uint32_t src = 0; src < rankSize; ++src) {
        uint64_t offset = 0;
        for (uint32_t dst = 0; dst < rankSize; ++dst) {
            sendOffsets[src][dst] = offset;
            offset += scenario.sizes[src][dst] + gap;
        }
        inputSizes[src] += offset;
    }
    for (uint32_t dst = 0; dst < rankSize; ++dst) {
        uint64_t offset = 0;
        for (uint32_t src = 0; src < rankSize; ++src) {
            recvOffsets[dst][src] = offset;
            offset += scenario.sizes[src][dst] + gap;
        }
        outputSizes[dst] += offset;
    }

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory &memory = data.memories[rank];
        memory.input.assign(inputSizes[rank], 0x5a);
        memory.output.assign(outputSizes[rank], 0xa5);
        AllToAllVLaunchConfig &config = data.configs[rank];
        config.inputAddress = memory.InputAddress();
        config.outputAddress = memory.OutputAddress();
        config.token = 0x1000U + rank;
        config.sourceBaseOffset = sourceBaseOffset;
        config.destinationBaseOffset = destinationBaseOffset;
        config.rankId = rank;
        config.peers.resize(rankSize);
        memory.parameter.reserve(rankSize * ALL_TO_ALL_V_EXT_FIELD_COUNT);
        for (uint32_t peer = 0; peer < rankSize; ++peer) {
            AllToAllVPeerParam &peerParam = config.peers[peer];
            peerParam.sendSize = scenario.sizes[rank][peer];
            peerParam.reserved = 0xcccc0000U + peer;
            peerParam.sendOffset = sendOffsets[rank][peer];
            peerParam.recvOffset = recvOffsets[rank][peer];
            memory.parameter.push_back(peerParam.sendSize);
            memory.parameter.push_back(peerParam.reserved);
            memory.parameter.push_back(peerParam.sendOffset);
            memory.parameter.push_back(peerParam.recvOffset);
            for (uint64_t byte = 0; byte < peerParam.sendSize; ++byte) {
                memory.input[sourceBaseOffset + peerParam.sendOffset + byte] =
                    static_cast<uint8_t>(17U + rank * 41U + peer * 13U + byte);
            }
        }
        config.parameterAddress = memory.ParameterAddress();
        data.launches[rank] = RankLaunch{handles[rank],
            PrepareAllToAllVTaskArgs(config), &memory};
    }
    return data;
}

} // namespace

void CcuStAllToAllV::VerifyScenario(const CcuStScenario &scenario)
{
    // Full implementation deferred to Task 10.
    // The selector/CalcRes/channel/capture flow will be wired up in Task 7
    // via PrepareKernel, then refactored into here in Task 10.
}

} // namespace CcuSt
} // namespace HcclSim
