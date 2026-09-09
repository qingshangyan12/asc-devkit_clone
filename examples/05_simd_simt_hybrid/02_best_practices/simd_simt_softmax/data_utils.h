/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#pragma once

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) std::fprintf(stderr, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& filePath, size_t expectedSize, void* buffer, size_t bufferSize)
{
    struct stat statBuf;
    if (stat(filePath.c_str(), &statBuf) != 0) {
        ERROR_LOG("failed to stat file: %s", filePath.c_str());
        return false;
    }
    if (S_ISREG(statBuf.st_mode) == 0) {
        ERROR_LOG("%s is not a regular file", filePath.c_str());
        return false;
    }
    if (static_cast<size_t>(statBuf.st_size) != expectedSize) {
        ERROR_LOG(
            "%s size mismatch, expected %zu bytes, actual %zu bytes", filePath.c_str(), expectedSize,
            static_cast<size_t>(statBuf.st_size));
        return false;
    }
    if (expectedSize > bufferSize) {
        ERROR_LOG("%s is larger than buffer", filePath.c_str());
        return false;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("failed to open file: %s", filePath.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expectedSize));
    if (static_cast<size_t>(file.gcount()) != expectedSize) {
        ERROR_LOG("failed to read complete file: %s", filePath.c_str());
        return false;
    }
    return true;
}

inline bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("write file failed because buffer is nullptr");
        return false;
    }

    int fd = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWRITE);
    if (fd < 0) {
        ERROR_LOG("failed to open file: %s", filePath.c_str());
        return false;
    }

    const ssize_t writeSize = write(fd, buffer, size);
    (void)close(fd);
    if (writeSize < 0 || static_cast<size_t>(writeSize) != size) {
        ERROR_LOG("failed to write complete file: %s", filePath.c_str());
        return false;
    }
    return true;
}
