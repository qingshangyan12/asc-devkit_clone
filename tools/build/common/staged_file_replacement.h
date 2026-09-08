/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_COMMON_STAGED_FILE_REPLACEMENT_H
#define ASCENDC_COMMON_STAGED_FILE_REPLACEMENT_H

#include <string>
#include <sys/types.h>

namespace ascendc {

// Owns a same-directory temporary file until replacement succeeds. Destruction
// removes any unpublished file, including during exception unwinding.
class StagedFileReplacement final {
public:
    StagedFileReplacement() = default;
    ~StagedFileReplacement() noexcept;
    StagedFileReplacement(const StagedFileReplacement&) = delete;
    StagedFileReplacement& operator=(const StagedFileReplacement&) = delete;

    // Each object stages one replacement. Success means all I/O is complete and
    // all descriptors are closed; the destination remains untouched until commit.
    bool StageTextFile(const std::string& destinationPath, const std::string& text);
    bool StageFileCopy(const std::string& sourcePath, const std::string& destinationPath);
    bool ReplaceDestinationAtomically() noexcept;

private:
    int CreateTemporaryFile(const std::string& destinationPath, mode_t mode);

    std::string canonicalDestinationPath_;
    std::string ownedTemporaryPath_;
    bool ownsTemporaryFile_{false};
    bool readyForReplacement_{false};
};

} // namespace ascendc

#endif // ASCENDC_COMMON_STAGED_FILE_REPLACEMENT_H
