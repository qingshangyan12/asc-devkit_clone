/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "bundle_publisher.h"

#include <cerrno>
#include <cstring>
#include <stdexcept>

#include <stdio.h>

#include "ascendc_tool_log.h"
#include "build_workspace.h"
#include "file_utils.h"

namespace ascendc {
namespace manifest_generator {
namespace {

std::runtime_error SystemError(const std::string& operation)
{
    return std::runtime_error(operation + ": " + std::strerror(errno));
}

void CheckOutputPath(const std::string& output)
{
    if (!FileUtils::CheckNonSymlink(output)) {
        throw std::runtime_error("bundle output must not be a symlink and must be inspectable: " + output);
    }
}

} // namespace

void BundlePublisher::Prepare(const std::string& output) { CheckOutputPath(output); }

void BundlePublisher::Publish(const std::string& staged, const std::string& output)
{
    CheckOutputPath(output);
    if (rename(staged.c_str(), output.c_str()) == 0) {
        ASCENDLOGI("Published compile resource bundle %s", output.c_str());
        return;
    }
    if (errno != EXDEV) {
        throw SystemError("failed to publish bundle output");
    }
    PublishAcrossFileSystems(staged, output);
    ASCENDLOGI("Published compile resource bundle %s", output.c_str());
}

void BundlePublisher::PublishAcrossFileSystems(const std::string& staged, const std::string& output)
{
    ASCENDLOGD("Copying staged bundle across filesystems: %s -> %s", staged.c_str(), output.c_str());
    BuildWorkspace publishWorkspace = BuildWorkspace::Create(FileUtils::ParentPath(output), false);
    const std::string copied = FileUtils::JoinPath(publishWorkspace.Session(), "compile_resource_bundle.so");
    if (!FileUtils::CopyFileExclusive(staged, copied)) {
        throw std::runtime_error("failed to copy staged bundle across filesystems: " + staged);
    }
    CheckOutputPath(output);
    if (rename(copied.c_str(), output.c_str()) != 0) {
        throw SystemError("failed to publish copied bundle output");
    }
}

} // namespace manifest_generator
} // namespace ascendc
