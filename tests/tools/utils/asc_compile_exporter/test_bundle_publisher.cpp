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

#include <sys/stat.h>
#include <unistd.h>

#include "build_workspace.h"
#include "bundle_publisher.h"
#include "file_utils.h"
#include "test_support.h"

namespace ascendc {
namespace manifest_generator {
namespace {

using asc_compile_exporter_test::LstatPathExists;
using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ReadTestFile;
using asc_compile_exporter_test::WriteTestFile;

TEST_F(ModuleTest, BundlePublisherPublishesFilesAndRejectsSymlinks)
{
    const std::string output = FileUtils::JoinPath(root_, "output/bundle.so");
    ASSERT_TRUE(FileUtils::CreateDirectories(FileUtils::ParentPath(output)));
    BundlePublisher::Prepare(output);
    EXPECT_TRUE(FileUtils::IsDirectory(FileUtils::ParentPath(output)));

    const std::string staged = FileUtils::JoinPath(root_, "staged.so");
    WriteTestFile(staged, "new bundle");
    WriteTestFile(output, "old bundle");
    BundlePublisher::Publish(staged, output);
    EXPECT_EQ(ReadTestFile(output), "new bundle");
    EXPECT_FALSE(LstatPathExists(staged));

    const std::string victim = FileUtils::JoinPath(root_, "victim.so");
    const std::string symlinkPath = FileUtils::JoinPath(root_, "output/symlink.so");
    WriteTestFile(victim, "victim");
    ASSERT_EQ(symlink(victim.c_str(), symlinkPath.c_str()), 0);
    EXPECT_THROW(BundlePublisher::Prepare(symlinkPath), std::runtime_error);
}

TEST_F(ModuleTest, BundlePublisherReportsRenameFailure)
{
    const std::string output = FileUtils::JoinPath(root_, "output/missing.so");
    ASSERT_TRUE(FileUtils::CreateDirectories(FileUtils::ParentPath(output)));
    EXPECT_THROW(BundlePublisher::Publish(FileUtils::JoinPath(root_, "missing-staged.so"), output), std::runtime_error);
    EXPECT_FALSE(LstatPathExists(output));
}

TEST_F(ModuleTest, BundlePublisherCopiesAcrossFileSystems)
{
    struct stat rootInfo {};
    struct stat sharedMemoryInfo {};
    if (stat(root_.c_str(), &rootInfo) != 0 || stat("/dev/shm", &sharedMemoryInfo) != 0 ||
        rootInfo.st_dev == sharedMemoryInfo.st_dev || access("/dev/shm", W_OK) != 0) {
        GTEST_SKIP() << "cross-filesystem writable directory is unavailable";
    }

    BuildWorkspace destinationWorkspace = BuildWorkspace::Create("/dev/shm", false);
    const std::string staged = FileUtils::JoinPath(root_, "large-staged.so");
    const std::string output = FileUtils::JoinPath(destinationWorkspace.Session(), "bundle.so");
    const std::string content(128U * 1024U + 17U, 'x');
    WriteTestFile(staged, content);

    BundlePublisher::Publish(staged, output);

    EXPECT_EQ(ReadTestFile(output), content);
    EXPECT_TRUE(LstatPathExists(staged));

    const std::string stagedDirectory = FileUtils::JoinPath(root_, "staged-directory");
    ASSERT_TRUE(FileUtils::CreateDirectories(stagedDirectory));
    EXPECT_THROW(
        BundlePublisher::Publish(stagedDirectory, FileUtils::JoinPath(destinationWorkspace.Session(), "directory.so")),
        std::runtime_error);

    const std::string stagedSymlink = FileUtils::JoinPath(root_, "staged-symlink.so");
    ASSERT_EQ(symlink(staged.c_str(), stagedSymlink.c_str()), 0);
    EXPECT_THROW(
        BundlePublisher::Publish(stagedSymlink, FileUtils::JoinPath(destinationWorkspace.Session(), "symlink.so")),
        std::runtime_error);

    const std::string outputDirectory = FileUtils::JoinPath(destinationWorkspace.Session(), "existing-directory");
    ASSERT_TRUE(FileUtils::CreateDirectories(outputDirectory));
    EXPECT_THROW(BundlePublisher::Publish(staged, outputDirectory), std::runtime_error);
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
