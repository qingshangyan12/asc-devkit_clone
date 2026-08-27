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

#include <gtest/gtest.h>
#include <cstring>
#include <functional>
#include <vector>

using namespace HcclSim::CcuSt;
using namespace AscendC::ccu;

namespace {

KernelHandle BuildProgram(const std::string& name, const RegisterManager::KernelFunction& fn)
{
    RegisterManager::Global().Reset();
    InstanceHandle instance = 1;
    KernelHandle handle = 0;
    RegisterManager::Global().RegisterStart(instance);
    RegisterManager::Global().Register(instance, name, fn, nullptr, handle);
    RegisterManager::Global().RegisterEnd(instance);
    return handle;
}

// Single-rank program builder: wraps body with ConfigureProgram(0, 1, {})
KernelHandle BuildSingleRank(const std::string& name, const std::function<void()>& body)
{
    auto fn = [&body](void*) {
        body();
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    return BuildProgram(name, fn);
}

RankMemory MakeMemory(uint64_t inputSize, uint64_t outputSize)
{
    RankMemory mem;
    mem.input.assign(inputSize, 0);
    mem.output.assign(outputSize, 0);
    mem.parameter.assign(1024, 0);
    return mem;
}

#define LAUNCH_SINGLE(h, mem)                                                                      \
    do {                                                                                           \
        std::vector<RankLaunch> launches = {{(h), {}, &(mem)}};                                   \
        RuntimeWorld world;                                                                        \
        ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();                   \
    } while (0)

} // namespace

// ===== L3: 变量操作 =====

TEST(CcuStFrameworkL3, AssignImm)
{
    auto h = BuildSingleRank("t", []() { Variable v; v = 42U; });
    RankMemory mem = MakeMemory(64, 64);
    LAUNCH_SINGLE(h, mem);
}

TEST(CcuStFrameworkL3, AssignVar)
{
    auto h = BuildSingleRank("t", []() {
        Variable a, b;
        a = 99U;
        b = a;
    });
    RankMemory mem = MakeMemory(64, 64);
    LAUNCH_SINGLE(h, mem);
}

TEST(CcuStFrameworkL3, AddVar)
{
    auto h = BuildSingleRank("t", []() {
        Variable a, b, c;
        a = 10U;
        b = 20U;
        c = a + b;
    });
    RankMemory mem = MakeMemory(64, 64);
    LAUNCH_SINGLE(h, mem);
}

TEST(CcuStFrameworkL3, LoadArg)
{
    auto h = BuildSingleRank("t", []() {
        Variable v;
        LoadArg(v, 0);
    });
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {123}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

TEST(CcuStFrameworkL3, LoadArgMultiMission)
{
    auto h = BuildSingleRank("t", []() {
        Variable vars[15];
        for (uint32_t i = 0; i < 15; ++i) {
            LoadArg(vars[i], i);
        }
    });
    RankMemory mem = MakeMemory(64, 64);
    std::vector<uint64_t> args(15, 0);
    std::vector<RankLaunch> launches = {{h, args, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

// ===== L3: 内存操作 =====

TEST(CcuStFrameworkL3, StoreAndLoad)
{
    auto fn = [](void*) {
        Variable addr, val;
        LoadArg(addr, 0);
        LoadArg(val, 1);
        Store(addr, val);
        Variable loaded;
        Load(addr, loaded);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    uint64_t val = 0xDEADBEEF;
    std::vector<RankLaunch> launches = {{h, {mem.OutputAddress(), val}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    uint64_t result = 0;
    std::memcpy(&result, mem.output.data(), sizeof(uint64_t));
    EXPECT_EQ(result, val);
}

// ===== L3: LocalCopy =====

TEST(CcuStFrameworkL3, LocalCopy)
{
    auto fn = [](void*) {
        Variable srcAddr, dstAddr, len;
        LoadArg(srcAddr, 0);
        LoadArg(dstAddr, 1);
        LoadArg(len, 2);
        LocalAddr src, dst;
        src.addr = srcAddr;
        dst.addr = dstAddr;
        Event evt;
        LocalCopy(dst, src, len, evt);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    for (uint32_t i = 0; i < 32; ++i) mem.input[i] = static_cast<uint8_t>(0x40 + i);
    std::vector<RankLaunch> launches = {{h, {mem.InputAddress(), mem.OutputAddress(), 32}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    for (uint32_t i = 0; i < 32; ++i) {
        EXPECT_EQ(mem.output[i], static_cast<uint8_t>(0x40 + i)) << "byte " << i;
    }
}

// ===== L3: LocalReduce — 数据类型 × 操作 =====

namespace {

void TestLocalReduce(HcclDataType dataType, HcclReduceOp opType,
    const std::vector<uint8_t>& src, const std::vector<uint8_t>& dst, const std::vector<uint8_t>& expected)
{
    auto fn = [=](void*) {
        Variable srcAddr, dstAddr, len;
        LoadArg(srcAddr, 0);
        LoadArg(dstAddr, 1);
        LoadArg(len, 2);
        LocalAddr src, dst;
        src.addr = srcAddr;
        dst.addr = dstAddr;
        Event evt;
        LocalReduce(dst, src, len, dataType, opType, evt);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    uint64_t len = src.size();
    RankMemory mem = MakeMemory(len + 16, len + 16);
    std::memcpy(mem.input.data(), src.data(), len);
    std::memcpy(mem.output.data(), dst.data(), len);
    std::vector<RankLaunch> launches = {{h, {mem.InputAddress(), mem.OutputAddress(), len}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    for (size_t i = 0; i < expected.size(); ++i) {
        EXPECT_EQ(mem.output[i], expected[i]) << "byte " << i;
    }
}

} // namespace

TEST(CcuStFrameworkL3, LocalReduce_Int32_Sum)
{
    int32_t src[] = {1, 2, 3, 4};
    int32_t dst[] = {10, 20, 30, 40};
    int32_t exp[] = {11, 22, 33, 44};
    TestLocalReduce(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_SUM,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 16),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 16),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 16));
}

TEST(CcuStFrameworkL3, LocalReduce_Int32_Max)
{
    int32_t src[] = {5, 50, 3};
    int32_t dst[] = {10, 20, 30};
    int32_t exp[] = {10, 50, 30};
    TestLocalReduce(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MAX,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 12),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 12),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 12));
}

TEST(CcuStFrameworkL3, LocalReduce_Int32_Min)
{
    int32_t src[] = {5, 1, 30};
    int32_t dst[] = {10, 20, 3};
    int32_t exp[] = {5, 1, 3};
    TestLocalReduce(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MIN,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 12),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 12),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 12));
}

TEST(CcuStFrameworkL3, LocalReduce_Int32_Prod)
{
    int32_t src[] = {2, 3, 4};
    int32_t dst[] = {10, 20, 30};
    int32_t exp[] = {20, 60, 120};
    TestLocalReduce(HCCL_DATA_TYPE_INT32, HCCL_REDUCE_PROD,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 12),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 12),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 12));
}

TEST(CcuStFrameworkL3, LocalReduce_Int8_Sum)
{
    int8_t src[] = {1, 2, 3, 4};
    int8_t dst[] = {10, 20, 30, 40};
    int8_t exp[] = {11, 22, 33, 44};
    TestLocalReduce(HCCL_DATA_TYPE_INT8, HCCL_REDUCE_SUM,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 4),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 4),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 4));
}

TEST(CcuStFrameworkL3, LocalReduce_Fp32_Sum)
{
    float src[] = {1.5f, 2.5f};
    float dst[] = {10.0f, 20.0f};
    float exp[] = {11.5f, 22.5f};
    TestLocalReduce(HCCL_DATA_TYPE_FP32, HCCL_REDUCE_SUM,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 8),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 8),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 8));
}

TEST(CcuStFrameworkL3, LocalReduce_Fp16_Sum)
{
    uint16_t src[] = {0x3C00, 0x4000, 0x4200, 0x4400}; // 1.0, 2.0, 3.0, 4.0
    uint16_t dst[] = {0x3C00, 0x3C00, 0x3C00, 0x3C00}; // 1.0, 1.0, 1.0, 1.0
    uint16_t exp[] = {0x4000, 0x4200, 0x4400, 0x4500}; // 2.0, 3.0, 4.0, 5.0
    TestLocalReduce(HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 8),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 8),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 8));
}

TEST(CcuStFrameworkL3, LocalReduce_Int64_Sum)
{
    int64_t src[] = {100, 200};
    int64_t dst[] = {1, 2};
    int64_t exp[] = {101, 202};
    TestLocalReduce(HCCL_DATA_TYPE_INT64, HCCL_REDUCE_SUM,
        std::vector<uint8_t>((uint8_t*)src, (uint8_t*)src + 16),
        std::vector<uint8_t>((uint8_t*)dst, (uint8_t*)dst + 16),
        std::vector<uint8_t>((uint8_t*)exp, (uint8_t*)exp + 16));
}

// ===== L3: 控制流 =====

TEST(CcuStFrameworkL3, WhileImmediateExit)
{
    auto fn = [](void*) {
        Variable v;
        v = 5U;
        CCU_WHILE(v == 10U) { v = 1U; }
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    auto* prog = RegisterManager::Global().GetProgram(h);
    ASSERT_NE(prog, nullptr);
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

TEST(CcuStFrameworkL3, WhileOneIteration)
{
    auto fn = [](void*) {
        Variable v;
        v = 0U;
        CCU_WHILE(v == 0U) { v = 1U; }
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    auto* prog = RegisterManager::Global().GetProgram(h);
    ASSERT_NE(prog, nullptr);
    for (size_t i = 0; i < prog->operations.size(); ++i) {
        const auto& op = prog->operations[i];
        fprintf(stderr, "  IR[%zu] code=%d dst=%lu src0=%lu imm=%lu equal=%d peer=%zu\n",
                i, static_cast<int>(op.code), op.dst, op.src0, op.immediate,
                static_cast<int>(op.equal), op.peerIndex);
    }
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

TEST(CcuStFrameworkL3, IfTrue)
{
    auto fn = [](void*) {
        Variable v, result, addr;
        v = 1U;
        result = 0U;
        CCU_IF(v == 1U) { result = 100U; }
        addr = 0U;
        LoadArg(addr, 0);
        Store(addr, result);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {mem.OutputAddress()}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), sizeof(uint64_t));
    EXPECT_EQ(val, 100U);
}

TEST(CcuStFrameworkL3, IfFalse)
{
    auto fn = [](void*) {
        Variable v, result, addr;
        v = 5U;
        result = 0U;
        CCU_IF(v == 1U) { result = 100U; }
        LoadArg(addr, 0);
        Store(addr, result);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {mem.OutputAddress()}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), sizeof(uint64_t));
    EXPECT_EQ(val, 0U);
}

TEST(CcuStFrameworkL3, WhileLoop)
{
    auto fn = [](void*) {
        Variable counter, one, addr;
        counter = 0U;
        one = 1U;
        CCU_WHILE(counter != 3U) { counter += one; }
        LoadArg(addr, 0);
        Store(addr, counter);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {mem.OutputAddress()}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), sizeof(uint64_t));
    EXPECT_EQ(val, 3U);
}

// ===== L3: 2-rank 跨 rank 操作 =====

TEST(CcuStFrameworkL3, TwoRankWrite)
{
    ChannelHandle ch01 = MakeChannel(0, 1);
    ChannelHandle ch10 = MakeChannel(1, 0);
    auto fn0 = [ch01](void*) {
        Variable localAddr, remoteAddr, len;
        LoadArg(localAddr, 0);
        LoadArg(remoteAddr, 1);
        LoadArg(len, 2);
        LocalAddr local;
        RemoteAddr remote;
        local.addr = localAddr;
        remote.addr = remoteAddr;
        Event evt;
        Write(ch01, remote, local, len, evt);
        NotifyRecord(ch01, 0, 0x1);
        CompilerContext::Current().ConfigureProgram(0, 2, {ch01});
        return Result::SUCCESS;
    };
    auto fn1 = [ch10](void*) {
        // rank 1 waits for rank 0's notify, then exits
        NotifyWait(ch10, 0, 0x1);
        CompilerContext::Current().ConfigureProgram(1, 2, {ch10});
        return Result::SUCCESS;
    };
    RegisterManager::Global().Reset();
    InstanceHandle instance = 1;
    KernelHandle h0 = 0, h1 = 0;
    RegisterManager::Global().RegisterStart(instance);
    RegisterManager::Global().Register(instance, "r0", fn0, nullptr, h0);
    RegisterManager::Global().Register(instance, "r1", fn1, nullptr, h1);
    RegisterManager::Global().RegisterEnd(instance);

    RankMemory mem0 = MakeMemory(64, 64);
    RankMemory mem1 = MakeMemory(64, 64);
    for (uint32_t i = 0; i < 32; ++i) mem0.input[i] = static_cast<uint8_t>(0x80 + i);
    std::vector<RankLaunch> launches = {
        {h0, {mem0.InputAddress(), mem1.OutputAddress(), 32}, &mem0},
        {h1, {}, &mem1},
    };
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    for (uint32_t i = 0; i < 32; ++i) {
        EXPECT_EQ(mem1.output[i], static_cast<uint8_t>(0x80 + i)) << "byte " << i;
    }
}

TEST(CcuStFrameworkL3, TwoRankNotifySync)
{
    ChannelHandle ch01 = MakeChannel(0, 1);
    ChannelHandle ch10 = MakeChannel(1, 0);
    auto fn0 = [ch01](void*) {
        Variable v;
        v = 42U;
        WriteVariableWithNotify(ch01, v, 0, 0, 0x1);
        NotifyRecord(ch01, 1, 0x1);
        NotifyWait(ch01, 2, 0x1);
        CompilerContext::Current().ConfigureProgram(0, 2, {ch01});
        return Result::SUCCESS;
    };
    auto fn1 = [ch10](void*) {
        NotifyWait(ch10, 0, 0x1);
        Variable v = GetResByChannel<Variable>(ch10, 0);
        Variable addr;
        LoadArg(addr, 0);
        Store(addr, v);
        NotifyRecord(ch10, 2, 0x1);
        CompilerContext::Current().ConfigureProgram(1, 2, {ch10});
        return Result::SUCCESS;
    };
    RegisterManager::Global().Reset();
    InstanceHandle instance = 1;
    KernelHandle h0 = 0, h1 = 0;
    RegisterManager::Global().RegisterStart(instance);
    RegisterManager::Global().Register(instance, "r0", fn0, nullptr, h0);
    RegisterManager::Global().Register(instance, "r1", fn1, nullptr, h1);
    RegisterManager::Global().RegisterEnd(instance);

    RankMemory mem0 = MakeMemory(64, 64);
    RankMemory mem1 = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {
        {h0, {}, &mem0},
        {h1, {mem1.OutputAddress()}, &mem1},
    };
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    uint64_t val = 0;
    std::memcpy(&val, mem1.output.data(), sizeof(uint64_t));
    EXPECT_EQ(val, 42U);
}

// ===== L3: 错误检测 =====

TEST(CcuStFrameworkL3, DeadlockDetection)
{
    auto fn = [](void*) {
        ChannelHandle ch = MakeChannel(0, 1);
        NotifyWait(ch, 0, 0x1);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    std::vector<RankLaunch> launches = {{h, {}, &mem}};
    RuntimeWorld world;
    EXPECT_EQ(world.Launch(launches), Result::DEADLOCK);
    EXPECT_FALSE(world.LastError().empty());
}

TEST(CcuStFrameworkL3, ZeroLengthSkips)
{
    auto fn = [](void*) {
        Variable srcAddr, dstAddr, len;
        LoadArg(srcAddr, 0);
        LoadArg(dstAddr, 1);
        len = 0U;
        LocalAddr src, dst;
        src.addr = srcAddr;
        dst.addr = dstAddr;
        Event evt;
        LocalCopy(dst, src, len, evt);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    for (uint32_t i = 0; i < 64; ++i) mem.output[i] = 0xAA;
    std::vector<RankLaunch> launches = {{h, {mem.InputAddress(), mem.OutputAddress()}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    EXPECT_EQ(mem.output[0], 0xAA);
}

TEST(CcuStFrameworkL3, BufferGrows)
{
    auto fn = [](void*) {
        Variable srcAddr, dstAddr, len;
        LoadArg(srcAddr, 0);
        LoadArg(dstAddr, 1);
        LoadArg(len, 2);
        CcuBuffer buf;
        LocalAddr src;
        src.addr = srcAddr;
        Event evt;
        LocalCopy(buf, src, len, evt);
        LocalAddr dst;
        dst.addr = dstAddr;
        LocalCopy(dst, buf, len, evt);
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        return Result::SUCCESS;
    };
    KernelHandle h = BuildProgram("t", fn);
    RankMemory mem = MakeMemory(64, 64);
    for (uint32_t i = 0; i < 16; ++i) mem.input[i] = static_cast<uint8_t>(0x50 + i);
    std::vector<RankLaunch> launches = {{h, {mem.InputAddress(), mem.OutputAddress(), 16}, &mem}};
    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    for (uint32_t i = 0; i < 16; ++i) {
        EXPECT_EQ(mem.output[i], static_cast<uint8_t>(0x50 + i)) << "byte " << i;
    }
}
