/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software: you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <string>

#include "alg_env_config.h"
#include "all_reduce_auto_selector.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace {
constexpr u64 AR_AICPU_1D_SMALL_DATA_SIZE = 8ULL * 1024ULL * 1024ULL;
constexpr u64 AR_AICPU_1D_MAX_DATA_SIZE = 32ULL * 1024ULL * 1024ULL;
constexpr u64 AR_AICPU_1D_64DATATYPE_DATA_SIZE = 8ULL * 1024ULL * 1024ULL;
constexpr u64 AR_AICPU_SEQUENCE_DATA_SIZE = 4ULL * 1024ULL * 1024ULL * 1024ULL;
constexpr u64 AR_2P_DETOUR_DATA_SIZE = 8ULL * 1024ULL * 1024ULL;

OpParam MakeAicpuParam(HcclCMDType opType, CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = opType;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    return param;
}

TopoInfoWithNetLayerDetails MakeMeshTopo(u32 rankSize = 8U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    return topo;
}

OpParam MakeDataParam(u64 countInBytes, HcclDataType dataType = HCCL_DATA_TYPE_FP16)
{
    OpParam param = MakeAicpuParam(HCCL_CMD_ALLREDUCE);
    param.DataDes.dataType = dataType;
    param.DataDes.count = countInBytes / 2U;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    return param;
}
} // namespace

class ST_ALL_REDUCE_MESH1D_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_selector_matches_hccl_mesh1d_boundaries)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMeshTopo(8U);
    std::string algName;

    // 小数据（<=8MB） -> OneShot
    OpParam param = MakeDataParam(AR_AICPU_1D_SMALL_DATA_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");

    // 8MB~32MB*ratio 中档 -> TwoShot
    param = MakeDataParam(16ULL * 1024ULL * 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");

    // 大数据（>32MB*ratio） -> ChunkTwoShot
    param = MakeDataParam(AR_AICPU_1D_MAX_DATA_SIZE + 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshChunkTwoShot");
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_selector_routes_special_dtype_and_prod)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMeshTopo(8U);
    std::string algName;

    // 64bit 小数据（<=8MB） -> OneShot
    OpParam param = MakeDataParam(AR_AICPU_1D_64DATATYPE_DATA_SIZE, HCCL_DATA_TYPE_FP64);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");

    // 64bit 大数据（>8MB） -> TwoShot
    param = MakeDataParam(AR_AICPU_1D_64DATATYPE_DATA_SIZE + 1024ULL, HCCL_DATA_TYPE_FP64);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");

    // PROD 与普通数据同规则（走数据量分支）
    param = MakeDataParam(AR_AICPU_1D_SMALL_DATA_SIZE);
    param.reduceType = HcclReduceOp::HCCL_REDUCE_PROD;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_selector_chunk_two_shot_exclusions)
{
    AllReduceAutoSelector selector;
    std::string algName;

    // 两层网络 + 超大数据（>4GB） -> 排除 ChunkTwoShot，中档回落 TwoShot
    TopoInfoWithNetLayerDetails topo = MakeMeshTopo(8U);
    topo.topoLevelNums = 2U;
    OpParam param = MakeDataParam(AR_AICPU_SEQUENCE_DATA_SIZE + 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");

    // 两层网络 + 2P + >=8MB -> 排除 ChunkTwoShot（hccl 走 SoleMeshConcur，未迁移，回落 TwoShot）
    TopoInfoWithNetLayerDetails topo2p = MakeMeshTopo(2U);
    topo2p.topoLevelNums = 2U;
    param = MakeDataParam(AR_2P_DETOUR_DATA_SIZE);
    EXPECT_EQ(selector.Select(param, &topo2p, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_selector_unsupported_topo_not_match)
{
    AllReduceAutoSelector selector;
    std::string algName;
    OpParam param = MakeDataParam(AR_AICPU_1D_SMALL_DATA_SIZE);

    // CLOS 拓扑不支持（hccl 走 NHR 系列，未迁移）
    TopoInfoWithNetLayerDetails closTopo = MakeMeshTopo(8U);
    closTopo.level0Topo = Level0Shape::CLOS;
    EXPECT_EQ(selector.Select(param, &closTopo, algName), SelectorStatus::NOT_MATCH);

    // 非 PCIE-mix 的 MESH_1D_CLOS（UBX）不支持
    TopoInfoWithNetLayerDetails ubxTopo = MakeMeshTopo(8U);
    ubxTopo.level0Topo = Level0Shape::MESH_1D_CLOS;
    ubxTopo.level0PcieMix = false;
    EXPECT_EQ(selector.Select(param, &ubxTopo, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_selector_pcie_mix_matches_local_algorithms)
{
    AllReduceAutoSelector selector;
    std::string algName;

    // PCIE-mix + mesh 全连通：与 MESH_1D 同样的数据量分支
    TopoInfoWithNetLayerDetails topo = MakeMeshTopo(8U);
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.level0PcieMix = true;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.netLayerDetails.localNetInsSizeOfLayer = {1U};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {8U};

    // 小数据 -> OneShot
    OpParam param = MakeDataParam(AR_AICPU_1D_SMALL_DATA_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");

    // 大数据 -> ChunkTwoShot
    param = MakeDataParam(AR_AICPU_1D_MAX_DATA_SIZE + 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshChunkTwoShot");

    // PCIE-mix 但 mesh 不全连通 -> NOT_MATCH
    TopoInfoWithNetLayerDetails notConnected = topo;
    notConnected.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {4U};
    param = MakeDataParam(AR_AICPU_1D_SMALL_DATA_SIZE);
    EXPECT_EQ(selector.Select(param, &notConnected, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_registry_contains_local_algorithms)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceSoleMeshOneShot"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceSoleMeshTwoShot"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceSoleMeshChunkTwoShot"), nullptr);
}

TEST_F(ST_ALL_REDUCE_MESH1D_AICPU_TEST, allreduce_not_use_cann_bridge_after_migration)
{
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, engine)));
    }
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, COMM_ENGINE_CPU)));

    // 未迁移的 opType 仍走 Bridge
    EXPECT_TRUE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLTOALLV, COMM_ENGINE_AICPU_TS)));
}
