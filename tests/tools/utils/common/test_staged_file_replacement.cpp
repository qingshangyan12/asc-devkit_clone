/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "staged_file_replacement.h"
#include "../asc_compile_exporter/test_support.h"

#include <boost/filesystem.hpp>
#include <sys/stat.h>

namespace ascendc {
namespace {
namespace fs = boost::filesystem;
using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ReadTestFile;
using asc_compile_exporter_test::WriteTestFile;

TEST_F(ModuleTest, StagedTextReplacesDestinationOnlyOnExplicitCommit)
{
    const std::string destination = root_ + "/output.txt";
    WriteTestFile(destination, "original");
    StagedFileReplacement replacement;
    const std::string content("a\0b", 3U);
    const mode_t previousMask = umask(0022);
    const bool staged = replacement.StageTextFile(destination, content);
    umask(previousMask);
    ASSERT_TRUE(staged);
    EXPECT_EQ(ReadTestFile(destination), "original");
    ASSERT_TRUE(replacement.ReplaceDestinationAtomically());
    EXPECT_EQ(ReadTestFile(destination), content);
    EXPECT_FALSE(replacement.ReplaceDestinationAtomically());
    struct stat status {};
    ASSERT_EQ(stat(destination.c_str(), &status), 0);
    EXPECT_EQ(status.st_mode & 0777U, 0600U);
}

TEST_F(ModuleTest, StagedReplacementRemovesTemporaryFileDuringExceptionUnwinding)
{
    const std::string destination = root_ + "/output.txt";
    WriteTestFile(destination, "original");
    try {
        StagedFileReplacement replacement;
        ASSERT_TRUE(replacement.StageTextFile(destination, "new"));
        throw std::runtime_error("simulated caller failure");
    } catch (const std::runtime_error&) {
    }
    EXPECT_EQ(ReadTestFile(destination), "original");
    EXPECT_EQ(std::distance(fs::directory_iterator(root_), fs::directory_iterator()), 1);
}

TEST_F(ModuleTest, StagedCopyCanBePublishedAfterSourceWorktreeIsRemoved)
{
    const std::string source = root_ + "/worktree/linked.elf";
    const std::string destination = root_ + "/output.elf";
    const std::string bytes(150000U, 'x');
    WriteTestFile(source, bytes);
    StagedFileReplacement replacement;
    const mode_t previousMask = umask(0027);
    const bool staged = replacement.StageFileCopy(source, destination);
    umask(previousMask);
    ASSERT_TRUE(staged);
    ASSERT_TRUE(FileUtils::RemoveAll(root_ + "/worktree"));
    ASSERT_TRUE(replacement.ReplaceDestinationAtomically());
    EXPECT_EQ(ReadTestFile(destination), bytes);
    struct stat status {};
    ASSERT_EQ(stat(destination.c_str(), &status), 0);
    EXPECT_EQ(status.st_mode & 0777U, 0640U);
}

TEST_F(ModuleTest, StagedReplacementCommitsToTheOriginalCanonicalParent)
{
    const std::string first = root_ + "/first";
    const std::string second = root_ + "/second";
    const std::string alias = root_ + "/alias";
    ASSERT_TRUE(FileUtils::CreateDirectories(first));
    ASSERT_TRUE(FileUtils::CreateDirectories(second));
    ASSERT_EQ(symlink(first.c_str(), alias.c_str()), 0);
    StagedFileReplacement replacement;
    ASSERT_TRUE(replacement.StageTextFile(alias + "/output", "new"));
    ASSERT_EQ(unlink(alias.c_str()), 0);
    ASSERT_EQ(symlink(second.c_str(), alias.c_str()), 0);
    ASSERT_TRUE(replacement.ReplaceDestinationAtomically());
    EXPECT_EQ(ReadTestFile(first + "/output"), "new");
    EXPECT_FALSE(FileUtils::PathExists(second + "/output"));
}

TEST_F(ModuleTest, StagedReplacementCleansUpWhenRenameFails)
{
    const std::string destination = root_ + "/output";
    {
        StagedFileReplacement replacement;
        ASSERT_TRUE(replacement.StageTextFile(destination, "new"));
        ASSERT_TRUE(FileUtils::CreateDirectories(destination));
        EXPECT_FALSE(replacement.ReplaceDestinationAtomically());
    }
    EXPECT_TRUE(FileUtils::IsDirectory(destination));
    EXPECT_EQ(std::distance(fs::directory_iterator(root_), fs::directory_iterator()), 1);
}

TEST_F(ModuleTest, StagedReplacementDoesNotRemoveUnownedFilesWhenTemporaryNamesCollide)
{
    const std::string destination = root_ + "/output";
    const std::string victim = root_ + "/victim";
    WriteTestFile(victim, "preserve");
    for (size_t sequence = 0U; sequence < 128U; ++sequence) {
        const std::string collision =
            destination + ".asc_tmp_" + std::to_string(getpid()) + "_" + std::to_string(sequence);
        ASSERT_EQ(symlink(victim.c_str(), collision.c_str()), 0);
    }
    {
        StagedFileReplacement replacement;
        EXPECT_FALSE(replacement.StageTextFile(destination, "unexpected"));
        EXPECT_FALSE(replacement.ReplaceDestinationAtomically());
    }
    EXPECT_EQ(ReadTestFile(victim), "preserve");
    EXPECT_EQ(std::distance(fs::directory_iterator(root_), fs::directory_iterator()), 129);
    EXPECT_FALSE(FileUtils::PathExists(destination));
}

TEST_F(ModuleTest, StagedReplacementRejectsMissingInputAndMultipleStaging)
{
    const std::string destination = root_ + "/output";
    StagedFileReplacement missing;
    EXPECT_FALSE(missing.StageFileCopy(root_ + "/missing", destination));
    EXPECT_FALSE(missing.ReplaceDestinationAtomically());
    EXPECT_FALSE(FileUtils::PathExists(destination));
    StagedFileReplacement replacement;
    ASSERT_TRUE(replacement.StageTextFile(destination, "first"));
    EXPECT_FALSE(replacement.StageTextFile(destination, "second"));
    ASSERT_TRUE(replacement.ReplaceDestinationAtomically());
    EXPECT_EQ(ReadTestFile(destination), "first");
}

} // namespace
} // namespace ascendc
