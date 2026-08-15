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

const char* GetTerminationName(CompilationProcessTermination termination)
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
        diagnosticDirectoryPath_ = fs::current_path() / "kernel_meta" / std::to_string(getpid()) /
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
        AppendDiagnosticText(replayScriptPath, "#!/bin/sh\nset -eu\n");
        if (chmod(replayScriptPath.c_str(), 0700) != 0) {
            const int chmodError = errno;
            ACLRTC_LOG_WARNING("Failed to make replay script executable: path=%s errno=%d message=%s",
                               replayScriptPath.c_str(), chmodError, std::strerror(chmodError));
        }
    } catch (...) {
        diagnosticsEnabled_ = false;
        ACLRTC_LOG_WARNING("Failed to initialize specialization diagnostics: session=%s path=%s",
                           specializationSessionId_.c_str(), diagnosticDirectoryPath_.c_str());
    }
}

void KernelSpecializationDiagnostics::RecordManifest(const nlohmann::json& resourceManifest) noexcept
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

void KernelSpecializationDiagnostics::RecordWorktree(const fs::path& resourceWorktreePath) noexcept
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

void KernelSpecializationDiagnostics::RecordCommand(const CompilationCommand& compilationCommand) noexcept
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

void KernelSpecializationDiagnostics::RecordCommandResult(
    const CompilationCommand& compilationCommand, const CompilationProcessResult& processResult) noexcept
{
    if (!diagnosticsEnabled_) {
        return;
    }
    try {
        std::ostringstream resultText;
        resultText << '[' << compilationCommand.diagnosticLabel << "] termination="
                   << GetTerminationName(processResult.termination) << " code=" << processResult.terminationCode
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

void KernelSpecializationDiagnostics::RecordFinalResult(
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
