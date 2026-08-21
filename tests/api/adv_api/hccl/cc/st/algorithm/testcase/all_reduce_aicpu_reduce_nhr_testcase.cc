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
#include "alg_v2_template_base.h"
#include "cann_host_bridge.h"
#include "coll_alg_v2_exec_registry.h"
#include "gtest/gtest.h"
#include "ins_temp_all_reduce_aicpu_reduce_nhr.h"

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

TopoInfoWithNetLayerDetails MakeTopo(Level0Shape level0)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = 8U;
    topo.serverNum = 1U;
    topo.level0Topo = level0;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    return topo;
}

OpParam MakeSpecialParam(
    HcclDataType dataType = HCCL_DATA_TYPE_FP64, HcclReduceOp reduceOp = HcclReduceOp::HCCL_REDUCE_SUM)
{
    OpParam param = MakeAicpuParam(HCCL_CMD_ALLREDUCE);
    param.DataDes.dataType = dataType;
    param.DataDes.count = 1024U;
    param.reduceType = reduceOp;
    return param;
}
} // namespace

class ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST, selector_routes_special_dtype_on_non_mesh_topologies)
{
    AllReduceAutoSelector selector;
    std::string algName;

    // 64bit 类型在非 MESH_1D 拓扑 -> AicpuReduceNHR（hccl 多层/CLOS/UBX 场景同款路由）
    for (Level0Shape level0 : {Level0Shape::CLOS, Level0Shape::MESH_1D_CLOS}) {
        TopoInfoWithNetLayerDetails topo = MakeTopo(level0);
        OpParam param = MakeSpecialParam(HCCL_DATA_TYPE_FP64);
        EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
        EXPECT_EQ(algName, "AicpuAllReduceSoleNHRAicpuReduce");
    }

    // PROD 规约同样路由
    TopoInfoWithNetLayerDetails closTopo = MakeTopo(Level0Shape::CLOS);
    OpParam param = MakeSpecialParam(HCCL_DATA_TYPE_FP16, HcclReduceOp::HCCL_REDUCE_PROD);
    EXPECT_EQ(selector.Select(param, &closTopo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHRAicpuReduce");
}

TEST_F(ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST, selector_keeps_mesh1d_special_on_mesh_algorithms)
{
    AllReduceAutoSelector selector;
    std::string algName;

    // hccl 语义：MESH_1D + special 走 mesh 算法（OneShot/TwoShot），不走 AicpuReduce
    TopoInfoWithNetLayerDetails topo = MakeTopo(Level0Shape::MESH_1D);
    OpParam param = MakeSpecialParam(HCCL_DATA_TYPE_FP64);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_NE(algName, "AicpuAllReduceSoleNHRAicpuReduce");

    // 普通类型 + CLOS 拓扑：非 special -> 保持 OneShot 占位（NHR TwoShotMultiLink 未迁移）
    param = MakeSpecialParam(HCCL_DATA_TYPE_FP16);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST, aicpu_reduce_registered_in_local_registry)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HCCL_CMD_ALLREDUCE, "AicpuAllReduceSoleNHRAicpuReduce"), nullptr);
}

TEST_F(ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST, aicpu_reduce_thread_num_dispatches_polymorphically)
{
    // GetThreadNum 必须是 const override 的虚覆写：经基类指针调用应返回 1
    // （回归 PR 修复：原实现漏写 const 导致函数隐藏，多态调用进不到本类）
    OpParam param = MakeSpecialParam(HCCL_DATA_TYPE_FP64);
    TopoInfoWithNetLayerDetails topo = MakeTopo(Level0Shape::CLOS);
    std::vector<std::vector<u32>> subCommRanks = {{0U, 1U, 2U, 3U}};
    InsTempAllReduceAicpuReduceNHR tpl(param, 0U, subCommRanks);

    InsAlgTemplateBase* base = &tpl;
    EXPECT_EQ(base->GetThreadNum(), 1U);
    EXPECT_EQ(tpl.GetThreadNum(), 1U);
}

TEST_F(ST_ALL_REDUCE_AICPU_REDUCE_NHR_TEST, allreduce_not_use_cann_bridge_after_migration)
{
    for (CommEngine engine : {COMM_ENGINE_AICPU_TS, COMM_ENGINE_AICPU}) {
        EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, engine)));
    }
    EXPECT_FALSE(UseCannBridge(MakeAicpuParam(HCCL_CMD_ALLREDUCE, COMM_ENGINE_CPU)));
}
