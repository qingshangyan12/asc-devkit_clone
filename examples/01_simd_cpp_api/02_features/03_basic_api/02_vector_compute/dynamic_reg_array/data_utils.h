/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DYNAMIC_REG_ARRAY_DATA_UTILS_H
#define DYNAMIC_REG_ARRAY_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <fstream>
#include <string>

inline bool ReadFile(const std::string& file_path, size_t& file_size, void* buffer, size_t buffer_size)
{
    struct stat file_stat {};
    if (stat(file_path.c_str(), &file_stat) != 0 || !S_ISREG(file_stat.st_mode)) {
        return false;
    }

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        return false;
    }

    const size_t size = static_cast<size_t>(file_stat.st_size);
    if (size == 0 || size > buffer_size) {
        return false;
    }

    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(size));
    if (!file) {
        return false;
    }

    file_size = size;
    return true;
}

inline bool WriteFile(const std::string& file_path, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        return false;
    }

    int fd = open(file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        return false;
    }

    const ssize_t written_size = write(fd, buffer, size);
    close(fd);
    return written_size == static_cast<ssize_t>(size);
}

#endif // DYNAMIC_REG_ARRAY_DATA_UTILS_H
