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
#include <utility>

#define private public
#include "build_workspace.h"
#undef private
#include "file_utils.h"
#include "test_support.h"

namespace {

bool g_failNextMkdtemp = false;

} // namespace

extern "C" char* __real_mkdtemp(char* pathTemplate);

extern "C" char* __wrap_mkdtemp(char* pathTemplate)
{
    if (g_failNextMkdtemp) {
        g_failNextMkdtemp = false;
        errno = EACCES;
        return nullptr;
    }
    return __real_mkdtemp(pathTemplate);
}

namespace ascendc {
namespace manifest_generator {
namespace {

using asc_compile_exporter_test::LstatPathExists;
using asc_compile_exporter_test::ModuleTest;
using asc_compile_exporter_test::ScopedEnvironment;
using asc_compile_exporter_test::WriteTestFile;

TEST_F(ModuleTest, BuildWorkspaceHonorsCleanupPolicy)
{
    const std::string parent = FileUtils::JoinPath(root_, "sessions");
    std::string removedSession;
    {
        BuildWorkspace workspace;
        ASSERT_TRUE(BuildWorkspace::Create(parent, false, workspace));
        removedSession = workspace.Session();
        EXPECT_TRUE(FileUtils::IsDirectory(removedSession));
    }
    EXPECT_FALSE(LstatPathExists(removedSession));

    std::string keptSession;
    {
        BuildWorkspace workspace;
        ASSERT_TRUE(BuildWorkspace::Create(parent, true, workspace));
        keptSession = workspace.Session();
    }
    EXPECT_TRUE(FileUtils::IsDirectory(keptSession));

    const std::string regularFile = FileUtils::JoinPath(root_, "not-a-directory");
    WriteTestFile(regularFile, "file");
    BuildWorkspace failedWorkspace;
    EXPECT_FALSE(BuildWorkspace::Create(regularFile, false, failedWorkspace));
    EXPECT_FALSE(BuildWorkspace::Create("/proc", false, failedWorkspace));
}

TEST_F(ModuleTest, BuildWorkspaceUsesTmpDirAndSupportsMoveConstruction)
{
    ScopedEnvironment temporary("TMPDIR", root_.c_str());
    std::string session;
    {
        BuildWorkspace workspace;
        ASSERT_TRUE(BuildWorkspace::Create("", false, workspace));
        session = workspace.Session();
        EXPECT_TRUE(FileUtils::IsPathWithin(session, root_));
        BuildWorkspace moved(std::move(workspace));
        EXPECT_TRUE(workspace.Session().empty());
        EXPECT_EQ(moved.Session(), session);
    }
    EXPECT_FALSE(LstatPathExists(session));
}

TEST_F(ModuleTest, BuildWorkspaceReportsCreationAndCleanupFailures)
{
    BuildWorkspace failedWorkspace;
    g_failNextMkdtemp = true;
    EXPECT_FALSE(BuildWorkspace::Create(root_, false, failedWorkspace));

    BuildWorkspace cleanupFailure("/dev/null/child", false);
    EXPECT_EQ(cleanupFailure.Session(), "/dev/null/child");
}

} // namespace
} // namespace manifest_generator
} // namespace ascendc
