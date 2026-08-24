/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include <gtest/gtest.h>

#include "process_executor.h"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <chrono>
#include <csignal>
#include <cstdlib>
#include <string>
#include <thread>
#include <type_traits>
#include <utility>
#include <unistd.h>
#include <vector>

namespace ascendc {
namespace {
namespace fs = boost::filesystem;
using namespace std::chrono_literals;

static_assert(
    std::is_same<std::underlying_type_t<ProcessOutputMode>, uint32_t>::value,
    "ProcessOutputMode must have a stable width");
static_assert(
    std::is_same<std::underlying_type_t<ProcessOutcome>, uint32_t>::value, "ProcessOutcome must have a stable width");
static_assert(
    std::is_same<decltype(ProcessExecutionResult::terminationCode), int32_t>::value,
    "terminationCode must have a stable width");

ProcessExecutionRequest CreateCapturedRequest(std::vector<std::string> arguments)
{
    ProcessExecutionRequest request;
    request.arguments = std::move(arguments);
    request.outputMode = ProcessOutputMode::CaptureCombined;
    request.executionTimeout = 1s;
    request.terminationGracePeriod = 20ms;
    request.capturedOutputByteLimit = 1024U;
    return request;
}

TEST(ProcessExecutorTest, RejectsInvalidRequestsBeforeSpawning)
{
    ProcessExecutionRequest emptyArgumentsRequest;
    emptyArgumentsRequest.executionTimeout = 1s;
    EXPECT_EQ(ProcessExecutor::Execute(emptyArgumentsRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest zeroTimeoutRequest = CreateCapturedRequest({"/bin/true"});
    zeroTimeoutRequest.executionTimeout = 0ms;
    EXPECT_EQ(ProcessExecutor::Execute(zeroTimeoutRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest negativeGracePeriodRequest = CreateCapturedRequest({"/bin/true"});
    negativeGracePeriodRequest.terminationGracePeriod = -1ms;
    EXPECT_EQ(ProcessExecutor::Execute(negativeGracePeriodRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest invalidOutputModeRequest = CreateCapturedRequest({"/bin/true"});
    invalidOutputModeRequest.outputMode = static_cast<ProcessOutputMode>(UINT32_MAX);
    EXPECT_EQ(ProcessExecutor::Execute(invalidOutputModeRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest embeddedNullRequest = CreateCapturedRequest({std::string("/bin/true\0invalid", 17U)});
    EXPECT_EQ(ProcessExecutor::Execute(embeddedNullRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest invalidOverrideNameRequest = CreateCapturedRequest({"/bin/true"});
    invalidOverrideNameRequest.environmentOverrides.emplace_back("INVALID=NAME", "value");
    EXPECT_EQ(ProcessExecutor::Execute(invalidOverrideNameRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest embeddedNullOverrideValueRequest = CreateCapturedRequest({"/bin/true"});
    embeddedNullOverrideValueRequest.environmentOverrides.emplace_back("VALID_NAME", std::string("value\0suffix", 12U));
    EXPECT_EQ(ProcessExecutor::Execute(embeddedNullOverrideValueRequest).outcome, ProcessOutcome::ExecutorFailure);

    ProcessExecutionRequest invalidRemovalNameRequest = CreateCapturedRequest({"/bin/true"});
    invalidRemovalNameRequest.removedEnvironmentVariables.emplace_back("INVALID=NAME");
    EXPECT_EQ(ProcessExecutor::Execute(invalidRemovalNameRequest).outcome, ProcessOutcome::ExecutorFailure);
}

TEST(ProcessExecutorTest, ExecutesArgumentsWithoutShellParsing)
{
    const fs::path unexpectedFilePath =
        fs::temp_directory_path() / ("process_executor_shell_" + std::to_string(getpid()));
    boost::system::error_code ignoredError;
    fs::remove(unexpectedFilePath, ignoredError);

    ProcessExecutionRequest request =
        CreateCapturedRequest({"/usr/bin/printf", "$(touch " + unexpectedFilePath.string() + ")"});
    const ProcessExecutionResult result = ProcessExecutor::Execute(request);

    EXPECT_TRUE(result.HasSuccessfulExit());
    EXPECT_EQ(result.capturedOutput, "$(touch " + unexpectedFilePath.string() + ")");
    EXPECT_FALSE(fs::exists(unexpectedFilePath));
}

TEST(ProcessExecutorTest, AppliesEnvironmentOverridesAndRemovals)
{
    ASSERT_EQ(setenv("PROCESS_EXECUTOR_REMOVE_ME", "inherited", 1), 0);
    ProcessExecutionRequest request = CreateCapturedRequest(
        {"/bin/sh", "-c", "printf '%s|%s' \"$PROCESS_EXECUTOR_VALUE\" \"${PROCESS_EXECUTOR_REMOVE_ME-unset}\""});
    request.environmentOverrides.emplace_back("PROCESS_EXECUTOR_VALUE", "visible");
    request.removedEnvironmentVariables.emplace_back("PROCESS_EXECUTOR_REMOVE_ME");

    const ProcessExecutionResult result = ProcessExecutor::Execute(request);
    EXPECT_TRUE(result.HasSuccessfulExit());
    EXPECT_EQ(result.capturedOutput, "visible|unset");
    EXPECT_EQ(unsetenv("PROCESS_EXECUTOR_REMOVE_ME"), 0);
}

TEST(ProcessExecutorTest, EnvironmentOverrideTakesPrecedenceOverRemoval)
{
    ProcessExecutionRequest request =
        CreateCapturedRequest({"/bin/sh", "-c", "printf '%s' \"$PROCESS_EXECUTOR_PRECEDENCE\""});
    request.removedEnvironmentVariables.emplace_back("PROCESS_EXECUTOR_PRECEDENCE");
    request.environmentOverrides.emplace_back("PROCESS_EXECUTOR_PRECEDENCE", "overridden");

    const ProcessExecutionResult result = ProcessExecutor::Execute(request);
    EXPECT_TRUE(result.HasSuccessfulExit());
    EXPECT_EQ(result.capturedOutput, "overridden");
}

TEST(ProcessExecutorTest, ReportsExitSignalAndSpawnFailureSeparately)
{
    const ProcessExecutionResult nonzeroExit =
        ProcessExecutor::Execute(CreateCapturedRequest({"/bin/sh", "-c", "exit 7"}));
    EXPECT_EQ(nonzeroExit.outcome, ProcessOutcome::Exited);
    EXPECT_EQ(nonzeroExit.terminationCode, 7);

    const ProcessExecutionResult signalExit =
        ProcessExecutor::Execute(CreateCapturedRequest({"/bin/sh", "-c", "kill -TERM $$"}));
    EXPECT_EQ(signalExit.outcome, ProcessOutcome::Signaled);
    EXPECT_EQ(signalExit.terminationCode, SIGTERM);

    const ProcessExecutionResult spawnFailure =
        ProcessExecutor::Execute(CreateCapturedRequest({"/path/that/does/not/exist"}));
    EXPECT_EQ(spawnFailure.outcome, ProcessOutcome::ExecutorFailure);
}

TEST(ProcessExecutorTest, BoundsCapturedCombinedOutput)
{
    ProcessExecutionRequest request = CreateCapturedRequest({"/bin/sh", "-c", "printf stdout; printf stderr >&2"});
    request.capturedOutputByteLimit = 8U;

    const ProcessExecutionResult result = ProcessExecutor::Execute(request);
    EXPECT_TRUE(result.HasSuccessfulExit());
    EXPECT_EQ(result.capturedOutput.size(), 8U);
    EXPECT_TRUE(result.capturedOutputTruncated);
}

TEST(ProcessExecutorTest, InheritModeDoesNotCaptureOutput)
{
    ProcessExecutionRequest request;
    request.arguments = {"/bin/true"};
    request.outputMode = ProcessOutputMode::Inherit;
    request.executionTimeout = 1s;
    request.terminationGracePeriod = 20ms;

    const ProcessExecutionResult result = ProcessExecutor::Execute(request);
    EXPECT_TRUE(result.HasSuccessfulExit());
    EXPECT_TRUE(result.capturedOutput.empty());
}

TEST(ProcessExecutorTest, TimeoutTerminatesDescendantsAfterLeaderExits)
{
    static uint64_t sequence = 0U;
    const fs::path descendantMarkerPath =
        fs::temp_directory_path() /
        ("process_executor_descendant_" + std::to_string(getpid()) + "_" + std::to_string(sequence++));
    const std::string commandScript =
        "(trap '' TERM; count=0; while [ $count -lt 100 ]; do printf x >> '" + descendantMarkerPath.string() +
        "'; count=$((count + 1)); sleep 0.01; done; rm -f '" + descendantMarkerPath.string() + "') & exec /bin/true";
    ProcessExecutionRequest request = CreateCapturedRequest({"/bin/sh", "-c", commandScript});
    request.executionTimeout = 20ms;

    const ProcessExecutionResult result = ProcessExecutor::Execute(request);
    EXPECT_EQ(result.outcome, ProcessOutcome::TimedOut);

    const uintmax_t markerSizeAfterTimeout =
        fs::exists(descendantMarkerPath) ? fs::file_size(descendantMarkerPath) : 0U;
    std::this_thread::sleep_for(100ms);
    const uintmax_t markerSizeAfterWait = fs::exists(descendantMarkerPath) ? fs::file_size(descendantMarkerPath) : 0U;
    EXPECT_EQ(markerSizeAfterWait, markerSizeAfterTimeout);
    boost::system::error_code ignoredError;
    fs::remove(descendantMarkerPath, ignoredError);
}

} // namespace
} // namespace ascendc
