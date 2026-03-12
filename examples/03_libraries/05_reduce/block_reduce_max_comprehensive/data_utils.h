/**
 * Copyright (c) 2025 Huawei Technologies Co.， Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <fstream>
#include <iostream>
#include <vector>

inline void ReadFile(const std::string& fileName, size_t fileSize, void* data, size_t dataSize)
{
    std::ifstream inFile(fileName, std::ios::binary);
    if (!inFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }
    inFile.read((char*)data, dataSize);
    inFile.close();
}

inline void WriteFile(const std::string& fileName, void* data, size_t dataSize)
{
    std::ofstream outFile(fileName, std::ios::binary);
    if (!outFile.is_open()) {
        std::cerr << "Failed to open file: " << fileName << std::endl;
        return;
    }
    outFile.write((char*)data, dataSize);
    outFile.close();
}

#endif // DATA_UTILS_H
