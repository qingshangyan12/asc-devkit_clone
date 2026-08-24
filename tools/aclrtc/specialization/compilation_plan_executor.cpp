/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include "compilation_plan_executor.h"

#include "ascendc_tool_log.h"
#include "kernel_specialization_diagnostics.h"
#include "process_executor.h"

#include <algorithm>
#include <exception>
#include <new>

namespace ascendc {
namespace aclrtc {

CompilationCommandExecutionLimits CompilationCommandExecutionLimits::ProductionDefaults() noexcept
{
    return {std::chrono::minutes(5), std::chrono::minutes(1), std::chrono::seconds(5), 4U * 1024U * 1024U};
}

CompilationPlanExecutor::CompilationPlanExecutor(
    KernelSpecializationDiagnostics& specializationDiagnostics, CompilationCommandExecutionLimits executionLimits)
    : specializationDiagnostics_(specializationDiagnostics), executionLimits_(executionLimits)
{}

aclError CompilationPlanExecutor::ExecuteCompilationPlan(const KernelCompilationPlan& compilationPlan) const
{
    try {
        for (const CompilationCommand& compilationCommand : compilationPlan.compilationCommands) {
            ASCENDLOGI(
                "Executing command %s with %s", compilationCommand.diagnosticLabel.c_str(),
                compilationCommand.executablePath.c_str());
            specializationDiagnostics_.AppendCommandToReplayScript(compilationCommand);
            ProcessExecutionResult processResult;
            const aclError executionResult = ExecuteCompilationCommand(compilationCommand, processResult);
            specializationDiagnostics_.AppendCommandResultToCompilationLog(compilationCommand, processResult);
            if (executionResult != ACLRTC_SUCCESS) {
                constexpr size_t maxLoggedProcessOutputBytes = 4096U;
                const size_t loggedOutputByteCount =
                    std::min(processResult.capturedOutput.size(), maxLoggedProcessOutputBytes);
                const bool loggedOutputTruncated = processResult.capturedOutput.size() > loggedOutputByteCount;
                ASCENDLOGE(
                    "Command %s failed: outcome=%s code=%d elapsed_ms=%lld captured_output_truncated=%s "
                    "logged_output_truncated=%s output_prefix_bytes=%zu output=%.*s",
                    compilationCommand.diagnosticLabel.c_str(), processResult.GetOutcomeName(),
                    static_cast<int>(processResult.terminationCode),
                    static_cast<long long>(processResult.elapsedTime.count()),
                    processResult.capturedOutputTruncated ? "true" : "false", loggedOutputTruncated ? "true" : "false",
                    loggedOutputByteCount, static_cast<int>(loggedOutputByteCount),
                    processResult.capturedOutput.c_str());
                specializationDiagnostics_.LogCommandFailureRecoveryHint();
                return executionResult;
            }
        }
        return ACLRTC_SUCCESS;
    } catch (const std::bad_alloc&) {
        ASCENDLOGE("Out of memory while executing kernel compilation commands");
        return ACLRTC_ERROR_OUT_OF_MEMORY;
    } catch (const std::exception& exception) {
        ASCENDLOGE("Kernel compilation command execution failed with exception: %s", exception.what());
        return ACLRTC_ERROR_FAILURE;
    } catch (...) {
        ASCENDLOGE("Kernel compilation command execution failed with an unknown exception");
        return ACLRTC_ERROR_FAILURE;
    }
}

aclError CompilationPlanExecutor::ExecuteCompilationCommand(
    const CompilationCommand& compilationCommand, ProcessExecutionResult& processResult) const
{
    ProcessExecutionRequest executionRequest;
    executionRequest.arguments.reserve(compilationCommand.commandArguments.size() + 1U);
    executionRequest.arguments.emplace_back(compilationCommand.executablePath.string());
    executionRequest.arguments.insert(
        executionRequest.arguments.end(), compilationCommand.commandArguments.begin(),
        compilationCommand.commandArguments.end());
    executionRequest.outputMode = ProcessOutputMode::CaptureCombined;
    executionRequest.executionTimeout = compilationCommand.commandKind == CompilationCommandKind::Compile ?
                                            executionLimits_.compileCommandTimeout :
                                            executionLimits_.objectCopyAndLinkCommandTimeout;
    executionRequest.terminationGracePeriod = executionLimits_.terminationGracePeriod;
    executionRequest.capturedOutputByteLimit = executionLimits_.capturedOutputByteLimit;

    processResult = ProcessExecutor::Execute(executionRequest);
    if (processResult.HasSuccessfulExit()) {
        return ACLRTC_SUCCESS;
    }
    if (processResult.outcome == ProcessOutcome::ExecutorFailure) {
        return ACLRTC_ERROR_FAILURE;
    }
    return compilationCommand.commandKind == CompilationCommandKind::Link ? ACLRTC_ERROR_LINKING :
                                                                            ACLRTC_ERROR_COMPILATION;
}

} // namespace aclrtc
} // namespace ascendc
