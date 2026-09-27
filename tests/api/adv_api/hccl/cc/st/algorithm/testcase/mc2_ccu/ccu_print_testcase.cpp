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

#include <chrono>
#include <cstring>
#include <limits>
#include <map>
#include <thread>
#include <vector>

#include "ccu_print.h"
#include "ccu_primitives_stub.h"
#include "log_test_stub.h"

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

bool WaitForLog(const std::string& expected)
{
    for (uint32_t retry = 0; retry < 100; ++retry) {
        for (const std::string& log : GetTestLogs()) {
            if (log.find(expected) != std::string::npos) {
                return true;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    return false;
}

CcuResult BuildCcuPrintProgram(Program& program)
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
    CCU_PRINTF("u=%u x=%x p=%p v=%v %%", unsignedValue, hexValue, pointerValue, variableValue);
    CompilerContext::SetCurrent(nullptr);
    return CCU_SUCCESS;
}

CcuResult BuildFirstOrderingProgram(Program& program)
{
    CompilerContext compiler(program);
    CompilerContext::SetCurrent(&compiler);
    CCU_PRINTF("ordering-first");
    CompilerContext::SetCurrent(nullptr);
    return CCU_SUCCESS;
}

CcuResult BuildSecondOrderingProgram(Program& program)
{
    CompilerContext compiler(program);
    CompilerContext::SetCurrent(&compiler);
    CCU_PRINTF("ordering-second");
    CompilerContext::SetCurrent(nullptr);
    return CCU_SUCCESS;
}

} // namespace

TEST(CcuPrintMockTest, EmitsCcuStoresAndHostConsumerLogsFormattedRecord)
{
    SetTestLogLevel(0);
    ClearTestLogs();
    Program program;

    ASSERT_EQ(BuildCcuPrintProgram(program), CCU_SUCCESS);
    ASSERT_TRUE(ExecuteCcuPrintProgram(program));
    EXPECT_TRUE(WaitForLog("u=42 x=abc p=0x123 v=99 %"));
}

TEST(CcuPrintMockTest, OrdersDifferentCallSitesBySharedSequence)
{
    SetTestLogLevel(0);
    Program firstProgram;
    Program secondProgram;
    Program restartProgram;

    ASSERT_EQ(BuildFirstOrderingProgram(firstProgram), CCU_SUCCESS);
    ASSERT_EQ(BuildSecondOrderingProgram(secondProgram), CCU_SUCCESS);
    AscendC::ccu::dfx::CcuPrintStream::GetInstance()->Stop();
    ClearTestLogs();

    ASSERT_TRUE(ExecuteCcuPrintProgram(secondProgram));
    ASSERT_TRUE(ExecuteCcuPrintProgram(firstProgram));
    ASSERT_EQ(BuildCcuPrintProgram(restartProgram), CCU_SUCCESS);
    ASSERT_TRUE(WaitForLog("ordering-first"));
    ASSERT_TRUE(WaitForLog("ordering-second"));

    size_t firstIndex = std::numeric_limits<size_t>::max();
    size_t secondIndex = std::numeric_limits<size_t>::max();
    const std::vector<std::string> logs = GetTestLogs();
    for (size_t index = 0; index < logs.size(); ++index) {
        if (logs[index].find("ordering-first") != std::string::npos) {
            firstIndex = index;
        }
        if (logs[index].find("ordering-second") != std::string::npos) {
            secondIndex = index;
        }
    }
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
