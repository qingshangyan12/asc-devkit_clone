/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file data_utils.h
 * \brief 数据读取和写入工具函数
 */

#include <cstdint>
#include <iostream>
#include <vector>

extern void *memcpy_s(void *dest, size_t destMax, const void *src, size_t count);
extern void *memset_s(void *dest, size_t destMax, int c, size_t count);

#define ERROR_IF(expr)                                           \
    do {                                                         \
        if (expr) {                                              \
            std::cout << "Error at line: " << __LINE__ << std::endl; \
        }                                                        \
    } while (0)

inline void ReadFile(const char *filePath, size_t fileSize, void *buffer, size_t bufferSize)
{
    ERROR_IF(bufferSize < fileSize);

    FILE *fp = fopen(filePath, "rb");
    ERROR_IF(fp == nullptr);

    size_t readSize = fread(buffer, 1, fileSize, fp);
    ERROR_IF(readSize != fileSize);

    fclose(fp);
}

inline void WriteFile(const char *filePath, void *buffer, size_t size)
{
    FILE *fp = fopen(filePath, "wb");
    ERROR_IF(fp == nullptr);

    size_t writeSize = fwrite(buffer, 1, size, fp);
    ERROR_IF(writeSize != size);

    fclose(fp);
}
