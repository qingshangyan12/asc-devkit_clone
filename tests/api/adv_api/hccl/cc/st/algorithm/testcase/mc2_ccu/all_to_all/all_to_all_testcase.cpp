/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_all_to_all.h"

#include <vector>

using namespace HcclSim::CcuSt;

namespace {

CcuStScenario MakeScenario(const TopoMeta &topoMeta, HcclDataType dataType, uint64_t sliceSize)
{
    const uint32_t rankSize = CcuStFixture::CountRanks(topoMeta);
    CcuStScenario scenario;
    scenario.topoMeta = topoMeta;
    scenario.dataType = dataType;
    scenario.opType = HcclCMDType::HCCL_CMD_ALLTOALL;
    scenario.sizes.assign(rankSize, std::vector<uint64_t>(rankSize, sliceSize));
    scenario.expectedAlgName = "CcuAlltoAllMesh1D";
    return scenario;
}

} // namespace

// ===== 拓扑泛化（FP16, Mesh1D）=====
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100));
}

TEST_F(CcuStAllToAll, Mesh1D_4Rank_SingleDie_Fp16_64B)
{
    TopoMeta topo{{{{0, 1, 2, 3}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 64));
}

TEST_F(CcuStAllToAll, Mesh1D_8Rank_SingleDie_Fp16_64B)
{
    TopoMeta topo{{{{0, 1, 2, 3, 4, 5, 6, 7}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 64));
}

// ===== 数据类型泛化（2Rank, SingleDie, 100B/rank）=====
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp32_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP32, 100));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Int8_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_INT8, 100));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Int32_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_INT32, 100));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Bfp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_BFP16, 100));
}

// ===== 数据量泛化（2Rank, SingleDie, FP16）=====
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_1KB)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 1024));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_1MB)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 1024 * 1024));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_10MB)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 10 * 1024 * 1024));
}

// ===== 边界条件 =====
TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_0B)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 0));
}

TEST_F(CcuStAllToAll, Mesh1D_2Rank_SingleDie_Fp16_256MB)
{
    TopoMeta topo{{{{0, 1}}}};
    VerifyScenario(MakeScenario(topo, HCCL_DATA_TYPE_FP16, 256ULL * 1024 * 1024));
}

// ===== 负向校验 =====
TEST_F(CcuStAllToAll, Negative_OutputCorruption_2Rank_SingleDie_Fp16_80B)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 80);
    scenario.corruptOutput = true;
    VerifyScenario(scenario);
}

// ===== 强制算法配置 =====
TEST_F(CcuStAllToAll, ForcedAlg_2Rank_SingleDie_Fp16_100B)
{
    TopoMeta topo{{{{0, 1}}}};
    CcuStScenario scenario = MakeScenario(topo, HCCL_DATA_TYPE_FP16, 100);
    scenario.algConfig = "CcuAlltoAllMesh1D";
    scenario.expectedAlgName = "CcuAlltoAllMesh1D";
    VerifyScenario(scenario);
}
