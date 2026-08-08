/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <algorithm>
#include <cerrno>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

#define private public
#include "file_utils.h"
#undef private

#include <gtest/gtest.h>
#include <mockcpp/mockcpp.hpp>

#include <dirent.h>
#include <unistd.h>

namespace ascendc {
namespace resource {
namespace {

class FileUtilsTest : public testing::Test {
protected:
    void SetUp() override { ASSERT_TRUE(FileUtils::MakeTempRoot(root_)); }

    void TearDown() override { FileUtils::RemoveTree(root_); }

    std::string root_;
};

dirent* ReaddirErrorStub(DIR*)
{
    errno = EIO;
    return nullptr;
}

TEST_F(FileUtilsTest, PathOperationsCoverBoundaryCases)
{
    EXPECT_EQ(FileUtils::JoinPath("", "right"), "right");
    EXPECT_EQ(FileUtils::JoinPath("left", ""), "left");
    EXPECT_EQ(FileUtils::JoinPath("left/", "right"), "left/right");
    EXPECT_EQ(FileUtils::JoinPath("left", "right"), "left/right");

    EXPECT_EQ(FileUtils::ParentPath("name"), "");
    EXPECT_EQ(FileUtils::ParentPath("/name"), "/");
    EXPECT_EQ(FileUtils::ParentPath("a/b"), "a");
    EXPECT_EQ(FileUtils::FileName(""), "");
    EXPECT_EQ(FileUtils::FileName("a/"), "");
    EXPECT_EQ(FileUtils::FileName("name"), "name");
    EXPECT_EQ(FileUtils::FileName("a/name"), "name");

    EXPECT_EQ(FileUtils::PathComponents("///a//b/"), (std::vector<std::string>{"a", "b"}));
    EXPECT_TRUE(FileUtils::PathComponents("///").empty());
    EXPECT_TRUE(FileUtils::IsWithinPath("root/child/file", "root/child"));
    EXPECT_FALSE(FileUtils::IsWithinPath("root/other", "root/child"));
    EXPECT_FALSE(FileUtils::IsWithinPath("", "root"));
    EXPECT_FALSE(FileUtils::IsWithinPath("root", ""));
}

TEST_F(FileUtilsTest, FileSystemQueriesAndDirectoryCreation)
{
    const std::string nested = FileUtils::JoinPath(root_, "one/two");
    EXPECT_TRUE(FileUtils::EnsureDirectory(""));
    EXPECT_TRUE(FileUtils::EnsureDirectory("/"));
    EXPECT_TRUE(FileUtils::CreateDirectories(""));
    EXPECT_TRUE(FileUtils::CreateDirectories(nested));
    EXPECT_TRUE(FileUtils::EnsureDirectory(nested));
    EXPECT_TRUE(FileUtils::IsDirectory(nested));
    EXPECT_TRUE(FileUtils::Exists(nested));
    EXPECT_FALSE(FileUtils::IsRegularFile(nested));

    const std::string file = FileUtils::JoinPath(root_, "data/file.bin");
    ASSERT_TRUE(FileUtils::WriteFile(file, {1U, 2U, 3U}));
    EXPECT_TRUE(FileUtils::IsRegularFile(file));
    EXPECT_FALSE(FileUtils::IsDirectory(file));
    EXPECT_FALSE(FileUtils::IsSymlink(file));

    std::string canonical;
    EXPECT_TRUE(FileUtils::CanonicalPath(file, canonical));
    EXPECT_EQ(canonical, file);
    EXPECT_FALSE(FileUtils::CanonicalPath(FileUtils::JoinPath(root_, "missing"), canonical));

    const std::string link = FileUtils::JoinPath(root_, "data/link.bin");
    ASSERT_EQ(symlink(file.c_str(), link.c_str()), 0);
    EXPECT_TRUE(FileUtils::IsSymlink(link));
    EXPECT_TRUE(FileUtils::IsRegularFile(link));

    EXPECT_FALSE(FileUtils::EnsureDirectory(file));
    EXPECT_FALSE(FileUtils::CreateDirectories(FileUtils::JoinPath(file, "child")));
}

TEST_F(FileUtilsTest, WriteAndWalkDirectory)
{
    const std::string emptyFile = FileUtils::JoinPath(root_, "empty.bin");
    const std::string nestedFile = FileUtils::JoinPath(root_, "sub/deep/data.bin");
    ASSERT_TRUE(FileUtils::WriteFile(emptyFile, {}));
    ASSERT_TRUE(FileUtils::WriteFile(nestedFile, {'a', 'b', 'c'}));
    EXPECT_FALSE(FileUtils::WriteFile(FileUtils::JoinPath(emptyFile, "child"), {1U}));
    EXPECT_FALSE(FileUtils::WriteFile(root_, {1U}));

    std::ifstream input(nestedFile, std::ios::binary);
    std::string contents((std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>());
    EXPECT_EQ(contents, "abc");

    std::vector<std::string> paths;
    ASSERT_TRUE(FileUtils::WalkDirectory(root_, false, false, paths));
    EXPECT_NE(std::find(paths.begin(), paths.end(), emptyFile), paths.end());
    EXPECT_EQ(std::find(paths.begin(), paths.end(), nestedFile), paths.end());

    paths.clear();
    ASSERT_TRUE(FileUtils::WalkDirectory(root_, true, false, paths));
    EXPECT_NE(std::find(paths.begin(), paths.end(), nestedFile), paths.end());

    const std::string missing = FileUtils::JoinPath(root_, "missing");
    EXPECT_FALSE(FileUtils::WalkDirectory(missing, false, false, paths));
    EXPECT_TRUE(FileUtils::WalkDirectory(missing, false, true, paths));
}

TEST_F(FileUtilsTest, WalkDirectoryReportsReaddirErrors)
{
    MOCKER(readdir).expects(once()).will(invoke(ReaddirErrorStub));
    std::vector<std::string> paths;
    EXPECT_FALSE(FileUtils::WalkDirectory(root_, false, false, paths));
    GlobalMockObject::verify();
}

TEST_F(FileUtilsTest, RemoveTreeHandlesFilesLinksDirectoriesAndMissingPaths)
{
    const std::string tree = FileUtils::JoinPath(root_, "tree");
    const std::string file = FileUtils::JoinPath(tree, "nested/file.bin");
    const std::string link = FileUtils::JoinPath(tree, "link.bin");
    ASSERT_TRUE(FileUtils::WriteFile(file, {1U}));
    ASSERT_EQ(symlink(file.c_str(), link.c_str()), 0);

    FileUtils::RemoveTree(link);
    EXPECT_FALSE(FileUtils::Exists(link));
    EXPECT_TRUE(FileUtils::Exists(file));
    FileUtils::RemoveTree(tree);
    EXPECT_FALSE(FileUtils::Exists(tree));
    FileUtils::RemoveTree(tree);
}

} // namespace
} // namespace resource
} // namespace ascendc
