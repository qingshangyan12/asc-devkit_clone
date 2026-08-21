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
#include "test_support.h"

#include <string>
#include <sys/stat.h>
#include <utility>

namespace ascendc {
namespace {

using asc_compile_exporter_test::LstatPathExists;
using asc_compile_exporter_test::ModuleTest;

TEST_F(ModuleTest, TemporaryDirectoryCreatesUniqueDirectoryAndRemovesItOnDestruction)
{
    const std::string parentPath = FileUtils::JoinPath(root_, "temporary-directories");
    ASSERT_TRUE(FileUtils::CreateDirectories(parentPath));
    std::string removedDirectoryPath;
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.CreateUniqueUnder(parentPath, "test-%%%%%%%%"));
        removedDirectoryPath = directory.GetPath();
        EXPECT_TRUE(FileUtils::IsDirectory(removedDirectoryPath));
        EXPECT_TRUE(FileUtils::IsPathWithin(removedDirectoryPath, parentPath));
        struct stat directoryStatus {};
        ASSERT_EQ(stat(removedDirectoryPath.c_str(), &directoryStatus), 0);
        constexpr mode_t permissionBits = S_IRWXU | S_IRWXG | S_IRWXO;
        EXPECT_EQ(directoryStatus.st_mode & permissionBits, S_IRWXU);
        EXPECT_FALSE(directory.CreateUniqueUnder(parentPath, "second-%%%%%%%%"));
    }
    EXPECT_FALSE(LstatPathExists(removedDirectoryPath));
}

TEST_F(ModuleTest, TemporaryDirectoryCanRetainCreatedOrAdoptedDirectory)
{
    const std::string createdParentPath = FileUtils::JoinPath(root_, "retained-created");
    ASSERT_TRUE(FileUtils::CreateDirectories(createdParentPath));
    std::string retainedCreatedPath;
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.CreateUniqueUnder(createdParentPath, "created-%%%%%%%%"));
        retainedCreatedPath = directory.GetPath();
        directory.RetainAfterDestruction();
    }
    EXPECT_TRUE(FileUtils::IsDirectory(retainedCreatedPath));

    const std::string retainedAdoptedPath = FileUtils::JoinPath(root_, "retained-adopted");
    ASSERT_TRUE(FileUtils::CreateDirectories(retainedAdoptedPath));
    {
        TemporaryDirectory directory;
        ASSERT_TRUE(directory.AdoptExistingForCleanup(retainedAdoptedPath));
        directory.RetainAfterDestruction();
    }
    EXPECT_TRUE(FileUtils::IsDirectory(retainedAdoptedPath));
}

TEST_F(ModuleTest, TemporaryDirectoryMovesCleanupOwnershipExactlyOnce)
{
    const std::string parentPath = FileUtils::JoinPath(root_, "moved");
    ASSERT_TRUE(FileUtils::CreateDirectories(parentPath));
    std::string movedDirectoryPath;
    {
        TemporaryDirectory original;
        ASSERT_TRUE(original.CreateUniqueUnder(parentPath, "original-%%%%%%%%"));
        movedDirectoryPath = original.GetPath();

        TemporaryDirectory moved(std::move(original));
        EXPECT_TRUE(original.GetPath().empty());
        EXPECT_EQ(moved.GetPath(), movedDirectoryPath);

        TemporaryDirectory assigned;
        assigned = std::move(moved);
        EXPECT_TRUE(moved.GetPath().empty());
        EXPECT_EQ(assigned.GetPath(), movedDirectoryPath);
    }
    EXPECT_FALSE(LstatPathExists(movedDirectoryPath));
}

TEST_F(ModuleTest, TemporaryDirectoryRemovesAdoptedDirectoryOnRequest)
{
    const std::string adoptedPath = FileUtils::JoinPath(root_, "adopted");
    ASSERT_TRUE(FileUtils::CreateDirectories(adoptedPath));
    TemporaryDirectory directory;
    ASSERT_TRUE(directory.AdoptExistingForCleanup(adoptedPath));
    EXPECT_TRUE(directory.RemoveNow());
    EXPECT_TRUE(directory.GetPath().empty());
    EXPECT_FALSE(LstatPathExists(adoptedPath));
    EXPECT_TRUE(directory.RemoveNow());
}

TEST_F(ModuleTest, TemporaryDirectoryPreservesBothOwnersWhenMoveAssignmentCannotCleanDestination)
{
    const std::string sourceParentPath = FileUtils::JoinPath(root_, "move-source");
    ASSERT_TRUE(FileUtils::CreateDirectories(sourceParentPath));
    std::string sourceDirectoryPath;
    {
        TemporaryDirectory source;
        ASSERT_TRUE(source.CreateUniqueUnder(sourceParentPath, "source-%%%%%%%%"));
        sourceDirectoryPath = source.GetPath();

        TemporaryDirectory destination;
        ASSERT_TRUE(destination.AdoptExistingForCleanup("/proc/self/status"));
        destination = std::move(source);

        EXPECT_EQ(destination.GetPath(), "/proc/self/status");
        EXPECT_EQ(source.GetPath(), sourceDirectoryPath);
        destination.RetainAfterDestruction();
    }
    EXPECT_FALSE(LstatPathExists(sourceDirectoryPath));
}

TEST_F(ModuleTest, TemporaryDirectoryRejectsInvalidCreationInputsAndReportsRemovalFailure)
{
    TemporaryDirectory directory;
    EXPECT_FALSE(directory.CreateUniqueUnder(FileUtils::JoinPath(root_, "missing"), "test-%%%%%%%%"));
    EXPECT_FALSE(directory.CreateUniqueUnder(root_, "nested/test-%%%%%%%%"));
    EXPECT_FALSE(directory.CreateUniqueUnder(root_, "not-unique"));
    EXPECT_FALSE(directory.AdoptExistingForCleanup(std::string()));

    ASSERT_TRUE(directory.AdoptExistingForCleanup("/proc/self/status"));
    EXPECT_FALSE(directory.RemoveNow());
    EXPECT_EQ(directory.GetPath(), "/proc/self/status");
    directory.RetainAfterDestruction();
}

} // namespace
} // namespace ascendc
