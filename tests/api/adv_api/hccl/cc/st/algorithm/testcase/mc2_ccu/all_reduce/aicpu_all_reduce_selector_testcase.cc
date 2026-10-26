/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

// AllReduce AICPU 选路用例：直接调用 AllReduceAutoSelector::SelectAicpuAlgo，
// 覆盖 SelectMeshAlgoAicpu / SelectMeshAlgoAicpuUBX 的迁移分支（多层 / 单层 Mesh1D / CLOS /
// PCIeMix / UBX）。经基类引用调用以规避派生类 private 访问限制，不依赖 SimWorld 设备仿真。

#include "all_reduce_auto_selector.h"

#include <string>

#include "gtest/gtest.h"

using namespace mc2_ops_hccl;

namespace {

constexpr u64 KB = 1024ULL;
constexpr u64 ONE_MB = 1024ULL * KB;

OpParam MakeArParam(
    u64 count, HcclDataType dataType = HCCL_DATA_TYPE_FP32, HcclReduceOp reduceType = HcclReduceOp::HCCL_REDUCE_SUM)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    param.reduceType = reduceType;
    param.DataDes.count = count;
    param.DataDes.dataType = dataType;
    return param;
}

// 用字节数换算 count，保证 dataSize 精确落在被测阈值上。
OpParam MakeArParamByBytes(
    u64 bytes, HcclDataType dataType = HCCL_DATA_TYPE_FP32, HcclReduceOp reduceType = HcclReduceOp::HCCL_REDUCE_SUM)
{
    return MakeArParam(bytes / DATATYPE_SIZE_TABLE[dataType], dataType, reduceType);
}

TopoInfoWithNetLayerDetails MakeSingleLevelMesh1DTopo(u32 rankSize)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.netLayerDetails.netLayerNum = 1U;
    return topo;
}

TopoInfoWithNetLayerDetails MakeSingleLevelClosTopo(u32 rankSize)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1U;
    topo.level0Topo = Level0Shape::CLOS;
    topo.netLayerDetails.netLayerNum = 1U;
    return topo;
}

// 多层拓扑：localNetInsSize0 需 !=1 才能进入 level0Topo 相关分支。
TopoInfoWithNetLayerDetails MakeMultiLevelTopo(Level0Shape level0, u32 topoLevelNums, u32 localNetInsSize0)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = 8U;
    topo.topoLevelNums = topoLevelNums;
    topo.level0Topo = level0;
    topo.netLayerDetails.netLayerNum = 2U;
    topo.netLayerDetails.netLayers = {0U, 1U};
    topo.netLayerDetails.localNetInsSizeOfLayer = {localNetInsSize0, 1U};
    return topo;
}

// PCIeMix 单层：localNetInsSize0 == meshRankNum 时 IsLayerAllConnetedWithTopo 判全连通。
TopoInfoWithNetLayerDetails MakePcieMeshTopo(u32 rankSize, u32 meshRankNum, u32 localNetInsSize0, bool pcieMix)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.level0PcieMix = pcieMix;
    topo.netLayerDetails.netLayerNum = 1U;
    topo.netLayerDetails.localNetInsSizeOfLayer = {localNetInsSize0};
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {meshRankNum};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {localNetInsSize0};
    return topo;
}

TopoInfoWithNetLayerDetails MakeUbxTopo(u32 rankSize, u32 meshRankNum, u32 closRankNum)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.topoLevelNums = 1U;
    topo.level0Topo = Level0Shape::MESH_1D_CLOS;
    topo.level0PcieMix = false;
    topo.netLayerDetails.netLayerNum = 1U;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {meshRankNum};
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_CLOS] = {closRankNum};
    return topo;
}

} // namespace

class AicpuAllReduceSelectorTest : public ::testing::Test {
protected:
    SelectorStatus SelectAicpu(const TopoInfoWithNetLayerDetails* topo, const OpParam& param, std::string& algName)
    {
        const AutoSelectorBase& base = selector_;
        return base.SelectAicpuAlgo(topo, param, {}, algName);
    }

    AllReduceAutoSelector selector_;
};

// ===== 多层拓扑（topoLevelNums > 1）=====

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Prod_MatchSoleNHRAicpuReduce)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 2U, 8U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_PROD);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHRAicpuReduce");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Symmetric3LevelUboe_MatchParallelMeshNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 3U, 8U);
    topo.level0Symmetric = true;
    topo.level1Symmetric = true;
    topo.deviceNumPerModule = 4U;
    topo.topLevelUboe = true;
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceParallelMeshNHR");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Level1Nhr_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 2U, 8U);
    topo.Level1Nhr = true;
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_LocalSingleInstance_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 2U, 1U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Mesh1D_3Level_NonSymmetric_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 3U, 8U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Mesh1D_2Level_MidData_MatchParallelMeshNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 2U, 8U);
    OpParam param = MakeArParamByBytes(64U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceParallelMeshNHR");
}

TEST_F(AicpuAllReduceSelectorTest, MultiLevel_Mesh1D_2Level_SmallData_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeMultiLevelTopo(Level0Shape::MESH_1D, 2U, 8U);
    OpParam param = MakeArParamByBytes(1U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}

// ===== 单层 MESH_1D =====

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_MidData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(16U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_OverMaxData_MatchChunkTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(512U * ONE_MB); // 512MB * 8/64 = 64MB > 32MB
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshChunkTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_SpecialInt64_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_SpecialInt64_MidData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(16U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, Mesh1D_Prod_MidData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelMesh1DTopo(8U);
    OpParam param = MakeArParamByBytes(16U * ONE_MB, HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_PROD);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

// ===== 单层 CLOS =====

TEST_F(AicpuAllReduceSelectorTest, Clos_Special_MatchSoleNHRAicpuReduce)
{
    TopoInfoWithNetLayerDetails topo = MakeSingleLevelClosTopo(8U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHRAicpuReduce");
}

// ===== 单层 MESH_1D_CLOS + PCIeMix 全连通 =====

TEST_F(AicpuAllReduceSelectorTest, PcieMixFull_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 8U, 8U, true);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, PcieMixFull_MidData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 8U, 8U, true);
    OpParam param = MakeArParamByBytes(16U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, PcieMixFull_OverMaxData_MatchChunkTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 8U, 8U, true);
    OpParam param = MakeArParamByBytes(512U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshChunkTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, PcieMixFull_Special_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 8U, 8U, true);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, PcieMixFull_Special_MidData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 8U, 8U, true);
    OpParam param = MakeArParamByBytes(16U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

// ===== 单层 MESH_1D_CLOS + PCIeMix 非全连通 =====

TEST_F(AicpuAllReduceSelectorTest, PcieMixNotFull_SmallData_MatchParallelMeshNHRPcie)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 2U, 8U, true);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllreduceParallelMeshNHRPcie");
}

TEST_F(AicpuAllReduceSelectorTest, PcieMixNotFull_LargeData_MatchPipeLinePcie)
{
    TopoInfoWithNetLayerDetails topo = MakePcieMeshTopo(8U, 2U, 8U, true);
    OpParam param = MakeArParamByBytes(64U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReducePipeLinePcie");
}

// ===== 单层 MESH_1D_CLOS（UBX，非 PCIeMix）=====

TEST_F(AicpuAllReduceSelectorTest, Ubx_MeshEqualClos4P_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, Ubx_MeshEqualClos4P_Special_SmallData_MatchOneShot)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshOneShot");
}

TEST_F(AicpuAllReduceSelectorTest, Ubx_MeshEqualClos4P_Special_LargeData_MatchTwoShot)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(4U, 4U, 4U);
    OpParam param = MakeArParamByBytes(16U * ONE_MB, HCCL_DATA_TYPE_INT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleMeshTwoShot");
}

TEST_F(AicpuAllReduceSelectorTest, Ubx_MeshEqualClosOver4P_SmallData_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U, 4U, 4U);
    OpParam param = MakeArParamByBytes(512U * KB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}

TEST_F(AicpuAllReduceSelectorTest, Ubx_Rectangular_Special_MatchSoleNHRAicpuReduce)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U, 2U, 4U);
    OpParam param = MakeArParamByBytes(4U * ONE_MB, HCCL_DATA_TYPE_UINT64);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHRAicpuReduce");
}

TEST_F(AicpuAllReduceSelectorTest, Ubx_RectangularNotMultiple_SmallData_MatchSoleNHR)
{
    TopoInfoWithNetLayerDetails topo = MakeUbxTopo(8U, 4U, 6U);
    OpParam param = MakeArParamByBytes(512U * KB);
    std::string algName;
    EXPECT_EQ(SelectAicpu(&topo, param, algName), SelectorStatus::MATCH);
    EXPECT_EQ(algName, "AicpuAllReduceSoleNHR");
}
