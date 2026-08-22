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

#include <gtest/gtest.h>
#include <cstring>

using namespace HcclSim::CcuSt;
using AscendC::ccu::Variable;
using AscendC::ccu::LocalAddr;
using AscendC::ccu::RemoteAddr;
using AscendC::ccu::CcuBuffer;
using AscendC::ccu::Event;

namespace {

ChannelHandle MakeTestChannel(uint32_t local, uint32_t remote)
{
    return (static_cast<uint64_t>(local) << 32U) | remote;
}

void RegisterTestChannel(uint32_t local, uint32_t remote)
{
    RegisterManager::Global().RegisterChannelMap(MakeTestChannel(local, remote), local, remote);
}

KernelHandle RegisterSingleKernel(const char* name, RegisterManager::KernelFunction func)
{
    static InstanceHandle nextInst = 200;
    InstanceHandle inst = nextInst++;
    RegisterManager::Global().RegisterStart(inst);
    KernelHandle handle = 0;
    RegisterManager::Global().Register(inst, name, func, nullptr, handle);
    RegisterManager::Global().RegisterEnd(inst);
    return handle;
}

void FillInput(RankMemory& mem, uint64_t offset, uint64_t size, uint8_t base)
{
    mem.input.assign(offset + size, 0x5a);
    mem.output.assign(offset + size, 0xa5);
    for (uint64_t i = 0; i < size; ++i) {
        mem.input[offset + i] = static_cast<uint8_t>(base + i);
    }
}

void VerifyOutput(const RankMemory& mem, uint64_t offset, uint64_t size, uint8_t base)
{
    for (uint64_t i = 0; i < size; ++i) {
        EXPECT_EQ(mem.output[offset + i], static_cast<uint8_t>(base + i)) << "byte " << i;
    }
}

} // namespace

// ============================================================================
// 1. Variable 运算：赋值/加法/比较
// ============================================================================

TEST(CcuPrimitiveTest, Variable_AssignImm)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("VarImm", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 0);
        Variable v;
        v = uint64_t(42);
        AscendC::ccu::Store(outAddr, v);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);
    const Program* prog = RegisterManager::Global().GetProgram(handle);
    ASSERT_NE(prog, nullptr);
    ASSERT_EQ(prog->arguments.size(), 1U) << "expected 1 arg, got " << prog->arguments.size();

    RankMemory mem;
    mem.output.assign(8, 0);
    std::vector<RankLaunch> launches;
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());
    launches.push_back(RankLaunch{handle, {outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 42U);
}

TEST(CcuPrimitiveTest, Variable_AssignVar_And_Add)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("VarAdd", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable a;
        AscendC::ccu::LoadArg(a, 0);
        Variable b;
        AscendC::ccu::LoadArg(b, 1);
        Variable sum;
        sum = a;
        sum += b;
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 2);
        AscendC::ccu::Store(outAddr, sum);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    RankMemory mem;
    mem.output.assign(8, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {10, 20, outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 30U);
}

// ============================================================================
// 2. Address 运算：赋值/加法
// ============================================================================

TEST(CcuPrimitiveTest, Address_AssignAndAdd)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("AddrTest", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable srcAddrVar;
        AscendC::ccu::LoadArg(srcAddrVar, 0);
        Variable dstAddrVar;
        AscendC::ccu::LoadArg(dstAddrVar, 1);
        Variable lenVar;
        AscendC::ccu::LoadArg(lenVar, 2);
        AscendC::ccu::Address srcAddr;
        srcAddr = srcAddrVar;
        AscendC::ccu::Address dstAddr;
        dstAddr = dstAddrVar;
        LocalAddr src;
        src.addr = srcAddr;
        LocalAddr dst;
        dst.addr = dstAddr;
        Event evt;
        AscendC::ccu::LocalCopy(dst, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    const uint64_t dataSize = 64;
    RankMemory mem;
    FillInput(mem, 0, dataSize, 100);
    mem.output.assign(dataSize, 0xa5);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem.input.data());
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {srcAddr, dstAddr, dataSize}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    for (uint64_t i = 0; i < dataSize; ++i) {
        EXPECT_EQ(mem.output[i], mem.input[i]) << "byte " << i;
    }
}

// ============================================================================
// 3. LoadArg — 参数加载
// ============================================================================

TEST(CcuPrimitiveTest, LoadArg_MultipleArgs)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("LoadArgTest", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable a, b, c;
        AscendC::ccu::LoadArg(a, 0);
        AscendC::ccu::LoadArg(b, 1);
        AscendC::ccu::LoadArg(c, 2);
        Variable sum;
        sum = a;
        sum += b;
        sum += c;
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 3);
        AscendC::ccu::Store(outAddr, sum);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    RankMemory mem;
    mem.output.assign(8, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {1, 2, 3, outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 6U);
}

// ============================================================================
// 4. Load/Store — 内存读写
// ============================================================================

TEST(CcuPrimitiveTest, LoadStore_RoundTrip)
{
    RegisterManager::Global().Reset();
    const uint64_t testValue = 0x1234567890ABCDEF;

    RankMemory mem;
    mem.parameter.assign(8, 0);
    std::memcpy(mem.parameter.data(), &testValue, 8);
    mem.output.assign(16, 0);

    uint64_t storeAddr = reinterpret_cast<uint64_t>(mem.output.data());
    uint64_t loadAddr = reinterpret_cast<uint64_t>(mem.parameter.data());

    KernelHandle handle = RegisterSingleKernel("LoadStore", [storeAddr, loadAddr](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable storeVal;
        AscendC::ccu::LoadArg(storeVal, 0);
        Variable storeAddrVar;
        storeAddrVar = storeAddr;
        AscendC::ccu::Store(storeAddrVar, storeVal);
        Variable loadAddrVar;
        loadAddrVar = loadAddr;
        Variable loadedVal;
        AscendC::ccu::Load(loadAddrVar, loadedVal);
        Variable outAddrVar;
        outAddrVar = storeAddr + 8;
        AscendC::ccu::Store(outAddrVar, loadedVal);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {testValue}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t storedVal = 0;
    std::memcpy(&storedVal, mem.output.data(), 8);
    EXPECT_EQ(storedVal, testValue);

    uint64_t loadedVal = 0;
    std::memcpy(&loadedVal, mem.output.data() + 8, 8);
    EXPECT_EQ(loadedVal, testValue);
}

// ============================================================================
// 5. WriteVariableWithNotify + NotifyWait — 跨 rank 变量同步
// ============================================================================

TEST(CcuPrimitiveTest, WriteVariableWithNotify_2Rank)
{
    RegisterManager::Global().Reset();
    RegisterTestChannel(0, 1);
    RegisterTestChannel(1, 0);

    KernelHandle h0 = RegisterSingleKernel("WVN0", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 2, {MakeTestChannel(0, 1)});
        Variable val;
        AscendC::ccu::LoadArg(val, 0);
        AscendC::ccu::WriteVariableWithNotify(MakeTestChannel(0, 1), val, 0, 0, 1);
        AscendC::ccu::NotifyWait(MakeTestChannel(0, 1), 0, 1);
        return Result::SUCCESS;
    });

    KernelHandle h1 = RegisterSingleKernel("WVN1", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(1, 2, {MakeTestChannel(1, 0)});
        Variable remoteVar = AscendC::ccu::GetResByChannel<Variable>(MakeTestChannel(1, 0), 0);
        AscendC::ccu::NotifyWait(MakeTestChannel(1, 0), 0, 1);
        AscendC::ccu::NotifyRecord(MakeTestChannel(1, 0), 0, 1);
        return Result::SUCCESS;
    });

    ASSERT_NE(h0, 0U);
    ASSERT_NE(h1, 0U);

    RankMemory mem0, mem1;
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{h0, {42}, &mem0});
    launches.push_back(RankLaunch{h1, {}, &mem1});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

// ============================================================================
// 6. NotifyRecord + NotifyWait — 跨 rank notify 同步
// ============================================================================

TEST(CcuPrimitiveTest, NotifyRecordWait_2Rank)
{
    RegisterManager::Global().Reset();
    RegisterTestChannel(0, 1);
    RegisterTestChannel(1, 0);

    KernelHandle h0 = RegisterSingleKernel("NR0", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 2, {MakeTestChannel(0, 1)});
        AscendC::ccu::NotifyRecord(MakeTestChannel(0, 1), 0, 1);
        AscendC::ccu::NotifyWait(MakeTestChannel(0, 1), 0, 1);
        return Result::SUCCESS;
    });

    KernelHandle h1 = RegisterSingleKernel("NR1", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(1, 2, {MakeTestChannel(1, 0)});
        AscendC::ccu::NotifyWait(MakeTestChannel(1, 0), 0, 1);
        AscendC::ccu::NotifyRecord(MakeTestChannel(1, 0), 0, 1);
        return Result::SUCCESS;
    });

    ASSERT_NE(h0, 0U);
    ASSERT_NE(h1, 0U);

    RankMemory mem0, mem1;
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{h0, {}, &mem0});
    launches.push_back(RankLaunch{h1, {}, &mem1});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
}

// ============================================================================
// 7. EventRecord + EventWait — 本地事件同步
// ============================================================================

TEST(CcuPrimitiveTest, EventRecordWait_SingleRank)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("EvtTest", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable srcAddrVar, dstAddrVar, lenVar;
        AscendC::ccu::LoadArg(srcAddrVar, 0);
        AscendC::ccu::LoadArg(dstAddrVar, 1);
        AscendC::ccu::LoadArg(lenVar, 2);
        LocalAddr src; src.addr = srcAddrVar;
        LocalAddr dst; dst.addr = dstAddrVar;
        Event evt;
        AscendC::ccu::LocalCopy(dst, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    const uint64_t dataSize = 128;
    RankMemory mem;
    FillInput(mem, 0, dataSize, 50);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem.input.data());
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {srcAddr, dstAddr, dataSize}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    VerifyOutput(mem, 0, dataSize, 50);
}

// ============================================================================
// 8. Write — 跨 rank 数据搬运
// ============================================================================

TEST(CcuPrimitiveTest, Write_CrossRank_256B)
{
    RegisterManager::Global().Reset();
    RegisterTestChannel(0, 1);
    RegisterTestChannel(1, 0);

    const uint64_t dataSize = 256;
    constexpr uint64_t baseOffset = 7;

    RankMemory mem0, mem1;
    FillInput(mem0, baseOffset, dataSize, 10);
    mem1.output.assign(baseOffset + dataSize, 0xa5);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem0.input.data()) + baseOffset;
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem1.output.data()) + baseOffset;

    KernelHandle h0 = RegisterSingleKernel("Write0", [srcAddr](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 2, {MakeTestChannel(0, 1)});
        Variable dstVar, lenVar;
        AscendC::ccu::LoadArg(dstVar, 0);
        AscendC::ccu::LoadArg(lenVar, 1);
        Variable srcVar;
        srcVar = srcAddr;
        LocalAddr src; src.addr = srcVar;
        RemoteAddr dst; dst.addr = dstVar;
        Event evt;
        AscendC::ccu::Write(MakeTestChannel(0, 1), dst, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        AscendC::ccu::NotifyRecord(MakeTestChannel(0, 1), 0, 1);
        return Result::SUCCESS;
    });

    KernelHandle h1 = RegisterSingleKernel("Write1", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(1, 2, {MakeTestChannel(1, 0)});
        AscendC::ccu::NotifyWait(MakeTestChannel(1, 0), 0, 1);
        return Result::SUCCESS;
    });

    ASSERT_NE(h0, 0U);
    ASSERT_NE(h1, 0U);

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{h0, {dstAddr, dataSize}, &mem0});
    launches.push_back(RankLaunch{h1, {}, &mem1});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    for (uint64_t i = 0; i < dataSize; ++i) {
        EXPECT_EQ(mem1.output[baseOffset + i], mem0.input[baseOffset + i]) << "byte " << i;
    }
}

// ============================================================================
// 9. LocalCopy (Mem→Mem) — 本地拷贝
// ============================================================================

TEST(CcuPrimitiveTest, LocalCopy_MemToMem_256B)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("LCopy", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable srcVar, dstVar, lenVar;
        AscendC::ccu::LoadArg(srcVar, 0);
        AscendC::ccu::LoadArg(dstVar, 1);
        AscendC::ccu::LoadArg(lenVar, 2);
        LocalAddr src; src.addr = srcVar;
        LocalAddr dst; dst.addr = dstVar;
        Event evt;
        AscendC::ccu::LocalCopy(dst, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    const uint64_t dataSize = 256;
    RankMemory mem;
    FillInput(mem, 0, dataSize, 200);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem.input.data());
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {srcAddr, dstAddr, dataSize}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    VerifyOutput(mem, 0, dataSize, 200);
}

// ============================================================================
// 10. LocalCopy (Buffer↔Mem) — buffer 中转
// ============================================================================

TEST(CcuPrimitiveTest, LocalCopy_BufferRelay_256B)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("BufRelay", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable srcVar, dstVar, lenVar;
        AscendC::ccu::LoadArg(srcVar, 0);
        AscendC::ccu::LoadArg(dstVar, 1);
        AscendC::ccu::LoadArg(lenVar, 2);
        LocalAddr src; src.addr = srcVar;
        LocalAddr dst; dst.addr = dstVar;
        CcuBuffer buf;
        Event evt;
        AscendC::ccu::LocalCopy(buf, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        AscendC::ccu::LocalCopy(dst, buf, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    const uint64_t dataSize = 256;
    RankMemory mem;
    FillInput(mem, 0, dataSize, 150);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem.input.data());
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem.output.data());
    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {srcAddr, dstAddr, dataSize}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();
    VerifyOutput(mem, 0, dataSize, 150);
}

// ============================================================================
// 11. Write with CcuBuffer — 从 buffer 写到远端
// ============================================================================

TEST(CcuPrimitiveTest, Write_FromBuffer_2Rank_256B)
{
    RegisterManager::Global().Reset();
    RegisterTestChannel(0, 1);
    RegisterTestChannel(1, 0);

    const uint64_t dataSize = 256;
    constexpr uint64_t baseOffset = 7;

    RankMemory mem0, mem1;
    FillInput(mem0, baseOffset, dataSize, 30);
    mem1.output.assign(baseOffset + dataSize, 0xa5);

    uint64_t srcAddr = reinterpret_cast<uint64_t>(mem0.input.data()) + baseOffset;
    uint64_t dstAddr = reinterpret_cast<uint64_t>(mem1.output.data()) + baseOffset;

    KernelHandle h0 = RegisterSingleKernel("WriteBuf0", [srcAddr](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 2, {MakeTestChannel(0, 1)});
        Variable dstVar, lenVar;
        AscendC::ccu::LoadArg(dstVar, 0);
        AscendC::ccu::LoadArg(lenVar, 1);
        Variable srcVar;
        srcVar = srcAddr;
        LocalAddr src; src.addr = srcVar;
        CcuBuffer buf;
        Event evt;
        AscendC::ccu::LocalCopy(buf, src, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        RemoteAddr dst; dst.addr = dstVar;
        AscendC::ccu::Write(MakeTestChannel(0, 1), dst, buf, lenVar, evt, 1);
        AscendC::ccu::EventWait(evt, 1);
        AscendC::ccu::NotifyRecord(MakeTestChannel(0, 1), 0, 1);
        return Result::SUCCESS;
    });

    KernelHandle h1 = RegisterSingleKernel("WriteBuf1", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(1, 2, {MakeTestChannel(1, 0)});
        AscendC::ccu::NotifyWait(MakeTestChannel(1, 0), 0, 1);
        return Result::SUCCESS;
    });

    ASSERT_NE(h0, 0U);
    ASSERT_NE(h1, 0U);

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{h0, {dstAddr, dataSize}, &mem0});
    launches.push_back(RankLaunch{h1, {}, &mem1});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    for (uint64_t i = 0; i < dataSize; ++i) {
        EXPECT_EQ(mem1.output[baseOffset + i], mem0.input[baseOffset + i]) << "byte " << i;
    }
}

// ============================================================================
// 12. CCU_IF / CCU_ELSE — 控制流
// ============================================================================

TEST(CcuPrimitiveTest, IfElse_BranchSelect)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("IfElse", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable cond;
        AscendC::ccu::LoadArg(cond, 0);
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 1);
        Variable val;
        CCU_IF(cond != 0)
        {
            val = uint64_t(100);
        }
        CCU_ELSE
        {
            val = uint64_t(200);
        }
        AscendC::ccu::Store(outAddr, val);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    RankMemory mem;
    mem.output.assign(16, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {1, outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 100U);
}

TEST(CcuPrimitiveTest, IfElse_ElseBranch)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("IfElse2", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable cond;
        AscendC::ccu::LoadArg(cond, 0);
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 1);
        Variable val;
        CCU_IF(cond != 0)
        {
            val = uint64_t(100);
        }
        CCU_ELSE
        {
            val = uint64_t(200);
        }
        AscendC::ccu::Store(outAddr, val);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    RankMemory mem;
    mem.output.assign(16, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {0, outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 200U);
}

// ============================================================================
// 13. CCU_WHILE — 循环控制流
// ============================================================================

TEST(CcuPrimitiveTest, While_Loop_Sum)
{
    RegisterManager::Global().Reset();
    KernelHandle handle = RegisterSingleKernel("While", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 1, {});
        Variable loopCount;
        AscendC::ccu::LoadArg(loopCount, 0);
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 1);
        Variable sum;
        sum = uint64_t(0);
        Variable one;
        one = uint64_t(1);
        CCU_WHILE(loopCount != UINT64_MAX)
        {
            sum += one;
            loopCount += one;
        }
        AscendC::ccu::Store(outAddr, sum);
        return Result::SUCCESS;
    });
    ASSERT_NE(handle, 0U);

    RankMemory mem;
    mem.output.assign(16, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem.output.data());

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{handle, {UINT64_MAX - 5, outAddr}, &mem});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem.output.data(), 8);
    EXPECT_EQ(val, 5U);
}

// ============================================================================
// 14. GetResByChannel — channel 变量获取
// ============================================================================

TEST(CcuPrimitiveTest, GetResByChannel_2Rank)
{
    RegisterManager::Global().Reset();
    RegisterTestChannel(0, 1);
    RegisterTestChannel(1, 0);

    KernelHandle h0 = RegisterSingleKernel("Chan0", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(0, 2, {MakeTestChannel(0, 1)});
        Variable val;
        AscendC::ccu::LoadArg(val, 0);
        AscendC::ccu::WriteVariableWithNotify(MakeTestChannel(0, 1), val, 0, 0, 1);
        AscendC::ccu::NotifyWait(MakeTestChannel(0, 1), 0, 1);
        return Result::SUCCESS;
    });

    KernelHandle h1 = RegisterSingleKernel("Chan1", [](void*) -> Result {
        CompilerContext::Current().ConfigureProgram(1, 2, {MakeTestChannel(1, 0)});
        Variable remoteVar = AscendC::ccu::GetResByChannel<Variable>(MakeTestChannel(1, 0), 0);
        AscendC::ccu::NotifyWait(MakeTestChannel(1, 0), 0, 1);
        Variable outAddr;
        AscendC::ccu::LoadArg(outAddr, 0);
        AscendC::ccu::Store(outAddr, remoteVar);
        AscendC::ccu::NotifyRecord(MakeTestChannel(1, 0), 0, 1);
        return Result::SUCCESS;
    });

    ASSERT_NE(h0, 0U);
    ASSERT_NE(h1, 0U);

    RankMemory mem0, mem1;
    mem1.output.assign(8, 0);
    uint64_t outAddr = reinterpret_cast<uint64_t>(mem1.output.data());

    std::vector<RankLaunch> launches;
    launches.push_back(RankLaunch{h0, {77}, &mem0});
    launches.push_back(RankLaunch{h1, {outAddr}, &mem1});

    RuntimeWorld world;
    ASSERT_EQ(world.Launch(launches), Result::SUCCESS) << world.LastError();

    uint64_t val = 0;
    std::memcpy(&val, mem1.output.data(), 8);
    EXPECT_EQ(val, 77U);
}
