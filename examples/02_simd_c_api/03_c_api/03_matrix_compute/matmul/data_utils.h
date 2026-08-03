/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MATMUL_DATA_UTILS_H
#define MATMUL_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cerrno>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <limits>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& filePath, void* buffer, size_t expectedSize)
{
    if (buffer == nullptr || expectedSize == 0) {
        ERROR_LOG("Read file failed. buffer is nullptr or expected size is 0");
        return false;
    }

    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) != 0) {
        ERROR_LOG("Failed to stat file. path = %s", filePath.c_str());
        return false;
    }
    if (!S_ISREG(fileStat.st_mode)) {
        ERROR_LOG("Path is not a regular file. path = %s", filePath.c_str());
        return false;
    }
    if (fileStat.st_size <= 0 || static_cast<uintmax_t>(fileStat.st_size) != expectedSize) {
        ERROR_LOG(
            "File size does not match. path = %s, expected = %zu, actual = %lld", filePath.c_str(), expectedSize,
            static_cast<long long>(fileStat.st_size));
        return false;
    }
    if (expectedSize > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())) {
        ERROR_LOG("File is too large for stream I/O. path = %s", filePath.c_str());
        return false;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("Open file failed. path = %s", filePath.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expectedSize));
    if (file.gcount() != static_cast<std::streamsize>(expectedSize)) {
        ERROR_LOG("Read file failed. path = %s", filePath.c_str());
        return false;
    }
    return true;
}

inline bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr || size == 0) {
        ERROR_LOG("Write file failed. buffer is nullptr or size is 0");
        return false;
    }

    int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWRITE);
    if (fd < 0) {
        ERROR_LOG("Open file failed. path = %s", filePath.c_str());
        return false;
    }

    size_t writtenSize = 0;
    const char* src = static_cast<const char*>(buffer);
    while (writtenSize < size) {
        ssize_t writeSize = write(fd, src + writtenSize, size - writtenSize);
        if (writeSize < 0) {
            if (errno == EINTR) {
                continue;
            }
            ERROR_LOG("Write file failed. path = %s", filePath.c_str());
            (void)close(fd);
            return false;
        }
        if (writeSize == 0) {
            ERROR_LOG("Write file failed. no data was written");
            (void)close(fd);
            return false;
        }
        writtenSize += static_cast<size_t>(writeSize);
    }

    if (close(fd) != 0) {
        ERROR_LOG("Close file failed. path = %s", filePath.c_str());
        return false;
    }
    return true;
}

#endif // MATMUL_DATA_UTILS_H
