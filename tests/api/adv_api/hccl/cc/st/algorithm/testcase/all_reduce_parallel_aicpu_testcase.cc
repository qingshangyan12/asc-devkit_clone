/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <string>

#include "all_reduce_auto_selector.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {

OpParam MakeAicpuParam()
{
    OpParam param{};
    param.opType = HCCL_CMD_ALLREDUCE;
    param.engine = COMM_ENGINE_AICPU_TS;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP32;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;
    return param;
}

TopoInfoWithNetLayerDetails MakeTwoLevelMeshTopo()
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = 4U;
    topo.serverNum = 1U;
    topo.deviceNumPerModule = 2U;
    topo.topoLevelNums = 2U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.level0Symmetric = true;
    topo.level1Symmetric = true;
    topo.Level1Nhr = false;
    topo.topLevelUboe = false;
    topo.netLayerDetails.localNetInsSizeOfLayer = {2U, 4U};
    return topo;
}

} // namespace

TEST(AllReduceParallelAicpuTest, UsesLocalRegistryAndNotCannBridge)
{
    OpParam param = MakeAicpuParam();
    EXPECT_FALSE(UseCannBridge(param));
    EXPECT_NE(
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceParallelMeshNHR"), nullptr);
}

TEST(AllReduceParallelAicpuTest, SelectorKeepsParallelDataRange)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeTwoLevelMeshTopo();
    OpParam param = MakeAicpuParam();
    std::string algName;

    param.DataDes.count = (32ULL * 1024 * 1024) / sizeof(float) + 1;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceParallelMeshNHR");

    param.DataDes.count = (32ULL * 1024 * 1024) / sizeof(float);
    algName.clear();
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    param.DataDes.count = (4ULL * 1024 * 1024 * 1024) / sizeof(float) + 1;
    algName.clear();
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);
}

} // namespace checker
