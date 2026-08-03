/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/* !
 * \file data_utils.h
 * \brief ELU样例的输入输出数据读写辅助工具，提供文件读取、写入功能（与同目录其它算子样例保持一致）
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#include <cerrno>
#include <cstdio>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>
#include <string>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR]  " fmt "\n", ##args)

bool ReadFile(const std::string& filePath, size_t& fileSize, void* buffer, size_t bufferSize)
{
    struct stat sBuf;
    int fileStatus = stat(filePath.data(), &sBuf);
    if (fileStatus == -1) {
        ERROR_LOG("failed to get file");
        return false;
    }
    if (S_ISREG(sBuf.st_mode) == 0) {
        ERROR_LOG("%s is not a file, please enter a file", filePath.c_str());
        return false;
    }

    std::ifstream file;
    file.open(filePath, std::ios::binary);
    if (!file.is_open()) {
        ERROR_LOG("Open file failed. path = %s", filePath.c_str());
        return false;
    }

    std::filebuf* buf = file.rdbuf();
    std::streampos endPos = buf->pubseekoff(0, std::ios::end, std::ios::in);
    if (endPos == std::streampos(-1)) {
        ERROR_LOG("failed to seek file. path = %s", filePath.c_str());
        file.close();
        return false;
    }
    size_t size = static_cast<size_t>(endPos);
    if (size == 0) {
        ERROR_LOG("file size is 0");
        file.close();
        return false;
    }
    // 要求文件大小与调用方期望的 bufferSize 完全一致，避免截短后使用未初始化/补零尾部继续测试
    if (size != bufferSize) {
        ERROR_LOG("file size (%zu) does not match expected buffer size (%zu)", size, bufferSize);
        file.close();
        return false;
    }
    if (buf->pubseekpos(0, std::ios::in) == std::streampos(-1)) {
        ERROR_LOG("failed to seek file. path = %s", filePath.c_str());
        file.close();
        return false;
    }
    std::streamsize readSize = buf->sgetn(static_cast<char*>(buffer), size);
    if (readSize != static_cast<std::streamsize>(size)) {
        ERROR_LOG("failed to read file. path = %s", filePath.c_str());
        file.close();
        return false;
    }
    fileSize = size;
    file.close();
    return true;
}

bool WriteFile(const std::string& filePath, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        ERROR_LOG("Write file failed. buffer is nullptr");
        return false;
    }

    int fd = open(filePath.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWRITE);
    if (fd < 0) {
        ERROR_LOG("Open file failed. path = %s", filePath.c_str());
        return false;
    }

    // 循环写入直至累计 size 字节：write 可能只写入部分字节，且被信号打断（EINTR）时需重试。
    // 用 ssize_t 接收返回值（write 返回 -1 或已写字节数，不能用无符号类型接收）。
    const char* p = static_cast<const char*>(buffer);
    size_t written = 0;
    while (written < size) {
        ssize_t writeSize = write(fd, p + written, size - written);
        if (writeSize < 0) {
            if (errno == EINTR) {
                continue; // 被信号打断，重试本次写入
            }
            (void)close(fd);
            ERROR_LOG("Write file failed. path = %s", filePath.c_str());
            return false;
        }
        written += static_cast<size_t>(writeSize);
    }
    (void)close(fd);
    return true;
}
#endif // DATA_UTILS_H
