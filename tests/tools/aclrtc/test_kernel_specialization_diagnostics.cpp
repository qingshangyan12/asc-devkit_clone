/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "specialization/kernel_specialization_diagnostics.h"
#include "stub/kernel_specialization_log.h"

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <atomic>
#include <cerrno>
#include <cstdlib>
#include <fstream>
#include <new>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {
std::atomic<bool> gRejectTestAllocations {false};

void* AllocateTestMemory(std::size_t byteCount) noexcept
{
    if (gRejectTestAllocations.load(std::memory_order_relaxed)) {
        return nullptr;
    }
    return std::malloc(byteCount == 0U ? 1U : byteCount);
}
}

void* operator new(std::size_t byteCount)
{
    void* allocation = AllocateTestMemory(byteCount);
    if (allocation == nullptr) {
        throw std::bad_alloc();
    }
    return allocation;
}

void operator delete(void* allocation) noexcept
{
    std::free(allocation);
}

void operator delete(void* allocation, std::size_t) noexcept
{
    std::free(allocation);
}

void* operator new(std::size_t byteCount, const std::nothrow_t&) noexcept
{
    return AllocateTestMemory(byteCount);
}

void operator delete(void* allocation, const std::nothrow_t&) noexcept
{
    std::free(allocation);
}

namespace {
namespace fs = boost::filesystem;
using ascendc::aclrtc::CompilationCommand;
using ascendc::aclrtc::CompilationCommandKind;
using ascendc::aclrtc::CompilationProcessResult;
using ascendc::aclrtc::DiagnosticCollectionMode;
using ascendc::aclrtc::KernelSpecializationDiagnostics;
using ascendc::aclrtc::OutputPublicationStatus;

class ScopedCurrentDirectory final {
public:
    explicit ScopedCurrentDirectory(const fs::path& currentDirectory)
        : previousDirectory_(fs::current_path())
    {
        fs::current_path(currentDirectory);
    }

    ~ScopedCurrentDirectory()
    {
        boost::system::error_code ignoredError;
        fs::current_path(previousDirectory_, ignoredError);
    }

private:
    fs::path previousDirectory_;
};

std::string ReadTextFile(const fs::path& filePath)
{
    std::ifstream inputStream(filePath);
    return std::string(std::istreambuf_iterator<char>(inputStream), std::istreambuf_iterator<char>());
}

int ExecuteReplayScriptAndWait(const fs::path& replayScriptPath)
{
    const pid_t childProcessId = fork();
    if (childProcessId == 0) {
        execl(replayScriptPath.c_str(), replayScriptPath.c_str(), static_cast<char*>(nullptr));
        _exit(127);
    }
    if (childProcessId < 0) {
        return -1;
    }

    int childStatus = 0;
    pid_t waitResult;
    do {
        waitResult = waitpid(childProcessId, &childStatus, 0);
    } while (waitResult < 0 && errno == EINTR);
    if (waitResult != childProcessId || !WIFEXITED(childStatus)) {
        return -1;
    }
    return WEXITSTATUS(childStatus);
}

TEST(KernelSpecializationDiagnosticsTest, SavesOnlyReviewedDiagnosticFiles)
{
    static uint64_t sequence = 0U;
    const fs::path testRoot = fs::temp_directory_path() /
                              ("aclrtc_diagnostics_test_" + std::to_string(getpid()) + "_" +
                               std::to_string(sequence++));
    fs::create_directories(testRoot);
    {
        ScopedCurrentDirectory currentDirectory(testRoot);
        KernelSpecializationDiagnostics diagnostics("session_1", DiagnosticCollectionMode::Enabled);
        diagnostics.WriteManifestSnapshot(nlohmann::json {{"schema_version", "1.0"}});
        diagnostics.RecordWorktreePath("/tmp/resource-worktree");

        const CompilationCommand command {CompilationCommandKind::Compile, 0U, "basic/compile[0]",
                                          "/bin/echo", {"argument with space", "quote'value"}};
        diagnostics.AppendCommandToReplayScript(command);
        const CompilationCommand workingDirectoryCheck {
            CompilationCommandKind::Compile,
            std::nullopt,
            "basic/compile[1]",
            "/bin/sh",
            {"-c", "test \"$PWD\" = \"$1\"", "replay-working-directory-check", testRoot.string()}};
        diagnostics.AppendCommandToReplayScript(workingDirectoryCheck);
        CompilationProcessResult processResult;
        processResult.capturedOutput = "compiler output\n";
        diagnostics.AppendCommandResultToCompilationLog(command, processResult);
        diagnostics.WriteSpecializationResult("/tmp/kernel.elf", OutputPublicationStatus::Published);
    }

    const fs::path diagnosticDirectory =
        testRoot / "kernel_meta" / std::to_string(getpid()) / "aclrtc_session_1";
    const fs::path replayScriptPath = diagnosticDirectory / "replay.sh";
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "manifest.json"));
    EXPECT_TRUE(fs::is_regular_file(replayScriptPath));
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "compile.log"));
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "result.json"));
    EXPECT_EQ(std::distance(fs::directory_iterator(diagnosticDirectory), fs::directory_iterator()), 4);

    const std::string replayScript = ReadTextFile(diagnosticDirectory / "replay.sh");
    EXPECT_NE(replayScript.find("'/bin/echo' 'argument with space' 'quote'\"'\"'value'"), std::string::npos);

    struct stat replayScriptStatus {};
    ASSERT_EQ(stat(replayScriptPath.c_str(), &replayScriptStatus), 0);
    EXPECT_EQ(replayScriptStatus.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO), S_IRWXU);
    EXPECT_EQ(ExecuteReplayScriptAndWait(replayScriptPath), 0);

    const nlohmann::json result = nlohmann::json::parse(ReadTextFile(diagnosticDirectory / "result.json"));
    EXPECT_EQ(result.at("specialization_session_id"), "session_1");
    EXPECT_EQ(result.at("output_path"), "/tmp/kernel.elf");
    EXPECT_TRUE(result.at("output_published"));
    EXPECT_FALSE(result.contains("result"));

    boost::system::error_code ignoredError;
    fs::remove_all(testRoot, ignoredError);
}

TEST(KernelSpecializationDiagnosticsTest, ReportsDiagnosticDirectoryCreationFailureWithPath)
{
    static uint64_t sequence = 0U;
    const fs::path testRoot = fs::temp_directory_path() /
                              ("aclrtc_diagnostics_error_test_" + std::to_string(getpid()) + "_" +
                               std::to_string(sequence++));
    fs::create_directories(testRoot);
    std::ofstream(testRoot / "kernel_meta") << "blocks directory creation";
    {
        ScopedCurrentDirectory currentDirectory(testRoot);
        ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
        KernelSpecializationDiagnostics diagnostics("session_error", DiagnosticCollectionMode::Enabled);
        EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
            (testRoot / "kernel_meta" / std::to_string(getpid()) / "aclrtc_session_error").string()));
    }

    boost::system::error_code ignoredError;
    fs::remove_all(testRoot, ignoredError);
}

TEST(KernelSpecializationDiagnosticsTest, NoexceptFailureLoggingDoesNotAllocateASecondPath)
{
    static uint64_t sequence = 0U;
    const fs::path testRoot = fs::temp_directory_path() /
                              ("aclrtc_diagnostics_noexcept_test_" + std::to_string(getpid()) + "_" +
                               std::to_string(sequence++));
    fs::create_directories(testRoot);
    {
        ScopedCurrentDirectory currentDirectory(testRoot);
        KernelSpecializationDiagnostics diagnostics("session_noexcept", DiagnosticCollectionMode::Enabled);
        const nlohmann::json manifest {{"schema_version", "1.0"}};
        EXPECT_EXIT(
            {
                gRejectTestAllocations.store(true, std::memory_order_relaxed);
                diagnostics.WriteManifestSnapshot(manifest);
                _exit(0);
            },
            ::testing::ExitedWithCode(0), "");
    }

    boost::system::error_code ignoredError;
    fs::remove_all(testRoot, ignoredError);
}
} // namespace
