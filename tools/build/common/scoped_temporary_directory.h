/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_COMMON_SCOPED_TEMPORARY_DIRECTORY_H
#define ASCENDC_COMMON_SCOPED_TEMPORARY_DIRECTORY_H

#include <string>

namespace ascendc {

class ScopedTemporaryDirectory final {
public:
    ScopedTemporaryDirectory() = default;
    ~ScopedTemporaryDirectory() noexcept;

    ScopedTemporaryDirectory(ScopedTemporaryDirectory&& other) noexcept;
    ScopedTemporaryDirectory& operator=(ScopedTemporaryDirectory&& other) noexcept;

    ScopedTemporaryDirectory(const ScopedTemporaryDirectory&) = delete;
    ScopedTemporaryDirectory& operator=(const ScopedTemporaryDirectory&) = delete;

    bool CreateUniqueSubdirectory(const std::string& parentDirectoryPath, const std::string& directoryNamePrefix);
    bool TrackDirectoryForAutomaticCleanup(std::string directoryPath);
    bool RemoveTrackedDirectory() noexcept;
    void DisableAutomaticCleanup() noexcept;

    const std::string& GetDirectoryPath() const noexcept;

private:
    void MoveStateFrom(ScopedTemporaryDirectory& other) noexcept;

    std::string trackedDirectoryPath_;
    bool automaticCleanupEnabled_{true};
};

} // namespace ascendc

#endif // ASCENDC_COMMON_SCOPED_TEMPORARY_DIRECTORY_H
