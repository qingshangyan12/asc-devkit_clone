/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "build_workspace.h"

#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <stdexcept>
#include <utility>
#include <vector>

#include "ascendc_tool_log.h"
#include "file_utils.h"

namespace ascendc {
namespace manifest_generator {
namespace {

std::runtime_error SystemError(const std::string& operation)
{
    return std::runtime_error(operation + ": " + std::strerror(errno));
}

} // namespace

BuildWorkspace BuildWorkspace::Create(const std::string& requestedParent, bool keepTemp)
{
    const char* temporary = std::getenv("TMPDIR");
    const std::string requested =
        requestedParent.empty() ? (temporary == nullptr || temporary[0] == '\0' ? "/tmp" : temporary) : requestedParent;
    std::string absolute;
    if (!FileUtils::MakeAbsolutePath(requested, absolute)) {
        throw std::runtime_error("failed to resolve bundle work directory: " + requested);
    }
    if (!FileUtils::CreateDirectories(absolute)) {
        throw std::runtime_error("failed to create bundle work directory: " + absolute);
    }
    std::string parent;
    if (!FileUtils::ResolveDirectory(absolute, parent)) {
        throw std::runtime_error("bundle work directory is unavailable: " + absolute);
    }
    std::string pathTemplate = FileUtils::JoinPath(parent, ".manifest-generator-XXXXXX");
    std::vector<char> buffer(pathTemplate.begin(), pathTemplate.end());
    buffer.push_back('\0');
    char* created = mkdtemp(buffer.data());
    if (created == nullptr) {
        throw SystemError("failed to create private bundle work directory");
    }
    ASCENDLOGD("Created private bundle work directory %s", created);
    return BuildWorkspace(created, keepTemp);
}

BuildWorkspace::BuildWorkspace(BuildWorkspace&& other) noexcept
    : session_(std::move(other.session_)), keepTemp_(other.keepTemp_)
{
    other.session_.clear();
}

BuildWorkspace::~BuildWorkspace()
{
    if (!session_.empty() && !keepTemp_) {
        if (FileUtils::RemoveAll(session_)) {
            ASCENDLOGD("Removed private bundle work directory %s", session_.c_str());
        } else {
            ASCENDLOGW("Failed to completely remove private bundle work directory %s", session_.c_str());
        }
    }
}

const std::string& BuildWorkspace::Session() const noexcept { return session_; }

BuildWorkspace::BuildWorkspace(std::string session, bool keepTemp) noexcept
    : session_(std::move(session)), keepTemp_(keepTemp)
{}

} // namespace manifest_generator
} // namespace ascendc
