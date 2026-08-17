/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_specialization_diagnostics.h"

#include "kernel_specialization_log.h"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstring>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;

std::string QuoteArgumentForReplayScript(const std::string& argument)
{
    std::string quotedArgument("'");
    for (char character : argument) {
        if (character == '\'') {
            quotedArgument += "'\"'\"'";
        } else {
            quotedArgument += character;
        }
    }
    quotedArgument += '\'';
    return quotedArgument;
}

const char* GetProcessTerminationName(CompilationProcessTermination termination)
{
    switch (termination) {
        case CompilationProcessTermination::InfrastructureFailure:
            return "infrastructure_failure";
        case CompilationProcessTermination::Exited:
            return "exited";
        case CompilationProcessTermination::Signaled:
            return "signaled";
        case CompilationProcessTermination::TimedOut:
            return "timed_out";
        default:
            return "unknown";
    }
}

bool WriteDiagnosticTextFile(const fs::path& filePath, const std::string& text, std::ios::openmode writeMode)
{
    std::ofstream outputStream(filePath, std::ios::out | writeMode);
    if (!outputStream) {
        const int openError = errno;
        ACLRTC_LOG_WARNING("Failed to open diagnostic file for writing: path=%s errno=%d message=%s",
                           filePath.c_str(), openError, std::strerror(openError));
        return false;
    }

    outputStream << text;
    outputStream.close();
    if (outputStream) {
        return true;
    }
    const int writeError = errno;
    ACLRTC_LOG_WARNING("Failed to write diagnostic file: path=%s errno=%d message=%s",
                       filePath.c_str(), writeError, std::strerror(writeError));
    return false;
}

void RemoveIncompleteReplayScript(const fs::path& replayScriptPath) noexcept
{
    boost::system::error_code removalError;
    fs::remove(replayScriptPath, removalError);
    if (removalError) {
        ACLRTC_LOG_WARNING("Failed to remove incomplete replay script: path=%s error=%d message=%s",
                           replayScriptPath.c_str(), removalError.value(), removalError.message().c_str());
    }
}

bool CreateExecutableReplayScript(const fs::path& replayScriptPath,
                                  const fs::path& compilationWorkingDirectoryPath)
{
    const std::string replayScriptHeader =
        "#!/bin/sh\nset -eu\ncd " + QuoteArgumentForReplayScript(compilationWorkingDirectoryPath.string()) + "\n";
    if (!WriteDiagnosticTextFile(replayScriptPath, replayScriptHeader, std::ios::trunc)) {
        RemoveIncompleteReplayScript(replayScriptPath);
        return false;
    }

    constexpr mode_t REPLAY_SCRIPT_PERMISSION = S_IRUSR | S_IWUSR | S_IXUSR;
    if (chmod(replayScriptPath.c_str(), REPLAY_SCRIPT_PERMISSION) == 0) {
        return true;
    }

    const int permissionError = errno;
    ACLRTC_LOG_WARNING("Failed to make replay script executable: path=%s errno=%d message=%s",
                       replayScriptPath.c_str(), permissionError, std::strerror(permissionError));
    RemoveIncompleteReplayScript(replayScriptPath);
    return false;
}
} // namespace

KernelSpecializationDiagnostics::KernelSpecializationDiagnostics(
    std::string specializationSessionId, DiagnosticCollectionMode collectionMode)
    : specializationSessionId_(std::move(specializationSessionId)),
      diagnosticsRequested_(collectionMode == DiagnosticCollectionMode::Enabled),
      diagnosticsEnabled_(collectionMode == DiagnosticCollectionMode::Enabled)
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        const fs::path compilationWorkingDirectoryPath = fs::current_path();
        diagnosticDirectoryPath_ = compilationWorkingDirectoryPath / "kernel_meta" / std::to_string(getpid()) /
                                   ("aclrtc_" + specializationSessionId_);
        boost::system::error_code directoryError;
        fs::create_directories(diagnosticDirectoryPath_, directoryError);
        if (directoryError) {
            diagnosticsEnabled_ = false;
            ACLRTC_LOG_WARNING("Failed to create diagnostic directory: path=%s error=%d message=%s",
                               diagnosticDirectoryPath_.c_str(), directoryError.value(),
                               directoryError.message().c_str());
            return;
        }
        const fs::path replayScriptPath = diagnosticDirectoryPath_ / "replay.sh";
        if (!CreateExecutableReplayScript(replayScriptPath, compilationWorkingDirectoryPath)) {
            diagnosticsEnabled_ = false;
        }
    } catch (...) {
        diagnosticsEnabled_ = false;
        ACLRTC_LOG_WARNING("Failed to initialize specialization diagnostics: session=%s path=%s",
                           specializationSessionId_.c_str(), diagnosticDirectoryPath_.c_str());
    }
}

bool KernelSpecializationDiagnostics::GetResourceWorktreeRetentionPath(
    fs::path& resourceWorktreeRetentionPath) const noexcept
{
    if (!diagnosticsEnabled_) {
        return false;
    }
    try {
        resourceWorktreeRetentionPath = diagnosticDirectoryPath_ / "resource_worktree";
        return true;
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to construct retained resource worktree path: diagnostic_directory=%s",
                           diagnosticDirectoryPath_.c_str());
        return false;
    }
}

void KernelSpecializationDiagnostics::WriteManifestSnapshot(const nlohmann::json& resourceManifest) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        const fs::path manifestPath = diagnosticDirectoryPath_ / "manifest.json";
        WriteDiagnosticTextFile(manifestPath, resourceManifest.dump(2) + '\n', std::ios::trunc);
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record specialization manifest: path=%s",
                           (diagnosticDirectoryPath_ / "manifest.json").c_str());
    }
}

void KernelSpecializationDiagnostics::RecordWorktreePath(const fs::path& resourceWorktreePath) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        AppendTextToDiagnosticFile(diagnosticDirectoryPath_ / "compile.log",
                                   "resource_worktree=" + resourceWorktreePath.string() + "\n");
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record resource worktree path: path=%s",
                           (diagnosticDirectoryPath_ / "compile.log").c_str());
    }
}

void KernelSpecializationDiagnostics::AppendCommandToReplayScript(
    const CompilationCommand& compilationCommand) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        std::string replayLine = QuoteArgumentForReplayScript(compilationCommand.executablePath.string());
        for (const std::string& argument : compilationCommand.commandArguments) {
            replayLine += " " + QuoteArgumentForReplayScript(argument);
        }
        replayLine += '\n';
        AppendTextToDiagnosticFile(diagnosticDirectoryPath_ / "replay.sh", replayLine);
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record command %s: path=%s",
                           compilationCommand.diagnosticLabel.c_str(),
                           (diagnosticDirectoryPath_ / "replay.sh").c_str());
    }
}

void KernelSpecializationDiagnostics::AppendCommandResultToCompilationLog(
    const CompilationCommand& compilationCommand, const CompilationProcessResult& processResult) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        std::ostringstream resultText;
        resultText << '[' << compilationCommand.diagnosticLabel << "] termination="
                   << GetProcessTerminationName(processResult.termination) << " code="
                   << processResult.terminationCode
                   << " elapsed_ms=" << processResult.elapsedTime.count()
                   << " output_truncated=" << (processResult.capturedOutputTruncated ? "true" : "false") << '\n'
                   << processResult.capturedOutput;
        if (!processResult.capturedOutput.empty() && processResult.capturedOutput.back() != '\n') {
            resultText << '\n';
        }
        AppendTextToDiagnosticFile(diagnosticDirectoryPath_ / "compile.log", resultText.str());
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record result for command %s: path=%s",
                           compilationCommand.diagnosticLabel.c_str(),
                           (diagnosticDirectoryPath_ / "compile.log").c_str());
    }
}

void KernelSpecializationDiagnostics::LogCommandFailureRecoveryHint() const noexcept
{
    if (!diagnosticsEnabled_) {
        if (diagnosticsRequested_) {
            ACLRTC_LOG_INFO(
                "Compilation diagnostics were requested but could not be initialized; fix the preceding diagnostic "
                "warning and retry");
            return;
        }
        ACLRTC_LOG_INFO(
            "Set ASCEND_OP_COMPILE_SAVE_KERNEL_META=1 and retry to save compile.log, replay.sh, and the resource "
            "worktree");
        return;
    }
    try {
        const fs::path compilationLogPath = diagnosticDirectoryPath_ / "compile.log";
        const fs::path replayScriptPath = diagnosticDirectoryPath_ / "replay.sh";
        ACLRTC_LOG_INFO(
            "Compilation diagnostics saved: compile_log=%s replay_script=%s; inspect compile.log or run replay.sh "
            "to reproduce the failed command",
            compilationLogPath.c_str(), replayScriptPath.c_str());
    } catch (...) {
        ACLRTC_LOG_INFO(
            "Compilation diagnostics were enabled; inspect kernel_meta for compile.log and replay.sh");
    }
}

void KernelSpecializationDiagnostics::WriteSpecializationResult(
    const fs::path& outputElfPath, OutputPublicationStatus publicationStatus) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        nlohmann::json resultJson {
            {"specialization_session_id", specializationSessionId_},
            {"output_path", outputElfPath.string()},
            {"output_published", publicationStatus == OutputPublicationStatus::Published},
        };
        const fs::path resultPath = diagnosticDirectoryPath_ / "result.json";
        WriteDiagnosticTextFile(resultPath, resultJson.dump(2) + '\n', std::ios::trunc);
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record specialization result: path=%s",
                           (diagnosticDirectoryPath_ / "result.json").c_str());
    }
}

void KernelSpecializationDiagnostics::AppendTextToDiagnosticFile(
    const fs::path& filePath, const std::string& text) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        WriteDiagnosticTextFile(filePath, text, std::ios::app);
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to append diagnostic file: path=%s", filePath.c_str());
    }
}

} // namespace aclrtc
} // namespace ascendc
