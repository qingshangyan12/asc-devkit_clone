/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software and you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <string>

#include "alg_env_config.h"
#include "all_reduce_auto_selector.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"
#include "ins_temp_all_gather_mesh_1D.h"
#include "ins_temp_all_gather_nhr.h"
#include "ins_temp_reduce_scatter_mesh_1D.h"
#include "ins_temp_reduce_scatter_nhr.h"
#include "ins_v2_all_reduce_parallel_executor.h"
#include "template_utils.h"
#include "topo_match_pcie_mix.h"

using namespace mc2_ops_hccl;

namespace {
constexpr u64 AR_PCIE_PARALLEL_MAX_DATA_SIZE = 32ULL * 1024ULL * 1024ULL;

OpParam MakeAicpuParam(HcclCMDType opType, CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = opType;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    return param;
}

// PCIE 定制机型：MESH_1D_CLOS + pcieMix，mesh 非全连通（rankNumForTopoType[1DMESH] != localNetInsSize）
TopoInfoWithNetLayerDetails MakePcieMixTopo(u32 rankSize = 8U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = true;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.netLayerDetails.localNetInsSizeOfLayer = {2U};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize / 2U, rankSize / 2U};
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

class ST_ALL_REDUCE_PARALLEL_PCIE_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, selector_routes_pcie_mix_to_parallel_algorithm)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakePcieMixTopo(8U);
    std::string algName;

    // PCIE-mix 且 mesh 非全连通：普通类型 < 32MB -> Parallel
    OpParam param = MakeDataParam(4ULL * 1024ULL * 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "InsAllReduceParallelMesh1DNHRPcie");

    // 边界：恰好 32MB 不满足（严格小于）
    param = MakeDataParam(AR_PCIE_PARALLEL_MAX_DATA_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");

    // special 类型（64bit/PROD）不走 Parallel（hccl 走 SequenceAicpuReducePcie，未迁移，回落 OneShot 占位）
    param = MakeDataParam(4ULL * 1024ULL * 1024ULL, HCCL_DATA_TYPE_FP64);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "InsAllReduceParallelMesh1DNHRPcie");

    param = MakeDataParam(4ULL * 1024ULL * 1024ULL);
    param.reduceType = HcclReduceOp::HCCL_REDUCE_PROD;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "InsAllReduceParallelMesh1DNHRPcie");
}

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, selector_keeps_fully_mesh_connected_on_mesh_path)
{
    AllReduceAutoSelector selector;
    std::string algName;

    // PCIE-mix 但 mesh 全连通（rankNumForTopoType[1DMESH] 有等于 localNetInsSize 的实例）-> 不走 Parallel
    TopoInfoWithNetLayerDetails topo = MakePcieMixTopo(8U);
    topo.netLayerDetails.localNetInsSizeOfLayer = {1U};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {8U};
    OpParam param = MakeDataParam(4ULL * 1024ULL * 1024ULL);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "InsAllReduceParallelMesh1DNHRPcie");

    // 非 PCIE-mix 的 MESH_1D 拓扑 -> 不走 Parallel
    TopoInfoWithNetLayerDetails meshTopo{};
    meshTopo.userRank = 0U;
    meshTopo.userRankSize = 8U;
    meshTopo.level0Topo = Level0Shape::MESH_1D;
    meshTopo.topoLevelNums = 1U;
    meshTopo.level0PcieMix = false;
    EXPECT_EQ(selector.Select(param, &meshTopo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "InsAllReduceParallelMesh1DNHRPcie");
}

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, parallel_executor_registered_in_local_registry)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLREDUCE, "InsAllReduceParallelMesh1DNHRPcie"), nullptr);
}

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, parallel_executor_rejects_invalid_hierarchy)
{
    // CalcRes 对层级数不足的 hierarchy 必须报错（防 infos[1] 越界）
    using ExecutorType = InsAllReduceParallelExecutor<
        TopoMatchPcieMix, InsTempReduceScatterMesh1D, InsTempReduceScatterNHR, InsTempAllGatherMesh1D,
        InsTempAllGatherNHR>;
    ExecutorType executor;

    TopoInfoWithNetLayerDetails topo = MakePcieMixTopo(8U);
    OpParam param = MakeDataParam(4ULL * 1024ULL * 1024ULL);

    AlgHierarchyInfoForAllLevel hierarchy{};
    hierarchy.infos.resize(1U); // 只有 1 层 -> 拒绝
    hierarchy.infos[0].push_back({0U, 1U, 2U, 3U});
    AlgResourceRequest request;
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_SUCCESS);

    AlgHierarchyInfoForAllLevel empty{};
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, empty, request), HCCL_SUCCESS);
}

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, split_ratio_formula_fallback_on_empty_channels)
{
    // 公式回退链：空 channel map -> 回退 fallbackRatio（0.5），不崩溃
    std::map<u32, std::vector<ChannelInfo>> emptyChannels;
    double ratio = CalcParallelDataSplitRatio(
        4U, 2U, emptyChannels, emptyChannels, ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, 0.5);
    EXPECT_DOUBLE_EQ(ratio, 0.5);
}

TEST_F(ST_ALL_REDUCE_PARALLEL_PCIE_TEST, allreduce_not_use_cann_bridge_after_migration)
{
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, engine)));
    }
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, COMM_ENGINE_CPU)));
}
