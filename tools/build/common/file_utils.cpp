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

#include <cerrno>
#include <chrono>
#include <cstdio>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <limits>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace ascendc {
namespace {

bool AssignString(const char* value, std::string& output) noexcept
{
    try {
        output.assign(value);
        return true;
    } catch (...) {
        output.clear();
        return false;
    }
}

} // namespace

std::string FileUtils::JoinPath(const std::string& left, const std::string& right)
{
    if (left.empty()) {
        return right;
    }
    if (right.empty()) {
        return left;
    }
    return left[left.size() - 1U] == '/' ? left + right : left + "/" + right;
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
    if (path.empty() || path[path.size() - 1U] == '/') {
        return std::string();
    }
    const size_t separator = path.find_last_of('/');
    return separator == std::string::npos ? path : path.substr(separator + 1U);
}

bool FileUtils::CurrentDirectory(std::string& current) noexcept
{
    char buffer[PATH_MAX] = {0};
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        current.clear();
        return false;
    }
    return AssignString(buffer, current);
}

bool FileUtils::MakeAbsolutePath(const std::string& path, std::string& absolute) noexcept
{
    try {
        if (!path.empty() && path[0] == '/') {
            return AssignString(path.c_str(), absolute);
        }
        std::string current;
        if (!CurrentDirectory(current)) {
            absolute.clear();
            return false;
        }
        absolute = JoinPath(current, path);
        return true;
    } catch (...) {
        absolute.clear();
        return false;
    }
}

std::vector<std::string> FileUtils::SplitPath(const std::string& path)
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

bool FileUtils::IsPathWithin(const std::string& path, const std::string& root) noexcept
{
    if (path.empty() || root.empty() || (path[0] == '/') != (root[0] == '/')) {
        return false;
    }
    if (root == "/") {
        return path[0] == '/';
    }
    size_t pathBegin = 0U;
    size_t rootBegin = 0U;
    bool matchedComponent = false;
    while (true) {
        while (rootBegin < root.size() && root[rootBegin] == '/') {
            ++rootBegin;
        }
        if (rootBegin == root.size()) {
            return matchedComponent;
        }
        while (pathBegin < path.size() && path[pathBegin] == '/') {
            ++pathBegin;
        }
        if (pathBegin == path.size()) {
            return false;
        }
        const size_t rootEnd = root.find('/', rootBegin);
        const size_t pathEnd = path.find('/', pathBegin);
        const size_t rootLength = (rootEnd == std::string::npos ? root.size() : rootEnd) - rootBegin;
        const size_t pathLength = (pathEnd == std::string::npos ? path.size() : pathEnd) - pathBegin;
        if (rootLength != pathLength || path.compare(pathBegin, pathLength, root, rootBegin, rootLength) != 0) {
            return false;
        }
        matchedComponent = true;
        rootBegin = rootEnd == std::string::npos ? root.size() : rootEnd;
        pathBegin = pathEnd == std::string::npos ? path.size() : pathEnd;
    }
}

bool FileUtils::PathExists(const std::string& path) noexcept
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0;
}

bool FileUtils::IsDirectory(const std::string& path) noexcept
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISDIR(info.st_mode);
}

bool FileUtils::IsRegularFile(const std::string& path) noexcept
{
    struct stat info {};
    return stat(path.c_str(), &info) == 0 && S_ISREG(info.st_mode);
}

bool FileUtils::IsSymlink(const std::string& path) noexcept
{
    struct stat info {};
    return lstat(path.c_str(), &info) == 0 && S_ISLNK(info.st_mode);
}

bool FileUtils::ResolveCanonicalPath(const std::string& path, std::string& resolved) noexcept
{
    char resolvedPath[PATH_MAX] = {0};
    if (realpath(path.c_str(), resolvedPath) == nullptr) {
        resolved.clear();
        return false;
    }
    return AssignString(resolvedPath, resolved);
}

bool FileUtils::ResolveDirectory(const std::string& path, std::string& resolved) noexcept
{
    struct stat inputInfo {};
    if (lstat(path.c_str(), &inputInfo) != 0 || S_ISLNK(inputInfo.st_mode)) {
        resolved.clear();
        return false;
    }
    std::string canonical;
    if (!ResolveCanonicalPath(path, canonical) || !IsDirectory(canonical)) {
        resolved.clear();
        return false;
    }
    resolved.swap(canonical);
    return true;
}

bool FileUtils::ResolveSubdirectory(const std::string& path, const std::string& root, std::string& resolved) noexcept
{
    std::string canonical;
    if (!ResolveDirectory(path, canonical) || !IsPathWithin(canonical, root)) {
        resolved.clear();
        return false;
    }
    resolved.swap(canonical);
    return true;
}

bool FileUtils::EnsureDirectory(const char* path) noexcept
{
    if (path == nullptr || path[0] == '\0' || (path[0] == '/' && path[1] == '\0')) {
        return true;
    }
    if (mkdir(path, 0700) == 0) {
        return true;
    }
    if (errno != EEXIST) {
        return false;
    }
    struct stat info {};
    return stat(path, &info) == 0 && S_ISDIR(info.st_mode);
}

bool FileUtils::CreateDirectories(const std::string& path) noexcept
{
    if (path.empty()) {
        return true;
    }
    if (path.size() >= PATH_MAX) {
        return false;
    }
    char component[PATH_MAX] = {0};
    for (size_t index = 0U; index < path.size(); ++index) {
        component[index] = path[index];
    }
    size_t begin = path[0] == '/' ? 1U : 0U;
    while (begin < path.size()) {
        const size_t separator = path.find('/', begin);
        if (separator != std::string::npos) {
            component[separator] = '\0';
        }
        if (!EnsureDirectory(component)) {
            return false;
        }
        if (separator == std::string::npos) {
            break;
        }
        component[separator] = '/';
        begin = separator + 1U;
    }
    return true;
}

bool FileUtils::ListDirectory(
    const std::string& directory, bool recursive, bool ignoreOpenFailure, std::vector<std::string>& paths) noexcept
{
    DIR* stream = opendir(directory.c_str());
    if (stream == nullptr) {
        return ignoreOpenFailure;
    }
    try {
        while (true) {
            errno = 0;
            dirent* entry = readdir(stream);
            if (entry == nullptr) {
                const int readError = errno;
                const int closeResult = closedir(stream);
                return readError == 0 && closeResult == 0;
            }
            const std::string name(entry->d_name);
            if (name == "." || name == "..") {
                continue;
            }
            const std::string path = JoinPath(directory, name);
            paths.push_back(path);
            if (recursive && IsDirectory(path) && !IsSymlink(path) && !ListDirectory(path, true, true, paths)) {
                (void)closedir(stream);
                return false;
            }
        }
    } catch (...) {
        if (stream != nullptr) {
            (void)closedir(stream);
        }
        return false;
    }
}

bool FileUtils::CreateTempDirectory(std::string& path) noexcept
{
    try {
        const auto timestamp =
            std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())
                .count();
        for (uint64_t suffix = 0U; suffix < 1000U; ++suffix) {
            path = "/tmp/" + std::to_string(timestamp + static_cast<int64_t>(suffix));
            if (mkdir(path.c_str(), 0700) == 0) {
                return true;
            }
            if (errno != EEXIST) {
                break;
            }
        }
    } catch (...) {
        path.clear();
        return false;
    }
    path.clear();
    return false;
}

bool FileUtils::RemoveAllImpl(const std::string& path)
{
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        return errno == ENOENT;
    }
    if (!S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        return unlink(path.c_str()) == 0 || errno == ENOENT;
    }
    DIR* directory = opendir(path.c_str());
    if (directory == nullptr) {
        return false;
    }
    bool success = true;
    while (true) {
        errno = 0;
        dirent* entry = readdir(directory);
        if (entry == nullptr) {
            if (errno != 0) {
                success = false;
            }
            break;
        }
        const std::string name(entry->d_name);
        if (name != "." && name != ".." && !RemoveAllImpl(JoinPath(path, name))) {
            success = false;
        }
    }
    if (closedir(directory) != 0) {
        success = false;
    }
    return (rmdir(path.c_str()) == 0 || errno == ENOENT) && success;
}

bool FileUtils::RemoveAll(const std::string& path) noexcept
{
    try {
        return RemoveAllImpl(path);
    } catch (...) {
        return false;
    }
}

bool FileUtils::WriteBinaryFile(const std::string& path, const std::vector<uint8_t>& bytes) noexcept
{
    try {
        if (!CreateDirectories(ParentPath(path))) {
            return false;
        }
        std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
        if (!output.is_open()) {
            return false;
        }
        if (!bytes.empty()) {
            output.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
        }
        output.close();
        return static_cast<bool>(output);
    } catch (...) {
        return false;
    }
}

bool FileUtils::ReadRegularFile(const std::string& path, uintmax_t maximum, std::vector<uint8_t>& data) noexcept
{
    data.clear();
    const int descriptor = open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (descriptor < 0) {
        return false;
    }
    struct stat info {};
    if (fstat(descriptor, &info) != 0 || !S_ISREG(info.st_mode) || info.st_size < 0 ||
        static_cast<uintmax_t>(info.st_size) > maximum ||
        static_cast<uintmax_t>(info.st_size) > static_cast<uintmax_t>(std::numeric_limits<size_t>::max())) {
        (void)close(descriptor);
        return false;
    }
    try {
        data.resize(static_cast<size_t>(info.st_size));
    } catch (...) {
        (void)close(descriptor);
        data.clear();
        return false;
    }
    size_t offset = 0U;
    while (offset < data.size()) {
        const ssize_t count = read(descriptor, data.data() + offset, data.size() - offset);
        if (count < 0 && errno == EINTR) {
            continue;
        }
        if (count <= 0) {
            (void)close(descriptor);
            data.clear();
            return false;
        }
        offset += static_cast<size_t>(count);
    }
    uint8_t extra = 0U;
    ssize_t extraCount = -1;
    do {
        extraCount = read(descriptor, &extra, 1U);
    } while (extraCount < 0 && errno == EINTR);
    const int closeResult = close(descriptor);
    const bool success = extraCount == 0 && closeResult == 0;
    if (!success) {
        data.clear();
        return false;
    }
    return true;
}

bool FileUtils::OpenExclusiveOutput(const std::string& path, std::ofstream& output) noexcept
{
    if (output.is_open()) {
        return false;
    }
    const int descriptor = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (descriptor < 0) {
        return false;
    }
    if (close(descriptor) != 0) {
        (void)unlink(path.c_str());
        return false;
    }
    try {
        output.open(path.c_str(), std::ios::trunc);
    } catch (...) {
        (void)unlink(path.c_str());
        return false;
    }
    if (output.is_open()) {
        return true;
    }
    (void)unlink(path.c_str());
    return false;
}

bool FileUtils::FinalizeOutput(std::ofstream& output) noexcept
{
    try {
        output.flush();
        const bool writeSucceeded = static_cast<bool>(output);
        output.close();
        return writeSucceeded && static_cast<bool>(output);
    } catch (...) {
        try {
            output.close();
        } catch (...) {
        }
        return false;
    }
}

bool FileUtils::CheckNonSymlink(const std::string& path) noexcept
{
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        return errno == ENOENT;
    }
    return !S_ISLNK(info.st_mode);
}

bool FileUtils::CopyFileExclusive(const std::string& source, const std::string& destination) noexcept
{
    const int input = open(source.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (input < 0) {
        return false;
    }
    struct stat info {};
    if (fstat(input, &info) != 0 || !S_ISREG(info.st_mode)) {
        (void)close(input);
        return false;
    }
    const int output = open(destination.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, info.st_mode & 0777);
    if (output < 0) {
        (void)close(input);
        return false;
    }
    bool success = true;
    uint8_t buffer[64U * 1024U] = {0};
    while (success) {
        const ssize_t count = read(input, buffer, sizeof(buffer));
        if (count < 0 && errno == EINTR) {
            continue;
        }
        if (count < 0) {
            success = false;
            break;
        }
        if (count == 0) {
            break;
        }
        size_t offset = 0U;
        while (offset < static_cast<size_t>(count)) {
            const ssize_t written = write(output, buffer + offset, static_cast<size_t>(count) - offset);
            if (written < 0 && errno == EINTR) {
                continue;
            }
            if (written <= 0) {
                success = false;
                break;
            }
            offset += static_cast<size_t>(written);
        }
    }
    success = close(input) == 0 && success;
    success = close(output) == 0 && success;
    if (!success) {
        (void)unlink(destination.c_str());
        return false;
    }
    return true;
}

} // namespace ascendc
