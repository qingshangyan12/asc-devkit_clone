/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_reduce.h"

#include <vector>

using namespace HcclSim::CcuSt;

namespace {

CcuStScenario MakeScenario(const TopoMeta& topoMeta, HcclDataType dataType, uint64_t count)
{
    CcuStScenario scenario;
    scenario.topoMeta = topoMeta;
    scenario.dataType = dataType;
    scenario.opType = HcclCMDType::HCCL_CMD_ALLREDUCE;
    scenario.count = count;
    scenario.reduceType = HCCL_REDUCE_SUM;
    return scenario;
}

} // namespace

// ===== topology generalization (FP16, CcuSchedAllReduceSoleMesh) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_4Rank_SingleDie_Fp16_100Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_8Rank_SingleDie_Fp16_100Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3, 4, 5, 6, 7}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== data type generalization (2Rank, SingleDie, 100 elements) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp32_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP32, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Int32_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_INT32, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Bfp16_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_BFP16, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== reduce op generalization (2Rank, SingleDie, FP16) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_MaxOp_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.reduceType = HCCL_REDUCE_MAX;
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_MinOp_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.reduceType = HCCL_REDUCE_MIN;
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== data size generalization (2Rank, SingleDie, FP16) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_1Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 1);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_1KElem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 1024);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_1MElem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 1024 * 1024);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== large shape chunking (2Rank, FP16, multi-chunk) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_5MELem_MultiChunk)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 5 * 1024 * 1024);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_8Rank_SingleDie_Fp16_5MElem_MultiChunk)
{
    TopoMeta topo{{{{0, 1, 2, 3, 4, 5, 6, 7}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 5 * 1024 * 1024);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== non-divisible count (lastSliceSize != normalSliceSize) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_4Rank_SingleDie_Fp15_NonDivisible_100Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_8Rank_SingleDie_Fp16_NonDivisible_105Elem)
{
    TopoMeta topo{{{{0, 1, 2, 3, 4, 5, 6, 7}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 105);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== boundary conditions =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_0Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 0);
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== forced algorithm config =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_ForcedAlg_2Rank_SingleDie_Fp16_100Elem)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.algConfig = "CcuSchedAllReduceSoleMesh";
    scenario.expectedAlgName = "CcuSchedAllReduceSoleMesh";
    VerifyScenario(scenario);
}

// ===== INT8 rejected (selector should NOT match) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Int8_NotMatched)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_INT8, 100);
    VerifyScenario(scenario);
}

// ===== PROD rejected (selector should NOT match) =====
TEST_F(CcuStAllReduce, CcuSchedAllReduceSoleMesh_2Rank_SingleDie_Fp16_Prod_NotMatched)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.reduceType = HCCL_REDUCE_PROD;
    VerifyScenario(scenario);
}
