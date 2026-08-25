/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef VECTOR_COPY_TENSOR_API_DATA_UTILS_H
#define VECTOR_COPY_TENSOR_API_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& filePath, size_t& fileSize, void* buffer, size_t bufferSize)
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

    auto* fileBuffer = file.rdbuf();
    size_t size = fileBuffer->pubseekoff(0, std::ios::end, std::ios::in);
    if (size == 0 || size > bufferSize) {
        ERROR_LOG("Invalid file size: %s", filePath.c_str());
        return false;
    }
    fileBuffer->pubseekpos(0, std::ios::in);
    fileBuffer->sgetn(static_cast<char*>(buffer), size);
    fileSize = size;
    return true;
}

inline bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("Output buffer is null: %s", filePath.c_str());
        return false;
    }

    int fd = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWRITE);
    if (fd < 0) {
        ERROR_LOG("Failed to open file: %s", filePath.c_str());
        return false;
    }
    size_t writeSize = write(fd, buffer, size);
    (void)close(fd);
    if (writeSize != size) {
        ERROR_LOG("Failed to write file: %s", filePath.c_str());
        return false;
    }
    return true;
}

#endif // VECTOR_COPY_TENSOR_API_DATA_UTILS_H
