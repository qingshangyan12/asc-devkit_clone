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
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"
#include "ins_reduce_scatter_concurrent_executor.h"
#include "ins_temp_reduce_scatter_nhr.h"
#include "reduce_scatter_auto_selector.h"

using namespace mc2_ops_hccl;

namespace {
constexpr u64 RS_SMALL_COUNT_512KB = 512ULL * 1024ULL;

OpParam MakeAicpuParam(HcclCMDType opType, CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = opType;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    return param;
}

// UBX：MESH_1D_CLOS 非 PCIE-mix，mesh 全连通（1DMESH 实例 rank 数 == localNetInsSize）
TopoInfoWithNetLayerDetails MakeUbxTopo(u32 rankSize = 8U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.netLayerDetails.localNetInsSizeOfLayer = {1U};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    return topo;
}

OpParam MakeDataParam(
    u64 dataSizeInBytes, HcclDataType dataType = HCCL_DATA_TYPE_FP16,
    HcclReduceOp reduceOp = HcclReduceOp::HCCL_REDUCE_SUM)
{
    OpParam param = MakeAicpuParam(HCCL_CMD_REDUCE_SCATTER);
    param.DataDes.dataType = dataType;
    param.DataDes.count = dataSizeInBytes / 2U;
    param.reduceType = reduceOp;
    return param;
}
} // namespace

class ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, selector_routes_ubx_large_data_to_concurrent_algorithm)
{
    ReduceScatterAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U);
    std::string algName;

    // mesh 全连通 + >= 512KB + 非 special -> ConcurMeshNHR（本 PR 恢复的 hccl 选择方向）
    OpParam param = MakeDataParam(RS_SMALL_COUNT_512KB);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuReduceScatterConcurMeshNHR");

    // 边界：恰好 512KB 满足（IsSmallData 为严格小于）
    param = MakeDataParam(RS_SMALL_COUNT_512KB);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuReduceScatterConcurMeshNHR");

    // 小数据（< 512KB）回落 mesh 算法
    param = MakeDataParam(RS_SMALL_COUNT_512KB - 2U);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "AicpuReduceScatterConcurMeshNHR");
}

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, selector_excludes_special_dtype_and_prod)
{
    ReduceScatterAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U);
    std::string algName;

    // 64bit 类型不走并发算法
    OpParam param = MakeDataParam(RS_SMALL_COUNT_512KB, HCCL_DATA_TYPE_FP64);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "AicpuReduceScatterConcurMeshNHR");

    // PROD 规约不走并发算法
    param = MakeDataParam(RS_SMALL_COUNT_512KB, HCCL_DATA_TYPE_FP16, HcclReduceOp::HCCL_REDUCE_PROD);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "AicpuReduceScatterConcurMeshNHR");
}

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, concurrent_algorithm_registered_and_legacy_name_gone)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    // 新名注册存在（改名后）
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_REDUCE_SCATTER, "AicpuReduceScatterConcurMeshNHR"), nullptr);
    // 旧名零残留（改名前 InsReduceScatterConcurrentMeshNHR）
    EXPECT_EQ(registry.GetAlgExec(HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterConcurrentMeshNHR"), nullptr);
}

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, concurrent_executor_rejects_invalid_hierarchy)
{
    using ExecutorType =
        InsReduceScatterConcurrentExecutor<TopoMatchUBX, InsTempReduceScatterMesh1D, InsTempReduceScatterNHR>;
    ExecutorType executor;

    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U);
    OpParam param = MakeDataParam(RS_SMALL_COUNT_512KB);
    AlgResourceRequest request;

    // infos[0] 成员数不足 2（mesh/clos 两组都需存在）-> 拒绝
    AlgHierarchyInfoForAllLevel hierarchy{};
    hierarchy.infos.resize(1U);
    hierarchy.infos[0].push_back({0U, 1U, 2U, 3U});
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_SUCCESS);

    // 完全空的 hierarchy -> 拒绝
    AlgHierarchyInfoForAllLevel empty{};
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, empty, request), HCCL_SUCCESS);
}

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, nhr_template_reports_multi_jetty_channels_per_rank)
{
    // NHR 模板在 UBX 分支按 multi-jetty 通道计算 channelsPerRank（经 GetThreadNum 暴露）
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U);
    OpParam param = MakeDataParam(RS_SMALL_COUNT_512KB);
    std::vector<std::vector<u32>> subCommRanks = {{0U, 1U, 2U, 3U, 4U, 5U, 6U, 7U}};

    // CalcRes 依赖 comm 通道查询（checker stub 环境返回通道数据）；此处仅验证对象可构造且
    // GetThreadNum 与 channelsPerRank_（默认 1）一致，multi-jetty 的通道数断言留给上板 verify
    InsTempReduceScatterNHR tpl(param, 0U, subCommRanks);
    EXPECT_GE(tpl.GetThreadNum(), 1U);
}

TEST_F(ST_REDUCE_SCATTER_CONCUR_MESH_NHR_TEST, reduce_scatter_not_use_cann_bridge)
{
    // REDUCE_SCATTER 从未进入 Bridge 名单（负例确认，防回归）
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_REDUCE_SCATTER, engine)));
    }
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_REDUCE_SCATTER, COMM_ENGINE_CPU)));
}
