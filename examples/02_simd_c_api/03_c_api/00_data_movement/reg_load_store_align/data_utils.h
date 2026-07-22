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

#include <cstdio>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) std::fprintf(stdout, "[ERROR] " fmt "\n", ##args)

inline bool read_file(const std::string& file_path, void* buffer, size_t expected_size)
{
    if (buffer == nullptr || expected_size == 0) {
        ERROR_LOG("invalid input buffer or size");
        return false;
    }
    std::ifstream file(file_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        ERROR_LOG("failed to open %s", file_path.c_str());
        return false;
    }
    if (file.tellg() != static_cast<std::streamsize>(expected_size)) {
        ERROR_LOG("unexpected size for %s", file_path.c_str());
        return false;
    }
    file.seekg(0, std::ios::beg);
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expected_size));
    return file.good();
}

inline bool write_file(const std::string& file_path, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("invalid output buffer");
        return false;
    }
    std::ofstream file(file_path, std::ios::binary | std::ios::trunc);
    if (!file.is_open()) {
        ERROR_LOG("failed to open %s", file_path.c_str());
        return false;
    }
    file.write(static_cast<const char*>(buffer), static_cast<std::streamsize>(size));
    return file.good();
}

#endif
