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
 * \brief
 */

#ifndef DATA_UTILS_H
#define DATA_UTILS_H
#ifdef ASCENDC_CPU_DEBUG
#include "tiling/platform/platform_ascendc.h"
#endif
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fstream>

#define ERROR_LOG(fmt, args...) fprintf(stdout, "[ERROR]  " fmt "\n", ##args)

#ifdef ASCENDC_CPU_DEBUG
static inline const char* GetCpuDebugSocVersion()
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201)
    return "Ascend910B1";
#elif defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    return "Ascend950PR_9599";
#else
    return nullptr;
#endif
}
#endif

static inline bool InitCpuDebugPlatform()
{
#ifdef ASCENDC_CPU_DEBUG
    const char* socVersion = GetCpuDebugSocVersion();
    if (socVersion == nullptr) {
        ERROR_LOG("CPU debug mode only supports dav-2201 and dav-3510 in this sample.");
        return false;
    }
    if (platform_ascendc::PlatformAscendCManager::GetInstance(socVersion) == nullptr) {
        ERROR_LOG("Failed to initialize CPU debug platform for %s.", socVersion);
        return false;
    }
#endif
    return true;
}

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
    size_t size = buf->pubseekoff(0, std::ios::end, std::ios::in);
    if (size == 0) {
        ERROR_LOG("file size is 0");
        file.close();
        return false;
    }
    if (size > bufferSize) {
        ERROR_LOG("file size is larger than buffer size");
        file.close();
        return false;
    }
    buf->pubseekpos(0, std::ios::in);
    buf->sgetn(static_cast<char*>(buffer), size);
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

    size_t writeSize = write(fd, buffer, size);
    (void)close(fd);
    if (writeSize != size) {
        ERROR_LOG("Write file Failed.");
        return false;
    }

    return true;
}
#endif // DATA_UTILS_H
