/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <algorithm>
#include <limits>
#include <string>
#include <vector>

#include "alg_env_config.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "execute_selector.h"
#include "executor_v2_base.h"
#include "gtest/gtest.h"
#include "ins_temp_all_to_all_v_mesh_1D.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {
constexpr u64 ALLTOALL_MULTI_CHANNEL_LIMIT = 150ULL * 1024ULL * 1024ULL;

OpParam MakeAicpuParam(HcclCMDType opType, CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = opType;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    return param;
}

TopoInfoWithNetLayerDetails MakeUbxTopo(u32 rankSize = 8U, bool level0PcieMix = false)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 2U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = level0PcieMix;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {rankSize};
    return topo;
}

} // namespace

class ST_ALL_TO_ALL_MESH1D_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, route_uses_expected_bridge)
{
    EXPECT_TRUE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALL)));
    EXPECT_TRUE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALLV)));
    EXPECT_TRUE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALL, COMM_ENGINE_AICPU)));
    EXPECT_TRUE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALLV, COMM_ENGINE_AICPU)));

    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALL, COMM_ENGINE_CPU)));
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALLV, COMM_ENGINE_CPU)));
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLGATHER)));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, registry_contains_expected_algorithms)
{
    EXPECT_NE(GetAlgExecViaCann(HCCL_CMD_ALLTOALLV, "InsAlltoAllVMesh1D"), nullptr);
    const auto& registry = CollAlgExecRegistryV2::Instance();

    EXPECT_TRUE(registry.IsRegistered(HCCL_CMD_ALLTOALLV, "InsAlltoAllVMesh1D"));
    EXPECT_TRUE(registry.IsRegistered(HCCL_CMD_ALLTOALL, "InsAlltoAllMesh1DSingleChannel"));
    EXPECT_TRUE(registry.IsRegistered(HCCL_CMD_ALLTOALL, "InsAlltoAllMesh1D"));
    EXPECT_TRUE(registry.IsRegistered(HCCL_CMD_ALLTOALL, "AicpuAllToAllSoleMeshUBX"));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, ubx_selector_matches_mesh1d_clos)
{
    ExecuteSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo();
    u64 sendCounts[8] = {1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U};
    OpParam param = MakeAicpuParam(HCCL_CMD_ALLTOALL);
    param.all2AllVDataDes.sendCounts = sendCounts;
    std::string algName;

    EXPECT_EQ(selector.Run(param, &topo, algName), HCCL_SUCCESS);
    EXPECT_EQ(algName, "AicpuAllToAllSoleMeshUBX");
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, ubx_selector_falls_back_when_concurrent_condition_matches)
{
    ExecuteSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U);
    u64 sendCounts[4] = {513U, 513U, 513U, 513U};
    OpParam param = MakeAicpuParam(HCCL_CMD_ALLTOALL);
    param.all2AllVDataDes.sendCounts = sendCounts;
    std::string algName;

    EXPECT_EQ(selector.Run(param, &topo, algName), HCCL_SUCCESS);
    EXPECT_EQ(algName, "AicpuAllToAllSoleMeshUBX");
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, ubx_selector_preserves_non_ubx_topology_path)
{
    ExecuteSelector selector;
    u64 sendCounts[8] = {1U, 1U, 1U, 1U, 1U, 1U, 1U, 1U};
    OpParam param = MakeAicpuParam(HCCL_CMD_ALLTOALL);
    param.all2AllVDataDes.sendCounts = sendCounts;
    std::string algName;

    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U, true);
    EXPECT_EQ(selector.Run(param, &topo, algName), HCCL_SUCCESS);
    EXPECT_EQ(algName, "InsAlltoAllMesh1D");

    topo = MakeUbxTopo();
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {4U};
    EXPECT_EQ(selector.Run(param, &topo, algName), HCCL_E_NOT_SUPPORT);

    topo = MakeUbxTopo();
    topo.level0Topo = Level0Shape::MESH_1D;
    EXPECT_EQ(selector.Run(param, &topo, algName), HCCL_SUCCESS);
    EXPECT_EQ(algName, "InsAlltoAllMesh1D");
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, alltoall_multi_channel_threshold_is_strict_and_overflow_safe)
{
    constexpr u64 rankSize = 3U;
    constexpr u64 dataTypeSize = 1U;
    constexpr u64 countAtLimit = ALLTOALL_MULTI_CHANNEL_LIMIT / rankSize;
    EXPECT_FALSE(ShouldUseMultiChannelForAlltoAll(countAtLimit - 1U, dataTypeSize, rankSize));
    EXPECT_FALSE(ShouldUseMultiChannelForAlltoAll(countAtLimit, dataTypeSize, rankSize));
    EXPECT_TRUE(ShouldUseMultiChannelForAlltoAll(countAtLimit + 1U, dataTypeSize, rankSize));
    EXPECT_TRUE(ShouldUseMultiChannelForAlltoAll(std::numeric_limits<u64>::max(), 16U, 18U));
}

TEST_F(ST_ALL_TO_ALL_MESH1D_AICPU_TEST, peer_group_rounds_cover_18_ranks)
{
    const std::vector<u32> firstRound = CalcAlltoallMesh1DCommRanks(0U, 18U, 16U, 0U, 17U);
    const std::vector<u32> secondRound = CalcAlltoallMesh1DCommRanks(0U, 18U, 16U, 1U, 1U);
    EXPECT_EQ(firstRound.size(), 16U);
    EXPECT_EQ(std::find(firstRound.begin(), firstRound.end(), 0U), firstRound.end());
    std::vector<u32> sortedFirstRound = firstRound;
    std::sort(sortedFirstRound.begin(), sortedFirstRound.end());
    EXPECT_EQ(std::unique(sortedFirstRound.begin(), sortedFirstRound.end()), sortedFirstRound.end());
    EXPECT_EQ(
        sortedFirstRound, std::vector<u32>({1U, 2U, 3U, 4U, 5U, 6U, 7U, 8U, 10U, 11U, 12U, 13U, 14U, 15U, 16U, 17U}));
    EXPECT_EQ(secondRound, std::vector<u32>({9U}));
}
} // namespace checker
