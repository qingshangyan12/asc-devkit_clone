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
#include <stdio.h>

#include "ascendc_tool_log.h"
#include "build_workspace.h"
#include "file_utils.h"

namespace ascendc {
namespace manifest_generator {

bool BundlePublisher::Publish(const std::string& staged, const std::string& output)
{
    // Prefer rename because replacing the output is atomic when both paths are on the same filesystem.
    if (!FileUtils::CheckNonSymlink(output)) {
        ASCENDLOGE("Bundle output must not be a symlink and must be inspectable: %s", output.c_str());
        return false;
    }
    if (rename(staged.c_str(), output.c_str()) == 0) {
        ASCENDLOGI("Published compile resource bundle %s", output.c_str());
        return true;
    }
    if (errno != EXDEV) {
        const int publishError = errno;
        ASCENDLOGE("Failed to publish bundle output: %s", std::strerror(publishError));
        return false;
    }
    // rename cannot cross filesystems. Fall back to copying into the output filesystem before renaming again.
    if (!PublishAcrossFileSystems(staged, output)) {
        return false;
    }
    ASCENDLOGI("Published compile resource bundle %s", output.c_str());
    return true;
}

bool BundlePublisher::PublishAcrossFileSystems(const std::string& staged, const std::string& output)
{
    ASCENDLOGD("Copying staged bundle across filesystems: %s -> %s", staged.c_str(), output.c_str());
    // A private workspace under the output parent places the copied file on the output filesystem.
    BuildWorkspace publishWorkspace;
    if (!publishWorkspace.Create(FileUtils::ParentPath(output), false)) {
        return false;
    }
    const std::string copied = FileUtils::JoinPath(publishWorkspace.Session(), "compile_resource_bundle.so");
    if (!FileUtils::CopyFileExclusive(staged, copied)) {
        ASCENDLOGE("Failed to copy staged bundle across filesystems: %s", staged.c_str());
        return false;
    }

    if (rename(copied.c_str(), output.c_str()) != 0) {
        const int publishError = errno;
        ASCENDLOGE("Failed to publish copied bundle output: %s", std::strerror(publishError));
        return false;
    }
    return true;
}

} // namespace manifest_generator
} // namespace ascendc
