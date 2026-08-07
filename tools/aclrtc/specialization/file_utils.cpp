/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "file_utils.h"

#include <algorithm>
#include <cerrno>
#include <chrono>
#include <cstring>
#include <dirent.h>
#include <exception>
#include <fstream>
#include <limits.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ascendc_tool_log.h"

namespace ascendc {
namespace resource {

std::string FileUtils::JoinPath(const std::string& left, const std::string& right)
{
    if (left.empty()) {
        return right;
    }
    if (right.empty()) {
        return left;
    }
    return left.back() == '/' ? left + right : left + "/" + right;
}

std::string FileUtils::ParentPath(const std::string& path)
{
    const size_t separator = path.find_last_of('/');
    if (separator == std::string::npos) {
        return std::string();
    }
    return separator == 0U ? "/" : path.substr(0U, separator);
}

std::string FileUtils::FileName(const std::string& path)
{
    if (path.empty() || path.back() == '/') {
        return std::string();
    }
    const size_t separator = path.find_last_of('/');
    return separator == std::string::npos ? path : path.substr(separator + 1U);
}

std::vector<std::string> FileUtils::PathComponents(const std::string& path)
{
    std::vector<std::string> components;
    size_t begin = 0U;
    while (begin < path.size()) {
        while (begin < path.size() && path[begin] == '/') {
            ++begin;
        }
        if (begin == path.size()) {
            break;
        }
        const size_t end = path.find('/', begin);
        components.push_back(path.substr(begin, end == std::string::npos ? std::string::npos : end - begin));
        if (end == std::string::npos) {
            break;
        }
        begin = end + 1U;
    }
    return components;
}

bool FileUtils::IsWithinPath(const std::string& path, const std::string& root)
{
    const std::vector<std::string> pathComponents = PathComponents(path);
    const std::vector<std::string> rootComponents = PathComponents(root);
    return !pathComponents.empty() && !rootComponents.empty() && pathComponents.size() >= rootComponents.size() &&
           std::equal(rootComponents.begin(), rootComponents.end(), pathComponents.begin());
}

bool FileUtils::IsDirectory(const std::string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

bool FileUtils::IsRegularFile(const std::string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool FileUtils::Exists(const std::string& path)
{
    struct stat info{};
    return stat(path.c_str(), &info) == 0;
}

bool FileUtils::IsSymlink(const std::string& path)
{
    struct stat info{};
    return lstat(path.c_str(), &info) == 0 && S_ISLNK(info.st_mode);
}

bool FileUtils::CanonicalPath(const std::string& path, std::string& canonical)
{
    char resolvedPath[PATH_MAX] = {0};
    if (realpath(path.c_str(), resolvedPath) == nullptr) {
        return false;
    }
    canonical.assign(resolvedPath);
    return true;
}

bool FileUtils::EnsureDirectory(const std::string& path)
{
    if (path.empty() || path == "/") {
        return true;
    }
    if (mkdir(path.c_str(), 0700) == 0) {
        ASCENDLOGD("Created resource directory: path=%s", path.c_str());
        return true;
    }
    const int mkdirError = errno;
    if (mkdirError == EEXIST && IsDirectory(path)) {
        return true;
    }
    ASCENDLOGE("Failed to create resource directory: path=%s, error=%s", path.c_str(), std::strerror(mkdirError));
    return false;
}

bool FileUtils::CreateDirectories(const std::string& path)
{
    if (path.empty()) {
        return true;
    }
    size_t begin = path[0] == '/' ? 1U : 0U;
    while (begin < path.size()) {
        const size_t separator = path.find('/', begin);
        const std::string component = path.substr(0U, separator);
        if (!component.empty() && !EnsureDirectory(component)) {
            return false;
        }
        if (separator == std::string::npos) {
            break;
        }
        begin = separator + 1U;
    }
    return true;
}

bool FileUtils::WalkDirectory(
    const std::string& directory, bool recursive, bool ignoreOpenFailure, std::vector<std::string>& paths)
{
    DIR* stream = opendir(directory.c_str());
    if (stream == nullptr) {
        if (ignoreOpenFailure) {
            ASCENDLOGW(
                "Skipping unreadable automatic compile resource directory: path=%s, error=%s", directory.c_str(),
                std::strerror(errno));
            return true;
        }
        ASCENDLOGE(
            "Failed to read compile resource directory: path=%s, error=%s", directory.c_str(), std::strerror(errno));
        return false;
    }
    while (dirent* entry = readdir(stream)) {
        const std::string name(entry->d_name);
        if (name == "." || name == "..") {
            continue;
        }
        const std::string path = JoinPath(directory, name);
        paths.push_back(path);
        if (recursive && IsDirectory(path) && !IsSymlink(path) && !WalkDirectory(path, true, true, paths)) {
            (void)closedir(stream);
            return false;
        }
    }
    return closedir(stream) == 0;
}

bool FileUtils::MakeTempRoot(std::string& root)
{
    const auto timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    for (uint64_t suffix = 0U; suffix < 1000U; ++suffix) {
        const std::string candidate = JoinPath("/tmp", std::to_string(timestamp + static_cast<int64_t>(suffix)));
        if (mkdir(candidate.c_str(), 0700) == 0) {
            root = candidate;
            ASCENDLOGI("Created compile resource temporary root: path=%s", root.c_str());
            return true;
        }
        if (errno != EEXIST) {
            ASCENDLOGE("Failed to create compile resource temporary root: error=%s", std::strerror(errno));
            return false;
        }
    }
    ASCENDLOGE("Failed to allocate a unique compile resource temporary root");
    return false;
}

bool FileUtils::WriteFile(const std::string& path, const std::vector<uint8_t>& bytes)
{
    if (!CreateDirectories(ParentPath(path))) {
        return false;
    }
    std::ofstream output(path, std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        ASCENDLOGE("Failed to open materialized resource file: path=%s", path.c_str());
        return false;
    }
    if (!bytes.empty()) {
        output.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }
    if (!output) {
        ASCENDLOGE("Failed to write materialized resource file: path=%s", path.c_str());
        return false;
    }
    ASCENDLOGD("Materialized resource file: path=%s, size=%zu", path.c_str(), bytes.size());
    return true;
}

bool FileUtils::RemoveTreeImpl(const std::string& path)
{
    struct stat info{};
    if (lstat(path.c_str(), &info) != 0) {
        return errno == ENOENT;
    }
    if (!S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        return unlink(path.c_str()) == 0 || errno == ENOENT;
    }
    bool success = true;
    DIR* directory = opendir(path.c_str());
    if (directory == nullptr) {
        return false;
    }
    while (dirent* entry = readdir(directory)) {
        const std::string name(entry->d_name);
        if (name != "." && name != "..") {
            success = RemoveTreeImpl(JoinPath(path, name)) && success;
        }
    }
    success = closedir(directory) == 0 && success;
    return (rmdir(path.c_str()) == 0 || errno == ENOENT) && success;
}

void FileUtils::RemoveTree(const std::string& path) noexcept
{
    try {
        if (!RemoveTreeImpl(path)) {
            ASCENDLOGW("Failed to completely remove compile resource path: path=%s", path.c_str());
        }
    } catch (const std::exception& error) {
        ASCENDLOGW("Compile resource cleanup stopped unexpectedly: path=%s, error=%s", path.c_str(), error.what());
    } catch (...) {
        ASCENDLOGW("Compile resource cleanup stopped unexpectedly: path=%s", path.c_str());
    }
}

} // namespace resource
} // namespace ascendc
