/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "process_executor.h"

#include "ascendc_tool_log.h"

#include <algorithm>
#include <cerrno>
#include <csignal>
#include <cstring>
#include <exception>
#include <fcntl.h>
#include <map>
#include <new>
#include <poll.h>
#include <spawn.h>
#include <sys/wait.h>
#include <thread>
#include <unistd.h>

namespace ascendc {
namespace {
using SteadyClock = std::chrono::steady_clock;

constexpr uint64_t MAX_OUTPUT_READ_BYTES_PER_WAIT_ITERATION = 64U * 1024U;
constexpr int64_t MAX_OUTPUT_WAIT_INTERVAL_MILLISECONDS = 50;
constexpr int64_t MAX_PROCESS_CHECK_INTERVAL_MILLISECONDS = 5;

enum class OutputDrainStatus : uint32_t {
    NoDataAvailable,
    EndOfFile,
    ReadBudgetExhausted,
    Failed,
};

enum class ProcessLeaderState : uint32_t {
    Running,
    Exited,
    InspectionFailed,
};

class ScopedFileDescriptor final {
public:
    explicit ScopedFileDescriptor(int fileDescriptor = -1) noexcept : fileDescriptor_(fileDescriptor) {}

    ~ScopedFileDescriptor() noexcept { Close(); }

    ScopedFileDescriptor(const ScopedFileDescriptor&) = delete;
    ScopedFileDescriptor& operator=(const ScopedFileDescriptor&) = delete;

    int Get() const noexcept { return fileDescriptor_; }

    void TakeOwnership(int fileDescriptor) noexcept
    {
        Close();
        fileDescriptor_ = fileDescriptor;
    }

    void Close() noexcept
    {
        if (fileDescriptor_ < 0) {
            return;
        }
        const int closingFileDescriptor = fileDescriptor_;
        fileDescriptor_ = -1;
        if (close(closingFileDescriptor) != 0) {
            const int closeError = errno;
            ASCENDLOGW(
                "Failed to close process output descriptor: fd=%d errno=%d message=%s", closingFileDescriptor,
                closeError, std::strerror(closeError));
        }
    }

private:
    int fileDescriptor_;
};

void CloseFileDescriptorBestEffort(int fileDescriptor) noexcept
{
    if (fileDescriptor >= 0 && close(fileDescriptor) != 0) {
        const int closeError = errno;
        ASCENDLOGW(
            "Failed to close process output descriptor: fd=%d errno=%d message=%s", fileDescriptor, closeError,
            std::strerror(closeError));
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
                ASCENDLOGW(
                    "Failed to destroy process spawn attributes: error=%d message=%s", destroyResult,
                    std::strerror(destroyResult));
            }
        }
        if (fileActionsInitialized_) {
            const int destroyResult = posix_spawn_file_actions_destroy(&fileActions_);
            if (destroyResult != 0) {
                ASCENDLOGW(
                    "Failed to destroy process spawn file actions: error=%d message=%s", destroyResult,
                    std::strerror(destroyResult));
            }
        }
    }

    SpawnConfiguration(const SpawnConfiguration&) = delete;
    SpawnConfiguration& operator=(const SpawnConfiguration&) = delete;

    bool Initialize(ProcessOutputMode outputMode, int outputReadFileDescriptor, int outputWriteFileDescriptor) noexcept
    {
        int setupResult = posix_spawnattr_init(&spawnAttributes_);
        if (setupResult != 0) {
            ASCENDLOGE(
                "Failed to initialize process spawn attributes: error=%d message=%s", setupResult,
                std::strerror(setupResult));
            return false;
        }
        spawnAttributesInitialized_ = true;
        setupResult = posix_spawnattr_setflags(&spawnAttributes_, POSIX_SPAWN_SETPGROUP);
        if (setupResult == 0) {
            setupResult = posix_spawnattr_setpgroup(&spawnAttributes_, 0);
        }
        if (setupResult != 0) {
            ASCENDLOGE(
                "Failed to configure process spawn group: error=%d message=%s", setupResult,
                std::strerror(setupResult));
            return false;
        }
        if (outputMode == ProcessOutputMode::Inherit) {
            return true;
        }

        setupResult = posix_spawn_file_actions_init(&fileActions_);
        if (setupResult != 0) {
            ASCENDLOGE(
                "Failed to initialize process spawn file actions: error=%d message=%s", setupResult,
                std::strerror(setupResult));
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
            ASCENDLOGE(
                "Failed to configure captured process output: error=%d message=%s", setupResult,
                std::strerror(setupResult));
            return false;
        }
        return true;
    }

    const posix_spawn_file_actions_t* GetFileActions() const noexcept
    {
        return fileActionsInitialized_ ? &fileActions_ : nullptr;
    }

    const posix_spawnattr_t* GetSpawnAttributes() const noexcept { return &spawnAttributes_; }

private:
    posix_spawn_file_actions_t fileActions_{};
    posix_spawnattr_t spawnAttributes_{};
    bool fileActionsInitialized_{false};
    bool spawnAttributesInitialized_{false};
};

bool ContainsEmbeddedNull(const std::string& text) noexcept { return text.find('\0') != std::string::npos; }

bool IsEnvironmentVariableNameValid(const std::string& environmentVariableName) noexcept
{
    return !environmentVariableName.empty() && environmentVariableName.find('=') == std::string::npos &&
           !ContainsEmbeddedNull(environmentVariableName);
}

bool IsExecutionRequestWellFormed(const ProcessExecutionRequest& request) noexcept
{
    if (request.arguments.empty()) {
        ASCENDLOGE("Process arguments must not be empty");
        return false;
    }
    if (request.executionTimeout.count() <= 0) {
        ASCENDLOGE("Process execution timeout must be positive");
        return false;
    }
    if (request.terminationGracePeriod.count() < 0) {
        ASCENDLOGE("Process termination grace period must not be negative");
        return false;
    }
    if (request.outputMode != ProcessOutputMode::Inherit && request.outputMode != ProcessOutputMode::CaptureCombined) {
        ASCENDLOGE("Process output mode is invalid: value=%u", static_cast<uint32_t>(request.outputMode));
        return false;
    }
    for (size_t argumentIndex = 0U; argumentIndex < request.arguments.size(); ++argumentIndex) {
        if (ContainsEmbeddedNull(request.arguments[argumentIndex])) {
            ASCENDLOGE("Process argv[%zu] contains an embedded NUL", argumentIndex);
            return false;
        }
    }
    for (const std::pair<std::string, std::string>& environmentOverride : request.environmentOverrides) {
        if (!IsEnvironmentVariableNameValid(environmentOverride.first) ||
            ContainsEmbeddedNull(environmentOverride.second)) {
            ASCENDLOGE("Process environment override has an invalid name or an embedded NUL value");
            return false;
        }
    }
    for (const std::string& environmentVariableName : request.removedEnvironmentVariables) {
        if (!IsEnvironmentVariableNameValid(environmentVariableName)) {
            ASCENDLOGE("Process environment removal contains an invalid variable name");
            return false;
        }
    }
    return true;
}

bool CreateNonblockingCloseOnExecPipe(int outputPipe[2])
{
    if (pipe2(outputPipe, O_CLOEXEC) != 0) {
        const int pipeError = errno;
        ASCENDLOGE("Failed to create process output pipe: errno=%d message=%s", pipeError, std::strerror(pipeError));
        return false;
    }
    const int readFlags = fcntl(outputPipe[0], F_GETFL, 0);
    if (readFlags < 0 || fcntl(outputPipe[0], F_SETFL, readFlags | O_NONBLOCK) != 0) {
        const int flagError = errno;
        ASCENDLOGE(
            "Failed to make process output pipe nonblocking: fd=%d errno=%d message=%s", outputPipe[0], flagError,
            std::strerror(flagError));
        CloseFileDescriptorBestEffort(outputPipe[0]);
        CloseFileDescriptorBestEffort(outputPipe[1]);
        return false;
    }
    return true;
}

bool IsEnvironmentVariableRemoved(
    const std::string& environmentVariableName, const std::vector<std::string>& removedEnvironmentVariables)
{
    return std::find(removedEnvironmentVariables.begin(), removedEnvironmentVariables.end(), environmentVariableName) !=
           removedEnvironmentVariables.end();
}

bool BuildSpawnEnvironment(
    const ProcessExecutionRequest& request, std::vector<std::string>& environmentStorage,
    std::vector<char*>& spawnEnvironment)
{
    std::map<std::string, std::string> environmentByName;
    for (char** inheritedEnvironment = ::environ; inheritedEnvironment != nullptr && *inheritedEnvironment != nullptr;
         ++inheritedEnvironment) {
        const std::string environmentEntry(*inheritedEnvironment);
        const size_t separatorPosition = environmentEntry.find('=');
        if (separatorPosition == std::string::npos) {
            continue;
        }
        const std::string environmentVariableName = environmentEntry.substr(0U, separatorPosition);
        if (!IsEnvironmentVariableRemoved(environmentVariableName, request.removedEnvironmentVariables)) {
            environmentByName[environmentVariableName] = environmentEntry.substr(separatorPosition + 1U);
        }
    }
    for (const std::pair<std::string, std::string>& environmentOverride : request.environmentOverrides) {
        environmentByName[environmentOverride.first] = environmentOverride.second;
    }

    environmentStorage.reserve(environmentByName.size());
    for (const std::pair<const std::string, std::string>& environmentVariable : environmentByName) {
        environmentStorage.emplace_back(environmentVariable.first + "=" + environmentVariable.second);
    }
    spawnEnvironment.reserve(environmentStorage.size() + 1U);
    for (std::string& environmentEntry : environmentStorage) {
        spawnEnvironment.push_back(const_cast<char*>(environmentEntry.c_str()));
    }
    spawnEnvironment.push_back(nullptr);
    return true;
}

void BuildSpawnArguments(
    const std::vector<std::string>& arguments, std::vector<std::string>& argumentStorage, std::vector<char*>& spawnArgv)
{
    argumentStorage = arguments;
    spawnArgv.reserve(argumentStorage.size() + 1U);
    for (std::string& argument : argumentStorage) {
        spawnArgv.push_back(const_cast<char*>(argument.c_str()));
    }
    spawnArgv.push_back(nullptr);
}

void SignalProcessGroup(pid_t processLeaderId, int signalNumber) noexcept
{
    if (kill(-processLeaderId, signalNumber) == 0 || errno == ESRCH) {
        return;
    }
    const int processGroupError = errno;
    ASCENDLOGW(
        "Failed to signal process group: pid=%d signal=%d errno=%d message=%s", static_cast<int>(processLeaderId),
        signalNumber, processGroupError, std::strerror(processGroupError));
    if (kill(processLeaderId, signalNumber) != 0 && errno != ESRCH) {
        const int processLeaderError = errno;
        ASCENDLOGW(
            "Failed to signal process leader: pid=%d signal=%d errno=%d message=%s", static_cast<int>(processLeaderId),
            signalNumber, processLeaderError, std::strerror(processLeaderError));
    }
}

OutputDrainStatus DrainCapturedOutput(
    int outputFileDescriptor, uint64_t capturedOutputByteLimit, ProcessExecutionResult& processResult)
{
    char outputBuffer[4096];
    uint64_t readByteCountThisIteration = 0U;
    while (readByteCountThisIteration < MAX_OUTPUT_READ_BYTES_PER_WAIT_ITERATION) {
        const uint64_t remainingReadBudget = MAX_OUTPUT_READ_BYTES_PER_WAIT_ITERATION - readByteCountThisIteration;
        const size_t requestedReadByteCount =
            static_cast<size_t>(std::min<uint64_t>(sizeof(outputBuffer), remainingReadBudget));
        const ssize_t readByteCount = read(outputFileDescriptor, outputBuffer, requestedReadByteCount);
        if (readByteCount > 0) {
            readByteCountThisIteration += static_cast<uint64_t>(readByteCount);
            const uint64_t capturedOutputByteCount = static_cast<uint64_t>(processResult.capturedOutput.size());
            const uint64_t availableCapacity = capturedOutputByteLimit > capturedOutputByteCount ?
                                                   capturedOutputByteLimit - capturedOutputByteCount :
                                                   0U;
            const size_t appendByteCount =
                static_cast<size_t>(std::min<uint64_t>(static_cast<uint64_t>(readByteCount), availableCapacity));
            processResult.capturedOutput.append(outputBuffer, appendByteCount);
            processResult.capturedOutputTruncated =
                processResult.capturedOutputTruncated || appendByteCount < static_cast<size_t>(readByteCount);
            continue;
        }
        if (readByteCount == 0) {
            return OutputDrainStatus::EndOfFile;
        }
        if (errno == EINTR) {
            continue;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return OutputDrainStatus::NoDataAvailable;
        }
        const int readError = errno;
        ASCENDLOGE(
            "Failed to read process output: fd=%d errno=%d message=%s", outputFileDescriptor, readError,
            std::strerror(readError));
        return OutputDrainStatus::Failed;
    }
    return OutputDrainStatus::ReadBudgetExhausted;
}

bool WaitForOutputOrNextProcessCheck(int outputFileDescriptor, bool outputPipeClosed, int pollTimeoutMilliseconds)
{
    pollfd outputPollDescriptor{outputFileDescriptor, POLLIN, 0};
    pollfd* pollDescriptors = outputPipeClosed ? nullptr : &outputPollDescriptor;
    const nfds_t pollDescriptorCount = outputPipeClosed ? 0U : 1U;
    const int pollResult = poll(pollDescriptors, pollDescriptorCount, pollTimeoutMilliseconds);
    if (pollResult < 0 && errno != EINTR) {
        const int pollError = errno;
        ASCENDLOGE(
            "Failed to poll process output: fd=%d errno=%d message=%s", outputFileDescriptor, pollError,
            std::strerror(pollError));
        return false;
    }
    return true;
}

class SpawnedProcess final {
public:
    explicit SpawnedProcess(std::chrono::milliseconds terminationGracePeriod) noexcept
        : terminationGracePeriod_(terminationGracePeriod)
    {}

    ~SpawnedProcess() noexcept
    {
        if (processLeaderNeedsReaping_) {
            int ignoredProcessStatus = 0;
            TerminateAndReapProcessGroup(ignoredProcessStatus);
        }
    }

    SpawnedProcess(const SpawnedProcess&) = delete;
    SpawnedProcess& operator=(const SpawnedProcess&) = delete;

    bool Start(const ProcessExecutionRequest& request)
    {
        std::vector<std::string> argumentStorage;
        std::vector<char*> spawnArgv;
        BuildSpawnArguments(request.arguments, argumentStorage, spawnArgv);
        std::vector<std::string> environmentStorage;
        std::vector<char*> spawnEnvironment;
        if (!BuildSpawnEnvironment(request, environmentStorage, spawnEnvironment)) {
            return false;
        }

        ScopedFileDescriptor outputWriteFileDescriptor;
        if (request.outputMode == ProcessOutputMode::CaptureCombined) {
            int outputPipe[2]{-1, -1};
            if (!CreateNonblockingCloseOnExecPipe(outputPipe)) {
                return false;
            }
            outputReadFileDescriptor_.TakeOwnership(outputPipe[0]);
            outputWriteFileDescriptor.TakeOwnership(outputPipe[1]);
        }

        SpawnConfiguration spawnConfiguration;
        if (!spawnConfiguration.Initialize(
                request.outputMode, outputReadFileDescriptor_.Get(), outputWriteFileDescriptor.Get())) {
            return false;
        }

        startedAt_ = SteadyClock::now();
        const int spawnResult = posix_spawn(
            &processLeaderId_, argumentStorage.front().c_str(), spawnConfiguration.GetFileActions(),
            spawnConfiguration.GetSpawnAttributes(), spawnArgv.data(), spawnEnvironment.data());
        outputWriteFileDescriptor.Close();
        if (spawnResult != 0) {
            ASCENDLOGE(
                "Failed to spawn process: executable=%s error=%d message=%s", argumentStorage.front().c_str(),
                spawnResult, std::strerror(spawnResult));
            return false;
        }
        processLeaderNeedsReaping_ = true;
        return true;
    }

    bool WaitForExit(const ProcessExecutionRequest& request, ProcessExecutionResult& processResult)
    {
        const SteadyClock::time_point executionDeadline = startedAt_ + request.executionTimeout;
        int processStatus = 0;
        bool infrastructureFailure = false;
        bool outputPipeClosed = request.outputMode == ProcessOutputMode::Inherit;
        bool processLeaderExited = false;
        while (true) {
            OutputDrainStatus outputDrainStatus = OutputDrainStatus::EndOfFile;
            if (!outputPipeClosed) {
                outputDrainStatus = DrainCapturedOutput(
                    outputReadFileDescriptor_.Get(), request.capturedOutputByteLimit, processResult);
                if (outputDrainStatus == OutputDrainStatus::Failed) {
                    infrastructureFailure = true;
                    break;
                }
                if (outputDrainStatus == OutputDrainStatus::EndOfFile) {
                    outputReadFileDescriptor_.Close();
                    outputPipeClosed = true;
                }
            }
            if (!processLeaderExited) {
                const ProcessLeaderState processLeaderState = ObserveProcessLeaderExitWithoutReaping();
                if (processLeaderState == ProcessLeaderState::InspectionFailed) {
                    infrastructureFailure = true;
                    break;
                }
                processLeaderExited = processLeaderState == ProcessLeaderState::Exited;
            }
            if (processLeaderExited && outputPipeClosed) {
                break;
            }
            if (SteadyClock::now() >= executionDeadline) {
                processResult.termination = ProcessTermination::TimedOut;
                TerminateAndReapProcessGroup(processStatus);
                break;
            }
            if (outputDrainStatus == OutputDrainStatus::ReadBudgetExhausted) {
                continue;
            }

            const std::chrono::milliseconds remainingTime =
                std::chrono::duration_cast<std::chrono::milliseconds>(executionDeadline - SteadyClock::now());
            const int64_t maximumPollInterval =
                outputPipeClosed ? MAX_PROCESS_CHECK_INTERVAL_MILLISECONDS : MAX_OUTPUT_WAIT_INTERVAL_MILLISECONDS;
            const int pollTimeoutMilliseconds =
                static_cast<int>(std::max<int64_t>(1, std::min<int64_t>(remainingTime.count(), maximumPollInterval)));
            if (!WaitForOutputOrNextProcessCheck(
                    outputReadFileDescriptor_.Get(), outputPipeClosed, pollTimeoutMilliseconds)) {
                infrastructureFailure = true;
                break;
            }
        }

        if (infrastructureFailure) {
            TerminateAndReapProcessGroup(processStatus);
        } else if (processResult.termination != ProcessTermination::TimedOut && !ReapProcessLeader(processStatus)) {
            infrastructureFailure = true;
            TerminateAndReapProcessGroup(processStatus);
        }
        if (!outputPipeClosed) {
            const OutputDrainStatus finalDrainStatus =
                DrainCapturedOutput(outputReadFileDescriptor_.Get(), request.capturedOutputByteLimit, processResult);
            infrastructureFailure = infrastructureFailure || finalDrainStatus == OutputDrainStatus::Failed;
        }
        outputReadFileDescriptor_.Close();
        processResult.elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - startedAt_);
        if (infrastructureFailure) {
            processResult.termination = ProcessTermination::InfrastructureFailure;
            return false;
        }
        if (processResult.termination == ProcessTermination::TimedOut) {
            return true;
        }
        if (WIFEXITED(processStatus)) {
            processResult.termination = ProcessTermination::Exited;
            processResult.terminationCode = static_cast<int32_t>(WEXITSTATUS(processStatus));
            return true;
        }
        if (WIFSIGNALED(processStatus)) {
            processResult.termination = ProcessTermination::Signaled;
            processResult.terminationCode = static_cast<int32_t>(WTERMSIG(processStatus));
            return true;
        }
        processResult.termination = ProcessTermination::InfrastructureFailure;
        ASCENDLOGE(
            "Process returned an unsupported wait status: pid=%d status=%d", static_cast<int>(processLeaderId_),
            processStatus);
        return false;
    }

private:
    ProcessLeaderState ObserveProcessLeaderExitWithoutReaping() const noexcept
    {
        siginfo_t processInformation{};
        int waitResult = 0;
        do {
            waitResult =
                waitid(P_PID, static_cast<id_t>(processLeaderId_), &processInformation, WEXITED | WNOHANG | WNOWAIT);
        } while (waitResult != 0 && errno == EINTR);
        if (waitResult != 0) {
            const int waitError = errno;
            ASCENDLOGE(
                "Failed to inspect process: pid=%d errno=%d message=%s", static_cast<int>(processLeaderId_), waitError,
                std::strerror(waitError));
            return ProcessLeaderState::InspectionFailed;
        }
        return processInformation.si_pid == 0 ? ProcessLeaderState::Running : ProcessLeaderState::Exited;
    }

    bool ReapProcessLeader(int& processStatus) noexcept
    {
        if (!processLeaderNeedsReaping_) {
            return true;
        }
        pid_t waitResult = -1;
        do {
            waitResult = waitpid(processLeaderId_, &processStatus, 0);
        } while (waitResult < 0 && errno == EINTR);
        if (waitResult != processLeaderId_) {
            const int waitError = errno;
            ASCENDLOGE(
                "Failed to reap process: pid=%d errno=%d message=%s", static_cast<int>(processLeaderId_), waitError,
                std::strerror(waitError));
            return false;
        }
        processLeaderNeedsReaping_ = false;
        return true;
    }

    void TerminateAndReapProcessGroup(int& processStatus) noexcept
    {
        if (!processLeaderNeedsReaping_) {
            return;
        }
        SignalProcessGroup(processLeaderId_, SIGTERM);
        // Keep the process leader unreaped so its PID cannot be reused as another process group ID during cleanup.
        std::this_thread::sleep_for(terminationGracePeriod_);
        SignalProcessGroup(processLeaderId_, SIGKILL);
        (void)ReapProcessLeader(processStatus);
    }

    ScopedFileDescriptor outputReadFileDescriptor_;
    pid_t processLeaderId_{-1};
    SteadyClock::time_point startedAt_{};
    std::chrono::milliseconds terminationGracePeriod_;
    bool processLeaderNeedsReaping_{false};
};

} // namespace

bool ProcessExecutionResult::HasSuccessfulExit() const noexcept
{
    return termination == ProcessTermination::Exited && terminationCode == 0;
}

const char* ProcessExecutionResult::GetTerminationName() const noexcept
{
    switch (termination) {
        case ProcessTermination::Exited:
            return "exited";
        case ProcessTermination::Signaled:
            return "signaled";
        case ProcessTermination::TimedOut:
            return "timed_out";
        case ProcessTermination::InfrastructureFailure:
            return "infrastructure_failure";
        default:
            return "unknown";
    }
}

ProcessExecutionResult ProcessExecutor::Execute(const ProcessExecutionRequest& request)
{
    ProcessExecutionResult processResult;
    if (!IsExecutionRequestWellFormed(request)) {
        return processResult;
    }
    try {
        SpawnedProcess spawnedProcess(request.terminationGracePeriod);
        if (!spawnedProcess.Start(request)) {
            return processResult;
        }
        (void)spawnedProcess.WaitForExit(request, processResult);
    } catch (const std::bad_alloc&) {
        ASCENDLOGE(
            "Out of memory while preparing or executing process: executable=%s", request.arguments.front().c_str());
    } catch (const std::exception& exception) {
        ASCENDLOGE(
            "Process execution failed with exception: executable=%s message=%s", request.arguments.front().c_str(),
            exception.what());
    } catch (...) {
        ASCENDLOGE(
            "Process execution failed with an unknown exception: executable=%s", request.arguments.front().c_str());
    }
    return processResult;
}

} // namespace ascendc
