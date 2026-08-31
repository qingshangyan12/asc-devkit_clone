/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software and you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include <cmath>
#include <cstdlib>
#include <map>
#include <string>
#include <vector>

#include "alg_env_config.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"
#include "ins_reduce_scatter_parallel_executor.h"
#include "ins_temp_all_gather_mesh_1D.h"
#include "ins_temp_reduce_scatter_mesh_1D.h"
#include "ins_temp_reduce_scatter_nhr.h"
#include "template_utils.h"

using namespace mc2_ops_hccl;

namespace {
OpParam MakeAicpuParam(HcclCMDType opType, CommEngine engine = COMM_ENGINE_AICPU_TS)
{
    OpParam param{};
    param.opType = opType;
    param.engine = engine;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    return param;
}

TopoInfoWithNetLayerDetails MakeUbxTopo(u32 rankSize = 8U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    return topo;
}

OpParam MakeRsParam(u64 dataSizeInBytes)
{
    OpParam param = MakeAicpuParam(HCCL_CMD_REDUCE_SCATTER);
    param.DataDes.dataType = HCCL_DATA_TYPE_FP16;
    param.DataDes.count = dataSizeInBytes / 2U;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    return param;
}

// 构造两级 channel map：每对 rank portGroupSize 条链路
std::map<u32, std::vector<ChannelInfo>> MakeChannelMap(u32 rankSize, u32 portGroupSize)
{
    std::map<u32, std::vector<ChannelInfo>> channels;
    for (u32 rank = 0U; rank < rankSize; ++rank) {
        for (u32 port = 0U; port < portGroupSize; ++port) {
            ChannelInfo ch{};
            ch.isValid = true;
            ch.remoteRank = rank;
            ch.portGroupSize = portGroupSize;
            channels[rank].push_back(ch);
        }
    }
    return channels;
}
} // namespace

class ST_REDUCE_SCATTER_PARALLEL_UBX_TEST : public ::testing::Test {
protected:
    void SetUp() override
    {
        ResetAlgEnvConfigInitState();
        unsetenv("HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO");
    }
    void TearDown() override { unsetenv("HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO"); }
};

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, parallel_algorithm_registered_in_local_registry)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterParallelMesh1DNHRUBX"), nullptr);
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, parallel_executor_rejects_invalid_hierarchy)
{
    using ExecutorType = InsReduceScatterParallelExecutor<
        TopoMatchUBX, InsTempReduceScatterMesh1D, InsTempReduceScatterNHR, InsTempReduceScatterMesh1D,
        InsTempReduceScatterNHR>;
    ExecutorType executor;

    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U);
    OpParam param = MakeRsParam(4ULL * 1024ULL * 1024ULL);
    AlgResourceRequest request;

    // M3 修复项：层级数不足（只有 1 层）-> 拒绝（防 infos[1] 越界）
    AlgHierarchyInfoForAllLevel hierarchy{};
    hierarchy.infos.resize(1U);
    hierarchy.infos[0].push_back({0U, 1U, 2U, 3U});
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, hierarchy, request), HCCL_SUCCESS);

    // 空 hierarchy -> 拒绝
    AlgHierarchyInfoForAllLevel empty{};
    EXPECT_NE(executor.CalcRes(nullptr, param, &topo, empty, request), HCCL_SUCCESS);
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, split_ratio_formula_symmetric_network_near_half)
{
    // 对称组网（两级同为 4 卡、同为 1 端口）：公式结果经八分位量化应为 0.5
    auto intra = MakeChannelMap(4U, 1U);
    auto inter = MakeChannelMap(4U, 1U);
    double ratio =
        CalcParallelDataSplitRatio(4U, 4U, intra, inter, ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, 0.5);
    EXPECT_DOUBLE_EQ(ratio, 0.5);
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, split_ratio_formula_falls_back_on_empty_channels)
{
    // 回退链：空 channel map -> 回退 fallbackRatio（不崩溃）
    std::map<u32, std::vector<ChannelInfo>> empty;
    double ratio =
        CalcParallelDataSplitRatio(4U, 2U, empty, empty, ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, 0.5);
    EXPECT_DOUBLE_EQ(ratio, 0.5);

    // 非法 fallback（NaN）-> 归一化为 0.5
    double nanRatio =
        CalcParallelDataSplitRatio(4U, 2U, empty, empty, ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, NAN);
    EXPECT_DOUBLE_EQ(nanRatio, 0.5);
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, split_ratio_quantized_to_eighths)
{
    // 非对称组网（框间端口多）：结果必须是 1/8 的整数倍且落在 [1/8, 7/8]
    auto intra = MakeChannelMap(4U, 1U);
    auto inter = MakeChannelMap(16U, 8U);
    double ratio =
        CalcParallelDataSplitRatio(4U, 16U, intra, inter, ParallelDataSplitType::REDUCE_SCATTER_WITH_LOCAL_REDUCE, 0.5);
    EXPECT_GE(ratio, 1.0 / 8.0);
    EXPECT_LE(ratio, 7.0 / 8.0);
    const double eighths = ratio * 8.0;
    EXPECT_DOUBLE_EQ(eighths, std::floor(eighths + 0.5));
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, env_split_ratio_parsed_into_env_config)
{
    // A7-full 环境变量链路：设置 HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO 后可读取
    setenv("HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO", "0.25", 1);
    EXPECT_EQ(ParseMultipleDimensionSplitRatio(), HCCL_SUCCESS);
    double ratio = 0.5;
    EXPECT_TRUE(GetExternalInputMultipleDimensionSplitRatio(ratio));
    EXPECT_DOUBLE_EQ(ratio, 0.25);

    // 非法值：解析失败，未设置标志
    setenv("HCCL_ALG_MULTIPLE_DIMENSION_SPLIT_RATIO", "abc", 1);
    EXPECT_NE(ParseMultipleDimensionSplitRatio(), HCCL_SUCCESS);
    double unused = 0.5;
    EXPECT_FALSE(GetExternalInputMultipleDimensionSplitRatio(unused));
}

TEST_F(ST_REDUCE_SCATTER_PARALLEL_UBX_TEST, reduce_scatter_not_use_cann_bridge)
{
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_REDUCE_SCATTER, engine)));
    }
}
