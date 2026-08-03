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

#define error_log(fmt, args...) fprintf(stdout, "[ERROR]  " fmt "\n", ##args)

bool read_file(const std::string& file_path, size_t& file_size, void* buffer, size_t buffer_size)
{
    struct stat s_buf;
    int file_status = stat(file_path.data(), &s_buf);
    if (file_status == -1) {
        error_log("failed to get file");
        return false;
    }
    if (S_ISREG(s_buf.st_mode) == 0) {
        error_log("%s is not a file, please enter a file", file_path.c_str());
        return false;
    }

    std::ifstream file;
    file.open(file_path, std::ios::binary);
    if (!file.is_open()) {
        error_log("Open file failed. path = %s", file_path.c_str());
        return false;
    }

    std::filebuf* buf = file.rdbuf();
    std::streampos end_pos = buf->pubseekoff(0, std::ios::end, std::ios::in);
    if (end_pos == std::streampos(-1)) {
        error_log("failed to seek file. path = %s", file_path.c_str());
        file.close();
        return false;
    }
    size_t size = static_cast<size_t>(end_pos);
    if (size == 0) {
        error_log("file size is 0");
        file.close();
        return false;
    }
    // 要求文件大小与调用方期望的 buffer_size 完全一致，避免截短后使用未初始化/补零尾部继续测试
    if (size != buffer_size) {
        error_log("file size (%zu) does not match expected buffer size (%zu)", size, buffer_size);
        file.close();
        return false;
    }
    if (buf->pubseekpos(0, std::ios::in) == std::streampos(-1)) {
        error_log("failed to seek file. path = %s", file_path.c_str());
        file.close();
        return false;
    }
    std::streamsize read_size = buf->sgetn(static_cast<char*>(buffer), size);
    if (read_size != static_cast<std::streamsize>(size)) {
        error_log("failed to read file. path = %s", file_path.c_str());
        file.close();
        return false;
    }
    file_size = size;
    file.close();
    return true;
}

bool write_file(const std::string& file_path, const void* buffer, size_t size)
{
    if (buffer == nullptr) {
        error_log("Write file failed. buffer is nullptr");
        return false;
    }

    int fd = open(file_path.c_str(), O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWRITE);
    if (fd < 0) {
        error_log("Open file failed. path = %s", file_path.c_str());
        return false;
    }

    // 循环写入直至累计 size 字节：write 可能只写入部分字节，且被信号打断（EINTR）时需重试。
    // 用 ssize_t 接收返回值（write 返回 -1 或已写字节数，不能用无符号类型接收）。
    const char* p = static_cast<const char*>(buffer);
    size_t written = 0;
    while (written < size) {
        ssize_t write_size = write(fd, p + written, size - written);
        if (write_size < 0) {
            if (errno == EINTR) {
                continue; // 被信号打断，重试本次写入
            }
            (void)close(fd);
            error_log("Write file failed. path = %s", file_path.c_str());
            return false;
        }
        written += static_cast<size_t>(write_size);
    }
    (void)close(fd);
    return true;
}
#endif // DATA_UTILS_H
