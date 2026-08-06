/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "make_bundle_builder.h"

#include <utility>

#include "ascendc_tool_log.h"
#include "process_runner.h"

namespace ascendc {
namespace manifest_generator {

MakeBundleBuilder::MakeBundleBuilder(
    std::string workDir, std::string output, std::string makeExecutable, std::string cxxCompiler,
    uint32_t jobs) noexcept
    : workDir_(std::move(workDir)),
      output_(std::move(output)),
      makeExecutable_(std::move(makeExecutable)),
      cxxCompiler_(std::move(cxxCompiler)),
      jobs_(jobs)
{}

void MakeBundleBuilder::Build() const
{
    ASCENDLOGD(
        "Building bundle with make=%s compiler=%s jobs=%u", makeExecutable_.c_str(), cxxCompiler_.c_str(), jobs_);
    ProcessRunner::Run(
        {makeExecutable_, "-C", workDir_, "-f", "Makefile", "-B", "-j" + std::to_string(jobs_)},
        {{"ASCENDC_BUNDLE_CXX", cxxCompiler_}, {"ASCENDC_BUNDLE_OUTPUT", output_}});
    ASCENDLOGI("Built staged compile resource bundle %s", output_.c_str());
}

} // namespace manifest_generator
} // namespace ascendc
