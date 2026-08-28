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
#include "ccu_checker.h"

#include <gtest/gtest.h>

using namespace HcclSim::CcuSt;
using namespace AscendC::ccu;

namespace {

// Build a program using CCU primitives, then return it for checker testing
Program BuildAndFinalize(const std::function<void()>& body)
{
    RegisterManager::Global().Reset();
    Program program;
    {
        CompilerContext compiler(program);
        CompilerContext::SetCurrent(&compiler);
        body();
        CompilerContext::SetCurrent(nullptr);
    }
    CompilerContext(program).Finalize();
    return program;
}

// Build a valid program by calling CCU primitives through RegisterManager
const Program* BuildValidProgram(const std::string& name, const RegisterManager::KernelFunction& fn)
{
    RegisterManager::Global().Reset();
    InstanceHandle instance = 1;
    KernelHandle handle = 0;
    RegisterManager::Global().RegisterStart(instance);
    RegisterManager::Global().Register(instance, name, fn, nullptr, handle);
    RegisterManager::Global().RegisterEnd(instance);
    return RegisterManager::Global().GetProgram(handle);
}

} // namespace

// ===== L2: 静态检查器 — 合法 IR 通过 =====

TEST(CcuStFrameworkL2, ValidWrite)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        Write(ch, remote, local, len, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidRead)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        Read(ch, local, remote, len, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidLocalCopy)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr dst;
        LocalAddr src;
        Variable len;
        Event evt;
        LocalCopy(dst, src, len, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidLocalReduce)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr dst;
        LocalAddr src;
        Variable len;
        Event evt;
        LocalReduce(dst, src, len, HCCL_DATA_TYPE_FP16, HCCL_REDUCE_SUM, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidWriteReduce)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        WriteReduce(ch, remote, local, len, HCCL_DATA_TYPE_INT32, HCCL_REDUCE_MAX, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidReadReduce)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr local;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        ReadReduce(ch, local, remote, len, HCCL_DATA_TYPE_INT8, HCCL_REDUCE_PROD, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidWriteWithBuffer)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        CcuBuffer buf;
        RemoteAddr remote;
        Variable len;
        Event evt;
        ChannelHandle ch = MakeChannel(0, 1);
        Write(ch, remote, buf, len, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidLocalCopyBufferToAddr)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        LocalAddr dst;
        CcuBuffer src;
        Variable len;
        Event evt;
        LocalCopy(dst, src, len, evt);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidControlFlow)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        Variable v;
        v = 0U;
        CCU_WHILE(v != 10U)
        {
            Variable one;
            one = 1U;
            v += one;
        }
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, ValidNotifySync)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        ChannelHandle ch = MakeChannel(0, 1);
        Variable v;
        WriteVariableWithNotify(ch, v, 0, 1, 0x1);
        NotifyWait(ch, 1, 0x1);
        NotifyRecord(ch, 2, 0x2);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

// ===== L2: 静态检查器 — 非法 IR 拒绝 =====

TEST(CcuStFrameworkL2, RejectZeroHandleVariable)
{
    Program program;
    Operation op;
    op.code = OpCode::ASSIGN_IMM;
    op.dst = 0; // invalid: zero handle
    program.operations.push_back(op);
    program.variables.insert(0); // zero handle in variables
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL2, RejectDuplicateResourceHandle)
{
    Program program;
    program.variables.insert(1);
    program.addresses.insert(1); // collision: same handle in variables and addresses
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL2, RejectWriteInvalidSrc)
{
    // WRITE with src0 = RemoteAddr (should be LocalAddr/Buffer)
    Program program;
    ResourceHandle varHandle = 1;
    ResourceHandle addrHandle = 2;
    ResourceHandle evtHandle = 3;
    ResourceHandle remoteHandle = 4;
    program.variables.insert(varHandle);
    program.variables.insert(addrHandle); // remote handle is in variables, not remoteAddresses
    program.events.insert(evtHandle);
    program.remoteAddresses[remoteHandle] = {addrHandle, varHandle};

    Operation op;
    op.code = OpCode::WRITE;
    op.src0 = remoteHandle; // invalid: not in localAddresses or buffers
    op.dst = remoteHandle;
    op.src1 = varHandle;
    op.immediate = evtHandle;
    program.operations.push_back(op);
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL2, RejectReadInvalidSrc)
{
    // READ with src0 = LocalAddr (should be RemoteAddr)
    Program program;
    ResourceHandle varHandle = 1;
    ResourceHandle addrHandle = 2;
    ResourceHandle evtHandle = 3;
    ResourceHandle localHandle = 4;
    program.variables.insert(varHandle);
    program.addresses.insert(addrHandle);
    program.events.insert(evtHandle);
    program.localAddresses[localHandle] = {addrHandle, varHandle};

    Operation op;
    op.code = OpCode::READ;
    op.src0 = localHandle; // invalid: not in remoteAddresses
    op.dst = localHandle;
    op.src1 = varHandle;
    op.immediate = evtHandle;
    program.operations.push_back(op);
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL2, RejectLocalReduceInvalidEvent)
{
    Program program;
    ResourceHandle varHandle = 1;
    ResourceHandle addrHandle = 2;
    ResourceHandle localHandle = 3;
    program.variables.insert(varHandle);
    program.addresses.insert(addrHandle);
    program.localAddresses[localHandle] = {addrHandle, varHandle};

    Operation op;
    op.code = OpCode::LOCAL_REDUCE;
    op.dst = localHandle;
    op.src0 = localHandle;
    op.src1 = varHandle;
    op.immediate = 999; // invalid: not an Event handle
    program.operations.push_back(op);
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

TEST(CcuStFrameworkL2, RejectGroupCopyOutOfRange)
{
    Program program;
    Operation op;
    op.code = OpCode::GROUP_COPY;
    op.index = 5; // out of range: no groupCopies
    program.operations.push_back(op);
    auto result = CcuLogicalResourceChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}

// ===== L2: LoadArg 契约校验 =====

TEST(CcuStFrameworkL2, LoadArgContractMatch)
{
    auto* prog = BuildValidProgram("test", [](void*) {
        Variable v;
        LoadArg(v, 0);
        return Result::SUCCESS;
    });
    ASSERT_NE(prog, nullptr);
    auto result = CcuProgramChecker().Check(*prog);
    EXPECT_TRUE(result.Ok()) << result.message;
}

TEST(CcuStFrameworkL2, LoadArgMissing)
{
    // ArgDescriptor exists but no LoadArg IR
    Program program;
    ResourceHandle varHandle = 1;
    program.variables.insert(varHandle);
    program.arguments.push_back(ArgDescriptor{0, varHandle});
    auto result = CcuProgramChecker().Check(program);
    EXPECT_FALSE(result.Ok());
}
