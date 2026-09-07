/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "gtest/gtest.h"
#include "coll_alg_v2_exec_registry.h"
#include "all_reduce_auto_selector.h"
#include "cann_host_bridge.h"

using namespace mc2_ops_hccl;

namespace checker {

TEST(AllReduceAicpuTest, SoleNhrIsRegisteredLocally)
{
    std::unique_ptr<InsCollAlgBase> executor =
        CollAlgExecRegistryV2::Instance().GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceSoleNHR");
    EXPECT_NE(executor, nullptr);
}

TEST(AllReduceAicpuTest, ClosTopologySelectsSoleNhr)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo{};
    topo.serverNum = 1;
    topo.topoLevelNums = 1;
    topo.userRankSize = 2;
    topo.level0Topo = Level0Shape::CLOS;

    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.DataDes.count = 1024;
    param.DataDes.dataType = HcclDataType::HCCL_DATA_TYPE_FP16;
    param.reduceType = HcclReduceOp::HCCL_REDUCE_SUM;

    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
    EXPECT_FALSE(UseCannBridge(param));
}

} // namespace checker
