/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "ccu_primitives_stub.h"
#include "ccu_kernel_proxy.h"
#include "ccu_sim_world.h"
#include "ccu_checker.h"
#include "ccu_dag_checker.h"

#include <gtest/gtest.h>
#include <cstring>
#include <vector>

using namespace HcclSim::CcuSt;
using namespace AscendC::ccu;

namespace {

struct ReduceKernelConfig {
    uint32_t rankId{0};
    uint32_t rankSize{1};
    HcclDataType dataType{HCCL_DATA_TYPE_INT32};
    HcclReduceOp opType{HCCL_REDUCE_SUM};
    std::vector<ChannelHandle> channels;
};

struct ReduceLaunchConfig {
    uint64_t srcAddress{0};
    uint64_t dstAddress{0};
    uint64_t length{0};
    uint64_t remoteAddress{0};
};

Result CaptureLocalReduceKernel(void* kernelArg)
{
    auto* config = static_cast<ReduceKernelConfig*>(kernelArg);
    Variable srcAddr, dstAddr, lenVar;
    LoadArg(srcAddr, 0);
    LoadArg(dstAddr, 1);
    LoadArg(lenVar, 2);

    LocalAddr srcLocal, dstLocal;
    srcLocal.addr = srcAddr;
    dstLocal.addr = dstAddr;

    Event event;
    LocalReduce(dstLocal, srcLocal, lenVar, config->dataType, config->opType, event);
    CompilerContext::Current().ConfigureProgram(config->rankId, config->rankSize, {});
    return Result::SUCCESS;
}

Result CaptureWriteReduceKernel(void* kernelArg)
{
    auto* config = static_cast<ReduceKernelConfig*>(kernelArg);
    Variable localAddr, remoteAddr, lenVar;
    LoadArg(localAddr, 0);
    LoadArg(remoteAddr, 1);
    LoadArg(lenVar, 2);

    LocalAddr localLocal;
    RemoteAddr remoteLocal;
    localLocal.addr = localAddr;
    remoteLocal.addr = remoteAddr;

    Event event;
    ChannelHandle ch = MakeChannel(config->rankId, config->rankSize == 1 ? 0 : (config->rankId == 0 ? 1 : 0));
    WriteReduce(ch, remoteLocal, localLocal, lenVar, config->dataType, config->opType, event);
    CompilerContext::Current().ConfigureProgram(config->rankId, config->rankSize, {ch});
    return Result::SUCCESS;
}

Result CaptureReadReduceKernel(void* kernelArg)
{
    auto* config = static_cast<ReduceKernelConfig*>(kernelArg);
    Variable localAddrVar, remoteAddrVar, lenVar;
    LoadArg(localAddrVar, 0);
    LoadArg(remoteAddrVar, 1);
    LoadArg(lenVar, 2);

    LocalAddr localLocal;
    RemoteAddr remoteLocal;
    localLocal.addr = localAddrVar;
    remoteLocal.addr = remoteAddrVar;

    Event event;
    ChannelHandle ch = MakeChannel(config->rankId, config->rankSize == 1 ? 0 : (config->rankId == 0 ? 1 : 0));
    ReadReduce(ch, localLocal, remoteLocal, lenVar, config->dataType, config->opType, event);
    CompilerContext::Current().ConfigureProgram(config->rankId, config->rankSize, {ch});
    return Result::SUCCESS;
}

Result CaptureBufferReduceKernel(void* kernelArg)
{
    auto* config = static_cast<ReduceKernelConfig*>(kernelArg);
    Variable lenVar;
    LoadArg(lenVar, 0);

    constexpr uint32_t BUFFER_COUNT = 3;
    CcuBuffer buffers[BUFFER_COUNT] = {CcuBuffer(), CcuBuffer(), CcuBuffer()};
    Variable srcAddr;
    LoadArg(srcAddr, 1);
    LocalAddr srcLocal;
    srcLocal.addr = srcAddr;

    Event event;
    LocalReduce(buffers, BUFFER_COUNT, config->dataType, config->dataType, config->opType, lenVar, event);
    CompilerContext::Current().ConfigureProgram(config->rankId, config->rankSize, {});
    return Result::SUCCESS;
}

KernelHandle RegisterReduceKernel(const std::string& name, const RegisterManager::KernelFunction& fn, void* arg)
{
    RegisterManager::Global().Reset();
    InstanceHandle instance = 1;
    KernelHandle handle = 0;
    RegisterManager::Global().RegisterStart(instance);
    RegisterManager::Global().Register(instance, name, fn, arg, handle);
    RegisterManager::Global().RegisterEnd(instance);
    return handle;
}

std::vector<uint64_t> MakeTaskArgs(const ReduceLaunchConfig& cfg)
{
    return {cfg.srcAddress, cfg.dstAddress, cfg.length};
}

} // namespace

// ===== IR 捕获验证 =====

TEST(CcuStReducePrimitive, LocalReduce_IrCapture)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("LocalReduce", CaptureLocalReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    bool foundReduce = false;
    for (const auto& op : program->operations) {
        if (op.code == OpCode::LOCAL_REDUCE) {
            foundReduce = true;
            EXPECT_EQ(op.dataType, HCCL_DATA_TYPE_INT32);
            EXPECT_EQ(op.opType, HCCL_REDUCE_SUM);
        }
        EXPECT_NE(op.code, OpCode::LOCAL_COPY) << "LocalReduce should not emit LOCAL_COPY";
    }
    EXPECT_TRUE(foundReduce) << "Program should contain at least one LOCAL_REDUCE operation";
}

TEST(CcuStReducePrimitive, WriteReduce_IrCapture)
{
    ReduceKernelConfig config;
    config.rankSize = 2;
    config.rankId = 0;
    config.dataType = HCCL_DATA_TYPE_FP32;
    config.opType = HCCL_REDUCE_MAX;
    KernelHandle handle = RegisterReduceKernel("WriteReduce", CaptureWriteReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    bool foundReduce = false;
    for (const auto& op : program->operations) {
        if (op.code == OpCode::WRITE_REDUCE) {
            foundReduce = true;
            EXPECT_EQ(op.dataType, HCCL_DATA_TYPE_FP32);
            EXPECT_EQ(op.opType, HCCL_REDUCE_MAX);
        }
        EXPECT_NE(op.code, OpCode::WRITE) << "WriteReduce should not emit WRITE";
    }
    EXPECT_TRUE(foundReduce);
}

TEST(CcuStReducePrimitive, ReadReduce_IrCapture)
{
    ReduceKernelConfig config;
    config.rankSize = 2;
    config.rankId = 0;
    config.dataType = HCCL_DATA_TYPE_INT8;
    config.opType = HCCL_REDUCE_PROD;
    KernelHandle handle = RegisterReduceKernel("ReadReduce", CaptureReadReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    bool foundReduce = false;
    for (const auto& op : program->operations) {
        if (op.code == OpCode::READ_REDUCE) {
            foundReduce = true;
            EXPECT_EQ(op.dataType, HCCL_DATA_TYPE_INT8);
            EXPECT_EQ(op.opType, HCCL_REDUCE_PROD);
        }
        EXPECT_NE(op.code, OpCode::WRITE) << "ReadReduce should not emit WRITE";
    }
    EXPECT_TRUE(foundReduce);
}

TEST(CcuStReducePrimitive, LocalReduce_BufferArray_GeneratesMultipleOps)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("BufferReduce", CaptureBufferReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    uint32_t reduceCount = 0;
    for (const auto& op : program->operations) {
        if (op.code == OpCode::LOCAL_REDUCE) {
            ++reduceCount;
            EXPECT_EQ(op.dataType, HCCL_DATA_TYPE_INT32);
        }
    }
    EXPECT_EQ(reduceCount, 2U) << "Buffer array LocalReduce(3 buffers) should generate 2 LOCAL_REDUCE ops (reduce buf[1],buf[2] into buf[0])";
}

// ===== 运行时执行验证 =====

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Sum_Int32)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Sum", CaptureLocalReduceKernel, &config);

    constexpr uint64_t elemCount = 4;
    constexpr uint64_t byteLen = elemCount * sizeof(int32_t);

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    int32_t srcData[4] = {1, 2, 3, 4};
    int32_t dstData[4] = {10, 20, 30, 40};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    int32_t result[4];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_EQ(result[0], 11);
    EXPECT_EQ(result[1], 22);
    EXPECT_EQ(result[2], 33);
    EXPECT_EQ(result[3], 44);
}

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Max_Int32)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_MAX;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Max", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 4 * sizeof(int32_t);

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    int32_t srcData[4] = {1, 25, 3, 45};
    int32_t dstData[4] = {10, 20, 30, 40};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    int32_t result[4];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_EQ(result[0], 10);
    EXPECT_EQ(result[1], 25);
    EXPECT_EQ(result[2], 30);
    EXPECT_EQ(result[3], 45);
}

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Min_Int32)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_MIN;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Min", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 4 * sizeof(int32_t);

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    int32_t srcData[4] = {1, 5, 50, 4};
    int32_t dstData[4] = {10, 20, 30, 40};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    int32_t result[4];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_EQ(result[0], 1);
    EXPECT_EQ(result[1], 5);
    EXPECT_EQ(result[2], 30);
    EXPECT_EQ(result[3], 4);
}

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Sum_Int8)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT8;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Int8", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 4;

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    int8_t srcData[4] = {1, 2, 3, 4};
    int8_t dstData[4] = {10, 20, 30, 40};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    int8_t result[4];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_EQ(result[0], 11);
    EXPECT_EQ(result[1], 22);
    EXPECT_EQ(result[2], 33);
    EXPECT_EQ(result[3], 44);
}

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Sum_Fp32)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_FP32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Fp32", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 2 * sizeof(float);

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    float srcData[2] = {1.5f, 2.5f};
    float dstData[2] = {10.0f, 20.0f};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    float result[2];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_FLOAT_EQ(result[0], 11.5f);
    EXPECT_FLOAT_EQ(result[1], 22.5f);
}

TEST(CcuStReducePrimitive, LocalReduce_Runtime_Prod_Int32)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_PROD;
    KernelHandle handle = RegisterReduceKernel("LocalReduce_Prod", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 3 * sizeof(int32_t);

    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    int32_t srcData[3] = {2, 3, 4};
    int32_t dstData[3] = {10, 20, 30};
    std::memcpy(memory.input.data(), srcData, byteLen);
    std::memcpy(memory.output.data(), dstData, byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    int32_t result[3];
    std::memcpy(result, memory.output.data(), byteLen);
    EXPECT_EQ(result[0], 20);
    EXPECT_EQ(result[1], 60);
    EXPECT_EQ(result[2], 120);
}

// ===== 静态检查器验证 =====

TEST(CcuStReducePrimitive, Checker_AcceptsLocalReduce)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("Checker_LocalReduce", CaptureLocalReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    CheckResult result = CcuProgramChecker().Check(*program);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStReducePrimitive, Checker_AcceptsWriteReduce)
{
    ReduceKernelConfig config;
    config.rankSize = 2;
    config.rankId = 0;
    config.dataType = HCCL_DATA_TYPE_FP32;
    config.opType = HCCL_REDUCE_MAX;
    KernelHandle handle = RegisterReduceKernel("Checker_WriteReduce", CaptureWriteReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    CheckResult result = CcuProgramChecker().Check(*program);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStReducePrimitive, Checker_AcceptsReadReduce)
{
    ReduceKernelConfig config;
    config.rankSize = 2;
    config.rankId = 0;
    config.dataType = HCCL_DATA_TYPE_INT8;
    config.opType = HCCL_REDUCE_PROD;
    KernelHandle handle = RegisterReduceKernel("Checker_ReadReduce", CaptureReadReduceKernel, &config);
    const Program* program = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(program, nullptr);

    CheckResult result = CcuProgramChecker().Check(*program);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ===== DAG 检查器验证：reduce 操作产生 ExecutionLog =====

TEST(CcuStReducePrimitive, DagChecker_IncludesReduceOps)
{
    ReduceKernelConfig config;
    config.dataType = HCCL_DATA_TYPE_INT32;
    config.opType = HCCL_REDUCE_SUM;
    KernelHandle handle = RegisterReduceKernel("Dag_LocalReduce", CaptureLocalReduceKernel, &config);

    constexpr uint64_t byteLen = 4 * sizeof(int32_t);
    RankMemory memory;
    memory.input.resize(byteLen);
    memory.output.resize(byteLen);
    std::memcpy(memory.input.data(), std::vector<int32_t>{1, 2, 3, 4}.data(), byteLen);
    std::memcpy(memory.output.data(), std::vector<int32_t>{10, 20, 30, 40}.data(), byteLen);

    ReduceLaunchConfig cfg;
    cfg.srcAddress = memory.InputAddress();
    cfg.dstAddress = memory.OutputAddress();
    cfg.length = byteLen;

    std::vector<RankLaunch> launches = {{handle, MakeTaskArgs(cfg), &memory}};

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    const ExecutionLog& log = world.ExecutionLogData();
    ASSERT_FALSE(log.rankEntries.empty());
    ASSERT_FALSE(log.rankEntries[0].empty());
    EXPECT_EQ(log.rankEntries[0][0].code, OpCode::LOCAL_REDUCE);

    CheckResult dagResult = CcuDagChecker().Check(log);
    EXPECT_TRUE(dagResult.Ok()) << dagResult.message;
}
