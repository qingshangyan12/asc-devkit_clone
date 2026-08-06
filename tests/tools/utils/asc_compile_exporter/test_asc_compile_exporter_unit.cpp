/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <gtest/gtest.h>

#include <sstream>

#define main AscCompileExporterProgramMain
#include "asc_compile_exporter.cpp"
#undef main

namespace {

class CerrCapture final {
public:
    CerrCapture() : previous_(std::cerr.rdbuf(stream_.rdbuf()))
    {
    }

    ~CerrCapture()
    {
        std::cerr.rdbuf(previous_);
    }

    std::string Content() const
    {
        return stream_.str();
    }

    CerrCapture(const CerrCapture &) = delete;
    CerrCapture &operator=(const CerrCapture &) = delete;

private:
    std::ostringstream stream_;
    std::streambuf *previous_;
};

TEST(CompileExporterCliUnitTest, ParseJobsAcceptsOnlyPositiveUint32Values)
{
    uint32_t jobs = 99U;
    EXPECT_TRUE(ParseJobs("1", jobs));
    EXPECT_EQ(jobs, 1U);
    EXPECT_TRUE(ParseJobs("4294967295", jobs));
    EXPECT_EQ(jobs, 4294967295U);

    EXPECT_FALSE(ParseJobs("0", jobs));
    EXPECT_FALSE(ParseJobs("-1", jobs));
    EXPECT_FALSE(ParseJobs("4jobs", jobs));
    EXPECT_FALSE(ParseJobs("4294967296", jobs));
    EXPECT_EQ(jobs, 4294967295U);
}

TEST(CompileExporterCliUnitTest, RunRejectsIncompleteAndUnknownCommands)
{
    CerrCapture capture;
    char program[] = "asc_compile_exporter";
    char command[] = "--build_collected_manifest_bundle";
    char unknown[] = "--unknown";
    char *noCommand[] = {program};
    char *incomplete[] = {program, command};
    char *unknownCommand[] = {program, unknown};

    EXPECT_EQ(::Run(1, noCommand), 1);
    EXPECT_EQ(::Run(2, incomplete), 1);
    EXPECT_EQ(::Run(2, unknownCommand), 1);
    EXPECT_NE(capture.Content().find("Usage:"), std::string::npos);
}

TEST(CompileExporterCliUnitTest, RunRejectsDuplicateAndInvalidOptionsBeforeCompilation)
{
    CerrCapture capture;
    char program[] = "asc_compile_exporter";
    char command[] = "--build_collected_manifest_bundle";
    char package[] = "package";
    char manifestRoot[] = "collection";
    char output[] = "output/bundle.so";
    char keepTemp[] = "--keep-temp";
    char jobsOption[] = "--jobs";
    char zeroJobs[] = "0";
    char *duplicateKeepTemp[] = {
        program, command, package, manifestRoot, output, keepTemp, keepTemp
    };
    char *invalidJobs[] = {
        program, command, package, manifestRoot, output, jobsOption, zeroJobs
    };

    EXPECT_EQ(::Run(7, duplicateKeepTemp), 1);
    EXPECT_EQ(::Run(7, invalidJobs), 1);
    EXPECT_NE(capture.Content().find("Usage:"), std::string::npos);
}

} // namespace
