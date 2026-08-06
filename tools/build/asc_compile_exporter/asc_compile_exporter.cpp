/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <charconv>
#include <cstdint>
#include <exception>
#include <filesystem>
#include <iostream>
#include <string>
#include <system_error>
#include <utility>

#include "ascendc_tool_log.h"
#include "manifest_generator.h"

namespace {

void PrintUsage(const char *program)
{
    std::cerr << "Usage:\n  " << program
              << " --build_collected_manifest_bundle <package_id> <manifest_search_root> <out.so>"
                 " [work_dir] [--keep-temp] [--jobs <N>]\n";
}

bool ParseJobs(const std::string &text, uint32_t &jobs)
{
    uint32_t parsed = 0U;
    const auto result = std::from_chars(text.data(), text.data() + text.size(), parsed);
    if (result.ec != std::errc{} || result.ptr != text.data() + text.size() || parsed == 0U) {
        return false;
    }
    jobs = parsed;
    return true;
}

int Run(int argc, char **argv)
{
    if (argc < 2) {
        PrintUsage(argv[0]);
        return 1;
    }
    const std::string command = argv[1];
    if (command == "--build_collected_manifest_bundle") {
        if (argc < 5 || argc > 9) {
            PrintUsage(argv[0]);
            return 1;
        }
        std::filesystem::path workDir;
        bool keepTemp = false;
        uint32_t jobs = 0U;
        bool jobsSpecified = false;
        for (int index = 5; index < argc; ++index) {
            const std::string argument = argv[index];
            if (argument == "--keep-temp" && !keepTemp) {
                keepTemp = true;
            } else if (argument == "--jobs" && !jobsSpecified && index + 1 < argc &&
                       ParseJobs(argv[index + 1], jobs)) {
                jobsSpecified = true;
                ++index;
            } else if (workDir.empty() && argument.rfind("--", 0U) != 0U) {
                workDir = argument;
            } else {
                PrintUsage(argv[0]);
                return 1;
            }
        }
        ascendc::manifest_generator::ManifestBundleCompiler compiler(
            {argv[2], argv[3], argv[4], std::move(workDir), keepTemp, jobs});
        compiler.Compile();
        return 0;
    }
    PrintUsage(argv[0]);
    return 1;
}

} // namespace

int main(int argc, char **argv)
{
    try {
        return Run(argc, argv);
    } catch (const std::exception &error) {
        ASCENDLOGE("asc_compile_exporter failed: %s", error.what());
        return 1;
    }
}
