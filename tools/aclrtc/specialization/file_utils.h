/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_ACLRTC_SPECIALIZATION_FILE_UTILS_H
#define ASCENDC_ACLRTC_SPECIALIZATION_FILE_UTILS_H

#include <cstdint>
#include <string>
#include <vector>

namespace ascendc {
namespace resource {

class FileUtils final {
public:
    FileUtils() = delete;

    static std::string JoinPath(const std::string& left, const std::string& right);
    static std::string ParentPath(const std::string& path);
    static std::string FileName(const std::string& path);
    static std::vector<std::string> PathComponents(const std::string& path);
    static bool IsWithinPath(const std::string& path, const std::string& root);

    static bool IsDirectory(const std::string& path);
    static bool IsRegularFile(const std::string& path);
    static bool Exists(const std::string& path);
    static bool IsSymlink(const std::string& path);
    static bool CanonicalPath(const std::string& path, std::string& canonical);

    static bool WalkDirectory(
        const std::string& directory, bool recursive, bool ignoreOpenFailure, std::vector<std::string>& paths);
    static bool MakeTempRoot(std::string& root);
    static bool WriteFile(const std::string& path, const std::vector<uint8_t>& bytes);
    static void RemoveTree(const std::string& path) noexcept;

private:
    static bool EnsureDirectory(const std::string& path);
    static bool CreateDirectories(const std::string& path);
    static bool RemoveTreeImpl(const std::string& path);
};

} // namespace resource
} // namespace ascendc

#endif // ASCENDC_ACLRTC_SPECIALIZATION_FILE_UTILS_H
