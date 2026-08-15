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
#include <fstream>
#include <string>
#include <unistd.h>

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
        diagnostics.RecordManifest(nlohmann::json {{"schema_version", "1.0"}});
        diagnostics.RecordWorktree("/tmp/resource-worktree");

        const CompilationCommand command {CompilationCommandKind::Compile, 0U, "basic/compile[0]",
                                          "/bin/echo", {"argument with space", "quote'value"}};
        diagnostics.RecordCommand(command);
        CompilationProcessResult processResult;
        processResult.capturedOutput = "compiler output\n";
        diagnostics.RecordCommandResult(command, processResult);
        diagnostics.RecordFinalResult("/tmp/kernel.elf", OutputPublicationStatus::Published);
    }

    const fs::path diagnosticDirectory =
        testRoot / "kernel_meta" / std::to_string(getpid()) / "aclrtc_session_1";
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "manifest.json"));
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "replay.sh"));
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "compile.log"));
    EXPECT_TRUE(fs::is_regular_file(diagnosticDirectory / "result.json"));
    EXPECT_EQ(std::distance(fs::directory_iterator(diagnosticDirectory), fs::directory_iterator()), 4);

    const std::string replayScript = ReadTextFile(diagnosticDirectory / "replay.sh");
    EXPECT_NE(replayScript.find("'/bin/echo' 'argument with space' 'quote'\"'\"'value'"), std::string::npos);

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
} // namespace
