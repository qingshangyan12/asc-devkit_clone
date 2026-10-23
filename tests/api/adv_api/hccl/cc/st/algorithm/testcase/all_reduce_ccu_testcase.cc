/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
// AllReduce CCU selector 行为用例：对齐 hccl SelectCcuScheduleAlgo 分支框架——
// 已注册算法（当前仅 CcuSchedAllReduceSoleMesh）MATCH；未注册算法设名后 NOT_MATCH，
// 由框架降级链兜底（CCU_SCHED -> CCU_FAIL -> AICPU）。
#include "alg_env_config.h"
#include "all_reduce_auto_selector.h"
#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {

constexpr u64 KB = 1024U;
constexpr u64 FOUR_KB = 4U * KB;
constexpr u64 ONE_MB = 1024U * KB;
constexpr u64 SIXTY_FOUR_MB = 64U * ONE_MB;
constexpr u64 FP32_SIZE = 4U;

OpParam MakeCcuParam(
    u64 count, HcclDataType dataType = HCCL_DATA_TYPE_FP32,
    HcclReduceOp reduceOp = HcclReduceOp::HCCL_REDUCE_SUM)
{
    OpParam param{};
    param.opType = HCCL_CMD_ALLREDUCE;
    param.engine = COMM_ENGINE_CCU;
    param.opExecuteConfig = OpExecuteConfig::CCU_SCHED;
    param.userRank = 0U;
    param.reduceType = reduceOp;
    param.DataDes.count = count;
    param.DataDes.dataType = dataType;
    return param;
}

TopoInfoWithNetLayerDetails MakeMesh1DTopo(u32 rankSize = 8U, u32 topoLevelNums = 1U)
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

// 两层 MESH_1D：localNetInsSizeOfLayer[0]=2 避开机内单实例分支（==1 时选未注册 SoleNHR）
TopoInfoWithNetLayerDetails MakeMultiLevelMesh1DTopo(u32 rankSize = 16U)
{
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(rankSize, 2U);
    topo.netLayerDetails.netLayerNum = 2U;
    topo.netLayerDetails.netLayers = {0U, 1U};
    topo.netLayerDetails.localNetInsSizeOfLayer = {2U, 1U};
    return topo;
}

// 单层 UBX（MESH_1D_CLOS 非 pcieMix）：mesh/clos rank 数决定 Equal/Multiple/其他形态
TopoInfoWithNetLayerDetails MakeUbxTopo(u32 rankSize, u32 meshRankNum, u32 closRankNum)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.level0MeshType = Level0MeshType::SINGLE_DIE;
    topo.topoLevelNums = 1U;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {meshRankNum};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {closRankNum};
    return topo;
}

// 框架行为无关的断言：CCU 分支不得命中。
// 当前框架 CCU NOT_MATCH 后降级 AICPU（整链 MATCH + Aicpu 名）；框架计划取消该兜底改为直接报错
// （整链 NOT_MATCH，且 algName 可能残留"设名后拒绝"的 CCU 名）。两版共同不变量：
// 只有 MATCH 且结果为 CCU 算法名才表示 CCU 分支命中——此情况绝不允许发生。
::testing::AssertionResult CcuNotMatched(SelectorStatus status, const std::string& algName)
{
    if (status == SelectorStatus::MATCH && algName.rfind("CcuSched", 0) == 0) {
        return ::testing::AssertionFailure() << "CCU algorithm unexpectedly matched: " << algName;
    }
    return ::testing::AssertionSuccess();
}

} // namespace

class ST_ALL_REDUCE_CCU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }
};

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_single_level_mesh1d_small_data_selects_sole_mesh)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE); // FP32 4KB
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllReduceSoleMesh");
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_mesh1d_small_data_selects_sole_mesh)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // 多级 ≤64P 且 ≤512KB：对齐 hccl 选 SoleMesh（asc 已注册）
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelMesh1DTopo(16U);
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllReduceSoleMesh");

    // 层实例信息缺失时不越界读，跳过单实例分支继续按数据量选算（重置 param，见 CcuNotMatched 注）
    topo.netLayerDetails.localNetInsSizeOfLayer.clear();
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllReduceSoleMesh");
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_ubx_4p_small_data_selects_sole_mesh)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U); // mesh 数等于 clos 数，4P
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE); // <512KB 小数据
    std::string algName;
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllReduceSoleMesh");
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_pcie_mix_full_mesh_selects_sole_mesh)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    topo.level0PcieMix = true;
    topo.netLayerDetails.localNetInsSizeOfLayer = {4U}; // 1DMESH 覆盖全部 4 卡
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    std::string algName;
    // PCIE-SW 定制机型全连通时按单层 Mesh1D 路径选算（dataSize*ratio ≤ 8MB）
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "CcuSchedAllReduceSoleMesh");
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_global_guards_never_match_ccu)
{
    AllReduceAutoSelector selector;
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    std::string algName;
    // level2UbRtp 不支持 ccu schedule（Select 会按降级链改写 opParam.opExecuteConfig，逐例重置）
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo();
    topo.level2UbRtp = true;
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // topoLevelNums >= 3 拒绝
    topo = MakeMesh1DTopo(8U, 3U);
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // ccu 模式不支持 PROD
    topo = MakeMesh1DTopo();
    param = MakeCcuParam(FOUR_KB / FP32_SIZE, HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_PROD);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // 64 位类型（INT64/UINT64/FP64）不支持
    param = MakeCcuParam(FOUR_KB / 8U, HCCL_DATA_TYPE_INT64);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // ccu 模式不支持 in-place（单层入口拦截；AICPU 兜底可承接原地）
    static u64 inplaceBuf[512] = {0};
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    param.inputPtr = inplaceBuf;
    param.outputPtr = inplaceBuf;
    param.inputSize = FOUR_KB;
    param.outputSize = FOUR_KB;
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_single_level_mesh1d_int8_and_over_limit_never_match_ccu)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // MS reduce 不支持 int8
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    OpParam param = MakeCcuParam(FOUR_KB, HCCL_DATA_TYPE_INT8);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // dataSize*ratio 超 Mem2Mem 8MB 上限：rankSize=2 时 ratio=2，8MB*2 > 8MB
    topo = MakeMesh1DTopo(2U);
    param = MakeCcuParam(ONE_MB / FP32_SIZE * 8U);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_single_level_two_die_never_match_ccu)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // TWO_DIE_REGULAR：SoleMesh2Die/SequenceMesh2Die 未注册，设名后拒绝
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    topo.level0MeshType = Level0MeshType::TWO_DIE_REGULAR;
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // TWO_DIE_NOT_REGULAR：直接不匹配
    topo = MakeMesh1DTopo(8U);
    topo.level0MeshType = Level0MeshType::TWO_DIE_NOT_REGULAR;
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_nhr_and_2die_full_mesh_never_match_ccu)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // Level1Nhr：对齐 hccl 选 SoleNHR（asc 未注册）
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelMesh1DTopo(16U);
    topo.Level1Nhr = true;
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // 机内单实例（localNetInsSizeOfLayer[0]==1）同样选未注册 SoleNHR
    topo = MakeMultiLevelMesh1DTopo(16U);
    topo.netLayerDetails.localNetInsSizeOfLayer = {1U, 1U};
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // 2DieFullMesh 不支持
    topo = MakeMultiLevelMesh1DTopo(16U);
    topo.is2DieFullMesh = true;
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_unregistered_sequence_never_match_ccu)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // >64P 且 ≤32MB：对齐 hccl 选 SequenceMeshMesh（asc 未注册）
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelMesh1DTopo(65U);
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // ≥64P 且 ≤16MB：同样落未注册 SequenceMeshMesh
    topo = MakeMultiLevelMesh1DTopo(64U);
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));

    // (512KB, 64MB] 且 <64P：仍落未注册 SequenceMeshMesh
    topo = MakeMultiLevelMesh1DTopo(16U);
    param = MakeCcuParam(ONE_MB / FP32_SIZE);
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_int8_never_match_ccu)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelMesh1DTopo(16U);
    OpParam param = MakeCcuParam(FOUR_KB, HCCL_DATA_TYPE_INT8);
    std::string algName;
    // 小数据 INT8 跳过 SoleMesh/Sequence（均要求非 INT8），在 Parallel 前置的 MS reduce 守卫被拒
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_over_64m_never_match_ccu)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelMesh1DTopo(16U);
    OpParam param = MakeCcuParam(SIXTY_FOUR_MB / FP32_SIZE + 1U); // 64MB+4B，超 CCU 并行上限
    std::string algName;
    // >64M 以上切为 AICPU：CCU 分支必须拒绝（断言与框架兜底行为无关）
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_pcie_mix_not_fully_connected_never_match_ccu)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 2U, 4U); // 1DMESH 仅覆盖 2 卡，无法全连通
    topo.level0PcieMix = true;
    topo.netLayerDetails.localNetInsSizeOfLayer = {4U};
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    std::string algName;
    // PCIE-SW 定制机型 Mesh 无法链接全卡时需跨 pcie 链路，不支持 ccu 模式
    EXPECT_TRUE(CcuNotMatched(selector.Select(param, &topo, algName), algName));
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_multilevel_mesh1d_clos_not_match)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    topo.topoLevelNums = 2U;
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    std::string algName;
    // 多级 MESH_1D_CLOS：CCU 分支不承接；AICPU 分支同样不支持该形态 → 整链 NOT_MATCH
    // （断言在框架取消兜底前后一致）
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_ubx_4p_large_data_not_match)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    OpParam param = MakeCcuParam(ONE_MB / FP32_SIZE); // ≥512KB 走并发
    std::string algName;
    // 4P mesh==clos 大数据量：对齐 hccl 选 ConcurMeshNHRMultiLink（asc 未注册）；
    // AICPU 分支不支持非 pcieMix 的 MESH_1D_CLOS → 整链 NOT_MATCH（断言在框架取消兜底前后一致）
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_ubx_rectangular_shapes_not_match)
{
    AllReduceAutoSelector selector;
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U, 4U, 8U); // clos 数为 mesh 数 2 倍的矩形
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE); // 小数据
    std::string algName;
    // 非 4P-Equal 矩形小数据：SoleNHRMultiLink 未注册；AICPU 不支持该形态 → 整链 NOT_MATCH
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    // 矩形大数据：ParallelMeshNHRMultiJetty 未注册 → 整链 NOT_MATCH
    param = MakeCcuParam(ONE_MB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    // 矩形 ≥64MB：PipeLineMeshNHR 未注册 → 整链 NOT_MATCH
    param = MakeCcuParam(SIXTY_FOUR_MB / FP32_SIZE + 1U);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);
}

TEST_F(ST_ALL_REDUCE_CCU_TEST, ccu_allreduce_clos_not_match)
{
    AllReduceAutoSelector selector;
    std::string algName;
    // 单层 CLOS 小数据：SoleNHR 未注册；AICPU 不支持 CLOS → 整链 NOT_MATCH
    TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(4U);
    topo.level0Topo = Level0Shape::CLOS;
    OpParam param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    // 多层 CLOS 小数据：同样落未注册 SoleNHR → 整链 NOT_MATCH
    topo = MakeMesh1DTopo(4U, 2U);
    topo.level0Topo = Level0Shape::CLOS;
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);

    // 单层 CLOS pcieMix：不支持 ccu 模式 → 整链 NOT_MATCH
    topo = MakeMesh1DTopo(4U);
    topo.level0Topo = Level0Shape::CLOS;
    topo.level0PcieMix = true;
    param = MakeCcuParam(FOUR_KB / FP32_SIZE);
    EXPECT_EQ(selector.Select(param, &topo, algName), SelectorStatus::NOT_MATCH);
}
} // namespace checker
