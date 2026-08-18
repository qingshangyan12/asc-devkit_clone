/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <cerrno>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <fstream>
#include <limits>
#include <string>
#include <sys/stat.h>

#define ERROR_LOG(fmt, args...) std::fprintf(stderr, "[ERROR] " fmt "\n", ##args)

inline bool ReadFile(const std::string& file_path, void* buffer, size_t expected_size)
{
    if (buffer == nullptr || expected_size == 0) {
        ERROR_LOG("invalid read buffer or size");
        return false;
    }

    struct stat stat_buffer;
    if (stat(file_path.c_str(), &stat_buffer) != 0 || !S_ISREG(stat_buffer.st_mode) ||
        static_cast<uintmax_t>(stat_buffer.st_size) != static_cast<uintmax_t>(expected_size)) {
        ERROR_LOG("invalid input file or size: %s", file_path.c_str());
        return false;
    }

    std::ifstream file(file_path, std::ios::binary);
    if (!file.is_open() || expected_size > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())) {
        ERROR_LOG("failed to open input file: %s", file_path.c_str());
        return false;
    }
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expected_size));
    return file.good() || file.gcount() == static_cast<std::streamsize>(expected_size);
}

inline bool WriteFile(const std::string& file_path, const void* buffer, size_t size)
{
    if (buffer == nullptr || size == 0) {
        ERROR_LOG("invalid output buffer or size");
        return false;
    }

    std::ofstream file(file_path, std::ios::binary | std::ios::trunc);
    if (!file.is_open() || size > static_cast<size_t>(std::numeric_limits<std::streamsize>::max())) {
        ERROR_LOG("failed to open output file: %s", file_path.c_str());
        return false;
    }
    file.write(static_cast<const char*>(buffer), static_cast<std::streamsize>(size));
    return file.good();
}

#endif // DATA_UTILS_H
