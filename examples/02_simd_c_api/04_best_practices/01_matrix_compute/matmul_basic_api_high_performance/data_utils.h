/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MMAD_DOUBLE_BUFFER_DATA_UTILS_H
#define MMAD_DOUBLE_BUFFER_DATA_UTILS_H

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& file_path, size_t& file_size, void* buffer, size_t buffer_size)
{
    struct stat file_stat;
    if (stat(file_path.c_str(), &file_stat) == -1 || S_ISREG(file_stat.st_mode) == 0) {
        ERROR_LOG("invalid input file: %s", file_path.c_str());
        return false;
    }
    if (static_cast<size_t>(file_stat.st_size) != buffer_size) {
        ERROR_LOG("unexpected file size: %s", file_path.c_str());
        return false;
    }

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("failed to open input file: %s", file_path.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(buffer_size));
    if (!file) {
        ERROR_LOG("failed to read input file: %s", file_path.c_str());
        return false;
    }
    file_size = buffer_size;
    return true;
}

inline bool WriteFile(const std::string& file_path, const void* buffer, size_t size)
{
    int fd = open(file_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd < 0) {
        ERROR_LOG("failed to open output file: %s", file_path.c_str());
        return false;
    }
    ssize_t written = write(fd, buffer, size);
    close(fd);
    if (written != static_cast<ssize_t>(size)) {
        ERROR_LOG("failed to write output file: %s", file_path.c_str());
        return false;
    }
    return true;
}

#endif
