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
#include <gtest/gtest-spi.h> // EXPECT_NONFATAL_FAILURE（能力守卫端到端用例）

#include "aicpu_reduce_scatter.h"

using namespace HcclSim::AicpuSt;

namespace {

// 单 die MESH_1D 小数据（分片 ≤16MB）下 RS 的 AICPU selector 选中 InsReduceScatterMesh1D
// （reduce_scatter_auto_selector.cc SelectMeshAlgoAicpuForMesh1D:349-355；PROD/64bit 也走它 :340-341）。
// 与 AllReduce 不同：RS 不在 UseCannBridge 劫持列表（仅 ALLTOALLV/ALLREDUCE），本地全链可测。
AicpuStScenario MakeScenario(
    const TopoMeta& topoMeta, HcclDataType dataType, uint64_t count, HcclReduceOp reduceType = HCCL_REDUCE_SUM)
{
    AicpuStScenario scenario;
    scenario.topoMeta = topoMeta;
    scenario.dataType = dataType;
    scenario.opType = HcclCMDType::HCCL_CMD_REDUCE_SCATTER;
    scenario.count = count; // 每 rank 输出元素数（分片大小）
    scenario.reduceType = reduceType;
    scenario.expectedAlgName = "InsReduceScatterMesh1D";
    return scenario;
}

} // namespace

// ===== 拓扑覆盖（INT8 SUM，每 rank 输出 16B）=====
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int8_Sum_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_4Rank_SingleDie_Int8_Sum_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1, 2, 3}}}}, HCCL_DATA_TYPE_INT8, 16));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_8Rank_SingleDie_Int8_Sum_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1, 2, 3, 4, 5, 6, 7}}}}, HCCL_DATA_TYPE_INT8, 16));
}

// ===== 数据类型覆盖（2 rank, SUM, 每 rank 输出 100B）=====
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 50));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Fp32_Sum_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP32, 25));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int32_Sum_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT32, 25));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Bfp16_Sum_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_BFP16, 50));
}

// ===== 归约类型覆盖（INT8 2 rank）=====
// PROD 不测的边界说明（产品与仿真两层原因）：
// 1) 产品层：LocalReduce wrapper 对 PROD 或 64bit 类型转发 AicpuReduce（alg_data_trans_wrapper.cc:738-744），
//    其仅支持 INT64/UINT64/FP64——PROD+常规类型是 selector 与执行能力的已知错配
//    （selector :340 照选 InsReduceScatterMesh1D，执行报 DataType not support）。
// 2) 仿真层：AicpuReduce 在 host 侧直接解引用数据指针（真机 HBM 可映射），而 sim 的 buffer
//    是假设备地址（经 SimNpu::GetSlice 解码），直接解引用必 SIGSEGV——64bit/PROD 路径
//    超出 ST 仿真能力边界，需真机验证。
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int8_Max_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16, HCCL_REDUCE_MAX));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int8_Min_16B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16, HCCL_REDUCE_MIN));
}

// PROD 语义见上方 Int8_Max 用例前的边界说明（产品错配 + 仿真边界，均不可测）

// 浮点顺序敏感组合：FP16 SUM 4 rank（各 rank 并入顺序不同，golden 按 rank 分别构造）
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_4Rank_SingleDie_Fp16_Sum_100B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1, 2, 3}}}}, HCCL_DATA_TYPE_FP16, 50));
}

// ===== 数据量与边界 =====
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int8_Sum_1B)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 1));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_1KB)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 512));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Fp16_Sum_1MB)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_FP16, 512ULL * 1024ULL));
}

TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_2Rank_SingleDie_Int8_Sum_ZeroLength)
{
    VerifyScenario(MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 0));
}

// ===== 强制算法 =====
TEST_F(AicpuStReduceScatter, InsReduceScatterMesh1D_ForcedAlg_2Rank_SingleDie_Int8_Sum_16B)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16);
    scenario.algConfig = "InsReduceScatterMesh1D";
    VerifyScenario(scenario);
}

// ===== expectedAlgName 留空模式 =====
// 留空 expectedAlgName：跳过算法断言，golden 仍按实际选中算法选择，数据面照常逐字节验证
TEST_F(AicpuStReduceScatter, EmptyExpectedAlgName_SkipsAssertion_StillVerifiesData)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16);
    scenario.expectedAlgName = ""; // 框架支持的合法模式：不关心 selector 选择
    VerifyScenario(scenario);
}

// ===== 能力守卫端到端（原语声明 → 守卫 → ADD_FAILURE 全链）=====
// 本 fixture 声明 IsReduceLikeOp()=true，Int64 场景应在 rank 线程启动前被守卫拦截
// （framework 层的判定矩阵单测见 aicpu_fixture_test.cpp AicpuStScenarioGuard 系列）
TEST_F(AicpuStReduceScatter, ScenarioGuard_RejectsInt64SumEarlyWithExplanation)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT64, 8);
    EXPECT_NONFATAL_FAILURE(VerifyScenario(scenario), "unsupported scenario");
}

// PROD + 常规类型（产品错配文案分支）
TEST_F(AicpuStReduceScatter, ScenarioGuard_RejectsProdWithMismatchExplanation)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16, HCCL_REDUCE_PROD);
    EXPECT_NONFATAL_FAILURE(VerifyScenario(scenario), "mismatch");
}

// golden 的 reduceOp 前置校验契约钉子：非法 op（如误设 RESERVED）必须报
// "invalid reduceType"（THROW 经 fixture 主线程 catch 转用例失败），而非静默按
// ApplyOp default 分支生成错误期望伪装成 "byte mismatch" 误导排查方向
TEST_F(AicpuStReduceScatter, GoldenValidation_InvalidReduceTypeFailsWithReason)
{
    AicpuStScenario scenario = MakeScenario(TopoMeta{{{{0, 1}}}}, HCCL_DATA_TYPE_INT8, 16);
    scenario.reduceType = HCCL_REDUCE_RESERVED;
    EXPECT_NONFATAL_FAILURE(VerifyScenario(scenario), "invalid reduceType");
}
