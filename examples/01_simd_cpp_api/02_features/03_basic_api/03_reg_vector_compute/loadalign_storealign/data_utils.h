/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*
 * \brief LoadAlignStoreAlign样例的输入输出数据读写辅助工具，提供文件读取、写入功能
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <cmath>
#include <iostream>
#include <acl/acl.h>

constexpr uint32_t TOTAL_LENGTH = 256;
uint32_t deviceId = 0;

static inline bool ReadFile(const std::string& filePath, size_t fileSize, void* buffer, size_t bufferSize)
{
    if (bufferSize < fileSize) {
        return false;
    }
    FILE* fp = fopen(filePath.c_str(), "rb");
    if (fp == nullptr) {
        return false;
    }
    size_t readSize = fread(buffer, 1, fileSize, fp);
    fclose(fp);
    if (readSize != fileSize) {
        return false;
    }
    return true;
}

static inline bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    FILE* fp = fopen(filePath.c_str(), "wb");
    if (fp == nullptr) {
        return false;
    }
    size_t writeSize = fwrite(buffer, 1, size, fp);
    fclose(fp);
    if (writeSize != size) {
        return false;
    }
    return true;
}

#endif // DATA_UTILS_H
