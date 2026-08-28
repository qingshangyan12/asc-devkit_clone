/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_runtime_checker.h"
#include "ccu_sim_world.h"
#include "ccu_scenario.h"

#include <gtest/gtest.h>
#include <cstring>

using namespace HcclSim::CcuSt;

namespace {

RankMemory MakeMemWithInput(const std::vector<uint8_t>& input, uint64_t outputSize)
{
    RankMemory mem;
    mem.input = input;
    mem.output.assign(outputSize, 0);
    mem.parameter.assign(64, 0);
    return mem;
}

CcuStScenario MakeScenario(HcclDataType dataType, HcclReduceOp reduceOp, uint32_t rankSize, uint64_t perRankBytes)
{
    CcuStScenario s;
    s.dataType = dataType;
    s.reduceType = reduceOp;
    s.count = perRankBytes; // used as byte count in expectation
    s.sizes.assign(rankSize, std::vector<uint64_t>(rankSize, perRankBytes));
    return s;
}

} // namespace

// ===== L5: Copy-only 期望值 =====

TEST(CcuStFrameworkL5, CopyOnly_2Rank)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT8, HCCL_REDUCE_RESERVED, rankSize, bytes);
    // override sizes to byte count
    scenario.sizes.assign(rankSize, std::vector<uint64_t>(rankSize, bytes));

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_RESERVED;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    for (uint32_t r = 0; r < rankSize; ++r) {
        data.memories[r] = MakeMemWithInput(
            {static_cast<uint8_t>(0x10 + r), static_cast<uint8_t>(0x20 + r), static_cast<uint8_t>(0x30 + r),
             static_cast<uint8_t>(0x40 + r)},
            bytes);
        for (uint32_t d = 0; d < rankSize; ++d) {
            data.srcOffsets[r][d] = 0;
            data.dstOffsets[r][d] = 0;
        }
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    // rank 0's output: copy from rank 0 (0x10,0x20,0x30,0x40) then copy from rank 1 (0x11,0x21,0x31,0x41) — overwrites
    ASSERT_EQ(exp.outputs.size(), rankSize);
    EXPECT_EQ(exp.outputs[0][0], 0x11); // rank 1 overwrites rank 0
    EXPECT_EQ(exp.outputs[1][0], 0x11); // rank 1 overwrites rank 0
}

// ===== L5: Reduce SUM (首次copy后续reduce) =====

TEST(CcuStFrameworkL5, ReduceSum_Int32_2Rank)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 4; // 1 int32
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_SUM, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_SUM;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    int32_t val0 = 10, val1 = 20;
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), r == 0 ? &val0 : &val1, bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    int32_t expected = val0 + val1; // first copy val0, then reduce val1
    int32_t actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_EQ(actual, expected);
}

TEST(CcuStFrameworkL5, ReduceMax_Int32_3Rank)
{
    const uint32_t rankSize = 3;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MAX, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_MAX;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    int32_t vals[] = {10, 50, 30};
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), &vals[r], bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    // first copy val0=10, then max(10,50)=50, then max(50,30)=50
    int32_t actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_EQ(actual, 50);
}

TEST(CcuStFrameworkL5, ReduceMin_Int32_3Rank)
{
    const uint32_t rankSize = 3;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MIN, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_MIN;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    int32_t vals[] = {10, 5, 30};
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), &vals[r], bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    // first copy 10, then min(10,5)=5, then min(5,30)=5
    int32_t actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_EQ(actual, 5);
}

TEST(CcuStFrameworkL5, ReduceProd_Int32_2Rank)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_PROD, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_PROD;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    int32_t vals[] = {3, 4};
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), &vals[r], bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    // first copy 3, then prod(3,4)=12
    int32_t actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_EQ(actual, 12);
}

TEST(CcuStFrameworkL5, ReduceFp16Sum_2Rank)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 2; // 1 fp16
    auto scenario = MakeScenario(HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_SUM;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    uint16_t vals[] = {0x3C00, 0x4000}; // 1.0, 2.0
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), &vals[r], bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    uint16_t actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_EQ(actual, 0x4200); // 3.0
}

TEST(CcuStFrameworkL5, ReduceFp32Sum_2Rank)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_FP32, HCCL_REDUCE_SUM, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_SUM;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    float vals[] = {1.5f, 2.5f};
    for (uint32_t r = 0; r < rankSize; ++r) {
        std::vector<uint8_t> inp(bytes);
        std::memcpy(inp.data(), &vals[r], bytes);
        data.memories[r] = MakeMemWithInput(inp, bytes);
    }

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    ASSERT_TRUE(result.Ok()) << result.message;
    float actual = 0;
    std::memcpy(&actual, exp.outputs[0].data(), bytes);
    EXPECT_FLOAT_EQ(actual, 4.0f);
}

// ===== L5: 输出比对 =====

TEST(CcuStFrameworkL5, CheckOutput_Match)
{
    RankMemory mem;
    mem.input.assign(1, 0);
    mem.output = {0xAB};
    mem.parameter.assign(64, 0);
    std::vector<RankMemory> mems = {mem};
    RuntimeExpectation exp;
    exp.outputs = {{0xAB}};
    auto result = CheckRuntimeOutput(mems, exp, RuntimeWorld{});
    EXPECT_TRUE(result.Ok());
}

TEST(CcuStFrameworkL5, CheckOutput_Mismatch)
{
    std::vector<RankMemory> mems = {MakeMemWithInput({}, 1)};
    mems[0].output = {0xCD};
    RuntimeExpectation exp;
    exp.outputs = {{0xAB}};
    auto result = CheckRuntimeOutput(mems, exp, RuntimeWorld{});
    EXPECT_FALSE(result.Ok());
    EXPECT_NE(result.message.find("mismatch"), std::string::npos);
}

TEST(CcuStFrameworkL5, CheckOutput_RankCountMismatch)
{
    std::vector<RankMemory> mems(2);
    RuntimeExpectation exp;
    exp.outputs.resize(1);
    auto result = CheckRuntimeOutput(mems, exp, RuntimeWorld{});
    EXPECT_FALSE(result.Ok());
}

// ===== L5: 越界检测 =====

TEST(CcuStFrameworkL5, OutOfRange)
{
    const uint32_t rankSize = 2;
    const uint64_t bytes = 4;
    auto scenario = MakeScenario(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_SUM, rankSize, bytes);

    ScenarioData data;
    data.reduceOp = HCCL_REDUCE_SUM;
    data.memories.resize(rankSize);
    data.srcOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));
    data.dstOffsets.assign(rankSize, std::vector<uint64_t>(rankSize, 0));

    // Input too small for the transfer
    data.memories[0] = MakeMemWithInput({0x01, 0x02}, bytes); // only 2 bytes, need 4
    data.memories[1] = MakeMemWithInput({0x03, 0x04}, bytes);

    RuntimeExpectation exp;
    auto result = CaptureRuntimeExpectation(data, scenario, exp);
    EXPECT_FALSE(result.Ok());
    EXPECT_NE(result.message.find("out of range"), std::string::npos);
}
