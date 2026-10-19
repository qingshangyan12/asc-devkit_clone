/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include "aicpu_all_gather.h"
#include "aicpu_all_gather_mesh1d_adapter.h"

using namespace HcclSim::AicpuSt;

namespace {

const bool g_registered = []() {
    RegisterAllGatherMesh1DVariant();
    return true;
}();

AicpuStScenario MakeScenario(const TopoMeta& topoMeta, HcclDataType dataType, uint64_t count)
{
    AicpuStScenario scenario;
    scenario.topoMeta = topoMeta;
    scenario.dataType = dataType;
    scenario.opType = HcclCMDType::HCCL_CMD_ALLGATHER;
    scenario.count = count;
    scenario.expectedAlgName = "InsAllGatherMesh1D";
    return scenario;
}

} // namespace

// ===== 拓扑覆盖（INT8）=====
TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Int8_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_4Rank_SingleDie_Int8_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1, 2, 3}}}}, HCCL_DATA_TYPE_INT8, 16));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_8Rank_SingleDie_Int8_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1, 2, 3, 4, 5, 6, 7}}}}, HCCL_DATA_TYPE_INT8, 16));
}

// ===== 数据类型覆盖（2 rank, 100B/rank）=====
TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Fp16_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 50));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Fp32_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP32, 25));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Int32_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT32, 25));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Bfp16_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_BFP16, 50));
}

// ===== 数据量与边界 =====
TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Int8_1B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 1));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Fp16_1KB)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 512));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Fp16_1MB)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 512ULL * 1024ULL));
}

TEST_F(AicpuStAllGather, InsAllGatherMesh1D_2Rank_SingleDie_Int8_ZeroLength)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 0));
}

// ===== 强制算法 =====
TEST_F(AicpuStAllGather, InsAllGatherMesh1D_ForcedAlg_2Rank_SingleDie_Int8_16B)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16);
    scenario.algConfig = "InsAllGatherMesh1D";
    VerifyScenario(scenario);
}

// ===== expectedAlgName 留空模式 =====
// 留空 expectedAlgName：跳过算法断言，golden 仍按实际选中算法（InsAllGatherMesh1D）选择，数据面照常逐字节验证
TEST_F(AicpuStAllGather, EmptyExpectedAlgName_SkipsAssertion_StillVerifiesData)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16);
    scenario.expectedAlgName = ""; // 框架支持的合法模式：不关心 selector 选择
    VerifyScenario(scenario);
}
