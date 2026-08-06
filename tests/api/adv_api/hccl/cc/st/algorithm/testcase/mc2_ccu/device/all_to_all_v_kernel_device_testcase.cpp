/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "gtest/gtest.h"

#include "ccu_all_to_all_v_semantics_checker.h"

#include <algorithm>
#include <vector>

using namespace HcclSim::CcuSt;

namespace {

class ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST : public ::testing::Test {
protected:
    void SetUp() override
    {
        RegisterManager::Global().Reset();
    }
};

AllToAllVKernelConfig BuildKernelConfig(uint32_t rankId, uint32_t rankSize)
{
    AllToAllVKernelConfig config;
    config.rankId = rankId;
    config.rankSize = rankSize;
    for (uint32_t peer = 0; peer < rankSize; ++peer) {
        if (peer != rankId) {
            config.channels.push_back(MakeChannel(rankId, peer));
        }
    }
    return config;
}

std::vector<KernelHandle> RegisterAllRanks(uint32_t rankSize)
{
    constexpr InstanceHandle instance = 17;
    RegisterManager &manager = RegisterManager::Global();
    if (manager.RegisterStart(instance) != Result::SUCCESS) {
        return {};
    }
    std::vector<KernelHandle> handles(rankSize);
    std::vector<AllToAllVKernelConfig> configs(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        configs[rank] = BuildKernelConfig(rank, rankSize);
        if (manager.Register(instance, "CcuAlltoallVMesh1D", CaptureAllToAllVMesh1DKernel,
                &configs[rank], handles[rank]) != Result::SUCCESS) {
            return {};
        }
    }
    return manager.RegisterEnd(instance) == Result::SUCCESS ? handles : std::vector<KernelHandle>{};
}

struct Scenario {
    std::vector<RankMemory> memories;
    std::vector<AllToAllVLaunchConfig> configs;
    std::vector<RankLaunch> launches;
};

Scenario BuildScenario(const std::vector<std::vector<uint64_t>> &sizes,
    const std::vector<KernelHandle> &handles)
{
    const uint32_t rankSize = static_cast<uint32_t>(sizes.size());
    constexpr uint64_t sourceBaseOffset = 7;
    constexpr uint64_t destinationBaseOffset = 11;
    constexpr uint64_t gap = 5;
    constexpr uint64_t guard = 32;
    Scenario scenario;
    scenario.memories.resize(rankSize);
    scenario.configs.resize(rankSize);
    scenario.launches.resize(rankSize);

    std::vector<std::vector<uint64_t>> sendOffsets(rankSize, std::vector<uint64_t>(rankSize));
    std::vector<std::vector<uint64_t>> recvOffsets(rankSize, std::vector<uint64_t>(rankSize));
    std::vector<uint64_t> inputSizes(rankSize, sourceBaseOffset + guard);
    std::vector<uint64_t> outputSizes(rankSize, destinationBaseOffset + guard);
    for (uint32_t source = 0; source < rankSize; ++source) {
        uint64_t offset = 0;
        for (uint32_t destination = 0; destination < rankSize; ++destination) {
            sendOffsets[source][destination] = offset;
            offset += sizes[source][destination] + gap;
        }
        inputSizes[source] += offset;
    }
    for (uint32_t destination = 0; destination < rankSize; ++destination) {
        uint64_t offset = 0;
        for (uint32_t source = 0; source < rankSize; ++source) {
            recvOffsets[destination][source] = offset;
            offset += sizes[source][destination] + gap;
        }
        outputSizes[destination] += offset;
    }

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory &memory = scenario.memories[rank];
        memory.input.assign(inputSizes[rank], 0x5a);
        memory.output.assign(outputSizes[rank], 0xa5);
        AllToAllVLaunchConfig &config = scenario.configs[rank];
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
            peerParam.sendSize = sizes[rank][peer];
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
        scenario.launches[rank] = RankLaunch{handles[rank], PrepareAllToAllVTaskArgs(config), &memory};
    }
    return scenario;
}

void VerifyScenario(const std::vector<std::vector<uint64_t>> &sizes, bool corruptOutput = false)
{
    const uint32_t rankSize = static_cast<uint32_t>(sizes.size());
    const std::vector<KernelHandle> handles = RegisterAllRanks(rankSize);
    ASSERT_EQ(handles.size(), rankSize);
    Scenario scenario = BuildScenario(sizes, handles);
    CcuAllToAllVSemanticsChecker checker;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        const Program *program = RegisterManager::Global().GetProgram(handles[rank]);
        ASSERT_NE(program, nullptr);
        const AllToAllVKernelConfig kernelConfig = BuildKernelConfig(rank, rankSize);
        CheckResult registration = checker.CheckRegistration(*program, kernelConfig);
        ASSERT_TRUE(registration.Ok()) << registration.message;
        CheckResult taskArgs = checker.CheckTaskArgs(*program, scenario.launches[rank].taskArgs,
            scenario.configs[rank]);
        ASSERT_TRUE(taskArgs.Ok()) << taskArgs.message;
    }
    AllToAllVRuntimeExpectation expectation;
    CheckResult capture = checker.CaptureRuntimeExpectation(scenario.memories, scenario.configs, expectation);
    ASSERT_TRUE(capture.Ok()) << capture.message;
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(scenario.launches), Result::SUCCESS) << world.LastError();
    CheckResult runtime = checker.CheckRuntime(expectation, scenario.memories, world);
    ASSERT_TRUE(runtime.Ok()) << runtime.message;
    if (corruptOutput) {
        scenario.memories[0].output[scenario.configs[0].destinationBaseOffset] ^= 1U;
        EXPECT_FALSE(checker.CheckRuntime(expectation, scenario.memories, world).Ok());
    }
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, registration_ir_matches_current_mesh1d_kernel)
{
    const std::vector<KernelHandle> handles = RegisterAllRanks(4);
    ASSERT_EQ(handles.size(), 4U);
    const Program *program = RegisterManager::Global().GetProgram(handles[2]);
    ASSERT_NE(program, nullptr);
    CheckResult result = CcuAllToAllVSemanticsChecker().CheckRegistration(*program, BuildKernelConfig(2, 4));
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, ten_dynamic_arguments_fit_one_task_mission)
{
    AllToAllVLaunchConfig config;
    config.inputAddress = 1;
    config.outputAddress = 2;
    config.token = 3;
    config.sourceBaseOffset = 4;
    config.destinationBaseOffset = 5;
    config.parameterAddress = 6;
    config.goSize = {{7, 8, 9, 10}};
    const std::vector<uint64_t> arguments = PrepareAllToAllVTaskArgs(config);
    ASSERT_EQ(arguments.size(), ALL_TO_ALL_V_TASK_ARG_COUNT);
    const std::vector<TaskParam> tasks = PackTaskArgs(arguments);
    ASSERT_EQ(tasks.size(), 1U);
    EXPECT_EQ(tasks[0].copiedArgCount, ALL_TO_ALL_V_TASK_ARG_COUNT);
    EXPECT_EQ(tasks[0].args[9], 10U);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, two_rank_asymmetric_sizes_and_offsets)
{
    VerifyScenario({{19, 37}, {11, 23}});
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, four_rank_supports_zero_length_peers)
{
    VerifyScenario({{5, 0, 17, 9}, {3, 7, 0, 11}, {0, 13, 19, 2}, {23, 0, 6, 15}});
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, launch_rejects_out_of_range_parameter_table)
{
    const std::vector<KernelHandle> handles = RegisterAllRanks(2);
    Scenario scenario = BuildScenario({{7, 9}, {11, 13}}, handles);
    scenario.configs[0].parameterAddress += scenario.memories[0].parameter.size() * sizeof(uint64_t);
    scenario.launches[0].taskArgs = PrepareAllToAllVTaskArgs(scenario.configs[0]);
    RuntimeWorld world;
    EXPECT_EQ(world.Launch(scenario.launches), Result::OUT_OF_RANGE);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, runtime_checker_rejects_output_corruption)
{
    VerifyScenario({{8, 12}, {16, 20}}, true);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, checker_rejects_task_argument_rebinding)
{
    const std::vector<KernelHandle> handles = RegisterAllRanks(2);
    Scenario scenario = BuildScenario({{7, 9}, {11, 13}}, handles);
    const Program *program = RegisterManager::Global().GetProgram(handles[0]);
    ASSERT_NE(program, nullptr);
    std::vector<uint64_t> corrupted = scenario.launches[0].taskArgs;
    std::swap(corrupted[3], corrupted[4]);
    EXPECT_FALSE(CcuAllToAllVSemanticsChecker().CheckTaskArgs(
        *program, corrupted, scenario.configs[0]).Ok());
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KERNEL_TEST, launch_rejects_missing_dynamic_argument)
{
    const std::vector<KernelHandle> handles = RegisterAllRanks(2);
    Scenario scenario = BuildScenario({{7, 9}, {11, 13}}, handles);
    scenario.launches[1].taskArgs.pop_back();
    RuntimeWorld world;
    EXPECT_EQ(world.Launch(scenario.launches), Result::CONTRACT_ERROR);
}

} // namespace
