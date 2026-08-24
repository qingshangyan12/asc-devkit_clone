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
#include <utility>

namespace ascendc {
namespace {
using SteadyClock = std::chrono::steady_clock;

constexpr uint64_t MAX_OUTPUT_READ_BYTES_PER_WAIT_ITERATION = 64U * 1024U;
constexpr int64_t MAX_OUTPUT_WAIT_INTERVAL_MILLISECONDS = 50;
constexpr int64_t MAX_PROCESS_CHECK_INTERVAL_MILLISECONDS = 5;

enum class CapturedOutputDrainStatus : uint32_t {
    NoDataAvailable,
    EndOfFile,
    ReadBudgetExhausted,
    Failed,
};

enum class ProcessLeaderObservation : uint32_t {
    Running,
    Terminated,
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

class PosixSpawnCStringArray final {
public:
    explicit PosixSpawnCStringArray(std::vector<std::string> strings) : stringStorage_(std::move(strings))
    {
        cStringPointers_.reserve(stringStorage_.size() + 1U);
        for (std::string& string : stringStorage_) {
            cStringPointers_.push_back(const_cast<char*>(string.c_str()));
        }
        cStringPointers_.push_back(nullptr);
    }

    PosixSpawnCStringArray(const PosixSpawnCStringArray&) = delete;
    PosixSpawnCStringArray& operator=(const PosixSpawnCStringArray&) = delete;
    PosixSpawnCStringArray(PosixSpawnCStringArray&&) = delete;
    PosixSpawnCStringArray& operator=(PosixSpawnCStringArray&&) = delete;

    char* const* Data() noexcept { return cStringPointers_.data(); }

private:
    std::vector<std::string> stringStorage_;
    std::vector<char*> cStringPointers_;
};

class PosixSpawnConfiguration final {
public:
    PosixSpawnConfiguration() = default;

    ~PosixSpawnConfiguration() noexcept
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

    PosixSpawnConfiguration(const PosixSpawnConfiguration&) = delete;
    PosixSpawnConfiguration& operator=(const PosixSpawnConfiguration&) = delete;

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

bool ValidateProcessExecutorRequest(const ProcessExecutorRequest& request) noexcept
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
    for (size_t overrideIndex = 0U; overrideIndex < request.environmentOverrides.size(); ++overrideIndex) {
        const std::pair<std::string, std::string>& environmentOverride = request.environmentOverrides[overrideIndex];
        if (!IsEnvironmentVariableNameValid(environmentOverride.first)) {
            ASCENDLOGE(
                "Process environment override[%zu] has an invalid variable name: name=%s", overrideIndex,
                environmentOverride.first.c_str());
            return false;
        }
        if (ContainsEmbeddedNull(environmentOverride.second)) {
            ASCENDLOGE(
                "Process environment override[%zu] contains an embedded NUL value: name=%s", overrideIndex,
                environmentOverride.first.c_str());
            return false;
        }
    }
    for (size_t removalIndex = 0U; removalIndex < request.removedEnvironmentVariables.size(); ++removalIndex) {
        const std::string& environmentVariableName = request.removedEnvironmentVariables[removalIndex];
        if (!IsEnvironmentVariableNameValid(environmentVariableName)) {
            ASCENDLOGE(
                "Process environment removal[%zu] has an invalid variable name: name=%s", removalIndex,
                environmentVariableName.c_str());
            return false;
        }
    }
    return true;
}

bool CreateNonblockingOutputCapturePipe(
    ScopedFileDescriptor& outputReadFileDescriptor, ScopedFileDescriptor& outputWriteFileDescriptor)
{
    int outputPipe[2]{-1, -1};
    if (pipe2(outputPipe, O_CLOEXEC) != 0) {
        const int pipeError = errno;
        ASCENDLOGE("Failed to create process output pipe: errno=%d message=%s", pipeError, std::strerror(pipeError));
        return false;
    }
    outputReadFileDescriptor.TakeOwnership(outputPipe[0]);
    outputWriteFileDescriptor.TakeOwnership(outputPipe[1]);
    const int readFlags = fcntl(outputReadFileDescriptor.Get(), F_GETFL, 0);
    if (readFlags < 0 || fcntl(outputReadFileDescriptor.Get(), F_SETFL, readFlags | O_NONBLOCK) != 0) {
        const int flagError = errno;
        ASCENDLOGE(
            "Failed to make process output pipe nonblocking: fd=%d errno=%d message=%s", outputReadFileDescriptor.Get(),
            flagError, std::strerror(flagError));
        return false;
    }
    return true;
}

std::vector<std::string> BuildEffectiveSpawnEnvironment(const ProcessExecutorRequest& request)
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
        environmentByName[environmentVariableName] = environmentEntry.substr(separatorPosition + 1U);
    }
    for (const std::string& removedEnvironmentVariable : request.removedEnvironmentVariables) {
        environmentByName.erase(removedEnvironmentVariable);
    }
    for (const std::pair<std::string, std::string>& environmentOverride : request.environmentOverrides) {
        environmentByName[environmentOverride.first] = environmentOverride.second;
    }

    std::vector<std::string> environmentEntries;
    environmentEntries.reserve(environmentByName.size());
    for (const std::pair<const std::string, std::string>& environmentVariable : environmentByName) {
        environmentEntries.emplace_back(environmentVariable.first + "=" + environmentVariable.second);
    }
    return environmentEntries;
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

CapturedOutputDrainStatus DrainCapturedOutput(
    int outputFileDescriptor, uint64_t capturedOutputByteLimit, ProcessExecutorResult& executorResult)
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
            const uint64_t capturedOutputByteCount = static_cast<uint64_t>(executorResult.capturedOutput.size());
            const uint64_t availableCapacity = capturedOutputByteLimit > capturedOutputByteCount ?
                                                   capturedOutputByteLimit - capturedOutputByteCount :
                                                   0U;
            const size_t appendByteCount =
                static_cast<size_t>(std::min<uint64_t>(static_cast<uint64_t>(readByteCount), availableCapacity));
            executorResult.capturedOutput.append(outputBuffer, appendByteCount);
            executorResult.capturedOutputTruncated =
                executorResult.capturedOutputTruncated || appendByteCount < static_cast<size_t>(readByteCount);
            continue;
        }
        if (readByteCount == 0) {
            return CapturedOutputDrainStatus::EndOfFile;
        }
        if (errno == EINTR) {
            continue;
        }
        if (errno == EAGAIN || errno == EWOULDBLOCK) {
            return CapturedOutputDrainStatus::NoDataAvailable;
        }
        const int readError = errno;
        ASCENDLOGE(
            "Failed to read process output: fd=%d errno=%d message=%s", outputFileDescriptor, readError,
            std::strerror(readError));
        return CapturedOutputDrainStatus::Failed;
    }
    return CapturedOutputDrainStatus::ReadBudgetExhausted;
}

bool WaitForOutputOrNextStatusCheck(int outputFileDescriptor, bool outputPipeClosed, int pollTimeoutMilliseconds)
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

class ChildProcessGroup final {
public:
    explicit ChildProcessGroup(std::chrono::milliseconds terminationGracePeriod) noexcept
        : terminationGracePeriod_(terminationGracePeriod)
    {}

    ~ChildProcessGroup() noexcept
    {
        if (processLeaderNeedsReaping_) {
            int ignoredWaitStatus = 0;
            TerminateProcessGroupAndReapLeader(ignoredWaitStatus);
        }
    }

    ChildProcessGroup(const ChildProcessGroup&) = delete;
    ChildProcessGroup& operator=(const ChildProcessGroup&) = delete;

    bool Start(const ProcessExecutorRequest& request)
    {
        PosixSpawnCStringArray spawnArguments(request.arguments);
        PosixSpawnCStringArray spawnEnvironment(BuildEffectiveSpawnEnvironment(request));

        ScopedFileDescriptor outputWriteFileDescriptor;
        if (request.outputMode == ProcessOutputMode::CaptureCombined) {
            if (!CreateNonblockingOutputCapturePipe(outputReadFileDescriptor_, outputWriteFileDescriptor)) {
                return false;
            }
        }

        PosixSpawnConfiguration spawnConfiguration;
        if (!spawnConfiguration.Initialize(
                request.outputMode, outputReadFileDescriptor_.Get(), outputWriteFileDescriptor.Get())) {
            return false;
        }

        startedAt_ = SteadyClock::now();
        const int spawnResult = posix_spawn(
            &processLeaderId_, request.arguments.front().c_str(), spawnConfiguration.GetFileActions(),
            spawnConfiguration.GetSpawnAttributes(), spawnArguments.Data(), spawnEnvironment.Data());
        outputWriteFileDescriptor.Close();
        if (spawnResult != 0) {
            ASCENDLOGE(
                "Failed to spawn process: executable=%s error=%d message=%s", request.arguments.front().c_str(),
                spawnResult, std::strerror(spawnResult));
            return false;
        }
        processLeaderNeedsReaping_ = true;
        return true;
    }

    void WaitForCompletion(const ProcessExecutorRequest& request, ProcessExecutorResult& executorResult)
    {
        const SteadyClock::time_point executionDeadline = startedAt_ + request.executionTimeout;
        int waitStatus = 0;
        bool infrastructureFailure = false;
        bool outputPipeClosed = request.outputMode == ProcessOutputMode::Inherit;
        bool processLeaderTerminated = false;
        while (true) {
            CapturedOutputDrainStatus outputDrainStatus = CapturedOutputDrainStatus::EndOfFile;
            if (!outputPipeClosed) {
                outputDrainStatus = DrainCapturedOutput(
                    outputReadFileDescriptor_.Get(), request.capturedOutputByteLimit, executorResult);
                if (outputDrainStatus == CapturedOutputDrainStatus::Failed) {
                    infrastructureFailure = true;
                    break;
                }
                if (outputDrainStatus == CapturedOutputDrainStatus::EndOfFile) {
                    outputReadFileDescriptor_.Close();
                    outputPipeClosed = true;
                }
            }
            if (!processLeaderTerminated) {
                const ProcessLeaderObservation processLeaderObservation = ObserveProcessLeaderWithoutReaping();
                if (processLeaderObservation == ProcessLeaderObservation::InspectionFailed) {
                    infrastructureFailure = true;
                    break;
                }
                processLeaderTerminated = processLeaderObservation == ProcessLeaderObservation::Terminated;
            }
            if (processLeaderTerminated && outputPipeClosed) {
                break;
            }
            if (SteadyClock::now() >= executionDeadline) {
                executorResult.outcome = ProcessOutcome::TimedOut;
                ASCENDLOGE(
                    "Process exceeded its execution timeout: executable=%s timeout_ms=%lld",
                    request.arguments.front().c_str(), static_cast<long long>(request.executionTimeout.count()));
                TerminateProcessGroupAndReapLeader(waitStatus);
                break;
            }
            if (outputDrainStatus == CapturedOutputDrainStatus::ReadBudgetExhausted) {
                continue;
            }

            const std::chrono::milliseconds remainingTime =
                std::chrono::duration_cast<std::chrono::milliseconds>(executionDeadline - SteadyClock::now());
            const int64_t maximumPollInterval =
                outputPipeClosed ? MAX_PROCESS_CHECK_INTERVAL_MILLISECONDS : MAX_OUTPUT_WAIT_INTERVAL_MILLISECONDS;
            const int pollTimeoutMilliseconds =
                static_cast<int>(std::max<int64_t>(1, std::min<int64_t>(remainingTime.count(), maximumPollInterval)));
            if (!WaitForOutputOrNextStatusCheck(
                    outputReadFileDescriptor_.Get(), outputPipeClosed, pollTimeoutMilliseconds)) {
                infrastructureFailure = true;
                break;
            }
        }

        if (infrastructureFailure) {
            TerminateProcessGroupAndReapLeader(waitStatus);
        } else if (executorResult.outcome != ProcessOutcome::TimedOut && !ReapProcessLeader(waitStatus)) {
            infrastructureFailure = true;
            TerminateProcessGroupAndReapLeader(waitStatus);
        }
        if (!outputPipeClosed) {
            const CapturedOutputDrainStatus finalDrainStatus =
                DrainCapturedOutput(outputReadFileDescriptor_.Get(), request.capturedOutputByteLimit, executorResult);
            infrastructureFailure = infrastructureFailure || finalDrainStatus == CapturedOutputDrainStatus::Failed;
        }
        outputReadFileDescriptor_.Close();
        executorResult.elapsedTime =
            std::chrono::duration_cast<std::chrono::milliseconds>(SteadyClock::now() - startedAt_);
        if (infrastructureFailure) {
            executorResult.outcome = ProcessOutcome::ExecutorFailure;
            return;
        }
        if (executorResult.outcome == ProcessOutcome::TimedOut) {
            return;
        }
        if (WIFEXITED(waitStatus)) {
            executorResult.outcome = ProcessOutcome::Exited;
            executorResult.terminationCode = static_cast<int32_t>(WEXITSTATUS(waitStatus));
            return;
        }
        if (WIFSIGNALED(waitStatus)) {
            executorResult.outcome = ProcessOutcome::Signaled;
            executorResult.terminationCode = static_cast<int32_t>(WTERMSIG(waitStatus));
            return;
        }
        executorResult.outcome = ProcessOutcome::ExecutorFailure;
        ASCENDLOGE(
            "Process returned an unsupported wait status: pid=%d status=%d", static_cast<int>(processLeaderId_),
            waitStatus);
    }

private:
    ProcessLeaderObservation ObserveProcessLeaderWithoutReaping() const noexcept
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
            return ProcessLeaderObservation::InspectionFailed;
        }
        return processInformation.si_pid == 0 ? ProcessLeaderObservation::Running :
                                                ProcessLeaderObservation::Terminated;
    }

    bool ReapProcessLeader(int& waitStatus) noexcept
    {
        if (!processLeaderNeedsReaping_) {
            return true;
        }
        pid_t waitResult = -1;
        do {
            waitResult = waitpid(processLeaderId_, &waitStatus, 0);
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

    void TerminateProcessGroupAndReapLeader(int& waitStatus) noexcept
    {
        if (!processLeaderNeedsReaping_) {
            return;
        }
        SignalProcessGroup(processLeaderId_, SIGTERM);
        // Keep the process leader unreaped so its PID cannot be reused as another process group ID during cleanup.
        std::this_thread::sleep_for(terminationGracePeriod_);
        SignalProcessGroup(processLeaderId_, SIGKILL);
        (void)ReapProcessLeader(waitStatus);
    }

    ScopedFileDescriptor outputReadFileDescriptor_;
    pid_t processLeaderId_{-1};
    SteadyClock::time_point startedAt_{};
    std::chrono::milliseconds terminationGracePeriod_;
    bool processLeaderNeedsReaping_{false};
};

} // namespace

bool ProcessExecutorResult::HasSuccessfulExit() const noexcept
{
    return outcome == ProcessOutcome::Exited && terminationCode == 0;
}

const char* ProcessExecutorResult::GetOutcomeName() const noexcept
{
    switch (outcome) {
        case ProcessOutcome::Exited:
            return "exited";
        case ProcessOutcome::Signaled:
            return "signaled";
        case ProcessOutcome::TimedOut:
            return "timed_out";
        case ProcessOutcome::ExecutorFailure:
            return "executor_failure";
        default:
            return "unknown";
    }
}

ProcessExecutorResult ProcessExecutor::Execute(const ProcessExecutorRequest& request)
{
    ProcessExecutorResult executorResult;
    if (!ValidateProcessExecutorRequest(request)) {
        return executorResult;
    }
    try {
        ChildProcessGroup childProcessGroup(request.terminationGracePeriod);
        if (!childProcessGroup.Start(request)) {
            return executorResult;
        }
        childProcessGroup.WaitForCompletion(request, executorResult);
    } catch (const std::bad_alloc&) {
        throw;
    } catch (const std::exception& exception) {
        ASCENDLOGE(
            "Process execution failed with exception: executable=%s message=%s", request.arguments.front().c_str(),
            exception.what());
    } catch (...) {
        ASCENDLOGE(
            "Process execution failed with an unknown exception: executable=%s", request.arguments.front().c_str());
    }
    return executorResult;
}

} // namespace ascendc
