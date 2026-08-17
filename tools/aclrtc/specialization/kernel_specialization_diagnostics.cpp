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
#include <fcntl.h>
#include <fstream>
#include <sstream>
#include <sys/stat.h>
#include <unistd.h>

namespace ascendc {
namespace aclrtc {
namespace {
namespace fs = boost::filesystem;

std::string QuoteShellArgument(const std::string& argument)
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

bool WriteCompleteReplayScriptHeader(int replayScriptFileDescriptor, const fs::path& replayScriptPath,
                                     const fs::path& compilationWorkingDirectoryPath) noexcept
{
    const std::string replayScriptHeader =
        "#!/bin/sh\nset -eu\ncd " + QuoteShellArgument(compilationWorkingDirectoryPath.string()) + "\n";
    size_t writtenByteCount = 0U;
    while (writtenByteCount < replayScriptHeader.size()) {
        const ssize_t writeResult = write(replayScriptFileDescriptor,
                                          replayScriptHeader.data() + writtenByteCount,
                                          replayScriptHeader.size() - writtenByteCount);
        if (writeResult > 0) {
            writtenByteCount += static_cast<size_t>(writeResult);
            continue;
        }
        if (writeResult < 0 && errno == EINTR) {
            continue;
        }
        const int writeError = errno;
        ACLRTC_LOG_WARNING("Failed to write replay script header: path=%s errno=%d message=%s",
                           replayScriptPath.c_str(), writeError, std::strerror(writeError));
        return false;
    }
    return true;
}

bool CreateExecutableReplayScript(const fs::path& replayScriptPath,
                                  const fs::path& compilationWorkingDirectoryPath) noexcept
{
    constexpr mode_t REPLAY_SCRIPT_PERMISSION = S_IRUSR | S_IWUSR | S_IXUSR;
    const int replayScriptFileDescriptor = open(
        replayScriptPath.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC | O_NOFOLLOW, REPLAY_SCRIPT_PERMISSION);
    if (replayScriptFileDescriptor < 0) {
        const int openError = errno;
        ACLRTC_LOG_WARNING("Failed to create replay script: path=%s errno=%d message=%s",
                           replayScriptPath.c_str(), openError, std::strerror(openError));
        return false;
    }

    bool replayScriptCreated = WriteCompleteReplayScriptHeader(
        replayScriptFileDescriptor, replayScriptPath, compilationWorkingDirectoryPath);
    if (replayScriptCreated && fchmod(replayScriptFileDescriptor, REPLAY_SCRIPT_PERMISSION) != 0) {
        const int permissionError = errno;
        ACLRTC_LOG_WARNING("Failed to make replay script executable: path=%s errno=%d message=%s",
                           replayScriptPath.c_str(), permissionError, std::strerror(permissionError));
        replayScriptCreated = false;
    }
    if (close(replayScriptFileDescriptor) != 0) {
        const int closeError = errno;
        ACLRTC_LOG_WARNING("Failed to close replay script: path=%s errno=%d message=%s",
                           replayScriptPath.c_str(), closeError, std::strerror(closeError));
        replayScriptCreated = false;
    }
    if (!replayScriptCreated && unlink(replayScriptPath.c_str()) != 0) {
        const int removalError = errno;
        ACLRTC_LOG_WARNING("Failed to remove incomplete replay script: path=%s errno=%d message=%s",
                           replayScriptPath.c_str(), removalError, std::strerror(removalError));
    }
    return replayScriptCreated;
}
} // namespace

KernelSpecializationDiagnostics::KernelSpecializationDiagnostics(
    std::string specializationSessionId, DiagnosticCollectionMode collectionMode)
    : specializationSessionId_(std::move(specializationSessionId)),
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

void KernelSpecializationDiagnostics::WriteManifestSnapshot(const nlohmann::json& resourceManifest) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        const fs::path manifestPath = diagnosticDirectoryPath_ / "manifest.json";
        std::ofstream manifestStream(manifestPath, std::ios::out | std::ios::trunc);
        if (!manifestStream) {
            const int openError = errno;
            ACLRTC_LOG_WARNING("Failed to open specialization manifest for writing: path=%s errno=%d message=%s",
                               manifestPath.c_str(), openError, std::strerror(openError));
            return;
        }
        manifestStream << resourceManifest.dump(2) << '\n';
        if (!manifestStream) {
            const int writeError = errno;
            ACLRTC_LOG_WARNING("Failed to write specialization manifest: path=%s errno=%d message=%s",
                               manifestPath.c_str(), writeError, std::strerror(writeError));
        }
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
        AppendDiagnosticText(diagnosticDirectoryPath_ / "compile.log",
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
        std::string replayLine = QuoteShellArgument(compilationCommand.executablePath.string());
        for (const std::string& argument : compilationCommand.commandArguments) {
            replayLine += " " + QuoteShellArgument(argument);
        }
        replayLine += '\n';
        AppendDiagnosticText(diagnosticDirectoryPath_ / "replay.sh", replayLine);
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
        AppendDiagnosticText(diagnosticDirectoryPath_ / "compile.log", resultText.str());
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record result for command %s: path=%s",
                           compilationCommand.diagnosticLabel.c_str(),
                           (diagnosticDirectoryPath_ / "compile.log").c_str());
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
        std::ofstream resultStream(resultPath, std::ios::out | std::ios::trunc);
        if (!resultStream) {
            const int openError = errno;
            ACLRTC_LOG_WARNING("Failed to open specialization result for writing: path=%s errno=%d message=%s",
                               resultPath.c_str(), openError, std::strerror(openError));
            return;
        }
        resultStream << resultJson.dump(2) << '\n';
        if (!resultStream) {
            const int writeError = errno;
            ACLRTC_LOG_WARNING("Failed to write specialization result: path=%s errno=%d message=%s",
                               resultPath.c_str(), writeError, std::strerror(writeError));
        }
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to record specialization result: path=%s",
                           (diagnosticDirectoryPath_ / "result.json").c_str());
    }
}

void KernelSpecializationDiagnostics::AppendDiagnosticText(
    const fs::path& filePath, const std::string& text) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        std::ofstream diagnosticStream(filePath, std::ios::out | std::ios::app);
        if (!diagnosticStream) {
            const int openError = errno;
            ACLRTC_LOG_WARNING("Failed to open diagnostic file for append: path=%s errno=%d message=%s",
                               filePath.c_str(), openError, std::strerror(openError));
            return;
        }
        diagnosticStream << text;
        if (!diagnosticStream) {
            const int writeError = errno;
            ACLRTC_LOG_WARNING("Failed to append diagnostic file: path=%s errno=%d message=%s",
                               filePath.c_str(), writeError, std::strerror(writeError));
        }
    } catch (...) {
        ACLRTC_LOG_WARNING("Failed to append diagnostic file: path=%s", filePath.c_str());
    }
}

} // namespace aclrtc
} // namespace ascendc
