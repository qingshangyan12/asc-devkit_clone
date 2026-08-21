/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_COMMON_TEMPORARY_DIRECTORY_H
#define ASCENDC_COMMON_TEMPORARY_DIRECTORY_H

#include <string>

namespace ascendc {

class TemporaryDirectory final {
public:
    TemporaryDirectory() = default;
    ~TemporaryDirectory() noexcept;

    TemporaryDirectory(TemporaryDirectory&& other) noexcept;
    TemporaryDirectory& operator=(TemporaryDirectory&& other) noexcept;

    TemporaryDirectory(const TemporaryDirectory&) = delete;
    TemporaryDirectory& operator=(const TemporaryDirectory&) = delete;

    bool CreateUniqueUnder(const std::string& parentDirectoryPath, const std::string& directoryNamePrefix);
    bool AdoptExistingForCleanup(std::string directoryPath);
    bool RemoveNow() noexcept;
    void RetainAfterDestruction() noexcept;

    const std::string& GetPath() const noexcept;

private:
    void TakeCleanupOwnershipFrom(TemporaryDirectory& other) noexcept;

    std::string ownedDirectoryPath_;
    bool removeAfterDestruction_{true};
};

} // namespace ascendc

#endif // ASCENDC_COMMON_TEMPORARY_DIRECTORY_H
