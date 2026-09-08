/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "staged_file_replacement.h"

#include "ascendc_tool_log.h"
#include "file_utils.h"

#include <algorithm>
#include <atomic>
#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

namespace ascendc {
namespace {
constexpr size_t FILE_COPY_BUFFER_BYTES = 64U * 1024U;

class OwnedFileDescriptor final {
public:
    explicit OwnedFileDescriptor(int descriptor) noexcept : descriptor_(descriptor) {}
    ~OwnedFileDescriptor() noexcept { (void)Close(); }
    OwnedFileDescriptor(const OwnedFileDescriptor&) = delete;
    OwnedFileDescriptor& operator=(const OwnedFileDescriptor&) = delete;

    int Get() const noexcept { return descriptor_; }
    bool Close() noexcept
    {
        if (descriptor_ < 0) {
            return true;
        }
        const int descriptor = descriptor_;
        descriptor_ = -1;
        if (close(descriptor) != 0) {
            ASCENDLOGE("Failed to close staged file descriptor: fd=%d errno=%d", descriptor, errno);
            return false;
        }
        return true;
    }

private:
    int descriptor_;
};

bool WriteAllBytes(int borrowedDescriptor, const char* data, size_t size) noexcept
{
    size_t written = 0U;
    while (written < size) {
        const size_t chunkSize = std::min(size - written, FILE_COPY_BUFFER_BYTES);
        const ssize_t result = write(borrowedDescriptor, data + written, chunkSize);
        if (result < 0 && errno == EINTR) {
            continue;
        }
        if (result <= 0) {
            ASCENDLOGE("Failed to write staged file: fd=%d errno=%d", borrowedDescriptor, errno);
            return false;
        }
        written += static_cast<size_t>(result);
    }
    return true;
}

bool CopyAllBytes(int borrowedSource, int borrowedDestination) noexcept
{
    char buffer[FILE_COPY_BUFFER_BYTES];
    while (true) {
        const ssize_t count = read(borrowedSource, buffer, sizeof(buffer));
        if (count == 0) {
            return true;
        }
        if (count < 0) {
            if (errno == EINTR) {
                continue;
            }
            ASCENDLOGE("Failed to read staged copy source: fd=%d errno=%d", borrowedSource, errno);
            return false;
        }
        if (!WriteAllBytes(borrowedDestination, buffer, static_cast<size_t>(count))) {
            return false;
        }
    }
}
} // namespace

StagedFileReplacement::~StagedFileReplacement() noexcept
{
    if (ownsTemporaryFile_ && unlink(ownedTemporaryPath_.c_str()) != 0 && errno != ENOENT) {
        ASCENDLOGW("Failed to remove unpublished staged file: path=%s errno=%d", ownedTemporaryPath_.c_str(), errno);
    }
}

int StagedFileReplacement::CreateTemporaryFile(const std::string& destinationPath, mode_t mode)
{
    if (!canonicalDestinationPath_.empty()) {
        ASCENDLOGE("File replacement has already been staged");
        return -1;
    }
    if (!FileUtils::ResolveRegularFilePathForWrite(destinationPath, canonicalDestinationPath_)) {
        return -1;
    }
    constexpr size_t maxAttempts = 32U;
    static std::atomic<uint64_t> sequence{0};
    for (size_t attempt = 0U; attempt < maxAttempts; ++attempt) {
        // Allocate the cleanup path before acquiring the descriptor. O_EXCL
        // rejects collisions without touching existing files or symlinks.
        ownedTemporaryPath_ = canonicalDestinationPath_ + ".asc_tmp_" + std::to_string(getpid()) + "_" +
                              std::to_string(sequence.fetch_add(1U, std::memory_order_relaxed));
        const int descriptor = open(ownedTemporaryPath_.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, mode);
        if (descriptor >= 0) {
            ownsTemporaryFile_ = true;
            return descriptor;
        }
        if (errno != EEXIST) {
            break;
        }
    }
    ASCENDLOGE("Failed to create staged output file: path=%s errno=%d", destinationPath.c_str(), errno);
    return -1;
}

bool StagedFileReplacement::StageTextFile(const std::string& destinationPath, const std::string& text)
{
    constexpr mode_t privateTextMode = 0600;
    OwnedFileDescriptor output(CreateTemporaryFile(destinationPath, privateTextMode));
    if (output.Get() < 0) {
        return false;
    }
    const bool written = WriteAllBytes(output.Get(), text.data(), text.size());
    const bool closed = output.Close();
    readyForReplacement_ = written && closed;
    return readyForReplacement_;
}

bool StagedFileReplacement::StageFileCopy(const std::string& sourcePath, const std::string& destinationPath)
{
    std::string canonicalSourcePath;
    if (!FileUtils::ResolveRegularFilePathForRead(sourcePath, canonicalSourcePath)) {
        return false;
    }
    OwnedFileDescriptor input(open(canonicalSourcePath.c_str(), O_RDONLY | O_CLOEXEC));
    if (input.Get() < 0) {
        ASCENDLOGE("Failed to open staged copy source: path=%s errno=%d", sourcePath.c_str(), errno);
        return false;
    }
    // open applies the caller's umask, preserving ordinary output-file permissions.
    constexpr mode_t copyMode = 0644;
    OwnedFileDescriptor output(CreateTemporaryFile(destinationPath, copyMode));
    if (output.Get() < 0) {
        return false;
    }
    const bool copied = CopyAllBytes(input.Get(), output.Get());
    const bool sourceClosed = input.Close();
    const bool outputClosed = output.Close();
    readyForReplacement_ = copied && sourceClosed && outputClosed;
    return readyForReplacement_;
}

bool StagedFileReplacement::ReplaceDestinationAtomically() noexcept
{
    if (!readyForReplacement_) {
        return false;
    }
    if (rename(ownedTemporaryPath_.c_str(), canonicalDestinationPath_.c_str()) != 0) {
        ASCENDLOGE("Failed to replace staged destination: path=%s errno=%d", canonicalDestinationPath_.c_str(), errno);
        return false;
    }
    ownsTemporaryFile_ = false;
    readyForReplacement_ = false;
    return true;
}

} // namespace ascendc
