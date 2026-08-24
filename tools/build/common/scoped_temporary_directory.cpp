/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "scoped_temporary_directory.h"

#include "ascendc_tool_log.h"
#include "file_utils.h"

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <cerrno>
#include <cstring>
#include <sys/stat.h>
#include <utility>

namespace ascendc {
namespace {
namespace fs = boost::filesystem;

bool IsValidDirectoryNamePrefix(const std::string& directoryNamePrefix)
{
    if (directoryNamePrefix.empty() || directoryNamePrefix.find('%') != std::string::npos ||
        directoryNamePrefix.find('\0') != std::string::npos || directoryNamePrefix.find('\\') != std::string::npos) {
        return false;
    }
    const fs::path prefixPath(directoryNamePrefix);
    return prefixPath.is_relative() && !prefixPath.has_parent_path();
}

} // namespace

ScopedTemporaryDirectory::~ScopedTemporaryDirectory() noexcept
{
    if (automaticCleanupEnabled_) {
        (void)RemoveTrackedDirectory();
    }
}

ScopedTemporaryDirectory::ScopedTemporaryDirectory(ScopedTemporaryDirectory&& other) noexcept { MoveStateFrom(other); }

ScopedTemporaryDirectory& ScopedTemporaryDirectory::operator=(ScopedTemporaryDirectory&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    ScopedTemporaryDirectory previousDirectory(std::move(*this));
    MoveStateFrom(other);
    return *this;
}

bool ScopedTemporaryDirectory::CreateUniqueSubdirectory(
    const std::string& parentDirectoryPath, const std::string& directoryNamePrefix)
{
    if (!trackedDirectoryPath_.empty()) {
        ASCENDLOGE("Temporary directory already tracks a path: path=%s", trackedDirectoryPath_.c_str());
        return false;
    }
    if (!IsValidDirectoryNamePrefix(directoryNamePrefix)) {
        ASCENDLOGE(
            "Temporary directory name prefix must be one path component without %% characters: prefix=%s",
            directoryNamePrefix.c_str());
        return false;
    }

    std::string resolvedParentDirectoryPath;
    if (!FileUtils::ResolveDirectory(parentDirectoryPath, resolvedParentDirectoryPath)) {
        ASCENDLOGE("Temporary directory parent is unavailable: parent=%s", parentDirectoryPath.c_str());
        return false;
    }
    boost::system::error_code filesystemError;
    const fs::path uniqueName = fs::unique_path(directoryNamePrefix + "-%%%%%%%%%%%%%%%%", filesystemError);
    if (filesystemError || uniqueName.empty() || uniqueName.has_parent_path()) {
        ASCENDLOGE(
            "Failed to generate temporary directory name: parent=%s prefix=%s error=%s",
            resolvedParentDirectoryPath.c_str(), directoryNamePrefix.c_str(),
            filesystemError ? filesystemError.message().c_str() : "invalid generated name");
        return false;
    }
    const fs::path createdDirectoryPath = fs::path(resolvedParentDirectoryPath) / uniqueName;
    constexpr mode_t privateDirectoryPermissions = S_IRWXU;
    if (mkdir(createdDirectoryPath.c_str(), privateDirectoryPermissions) != 0) {
        const int createError = errno;
        ASCENDLOGE(
            "Failed to create temporary directory: path=%s errno=%d message=%s", createdDirectoryPath.c_str(),
            createError, std::strerror(createError));
        return false;
    }

    if (chmod(createdDirectoryPath.c_str(), privateDirectoryPermissions) != 0) {
        const int permissionError = errno;
        ASCENDLOGE(
            "Failed to make temporary directory private: path=%s errno=%d message=%s", createdDirectoryPath.c_str(),
            permissionError, std::strerror(permissionError));
        (void)FileUtils::RemoveAll(createdDirectoryPath.string());
        return false;
    }

    std::string resolvedCreatedDirectoryPath;
    if (!FileUtils::ResolveDirectory(createdDirectoryPath.string(), resolvedCreatedDirectoryPath) ||
        fs::path(resolvedCreatedDirectoryPath) != createdDirectoryPath.lexically_normal()) {
        ASCENDLOGE("Created temporary directory has an unexpected identity: path=%s", createdDirectoryPath.c_str());
        (void)FileUtils::RemoveAll(createdDirectoryPath.string());
        return false;
    }
    trackedDirectoryPath_ = std::move(resolvedCreatedDirectoryPath);
    automaticCleanupEnabled_ = true;
    ASCENDLOGD("Created temporary directory: path=%s", trackedDirectoryPath_.c_str());
    return true;
}

bool ScopedTemporaryDirectory::TrackDirectoryForAutomaticCleanup(std::string directoryPath)
{
    if (directoryPath.empty()) {
        ASCENDLOGE("Cannot track an empty temporary directory path for automatic cleanup");
        return false;
    }
    if (!trackedDirectoryPath_.empty()) {
        ASCENDLOGE("Temporary directory already tracks a path: path=%s", trackedDirectoryPath_.c_str());
        return false;
    }
    trackedDirectoryPath_ = std::move(directoryPath);
    automaticCleanupEnabled_ = true;
    return true;
}

bool ScopedTemporaryDirectory::RemoveTrackedDirectory() noexcept
{
    if (trackedDirectoryPath_.empty()) {
        return true;
    }
    if (!FileUtils::RemoveAll(trackedDirectoryPath_)) {
        ASCENDLOGW("Failed to remove tracked temporary directory: path=%s", trackedDirectoryPath_.c_str());
        return false;
    }
    trackedDirectoryPath_.clear();
    return true;
}

void ScopedTemporaryDirectory::DisableAutomaticCleanup() noexcept { automaticCleanupEnabled_ = false; }

const std::string& ScopedTemporaryDirectory::GetDirectoryPath() const noexcept { return trackedDirectoryPath_; }

void ScopedTemporaryDirectory::MoveStateFrom(ScopedTemporaryDirectory& other) noexcept
{
    trackedDirectoryPath_ = std::move(other.trackedDirectoryPath_);
    automaticCleanupEnabled_ = other.automaticCleanupEnabled_;
    other.trackedDirectoryPath_.clear();
    other.automaticCleanupEnabled_ = false;
}

} // namespace ascendc
