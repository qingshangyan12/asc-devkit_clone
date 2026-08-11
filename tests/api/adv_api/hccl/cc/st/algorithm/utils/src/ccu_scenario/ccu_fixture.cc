/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_fixture.h"

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

Mc2CcTilingInner CcuStFixture::BuildTiling(HcclCMDType opType, HcclDataType dataType,
                                             const std::string &algConfig)
{
    Mc2CcTilingInner tiling{};
    tiling.opType = static_cast<uint32_t>(opType);
    tiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED);
    tiling.srcDataType = static_cast<uint8_t>(dataType);
    tiling.dstDataType = static_cast<uint8_t>(dataType);
    strcpy(tiling.groupName, "test_group");
    strcpy(tiling.algConfig, algConfig.empty() ? "default" : algConfig.c_str());
    return tiling;
}

TopoMeta CcuStFixture::BuildTopoMeta(uint32_t rankSize, StTopoType topoType)
{
    switch (topoType) {
    case StTopoType::TWO_DIE_REGULAR: {
        ServerMeta server = {0, 1, 2, 3, 8, 9, 10, 11};
        return {{{server}}};
    }
    case StTopoType::MESH_1D_CLOS: {
        uint32_t half = rankSize / 2;
        ServerMeta server0;
        ServerMeta server1;
        for (uint32_t i = 0; i < half; ++i) server0.push_back(i);
        for (uint32_t i = half; i < rankSize; ++i) server1.push_back(i);
        return {{{server0, server1}}};
    }
    case StTopoType::SINGLE_DIE_MESH1D:
    default: {
        ServerMeta server;
        for (uint32_t rank = 0; rank < rankSize; ++rank) {
            server.push_back(rank);
        }
        return {{{server}}};
    }
    }
}

void CcuStFixture::CleanupHostEnv(HcclComm &comm)
{
    if (comm != nullptr) {
        delete static_cast<HcclSim::SimCommunicator *>(comm);
        comm = nullptr;
    }
    HcclSim::SimWorld::Global()->Deinit();
    unsetenv("HCCL_OP_EXPANSION_MODE");
    unsetenv("ASCEND_ENABLE_CCU_KFC_BRANCH");
}

KernelHandle CcuStFixture::RegisterKernelForRank(uint32_t rank, uint32_t rankSize,
                                                   const CcuStScenario &scenario,
                                                   const TopoMeta &topoMeta,
                                                   const Mc2CcTilingInner &tiling)
{
    setenv("HCCL_OP_EXPANSION_MODE", "CCU_SCHED", 1);
    setenv("ASCEND_ENABLE_CCU_KFC_BRANCH", "1", 1);
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
    if (InitOpParamByTiling(comm, stream, "ccu_st", &tiling, opParam) != HCCL_SUCCESS) {
        CleanupHostEnv(comm);
        return 0;
    }

    std::vector<uint64_t> sendCounts(rankSize, 1024U);
    opParam.all2AllVDataDes.sendCounts = reinterpret_cast<void *>(sendCounts.data());

    opParam.opExecuteConfig = mc2_ops_hccl::OpExecuteConfig::CCU_SCHED;

    std::string algName;
    auto topoInfo = std::make_unique<TopoInfoWithNetLayerDetails>();

    bool forcedAlgAccepted = false;
    if (TryForcedAlgAndPrepareEngine(comm, &tiling, opParam, algName, topoInfo, forcedAlgAccepted) != HCCL_SUCCESS) {
        CleanupHostEnv(comm);
        return 0;
    }
    if (!forcedAlgAccepted) {
        if (SelectAlgAndPrepareEngine(comm, opParam, algName, topoInfo) != HCCL_SUCCESS || algName.empty()) {
            CleanupHostEnv(comm);
            return 0;
        }
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

    const auto &lastGroup = RegisterManager::Global().GetLastCompiledGroup();
    KernelHandle handle = lastGroup.empty() ? 0 : lastGroup.back();

    CleanupHostEnv(comm);
    return handle;
}

void CcuStFixture::VerifyScenario(const CcuStScenario &scenario)
{
    currentScenario_ = scenario;
    const uint32_t rankSize = scenario.rankSize;

    RegisterManager::Global().Reset();
    ResetCcuKernelRegisterCount();

    const TopoMeta topoMeta = BuildTopoMeta(rankSize, scenario.topoType);
    const Mc2CcTilingInner tiling = BuildTiling(scenario.opType, scenario.dataType, scenario.algConfig);

    std::vector<KernelHandle> handles(rankSize);
    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        handles[rank] = RegisterKernelForRank(rank, rankSize, scenario, topoMeta, tiling);
        ASSERT_NE(handles[rank], 0U) << "failed to register kernel for rank " << rank;
    }

    ScenarioData data = BuildScenarioData(scenario, handles);

    for (uint32_t rank = 0; rank < rankSize; ++rank) {
        const Program *program = RegisterManager::Global().GetProgram(handles[rank]);
        ASSERT_NE(program, nullptr);
        CheckResult registration = DoCheckRegistration(*program, rank, rankSize);
        ASSERT_TRUE(registration.Ok()) << registration.message;
        CheckResult taskArgs = DoCheckTaskArgs(*program, rank, data);
        ASSERT_TRUE(taskArgs.Ok()) << taskArgs.message;
    }

    if (scenario.swapTaskArgs) {
        auto [i, j] = SwapTaskArgIndices();
        std::swap(data.launches[0].taskArgs[i], data.launches[0].taskArgs[j]);
        EXPECT_FALSE(DoCheckTaskArgs(
            *RegisterManager::Global().GetProgram(handles[0]), 0, data).Ok());
        return;
    }

    CheckResult capture = DoCaptureRuntimeExpectation(data);
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
    CheckResult runtime = DoCheckRuntime(data, world);
    ASSERT_TRUE(runtime.Ok()) << runtime.message;

    if (scenario.corruptOutput) {
        if (!data.memories.empty() && !data.memories[0].output.empty()) {
            data.memories[0].output[0] ^= 1U;
        }
        EXPECT_FALSE(DoCheckRuntime(data, world).Ok());
    }
}

} // namespace CcuSt
} // namespace HcclSim
