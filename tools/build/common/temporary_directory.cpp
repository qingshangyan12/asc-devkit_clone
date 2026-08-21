/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "temporary_directory.h"

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

bool IsSinglePathComponent(const std::string& namePattern)
{
    if (namePattern.empty() || namePattern.find('%') == std::string::npos ||
        namePattern.find('\0') != std::string::npos || namePattern.find('\\') != std::string::npos) {
        return false;
    }
    const fs::path patternPath(namePattern);
    return patternPath.is_relative() && !patternPath.has_parent_path();
}

} // namespace

TemporaryDirectory::~TemporaryDirectory() noexcept
{
    if (removeAfterDestruction_) {
        (void)RemoveNow();
    }
}

TemporaryDirectory::TemporaryDirectory(TemporaryDirectory&& other) noexcept { TakeCleanupOwnershipFrom(other); }

TemporaryDirectory& TemporaryDirectory::operator=(TemporaryDirectory&& other) noexcept
{
    if (this == &other) {
        return *this;
    }
    if (removeAfterDestruction_ && !RemoveNow()) {
        ASCENDLOGW(
            "Temporary directory cleanup ownership was not transferred because the previously owned path could not "
            "be removed: path=%s",
            ownedDirectoryPath_.c_str());
        return *this;
    }
    TakeCleanupOwnershipFrom(other);
    return *this;
}

bool TemporaryDirectory::CreateUniqueUnder(const std::string& parentDirectoryPath, const std::string& namePattern)
{
    if (!ownedDirectoryPath_.empty()) {
        ASCENDLOGE("Temporary directory already owns a path: path=%s", ownedDirectoryPath_.c_str());
        return false;
    }
    if (!IsSinglePathComponent(namePattern)) {
        ASCENDLOGE(
            "Temporary directory name pattern must be one path component containing %% placeholders: pattern=%s",
            namePattern.c_str());
        return false;
    }

    std::string resolvedParentDirectoryPath;
    if (!FileUtils::ResolveDirectory(parentDirectoryPath, resolvedParentDirectoryPath)) {
        ASCENDLOGE("Temporary directory parent is unavailable: parent=%s", parentDirectoryPath.c_str());
        return false;
    }
    boost::system::error_code filesystemError;
    const fs::path uniqueName = fs::unique_path(namePattern, filesystemError);
    if (filesystemError || uniqueName.empty() || uniqueName.has_parent_path()) {
        ASCENDLOGE(
            "Failed to generate temporary directory name: parent=%s pattern=%s error=%s",
            resolvedParentDirectoryPath.c_str(), namePattern.c_str(),
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
    ownedDirectoryPath_ = std::move(resolvedCreatedDirectoryPath);
    removeAfterDestruction_ = true;
    ASCENDLOGD("Created temporary directory: path=%s", ownedDirectoryPath_.c_str());
    return true;
}

bool TemporaryDirectory::AdoptExistingForCleanup(std::string directoryPath)
{
    if (directoryPath.empty()) {
        ASCENDLOGE("Cannot adopt an empty temporary directory path");
        return false;
    }
    if (!ownedDirectoryPath_.empty()) {
        ASCENDLOGE("Temporary directory already owns a path: path=%s", ownedDirectoryPath_.c_str());
        return false;
    }
    ownedDirectoryPath_ = std::move(directoryPath);
    removeAfterDestruction_ = true;
    return true;
}

bool TemporaryDirectory::RemoveNow() noexcept
{
    if (ownedDirectoryPath_.empty()) {
        return true;
    }
    if (!FileUtils::RemoveAll(ownedDirectoryPath_)) {
        ASCENDLOGW("Failed to remove owned temporary directory: path=%s", ownedDirectoryPath_.c_str());
        return false;
    }
    ownedDirectoryPath_.clear();
    return true;
}

void TemporaryDirectory::RetainAfterDestruction() noexcept { removeAfterDestruction_ = false; }

const std::string& TemporaryDirectory::GetPath() const noexcept { return ownedDirectoryPath_; }

void TemporaryDirectory::TakeCleanupOwnershipFrom(TemporaryDirectory& other) noexcept
{
    ownedDirectoryPath_ = std::move(other.ownedDirectoryPath_);
    removeAfterDestruction_ = other.removeAfterDestruction_;
    other.ownedDirectoryPath_.clear();
    other.removeAfterDestruction_ = false;
}

} // namespace ascendc
