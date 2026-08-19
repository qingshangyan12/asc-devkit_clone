/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "specialization/compilation_plan_executor.h"
#include "specialization/kernel_specialization_diagnostics.h"

#include <gtest/gtest.h>

#include <chrono>
#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <csignal>
#include <fstream>
#include <thread>
#include <time.h>
#include <type_traits>
#include <unistd.h>

namespace {
namespace fs = boost::filesystem;
using namespace std::chrono_literals;
using ascendc::aclrtc::CompilationCommand;
using ascendc::aclrtc::CompilationCommandExecutionLimits;
using ascendc::aclrtc::CompilationPlanExecutor;
using ascendc::aclrtc::CompilationCommandKind;
using ascendc::aclrtc::CompilationProcessResult;
using ascendc::aclrtc::CompilationProcessTermination;
using ascendc::aclrtc::CompilationVariant;
using ascendc::aclrtc::KernelCompilationPlan;
using ascendc::aclrtc::KernelSpecializationDiagnostics;

static_assert(std::is_same<std::underlying_type_t<CompilationVariant>, uint32_t>::value,
              "CompilationVariant must have a stable width");
static_assert(std::is_same<std::underlying_type_t<CompilationCommandKind>, uint32_t>::value,
              "CompilationCommandKind must have a stable width");
static_assert(std::is_same<std::underlying_type_t<CompilationProcessTermination>, uint32_t>::value,
              "CompilationProcessTermination must have a stable width");
static_assert(std::is_same<decltype(CompilationProcessResult::terminationCode), int32_t>::value,
              "terminationCode must have a stable width");
static_assert(
    std::is_same<decltype(CompilationCommandExecutionLimits::capturedOutputByteLimit), uint64_t>::value,
    "capturedOutputByteLimit must have a stable width");

CompilationCommand CreateCommand(CompilationCommandKind commandKind, const char* executable,
                                 std::vector<std::string> arguments)
{
    return {commandKind, 0U, "test-command", executable, std::move(arguments)};
}

class ScopedDiagnosticCurrentDirectory final {
public:
    ScopedDiagnosticCurrentDirectory()
        : previousDirectoryPath_(fs::current_path()),
          temporaryDirectoryPath_(fs::temp_directory_path() /
                                  fs::unique_path("aclrtc_executor_diagnostics_%%%%-%%%%"))
    {
        fs::create_directories(temporaryDirectoryPath_);
        fs::current_path(temporaryDirectoryPath_);
    }

    ~ScopedDiagnosticCurrentDirectory()
    {
        boost::system::error_code ignoredError;
        fs::current_path(previousDirectoryPath_, ignoredError);
        fs::remove_all(temporaryDirectoryPath_, ignoredError);
    }

private:
    fs::path previousDirectoryPath_;
    fs::path temporaryDirectoryPath_;
};

std::string ReadTextFile(const fs::path& filePath)
{
    std::ifstream inputStream(filePath.string());
    return {std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>()};
}

TEST(CompilationPlanExecutorTest, ProductionLimitsMatchReviewedPerCommandTimeouts)
{
    const CompilationCommandExecutionLimits limits = CompilationCommandExecutionLimits::ProductionDefaults();
    EXPECT_EQ(limits.compileCommandTimeout, 5min);
    EXPECT_EQ(limits.objectCopyAndLinkCommandTimeout, 1min);
    EXPECT_EQ(limits.terminationGracePeriod, 5s);
    EXPECT_EQ(limits.capturedOutputByteLimit, 4U * 1024U * 1024U);
}

TEST(CompilationPlanExecutorTest, ExecutesCommandsWithoutShellParsing)
{
    boost::system::error_code ignoredError;
    fs::remove("/tmp/aclrtc_should_not_exist", ignoredError);
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_success");
    CompilationPlanExecutor executor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(
        CompilationCommandKind::Compile, "/bin/echo", {"$(touch /tmp/aclrtc_should_not_exist);literal"}));

    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), 0);
    EXPECT_FALSE(fs::exists("/tmp/aclrtc_should_not_exist"));
}

TEST(CompilationPlanExecutorTest, RejectsEmbeddedNullBeforeSpawningProcess)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_embedded_null");
    CompilationPlanExecutor executor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());

    std::string executableWithEmbeddedNull("/bin/true");
    executableWithEmbeddedNull.push_back('\0');
    executableWithEmbeddedNull += "/bin/false";
    KernelCompilationPlan executablePlan;
    CompilationCommand executableCommand =
        CreateCommand(CompilationCommandKind::Compile, "/bin/true", {});
    executableCommand.executablePath = fs::path(executableWithEmbeddedNull);
    executablePlan.compilationCommands.push_back(std::move(executableCommand));
    EXPECT_EQ(executor.ExecuteCompilationPlan(executablePlan), ascendc::aclrtc::ACLRTC_ERROR_FAILURE);

    std::string argumentWithEmbeddedNull("accepted-prefix");
    argumentWithEmbeddedNull.push_back('\0');
    argumentWithEmbeddedNull += "discarded-suffix";
    KernelCompilationPlan argumentPlan;
    argumentPlan.compilationCommands.push_back(
        CreateCommand(CompilationCommandKind::Compile, "/bin/true", {argumentWithEmbeddedNull}));
    EXPECT_EQ(executor.ExecuteCompilationPlan(argumentPlan), ascendc::aclrtc::ACLRTC_ERROR_FAILURE);
}

TEST(CompilationPlanExecutorTest, MapsCompileAndLinkFailuresToDifferentErrors)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_failure");
    CompilationPlanExecutor executor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(CompilationCommandKind::Compile, "/bin/false", {}));
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_COMPILATION);

    plan.compilationCommands[0].commandKind = CompilationCommandKind::Link;
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_LINKING);
}

TEST(CompilationPlanExecutorTest, UsesInjectedShortTimeoutInUnitTests)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_timeout");
    CompilationCommandExecutionLimits limits;
    limits.compileCommandTimeout = 20ms;
    limits.objectCopyAndLinkCommandTimeout = 20ms;
    limits.terminationGracePeriod = 20ms;
    limits.capturedOutputByteLimit = 1024U;
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(CompilationCommandKind::Compile, "/bin/sleep", {"10"}));

    const auto startedAt = std::chrono::steady_clock::now();
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_COMPILATION);
    EXPECT_LT(std::chrono::steady_clock::now() - startedAt, 2s);
}

TEST(CompilationPlanExecutorTest, EnforcesTimeoutWhileCommandContinuouslyProducesOutput)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_continuous_output");
    CompilationCommandExecutionLimits limits {20ms, 20ms, 20ms, 1024U};
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(
        CompilationCommandKind::Compile, "/bin/sh",
        {"-c", "yes & yes & yes & yes & yes & yes & yes & yes & wait"}));

    const auto startedAt = std::chrono::steady_clock::now();
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_COMPILATION);
    EXPECT_LT(std::chrono::steady_clock::now() - startedAt, 2s);
}

TEST(CompilationPlanExecutorTest, WaitsWithoutBusyPollingAfterCommandOutputCloses)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_closed_output");
    CompilationCommandExecutionLimits limits {1s, 1s, 20ms, 1024U};
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(
        CompilationCommandKind::Compile, "/bin/sh", {"-c", "exec >/dev/null 2>&1; sleep 0.25"}));

    timespec cpuStartedAt {};
    timespec cpuFinishedAt {};
    ASSERT_EQ(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpuStartedAt), 0);
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_SUCCESS);
    ASSERT_EQ(clock_gettime(CLOCK_THREAD_CPUTIME_ID, &cpuFinishedAt), 0);
    const int64_t consumedCpuNanoseconds =
        (static_cast<int64_t>(cpuFinishedAt.tv_sec) - static_cast<int64_t>(cpuStartedAt.tv_sec)) * 1000000000LL +
        static_cast<int64_t>(cpuFinishedAt.tv_nsec) - static_cast<int64_t>(cpuStartedAt.tv_nsec);
    EXPECT_LT(consumedCpuNanoseconds, 100000000LL);
}

TEST(CompilationPlanExecutorTest, WaitsForOutputPipeClosureAfterCommandLeaderExits)
{
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_leader_exit");
    CompilationCommandExecutionLimits limits {500ms, 500ms, 20ms, 1024U};
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(CreateCommand(
        CompilationCommandKind::Compile, "/bin/sh", {"-c", "sleep 0.3 & exec /bin/true"}));

    const auto startedAt = std::chrono::steady_clock::now();
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_SUCCESS);
    EXPECT_GE(std::chrono::steady_clock::now() - startedAt, 200ms);
}

TEST(CompilationPlanExecutorTest, TimeoutTerminatesDescendantsAfterCommandLeaderExits)
{
    static uint64_t sequence = 0U;
    const fs::path descendantProcessIdPath =
        fs::temp_directory_path() /
        ("aclrtc_descendant_pid_" + std::to_string(getpid()) + "_" + std::to_string(sequence++));
    KernelSpecializationDiagnostics diagnostics(fs::path(), "executor_process_group");
    CompilationCommandExecutionLimits limits {20ms, 20ms, 20ms, 1024U};
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    const std::string commandScript =
        "(trap '' TERM; sleep 10) & echo $! > '" + descendantProcessIdPath.string() +
        "'; exec /bin/true";
    plan.compilationCommands.push_back(
        CreateCommand(CompilationCommandKind::Compile, "/bin/sh", {"-c", commandScript}));

    ASSERT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_COMPILATION);
    std::ifstream descendantProcessIdStream(descendantProcessIdPath.string());
    pid_t descendantProcessId = -1;
    ASSERT_TRUE(descendantProcessIdStream >> descendantProcessId);
    bool descendantProcessAlive = true;
    for (size_t attempt = 0U; attempt < 40U; ++attempt) {
        if (kill(descendantProcessId, 0) != 0 && errno == ESRCH) {
            descendantProcessAlive = false;
            break;
        }
        std::this_thread::sleep_for(5ms);
    }
    if (descendantProcessAlive) {
        kill(descendantProcessId, SIGKILL);
    }
    EXPECT_FALSE(descendantProcessAlive);
    boost::system::error_code ignoredError;
    fs::remove(descendantProcessIdPath, ignoredError);
}

TEST(CompilationPlanExecutorTest, RecordsSpawnFailureAsInfrastructureFailure)
{
    ScopedDiagnosticCurrentDirectory diagnosticCurrentDirectory;
    const std::string specializationSessionId = "executor_spawn_failure";
    KernelSpecializationDiagnostics diagnostics(fs::current_path(), specializationSessionId);
    CompilationPlanExecutor executor(diagnostics, CompilationCommandExecutionLimits::ProductionDefaults());
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(
        CreateCommand(CompilationCommandKind::Compile, "/path/that/does/not/exist", {}));
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_ERROR_FAILURE);

    const std::string compilationLog = ReadTextFile(fs::current_path() / "aclrtc_compile.log");
    EXPECT_NE(compilationLog.find("termination=infrastructure_failure"), std::string::npos);
    EXPECT_EQ(compilationLog.find("termination=exited code=0"), std::string::npos);
}

TEST(CompilationPlanExecutorTest, BoundsCapturedProcessOutput)
{
    ScopedDiagnosticCurrentDirectory diagnosticCurrentDirectory;
    const std::string specializationSessionId = "executor_output_limit";
    KernelSpecializationDiagnostics diagnostics(fs::current_path(), specializationSessionId);
    CompilationCommandExecutionLimits limits = CompilationCommandExecutionLimits::ProductionDefaults();
    limits.capturedOutputByteLimit = 8U;
    CompilationPlanExecutor executor(diagnostics, limits);
    KernelCompilationPlan plan;
    plan.compilationCommands.push_back(
        CreateCommand(CompilationCommandKind::Compile, "/usr/bin/printf", {"0123456789abcdef"}));
    EXPECT_EQ(executor.ExecuteCompilationPlan(plan), ascendc::aclrtc::ACLRTC_SUCCESS);

    const std::string compilationLog = ReadTextFile(fs::current_path() / "aclrtc_compile.log");
    EXPECT_NE(compilationLog.find("output_truncated=true"), std::string::npos);
    EXPECT_NE(compilationLog.find("01234567"), std::string::npos);
    EXPECT_EQ(compilationLog.find("89abcdef"), std::string::npos);
}
} // namespace
