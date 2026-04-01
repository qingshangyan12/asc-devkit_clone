/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software; you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/


/* !
 * \file data_utils.h
 * \brief 数据读写工具函数
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

const int SUCCESS = 0;
const int FAILED = 1;

/**
 * @brief 读取二进制文件
 */
bool ReadFile(const char* filePath, size_t fileSize, void* buffer, size_t bufferSize)
{
    if (buffer == nullptr) {
        std::cerr << "buffer is nullptr" << std::endl;
        return false;
    }

    struct stat sBuf;
    int fileStatus = stat(filePath, &sBuf);
    if (fileStatus == -1) {
        std::cerr << "stat file failed, file path is " << filePath << std::endl;
        return false;
    }

    if (sBuf.st_size != static_cast<long>(fileSize)) {
        std::cerr << "file size not equal, file path is " << filePath << std::endl;
        return false;
    }

    std::ifstream file(filePath, std::ifstream::binary);
    if (!file.is_open()) {
        std::cerr << "open file failed, file path is " << filePath << std::endl;
        return false;
    }

    file.read(static_cast<char*>(buffer), bufferSize);
    file.close();
    return true;
}

/**
 * @brief 写入二进制文件
 */
bool WriteFile(const char* filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        std::cerr << "buffer is nullptr" << std::endl;
        return false;
    }

    std::ofstream file(filePath, std::ofstream::binary);
    if (!file.is_open()) {
        std::cerr << "open file failed, file path is " << filePath << std::endl;
        return false;
    }

    file.write(static_cast<const char*>(buffer), size);
    file.close();
    return true;
}
