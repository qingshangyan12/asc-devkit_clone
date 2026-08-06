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
#include <cstring>
#include <dirent.h>
#include <fcntl.h>
#include <limits.h>
#include <limits>
#include <memory>

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "ascendc_tool_log.h"

namespace ascendc {
namespace {

struct DirectoryCloser {
    void operator()(DIR* directory) const noexcept
    {
        if (directory != nullptr) {
            (void)closedir(directory);
        }
    }
};

using DirectoryHandle = std::unique_ptr<DIR, DirectoryCloser>;

class FileDescriptor final {
public:
    explicit FileDescriptor(int descriptor) noexcept : descriptor_(descriptor) {}

    ~FileDescriptor() { (void)Close(); }

    FileDescriptor(const FileDescriptor&) = delete;
    FileDescriptor& operator=(const FileDescriptor&) = delete;

    bool IsValid() const noexcept { return descriptor_ >= 0; }
    int Get() const noexcept { return descriptor_; }

    int Close() noexcept
    {
        if (!IsValid()) {
            return 0;
        }
        const int descriptor = descriptor_;
        descriptor_ = -1;
        return close(descriptor);
    }

private:
    int descriptor_;
};

bool ListDirectoryImpl(
    const std::string& directory, bool recursive, bool ignoreOpenFailure, std::vector<std::string>& paths)
{
    DirectoryHandle stream(opendir(directory.c_str()));
    if (!stream) {
        const int openError = errno;
        if (ignoreOpenFailure) {
            ASCENDLOGD(
                "Ignoring directory open failure: path=%s error=%s", directory.c_str(), std::strerror(openError));
            return true;
        }
        ASCENDLOGW("Failed to open directory: path=%s error=%s", directory.c_str(), std::strerror(openError));
        return false;
    }
    while (true) {
        errno = 0;
        dirent* entry = readdir(stream.get());
        if (entry == nullptr) {
            const int readError = errno;
            const int closeResult = closedir(stream.release());
            if (readError != 0) {
                ASCENDLOGW("Failed to read directory: path=%s error=%s", directory.c_str(), std::strerror(readError));
                return false;
            }
            if (closeResult != 0) {
                const int closeError = errno;
                ASCENDLOGW("Failed to close directory: path=%s error=%s", directory.c_str(), std::strerror(closeError));
                return false;
            }
            return true;
        }
        const std::string name(entry->d_name);
        if (name == "." || name == "..") {
            continue;
        }
        const std::string path = FileUtils::JoinPath(directory, name);
        paths.push_back(path);
        if (recursive && FileUtils::IsDirectory(path) && !FileUtils::IsSymlink(path) &&
            !ListDirectoryImpl(path, true, ignoreOpenFailure, paths)) {
            return false;
        }
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

bool FileUtils::CurrentDirectory(std::string& current)
{
    char buffer[PATH_MAX] = {0};
    if (getcwd(buffer, sizeof(buffer)) == nullptr) {
        const int currentDirectoryError = errno;
        ASCENDLOGW("Failed to get current directory: error=%s", std::strerror(currentDirectoryError));
        current.clear();
        return false;
    }
    current.assign(buffer);
    return true;
}

bool FileUtils::MakeAbsolutePath(const std::string& path, std::string& absolute)
{
    ASCENDLOGD("Making path absolute: path=%s", path.c_str());
    if (!path.empty() && path[0] == '/') {
        absolute = path;
        ASCENDLOGD("Path is already absolute: path=%s", absolute.c_str());
        return true;
    }
    std::string current;
    if (!CurrentDirectory(current)) {
        absolute.clear();
        return false;
    }
    absolute = JoinPath(current, path);
    ASCENDLOGD("Made path absolute: input=%s absolute=%s", path.c_str(), absolute.c_str());
    return true;
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

bool FileUtils::IsPathWithin(const std::string& path, const std::string& root)
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

bool FileUtils::ResolveCanonicalPath(const std::string& path, std::string& resolved)
{
    ASCENDLOGD("Resolving canonical path: path=%s", path.c_str());
    char resolvedPath[PATH_MAX] = {0};
    if (realpath(path.c_str(), resolvedPath) == nullptr) {
        const int resolveError = errno;
        ASCENDLOGW("Failed to resolve canonical path: path=%s error=%s", path.c_str(), std::strerror(resolveError));
        resolved.clear();
        return false;
    }
    resolved.assign(resolvedPath);
    ASCENDLOGD("Resolved canonical path: path=%s resolved=%s", path.c_str(), resolved.c_str());
    return true;
}

bool FileUtils::ResolveDirectory(const std::string& path, std::string& resolved)
{
    ASCENDLOGD("Resolving directory: path=%s", path.c_str());
    struct stat inputInfo {};
    if (lstat(path.c_str(), &inputInfo) != 0) {
        const int inspectError = errno;
        ASCENDLOGW("Failed to inspect directory: path=%s error=%s", path.c_str(), std::strerror(inspectError));
        resolved.clear();
        return false;
    }
    if (S_ISLNK(inputInfo.st_mode)) {
        ASCENDLOGW("Rejected symlink directory: path=%s", path.c_str());
        resolved.clear();
        return false;
    }
    std::string canonical;
    if (!ResolveCanonicalPath(path, canonical)) {
        resolved.clear();
        return false;
    }
    if (!IsDirectory(canonical)) {
        ASCENDLOGW("Resolved path is not a directory: path=%s resolved=%s", path.c_str(), canonical.c_str());
        resolved.clear();
        return false;
    }
    resolved.swap(canonical);
    ASCENDLOGD("Resolved directory: path=%s resolved=%s", path.c_str(), resolved.c_str());
    return true;
}

bool FileUtils::ResolveSubdirectory(const std::string& path, const std::string& root, std::string& resolved)
{
    ASCENDLOGD("Resolving subdirectory: path=%s root=%s", path.c_str(), root.c_str());
    std::string canonical;
    if (!ResolveDirectory(path, canonical)) {
        resolved.clear();
        return false;
    }
    if (!IsPathWithin(canonical, root)) {
        ASCENDLOGW(
            "Resolved directory is outside root: path=%s resolved=%s root=%s", path.c_str(), canonical.c_str(),
            root.c_str());
        resolved.clear();
        return false;
    }
    resolved.swap(canonical);
    ASCENDLOGD("Resolved subdirectory: path=%s resolved=%s", path.c_str(), resolved.c_str());
    return true;
}

bool FileUtils::EnsureDirectory(const char* path) noexcept
{
    if (path == nullptr || path[0] == '\0' || (path[0] == '/' && path[1] == '\0')) {
        return true;
    }
    if (mkdir(path, 0700) == 0) {
        ASCENDLOGD("Created directory component: path=%s", path);
        return true;
    }
    if (errno != EEXIST) {
        const int createError = errno;
        ASCENDLOGW("Failed to create directory component: path=%s error=%s", path, std::strerror(createError));
        return false;
    }
    struct stat info {};
    if (stat(path, &info) != 0) {
        const int inspectError = errno;
        ASCENDLOGW("Failed to inspect existing path: path=%s error=%s", path, std::strerror(inspectError));
        return false;
    }
    if (!S_ISDIR(info.st_mode)) {
        ASCENDLOGW("Existing path is not a directory: path=%s", path);
        return false;
    }
    return true;
}

bool FileUtils::CreateDirectories(const std::string& path)
{
    ASCENDLOGD("Creating directories: path=%s", path.c_str());
    if (path.empty()) {
        return true;
    }
    if (path.size() >= PATH_MAX) {
        ASCENDLOGW("Directory path exceeds PATH_MAX: path=%s size=%zu", path.c_str(), path.size());
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
    ASCENDLOGD("Created or verified directories: path=%s", path.c_str());
    return true;
}

bool FileUtils::ListDirectory(
    const std::string& directory, bool recursive, bool ignoreOpenFailure, std::vector<std::string>& paths)
{
    ASCENDLOGD("Listing directory: path=%s recursive=%s", directory.c_str(), recursive ? "true" : "false");
    const size_t initialCount = paths.size();
    const bool success = ListDirectoryImpl(directory, recursive, ignoreOpenFailure, paths);
    if (success) {
        ASCENDLOGD("Listed directory: path=%s entries=%zu", directory.c_str(), paths.size() - initialCount);
    }
    return success;
}

bool FileUtils::CreateTempDirectory(std::string& path)
{
    ASCENDLOGD("Creating temporary directory under /tmp");
    const auto timestamp =
        std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now().time_since_epoch())
            .count();
    bool exhaustedCandidates = true;
    for (uint64_t suffix = 0U; suffix < 1000U; ++suffix) {
        path = "/tmp/" + std::to_string(timestamp + static_cast<int64_t>(suffix));
        if (mkdir(path.c_str(), 0700) == 0) {
            ASCENDLOGI("Created temporary directory: path=%s", path.c_str());
            return true;
        }
        if (errno != EEXIST) {
            const int createError = errno;
            ASCENDLOGW(
                "Failed to create temporary directory: path=%s error=%s", path.c_str(), std::strerror(createError));
            exhaustedCandidates = false;
            break;
        }
    }
    if (exhaustedCandidates) {
        ASCENDLOGW("Exhausted temporary directory candidates: last_path=%s", path.c_str());
    }
    path.clear();
    return false;
}

bool FileUtils::RemoveAllImpl(const std::string& path)
{
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        const int inspectError = errno;
        if (inspectError == ENOENT) {
            return true;
        }
        ASCENDLOGW("Failed to inspect removal path: path=%s error=%s", path.c_str(), std::strerror(inspectError));
        return false;
    }
    if (!S_ISDIR(info.st_mode) || S_ISLNK(info.st_mode)) {
        if (unlink(path.c_str()) == 0) {
            return true;
        }
        const int unlinkError = errno;
        if (unlinkError == ENOENT) {
            return true;
        }
        ASCENDLOGW("Failed to remove file: path=%s error=%s", path.c_str(), std::strerror(unlinkError));
        return false;
    }
    DirectoryHandle directory(opendir(path.c_str()));
    if (!directory) {
        const int openError = errno;
        ASCENDLOGW("Failed to open directory for removal: path=%s error=%s", path.c_str(), std::strerror(openError));
        return false;
    }
    bool success = true;
    while (true) {
        errno = 0;
        dirent* entry = readdir(directory.get());
        if (entry == nullptr) {
            if (errno != 0) {
                const int readError = errno;
                ASCENDLOGW(
                    "Failed to read directory for removal: path=%s error=%s", path.c_str(), std::strerror(readError));
                success = false;
            }
            break;
        }
        const std::string name(entry->d_name);
        if (name != "." && name != ".." && !RemoveAllImpl(JoinPath(path, name))) {
            success = false;
        }
    }
    if (closedir(directory.release()) != 0) {
        const int closeError = errno;
        ASCENDLOGW(
            "Failed to close directory after removal: path=%s error=%s", path.c_str(), std::strerror(closeError));
        success = false;
    }
    if (rmdir(path.c_str()) != 0) {
        const int removeError = errno;
        if (removeError != ENOENT) {
            ASCENDLOGW("Failed to remove directory: path=%s error=%s", path.c_str(), std::strerror(removeError));
            success = false;
        }
    }
    return success;
}

bool FileUtils::RemoveAll(const std::string& path)
{
    ASCENDLOGD("Removing path recursively: path=%s", path.c_str());
    const bool success = RemoveAllImpl(path);
    if (success) {
        ASCENDLOGI("Removed path recursively: path=%s", path.c_str());
    }
    return success;
}

bool FileUtils::WriteBinaryFile(const std::string& path, const std::vector<uint8_t>& bytes)
{
    ASCENDLOGD("Writing binary file: path=%s bytes=%zu", path.c_str(), bytes.size());
    if (!CreateDirectories(ParentPath(path))) {
        return false;
    }
    std::ofstream output(path.c_str(), std::ios::binary | std::ios::trunc);
    if (!output.is_open()) {
        ASCENDLOGW("Failed to open binary file for writing: path=%s", path.c_str());
        return false;
    }
    if (!bytes.empty()) {
        output.write(reinterpret_cast<const char*>(bytes.data()), static_cast<std::streamsize>(bytes.size()));
    }
    output.close();
    if (!output) {
        ASCENDLOGW("Failed to write or close binary file: path=%s", path.c_str());
        return false;
    }
    ASCENDLOGI("Wrote binary file: path=%s bytes=%zu", path.c_str(), bytes.size());
    return true;
}

bool FileUtils::ReadRegularFile(const std::string& path, uintmax_t maximum, std::vector<uint8_t>& data)
{
    ASCENDLOGD("Reading regular file: path=%s maximum=%ju", path.c_str(), maximum);
    data.clear();
    FileDescriptor descriptor(open(path.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW));
    if (!descriptor.IsValid()) {
        const int openError = errno;
        ASCENDLOGW("Failed to open regular file: path=%s error=%s", path.c_str(), std::strerror(openError));
        return false;
    }
    struct stat info {};
    if (fstat(descriptor.Get(), &info) != 0) {
        const int inspectError = errno;
        ASCENDLOGW("Failed to inspect regular file: path=%s error=%s", path.c_str(), std::strerror(inspectError));
        return false;
    }
    if (!S_ISREG(info.st_mode) || info.st_size < 0) {
        ASCENDLOGW("Rejected non-regular file: path=%s", path.c_str());
        return false;
    }
    const uintmax_t fileSize = static_cast<uintmax_t>(info.st_size);
    if (fileSize > maximum || fileSize > static_cast<uintmax_t>(std::numeric_limits<size_t>::max())) {
        ASCENDLOGW("Regular file exceeds size limit: path=%s size=%ju maximum=%ju", path.c_str(), fileSize, maximum);
        return false;
    }
    data.resize(static_cast<size_t>(fileSize));
    size_t offset = 0U;
    while (offset < data.size()) {
        const ssize_t count = read(descriptor.Get(), data.data() + offset, data.size() - offset);
        if (count < 0 && errno == EINTR) {
            continue;
        }
        if (count <= 0) {
            if (count < 0) {
                const int readError = errno;
                ASCENDLOGW("Failed to read regular file: path=%s error=%s", path.c_str(), std::strerror(readError));
            } else {
                ASCENDLOGW(
                    "Regular file ended before expected size: path=%s offset=%zu size=%zu", path.c_str(), offset,
                    data.size());
            }
            data.clear();
            return false;
        }
        offset += static_cast<size_t>(count);
    }
    ASCENDLOGD("Read regular file: path=%s bytes=%zu", path.c_str(), data.size());
    return true;
}

bool FileUtils::OpenExclusiveOutput(const std::string& path, std::ofstream& output)
{
    ASCENDLOGD("Opening exclusive output: path=%s", path.c_str());
    if (output.is_open()) {
        ASCENDLOGW("Output stream is already open: path=%s", path.c_str());
        return false;
    }
    const int descriptor = open(path.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, 0600);
    if (descriptor < 0) {
        const int openError = errno;
        ASCENDLOGW("Failed to create exclusive output: path=%s error=%s", path.c_str(), std::strerror(openError));
        return false;
    }
    if (close(descriptor) != 0) {
        const int closeError = errno;
        ASCENDLOGW(
            "Failed to close exclusive output descriptor: path=%s error=%s", path.c_str(), std::strerror(closeError));
        if (unlink(path.c_str()) != 0 && errno != ENOENT) {
            const int cleanupError = errno;
            ASCENDLOGW(
                "Failed to remove incomplete output: path=%s error=%s", path.c_str(), std::strerror(cleanupError));
        }
        return false;
    }

    output.open(path.c_str(), std::ios::trunc);
    if (output.is_open()) {
        ASCENDLOGI("Opened exclusive output: path=%s", path.c_str());
        return true;
    }
    ASCENDLOGW("Failed to open exclusive output stream: path=%s", path.c_str());
    if (unlink(path.c_str()) != 0 && errno != ENOENT) {
        const int cleanupError = errno;
        ASCENDLOGW("Failed to remove incomplete output: path=%s error=%s", path.c_str(), std::strerror(cleanupError));
    }
    return false;
}

bool FileUtils::FinalizeOutput(std::ofstream& output)
{
    ASCENDLOGD("Finalizing output stream");
    output.flush();
    const bool writeSucceeded = static_cast<bool>(output);
    output.close();
    const bool success = writeSucceeded && static_cast<bool>(output);
    if (!success) {
        ASCENDLOGW("Failed to flush or close output stream");
        return false;
    }
    ASCENDLOGD("Finalized output stream");
    return true;
}

bool FileUtils::CheckNonSymlink(const std::string& path) noexcept
{
    ASCENDLOGD("Checking output path for symlink: path=%s", path.c_str());
    struct stat info {};
    if (lstat(path.c_str(), &info) != 0) {
        const int inspectError = errno;
        if (inspectError == ENOENT) {
            return true;
        }
        ASCENDLOGW("Failed to inspect output path: path=%s error=%s", path.c_str(), std::strerror(inspectError));
        return false;
    }
    if (S_ISLNK(info.st_mode)) {
        ASCENDLOGW("Rejected symlink output path: path=%s", path.c_str());
        return false;
    }
    return true;
}

bool FileUtils::CopyFileExclusive(const std::string& source, const std::string& destination) noexcept
{
    ASCENDLOGD("Copying file exclusively: source=%s destination=%s", source.c_str(), destination.c_str());
    const int input = open(source.c_str(), O_RDONLY | O_CLOEXEC | O_NOFOLLOW);
    if (input < 0) {
        const int openError = errno;
        ASCENDLOGW("Failed to open copy source: path=%s error=%s", source.c_str(), std::strerror(openError));
        return false;
    }
    struct stat info {};
    if (fstat(input, &info) != 0) {
        const int inspectError = errno;
        ASCENDLOGW("Failed to inspect copy source: path=%s error=%s", source.c_str(), std::strerror(inspectError));
        (void)close(input);
        return false;
    }
    if (!S_ISREG(info.st_mode)) {
        ASCENDLOGW("Rejected non-regular copy source: path=%s", source.c_str());
        (void)close(input);
        return false;
    }
    const int output = open(destination.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_CLOEXEC, info.st_mode & 0777);
    if (output < 0) {
        const int openError = errno;
        ASCENDLOGW(
            "Failed to create copy destination: path=%s error=%s", destination.c_str(), std::strerror(openError));
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
            const int readError = errno;
            ASCENDLOGW("Failed to read copy source: path=%s error=%s", source.c_str(), std::strerror(readError));
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
                const int writeError = errno;
                ASCENDLOGW(
                    "Failed to write copy destination: path=%s error=%s", destination.c_str(),
                    std::strerror(writeError));
                success = false;
                break;
            }
            offset += static_cast<size_t>(written);
        }
    }
    if (close(input) != 0) {
        const int closeError = errno;
        ASCENDLOGW("Failed to close copy source: path=%s error=%s", source.c_str(), std::strerror(closeError));
        success = false;
    }
    if (close(output) != 0) {
        const int closeError = errno;
        ASCENDLOGW(
            "Failed to close copy destination: path=%s error=%s", destination.c_str(), std::strerror(closeError));
        success = false;
    }
    if (!success) {
        if (unlink(destination.c_str()) != 0 && errno != ENOENT) {
            const int cleanupError = errno;
            ASCENDLOGW(
                "Failed to remove incomplete copy: path=%s error=%s", destination.c_str(), std::strerror(cleanupError));
        }
        return false;
    }
    ASCENDLOGI("Copied file exclusively: source=%s destination=%s", source.c_str(), destination.c_str());
    return true;
}

} // namespace ascendc
