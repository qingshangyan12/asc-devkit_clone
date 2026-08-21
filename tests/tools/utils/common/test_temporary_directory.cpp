/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the LICENSE.
 */

#include <gtest/gtest.h>

#include "file_utils.h"
#include "temporary_directory.h"

#include <cstdlib>
#include <string>
#include <sys/stat.h>
#include <utility>
#include <vector>

namespace ascendc {
namespace {

bool PathExistsWithoutFollowingSymlinks(const std::string& path)
{
    struct stat pathStatus {};
    return lstat(path.c_str(), &pathStatus) == 0;
}

class TemporaryDirectoryTest : public testing::Test {
protected:
    void SetUp() override
    {
        const char* configuredTemporaryDirectory = std::getenv("TMPDIR");
        const std::string parentDirectoryPath =
            configuredTemporaryDirectory == nullptr || configuredTemporaryDirectory[0] == '\0' ?
                "/tmp" :
                configuredTemporaryDirectory;
        std::string rootDirectoryPattern = FileUtils::JoinPath(parentDirectoryPath, "temporary_directory_ut_XXXXXX");
        std::vector<char> writablePattern(rootDirectoryPattern.begin(), rootDirectoryPattern.end());
        writablePattern.push_back('\0');
        char* createdDirectoryPath = mkdtemp(writablePattern.data());
        ASSERT_NE(createdDirectoryPath, nullptr);
        rootDirectoryPath_ = createdDirectoryPath;
    }

    void TearDown() override { (void)FileUtils::RemoveAll(rootDirectoryPath_); }

    std::string rootDirectoryPath_;
};

TEST_F(TemporaryDirectoryTest, CreatesUniqueDirectoryAndRemovesItOnDestruction)
{
    const std::string parentPath = FileUtils::JoinPath(rootDirectoryPath_, "temporary-directories");
    ASSERT_TRUE(FileUtils::CreateDirectories(parentPath));
    std::string removedDirectoryPath;
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.CreateUniqueUnder(parentPath, "test"));
        removedDirectoryPath = directory.GetPath();
        EXPECT_TRUE(FileUtils::IsDirectory(removedDirectoryPath));
        EXPECT_TRUE(FileUtils::IsPathWithin(removedDirectoryPath, parentPath));
        struct stat directoryStatus {};
        ASSERT_EQ(stat(removedDirectoryPath.c_str(), &directoryStatus), 0);
        constexpr mode_t permissionBits = S_IRWXU | S_IRWXG | S_IRWXO;
        EXPECT_EQ(directoryStatus.st_mode & permissionBits, S_IRWXU);
        EXPECT_FALSE(directory.CreateUniqueUnder(parentPath, "second"));
    }
    EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(removedDirectoryPath));
}

TEST_F(TemporaryDirectoryTest, CanRetainCreatedOrAdoptedDirectory)
{
    const std::string createdParentPath = FileUtils::JoinPath(rootDirectoryPath_, "retained-created");
    ASSERT_TRUE(FileUtils::CreateDirectories(createdParentPath));
    std::string retainedCreatedPath;
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.CreateUniqueUnder(createdParentPath, "created"));
        retainedCreatedPath = directory.GetPath();
        directory.RetainAfterDestruction();
    }
    EXPECT_TRUE(FileUtils::IsDirectory(retainedCreatedPath));

    const std::string retainedAdoptedPath = FileUtils::JoinPath(rootDirectoryPath_, "retained-adopted");
    ASSERT_TRUE(FileUtils::CreateDirectories(retainedAdoptedPath));
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.AdoptExistingForCleanup(retainedAdoptedPath));
        directory.RetainAfterDestruction();
    }
    EXPECT_TRUE(FileUtils::IsDirectory(retainedAdoptedPath));
}

TEST_F(TemporaryDirectoryTest, MovesCleanupOwnershipExactlyOnce)
{
    const std::string parentPath = FileUtils::JoinPath(rootDirectoryPath_, "moved");
    ASSERT_TRUE(FileUtils::CreateDirectories(parentPath));
    std::string movedDirectoryPath;
    {
        TemporaryDirectory original;
        ASSERT_TRUE(original.CreateUniqueUnder(parentPath, "original"));
        movedDirectoryPath = original.GetPath();

        TemporaryDirectory moved(std::move(original));
        EXPECT_TRUE(original.GetPath().empty());
        EXPECT_EQ(moved.GetPath(), movedDirectoryPath);

        TemporaryDirectory assigned;
        assigned = std::move(moved);
        EXPECT_TRUE(moved.GetPath().empty());
        EXPECT_EQ(assigned.GetPath(), movedDirectoryPath);
    }
    EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(movedDirectoryPath));
}

TEST_F(TemporaryDirectoryTest, RemovesAdoptedDirectoryOnRequest)
{
    const std::string adoptedPath = FileUtils::JoinPath(rootDirectoryPath_, "adopted");
    ASSERT_TRUE(FileUtils::CreateDirectories(adoptedPath));
    TemporaryDirectory directory;
    ASSERT_TRUE(directory.AdoptExistingForCleanup(adoptedPath));
    EXPECT_TRUE(directory.RemoveNow());
    EXPECT_TRUE(directory.GetPath().empty());
    EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(adoptedPath));
    EXPECT_TRUE(directory.RemoveNow());
}

TEST_F(TemporaryDirectoryTest, MoveAssignmentRemovesPreviouslyOwnedDirectoryAndEmptiesSource)
{
    const std::string sourceParentPath = FileUtils::JoinPath(rootDirectoryPath_, "move-source");
    const std::string destinationParentPath = FileUtils::JoinPath(rootDirectoryPath_, "move-destination");
    ASSERT_TRUE(FileUtils::CreateDirectories(sourceParentPath));
    ASSERT_TRUE(FileUtils::CreateDirectories(destinationParentPath));
    std::string sourceDirectoryPath;
    std::string destinationDirectoryPath;
    {
        TemporaryDirectory source;
        ASSERT_TRUE(source.CreateUniqueUnder(sourceParentPath, "source"));
        sourceDirectoryPath = source.GetPath();

        TemporaryDirectory destination;
        ASSERT_TRUE(destination.CreateUniqueUnder(destinationParentPath, "destination"));
        destinationDirectoryPath = destination.GetPath();
        destination = std::move(source);

        EXPECT_EQ(destination.GetPath(), sourceDirectoryPath);
        EXPECT_TRUE(source.GetPath().empty());
        EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(destinationDirectoryPath));
    }
    EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(sourceDirectoryPath));
    EXPECT_FALSE(PathExistsWithoutFollowingSymlinks(destinationDirectoryPath));
}

TEST_F(TemporaryDirectoryTest, RejectsInvalidCreationInputsAndReportsRemovalFailure)
{
    TemporaryDirectory directory;
    EXPECT_FALSE(directory.CreateUniqueUnder(FileUtils::JoinPath(rootDirectoryPath_, "missing"), "test"));
    EXPECT_FALSE(directory.CreateUniqueUnder(rootDirectoryPath_, "nested/test"));
    EXPECT_FALSE(directory.CreateUniqueUnder(rootDirectoryPath_, "invalid%prefix"));
    EXPECT_FALSE(directory.AdoptExistingForCleanup(std::string()));

    ASSERT_TRUE(directory.AdoptExistingForCleanup("/proc/self/status"));
    EXPECT_FALSE(directory.RemoveNow());
    EXPECT_EQ(directory.GetPath(), "/proc/self/status");
    directory.RetainAfterDestruction();
}

} // namespace
} // namespace ascendc
