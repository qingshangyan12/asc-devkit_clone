/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "gtest/gtest.h"
#include "ccu_temp_all_gather_mesh_1D_mem2mem.h"
#include <cstring>
#include <vector>

using namespace mc2_ops_hccl;

class CcuTempAllGatherMesh1DMem2MemTest : public ::testing::Test {
protected:
    void SetUp() override {}
    void TearDown() override {}
};

// 测试构造函数和基本属性
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, construct_2rank_basic)
{
    OpParam param;
    std::vector<std::vector<u32>> subCommRanks = {{0, 1}};
    u32 rankId = 0;

    CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

    EXPECT_EQ(tmpl.GetThreadNum(), 1u);
    EXPECT_EQ(tmpl.CalcScratchMultiple(BufferType::INPUT, BufferType::OUTPUT), 0u);
    std::string desc = tmpl.Describe();
    EXPECT_TRUE(desc.find("Mem2Mem") != std::string::npos);
    EXPECT_TRUE(desc.find("2") != std::string::npos);
}

// 测试 3 rank 构造
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, construct_4rank)
{
    OpParam param;
    std::vector<std::vector<u32>> subCommRanks = {{0, 1, 2, 3}};
    u32 rankId = 2;

    CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

    EXPECT_EQ(tmpl.GetThreadNum(), 1u);
    std::string desc = tmpl.Describe();
    EXPECT_TRUE(desc.find("4") != std::string::npos);
}

// 测试 PrepareLaunchArgs 基本参数计算（3 rank, fp32, 小数据）
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, prepare_launch_args_3rank_fp32_small)
{
    u32 rankSize = 3;
    OpParam param;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP32;
    param.DataDes.count = 0; // 通过 TemplateDataParams 传入 size
    
    for (u32 rankId = 0; rankId < rankSize; rankId++) {
        std::vector<std::vector<u32>> subCommRanks = {{0, 1, 2}};
        CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

        // 模拟 KFC Server 计算好的 TemplateDataParams
        // 每个 rank 有 100 个 fp32 数据，sendCount=100, dataTypeSize=4
        u64 sendCount = 100;
        u64 dataTypeSize = 4; // fp32
        u64 sliceSize = sendCount * dataTypeSize; // 400 bytes per rank input

        TemplateDataParams tdp;
        tdp.buffInfo.inputPtr = reinterpret_cast<void*>(0x1000);
        tdp.buffInfo.outputPtr = reinterpret_cast<void*>(0x2000);
        tdp.buffInfo.inputSize = sliceSize;
        tdp.buffInfo.outputSize = sliceSize * rankSize;
        tdp.buffInfo.inBuffType = BufferType::INPUT;
        tdp.buffInfo.outBuffType = BufferType::OUTPUT;
        tdp.sliceSize = sliceSize;
        tdp.inputSliceStride = sliceSize;
        tdp.outputSliceStride = sliceSize;
        tdp.repeatNum = 1;
        tdp.inputRepeatStride = 0;
        tdp.outputRepeatStride = 0;
        tdp.tailSize = 0;

        std::vector<uint64_t> taskArgs;
        uint64_t argSize = 0;
        HcclResult ret = tmpl.PrepareLaunchArgs(param, tdp, taskArgs, argSize);
        EXPECT_EQ(ret, HCCL_SUCCESS);
        EXPECT_EQ(argSize, 15u);
        EXPECT_EQ(taskArgs.size(), 15u);

        // 验证 inputAddr = inputPtr + inBuffBaseOff = 0x1000 + 0
        EXPECT_EQ(taskArgs[0], 0x1000u);
        // 验证 outputAddr = outputPtr + outBuffBaseOff = 0x2000 + 0
        EXPECT_EQ(taskArgs[1], 0x2000u);
        // token（由 GetToken 提供，如无 stub 则为 0）
        // currentRankSliceInputOffset = inputSliceStride * mySubCommRank
        EXPECT_EQ(taskArgs[3], sliceSize * rankId);
        // currentRankSliceOutputOffset = outputSliceStride * mySubCommRank
        EXPECT_EQ(taskArgs[4], sliceSize * rankId);
        // tmpRepeatNum = UINT64_MAX - repeatNum = UINT64_MAX - 1
        EXPECT_EQ(taskArgs[5], UINT64_MAX - 1);
        // inputRepeatStride = 0
        EXPECT_EQ(taskArgs[6], 0u);
        // outputRepeatStride = 0
        EXPECT_EQ(taskArgs[7], 0u);
        // normalSliceSize = sliceSize (tailSize=0 且在循环外赋值，所有rank同)
        // taskArgs[8] 是 normalSliceSize
        // taskArgs[9] 是 lastSliceSize = 0
        EXPECT_EQ(taskArgs[9], 0u);
        // isInputOutputEqual: (inputAddr + inputSliceStride*rankId == outputAddr + outputSliceStride*rankId)
        // = (0x1000 + 400*rankId == 0x2000 + 400*rankId) = false
        EXPECT_EQ(taskArgs[10], 0u);
    }
}

// 测试带尾块的情况（8 rank, 非对齐数据）
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, prepare_launch_args_8rank_fp16_tail)
{
    u32 rankSize = 8;
    OpParam param;
    param.DataDes.dataType = HCCL_DATA_TYPE_FP16;

    for (u32 rankId = 0; rankId < rankSize; rankId++) {
        std::vector<std::vector<u32>> subCommRanks = {{0, 1, 2, 3, 4, 5, 6, 7}};
        CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

        u64 dataTypeSize = 2; // fp16
        u64 normalSliceSize = 100 * dataTypeSize; // 200 bytes
        u64 tailSize = 30; // 非对齐尾块

        TemplateDataParams tdp;
        tdp.buffInfo.inputPtr = reinterpret_cast<void*>(0x3000);
        tdp.buffInfo.outputPtr = reinterpret_cast<void*>(0x4000);
        tdp.buffInfo.inputSize = normalSliceSize;
        tdp.buffInfo.outputSize = normalSliceSize * rankSize;
        tdp.buffInfo.inBuffType = BufferType::INPUT;
        tdp.buffInfo.outBuffType = BufferType::OUTPUT;
        tdp.sliceSize = normalSliceSize;
        tdp.inputSliceStride = normalSliceSize;
        tdp.outputSliceStride = normalSliceSize;
        tdp.repeatNum = 1;
        tdp.tailSize = tailSize;

        std::vector<uint64_t> taskArgs;
        uint64_t argSize = 0;
        HcclResult ret = tmpl.PrepareLaunchArgs(param, tdp, taskArgs, argSize);
        EXPECT_EQ(ret, HCCL_SUCCESS);
        EXPECT_EQ(argSize, 15u);

        // normalSliceSize: 对于最后一个 rank (rankId == rankSize-1)，会被 tailSize 替换
        if (rankId == rankSize - 1) {
            EXPECT_EQ(taskArgs[8], tailSize);
        } else {
            EXPECT_EQ(taskArgs[8], normalSliceSize);
        }
        EXPECT_EQ(taskArgs[9], tailSize);
    }
}

// 测试多 repeat 场景
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, prepare_launch_args_multi_repeat)
{
    u32 rankSize = 4;
    OpParam param;
    param.DataDes.dataType = HCCL_DATA_TYPE_INT32;

    for (u32 rankId = 0; rankId < rankSize; rankId++) {
        std::vector<std::vector<u32>> subCommRanks = {{0, 1, 2, 3}};
        CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

        u64 dataTypeSize = 4;
        u64 sliceSize = 256;
        u64 repeatNum = 5;
        u64 inputRepeatStride = 1024;
        u64 outputRepeatStride = 2048;

        TemplateDataParams tdp;
        tdp.buffInfo.inputPtr = reinterpret_cast<void*>(0x5000);
        tdp.buffInfo.outputPtr = reinterpret_cast<void*>(0x6000);
        tdp.buffInfo.inputSize = sliceSize * repeatNum;
        tdp.buffInfo.outputSize = sliceSize * repeatNum * rankSize;
        tdp.buffInfo.inBuffType = BufferType::INPUT;
        tdp.buffInfo.outBuffType = BufferType::OUTPUT;
        tdp.sliceSize = sliceSize;
        tdp.inputSliceStride = sliceSize;
        tdp.outputSliceStride = sliceSize;
        tdp.repeatNum = repeatNum;
        tdp.inputRepeatStride = inputRepeatStride;
        tdp.outputRepeatStride = outputRepeatStride;
        tdp.tailSize = 0;

        std::vector<uint64_t> taskArgs;
        uint64_t argSize = 0;
        HcclResult ret = tmpl.PrepareLaunchArgs(param, tdp, taskArgs, argSize);
        EXPECT_EQ(ret, HCCL_SUCCESS);
        EXPECT_EQ(argSize, 15u);

        EXPECT_EQ(taskArgs[5], UINT64_MAX - repeatNum);
        EXPECT_EQ(taskArgs[6], inputRepeatStride);
        EXPECT_EQ(taskArgs[7], outputRepeatStride);
        EXPECT_EQ(taskArgs[8], sliceSize);
        EXPECT_EQ(taskArgs[9], 0u);
    }
}

// 测试 input == output 同地址场景（isInputOutputEqual 应该为 1）
TEST_F(CcuTempAllGatherMesh1DMem2MemTest, prepare_launch_args_input_equals_output)
{
    u32 rankSize = 4;
    OpParam param;
    param.DataDes.dataType = HCCL_DATA_TYPE_INT64;

    for (u32 rankId = 0; rankId < rankSize; rankId++) {
        std::vector<std::vector<u32>> subCommRanks = {{0, 1, 2, 3}};
        CcuTempAllGatherMesh1DMem2Mem tmpl(param, rankId, subCommRanks);

        uint64_t baseAddr = 0x7000;
        u64 sliceSize = 1024;

        // 让 input + inputSliceStride*rankId == output + outputSliceStride*rankId
        // 即 baseAddr + 1024*r == baseAddr + 1024*r，总是 true
        TemplateDataParams tdp;
        tdp.buffInfo.inputPtr = reinterpret_cast<void*>(baseAddr);
        tdp.buffInfo.outputPtr = reinterpret_cast<void*>(baseAddr);
        tdp.buffInfo.inputSize = sliceSize * rankSize;
        tdp.buffInfo.outputSize = sliceSize * rankSize;
        tdp.buffInfo.inBuffType = BufferType::INPUT;
        tdp.buffInfo.outBuffType = BufferType::OUTPUT;
        tdp.sliceSize = sliceSize;
        tdp.inputSliceStride = sliceSize;
        tdp.outputSliceStride = sliceSize;
        tdp.repeatNum = 1;
        tdp.tailSize = 0;

        std::vector<uint64_t> taskArgs;
        uint64_t argSize = 0;
        HcclResult ret = tmpl.PrepareLaunchArgs(param, tdp, taskArgs, argSize);
        EXPECT_EQ(ret, HCCL_SUCCESS);
        EXPECT_EQ(argSize, 15u);

        // inputAddr == outputAddr 且 stride 相同，所以 isInputOutputEqual = 1
        EXPECT_EQ(taskArgs[10], 1u);
    }
}

