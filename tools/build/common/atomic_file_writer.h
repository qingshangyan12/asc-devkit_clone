/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ASCENDC_COMMON_ATOMIC_FILE_WRITER_H
#define ASCENDC_COMMON_ATOMIC_FILE_WRITER_H

#include <cstdint>
#include <string>
#include <sys/types.h>

namespace ascendc {

// Owns a same-directory temporary file until replacement succeeds. Destruction
// removes any unpublished file, including during exception unwinding.
class AtomicFileWriter final {
public:
    AtomicFileWriter() = default;
    ~AtomicFileWriter() noexcept;
    AtomicFileWriter(const AtomicFileWriter&) = delete;
    AtomicFileWriter& operator=(const AtomicFileWriter&) = delete;

    // Each object writes one temporary file. Success means all I/O is complete and
    // all descriptors are closed; the destination remains untouched until ReplaceDestinationAtomically succeeds.
    bool WriteTextToTemporaryFile(const std::string& destinationPath, const std::string& text);
    bool CopyFileToTemporaryFile(const std::string& sourcePath, const std::string& destinationPath);
    bool ReplaceDestinationAtomically() noexcept;

private:
    int32_t CreateUniqueTemporaryFile(const std::string& destinationPath, mode_t permissions);

    std::string canonicalDestinationPath_;
    std::string ownedTemporaryFilePath_;
    bool ownsTemporaryFile_{false};
    bool temporaryFileWriteSucceeded_{false};
};

} // namespace ascendc

#endif // ASCENDC_COMMON_ATOMIC_FILE_WRITER_H
