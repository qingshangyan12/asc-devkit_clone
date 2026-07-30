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

#include "ccu_all_to_all_v_mc2_checker.h"
#include "ccu_kfc_checker.h"
#include "ccu_kfc_server_adapter.h"

#include <algorithm>
#include <memory>

using namespace HcclSim::CcuSt;

namespace {

constexpr uint64_t FP16_ALL_TO_ALL_V_OP_ID = (UINT64_C(1) << 8U) | CCU_KFC_ALL_TO_ALL_V;

class ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST : public ::testing::Test {
protected:
    void SetUp() override
    {
        RegisterManager::Global().Reset();
    }
};

std::vector<ChannelHandle> BuildChannels(uint32_t rankId, uint32_t rankSize)
{
    std::vector<ChannelHandle> channels;
    for (uint32_t peer = 0; peer < rankSize; ++peer) {
        if (peer != rankId) {
            channels.push_back(MakeChannel(rankId, peer));
        }
    }
    return channels;
}

struct CapturedKernels {
    std::vector<KernelHandle> server;
    std::vector<KernelHandle> collective;
};

CapturedKernels CaptureKernels(uint32_t rankSize)
{
    constexpr InstanceHandle instance = 41;
    CapturedKernels kernels;
    kernels.server.resize(rankSize);
    kernels.collective.resize(rankSize);
    std::vector<KfcServerKernelConfig> serverConfigs(rankSize);
    std::vector<AllToAllVKernelConfig> collectiveConfigs(rankSize);
    RegisterManager &manager = RegisterManager::Global();
    if (manager.RegisterStart(instance) != Result::SUCCESS) {
        return {};
    }
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        serverConfigs[rank] = {rank, rankSize, FP16_ALL_TO_ALL_V_OP_ID, BuildChannels(rank, rankSize)};
        collectiveConfigs[rank] = {rank, rankSize, BuildChannels(rank, rankSize)};
        if (manager.Register(instance, "CcuKfcServerKernel", CaptureKfcServerKernel,
                &serverConfigs[rank], kernels.server[rank]) != Result::SUCCESS ||
            manager.Register(instance, "CcuAlltoallVMesh1D", CaptureAllToAllVMesh1DKernel,
                &collectiveConfigs[rank], kernels.collective[rank]) != Result::SUCCESS) {
            return {};
        }
    }
    return manager.RegisterEnd(instance) == Result::SUCCESS ? kernels : CapturedKernels{};
}

struct KfcScenario {
    std::vector<RankMemory> memories;
    std::vector<AllToAllVLaunchConfig> configs;
    std::vector<CcuKfcMemory> queueMemory;
    std::vector<std::unique_ptr<CcuKfcQueueProxy>> queueOwners;
    std::vector<CcuKfcQueueProxy *> queues;
    std::vector<CcuKfcRegistration> registrations;
    CapturedKernels kernels;
};

KfcScenario BuildScenario(const std::vector<std::vector<uint64_t>> &sizes)
{
    const uint32_t rankSize = static_cast<uint32_t>(sizes.size());
    KfcScenario scenario;
    scenario.kernels = CaptureKernels(rankSize);
    if (scenario.kernels.server.size() != rankSize) {
        return scenario;
    }
    scenario.memories.resize(rankSize);
    scenario.configs.resize(rankSize);
    scenario.queueMemory.resize(rankSize);
    scenario.queueOwners.resize(rankSize);
    scenario.queues.resize(rankSize);
    scenario.registrations.resize(rankSize);

    std::vector<std::vector<uint64_t>> sendOffsets(rankSize, std::vector<uint64_t>(rankSize));
    std::vector<std::vector<uint64_t>> recvOffsets(rankSize, std::vector<uint64_t>(rankSize));
    for (uint32_t source = 0; source < rankSize; ++source) {
        uint64_t offset = 0;
        for (uint32_t destination = 0; destination < rankSize; ++destination) {
            sendOffsets[source][destination] = offset;
            offset += sizes[source][destination] + 3U;
        }
        scenario.memories[source].input.assign(offset + 32U, 0x5a);
    }
    for (uint32_t destination = 0; destination < rankSize; ++destination) {
        uint64_t offset = 0;
        for (uint32_t source = 0; source < rankSize; ++source) {
            recvOffsets[destination][source] = offset;
            offset += sizes[source][destination] + 3U;
        }
        scenario.memories[destination].output.assign(offset + 32U, 0xa5);
    }

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory &memory = scenario.memories[rank];
        AllToAllVLaunchConfig &config = scenario.configs[rank];
        config.rankId = rank;
        config.inputAddress = memory.InputAddress();
        config.outputAddress = memory.OutputAddress();
        config.peers.resize(rankSize);
        memory.parameter.reserve(rankSize * ALL_TO_ALL_V_EXT_FIELD_COUNT);
        for (uint32_t peer = 0; peer < rankSize; ++peer) {
            AllToAllVPeerParam &peerParam = config.peers[peer];
            peerParam.sendSize = sizes[rank][peer];
            peerParam.reserved = 0xface0000U + peer;
            peerParam.sendOffset = sendOffsets[rank][peer];
            peerParam.recvOffset = recvOffsets[rank][peer];
            memory.parameter.push_back(peerParam.sendSize);
            memory.parameter.push_back(peerParam.reserved);
            memory.parameter.push_back(peerParam.sendOffset);
            memory.parameter.push_back(peerParam.recvOffset);
            for (uint64_t byte = 0; byte < peerParam.sendSize; ++byte) {
                memory.input[peerParam.sendOffset + byte] =
                    static_cast<uint8_t>(17U + rank * 41U + peer * 13U + byte);
            }
        }
        config.parameterAddress = memory.ParameterAddress();

        scenario.queueOwners[rank].reset(new CcuKfcQueueProxy(scenario.queueMemory[rank]));
        scenario.queues[rank] = scenario.queueOwners[rank].get();
        CcuKfcStartupArgs startup;
        startup.xnAddr = scenario.queueMemory[rank].XnAddress();
        startup.ckeAddr = scenario.queueMemory[rank].CkeAddress();
        startup.token = 0x1200U + rank;
        EXPECT_EQ(MakeKfcRegistration(41, scenario.kernels.server[rank], scenario.kernels.collective[rank],
            FP16_ALL_TO_ALL_V_OP_ID, startup, BuildAllToAllVKfcTaskArgs,
            scenario.registrations[rank]), Result::SUCCESS);

        std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT> message{};
        message[0] = FP16_ALL_TO_ALL_V_OP_ID;
        message[1] = config.inputAddress;
        message[2] = config.outputAddress;
        message[3] = config.sourceBaseOffset;
        message[4] = config.destinationBaseOffset;
        message[9] = config.parameterAddress;
        EXPECT_EQ(scenario.queues[rank]->Commit(message), Result::SUCCESS);
    }
    return scenario;
}

void DispatchAndVerify(KfcScenario &scenario)
{
    CcuKfcChecker kfcChecker;
    CcuAllToAllVMc2Checker messageChecker;
    for (uint32_t rank = 0; rank < scenario.registrations.size(); ++rank) {
        CheckResult result = kfcChecker.CheckRegistration(scenario.registrations[rank], *scenario.queues[rank]);
        ASSERT_TRUE(result.Ok()) << result.message;
        result = messageChecker.CheckMessage(scenario.queues[rank]->Message(0), scenario.memories[rank],
            scenario.registrations.size(), FP16_ALL_TO_ALL_V_OP_ID);
        ASSERT_TRUE(result.Ok()) << result.message;
        result = messageChecker.CheckPeerTable(scenario.memories[rank],
            scenario.configs[rank].parameterAddress, scenario.configs[rank].peers);
        ASSERT_TRUE(result.Ok()) << result.message;
    }
    ASSERT_TRUE(kfcChecker.CheckContext(scenario.registrations).Ok());

    CcuAllToAllVSemanticsChecker dataChecker;
    AllToAllVRuntimeExpectation expectation;
    ASSERT_TRUE(dataChecker.CaptureRuntimeExpectation(
        scenario.memories, scenario.configs, expectation).Ok());
    CcuKfcSimWorld world;
    ASSERT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories), Result::SUCCESS)
        << world.LastError();
    CheckResult result = dataChecker.CheckRuntime(expectation, scenario.memories, world.AlgorithmWorld());
    ASSERT_TRUE(result.Ok()) << result.message;
    for (CcuKfcQueueProxy *queue : scenario.queues) {
        EXPECT_TRUE(kfcChecker.CheckQueue(*queue).Ok());
        EXPECT_EQ(queue->Wait(), Result::SUCCESS);
    }
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, real_server_ir_matches_current_queue_and_dispatch_contract)
{
    CapturedKernels kernels = CaptureKernels(2);
    ASSERT_EQ(kernels.server.size(), 2U);
    const Program *program = RegisterManager::Global().GetProgram(kernels.server[0]);
    ASSERT_NE(program, nullptr);
    CcuKfcChecker checker;
    CheckResult result = checker.CheckServerProgram(*program, FP16_ALL_TO_ALL_V_OP_ID);
    EXPECT_TRUE(result.Ok()) << result.message;

    Program corrupt = *program;
    auto stride = std::find_if(corrupt.operations.begin(), corrupt.operations.end(), [](const Operation &operation) {
        return operation.code == OpCode::ASSIGN_IMM &&
            operation.immediate == CCU_KFC_WORDS_PER_SLOT * sizeof(uint64_t);
    });
    ASSERT_NE(stride, corrupt.operations.end());
    stride->immediate = 7;
    EXPECT_FALSE(checker.CheckServerProgram(corrupt, FP16_ALL_TO_ALL_V_OP_ID).Ok());
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, startup_arguments_bind_real_server_and_queue_resources)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    ASSERT_EQ(scenario.registrations.size(), 2U);
    for (uint32_t rank = 0; rank < 2; ++rank) {
        CheckResult result = CcuKfcChecker().CheckRegistration(
            scenario.registrations[rank], *scenario.queues[rank]);
        EXPECT_TRUE(result.Ok()) << result.message;
        EXPECT_EQ(scenario.registrations[rank].startup.Pack().size(), 6U);
        EXPECT_NE(scenario.registrations[rank].serverKernel,
            scenario.registrations[rank].collectiveKernel);
    }
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, two_rank_asymmetric_dispatch_end_to_end)
{
    KfcScenario scenario = BuildScenario({{19, 37}, {11, 23}});
    DispatchAndVerify(scenario);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, four_rank_zero_length_peer_dispatch_end_to_end)
{
    KfcScenario scenario = BuildScenario(
        {{5, 0, 17, 9}, {3, 7, 0, 11}, {0, 13, 19, 2}, {23, 0, 6, 15}});
    DispatchAndVerify(scenario);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, peer_table_uses_current_send_reserved_send_offset_recv_offset_abi)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    CcuAllToAllVMc2Checker checker;
    EXPECT_TRUE(checker.CheckPeerTable(scenario.memories[0], scenario.memories[0].ParameterAddress(),
        scenario.configs[0].peers).Ok());
    std::swap(scenario.memories[0].parameter[1], scenario.memories[0].parameter[2]);
    EXPECT_FALSE(checker.CheckPeerTable(scenario.memories[0], scenario.memories[0].ParameterAddress(),
        scenario.configs[0].peers).Ok());
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, queue_executes_multiple_slots_and_wraps_after_64)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    CcuKfcSimWorld world;
    for (uint32_t turn = 0; turn < CCU_KFC_QUEUE_DEPTH + 1U; ++turn) {
        ASSERT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories), Result::SUCCESS)
            << world.LastError();
        for (CcuKfcQueueProxy *queue : scenario.queues) {
            ASSERT_EQ(queue->Wait(), Result::SUCCESS);
            std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT> message = queue->Message(0);
            message[0] = FP16_ALL_TO_ALL_V_OP_ID;
            const uint32_t rank = queue == scenario.queues[0] ? 0U : 1U;
            message[1] = scenario.memories[rank].InputAddress();
            message[2] = scenario.memories[rank].OutputAddress();
            message[9] = scenario.memories[rank].ParameterAddress();
            ASSERT_EQ(queue->Commit(message), Result::SUCCESS);
        }
    }
    EXPECT_EQ(scenario.queues[0]->ConsumerSlot(), 1U);
    EXPECT_EQ(scenario.queues[0]->WaitSlot(), 1U);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, queue_rejects_reuse_of_uncompleted_slot)
{
    CcuKfcMemory memory;
    CcuKfcQueueProxy queue(memory);
    std::array<uint64_t, CCU_KFC_WORDS_PER_SLOT> message{};
    message[0] = FP16_ALL_TO_ALL_V_OP_ID;
    for (uint32_t slot = 0; slot < CCU_KFC_QUEUE_DEPTH; ++slot) {
        ASSERT_EQ(queue.Commit(message), Result::SUCCESS);
    }
    EXPECT_EQ(queue.Commit(message), Result::STATE_ERROR);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, dispatch_rejects_unregistered_op_id)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    auto message = scenario.queues[0]->Message(0);
    ASSERT_EQ(scenario.queues[0]->MarkCompleted(0), Result::SUCCESS);
    ASSERT_EQ(scenario.queues[0]->Wait(), Result::SUCCESS);
    message[0] += 1U;
    ASSERT_EQ(scenario.queues[0]->Commit(message), Result::SUCCESS);
    CcuKfcSimWorld world;
    EXPECT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories),
        Result::CONTRACT_ERROR);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, message_checker_rejects_truncated_peer_table)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    auto message = scenario.queues[0]->Message(0);
    message[9] += scenario.memories[0].parameter.size() * sizeof(uint64_t);
    CheckResult result = CcuAllToAllVMc2Checker().CheckMessage(
        message, scenario.memories[0], 2, FP16_ALL_TO_ALL_V_OP_ID);
    EXPECT_EQ(result.result, Result::OUT_OF_RANGE);
}

TEST_F(ST_MC2_CCU_ALL_TO_ALL_V_KFC_TEST, collective_finalize_stops_without_completion_signal)
{
    KfcScenario scenario = BuildScenario({{7, 11}, {13, 5}});
    CcuKfcSimWorld world;
    ASSERT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories), Result::SUCCESS);
    for (CcuKfcQueueProxy *queue : scenario.queues) {
        ASSERT_EQ(queue->Wait(), Result::SUCCESS);
        ASSERT_EQ(queue->Finalize(), Result::SUCCESS);
    }
    ASSERT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories), Result::SUCCESS);
    EXPECT_TRUE(world.Stopped());
    for (CcuKfcQueueProxy *queue : scenario.queues) {
        EXPECT_EQ(queue->State(1), CcuKfcSlotState::FINALIZED);
        EXPECT_EQ(queue->Memory().completionCke[1], 0U);
        EXPECT_EQ(queue->Wait(), Result::STATE_ERROR);
    }
    EXPECT_EQ(world.DispatchOne(scenario.registrations, scenario.queues, scenario.memories), Result::STATE_ERROR);
}

} // namespace
