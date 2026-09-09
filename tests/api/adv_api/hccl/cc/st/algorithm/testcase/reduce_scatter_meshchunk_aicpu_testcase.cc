/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software under the CANN Open Software License Agreement Version 2.0.
 * Refer to the License for details.
 */
#include <map>
#include <string>
#include <vector>

#include "alg_env_config.h"
#include "coll_alg_v2_exec_registry.h"
#include "executor_v2_base.h"
#include "gtest/gtest.h"
#include "reduce_scatter_auto_selector.h"

using namespace mc2_ops_hccl;

namespace checker {
namespace {

constexpr u64 RS_AICPU_1D_MAX_DATA_SIZE = 16ULL * 1024ULL * 1024ULL; // 与 selector 阈值常量保持一致

TopoInfoWithNetLayerDetails MakeMesh1DTopo(u32 rankSize = 8U)
{
    TopoInfoWithNetLayerDetails topo{};
    topo.userRank = 0U;
    topo.userRankSize = rankSize;
    topo.serverNum = 1U;
    topo.level0Topo = Level0Shape::MESH_1D;
    topo.topoLevelNums = 1U;
    topo.level0PcieMix = false;
    topo.topoInstDetailsOfLayer.resize(1U);
    topo.topoInstDetailsOfLayer[0].rankNumForTopoType[COMM_TOPO_1DMESH] = {rankSize};
    return topo;
}

OpParam MakeAicpuParam(u64 recvCount, HcclDataType dataType, HcclReduceOp reduceOp)
{
    OpParam param{};
    param.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    param.engine = CommEngine::COMM_ENGINE_AICPU_TS;
    param.opExecuteConfig = OpExecuteConfig::AICPU_TS;
    param.userRank = 0U;
    param.DataDes.count = recvCount;
    param.DataDes.dataType = dataType;
    param.reduceType = reduceOp;
    return param;
}

} // namespace

class ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST : public ::testing::Test {
protected:
    void SetUp() override { ResetAlgEnvConfigInitState(); }

    static SelectorStatus SelectAicpu(
        const TopoInfoWithNetLayerDetails& topo, OpParam& param, std::string& selectAlgName)
    {
        ReduceScatterAutoSelector selector;
        return selector.Select(param, const_cast<TopoInfoWithNetLayerDetails*>(&topo), selectAlgName);
    }
};

// 改名闭环：selector 返回名与本地注册表条目一致，确保 AicpuReduceScatterSoleMeshChunk 全链路可达
TEST_F(ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST, meshchunk_selector_name_resolves_in_registry)
{
    CollAlgExecRegistryV2& registry = CollAlgExecRegistryV2::Instance();
    EXPECT_NE(registry.GetAlgExec(HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "AicpuReduceScatterSoleMeshChunk"), nullptr);
    EXPECT_NE(registry.GetAlgExec(HcclCMDType::HCCL_CMD_REDUCE_SCATTER, "InsReduceScatterMesh1D"), nullptr);
}

// 大数据量 MESH_1D 选 MeshChunk，小数据量回落普通 Mesh1D（阈值：dataSize * (8/rankSize)^2 > 16MB）
TEST_F(ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST, meshchunk_selected_for_large_mesh1d_data)
{
    const TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    const u64 largeCount = 5ULL * 1024ULL * 1024ULL; // FP32 下 20MB > 16MB 阈值

    std::string selectAlgName;
    OpParam largeParam = MakeAicpuParam(largeCount, HcclDataType::HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_SUM);
    EXPECT_EQ(SelectAicpu(topo, largeParam, selectAlgName), SelectorStatus::MATCH);
    EXPECT_EQ(selectAlgName, "AicpuReduceScatterSoleMeshChunk");

    const u64 smallCount = 1024ULL * 1024ULL; // FP32 下 4MB < 16MB
    OpParam smallParam = MakeAicpuParam(smallCount, HcclDataType::HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_SUM);
    EXPECT_EQ(SelectAicpu(topo, smallParam, selectAlgName), SelectorStatus::MATCH);
    EXPECT_EQ(selectAlgName, "InsReduceScatterMesh1D");
}

// 64bit 数据类型与 PROREduce 不走 MeshChunk，回落普通 Mesh1D（即使数据量超阈值）
TEST_F(ST_REDUCE_SCATTER_MESHCHUNK_AICPU_TEST, meshchunk_excluded_for_64bit_and_prod)
{
    const TopoInfoWithNetLayerDetails topo = MakeMesh1DTopo(8U);
    const u64 largeCount = 5ULL * 1024ULL * 1024ULL;
    std::string selectAlgName;

    OpParam int64Param = MakeAicpuParam(largeCount, HcclDataType::HCCL_DATA_TYPE_INT64, HcclReduceOp::HCCL_REDUCE_SUM);
    EXPECT_EQ(SelectAicpu(topo, int64Param, selectAlgName), SelectorStatus::MATCH);
    EXPECT_EQ(selectAlgName, "InsReduceScatterMesh1D");

    OpParam prodParam = MakeAicpuParam(largeCount, HcclDataType::HCCL_DATA_TYPE_FP32, HcclReduceOp::HCCL_REDUCE_PROD);
    EXPECT_EQ(SelectAicpu(topo, prodParam, selectAlgName), SelectorStatus::MATCH);
    EXPECT_EQ(selectAlgName, "InsReduceScatterMesh1D");
}

} // namespace checker
