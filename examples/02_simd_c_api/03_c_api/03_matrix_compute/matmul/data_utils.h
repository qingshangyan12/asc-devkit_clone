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

#include <cstdio>
#include <fstream>
#include <string>

inline bool ReadFileExact(const std::string& path, void* buffer, size_t expectedSize)
{
    struct stat fileStat {};
    if (stat(path.c_str(), &fileStat) != 0 || !S_ISREG(fileStat.st_mode)) {
        std::fprintf(stderr, "[ERROR] input is not a regular file: %s\n", path.c_str());
        return false;
    }
    if (static_cast<size_t>(fileStat.st_size) != expectedSize) {
        std::fprintf(
            stderr, "[ERROR] unexpected file size for %s: expected %zu, got %lld\n", path.c_str(), expectedSize,
            static_cast<long long>(fileStat.st_size));
        return false;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        std::fprintf(stderr, "[ERROR] failed to open input: %s\n", path.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expectedSize));
    if (!file || static_cast<size_t>(file.gcount()) != expectedSize) {
        std::fprintf(stderr, "[ERROR] failed to read complete input: %s\n", path.c_str());
        return false;
    }
    return true;
}

inline bool WriteFileExact(const std::string& path, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        std::fprintf(stderr, "[ERROR] output buffer is null\n");
        return false;
    }

    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        std::fprintf(stderr, "[ERROR] failed to open output: %s\n", path.c_str());
        return false;
    }
    const ssize_t written = write(fd, buffer, size);
    const int closeResult = close(fd);
    if (written != static_cast<ssize_t>(size) || closeResult != 0) {
        std::fprintf(stderr, "[ERROR] failed to write complete output: %s\n", path.c_str());
        return false;
    }
    return true;
}

#endif
