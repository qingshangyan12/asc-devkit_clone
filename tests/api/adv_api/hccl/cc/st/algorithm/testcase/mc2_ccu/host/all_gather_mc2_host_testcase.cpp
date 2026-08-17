/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <gtest/gtest.h>
#include <cstdlib>
#include <cstring>
#include <vector>

#include "all_gather_auto_selector.h"
#include "base.h"
#include "hccl_alloc_ctx_res.h"
#include "hccl_mc2.h"
#include "kfc_server_config.h"
#include "kfc_server_protocol.h"
#include "sim_communicator.h"
#include "sim_world.h"
#include "topo_model.h"

namespace {

using mc2_ops_hccl::AllGatherAutoSelector;
using mc2_ops_hccl::OpParam;
using mc2_ops_hccl::SelectorStatus;
using mc2_ops_hccl::TopoInfoWithNetLayerDetails;

extern "C" void ResetCcuKernelRegisterCount();
extern "C" uint32_t GetCcuKernelRegisterCount();
extern "C" void ResetCcuKernelLaunchRecords();
extern "C" uint32_t GetCcuKernelLaunchCount();
extern "C" uint64_t GetCcuKernelLaunchThread(uint32_t launchIndex);
extern "C" uint64_t GetCcuKernelLaunchKernel(uint32_t launchIndex);
extern "C" uint32_t GetCcuKernelLaunchArgSize(uint32_t launchIndex);
extern "C" uint64_t GetCcuKernelLaunchArg(uint32_t launchIndex, uint32_t argIndex);

constexpr uint32_t TEST_RANK_ID = 0;
constexpr char ALL_GATHER_ALG_NAME[] = "CcuSchedAllGatherSoleMesh";
constexpr char CONCURRENT_ALL_GATHER_ALG_NAME[] = "CcuSchedAllGatherConcurMeshNHRMultiLink";

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

TopoInfoWithNetLayerDetails BuildSingleDieMeshTopo(uint32_t rankSize)
{
    TopoInfoWithNetLayerDetails topoInfo{};
    topoInfo.userRank = TEST_RANK_ID;
    topoInfo.userRankSize = rankSize;
    topoInfo.serverNum = 1;
    topoInfo.topoLevelNums = 1;
    topoInfo.level0Topo = Level0Shape::MESH_1D;
    topoInfo.level0MeshType = Level0MeshType::SINGLE_DIE;
    return topoInfo;
}

OpParam BuildSelectorParam(OpExecuteConfig config)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    param.opExecuteConfig = config;
    param.engine = COMM_ENGINE_CCU;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    param.DataDes.count = 16;
    return param;
}

TopoInfoWithNetLayerDetails BuildSingleLevelUbxTopo(uint32_t rankSize, uint32_t meshRankNum, uint32_t closRankNum)
{
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(rankSize);
    topoInfo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topoInfo.topoInstDetailsOfLayer.resize(1);
    topoInfo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {meshRankNum};
    topoInfo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {closRankNum};
    return topoInfo;
}

struct Mc2TilingTestData {
    uint32_t version;
    uint32_t cnt;
    uint32_t offset[MAX_CC_TILING_NUM];
    Mc2ServerCfg serverCfg;
    Mc2CcTilingInner ccTiling[MAX_CC_TILING_NUM];
};

Mc2TilingTestData BuildAllGatherTiling(const char* algName = ALL_GATHER_ALG_NAME)
{
    Mc2TilingTestData result{};
    result.version = INIT_TILING_CCU_NEW_VERSION;
    result.cnt = 1;
    const uint32_t baseOffset = offsetof(Mc2TilingTestData, ccTiling);
    for (uint32_t i = 0; i < MAX_CC_TILING_NUM; ++i) {
        result.offset[i] = baseOffset + i * sizeof(Mc2CcTilingInner);
    }
    Mc2CcTilingInner& tiling = result.ccTiling[0];
    tiling.opType = static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER);
    tiling.commEngine = static_cast<uint8_t>(OpExecuteConfig::CCU_SCHED);
    tiling.srcDataType = HCCL_DATA_TYPE_FP16;
    tiling.dstDataType = HCCL_DATA_TYPE_FP16;
    strcpy(tiling.groupName, "all_gather_group");
    strcpy(tiling.algConfig, algName);
    return result;
}

Mc2TilingTestData BuildMixedAllGatherTiling()
{
    Mc2TilingTestData result = BuildAllGatherTiling(CONCURRENT_ALL_GATHER_ALG_NAME);
    result.cnt = 2;
    result.ccTiling[1] = result.ccTiling[0];
    strcpy(result.ccTiling[1].algConfig, ALL_GATHER_ALG_NAME);
    return result;
}

class AllGatherProductionSelectorTest : public testing::Test {};

TEST_F(AllGatherProductionSelectorTest, CcuScheduleSelectsCanonicalAlgorithm)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(8);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    std::string algName;

    EXPECT_EQ(selector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, ALL_GATHER_ALG_NAME);
    EXPECT_EQ(param.opExecuteConfig, OpExecuteConfig::CCU_SCHED);
}

TEST_F(AllGatherProductionSelectorTest, CcuMsFallsBackToScheduleAlgorithm)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(8);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_MS);
    std::string algName;

    EXPECT_EQ(selector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, ALL_GATHER_ALG_NAME);
    EXPECT_EQ(param.opExecuteConfig, OpExecuteConfig::CCU_SCHED);
}

TEST_F(AllGatherProductionSelectorTest, TwoDieDoesNotSelectSoleMeshScheduleAlgorithm)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(8);
    topoInfo.level0MeshType = Level0MeshType::TWO_DIE_REGULAR;
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    std::string algName;

    (void)selector.Select(param, &topoInfo, algName);
    EXPECT_NE(algName, ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, MultiLevelDoesNotSelectSoleMeshScheduleAlgorithm)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(8);
    topoInfo.topoLevelNums = 2;
    topoInfo.netLayerDetails.localNetInsSizeOfLayer = {8, 1};
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    std::string algName;

    (void)selector.Select(param, &topoInfo, algName);
    EXPECT_NE(algName, ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, OverlapDoesNotSelectSoleMeshScheduleAlgorithm)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleDieMeshTopo(8);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    std::vector<uint8_t> buffer(64);
    param.inputPtr = buffer.data();
    param.outputPtr = buffer.data() + 16;
    param.inputSize = 32;
    param.outputSize = 32;
    std::string algName;

    (void)selector.Select(param, &topoInfo, algName);
    EXPECT_NE(algName, ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, LargeEqualUbxSelectsConcurrentMeshNhr)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleLevelUbxTopo(4U, 4U, 4U);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    param.DataDes.count = SMALL_SIZE_512KB / sizeof(uint16_t) + 1U;
    std::string algName;

    EXPECT_EQ(selector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, CONCURRENT_ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, SmallUbxKeepsSoleMesh)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleLevelUbxTopo(4U, 4U, 4U);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    param.DataDes.count = SMALL_SIZE_512KB / sizeof(uint16_t);
    std::string algName;

    EXPECT_EQ(selector.Select(param, &topoInfo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, UnequalUbxDoesNotSelectConcurrentMeshNhr)
{
    AllGatherAutoSelector selector;
    TopoInfoWithNetLayerDetails topoInfo = BuildSingleLevelUbxTopo(4U, 2U, 4U);
    OpParam param = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    param.DataDes.count = SMALL_SIZE_512KB / sizeof(uint16_t) + 1U;
    std::string algName;

    (void)selector.Select(param, &topoInfo, algName);
    EXPECT_NE(algName, CONCURRENT_ALL_GATHER_ALG_NAME);
}

TEST_F(AllGatherProductionSelectorTest, KfcCacheSeparatesSoleMeshAndConcurrentMeshNhr)
{
    OpParam soleMesh = BuildSelectorParam(OpExecuteConfig::CCU_SCHED);
    OpParam concurrent = soleMesh;
    strcpy(soleMesh.algName, ALL_GATHER_ALG_NAME);
    strcpy(concurrent.algName, CONCURRENT_ALL_GATHER_ALG_NAME);

    const uint64_t soleMeshHash = mc2_ops_hccl::BuildKfcServerCacheHash("same_tag", {soleMesh});
    const uint64_t concurrentHash = mc2_ops_hccl::BuildKfcServerCacheHash("same_tag", {concurrent});
    EXPECT_NE(soleMeshHash, concurrentHash);
    EXPECT_EQ(mc2_ops_hccl::GetKfcServerMissionNum(soleMesh.algName), 1U);
    EXPECT_EQ(mc2_ops_hccl::GetKfcServerMissionNum(concurrent.algName), 2U);
    EXPECT_EQ(mc2_ops_hccl::GetKfcServerRole(concurrent.algName, 0U), mc2_ops_hccl::KfcServerRole::ALL_GATHER_MESH);
    EXPECT_EQ(mc2_ops_hccl::GetKfcServerRole(concurrent.algName, 1U), mc2_ops_hccl::KfcServerRole::ALL_GATHER_NHR);
}

TEST_F(AllGatherProductionSelectorTest, ConcurrentMissionSyncUsesFourFixedSignalRegions)
{
    EXPECT_EQ(mc2_ops_hccl::KFC_SERVER_PROTOCOL_VERSION, 3U);
    EXPECT_EQ(mc2_ops_hccl::KFC_SERVER_SIGNAL_REGION_NUM, 4U);
    EXPECT_EQ(HcclKfcProtocol::KFC_SIGNAL_HOST_START, 0U);
    EXPECT_EQ(HcclKfcProtocol::KFC_SIGNAL_HOST_DONE, 1U);
    EXPECT_EQ(HcclKfcProtocol::KFC_SIGNAL_MISSION_START, 2U);
    EXPECT_EQ(HcclKfcProtocol::KFC_SIGNAL_MISSION_DONE, 3U);
}

class AllGatherMc2HostTest : public testing::TestWithParam<uint32_t> {
protected:
    void SetUp() override
    {
        ResetCcuKernelRegisterCount();
        ResetCcuKernelLaunchRecords();
        setenv("HCCL_OP_EXPANSION_MODE", "CCU_SCHED", 1);
        topoMeta_ = BuildTopoMeta(GetParam());
        HcclSim::SimWorld::Global()->Init(topoMeta_, DevType::DEV_TYPE_950);
        ASSERT_EQ(aclrtSetDevice(TEST_RANK_ID), ACL_SUCCESS);
        ASSERT_EQ(HcclSim::Sim_HcclCommInitClusterInfo(topoMeta_, TEST_RANK_ID, &comm_), HCCL_SUCCESS);
        stream_ = &dummyStream_;
    }

    void TearDown() override
    {
        if (comm_ != nullptr) {
            delete static_cast<HcclSim::SimCommunicator*>(comm_);
            comm_ = nullptr;
        }
        HcclSim::SimWorld::Global()->Deinit();
        unsetenv("HCCL_OP_EXPANSION_MODE");
        ResetCcuKernelRegisterCount();
        ResetCcuKernelLaunchRecords();
    }

    TopoMeta topoMeta_;
    HcclComm comm_ = nullptr;
    aclrtStream stream_ = nullptr;
    int dummyStream_ = 1;
};

TEST_P(AllGatherMc2HostTest, Mc2TilingBuildsSingleKfcServerContext)
{
    Mc2TilingTestData tiling = BuildAllGatherTiling();
    void* rawCtx = nullptr;
    ASSERT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &rawCtx), HCCL_SUCCESS);
    ASSERT_NE(rawCtx, nullptr);

    auto* resCtx = static_cast<OpResCtx*>(rawCtx);
    EXPECT_EQ(resCtx->rankId, TEST_RANK_ID);
    EXPECT_EQ(resCtx->rankSize, GetParam());
    EXPECT_EQ(resCtx->opType[0], static_cast<uint32_t>(HcclCMDType::HCCL_CMD_ALLGATHER));
    EXPECT_EQ(resCtx->algorithmType[0], static_cast<uint32_t>(CcuSchedAllGatherSoleMesh));
    EXPECT_TRUE(resCtx->isKfc[0]);
    EXPECT_EQ(GetCcuKernelRegisterCount(), 1U);

    OpParam opParam{};
    ASSERT_NE(resCtx->algInfo[0].opParam, 0U);
    ASSERT_EQ(aclrtMemcpy(
                  &opParam, sizeof(opParam), reinterpret_cast<void*>(resCtx->algInfo[0].opParam), sizeof(opParam),
                  ACL_MEMCPY_DEVICE_TO_HOST),
        ACL_SUCCESS);
    EXPECT_STREQ(opParam.algName, ALL_GATHER_ALG_NAME);
    EXPECT_TRUE(opParam.isKfc);
    EXPECT_EQ(opParam.engine, COMM_ENGINE_CCU);

    ASSERT_NE(opParam.resCtx, nullptr);
    ASSERT_NE(opParam.ctxSize, 0U);
    auto* serializedCtx = static_cast<char*>(opParam.resCtx);
    std::vector<char> sequence(serializedCtx, serializedCtx + opParam.ctxSize);
    mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
    resourceCtx.DeSerialize(sequence);
    ASSERT_EQ(resourceCtx.kfcServerArgs.size(), 6U);
    EXPECT_EQ(resourceCtx.kfcServerArgSize, 6U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[0], resCtx->xnAddr);
    EXPECT_EQ(resourceCtx.kfcServerArgs[1], resCtx->ckeAddr);
    EXPECT_EQ(resourceCtx.kfcServerArgs[2], 1U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[3], 1U);
    EXPECT_EQ(resourceCtx.kfcServerArgs[4], 0U);
    EXPECT_NE(resourceCtx.kfcServerArgs[5], 0U);
}

TEST_P(AllGatherMc2HostTest, ConcurrentMc2TilingBuildsAndLaunchesTwoKfcMissions)
{
    Mc2TilingTestData tiling = BuildAllGatherTiling(CONCURRENT_ALL_GATHER_ALG_NAME);
    void* rawCtx = nullptr;
    ASSERT_EQ(HcclAllocComResourceByTiling(comm_, stream_, &tiling, &rawCtx), HCCL_SUCCESS);
    ASSERT_NE(rawCtx, nullptr);

    auto* resCtx = static_cast<OpResCtx*>(rawCtx);
    EXPECT_EQ(resCtx->algorithmType[0], static_cast<uint32_t>(CcuSchedAllGatherConcurMeshNHRMultiLink));
    EXPECT_TRUE(resCtx->isKfc[0]);
    EXPECT_EQ(GetCcuKernelRegisterCount(), 2U);

    OpParam opParam{};
    ASSERT_EQ(aclrtMemcpy(
                  &opParam, sizeof(opParam), reinterpret_cast<void*>(resCtx->algInfo[0].opParam), sizeof(opParam),
                  ACL_MEMCPY_DEVICE_TO_HOST),
        ACL_SUCCESS);
    EXPECT_STREQ(opParam.algName, CONCURRENT_ALL_GATHER_ALG_NAME);

    auto* serializedCtx = static_cast<char*>(opParam.resCtx);
    std::vector<char> sequence(serializedCtx, serializedCtx + opParam.ctxSize);
    mc2_ops_hccl::AlgResourceCtxSerializable resourceCtx;
    resourceCtx.DeSerialize(sequence);
    ASSERT_EQ(resourceCtx.threads.size(), 2U);
    ASSERT_EQ(resourceCtx.ccuKernels.size(), 2U);
    ASSERT_EQ(resourceCtx.ccuKernelNum.size(), 2U);
    EXPECT_EQ(resourceCtx.ccuKernelNum[0], 1U);
    EXPECT_EQ(resourceCtx.ccuKernelNum[1], 1U);
    ASSERT_EQ(resourceCtx.kfcServerArgs.size(), 12U);
    EXPECT_EQ(resourceCtx.kfcServerArgSize, mc2_ops_hccl::KFC_SERVER_ARG_NUM);
    for (uint32_t mission = 0; mission < 2U; ++mission) {
        const uint32_t offset = mission * mc2_ops_hccl::KFC_SERVER_ARG_NUM;
        EXPECT_EQ(resourceCtx.kfcServerArgs[offset], resCtx->xnAddr);
        EXPECT_EQ(resourceCtx.kfcServerArgs[offset + 1U], resCtx->ckeAddr);
        EXPECT_EQ(resourceCtx.kfcServerArgs[offset + 2U], 1U);
        EXPECT_EQ(resourceCtx.kfcServerArgs[offset + 3U], 2U);
        EXPECT_EQ(resourceCtx.kfcServerArgs[offset + 4U], mission);
        EXPECT_NE(resourceCtx.kfcServerArgs[offset + 5U], 0U);
    }
    EXPECT_EQ(resourceCtx.kfcServerArgs[5], resourceCtx.kfcServerArgs[11]);

    ASSERT_EQ(CcuKernelLaunch(comm_, rawCtx), CCU_SUCCESS);
    ASSERT_EQ(GetCcuKernelLaunchCount(), 2U);
    for (uint32_t mission = 0; mission < 2U; ++mission) {
        EXPECT_EQ(GetCcuKernelLaunchThread(mission), resourceCtx.threads[mission]);
        EXPECT_EQ(GetCcuKernelLaunchKernel(mission), resourceCtx.ccuKernels[mission]);
        EXPECT_EQ(GetCcuKernelLaunchArgSize(mission), mc2_ops_hccl::KFC_SERVER_ARG_NUM);
        for (uint32_t arg = 0; arg < mc2_ops_hccl::KFC_SERVER_ARG_NUM; ++arg) {
            EXPECT_EQ(
                GetCcuKernelLaunchArg(mission, arg),
                resourceCtx.kfcServerArgs[mission * mc2_ops_hccl::KFC_SERVER_ARG_NUM + arg]);
        }
    }
}

TEST_P(AllGatherMc2HostTest, ConcurrentAllGatherRejectsMixedMc2Context)
{
    Mc2TilingTestData tiling = BuildMixedAllGatherTiling();
    OpResCtx resCtx{};
    ASSERT_EQ(AllocCcuOpResCtx(comm_, "concurrent_mixed_ctx", GetParam(), TEST_RANK_ID, resCtx), HCCL_SUCCESS);
    const void* tilingList[] = {&tiling.ccTiling[0], &tiling.ccTiling[1]};
    std::string topoTags[] = {"concurrent_all_gather", "sole_mesh_all_gather"};

    EXPECT_EQ(CcuSelectAlg(comm_, stream_, topoTags, tilingList, 2U, &tiling, resCtx), HCCL_E_NOT_SUPPORT);
}

INSTANTIATE_TEST_SUITE_P(RankBoundaries, AllGatherMc2HostTest, testing::Values(2U, 4U, 8U, 16U, 17U));

} // namespace
