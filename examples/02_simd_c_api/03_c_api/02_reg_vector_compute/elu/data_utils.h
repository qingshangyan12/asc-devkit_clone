/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef ELU_DATA_UTILS_H
#define ELU_DATA_UTILS_H

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

inline bool ReadBinaryFileExact(const std::string& path, void* buffer, size_t expectedSize)
{
    std::ifstream file(path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "[ERROR] Failed to open " << path << std::endl;
        return false;
    }

    const std::streamoff fileSize = file.tellg();
    if (fileSize < 0 || static_cast<size_t>(fileSize) != expectedSize) {
        std::cerr << "[ERROR] Unexpected file size for " << path << ": expected " << expectedSize << ", got "
                  << fileSize << std::endl;
        return false;
    }

    if (expectedSize == 0) {
        return true;
    }

    file.seekg(0, std::ios::beg);
    file.read(static_cast<char*>(buffer), static_cast<std::streamsize>(expectedSize));
    if (!file || static_cast<size_t>(file.gcount()) != expectedSize) {
        std::cerr << "[ERROR] Failed to read all bytes from " << path << std::endl;
        return false;
    }
    return true;
}

template <typename T>
inline bool ReadVectorExact(const std::string& path, std::vector<T>& data)
{
    return ReadBinaryFileExact(path, data.empty() ? nullptr : data.data(), data.size() * sizeof(T));
}

#endif // ELU_DATA_UTILS_H
