/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "compilation_command_executor.h"

#include "kernel_specialization_diagnostics.h"
#include "kernel_specialization_log.h"

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <exception>
#include <fcntl.h>
#include <new>
#include <poll.h>
#include <spawn.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

extern char** environ;

namespace ascendc {
namespace aclrtc {
namespace {
using SteadyClock = std::chrono::steady_clock;

class ScopedFileDescriptor final {
public:
    explicit ScopedFileDescriptor(int fileDescriptor) noexcept : fileDescriptor_(fileDescriptor) {}

    ~ScopedFileDescriptor() noexcept
    {
        Close();
    }

    ScopedFileDescriptor(const ScopedFileDescriptor&) = delete;
    ScopedFileDescriptor& operator=(const ScopedFileDescriptor&) = delete;

    int GetFileDescriptor() const noexcept
    {
        return fileDescriptor_;
    }

    void Close() noexcept
    {
        if (fileDescriptor_ < 0) {
            return;
        }
        const int closingFileDescriptor = fileDescriptor_;
        fileDescriptor_ = -1;
        if (close(closingFileDescriptor) != 0) {
            ACLRTC_LOG_WARNING("Failed to close command pipe descriptor: fd=%d errno=%d", closingFileDescriptor,
                               errno);
        }
    }

private:
    int fileDescriptor_ {-1};
};

void CloseUnownedFileDescriptor(int fileDescriptor) noexcept
{
    if (fileDescriptor >= 0 && close(fileDescriptor) != 0) {
        ACLRTC_LOG_WARNING("Failed to close command pipe descriptor: fd=%d errno=%d", fileDescriptor, errno);
    }
}

class SpawnConfiguration final {
public:
    SpawnConfiguration() = default;

    ~SpawnConfiguration() noexcept
    {
        if (spawnAttributesInitialized_) {
            const int destroyResult = posix_spawnattr_destroy(&spawnAttributes_);
            if (destroyResult != 0) {
                ACLRTC_LOG_WARNING("Failed to destroy spawn attributes: error=%d", destroyResult);
            }
        }
        if (fileActionsInitialized_) {
            const int destroyResult = posix_spawn_file_actions_destroy(&fileActions_);
            if (destroyResult != 0) {
                ACLRTC_LOG_WARNING("Failed to destroy spawn file actions: error=%d", destroyResult);
            }
        }
    }

    SpawnConfiguration(const SpawnConfiguration&) = delete;
    SpawnConfiguration& operator=(const SpawnConfiguration&) = delete;

    bool Initialize(int outputReadFileDescriptor, int outputWriteFileDescriptor) noexcept
    {
        int setupResult = posix_spawn_file_actions_init(&fileActions_);
        if (setupResult != 0) {
            ACLRTC_LOG_ERROR("Failed to initialize spawn file actions: error=%d", setupResult);
            return false;
        }
        fileActionsInitialized_ = true;

        setupResult = posix_spawn_file_actions_addclose(&fileActions_, outputReadFileDescriptor);
        if (setupResult == 0) {
            setupResult = posix_spawn_file_actions_adddup2(&fileActions_, outputWriteFileDescriptor, STDOUT_FILENO);
        }
        if (setupResult == 0) {
            setupResult = posix_spawn_file_actions_adddup2(&fileActions_, outputWriteFileDescriptor, STDERR_FILENO);
        }
        if (setupResult == 0) {
            setupResult = posix_spawn_file_actions_addclose(&fileActions_, outputWriteFileDescriptor);
        }
        if (setupResult != 0) {
            ACLRTC_LOG_ERROR("Failed to configure spawn file actions: error=%d", setupResult);
            return false;
        }

        setupResult = posix_spawnattr_init(&spawnAttributes_);
        if (setupResult != 0) {
            ACLRTC_LOG_ERROR("Failed to initialize spawn attributes: error=%d", setupResult);
            return false;
        }
        spawnAttributesInitialized_ = true;
        setupResult = posix_spawnattr_setflags(&spawnAttributes_, POSIX_SPAWN_SETPGROUP);
        if (setupResult == 0) {
            setupResult = posix_spawnattr_setpgroup(&spawnAttributes_, 0);
        }
        if (setupResult != 0) {
            ACLRTC_LOG_ERROR("Failed to configure spawn process group: error=%d", setupResult);
            return false;
        }
        return true;
    }

    posix_spawn_file_actions_t* GetFileActions() noexcept
    {
        return &fileActions_;
    }

    posix_spawnattr_t* GetSpawnAttributes() noexcept
    {
        return &spawnAttributes_;
    }

private:
    posix_spawn_file_actions_t fileActions_ {};
    posix_spawnattr_t spawnAttributes_ {};
    bool fileActionsInitialized_ {false};
    bool spawnAttributesInitialized_ {false};
};

bool SignalProcessGroup(pid_t childProcessId, int signalNumber) noexcept
{
    if (kill(-childProcessId, signalNumber) == 0) {
        return true;
    }
    const int processGroupError = errno;
    if (processGroupError != ESRCH) {
        ACLRTC_LOG_WARNING("Failed to signal command process group: pid=%d signal=%d errno=%d",
                           static_cast<int>(childProcessId), signalNumber, processGroupError);
    }
    if (kill(childProcessId, signalNumber) == 0 || errno == ESRCH) {
        return processGroupError == ESRCH;
    }
    ACLRTC_LOG_WARNING("Failed to signal command process leader: pid=%d signal=%d errno=%d",
                       static_cast<int>(childProcessId), signalNumber, errno);
    return false;
}

bool TerminateAndReapProcessGroup(pid_t childProcessId, std::chrono::milliseconds terminationGracePeriod,
                                  int& childStatus) noexcept
{
    bool cleanupSucceeded = SignalProcessGroup(childProcessId, SIGTERM);
    // Keep the leader unreaped during the grace period so its PID cannot be reused as another process group ID.
    std::this_thread::sleep_for(terminationGracePeriod);
    cleanupSucceeded = SignalProcessGroup(childProcessId, SIGKILL) && cleanupSucceeded;

    pid_t waitResult;
    do {
        waitResult = waitpid(childProcessId, &childStatus, 0);
    } while (waitResult < 0 && errno == EINTR);
    if (waitResult != childProcessId) {
        ACLRTC_LOG_ERROR("Failed to reap command process: pid=%d errno=%d", static_cast<int>(childProcessId), errno);
        return false;
    }
    return cleanupSucceeded;
}

class RunningProcessGroup final {
public:
    RunningProcessGroup(pid_t childProcessId, std::chrono::milliseconds terminationGracePeriod) noexcept
        : childProcessId_(childProcessId), terminationGracePeriod_(terminationGracePeriod)
    {}

    ~RunningProcessGroup() noexcept
    {
        if (!running_) {
            return;
        }
        ACLRTC_LOG_WARNING("Cleaning command process group during exception unwinding: pid=%d",
                           static_cast<int>(childProcessId_));
        int ignoredChildStatus = 0;
        TerminateAndReapProcessGroup(childProcessId_, terminationGracePeriod_, ignoredChildStatus);
    }

    RunningProcessGroup(const RunningProcessGroup&) = delete;
    RunningProcessGroup& operator=(const RunningProcessGroup&) = delete;

    void MarkReaped() noexcept
    {
        running_ = false;
    }

    bool TerminateAndReap(int& childStatus) noexcept
    {
        if (!running_) {
            return true;
        }
        const bool cleanupSucceeded =
            TerminateAndReapProcessGroup(childProcessId_, terminationGracePeriod_, childStatus);
        running_ = false;
        return cleanupSucceeded;
    }

private:
    pid_t childProcessId_ {-1};
    std::chrono::milliseconds terminationGracePeriod_ {0};
    bool running_ {true};
};

bool AppendAvailableProcessOutput(int outputFileDescriptor, uint64_t capturedOutputByteLimit,
                                  CompilationProcessResult& processResult)
{
    char outputBuffer[4096];
    while (true) {
        const ssize_t readByteCount = read(outputFileDescriptor, outputBuffer, sizeof(outputBuffer));
        if (readByteCount > 0) {
            const uint64_t capturedOutputByteCount =
                static_cast<uint64_t>(processResult.capturedOutput.size());
            const uint64_t availableCapacity = capturedOutputByteLimit > capturedOutputByteCount
                ? capturedOutputByteLimit - capturedOutputByteCount
                : 0U;
            const size_t appendByteCount = static_cast<size_t>(
                std::min(static_cast<uint64_t>(readByteCount), availableCapacity));
            processResult.capturedOutput.append(outputBuffer, appendByteCount);
            processResult.capturedOutputTruncated =
                processResult.capturedOutputTruncated || appendByteCount < static_cast<size_t>(readByteCount);
            continue;
        }
        if (readByteCount == 0) {
            return true;
        }
        if (errno == EINTR) {
            continue;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return true;
        }
        ACLRTC_LOG_ERROR("Failed to read command output: errno=%d", errno);
        return false;
    }
}

bool ConfigureOutputPipe(int outputPipe[2])
{
    if (pipe(outputPipe) != 0) {
        ACLRTC_LOG_ERROR("Failed to create command output pipe: errno=%d", errno);
        return false;
    }
    const int readFlags = fcntl(outputPipe[0], F_GETFL, 0);
    if (readFlags < 0) {
        ACLRTC_LOG_ERROR("Failed to read command pipe flags: errno=%d", errno);
        CloseUnownedFileDescriptor(outputPipe[0]);
        CloseUnownedFileDescriptor(outputPipe[1]);
        return false;
    }
    if (fcntl(outputPipe[0], F_SETFL, readFlags | O_NONBLOCK) != 0) {
        ACLRTC_LOG_ERROR("Failed to make command pipe nonblocking: errno=%d", errno);
        CloseUnownedFileDescriptor(outputPipe[0]);
        CloseUnownedFileDescriptor(outputPipe[1]);
        return false;
    }
    if (fcntl(outputPipe[0], F_SETFD, FD_CLOEXEC) != 0 ||
        fcntl(outputPipe[1], F_SETFD, FD_CLOEXEC) != 0) {
        ACLRTC_LOG_ERROR("Failed to make command pipe close-on-exec: errno=%d", errno);
        CloseUnownedFileDescriptor(outputPipe[0]);
        CloseUnownedFileDescriptor(outputPipe[1]);
        return false;
    }
    return true;
}

bool HasEmbeddedNull(const std::string& text) noexcept
{
    return text.find('\0') != std::string::npos;
}

bool CreateSpawnArguments(const CompilationCommand& compilationCommand, std::vector<std::string>& ownedArguments,
                          std::vector<char*>& spawnArguments)
{
    const std::string executablePath = compilationCommand.executablePath.string();
    if (HasEmbeddedNull(executablePath)) {
        ACLRTC_LOG_ERROR("Command %s has an executable path containing an embedded NUL",
                         compilationCommand.diagnosticLabel.c_str());
        return false;
    }
    for (const std::string& argument : compilationCommand.commandArguments) {
        if (HasEmbeddedNull(argument)) {
            ACLRTC_LOG_ERROR("Command %s has an argv item containing an embedded NUL",
                             compilationCommand.diagnosticLabel.c_str());
            return false;
        }
    }

    ownedArguments.reserve(compilationCommand.commandArguments.size() + 1U);
    ownedArguments.emplace_back(executablePath);
    ownedArguments.insert(ownedArguments.end(), compilationCommand.commandArguments.begin(),
                          compilationCommand.commandArguments.end());
    spawnArguments.reserve(ownedArguments.size() + 1U);
    for (std::string& argument : ownedArguments) {
        spawnArguments.push_back(argument.data());
    }
    spawnArguments.push_back(nullptr);
    return true;
}
} // namespace

CompilationCommandExecutionLimits CompilationCommandExecutionLimits::ProductionDefaults() noexcept
{
    return {std::chrono::minutes(5), std::chrono::minutes(1), std::chrono::seconds(5), 4U * 1024U * 1024U};
}

CompilationCommandExecutor::CompilationCommandExecutor(
    KernelSpecializationDiagnostics& specializationDiagnostics, CompilationCommandExecutionLimits executionLimits)
    : specializationDiagnostics_(specializationDiagnostics), executionLimits_(executionLimits)
{}

aclError CompilationCommandExecutor::ExecuteCompilationPlan(const KernelCompilationPlan& compilationPlan) const
{
    try {
        for (const CompilationCommand& compilationCommand : compilationPlan.compilationCommands) {
            ACLRTC_LOG_INFO("Executing command %s with %s", compilationCommand.diagnosticLabel.c_str(),
                            compilationCommand.executablePath.c_str());
            specializationDiagnostics_.RecordCompilationCommand(compilationCommand);
            CompilationProcessResult processResult;
            const aclError executionResult = ExecuteCompilationCommand(compilationCommand, processResult);
            specializationDiagnostics_.RecordCompilationProcessResult(compilationCommand, processResult);
            if (executionResult != ACLRTC_SUCCESS) {
                constexpr size_t MAX_LOGGED_PROCESS_OUTPUT_BYTES = 4096U;
                const int loggedOutputByteCount = static_cast<int>(
                    std::min(processResult.capturedOutput.size(), MAX_LOGGED_PROCESS_OUTPUT_BYTES));
                ACLRTC_LOG_ERROR(
                    "Command %s failed: termination=%u code=%d elapsed_ms=%lld output=%.*s",
                    compilationCommand.diagnosticLabel.c_str(), static_cast<uint32_t>(processResult.termination),
                    static_cast<int>(processResult.terminationCode),
                    static_cast<long long>(processResult.elapsedTime.count()), loggedOutputByteCount,
                    processResult.capturedOutput.c_str());
                return executionResult;
            }
        }
        return ACLRTC_SUCCESS;
    } catch (const std::bad_alloc&) {
        ACLRTC_LOG_ERROR("Out of memory while executing kernel compilation commands");
        return ACLRTC_ERROR_OUT_OF_MEMORY;
    } catch (const std::exception& exception) {
        ACLRTC_LOG_ERROR("Kernel compilation command execution failed with exception: %s", exception.what());
        return ACLRTC_ERROR_FAILURE;
    } catch (...) {
        ACLRTC_LOG_ERROR("Kernel compilation command execution failed with an unknown exception");
        return ACLRTC_ERROR_FAILURE;
    }
}

aclError CompilationCommandExecutor::ExecuteCompilationCommand(
    const CompilationCommand& compilationCommand, CompilationProcessResult& processResult) const
{
    std::vector<std::string> ownedArguments;
    std::vector<char*> spawnArguments;
    if (!CreateSpawnArguments(compilationCommand, ownedArguments, spawnArguments)) {
        return ACLRTC_ERROR_FAILURE;
    }

    int outputPipe[2] {-1, -1};
    if (!ConfigureOutputPipe(outputPipe)) {
        return ACLRTC_ERROR_FAILURE;
    }
    ScopedFileDescriptor outputReadFileDescriptor(outputPipe[0]);
    ScopedFileDescriptor outputWriteFileDescriptor(outputPipe[1]);

    pid_t childProcessId = -1;
    const SteadyClock::time_point startedAt = SteadyClock::now();
    int spawnResult;
    {
        SpawnConfiguration spawnConfiguration;
        if (!spawnConfiguration.Initialize(outputReadFileDescriptor.GetFileDescriptor(),
                                           outputWriteFileDescriptor.GetFileDescriptor())) {
            return ACLRTC_ERROR_FAILURE;
        }
        spawnResult = posix_spawn(&childProcessId, ownedArguments.front().c_str(),
                                  spawnConfiguration.GetFileActions(), spawnConfiguration.GetSpawnAttributes(),
                                  spawnArguments.data(), environ);
    }
    outputWriteFileDescriptor.Close();
    if (spawnResult != 0) {
        ACLRTC_LOG_ERROR("Failed to spawn command %s: error=%d", compilationCommand.diagnosticLabel.c_str(),
                         spawnResult);
        return ACLRTC_ERROR_FAILURE;
    }
    RunningProcessGroup runningProcessGroup(childProcessId, executionLimits_.terminationGracePeriod);

    const SteadyClock::time_point executionDeadline =
        startedAt + GetCommandExecutionTimeout(compilationCommand.commandKind);
    int childStatus = 0;
    bool childExited = false;
    bool infrastructureFailure = false;
    while (!childExited) {
        if (!AppendAvailableProcessOutput(outputReadFileDescriptor.GetFileDescriptor(),
                                          executionLimits_.capturedOutputByteLimit, processResult)) {
            infrastructureFailure = true;
            break;
        }
        const pid_t waitResult = waitpid(childProcessId, &childStatus, WNOHANG);
        if (waitResult == childProcessId) {
            runningProcessGroup.MarkReaped();
            childExited = true;
            break;
        }
        if (waitResult < 0) {
            if (errno == EINTR) {
                continue;
            }
            ACLRTC_LOG_ERROR("Failed to inspect command process: pid=%d errno=%d",
                             static_cast<int>(childProcessId), errno);
            infrastructureFailure = true;
            break;
        }
        if (SteadyClock::now() >= executionDeadline) {
            processResult.termination = CompilationProcessTermination::TimedOut;
            runningProcessGroup.TerminateAndReap(childStatus);
            childExited = true;
            break;
        }

        const auto remainingTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(executionDeadline - SteadyClock::now());
        const int pollTimeout = static_cast<int>(std::max<int64_t>(1, std::min<int64_t>(remainingTime.count(), 50)));
        pollfd outputPollDescriptor {outputReadFileDescriptor.GetFileDescriptor(), POLLIN, 0};
        const int pollResult = poll(&outputPollDescriptor, 1U, pollTimeout);
        if (pollResult < 0 && errno != EINTR) {
            ACLRTC_LOG_ERROR("Failed to poll command output: errno=%d", errno);
            infrastructureFailure = true;
            break;
        }
    }

    if (infrastructureFailure) {
        runningProcessGroup.TerminateAndReap(childStatus);
    }
    if (!AppendAvailableProcessOutput(outputReadFileDescriptor.GetFileDescriptor(),
                                      executionLimits_.capturedOutputByteLimit, processResult)) {
        infrastructureFailure = true;
    }
    outputReadFileDescriptor.Close();
    processResult.elapsedTime =
        std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - startedAt);
    if (infrastructureFailure) {
        processResult.termination = CompilationProcessTermination::InfrastructureFailure;
        return ACLRTC_ERROR_FAILURE;
    }

    if (processResult.termination != CompilationProcessTermination::TimedOut) {
        if (WIFEXITED(childStatus)) {
            processResult.termination = CompilationProcessTermination::Exited;
            processResult.terminationCode = static_cast<int32_t>(WEXITSTATUS(childStatus));
        } else if (WIFSIGNALED(childStatus)) {
            processResult.termination = CompilationProcessTermination::Signaled;
            processResult.terminationCode = static_cast<int32_t>(WTERMSIG(childStatus));
        } else {
            processResult.termination = CompilationProcessTermination::InfrastructureFailure;
            ACLRTC_LOG_ERROR("Command process returned an unsupported wait status: pid=%d status=%d",
                             static_cast<int>(childProcessId), childStatus);
            return ACLRTC_ERROR_FAILURE;
        }
    }

    const bool commandSucceeded = processResult.termination == CompilationProcessTermination::Exited &&
                                  processResult.terminationCode == 0;
    if (commandSucceeded) {
        return ACLRTC_SUCCESS;
    }
    return compilationCommand.commandKind == CompilationCommandKind::Link ? ACLRTC_ERROR_LINKING
                                                                           : ACLRTC_ERROR_COMPILATION;
}

std::chrono::milliseconds CompilationCommandExecutor::GetCommandExecutionTimeout(
    CompilationCommandKind commandKind) const noexcept
{
    return commandKind == CompilationCommandKind::Compile ? executionLimits_.compileCommandTimeout
                                                           : executionLimits_.objectCopyAndLinkCommandTimeout;
}

} // namespace aclrtc
} // namespace ascendc
