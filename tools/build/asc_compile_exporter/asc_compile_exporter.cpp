/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cerrno>
#include <climits>
#include <cstdlib>
#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <utility>

#include "ascendc_tool_log.h"
#include "manifest_bundle_compiler.h"

namespace {

void PrintUsage(const char* program)
{
    std::cerr << "Usage:\n  " << program
              << " <manifest_search_root> <out.so> --make <path> --cxx <path>"
                 " [--keep-temp] [--jobs <N>]\n\n"
                 "Arguments:\n"
                 "  <manifest_search_root>  Directory recursively searched for *_manifest.json files.\n"
                 "  <out.so>                Output shared-library path, including its parent directory.\n"
                 "  --make <path>           Path to the make executable used to build generated sources.\n"
                 "  --cxx <path>            Path to the C++ compiler used by the generated Makefile.\n"
                 "  --keep-temp             Preserve the private build directory under TMPDIR or /tmp.\n"
                 "  --jobs <N>              Positive make job count, capped at the detected CPU count;\n"
                 "                            defaults to half the CPU count, with a minimum of one.\n";
}

bool ParseJobs(const std::string& text, uint32_t& jobs)
{
    if (text.empty()) {
        return false;
    }
    for (const char character : text) {
        if (character < '0' || character > '9') {
            return false;
        }
    }
    errno = 0;
    char* end = nullptr;
    const unsigned long parsed = std::strtoul(text.c_str(), &end, 10);
    if (errno == ERANGE || end == text.c_str() || *end != '\0' || parsed == 0UL || parsed > UINT32_MAX) {
        return false;
    }
    jobs = static_cast<uint32_t>(parsed);
    return true;
}

int Run(int argc, char** argv)
{
    if (argc < 3 || argc > 10) {
        PrintUsage(argv[0]);
        return 1;
    }
    std::string makeExecutable;
    std::string cxxCompiler;
    bool keepTemp = false;
    uint32_t jobs = 0U;
    bool jobsSpecified = false;
    for (int index = 3; index < argc; ++index) {
        const std::string argument = argv[index];
        if (argument == "--keep-temp" && !keepTemp) {
            keepTemp = true;
        } else if (argument == "--make" && makeExecutable.empty() && index + 1 < argc) {
            makeExecutable = argv[++index];
        } else if (argument == "--cxx" && cxxCompiler.empty() && index + 1 < argc) {
            cxxCompiler = argv[++index];
        } else if (argument == "--jobs" && !jobsSpecified && index + 1 < argc && ParseJobs(argv[index + 1], jobs)) {
            jobsSpecified = true;
            ++index;
        } else {
            PrintUsage(argv[0]);
            return 1;
        }
    }
    if (makeExecutable.empty() || cxxCompiler.empty()) {
        PrintUsage(argv[0]);
        return 1;
    }
    ascendc::manifest_generator::BuildCollectedBundleRequest request;
    request.manifestSearchRoot = argv[1];
    request.outputSo = argv[2];
    request.makeExecutable = std::move(makeExecutable);
    request.cxxCompiler = std::move(cxxCompiler);
    request.keepTemp = keepTemp;
    request.jobs = jobs;
    ascendc::manifest_generator::ManifestBundleCompiler compiler(std::move(request));
    compiler.Compile();
    return 0;
}

} // namespace

int main(int argc, char** argv)
{
    try {
        return Run(argc, argv);
    } catch (const std::exception& error) {
        ASCENDLOGE("asc_compile_exporter failed: %s", error.what());
        return 1;
    }
}
