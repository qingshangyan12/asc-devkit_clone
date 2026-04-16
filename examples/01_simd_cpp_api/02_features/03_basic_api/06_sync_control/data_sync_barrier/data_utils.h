/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdio>
#include <cstdint>

constexpr uint32_t MAX_FILE_SIZE = 1024 * 1024;

void ReadFile(const char* filePath, size_t fileSize, void* buffer, size_t bufferSize)
{
    FILE* fp = fopen(filePath, "rb");
    if (fp == nullptr) {
        printf("ReadFile: open file %s failed\n", filePath);
        return;
    }
    size_t readSize = fread(buffer, 1, fileSize, fp);
    if (readSize != fileSize) {
        printf("ReadFile: read file %s failed, readSize %zu, fileSize %zu\n", filePath, readSize, fileSize);
    }
    fclose(fp);
}

void WriteFile(const char* filePath, void* buffer, size_t bufferSize)
{
    FILE* fp = fopen(filePath, "wb");
    if (fp == nullptr) {
        printf("WriteFile: open file %s failed\n", filePath);
        return;
    }
    size_t writeSize = fwrite(buffer, 1, bufferSize, fp);
    if (writeSize != bufferSize) {
        printf("WriteFile: write file %s failed, writeSize %zu, bufferSize %zu\n", filePath, writeSize, bufferSize);
    }
    fclose(fp);
}
