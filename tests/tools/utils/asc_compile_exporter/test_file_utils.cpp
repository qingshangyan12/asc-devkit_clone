/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include <gtest/gtest.h>

#include <cerrno>
#include <cstdarg>
#include <dirent.h>
#include <fcntl.h>
#include <fstream>
#include <limits.h>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "file_utils.h"
#include "test_support.h"

namespace {

bool g_failNextGetcwd = false;
int g_mkdirFailureCount = 0;
int g_mkdirFailureError = EACCES;
bool g_failNextReaddir = false;
std::string g_readdirFailurePath;
DIR* g_readdirFailureDirectory = nullptr;
std::string g_closedirFailurePath;
DIR* g_closedirFailureDirectory = nullptr;
std::string g_opendirFailurePath;
std::string g_lstatFailurePath;
std::string g_statFailurePath;
std::string g_unlinkFailurePath;

enum class DescriptorOperation {
    kNone,
    kFstat,
    kRead,
    kWrite,
    kClose,
};

std::string g_descriptorPath;
int g_descriptor = -1;
DescriptorOperation g_descriptorOperation = DescriptorOperation::kNone;
size_t g_descriptorCallsBeforeFailure = 0U;
int g_descriptorFailureError = EIO;
ssize_t g_descriptorFailureResult = -1;

bool MatchesPath(const std::string& expected, const char* actual)
{
    return !expected.empty() && actual != nullptr && expected == actual;
}

bool ShouldFailDescriptor(int descriptor, DescriptorOperation operation)
{
    if (descriptor != g_descriptor || operation != g_descriptorOperation) {
        return false;
    }
    if (g_descriptorCallsBeforeFailure != 0U) {
        --g_descriptorCallsBeforeFailure;
        return false;
    }
    g_descriptorOperation = DescriptorOperation::kNone;
    return true;
}

} // namespace

extern "C" int __real_close(int descriptor);
extern "C" int __real_closedir(DIR* directory);
extern "C" int __real_fstat(int descriptor, struct stat* info);
extern "C" char* __real_getcwd(char* buffer, size_t size);
extern "C" int __real_lstat(const char* path, struct stat* info);
extern "C" int __real_mkdir(const char* path, mode_t mode);
extern "C" int __real_open(const char* path, int flags, ...);
extern "C" DIR* __real_opendir(const char* path);
extern "C" ssize_t __real_read(int descriptor, void* buffer, size_t count);
extern "C" dirent* __real_readdir(DIR* directory);
extern "C" int __real_stat(const char* path, struct stat* info);
extern "C" int __real_unlink(const char* path);
extern "C" ssize_t __real_write(int descriptor, const void* buffer, size_t count);

extern "C" int __wrap_close(int descriptor)
{
    if (ShouldFailDescriptor(descriptor, DescriptorOperation::kClose)) {
        (void)__real_close(descriptor);
        errno = g_descriptorFailureError;
        return -1;
    }
    return __real_close(descriptor);
}

extern "C" int __wrap_closedir(DIR* directory)
{
    if (directory == g_closedirFailureDirectory) {
        g_closedirFailureDirectory = nullptr;
        (void)__real_closedir(directory);
        errno = EIO;
        return -1;
    }
    return __real_closedir(directory);
}

extern "C" int __wrap_fstat(int descriptor, struct stat* info)
{
    if (ShouldFailDescriptor(descriptor, DescriptorOperation::kFstat)) {
        errno = g_descriptorFailureError;
        return -1;
    }
    return __real_fstat(descriptor, info);
}

extern "C" char* __wrap_getcwd(char* buffer, size_t size)
{
    if (g_failNextGetcwd) {
        g_failNextGetcwd = false;
        errno = EACCES;
        return nullptr;
    }
    return __real_getcwd(buffer, size);
}

extern "C" int __wrap_mkdir(const char* path, mode_t mode)
{
    if (g_mkdirFailureCount > 0) {
        --g_mkdirFailureCount;
        errno = g_mkdirFailureError;
        return -1;
    }
    return __real_mkdir(path, mode);
}

extern "C" int __wrap_lstat(const char* path, struct stat* info)
{
    if (MatchesPath(g_lstatFailurePath, path)) {
        g_lstatFailurePath.clear();
        errno = EACCES;
        return -1;
    }
    return __real_lstat(path, info);
}

extern "C" int __wrap_open(const char* path, int flags, ...)
{
    mode_t mode = 0;
    if ((flags & O_CREAT) != 0) {
        va_list arguments;
        va_start(arguments, flags);
        mode = static_cast<mode_t>(va_arg(arguments, int));
        va_end(arguments);
    }
    const int descriptor = (flags & O_CREAT) != 0 ? __real_open(path, flags, mode) : __real_open(path, flags);
    if (descriptor >= 0 && MatchesPath(g_descriptorPath, path)) {
        g_descriptor = descriptor;
        g_descriptorPath.clear();
    }
    return descriptor;
}

extern "C" DIR* __wrap_opendir(const char* path)
{
    if (MatchesPath(g_opendirFailurePath, path)) {
        g_opendirFailurePath.clear();
        errno = EACCES;
        return nullptr;
    }
    DIR* directory = __real_opendir(path);
    if (MatchesPath(g_readdirFailurePath, path)) {
        g_readdirFailurePath.clear();
        g_readdirFailureDirectory = directory;
    }
    if (MatchesPath(g_closedirFailurePath, path)) {
        g_closedirFailurePath.clear();
        g_closedirFailureDirectory = directory;
    }
    return directory;
}

extern "C" ssize_t __wrap_read(int descriptor, void* buffer, size_t count)
{
    if (ShouldFailDescriptor(descriptor, DescriptorOperation::kRead)) {
        errno = g_descriptorFailureError;
        return g_descriptorFailureResult;
    }
    return __real_read(descriptor, buffer, count);
}

extern "C" dirent* __wrap_readdir(DIR* directory)
{
    if (g_failNextReaddir || directory == g_readdirFailureDirectory) {
        g_failNextReaddir = false;
        g_readdirFailureDirectory = nullptr;
        errno = EIO;
        return nullptr;
    }
    return __real_readdir(directory);
}

extern "C" int __wrap_stat(const char* path, struct stat* info)
{
    if (MatchesPath(g_statFailurePath, path)) {
        g_statFailurePath.clear();
        errno = EACCES;
        return -1;
    }
    return __real_stat(path, info);
}

extern "C" int __wrap_unlink(const char* path)
{
    if (MatchesPath(g_unlinkFailurePath, path)) {
        g_unlinkFailurePath.clear();
        errno = EACCES;
        return -1;
    }
    return __real_unlink(path);
}

extern "C" ssize_t __wrap_write(int descriptor, const void* buffer, size_t count)
{
    if (ShouldFailDescriptor(descriptor, DescriptorOperation::kWrite)) {
        errno = g_descriptorFailureError;
        return g_descriptorFailureResult;
    }
    return __real_write(descriptor, buffer, count);
}

namespace ascendc {
namespace {

using asc_compile_exporter_test::LstatPathExists;
using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ReadTestFile;
using asc_compile_exporter_test::WriteTestFile;

TEST_F(ModuleTest, FileUtilsHandlesPathOperations)
{
    EXPECT_EQ(FileUtils::JoinPath("", "child"), "child");
    EXPECT_EQ(FileUtils::JoinPath("parent", ""), "parent");
    EXPECT_EQ(FileUtils::JoinPath("parent/", "child"), "parent/child");
    EXPECT_EQ(FileUtils::ParentPath("file"), "");
    EXPECT_EQ(FileUtils::ParentPath("/file"), "/");
    EXPECT_EQ(FileUtils::ParentPath("directory/file"), "directory");
    EXPECT_EQ(FileUtils::FileName(""), "");
    EXPECT_EQ(FileUtils::FileName("/directory/"), "");
    EXPECT_EQ(FileUtils::FileName("directory/file"), "file");

    std::string current;
    ASSERT_TRUE(FileUtils::MakeAbsolutePath("", current));
    EXPECT_FALSE(current.empty());
    std::string absolute;
    EXPECT_TRUE(FileUtils::MakeAbsolutePath("/absolute", absolute));
    EXPECT_EQ(absolute, "/absolute");
    EXPECT_TRUE(FileUtils::MakeAbsolutePath("relative", absolute));
    EXPECT_EQ(absolute, FileUtils::JoinPath(current, "relative"));

    EXPECT_EQ(FileUtils::SplitPath("//one///two/"), (std::vector<std::string>{"one", "two"}));
    EXPECT_EQ(FileUtils::SplitPath("single"), (std::vector<std::string>{"single"}));
    EXPECT_TRUE(FileUtils::SplitPath("/").empty());
    EXPECT_TRUE(FileUtils::IsPathWithin("/root/child", "/root"));
    EXPECT_TRUE(FileUtils::IsPathWithin("/root", "/root"));
    EXPECT_TRUE(FileUtils::IsPathWithin("/child", "/"));
    EXPECT_FALSE(FileUtils::IsPathWithin("/rooted", "/root"));
    EXPECT_FALSE(FileUtils::IsPathWithin("relative", "/"));
    EXPECT_FALSE(FileUtils::IsPathWithin("", "/"));
}

TEST_F(ModuleTest, FileUtilsInspectsAndValidatesPaths)
{
    const std::string directory = FileUtils::JoinPath(root_, "directory/nested");
    const std::string regular = FileUtils::JoinPath(directory, "regular.bin");
    const std::string link = FileUtils::JoinPath(root_, "regular-link");
    ASSERT_TRUE(FileUtils::CreateDirectories(directory));
    WriteTestFile(regular, "data");
    ASSERT_EQ(symlink(regular.c_str(), link.c_str()), 0);

    EXPECT_TRUE(FileUtils::PathExists(regular));
    EXPECT_TRUE(FileUtils::IsDirectory(directory));
    EXPECT_TRUE(FileUtils::IsRegularFile(regular));
    EXPECT_TRUE(FileUtils::IsRegularFile(link));
    EXPECT_TRUE(FileUtils::IsSymlink(link));
    EXPECT_FALSE(FileUtils::IsSymlink(regular));
    EXPECT_FALSE(FileUtils::PathExists(FileUtils::JoinPath(root_, "missing")));

    std::string resolved;
    EXPECT_TRUE(FileUtils::ResolveCanonicalPath(directory, resolved));
    std::string validated;
    EXPECT_TRUE(FileUtils::ResolveDirectory(directory, validated));
    EXPECT_EQ(validated, resolved);
    EXPECT_TRUE(FileUtils::ResolveSubdirectory(directory, root_, validated));
    EXPECT_EQ(validated, resolved);
    EXPECT_FALSE(FileUtils::ResolveCanonicalPath(FileUtils::JoinPath(root_, "missing"), resolved));
    EXPECT_TRUE(resolved.empty());
    EXPECT_FALSE(FileUtils::ResolveDirectory(regular, validated));
    EXPECT_FALSE(FileUtils::ResolveDirectory(link, validated));
    EXPECT_FALSE(FileUtils::ResolveDirectory(FileUtils::JoinPath(root_, "missing"), validated));
    EXPECT_FALSE(FileUtils::ResolveSubdirectory("/tmp", root_, validated));
}

TEST_F(ModuleTest, FileUtilsCreatesListsAndRemovesDirectoryTrees)
{
    EXPECT_TRUE(FileUtils::CreateDirectories(""));
    EXPECT_TRUE(FileUtils::CreateDirectories("/"));
    const std::string tree = FileUtils::JoinPath(root_, "tree");
    const std::string nested = FileUtils::JoinPath(tree, "nested");
    ASSERT_TRUE(FileUtils::CreateDirectories(nested));
    EXPECT_TRUE(FileUtils::CreateDirectories(nested));
    WriteTestFile(FileUtils::JoinPath(tree, "top.txt"), "top");
    WriteTestFile(FileUtils::JoinPath(nested, "leaf.txt"), "leaf");

    std::vector<std::string> shallow;
    EXPECT_TRUE(FileUtils::ListDirectory(tree, false, false, shallow));
    EXPECT_EQ(shallow.size(), 2U);
    std::vector<std::string> recursive;
    EXPECT_TRUE(FileUtils::ListDirectory(tree, true, false, recursive));
    EXPECT_EQ(recursive.size(), 3U);
    std::vector<std::string> ignored;
    EXPECT_TRUE(FileUtils::ListDirectory(FileUtils::JoinPath(root_, "missing"), true, true, ignored));
    EXPECT_FALSE(FileUtils::ListDirectory(FileUtils::JoinPath(root_, "missing"), true, false, ignored));

    const std::string blockingFile = FileUtils::JoinPath(root_, "blocking-file");
    WriteTestFile(blockingFile, "block");
    EXPECT_FALSE(FileUtils::CreateDirectories(FileUtils::JoinPath(blockingFile, "child")));
    EXPECT_FALSE(FileUtils::CreateDirectories(std::string(PATH_MAX, 'x')));

    std::string temporary;
    ASSERT_TRUE(FileUtils::CreateTempDirectory(temporary));
    EXPECT_TRUE(FileUtils::IsDirectory(temporary));
    EXPECT_TRUE(FileUtils::RemoveAll(temporary));
    EXPECT_TRUE(FileUtils::RemoveAll(FileUtils::JoinPath(root_, "missing")));
    EXPECT_TRUE(FileUtils::RemoveAll(tree));
    EXPECT_FALSE(LstatPathExists(tree));
    EXPECT_FALSE(FileUtils::RemoveAll("/dev/null/child"));
}

TEST_F(ModuleTest, FileUtilsReadsWritesAndFinalizesFiles)
{
    const std::string binary = FileUtils::JoinPath(root_, "binary/data.bin");
    const std::vector<uint8_t> bytes = {0U, 127U, 255U};
    EXPECT_TRUE(FileUtils::WriteBinaryFile(binary, bytes));
    std::vector<uint8_t> data;
    EXPECT_TRUE(FileUtils::ReadRegularFile(binary, bytes.size(), data));
    EXPECT_EQ(data, bytes);

    const std::string empty = FileUtils::JoinPath(root_, "binary/empty.bin");
    EXPECT_TRUE(FileUtils::WriteBinaryFile(empty, {}));
    EXPECT_TRUE(FileUtils::ReadRegularFile(empty, 0U, data));
    EXPECT_TRUE(data.empty());
    EXPECT_FALSE(FileUtils::ReadRegularFile(binary, bytes.size() - 1U, data));
    EXPECT_TRUE(data.empty());
    EXPECT_FALSE(FileUtils::ReadRegularFile(root_, 100U, data));
    EXPECT_FALSE(FileUtils::ReadRegularFile(FileUtils::JoinPath(root_, "missing"), 100U, data));

    const std::string link = FileUtils::JoinPath(root_, "binary-link");
    ASSERT_EQ(symlink(binary.c_str(), link.c_str()), 0);
    EXPECT_FALSE(FileUtils::ReadRegularFile(link, 100U, data));

    const std::string generated = FileUtils::JoinPath(root_, "generated.cpp");
    std::ofstream output;
    ASSERT_TRUE(FileUtils::OpenExclusiveOutput(generated, output));
    output << "generated";
    EXPECT_TRUE(FileUtils::FinalizeOutput(output));
    EXPECT_EQ(ReadTestFile(generated), "generated");
    EXPECT_FALSE(FileUtils::OpenExclusiveOutput(generated, output));

    std::ofstream failedOutput;
    failedOutput.setstate(std::ios::badbit);
    EXPECT_FALSE(FileUtils::FinalizeOutput(failedOutput));

    const std::string parentFile = FileUtils::JoinPath(root_, "parent-file");
    WriteTestFile(parentFile, "parent");
    EXPECT_FALSE(FileUtils::WriteBinaryFile(FileUtils::JoinPath(parentFile, "child"), bytes));
}

TEST_F(ModuleTest, FileUtilsProtectsSymlinksAndCopiesExclusively)
{
    const std::string source = FileUtils::JoinPath(root_, "source.bin");
    const std::string destination = FileUtils::JoinPath(root_, "destination.bin");
    WriteTestFile(source, std::string(128U * 1024U + 5U, 'x'));

    EXPECT_TRUE(FileUtils::CheckNonSymlink(FileUtils::JoinPath(root_, "missing")));
    EXPECT_TRUE(FileUtils::CheckNonSymlink(source));
    const std::string sourceLink = FileUtils::JoinPath(root_, "source-link");
    ASSERT_EQ(symlink(source.c_str(), sourceLink.c_str()), 0);
    EXPECT_FALSE(FileUtils::CheckNonSymlink(sourceLink));

    EXPECT_TRUE(FileUtils::CopyFileExclusive(source, destination));
    EXPECT_EQ(ReadTestFile(destination), ReadTestFile(source));
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, destination));
    EXPECT_FALSE(
        FileUtils::CopyFileExclusive(FileUtils::JoinPath(root_, "missing"), FileUtils::JoinPath(root_, "copy")));
    EXPECT_FALSE(FileUtils::CopyFileExclusive(root_, FileUtils::JoinPath(root_, "directory-copy")));
    EXPECT_FALSE(FileUtils::CopyFileExclusive(sourceLink, FileUtils::JoinPath(root_, "link-copy")));
}

TEST_F(ModuleTest, FileUtilsReportsInjectedSystemCallFailures)
{
    std::string absolute = "stale";
    g_failNextGetcwd = true;
    EXPECT_FALSE(FileUtils::MakeAbsolutePath("relative", absolute));
    EXPECT_TRUE(absolute.empty());

    std::vector<std::string> paths;
    g_failNextReaddir = true;
    EXPECT_FALSE(FileUtils::ListDirectory(root_, false, false, paths));

    std::string temporary = "stale";
    g_mkdirFailureCount = 1;
    g_mkdirFailureError = EACCES;
    EXPECT_FALSE(FileUtils::CreateTempDirectory(temporary));
    EXPECT_TRUE(temporary.empty());
}

TEST_F(ModuleTest, FileUtilsReportsDirectoryOperationFailures)
{
    const std::string tree = FileUtils::JoinPath(root_, "failure-tree");
    const std::string nested = FileUtils::JoinPath(tree, "nested");
    ASSERT_TRUE(FileUtils::CreateDirectories(nested));
    WriteTestFile(FileUtils::JoinPath(nested, "leaf.txt"), "leaf");

    std::vector<std::string> paths;
    g_readdirFailurePath = nested;
    EXPECT_FALSE(FileUtils::ListDirectory(tree, true, false, paths));

    g_closedirFailurePath = tree;
    EXPECT_FALSE(FileUtils::ListDirectory(tree, false, false, paths));

    const std::string deniedDirectory = FileUtils::JoinPath(root_, "denied-directory");
    ASSERT_TRUE(FileUtils::CreateDirectories(deniedDirectory));
    g_opendirFailurePath = deniedDirectory;
    EXPECT_FALSE(FileUtils::RemoveAll(deniedDirectory));

    const std::string unreadableDirectory = FileUtils::JoinPath(root_, "unreadable-directory");
    WriteTestFile(FileUtils::JoinPath(unreadableDirectory, "entry"), "entry");
    g_readdirFailurePath = unreadableDirectory;
    EXPECT_FALSE(FileUtils::RemoveAll(unreadableDirectory));

    const std::string deniedFile = FileUtils::JoinPath(root_, "denied-file");
    WriteTestFile(deniedFile, "data");
    g_unlinkFailurePath = deniedFile;
    EXPECT_FALSE(FileUtils::RemoveAll(deniedFile));

    g_lstatFailurePath = deniedFile;
    EXPECT_FALSE(FileUtils::RemoveAll(deniedFile));
    g_lstatFailurePath = deniedFile;
    EXPECT_FALSE(FileUtils::CheckNonSymlink(deniedFile));
}

TEST_F(ModuleTest, FileUtilsReportsDirectoryCreationFailures)
{
    g_mkdirFailureCount = 1;
    g_mkdirFailureError = EACCES;
    EXPECT_FALSE(FileUtils::CreateDirectories(FileUtils::JoinPath(root_, "denied/child")));

    const std::string existing = FileUtils::JoinPath(root_, "existing-directory");
    ASSERT_TRUE(FileUtils::CreateDirectories(existing));
    g_statFailurePath = existing;
    EXPECT_FALSE(FileUtils::CreateDirectories(existing));

    std::string temporary;
    g_mkdirFailureCount = 1000;
    g_mkdirFailureError = EEXIST;
    EXPECT_FALSE(FileUtils::CreateTempDirectory(temporary));
    EXPECT_TRUE(temporary.empty());
}

TEST_F(ModuleTest, FileUtilsReportsDescriptorReadFailures)
{
    const std::string source = FileUtils::JoinPath(root_, "descriptor-source.bin");
    WriteTestFile(source, "data");
    std::vector<uint8_t> data;

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kFstat;
    EXPECT_FALSE(FileUtils::ReadRegularFile(source, 16U, data));

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kRead;
    g_descriptorFailureError = EIO;
    g_descriptorFailureResult = -1;
    EXPECT_FALSE(FileUtils::ReadRegularFile(source, 16U, data));

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kRead;
    g_descriptorFailureResult = 0;
    EXPECT_FALSE(FileUtils::ReadRegularFile(source, 16U, data));

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kRead;
    g_descriptorFailureError = EINTR;
    g_descriptorFailureResult = -1;
    EXPECT_TRUE(FileUtils::ReadRegularFile(source, 16U, data));
}

TEST_F(ModuleTest, FileUtilsReportsDescriptorCopyFailures)
{
    const std::string source = FileUtils::JoinPath(root_, "copy-source.bin");
    WriteTestFile(source, "copy data");

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kFstat;
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, FileUtils::JoinPath(root_, "fstat-copy.bin")));

    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kRead;
    g_descriptorFailureError = EIO;
    g_descriptorFailureResult = -1;
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, FileUtils::JoinPath(root_, "read-copy.bin")));

    const std::string writeDestination = FileUtils::JoinPath(root_, "write-copy.bin");
    g_descriptorPath = writeDestination;
    g_descriptorOperation = DescriptorOperation::kWrite;
    g_unlinkFailurePath = writeDestination;
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, writeDestination));

    const std::string interruptedReadDestination = FileUtils::JoinPath(root_, "interrupted-read-copy.bin");
    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kRead;
    g_descriptorFailureError = EINTR;
    EXPECT_TRUE(FileUtils::CopyFileExclusive(source, interruptedReadDestination));

    const std::string interruptedWriteDestination = FileUtils::JoinPath(root_, "interrupted-write-copy.bin");
    g_descriptorPath = interruptedWriteDestination;
    g_descriptorOperation = DescriptorOperation::kWrite;
    g_descriptorFailureError = EINTR;
    EXPECT_TRUE(FileUtils::CopyFileExclusive(source, interruptedWriteDestination));

    const std::string inputCloseDestination = FileUtils::JoinPath(root_, "input-close-copy.bin");
    g_descriptorPath = source;
    g_descriptorOperation = DescriptorOperation::kClose;
    g_descriptorFailureError = EIO;
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, inputCloseDestination));

    const std::string outputCloseDestination = FileUtils::JoinPath(root_, "output-close-copy.bin");
    g_descriptorPath = outputCloseDestination;
    g_descriptorOperation = DescriptorOperation::kClose;
    EXPECT_FALSE(FileUtils::CopyFileExclusive(source, outputCloseDestination));
}

TEST_F(ModuleTest, FileUtilsReportsExclusiveOutputDescriptorCloseFailure)
{
    const std::string outputPath = FileUtils::JoinPath(root_, "close-failure-output.bin");
    g_descriptorPath = outputPath;
    g_descriptorOperation = DescriptorOperation::kClose;
    g_descriptorFailureError = EIO;
    g_unlinkFailurePath = outputPath;
    std::ofstream output;
    EXPECT_FALSE(FileUtils::OpenExclusiveOutput(outputPath, output));
}

TEST_F(ModuleTest, FileUtilsReportsStreamAndSpecialFileFailures)
{
    const std::string procOutput = "/proc/asc_compile_exporter_forbidden_output.bin";
    EXPECT_FALSE(FileUtils::WriteBinaryFile(procOutput, {1U}));
    EXPECT_FALSE(FileUtils::WriteBinaryFile("/dev/full", {1U}));

    const std::string openedPath = FileUtils::JoinPath(root_, "opened.bin");
    const std::string exclusivePath = FileUtils::JoinPath(root_, "exclusive.bin");
    std::ofstream opened(openedPath.c_str());
    ASSERT_TRUE(opened.is_open());
    EXPECT_FALSE(FileUtils::OpenExclusiveOutput(exclusivePath, opened));
    opened.close();

    EXPECT_FALSE(FileUtils::CopyFileExclusive("/proc/self/mem", FileUtils::JoinPath(root_, "memory-copy")));
}

} // namespace
} // namespace ascendc
