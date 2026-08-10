/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef EXAMPLES_REG_COMPUTE_TENSOR_API_DATA_UTILS_H
#define EXAMPLES_REG_COMPUTE_TENSOR_API_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stderr, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& filePath, size_t& fileSize, void* buffer, size_t bufferSize)
{
    struct stat fileStat {};
    if (stat(filePath.c_str(), &fileStat) != 0 || !S_ISREG(fileStat.st_mode)) {
        ERROR_LOG("Invalid input file: %s", filePath.c_str());
        return false;
    }

    const size_t size = static_cast<size_t>(fileStat.st_size);
    if (size == 0 || size > bufferSize) {
        ERROR_LOG("Invalid file size for %s: %zu", filePath.c_str(), size);
        return false;
    }

    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open() || !file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(size))) {
        ERROR_LOG("Failed to read file: %s", filePath.c_str());
        return false;
    }
    fileSize = size;
    return true;
}

inline bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("Cannot write a null buffer");
        return false;
    }

    const int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        ERROR_LOG("Failed to open output file: %s", filePath.c_str());
        return false;
    }

    const ssize_t written = write(fd, buffer, size);
    (void)close(fd);
    if (written < 0 || static_cast<size_t>(written) != size) {
        ERROR_LOG("Failed to write output file: %s", filePath.c_str());
        return false;
    }
    return true;
}

#endif // EXAMPLES_REG_COMPUTE_TENSOR_API_DATA_UTILS_H
