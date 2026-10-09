/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>
#include "acl/acl_rt.h"
#include <cstdlib>

#include <cstring>
#include <map>
#include <vector>

#include "ccu_print.h"
#include "ccu_primitives_stub.h"
extern "C" void SetTestLogLevel(uint32_t level);

// CPU memory stands in for GM in this standalone test executable.
extern "C" aclError aclrtMalloc(void** pointer, size_t size, aclrtMemMallocPolicy)
{
    if (pointer == nullptr || size == 0) {
        return ACL_ERROR_INVALID_PARAM;
    }
    *pointer = std::malloc(size);
    return *pointer == nullptr ? ACL_ERROR_INTERNAL_ERROR : ACL_SUCCESS;
}

extern "C" aclError aclrtMemcpy(void* dst, size_t destMax, const void* src, size_t count, aclrtMemcpyKind)
{
    if (dst == nullptr || src == nullptr || count > destMax) {
        return ACL_ERROR_INVALID_PARAM;
    }
    std::memcpy(dst, src, count);
    return ACL_SUCCESS;
}

extern "C" aclError aclrtMemset(void* dst, size_t maxCount, int32_t value, size_t count)
{
    if (dst == nullptr || count > maxCount) {
        return ACL_ERROR_INVALID_PARAM;
    }
    std::memset(dst, value, count);
    return ACL_SUCCESS;
}

extern "C" aclError aclrtFree(void* pointer)
{
    std::free(pointer);
    return ACL_SUCCESS;
}

extern "C" aclError aclrtGetCurrentContext(aclrtContext* context)
{
    static int testContext;
    if (context == nullptr) {
        return ACL_ERROR_INVALID_PARAM;
    }
    *context = &testContext;
    return ACL_SUCCESS;
}

extern "C" aclError aclrtSetCurrentContext(aclrtContext context)
{
    return context == nullptr ? ACL_ERROR_INVALID_PARAM : ACL_SUCCESS;
}

namespace AscendC {
namespace ccu {
namespace dfx {
namespace detail {

// Own a stream per test so all allocations are released before the next test.
struct CcuPrintTestAccess {
    static CcuPrintStream* Create() { return new CcuPrintStream; }
    static void Destroy(CcuPrintStream* stream) { delete stream; }
};

} // namespace detail
} // namespace dfx
} // namespace ccu
} // namespace AscendC

namespace {

using HcclSim::CcuSt::CompilerContext;
using HcclSim::CcuSt::OpCode;
using HcclSim::CcuSt::Operation;
using HcclSim::CcuSt::Program;
using HcclSim::CcuSt::ResourceHandle;

static_assert(
    AscendC::ccu::dfx::detail::AreCcuVariables<AscendC::ccu::Variable>::value, "ccu::Variable must be accepted");
static_assert(
    !AscendC::ccu::dfx::detail::AreCcuVariables<uint64_t>::value, "non-ccu::Variable arguments must be rejected");

class CcuPrintMockTest : public testing::Test {
protected:
    void SetUp() override
    {
        stream = AscendC::ccu::dfx::detail::CcuPrintTestAccess::Create();
        SetTestLogLevel(0);
        testing::internal::CaptureStdout();
        capturing = true;
    }

    void TearDown() override
    {
        // Join the consumer and release GM before restoring stdout.
        AscendC::ccu::dfx::detail::CcuPrintTestAccess::Destroy(stream);
        if (capturing) {
            (void)testing::internal::GetCapturedStdout();
            capturing = false;
        }
        CompilerContext::SetCurrent(nullptr);
        SetTestLogLevel(3);
    }

    std::string StopAndGetOutput()
    {
        // Stop joins the consumer after its final drain, before restoring stdout.
        stream->Stop();
        std::string output = testing::internal::GetCapturedStdout();
        capturing = false;
        return output;
    }

    bool capturing{false};
    AscendC::ccu::dfx::CcuPrintStream* stream{nullptr};
};

bool EvaluateCondition(const std::map<ResourceHandle, uint64_t>& variables, const Operation& operation)
{
    auto found = variables.find(operation.src0);
    if (found == variables.end()) {
        return false;
    }
    const bool equal = found->second == operation.immediate;
    return operation.equal ? equal : !equal;
}

bool ExecuteCcuPrintProgram(const Program& program)
{
    std::map<ResourceHandle, uint64_t> variables;
    size_t pc = 0;
    while (pc < program.operations.size()) {
        const Operation& operation = program.operations[pc];
        switch (operation.code) {
            case OpCode::ASSIGN_IMM:
                variables[operation.dst] = operation.immediate;
                break;
            case OpCode::ASSIGN_VAR:
                variables[operation.dst] = variables[operation.src0];
                break;
            case OpCode::ADD_VAR:
                variables[operation.dst] = variables[operation.src0] + variables[operation.src1];
                break;
            case OpCode::LOAD_MEMORY: {
                uint64_t value = 0;
                std::memcpy(&value, reinterpret_cast<const void*>(variables[operation.src0]), sizeof(value));
                variables[operation.dst] = value;
                break;
            }
            case OpCode::STORE_MEMORY: {
                const uint64_t value = variables[operation.src0];
                std::memcpy(reinterpret_cast<void*>(variables[operation.dst]), &value, sizeof(value));
                break;
            }
            case OpCode::IF_BEGIN:
                if (!EvaluateCondition(variables, operation)) {
                    size_t depth = 1;
                    while (depth != 0 && ++pc < program.operations.size()) {
                        if (program.operations[pc].code == OpCode::IF_BEGIN) {
                            ++depth;
                        } else if (program.operations[pc].code == OpCode::IF_END) {
                            --depth;
                        }
                    }
                }
                break;
            case OpCode::IF_END:
                break;
            default:
                return false;
        }
        ++pc;
    }
    return true;
}

CcuResult BuildCcuPrintProgram(Program& program, AscendC::ccu::dfx::CcuPrintStream* stream)
{
    CompilerContext compiler(program);
    CompilerContext::SetCurrent(&compiler);
    AscendC::ccu::Variable unsignedValue;
    AscendC::ccu::Variable hexValue;
    AscendC::ccu::Variable pointerValue;
    AscendC::ccu::Variable variableValue;
    unsignedValue = 42;
    hexValue = 0xabc;
    pointerValue = 0x123;
    variableValue = 99;
    auto* handle = stream->AllocPrintInfo(__FILE__, __LINE__, "u=%u x=%x p=%p v=%v %%", 4);
    const CcuResult result =
        AscendC::ccu::dfx::EmitPrintf(handle, unsignedValue, hexValue, pointerValue, variableValue);
    CompilerContext::SetCurrent(nullptr);
    return handle == nullptr ? CCU_E_INTERNAL : result;
}

CcuResult BuildFirstOrderingProgram(Program& program, AscendC::ccu::dfx::CcuPrintStream* stream)
{
    CompilerContext compiler(program);
    CompilerContext::SetCurrent(&compiler);
    auto* handle = stream->AllocPrintInfo(__FILE__, __LINE__, "ordering-first", 0);
    const CcuResult result = AscendC::ccu::dfx::EmitPrintf(handle);
    CompilerContext::SetCurrent(nullptr);
    return handle == nullptr ? CCU_E_INTERNAL : result;
}

CcuResult BuildSecondOrderingProgram(Program& program, AscendC::ccu::dfx::CcuPrintStream* stream)
{
    CompilerContext compiler(program);
    CompilerContext::SetCurrent(&compiler);
    auto* handle = stream->AllocPrintInfo(__FILE__, __LINE__, "ordering-second", 0);
    const CcuResult result = AscendC::ccu::dfx::EmitPrintf(handle);
    CompilerContext::SetCurrent(nullptr);
    return handle == nullptr ? CCU_E_INTERNAL : result;
}

} // namespace

TEST_F(CcuPrintMockTest, EmitsCcuStoresAndHostConsumerLogsFormattedRecord)
{
    Program program;

    ASSERT_EQ(BuildCcuPrintProgram(program, stream), CCU_SUCCESS);
    ASSERT_TRUE(ExecuteCcuPrintProgram(program));
    const std::string output = StopAndGetOutput();
    EXPECT_NE(output.find("u=42 x=abc p=0x123 v=99 %"), std::string::npos);
}

TEST_F(CcuPrintMockTest, OrdersDifferentCallSitesBySharedSequence)
{
    Program firstProgram;
    Program secondProgram;
    Program restartProgram;

    ASSERT_EQ(BuildFirstOrderingProgram(firstProgram, stream), CCU_SUCCESS);
    ASSERT_EQ(BuildSecondOrderingProgram(secondProgram, stream), CCU_SUCCESS);
    stream->Stop();

    ASSERT_TRUE(ExecuteCcuPrintProgram(secondProgram));
    ASSERT_TRUE(ExecuteCcuPrintProgram(firstProgram));
    ASSERT_EQ(BuildCcuPrintProgram(restartProgram, stream), CCU_SUCCESS);
    const std::string output = StopAndGetOutput();
    const size_t firstIndex = output.find("ordering-first");
    const size_t secondIndex = output.find("ordering-second");
    ASSERT_NE(firstIndex, std::string::npos);
    ASSERT_NE(secondIndex, std::string::npos);
    EXPECT_LT(secondIndex, firstIndex);
}

TEST(CcuPrintFormatTest, FormatsSupportedVariableConversions)
{
    const uint64_t args[] = {42, 0xabc, 0x123, 99};
    std::string output;

    EXPECT_TRUE(AscendC::ccu::dfx::detail::Format("u=%u x=%x p=%p v=%v %%", args, 4, output));
    EXPECT_EQ(output, "u=42 x=abc p=0x123 v=99 %");
}

TEST(CcuPrintFormatTest, SupportsNoArgumentMessage)
{
    std::string output;

    EXPECT_TRUE(AscendC::ccu::dfx::detail::Format("mission complete", nullptr, 0, output));
    EXPECT_EQ(output, "mission complete");
}

TEST(CcuPrintFormatTest, RejectsUnsupportedOrMismatchedFormat)
{
    EXPECT_FALSE(AscendC::ccu::dfx::detail::ValidateFormat("value=%s", 1));
    EXPECT_FALSE(AscendC::ccu::dfx::detail::ValidateFormat("value=%u", 0));
    EXPECT_FALSE(AscendC::ccu::dfx::detail::ValidateFormat("value=%", 0));
}

TEST(CcuPrintFormatTest, MacroReturnsStatusWithoutReturningFromCaller)
{
    // An invalid format avoids allocating GM or emitting instructions.
    // The macro must also be usable in a void function and as an expression.
    bool continued = false;
    auto caller = [&]() -> void {
        const CcuResult result = CCU_PRINTF("%");
        EXPECT_EQ(result, CCU_SUCCESS);
        CCU_PRINTF("%");
        continued = true;
    };
    caller();
    EXPECT_TRUE(continued);
}
