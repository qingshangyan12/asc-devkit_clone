/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "atomic_file_writer.h"

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
    explicit OwnedFileDescriptor(int32_t descriptor) noexcept : descriptor_(descriptor) {}
    ~OwnedFileDescriptor() noexcept { (void)Close(); }
    OwnedFileDescriptor(const OwnedFileDescriptor&) = delete;
    OwnedFileDescriptor& operator=(const OwnedFileDescriptor&) = delete;

    int32_t Get() const noexcept { return descriptor_; }
    bool Close() noexcept
    {
        if (descriptor_ < 0) {
            return true;
        }
        const int32_t descriptor = descriptor_;
        descriptor_ = -1;
        if (close(descriptor) != 0) {
            ASCENDLOGE("Failed to close temporary file descriptor: fd=%d errno=%d", descriptor, errno);
            return false;
        }
        return true;
    }

private:
    int32_t descriptor_;
};

bool WriteAllBytes(int32_t borrowedFileDescriptor, const char* data, size_t byteCount) noexcept
{
    size_t writtenByteCount = 0U;
    while (writtenByteCount < byteCount) {
        const size_t chunkSize = std::min(byteCount - writtenByteCount, FILE_COPY_BUFFER_BYTES);
        const ssize_t writeResult = write(borrowedFileDescriptor, data + writtenByteCount, chunkSize);
        if (writeResult < 0 && errno == EINTR) {
            continue;
        }
        if (writeResult <= 0) {
            ASCENDLOGE("Failed to write temporary file: fd=%d errno=%d", borrowedFileDescriptor, errno);
            return false;
        }
        writtenByteCount += static_cast<size_t>(writeResult);
    }
    return true;
}

bool CopyAllBytes(int32_t borrowedSourceDescriptor, int32_t borrowedDestinationDescriptor) noexcept
{
    char buffer[FILE_COPY_BUFFER_BYTES];
    while (true) {
        const ssize_t readByteCount = read(borrowedSourceDescriptor, buffer, sizeof(buffer));
        if (readByteCount == 0) {
            return true;
        }
        if (readByteCount < 0) {
            if (errno == EINTR) {
                continue;
            }
            ASCENDLOGE("Failed to read copy source: fd=%d errno=%d", borrowedSourceDescriptor, errno);
            return false;
        }
        if (!WriteAllBytes(borrowedDestinationDescriptor, buffer, static_cast<size_t>(readByteCount))) {
            return false;
        }
    }
}
} // namespace

AtomicFileWriter::~AtomicFileWriter() noexcept
{
    if (ownsTemporaryFile_ && unlink(ownedTemporaryFilePath_.c_str()) != 0 && errno != ENOENT) {
        ASCENDLOGW(
            "Failed to remove unpublished temporary file: path=%s errno=%d", ownedTemporaryFilePath_.c_str(), errno);
    }
}

int32_t AtomicFileWriter::CreateUniqueTemporaryFile(const std::string& destinationPath, mode_t permissions)
{
    if (!canonicalDestinationPath_.empty()) {
        ASCENDLOGE("File replacement has already been prepared");
        return -1;
    }
    if (!FileUtils::ResolveRegularFilePathForWrite(destinationPath, canonicalDestinationPath_)) {
        return -1;
    }
    constexpr uint32_t maxCreationAttempts = 32U;
    static std::atomic<uint64_t> temporaryFileSequence{0};
    for (uint32_t attempt = 0U; attempt < maxCreationAttempts; ++attempt) {
        // Allocate the cleanup path before acquiring the descriptor. O_EXCL
        // rejects collisions without touching existing files or symlinks.
        ownedTemporaryFilePath_ = canonicalDestinationPath_ + ".asc_tmp_" + std::to_string(getpid()) + "_" +
                                  std::to_string(temporaryFileSequence.fetch_add(1U, std::memory_order_relaxed));
        const int32_t descriptor =
            open(ownedTemporaryFilePath_.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, permissions);
        if (descriptor >= 0) {
            ownsTemporaryFile_ = true;
            return descriptor;
        }
        if (errno != EEXIST) {
            break;
        }
    }
    ASCENDLOGE("Failed to create temporary output file: path=%s errno=%d", destinationPath.c_str(), errno);
    return -1;
}

bool AtomicFileWriter::WriteTextToTemporaryFile(const std::string& destinationPath, const std::string& text)
{
    constexpr mode_t privateTextPermissions = 0600;
    OwnedFileDescriptor output(CreateUniqueTemporaryFile(destinationPath, privateTextPermissions));
    if (output.Get() < 0) {
        return false;
    }
    const bool writeSucceeded = WriteAllBytes(output.Get(), text.data(), text.size());
    const bool closeSucceeded = output.Close();
    temporaryFileWriteSucceeded_ = writeSucceeded && closeSucceeded;
    return temporaryFileWriteSucceeded_;
}

bool AtomicFileWriter::CopyFileToTemporaryFile(const std::string& sourcePath, const std::string& destinationPath)
{
    std::string canonicalSourcePath;
    if (!FileUtils::ResolveRegularFilePathForRead(sourcePath, canonicalSourcePath)) {
        return false;
    }
    OwnedFileDescriptor input(open(canonicalSourcePath.c_str(), O_RDONLY | O_CLOEXEC));
    if (input.Get() < 0) {
        ASCENDLOGE("Failed to open copy source: path=%s errno=%d", sourcePath.c_str(), errno);
        return false;
    }
    // open applies the caller's umask, preserving ordinary output-file permissions.
    constexpr mode_t copiedFilePermissions = 0644;
    OwnedFileDescriptor output(CreateUniqueTemporaryFile(destinationPath, copiedFilePermissions));
    if (output.Get() < 0) {
        return false;
    }
    const bool copySucceeded = CopyAllBytes(input.Get(), output.Get());
    const bool sourceCloseSucceeded = input.Close();
    const bool outputCloseSucceeded = output.Close();
    temporaryFileWriteSucceeded_ = copySucceeded && sourceCloseSucceeded && outputCloseSucceeded;
    return temporaryFileWriteSucceeded_;
}

bool AtomicFileWriter::ReplaceDestinationAtomically() noexcept
{
    if (!temporaryFileWriteSucceeded_) {
        return false;
    }
    if (rename(ownedTemporaryFilePath_.c_str(), canonicalDestinationPath_.c_str()) != 0) {
        ASCENDLOGE("Failed to replace destination file: path=%s errno=%d", canonicalDestinationPath_.c_str(), errno);
        return false;
    }
    ownsTemporaryFile_ = false;
    temporaryFileWriteSucceeded_ = false;
    return true;
}

} // namespace ascendc
