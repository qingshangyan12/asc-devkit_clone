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
#include "ccu_runtime_checker.h"

#include <gtest/gtest.h>
#include <cstring>
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

const Program* GetProgram(KernelHandle handle)
{
    return RegisterManager::Global().GetProgram(handle);
}

size_t CountOp(const Program& prog, OpCode code)
{
    size_t n = 0;
    for (const auto& op : prog.operations) {
        if (op.code == code) ++n;
    }
    return n;
}

const Operation* FindOp(const Program& prog, OpCode code)
{
    for (const auto& op : prog.operations) {
        if (op.code == code) return &op;
    }
    return nullptr;
}

struct SelfTestConfig {
    uint32_t rankId{0};
    uint32_t rankSize{2};
    HcclDataType dataType{HCCL_DATA_TYPE_INT32};
    HcclReduceOp opType{HCCL_REDUCE_SUM};
    std::vector<ChannelHandle> channels;
};

} // namespace

// ===== L1: IR 捕获 — 每个原语的 OpCode/字段正确性 =====

// --- 变量操作 ---
TEST(CcuStFrameworkL1, VariableAssignImm)
{
    auto fn = [](void*) {
        Variable v;
        v = 42U;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ASSIGN_IMM), 1U);
    auto* op = FindOp(*prog, OpCode::ASSIGN_IMM);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->immediate, 42U);
}

TEST(CcuStFrameworkL1, VariableAssignVar)
{
    auto fn = [](void*) {
        Variable a;
        Variable b;
        a = b;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ASSIGN_VAR), 1U);
}

TEST(CcuStFrameworkL1, VariableAdd)
{
    auto fn = [](void*) {
        Variable a;
        Variable b;
        Variable c;
        c = a + b;
        (void)c;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ADD_VAR), 1U);
    auto* op = FindOp(*prog, OpCode::ADD_VAR);
    ASSERT_NE(op, nullptr);
    EXPECT_NE(op->dst, 0U);
    EXPECT_NE(op->dst, op->src0);
    EXPECT_NE(op->dst, op->src1);
}

TEST(CcuStFrameworkL1, VariableAddAssign)
{
    auto fn = [](void*) {
        Variable a;
        Variable b;
        a += b;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::ADD_VAR);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->dst, op->src0);
}

TEST(CcuStFrameworkL1, VariableEqualCondition)
{
    auto fn = [](void*) {
        Variable a;
        CCU_IF(a == 5U) {}
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::IF_BEGIN);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->immediate, 5U);
    EXPECT_TRUE(op->equal);
}

TEST(CcuStFrameworkL1, VariableNotEqualCondition)
{
    auto fn = [](void*) {
        Variable a;
        CCU_IF(a != 3U) {}
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::IF_BEGIN);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->immediate, 3U);
    EXPECT_FALSE(op->equal);
}

// --- 参数加载 ---
TEST(CcuStFrameworkL1, LoadArg)
{
    auto fn = [](void*) {
        Variable v;
        LoadArg(v, 0);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::LOAD_ARG);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->index, 0U);
    ASSERT_EQ(prog->arguments.size(), 1U);
    EXPECT_EQ(prog->arguments[0].id, 0U);
}

TEST(CcuStFrameworkL1, LoadArgMultiple)
{
    auto fn = [](void*) {
        Variable a, b, c;
        LoadArg(a, 0);
        LoadArg(b, 1);
        LoadArg(c, 2);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::LOAD_ARG), 3U);
    EXPECT_EQ(prog->arguments.size(), 3U);
    EXPECT_EQ(prog->arguments[0].id, 0U);
    EXPECT_EQ(prog->arguments[1].id, 1U);
    EXPECT_EQ(prog->arguments[2].id, 2U);
}

// --- 地址操作 ---
TEST(CcuStFrameworkL1, AddressAssignVar)
{
    auto fn = [](void*) {
        Variable v;
        Address a;
        a = v;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ADDRESS_ASSIGN_VAR), 1U);
}

TEST(CcuStFrameworkL1, AddressAssignAddress)
{
    auto fn = [](void*) {
        Address a;
        Address b;
        a = b;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ADDRESS_ASSIGN_ADDRESS), 1U);
}

TEST(CcuStFrameworkL1, AddressAddVar)
{
    auto fn = [](void*) {
        Variable v;
        Address a;
        a += v;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::ADDRESS_ADD_VAR), 1U);
}

// --- 内存操作 ---
TEST(CcuStFrameworkL1, LoadMemory)
{
    auto fn = [](void*) {
        Variable addr;
        Variable dst;
        Load(addr, dst);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::LOAD_MEMORY);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->byteOffset, 0U);
}

TEST(CcuStFrameworkL1, LoadMemoryArray)
{
    auto fn = [](void*) {
        Variable addr;
        Array<Variable> dst(3);
        Load(addr, dst, 3);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::LOAD_MEMORY), 3U);
    const auto& ops = prog->operations;
    EXPECT_EQ(ops[0].byteOffset, 0U);
    EXPECT_EQ(ops[1].byteOffset, sizeof(uint64_t));
    EXPECT_EQ(ops[2].byteOffset, 2 * sizeof(uint64_t));
}

TEST(CcuStFrameworkL1, StoreMemory)
{
    auto fn = [](void*) {
        Variable addr;
        Variable src;
        Store(addr, src);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::STORE_MEMORY), 1U);
}

// --- Write/Read/LocalCopy ---
TEST(CcuStFrameworkL1, Write)
{
    auto fn = [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        Write(ch, remote, local, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::WRITE);
    ASSERT_NE(op, nullptr);
    EXPECT_NE(op->channel, 0U);
    EXPECT_NE(op->src0, 0U);
    EXPECT_NE(op->dst, 0U);
    EXPECT_NE(op->src1, 0U);
    EXPECT_NE(op->immediate, 0U);
    EXPECT_NE(op->mask, 0U);
}

TEST(CcuStFrameworkL1, Read)
{
    auto fn = [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        Read(ch, local, remote, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::READ), 1U);
    EXPECT_EQ(CountOp(*prog, OpCode::WRITE), 0U);
    auto* op = FindOp(*prog, OpCode::READ);
    ASSERT_NE(op, nullptr);
    EXPECT_NE(op->channel, 0U);
}

TEST(CcuStFrameworkL1, LocalCopy)
{
    auto fn = [](void*) {
        LocalAddr dst;
        LocalAddr src;
        Variable len;
        Event evt;
        LocalCopy(dst, src, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::LOCAL_COPY), 1U);
}

TEST(CcuStFrameworkL1, LocalCopyBuffer)
{
    auto fn = [](void*) {
        LocalAddr src;
        CcuBuffer buf;
        Variable len;
        Event evt;
        LocalCopy(buf, src, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::LOCAL_COPY), 1U);
}

// --- Reduce 原语 ---
TEST(CcuStFrameworkL1, LocalReduce)
{
    auto fn = [](void*) {
        LocalAddr dst;
        LocalAddr src;
        Variable len;
        Event evt;
        LocalReduce(dst, src, len, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::LOCAL_REDUCE);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->dataType, HCCL_DATA_TYPE_FP16);
    EXPECT_EQ(op->opType, HCCL_REDUCE_SUM);
}

TEST(CcuStFrameworkL1, WriteReduce)
{
    auto fn = [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        WriteReduce(ch, remote, local, len, HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MAX, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::WRITE_REDUCE);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->dataType, HCCL_DATA_TYPE_INT32);
    EXPECT_EQ(op->opType, HCCL_REDUCE_MAX);
    EXPECT_NE(op->channel, 0U);
}

TEST(CcuStFrameworkL1, ReadReduce)
{
    auto fn = [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        ReadReduce(ch, local, remote, len, HCCL_DATA_TYPE_INT8, HCCL_REDUCE_PROD, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::READ_REDUCE);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->dataType, HCCL_DATA_TYPE_INT8);
    EXPECT_EQ(op->opType, HCCL_REDUCE_PROD);
}

TEST(CcuStFrameworkL1, LocalReduceBufferArray)
{
    auto fn = [](void*) {
        CcuBuffer bufs[4] = {CcuBuffer(), CcuBuffer(), CcuBuffer(), CcuBuffer()};
        Variable len;
        Event evt;
        LocalReduce(bufs, 4, HCCL_DATA_TYPE_FP32, HCCL_DATA_TYPE_FP32, HCCL_REDUCE_SUM, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::LOCAL_REDUCE), 3U) << "4 buffers → 3 LOCAL_REDUCE (reduce buf[1..3] into buf[0])";
}

// --- Notify/Event ---
TEST(CcuStFrameworkL1, NotifyWait)
{
    auto fn = [](void*) {
        ChannelHandle ch = MakeChannel(0, 1);
        NotifyWait(ch, 5, 0x3);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::NOTIFY_WAIT);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->index, 5U);
    EXPECT_EQ(op->mask, 0x3U);
}

TEST(CcuStFrameworkL1, NotifyRecord)
{
    auto fn = [](void*) {
        ChannelHandle ch = MakeChannel(0, 1);
        NotifyRecord(ch, 7, 0x1);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::NOTIFY_RECORD);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->index, 7U);
    EXPECT_EQ(op->mask, 0x1U);
}

TEST(CcuStFrameworkL1, EventRecordWait)
{
    auto fn = [](void*) {
        Event evt;
        EventRecord(evt, 0x2);
        EventWait(evt, 0x2);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::EVENT_RECORD), 1U);
    EXPECT_EQ(CountOp(*prog, OpCode::EVENT_WAIT), 1U);
    auto* rec = FindOp(*prog, OpCode::EVENT_RECORD);
    ASSERT_NE(rec, nullptr);
    EXPECT_EQ(rec->mask, 0x2U);
}

TEST(CcuStFrameworkL1, WriteVariableNotify)
{
    auto fn = [](void*) {
        Variable v;
        ChannelHandle ch = MakeChannel(0, 1);
        WriteVariableWithNotify(ch, v, 3, 5, 0x1);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    auto* op = FindOp(*prog, OpCode::WRITE_VARIABLE_NOTIFY);
    ASSERT_NE(op, nullptr);
    EXPECT_EQ(op->index, 3U);
    EXPECT_EQ(op->immediate, 5U);
    EXPECT_EQ(op->mask, 0x1U);
}

// --- 控制流 ---
TEST(CcuStFrameworkL1, CcuIf)
{
    auto fn = [](void*) {
        Variable v;
        CCU_IF(v == 1U) {
            v = 10U;
        }
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::IF_BEGIN), 1U);
    EXPECT_EQ(CountOp(*prog, OpCode::IF_END), 1U);
    auto* begin = FindOp(*prog, OpCode::IF_BEGIN);
    auto* end = FindOp(*prog, OpCode::IF_END);
    ASSERT_NE(begin, nullptr);
    ASSERT_NE(end, nullptr);
    size_t beginIdx = std::distance(prog->operations.data(), begin);
    size_t endIdx = std::distance(prog->operations.data(), end);
    EXPECT_EQ(begin->peerIndex, endIdx);
    EXPECT_EQ(end->peerIndex, beginIdx);
}

TEST(CcuStFrameworkL1, CcuWhile)
{
    auto fn = [](void*) {
        Variable v;
        CCU_WHILE(v != 0U) {
            v = 1U;
        }
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::WHILE_BEGIN), 1U);
    EXPECT_EQ(CountOp(*prog, OpCode::WHILE_END), 1U);
    auto* begin = FindOp(*prog, OpCode::WHILE_BEGIN);
    auto* end = FindOp(*prog, OpCode::WHILE_END);
    ASSERT_NE(begin, nullptr);
    ASSERT_NE(end, nullptr);
    size_t beginIdx = std::distance(prog->operations.data(), begin);
    size_t endIdx = std::distance(prog->operations.data(), end);
    EXPECT_EQ(begin->peerIndex, endIdx);
    EXPECT_EQ(end->peerIndex, beginIdx);
}

TEST(CcuStFrameworkL1, CcuIfElse)
{
    auto fn = [](void*) {
        Variable v;
        CCU_IF(v == 1U) {
            v = 10U;
        }
        CCU_ELSE
        {
            v = 20U;
        }
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(CountOp(*prog, OpCode::IF_BEGIN), 1U);
    EXPECT_EQ(CountOp(*prog, OpCode::IF_END), 2U);
    EXPECT_EQ(CountOp(*prog, OpCode::ELSE_BEGIN), 1U);
}

// --- GetResByChannel ---
TEST(CcuStFrameworkL1, GetResByChannel)
{
    auto fn = [](void*) {
        ChannelHandle ch = MakeChannel(0, 1);
        Variable v = GetResByChannel<Variable>(ch, 0);
        (void)v;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_FALSE(prog->channelVariables.empty());
    auto key = std::make_pair(1U, 0U);
    EXPECT_EQ(prog->channelVariables.count(key), 1U);
}

// ===== L1: Finalize 验证 =====

TEST(CcuStFrameworkL1, Finalize_BalancedIf)
{
    auto fn = [](void*) {
        Variable v;
        CCU_IF(v == 1U) {}
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
}

TEST(CcuStFrameworkL1, Finalize_BalancedWhile)
{
    auto fn = [](void*) {
        Variable v;
        CCU_WHILE(v != 0U) {}
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
}

TEST(CcuStFrameworkL1, Finalize_UnmatchedIf)
{
    RegisterManager::Global().Reset();
    Program program;
    {
        CompilerContext compiler(program);
        CompilerContext::SetCurrent(&compiler);
        Operation op;
        op.code = OpCode::IF_BEGIN;
        op.src0 = compiler.AllocateVariable();
        op.immediate = 1;
        op.equal = true;
        compiler.Append(op);
        CompilerContext::SetCurrent(nullptr);
    }
    Result result = CompilerContext(program).Finalize();
    EXPECT_NE(result, Result::SUCCESS);
}

TEST(CcuStFrameworkL1, Finalize_UnmatchedWhile)
{
    RegisterManager::Global().Reset();
    Program program;
    {
        CompilerContext compiler(program);
        CompilerContext::SetCurrent(&compiler);
        Operation op;
        op.code = OpCode::WHILE_BEGIN;
        op.src0 = compiler.AllocateVariable();
        op.immediate = 1;
        op.equal = true;
        compiler.Append(op);
        CompilerContext::SetCurrent(nullptr);
    }
    Result result = CompilerContext(program).Finalize();
    EXPECT_NE(result, Result::SUCCESS);
}

TEST(CcuStFrameworkL1, Finalize_ElseWithoutIf)
{
    RegisterManager::Global().Reset();
    Program program;
    {
        CompilerContext compiler(program);
        CompilerContext::SetCurrent(&compiler);
        Operation op;
        op.code = OpCode::ELSE_BEGIN;
        compiler.Append(op);
        CompilerContext::SetCurrent(nullptr);
    }
    Result result = CompilerContext(program).Finalize();
    EXPECT_NE(result, Result::SUCCESS);
}

TEST(CcuStFrameworkL1, Finalize_ArgIdsContiguous)
{
    auto fn = [](void*) {
        Variable a, b, c;
        LoadArg(a, 0);
        LoadArg(b, 1);
        LoadArg(c, 2);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_EQ(prog->arguments.size(), 3U);
    EXPECT_EQ(prog->arguments[0].id, 0U);
    EXPECT_EQ(prog->arguments[1].id, 1U);
    EXPECT_EQ(prog->arguments[2].id, 2U);
}

// ===== L1 补充: hcomm 对齐验证 =====

TEST(CcuStFrameworkL1, LocalReduceBufferArray_ReduceIntoFirst)
{
    // hcomm semantics: CcuLocalBufferReduce reduces all buffers into buffers[0]
    // Our stub should generate N-1 LOCAL_REDUCE ops: dst=buf[0], src0=buf[i]
    auto fn = [](void*) {
        CcuBuffer bufs[4] = {CcuBuffer(), CcuBuffer(), CcuBuffer(), CcuBuffer()};
        Variable len;
        Event evt;
        LocalReduce(bufs, 4, HCCL_DATA_TYPE_INT32, HCCL_DATA_TYPE_INT32, HCCL_REDUCE_SUM, len, evt);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    // Should generate 3 LOCAL_REDUCE (count-1), not 4
    EXPECT_EQ(CountOp(*prog, OpCode::LOCAL_REDUCE), 3U);
    // All should have dst = bufs[0] handle (first buffer allocated)
    ResourceHandle firstBuf = *prog->buffers.begin();
    for (const auto& op : prog->operations) {
        if (op.code == OpCode::LOCAL_REDUCE) {
            EXPECT_EQ(op.dst, firstBuf);
            EXPECT_NE(op.src0, firstBuf); // src should be buf[1], buf[2], buf[3]
        }
    }
}

TEST(CcuStFrameworkL1, AddressPlusAddress)
{
    // hcomm: addr + addr returns CcuArithmeticOperator<Address,Address>
    // Our stub: addr + addr returns AddressExpr
    auto fn = [](void*) {
        Address a;
        Address b;
        Address c;
        c = a + b;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    // Should generate ADDRESS_ADD_VAR or ADDRESS_ASSIGN_ADDRESS
    // The key: it should NOT be a move (no IR) — must generate an operation
    EXPECT_GE(prog->operations.size(), 1u);
}

TEST(CcuStFrameworkL1, AddressPlusVariable)
{
    auto fn = [](void*) {
        Address a;
        Variable v;
        Address c;
        c = a + v;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_GE(prog->operations.size(), 1u);
}

TEST(CcuStFrameworkL1, VariablePlusAddress)
{
    auto fn = [](void*) {
        Address a;
        Variable v;
        Address c;
        c = v + a;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_GE(prog->operations.size(), 1u);
}

TEST(CcuStFrameworkL1, AddressPlusEqualAddress)
{
    auto fn = [](void*) {
        Address a;
        Address b;
        a += b;
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_GE(prog->operations.size(), 1u);
}

TEST(CcuStFrameworkL1, LoadUint64Addr)
{
    auto fn = [](void*) {
        Variable v;
        Load(static_cast<uint64_t>(0x1000), v);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    // Should generate ASSIGN_IMM (for addr var) + LOAD_MEMORY
    EXPECT_GE(CountOp(*prog, OpCode::LOAD_MEMORY), 1u);
}

TEST(CcuStFrameworkL1, StoreUint64Addr)
{
    auto fn = [](void*) {
        Variable v;
        Store(static_cast<uint64_t>(0x2000), v);
        return Result::SUCCESS;
    };
    auto* prog = GetProgram(BuildProgram("test", fn));
    ASSERT_NE(prog, nullptr);
    EXPECT_GE(CountOp(*prog, OpCode::STORE_MEMORY), 1u);
}
