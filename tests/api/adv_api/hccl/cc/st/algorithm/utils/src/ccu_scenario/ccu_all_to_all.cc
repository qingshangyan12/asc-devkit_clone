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
#include "ccu_kernel_proxy.h"
#include "ccu_sim_world.h"
#include "ccu_dag_checker.h"
#include "ccu_all_to_all_semantics_checker.h"

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
    std::vector<AllToAllLaunchConfig> configs;
    std::vector<RankLaunch> launches;
};

ScenarioData BuildScenarioData(const CcuStScenario &scenario,
                                const std::vector<KernelHandle> &handles)
{
    const uint32_t rankSize = scenario.rankSize;
    const uint64_t sliceSize = scenario.sizes[0][0];
    constexpr uint64_t sourceBaseOffset = 7;
    constexpr uint64_t destinationBaseOffset = 11;
    constexpr uint64_t guard = 32;

    ScenarioData data;
    data.memories.resize(rankSize);
    data.configs.resize(rankSize);
    data.launches.resize(rankSize);

    const uint64_t inputSize = sourceBaseOffset + rankSize * sliceSize + guard;
    const uint64_t outputSize = destinationBaseOffset + rankSize * sliceSize + guard;

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        RankMemory &memory = data.memories[rank];
        memory.input.assign(inputSize, 0x5a);
        memory.output.assign(outputSize, 0xa5);
        AllToAllLaunchConfig &config = data.configs[rank];
        config.inputAddress = memory.InputAddress();
        config.outputAddress = memory.OutputAddress();
        config.token = 0x1000U + rank;
        config.sliceSize = sliceSize;
        config.srcStride = sliceSize;
        config.srcOffset = sourceBaseOffset;
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

Mc2CcTilingInner BuildAllToAllTiling(HcclDataType dataType)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLTOALL);
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

KernelHandle RegisterKernelForRank(uint32_t rank, uint32_t rankSize, const CcuStScenario &scenario,
    const TopoMeta &topoMeta, const Mc2CcTilingInner &tiling)
{
    setenv("HCCL_OP_EXPANSION_MODE", "CCU_SCHED", 1);
    HcclSim::SimWorld::Global()->Init(topoMeta, DevType::DEV_TYPE_950);
    if (aclrtSetDevice(rank) != ACL_SUCCESS) return 0;

    aclrtStream stream = nullptr;
    if (aclrtCreateStream(&stream) != ACL_SUCCESS) return 0;

    HcclComm comm = nullptr;
    if (HcclSim::Sim_HcclCommInitClusterInfo(topoMeta, rank, &comm) != HCCL_SUCCESS) return 0;

    OpResCtx resCtx{};
    if (AllocCcuOpResCtx(comm, "selector_st_ctx", rankSize, rank, resCtx) != HCCL_SUCCESS) {
        CleanupHostEnv(comm);
        return 0;
    }

    OpParam opParam{};
    if (InitOpParamByTiling(comm, stream, "alltoall_st", &tiling, opParam) != HCCL_SUCCESS) {
        CleanupHostEnv(comm);
        return 0;
    }

    std::vector<uint64_t> sendCounts(rankSize, 1024U);
    opParam.all2AllVDataDes.sendCounts = reinterpret_cast<void *>(sendCounts.data());

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();
    if (SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo) != HCCL_SUCCESS || algName.empty()) {
        CleanupHostEnv(comm);
        return 0;
    }
    if (rank == 0 && !scenario.expectedAlgName.empty() && algName != scenario.expectedAlgName) {
        CleanupHostEnv(comm);
        return 0;
    }
    sprintf_s(opParam.algName, sizeof(opParam.algName), "%s", algName.c_str());

    opParam.isMc2 = true;
    bool skipGetRes = false;
    if (HandleSingleRankAndCommMode(comm, opParam, skipGetRes) != HCCL_SUCCESS || skipGetRes) {
        CleanupHostEnv(comm);
        return 0;
    }

    void *resCtxOut = nullptr;
    if (GetCcuOpParamResCtx(comm, algName, opParam, topoInfo.get(), resCtx, &resCtxOut) != HCCL_SUCCESS) {
        CleanupHostEnv(comm);
        return 0;
    }

    if (rank == 0) {
        auto *raw = static_cast<char *>(resCtxOut);
        std::vector<char> sequence(raw, raw + opParam.ctxSize);
        mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
        resourceCtx.DeSerialize(sequence);
        EXPECT_EQ(resourceCtx.kfcServerArgs.size(), 6U);
        EXPECT_EQ(resourceCtx.kfcServerArgs[0], resCtx.xnAddr);
        EXPECT_EQ(resourceCtx.kfcServerArgs[1], resCtx.ckeAddr);
    }

    const auto &lastGroup = RegisterManager::Global().GetLastCompiledGroup();
    KernelHandle handle = lastGroup.empty() ? 0 : lastGroup.back();

    CleanupHostEnv(comm);
    return handle;
}

} // namespace

void CcuStAllToAll::VerifyScenario(const CcuStScenario &scenario)
{
    const uint32_t rankSize = scenario.rankSize;

    RegisterManager::Global().Reset();
    ResetCcuKernelRegisterCount();

    const TopoMeta topoMeta = BuildTopoMeta(rankSize);
    const Mc2CcTilingInner tiling = BuildAllToAllTiling(scenario.dataType);

    std::vector<KernelHandle> handles(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        handles[rank] = RegisterKernelForRank(rank, rankSize, scenario, topoMeta, tiling);
        ASSERT_NE(handles[rank], 0U) << "failed to register kernel for rank " << rank;
    }

    ScenarioData data = BuildScenarioData(scenario, handles);

    CcuAllToAllSemanticsChecker checker;
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        const Program *program = RegisterManager::Global().GetProgram(handles[rank]);
        ASSERT_NE(program, nullptr);
        AllToAllKernelConfig kernelConfig;
        kernelConfig.rankId = rank;
        kernelConfig.rankSize = rankSize;
        kernelConfig.loadFromMem = true;
        for (uint32_t peer = 0; peer < rankSize; ++peer) {
            if (peer != rank) {
                kernelConfig.channels.push_back(MakeChannel(rank, peer));
            }
        }
        CheckResult registration = checker.CheckRegistration(*program, kernelConfig);
        ASSERT_TRUE(registration.Ok()) << registration.message;
        CheckResult taskArgs = checker.CheckTaskArgs(*program, data.launches[rank].taskArgs,
            data.configs[rank]);
        ASSERT_TRUE(taskArgs.Ok()) << taskArgs.message;
    }

    if (scenario.swapTaskArgs) {
        std::swap(data.launches[0].taskArgs[5], data.launches[0].taskArgs[6]);
        EXPECT_FALSE(checker.CheckTaskArgs(
            *RegisterManager::Global().GetProgram(handles[0]), data.launches[0].taskArgs,
            data.configs[0]).Ok());
        return;
    }

    AllToAllRuntimeExpectation expectation;
    CheckResult capture = checker.CaptureRuntimeExpectation(data.memories, data.configs, expectation);
    ASSERT_TRUE(capture.Ok()) << capture.message;

    if (scenario.missingTaskArg) {
        data.launches[1].taskArgs.pop_back();
        RuntimeWorld world;
        EXPECT_EQ(world.Launch(data.launches), Result::CONTRACT_ERROR);
        return;
    }

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(data.launches), Result::SUCCESS) << world.LastError();
    CheckResult dagResult = CcuDagChecker().Check(world.ExecutionLogData());
    ASSERT_TRUE(dagResult.Ok()) << dagResult.message;
    CheckResult runtime = checker.CheckRuntime(expectation, data.memories, world);
    ASSERT_TRUE(runtime.Ok()) << runtime.message;

    if (scenario.corruptOutput) {
        data.memories[0].output[data.configs[0].dstOffset] ^= 1U;
        EXPECT_FALSE(checker.CheckRuntime(expectation, data.memories, world).Ok());
    }
}

} // namespace CcuSt
} // namespace HcclSim
