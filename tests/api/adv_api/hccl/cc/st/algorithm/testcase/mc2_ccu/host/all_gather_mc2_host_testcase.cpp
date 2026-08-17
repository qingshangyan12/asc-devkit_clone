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

constexpr uint32_t TEST_RANK_ID = 0;
constexpr char ALL_GATHER_ALG_NAME[] = "CcuSchedAllGatherSoleMesh";

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

struct Mc2TilingTestData {
    uint32_t version;
    uint32_t cnt;
    uint32_t offset[MAX_CC_TILING_NUM];
    Mc2ServerCfg serverCfg;
    Mc2CcTilingInner ccTiling[MAX_CC_TILING_NUM];
};

Mc2TilingTestData BuildAllGatherTiling()
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
    strcpy(tiling.algConfig, ALL_GATHER_ALG_NAME);
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

class AllGatherMc2HostTest : public testing::TestWithParam<uint32_t> {
protected:
    void SetUp() override
    {
        ResetCcuKernelRegisterCount();
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

INSTANTIATE_TEST_SUITE_P(RankBoundaries, AllGatherMc2HostTest, testing::Values(2U, 4U, 8U, 16U, 17U));

} // namespace
