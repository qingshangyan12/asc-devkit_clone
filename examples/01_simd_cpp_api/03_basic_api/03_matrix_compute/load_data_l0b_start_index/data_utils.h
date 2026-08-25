/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef LOAD_DATA_L0B_START_INDEX_DATA_UTILS_H
#define LOAD_DATA_L0B_START_INDEX_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR] " fmt "\n", ##args)

bool ReadFile(const std::string& filePath, size_t& fileSize, void* buffer, size_t bufferSize)
{
    struct stat fileStat;
    if (stat(filePath.c_str(), &fileStat) == -1 || S_ISREG(fileStat.st_mode) == 0) {
        ERROR_LOG("Invalid input file: %s", filePath.c_str());
        return false;
    }
    std::ifstream file(filePath, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("Failed to open file: %s", filePath.c_str());
        return false;
    }
    file.seekg(0, std::ios::end);
    const size_t size = static_cast<size_t>(file.tellg());
    if (size == 0 || size > bufferSize) {
        ERROR_LOG("Invalid file size: %zu, buffer size: %zu", size, bufferSize);
        return false;
    }
    file.seekg(0, std::ios::beg);
    file.read(static_cast<char*>(buffer), size);
    if (!file) {
        ERROR_LOG("Failed to read file: %s", filePath.c_str());
        return false;
    }
    fileSize = size;
    return true;
}

bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("Output buffer is null");
        return false;
    }
    const int fd = open(filePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        ERROR_LOG("Failed to open file: %s", filePath.c_str());
        return false;
    }
    const ssize_t writeSize = write(fd, buffer, size);
    (void)close(fd);
    if (writeSize != static_cast<ssize_t>(size)) {
        ERROR_LOG("Failed to write file: %s", filePath.c_str());
        return false;
    }
    return true;
}

#endif  // LOAD_DATA_L0B_START_INDEX_DATA_UTILS_H
