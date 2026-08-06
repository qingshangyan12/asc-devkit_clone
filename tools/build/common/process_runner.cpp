/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "process_runner.h"

#include <cerrno>
#include <csignal>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <thread>

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "ascendc_tool_log.h"

namespace ascendc {
namespace {

constexpr uint32_t kSubprocessTimeoutSeconds = 30U * 60U;
constexpr uint32_t kSubprocessTerminationGraceSeconds = 5U;
constexpr uint32_t kSubprocessPollMilliseconds = 10U;

std::runtime_error SystemError(const std::string& operation)
{
    return std::runtime_error(operation + ": " + std::strerror(errno));
}

} // namespace

void ProcessRunner::Run(
    const std::vector<std::string>& arguments, const std::vector<std::pair<std::string, std::string>>& environment)
{
    Run(arguments, environment, std::chrono::seconds(kSubprocessTimeoutSeconds));
}

void ProcessRunner::Run(
    const std::vector<std::string>& arguments, const std::vector<std::pair<std::string, std::string>>& environment,
    std::chrono::milliseconds timeout)
{
    if (arguments.empty()) {
        throw std::invalid_argument("subprocess arguments must not be empty");
    }
    if (timeout.count() <= 0) {
        throw std::invalid_argument("subprocess timeout must be positive");
    }
    std::vector<char*> argv;
    argv.reserve(arguments.size() + 1U);
    for (const std::string& argument : arguments) {
        argv.push_back(const_cast<char*>(argument.c_str()));
    }
    argv.push_back(nullptr);

    const pid_t child = fork();
    if (child < 0) {
        const std::runtime_error error = SystemError("failed to fork subprocess");
        ASCENDLOGD("Subprocess launch failed: %s", error.what());
        throw error;
    }
    if (child == 0) {
        if (setpgid(0, 0) != 0) {
            _exit(126);
        }
        for (const char* name : {"MAKEFLAGS", "MFLAGS", "MAKEFILES", "GNUMAKEFLAGS"}) {
            (void)unsetenv(name);
        }
        for (const auto& item : environment) {
            if (setenv(item.first.c_str(), item.second.c_str(), 1) != 0) {
                _exit(126);
            }
        }
        execv(argv[0], argv.data());
        _exit(127);
    }

    ASCENDLOGD("Started subprocess pid=%d executable=%s", static_cast<int>(child), arguments.front().c_str());

    int status = 0;
    const std::chrono::steady_clock::time_point deadline = std::chrono::steady_clock::now() + timeout;
    while (true) {
        const pid_t waited = waitpid(child, &status, WNOHANG);
        if (waited == child) {
            break;
        }
        if (waited < 0 && errno != EINTR) {
            const int waitError = errno;
            TerminateAndReap(child);
            const std::runtime_error error("failed to wait for subprocess: " + std::string(std::strerror(waitError)));
            ASCENDLOGD("Subprocess wait failed: %s", error.what());
            throw error;
        }
        if (std::chrono::steady_clock::now() >= deadline) {
            TerminateAndReap(child);
            throw std::runtime_error("subprocess timed out: " + arguments.front());
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kSubprocessPollMilliseconds));
    }
    if (WIFSIGNALED(status)) {
        const std::string message =
            "subprocess terminated by signal " + std::to_string(WTERMSIG(status)) + ": " + arguments.front();
        ASCENDLOGD("%s", message.c_str());
        throw std::runtime_error(message);
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0) {
        const int exitCode = WIFEXITED(status) ? WEXITSTATUS(status) : -1;
        const std::string message =
            "subprocess exited with code " + std::to_string(exitCode) + ": " + arguments.front();
        ASCENDLOGD("%s", message.c_str());
        throw std::runtime_error(message);
    }
    ASCENDLOGD("Subprocess pid=%d completed successfully", static_cast<int>(child));
}

void ProcessRunner::TerminateAndReap(pid_t child) noexcept
{
    (void)kill(-child, SIGTERM);
    const std::chrono::steady_clock::time_point deadline =
        std::chrono::steady_clock::now() + std::chrono::seconds(kSubprocessTerminationGraceSeconds);
    int status = 0;
    bool childReaped = false;
    while (std::chrono::steady_clock::now() < deadline) {
        if (!childReaped) {
            const pid_t waited = waitpid(child, &status, WNOHANG);
            childReaped = waited == child || (waited < 0 && errno == ECHILD);
        }
        errno = 0;
        if (kill(-child, 0) != 0 && errno == ESRCH) {
            break;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(kSubprocessPollMilliseconds));
    }
    errno = 0;
    if (kill(-child, 0) == 0 || errno != ESRCH) {
        (void)kill(-child, SIGKILL);
    }
    if (!childReaped) {
        pid_t waited = -1;
        do {
            waited = waitpid(child, &status, 0);
        } while (waited < 0 && errno == EINTR);
    }
}

} // namespace ascendc
