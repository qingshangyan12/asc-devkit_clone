/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "alg_env_config.h"
#include "alltoall_auto_selector.h"
#include "alltoallv_auto_selector.h"
#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {

OpParam MakeCcuParam(HcclCMDType opType)
{
    OpParam param{};
    param.opType = opType;
    param.engine = COMM_ENGINE_CCU;
    param.opExecuteConfig = OpExecuteConfig::CCU_SCHED;
    param.userRank = 0U;
    param.all2AllVDataDes.sendType = HCCL_DATA_TYPE_FP32; // 零值0是INT8，会被多级CCU分支拒绝
    param.all2AllVDataDes.recvType = HCCL_DATA_TYPE_FP32;
    return param;
}

TopoInfoWithNetLayerDetails MakeMesh1DTopo(u32 rankSize = 4U, u32 topoLevelNums = 1U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.level0MeshType = Level0MeshType::SINGLE_DIE;
    topo.topoLevelNums = topoLevelNums;
    return topo;
}

TopoInfoWithNetLayerDetails MakeConcurrentTopo(u32 rankSize = 4U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {rankSize};
    return topo;
}

} // namespace

class ST_ALL_TO_ALL_CCU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_mesh1d_clos_nullptr_sendcounts_selects_smallest_path)
{
    AlltoAllAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo();
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALL);
    param.all2AllVDataDes.sendCounts = nullptr;
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllToAllMesh1DMultiJetty"); // dataSize=0 → 非并发
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_multilevel_mesh1d_falls_back_to_aicpu_sole_mesh)
{
    AlltoAllAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U, 2U); // 多级 MESH_1D ≤64
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALL);
    std::string algName;
    // CCU MultiLink 未注册 → NOT_MATCH → AICPU 兜底 MESH_1D → SoleMesh（路径守护用例）
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_single_level_mesh1d_selects_sole_mesh)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U); // 单级 MESH_1D，alltoallv kernel 多event无rank上限
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_multilevel_small_rank_selects_sole_mesh)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(4U, 2U); // 多级 ≤4P
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_pcie_mix_all_connected_selects_sole_mesh)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo();
    topo.level0PcieMix = true;
    topo.netLayerDetails.localNetInsSizeOfLayer = {4U}; // 全连通 1DMESH
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_mesh1d_clos_unregistered_algos_stay_not_match)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo();
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {3U}; // mesh != clos
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;
    // CCU: MultiJetty 未注册 NOT_MATCH；AICPU: mesh!=clos 也 NOT_MATCH → 整链 NOT_MATCH
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    // mesh == clos 且 4P：CCU Concurrent 未注册 NOT_MATCH → AICPU Concurrent 兜底 MATCH
    topo = MakeConcurrentTopo();
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMeshConcurrent");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_int8_multilevel_falls_back_to_aicpu)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(4U, 2U);
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    param.all2AllVDataDes.sendType = HCCL_DATA_TYPE_INT8;
    std::string algName;
    // CCU: int8 NOT_MATCH（对齐 hccl）→ AICPU 兜底 SoleMesh（守护用例）
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_inplace_falls_back_to_aicpu)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    static u64 inplaceBuf[64] = {0};
    param.inputPtr = inplaceBuf; // sendbuf == recvbuf：原地 AllToAllV
    param.outputPtr = inplaceBuf;
    param.inputSize = sizeof(inplaceBuf);
    param.outputSize = sizeof(inplaceBuf);
    std::string algName;
    // ccu_schedule 不支持原地算子（与 alltoall/all_gather 等兄弟实现契约一致）：
    // 应 NOT_MATCH → 框架置 CCU_FAIL → AICPU 兜底 SoleMesh，绝不能 MATCH 出 CCU 算法
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_multilevel_2die_falls_back_to_aicpu)
{
    AlltoAllVAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(4U, 2U); // 多级 MESH_1D ≤4P
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;
    // 2Die 形态未经 SoleMesh 验证（单级路径同样拒绝），对齐单级契约：NOT_MATCH → AICPU 兜底
    topo.level0MeshType = Level0MeshType::TWO_DIE_REGULAR;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMesh");
    topo.level0MeshType = Level0MeshType::TWO_DIE_NOT_REGULAR;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMesh");
}

TEST_F(ST_ALL_TO_ALL_CCU_TEST, ccu_alltoallv_multilevel_pcie_mix_connectivity_contract)
{
    AlltoAllVAutoSelector selector;
    OpParam param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    std::string algName;

    // 多级 pcie 混插未全连通：与单级契约一致拒绝（SoleMesh 走不了 pcie 通道）→ AICPU 兜底
    TopoInfoWithNetLayerDetails topo = MakeConcurrentTopo(4U);
    topo.topoLevelNums = 2U;
    topo.level0PcieMix = true; // 未设 localNetInsSizeOfLayer → 非全连通
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllToAllVSoleMesh");

    // 多级 pcie 混插全连通：与单级一致，仍可 MATCH CCU SoleMesh
    // （Select 会按降级链改写 opParam.opExecuteConfig，二次调用前需重置）
    topo.netLayerDetails.localNetInsSizeOfLayer = {4U};
    param = MakeCcuParam(HCCL_CMD_ALLTOALLV);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllToAllVSoleMesh");
}
} // namespace checker
