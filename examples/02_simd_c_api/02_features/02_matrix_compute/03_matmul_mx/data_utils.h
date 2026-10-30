/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MATMUL_MX_DATA_UTILS_H
#define MATMUL_MX_DATA_UTILS_H

#include <cstdio>
#include <cstdlib>

inline void read_file(const char* file_path, void* buffer, size_t file_size)
{
    FILE* file = fopen(file_path, "rb");
    if (file == nullptr) {
        fprintf(stderr, "Cannot open %s\n", file_path);
        exit(EXIT_FAILURE);
    }
    bool valid = fread(buffer, 1, file_size, file) == file_size && fgetc(file) == EOF && !ferror(file);
    int close_status = fclose(file);
    if (!valid || close_status != 0) {
        fprintf(stderr, "Cannot read %s (expected exactly %zu bytes)\n", file_path, file_size);
        exit(EXIT_FAILURE);
    }
}

inline void write_file(const char* file_path, const void* buffer, size_t file_size)
{
    FILE* file = fopen(file_path, "wb");
    if (file == nullptr) {
        fprintf(stderr, "Cannot open %s\n", file_path);
        exit(EXIT_FAILURE);
    }
    bool valid = fwrite(buffer, 1, file_size, file) == file_size;
    int close_status = fclose(file);
    if (!valid || close_status != 0) {
        fprintf(stderr, "Cannot write %s\n", file_path);
        exit(EXIT_FAILURE);
    }
}

#endif // MATMUL_MX_DATA_UTILS_H
