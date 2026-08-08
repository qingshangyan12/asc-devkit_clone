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
#include <dirent.h>
#include <fstream>
#include <limits.h>
#include <limits>
#include <string>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>

#include "file_utils.h"
#include "test_support.h"

namespace {

bool g_failNextGetcwd = false;
bool g_failNextMkdir = false;
bool g_failNextReaddir = false;

} // namespace

extern "C" char* __real_getcwd(char* buffer, size_t size);
extern "C" int __real_mkdir(const char* path, mode_t mode);
extern "C" dirent* __real_readdir(DIR* directory);

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
    if (g_failNextMkdir) {
        g_failNextMkdir = false;
        errno = EACCES;
        return -1;
    }
    return __real_mkdir(path, mode);
}

extern "C" dirent* __wrap_readdir(DIR* directory)
{
    if (g_failNextReaddir) {
        g_failNextReaddir = false;
        errno = EIO;
        return nullptr;
    }
    return __real_readdir(directory);
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
    g_failNextMkdir = true;
    EXPECT_FALSE(FileUtils::CreateTempDirectory(temporary));
    EXPECT_TRUE(temporary.empty());
}

TEST_F(ModuleTest, FileUtilsReportsStreamAndSpecialFileFailures)
{
    const std::string procOutput = "/proc/asc_compile_exporter_forbidden_output.bin";
    EXPECT_FALSE(FileUtils::WriteBinaryFile(procOutput, {1U}));

    std::vector<uint8_t> data = {1U};
    EXPECT_FALSE(FileUtils::ReadRegularFile("/proc/self/cmdline", std::numeric_limits<uintmax_t>::max(), data));
    EXPECT_TRUE(data.empty());

    const std::string openedPath = FileUtils::JoinPath(root_, "opened.bin");
    const std::string exclusivePath = FileUtils::JoinPath(root_, "exclusive.bin");
    std::ofstream opened(openedPath.c_str());
    ASSERT_TRUE(opened.is_open());
    EXPECT_FALSE(FileUtils::OpenExclusiveOutput(exclusivePath, opened));
    opened.close();

    const std::string throwingPath = FileUtils::JoinPath(root_, "throwing.bin");
    std::ofstream throwingOutput(throwingPath.c_str());
    ASSERT_TRUE(throwingOutput.is_open());
    throwingOutput.close();
    throwingOutput.exceptions(std::ios::failbit | std::ios::badbit);
    EXPECT_FALSE(FileUtils::FinalizeOutput(throwingOutput));

    EXPECT_FALSE(FileUtils::CopyFileExclusive("/proc/self/mem", FileUtils::JoinPath(root_, "memory-copy")));
}

} // namespace
} // namespace ascendc
