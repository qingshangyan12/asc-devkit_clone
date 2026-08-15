/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "specialization/kernel_compilation_workspace.h"
#include "stub/kernel_specialization_log.h"

#include <gtest/gtest.h>

#include <boost/filesystem.hpp>
#include <boost/system/error_code.hpp>
#include <fstream>
#include <string>
#include <unistd.h>

namespace {
namespace fs = boost::filesystem;
using ascendc::aclrtc::KernelCompilationPlan;
using ascendc::aclrtc::KernelCompilationWorkspace;
using ascendc::aclrtc::WorktreeRetentionPolicy;

fs::path CreateWorkspacePath(const std::string& name)
{
    static uint64_t sequence = 0;
    const fs::path path = fs::temp_directory_path() /
                          (name + "_" + std::to_string(getpid()) + "_" + std::to_string(sequence++));
    fs::create_directories(path);
    return path;
}

void WriteFile(const fs::path& path, const std::string& text)
{
    fs::create_directories(path.parent_path());
    std::ofstream stream(path);
    stream << text;
}

std::string ReadFile(const fs::path& path)
{
    std::ifstream stream(path);
    return std::string(std::istreambuf_iterator<char>(stream), std::istreambuf_iterator<char>());
}

TEST(KernelCompilationWorkspaceTest, CreatesOutputDirectoriesAndAppliesAllPatchesPerFile)
{
    const fs::path worktreePath = CreateWorkspacePath("aclrtc_workspace_patch");
    const fs::path sourcePath = worktreePath / "resources/constants.h";
    WriteFile(sourcePath, "A=@@A@@; B=@@B@@;\n");
    KernelCompilationPlan plan;
    plan.requiredOutputDirectoryPaths = {worktreePath / "outputs/nested"};
    plan.sourcePatches = {
        {sourcePath, "@@A@@", "{0x01}"},
        {sourcePath, "@@B@@", "{0x02}"},
    };

    KernelCompilationWorkspace workspace(worktreePath, WorktreeRetentionPolicy::RetainForDiagnostics);
    EXPECT_EQ(workspace.MaterializeCompilationInputs(plan), 0);
    EXPECT_TRUE(fs::is_directory(worktreePath / "outputs/nested"));
    EXPECT_EQ(ReadFile(sourcePath), "A={0x01}; B={0x02};\n");

    boost::system::error_code ignoredError;
    fs::remove_all(worktreePath, ignoredError);
}

TEST(KernelCompilationWorkspaceTest, ReportsMissingPatchTemplateAsResourceFailure)
{
    const fs::path worktreePath = CreateWorkspacePath("aclrtc_workspace_missing_template");
    const fs::path sourcePath = worktreePath / "resources/constants.h";
    WriteFile(sourcePath, "unchanged\n");
    KernelCompilationPlan plan;
    plan.sourcePatches = {{sourcePath, "@@MISSING@@", "{0x01}"}};

    KernelCompilationWorkspace workspace(worktreePath, WorktreeRetentionPolicy::RetainForDiagnostics);
    ascendc::aclrtc::ClearCapturedKernelSpecializationLogs();
    EXPECT_EQ(workspace.MaterializeCompilationInputs(plan), ascendc::aclrtc::ACLRTC_ERROR_FAILURE);
    EXPECT_TRUE(ascendc::aclrtc::CapturedKernelSpecializationLogsContain(
        "patch template was not found"));
    EXPECT_EQ(ReadFile(sourcePath), "unchanged\n");

    boost::system::error_code ignoredError;
    fs::remove_all(worktreePath, ignoredError);
}

TEST(KernelCompilationWorkspaceTest, DeletesWorktreeByDefaultAndRetainsItWhenRequested)
{
    const fs::path removedWorktreePath = CreateWorkspacePath("aclrtc_workspace_remove");
    {
        KernelCompilationWorkspace workspace(removedWorktreePath, WorktreeRetentionPolicy::RemoveAfterCompilation);
    }
    EXPECT_FALSE(fs::exists(removedWorktreePath));

    const fs::path retainedWorktreePath = CreateWorkspacePath("aclrtc_workspace_retain");
    {
        KernelCompilationWorkspace workspace(retainedWorktreePath, WorktreeRetentionPolicy::RetainForDiagnostics);
    }
    EXPECT_TRUE(fs::is_directory(retainedWorktreePath));
    boost::system::error_code ignoredError;
    fs::remove_all(retainedWorktreePath, ignoredError);
}

TEST(KernelCompilationWorkspaceTest, FinalizesCleanupBeforeOutputPublication)
{
    const fs::path worktreePath = CreateWorkspacePath("aclrtc_workspace_finalize");
    KernelCompilationWorkspace workspace(worktreePath, WorktreeRetentionPolicy::RemoveAfterCompilation);
    EXPECT_EQ(workspace.FinalizeBeforePublishingKernelElf(), 0);
    EXPECT_FALSE(fs::exists(worktreePath));
}
} // namespace
