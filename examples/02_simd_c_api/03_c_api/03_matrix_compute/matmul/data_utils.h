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

#define ERROR_LOG(fmt, args...) fprintf(stderr, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& path, void* buffer, size_t expectedSize)
{
    struct stat fileStat;
    if (stat(path.c_str(), &fileStat) != 0 || !S_ISREG(fileStat.st_mode)) {
        ERROR_LOG("%s is not a regular file", path.c_str());
        return false;
    }
    if (static_cast<size_t>(fileStat.st_size) != expectedSize) {
        ERROR_LOG(
            "unexpected size of %s: expected %zu, got %zu", path.c_str(), expectedSize,
            static_cast<size_t>(fileStat.st_size));
        return false;
    }

    std::ifstream file(path, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("failed to open %s", path.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expectedSize));
    if (static_cast<size_t>(file.gcount()) != expectedSize) {
        ERROR_LOG("failed to read %s", path.c_str());
        return false;
    }
    return true;
}

inline bool WriteFile(const std::string& path, const void* buffer, size_t size)
{
    int fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        ERROR_LOG("failed to open %s", path.c_str());
        return false;
    }
    const ssize_t written = write(fd, buffer, size);
    (void)close(fd);
    if (written < 0 || static_cast<size_t>(written) != size) {
        ERROR_LOG("failed to write %s", path.c_str());
        return false;
    }
    return true;
}

#endif // MATMUL_DATA_UTILS_H
