/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_st_all_to_all_v.h"
#include "ccu_kernel_proxy.h"
#include "ccu_sim_world.h"
#include "ccu_all_to_all_v_semantics_checker.h"

#include "sim_communicator.h"
#include "sim_world.h"
#include "topo_model.h"
#include "hccl_alloc_ctx_res.h"
#include "alg_param.h"

#include <algorithm>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>

extern "C" {
void ResetCcuKernelRegisterCount();
uint32_t GetCcuKernelRegisterCount();
}

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

Mc2CcTilingInner BuildAllToAllVTiling(HcclDataType dataType)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALLV);
    tiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED);
    tiling.srcDataType = static_cast<uint8_t>(dataType);
    tiling.dstDataType = static_cast<uint8_t>(dataType);
    strcpy(tiling.groupName, "test_group");
    strcpy(tiling.algConfig, "default");
    return tiling;
}

TopoMeta BuildTopoMeta(uint32_t rankSize)
{
    TopoMeta topoMeta;
    SuperPodMeta superPod;
    ServerMeta server;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        server.push_back(rank);
    }
    superPod.push_back(server);
    topoMeta.push_back(superPod);
    return topoMeta;
}

void CleanupHostEnv(HcclComm &comm)
{
    if (comm != nullptr) {
        delete static_cast<HcclSim::SimCommunicator *>(comm);
        comm = nullptr;
    }
    HcclSim::SimWorld::Global()->Deinit();
    unsetenv("HCCL_OP_EXPANSION_MODE");
}

} // namespace

void CcuStAllToAllV::VerifyScenario(const CcuStScenario &scenario)
{
    const uint32_t rankSize = scenario.rankSize;

    ResetCcuKernelRegisterCount();
    setenv("HCCL_OP_EXPANSION_MODE", "CCU_MS", 1);

    const TopoMeta topoMeta = BuildTopoMeta(rankSize);
    HcclSim::SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
    ASSERT_EQ(aclrtSetDevice(0), ACL_SUCCESS);

    HcclComm comm = nullptr;
    ASSERT_EQ(HcclSim::Sim_HcclCommInitClusterInfo(topoMeta, 0, &comm), HCCL_SUCCESS);

    OpResCtx resCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm, "selector_st_ctx", rankSize, 0, resCtx), HCCL_SUCCESS);

    Mc2CcTilingInner tiling = BuildAllToAllVTiling(scenario.dataType);

    int dummyStream = 1;
    OpParam opParam{};
    ASSERT_EQ(InitOpParamByTiling(comm, &dummyStream, "alltoallv_st", &tiling, opParam), HCCL_SUCCESS);

    std::vector<uint64_t> sendCounts(rankSize, 1024U);
    opParam.all2AllVDataDes.sendCounts = reinterpret_cast<void *>(sendCounts.data());

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    ASSERT_EQ(SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo), HCCL_SUCCESS);
    ASSERT_EQ(algName, "CcuAlltoAllVMesh1D");
    ASSERT_GT(sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str()), 0);

    EXPECT_EQ(GetCcuKernelRegisterCount(), 0U);

    opParam.isKfc = true;
    bool skipGetRes = false;
    ASSERT_EQ(HandleSingleRankAndCommMode(comm, opParam, skipGetRes), HCCL_SUCCESS);
    ASSERT_FALSE(skipGetRes);

    void *resCtxOut = nullptr;
    ASSERT_EQ(GetCcuOpParamResCtx(comm, algName, opParam, topoInfo.get(), resCtx, &resCtxOut), HCCL_SUCCESS);
    ASSERT_NE(resCtxOut, nullptr);

    {
        auto *raw = static_cast<char *>(resCtxOut);
        std::vector<char> sequence(raw, raw + opParam.ctxSize);
        mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
        resourceCtx.DeSerialize(sequence);
        EXPECT_EQ(resourceCtx.kfcServerArgs.size(), 6U);
        EXPECT_EQ(resourceCtx.kfcServerArgs[0], resCtx.xnAddr);
        EXPECT_EQ(resourceCtx.kfcServerArgs[1], resCtx.ckeAddr);
    }

    CleanupHostEnv(comm);
    ResetCcuKernelRegisterCount();

    const std::vector<KernelHandle> handles = RegisterAllRanks(rankSize);
    ASSERT_EQ(handles.size(), rankSize);

    ScenarioData data = BuildScenarioData(scenario, handles);

    CcuAllToAllVSemanticsChecker checker;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        const Program *program = RegisterManager::Global().GetProgram(handles[rank]);
        ASSERT_NE(program, nullptr);
        const AllToAllVKernelConfig kernelConfig = BuildKernelConfig(rank, rankSize);
        CheckResult registration = checker.CheckRegistration(*program, kernelConfig);
        ASSERT_TRUE(registration.Ok()) << registration.message;
        CheckResult taskArgs = checker.CheckTaskArgs(*program, data.launches[rank].taskArgs,
            data.configs[rank]);
        ASSERT_TRUE(taskArgs.Ok()) << taskArgs.message;
    }

    if (scenario.swapTaskArgs) {
        std::swap(data.launches[0].taskArgs[3], data.launches[0].taskArgs[4]);
        EXPECT_FALSE(checker.CheckTaskArgs(
            *RegisterManager::Global().GetProgram(handles[0]), data.launches[0].taskArgs,
            data.configs[0]).Ok());
        return;
    }

    AllToAllVRuntimeExpectation expectation;
    CheckResult capture = checker.CaptureRuntimeExpectation(data.memories, data.configs, expectation);
    ASSERT_TRUE(capture.Ok()) << capture.message;

    if (scenario.missingTaskArg) {
        data.launches[1].taskArgs.pop_back();
        RuntimeWorld world;
        EXPECT_EQ(world.Launch(data.launches), Result::CONTRACT_ERROR);
        return;
    }

    if (scenario.outOfRangeParam) {
        data.configs[0].parameterAddress += data.memories[0].parameter.size() * sizeof(uint64_t);
        data.launches[0].taskArgs = PrepareAllToAllVTaskArgs(data.configs[0]);
        RuntimeWorld world;
        EXPECT_EQ(world.Launch(data.launches), Result::OUT_OF_RANGE);
        return;
    }

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(data.launches), Result::SUCCESS) << world.LastError();
    CheckResult runtime = checker.CheckRuntime(expectation, data.memories, world);
    ASSERT_TRUE(runtime.Ok()) << runtime.message;

    if (scenario.corruptOutput) {
        data.memories[0].output[data.configs[0].destinationBaseOffset] ^= 1U;
        EXPECT_FALSE(checker.CheckRuntime(expectation, data.memories, world).Ok());
    }
}

} // namespace CcuSt
} // namespace HcclSim
