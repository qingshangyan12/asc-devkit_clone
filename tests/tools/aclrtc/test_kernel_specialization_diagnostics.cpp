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

#include <gtest/gtest.h>
#include <nlohmann/json.hpp>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstdint>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

namespace {
namespace fs = boost::filesystem;
using ascendc::aclrtc::CompilationCommand;
using ascendc::aclrtc::CompilationCommandKind;
using ascendc::aclrtc::CompilationProcessResult;
using ascendc::aclrtc::KernelSpecializationDiagnostics;
using ascendc::aclrtc::OutputPublicationStatus;

class ScopedCurrentDirectory final {
public:
    explicit ScopedCurrentDirectory(const fs::path& currentDirectory) : previousDirectory_(fs::current_path())
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
    std::ifstream inputStream(filePath.string());
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
    const fs::path testRoot = fs::temp_directory_path() / ("aclrtc_diagnostics_test_" + std::to_string(getpid()) + "_" +
                                                           std::to_string(sequence++));
    fs::create_directories(testRoot);
    {
        ScopedCurrentDirectory currentDirectory(testRoot);
        KernelSpecializationDiagnostics diagnostics(testRoot, "session_1");
        diagnostics.WriteManifestSnapshot(nlohmann::json{{"schema_version", "1.0"}});

        const CompilationCommand command{
            CompilationCommandKind::Compile,
            0U,
            "basic/compile[0]",
            "/bin/echo",
            {"argument with space", "quote'value"}};
        diagnostics.AppendCommandToReplayScript(command);
        const CompilationCommand workingDirectoryCheck{
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

    const fs::path replayScriptPath = testRoot / "aclrtc_replay.sh";
    EXPECT_TRUE(fs::is_regular_file(testRoot / "aclrtc_manifest.json"));
    EXPECT_TRUE(fs::is_regular_file(replayScriptPath));
    EXPECT_TRUE(fs::is_regular_file(testRoot / "aclrtc_compile.log"));
    EXPECT_TRUE(fs::is_regular_file(testRoot / "aclrtc_result.json"));
    EXPECT_EQ(std::distance(fs::directory_iterator(testRoot), fs::directory_iterator()), 4);

    const std::string replayScript = ReadTextFile(replayScriptPath);
    EXPECT_NE(replayScript.find("'/bin/echo' 'argument with space' 'quote'\"'\"'value'"), std::string::npos);

    struct stat replayScriptStatus {};
    ASSERT_EQ(stat(replayScriptPath.c_str(), &replayScriptStatus), 0);
    EXPECT_EQ(replayScriptStatus.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO), S_IRWXU);
    EXPECT_EQ(ExecuteReplayScriptAndWait(replayScriptPath), 0);

    const nlohmann::json result = nlohmann::json::parse(ReadTextFile(testRoot / "aclrtc_result.json"));
    EXPECT_EQ(result.at("specialization_session_id"), "session_1");
    EXPECT_EQ(result.at("output_path"), "/tmp/kernel.elf");
    EXPECT_TRUE(result.at("output_published"));
    EXPECT_FALSE(result.contains("result"));

    boost::system::error_code ignoredError;
    fs::remove_all(testRoot, ignoredError);
}

} // namespace
