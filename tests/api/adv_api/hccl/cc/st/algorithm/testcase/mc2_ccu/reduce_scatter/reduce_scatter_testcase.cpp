/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_reduce_scatter.h"

#include <vector>

using namespace HcclSim::CcuSt;

namespace {

CcuStScenario MakeScenario(const TopoMeta& topoMeta, HcclDataType dataType, HcclReduceOp reduceOp, uint64_t count)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(topoMeta);
    CcuStScenario scenario;
    scenario.topoMeta = topoMeta;
    scenario.dataType = dataType;
    scenario.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    scenario.reduceType = reduceOp;
    scenario.count = count * rankSize;
    scenario.sizes.assign(rankSize, std::vector<uint64_t>(rankSize, count * DATATYPE_SIZE_TABLE[dataType]));
    return scenario;
}

} // namespace

// ===== 基础功能验证 =====

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_4Rank_SingleDie_Fp16_Sum_64Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 64);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_8Rank_SingleDie_Fp16_Sum_32Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3, 4, 5, 6, 7}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 32);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

// ===== Reduce 操作类型泛化 =====

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Max_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_MAX, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Min_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_MIN, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Prod_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_PROD, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

// ===== 数据类型泛化 =====

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Int8_Sum_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_INT8, HCCL_REDUCE_SUM, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Int32_Sum_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_INT32, HCCL_REDUCE_SUM, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp32_Sum_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP32, HCCL_REDUCE_SUM, 100);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

// ===== 边界条件 =====

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_1Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 1);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_LargeData)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 100000);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

// ===== 强制算法配置 =====

TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_ForcedAlg_2Rank_SingleDie_Fp16_Sum_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 100);
    scenario.algConfig = "CcuReduceScatterMesh1DMem2Mem";
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}

// ===== 调试用例 =====
TEST_F(CcuStReduceScatter, ReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_2Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, 2);
    scenario.expectedAlgName = "CcuKfcReduceScatterMesh1DMem2Mem";
    VerifyScenario(scenario);
}
