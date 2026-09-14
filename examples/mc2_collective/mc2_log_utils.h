/*
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#ifndef MC2_LOG_UTILS_H
#define MC2_LOG_UTILS_H

#include <cerrno>
#include <cstdio>
#include <cstdlib>
#include <ftw.h>

namespace Mc2LogUtils {

inline int RemoveLogPathEntry(const char* path, const struct stat* pathStatus, int typeFlag, struct FTW* ftwBuffer)
{
    (void)pathStatus;
    (void)typeFlag;
    (void)ftwBuffer;
    if (remove(path) != 0 && errno != ENOENT) {
        printf("[ERROR] remove failed for %s, errno=%d\n", path, errno);
        return -1;
    }
    return 0;
}

inline bool ClearAscendProcessLogPath()
{
    const char* logPath = std::getenv("ASCEND_PROCESS_LOG_PATH");
    if (logPath == nullptr || logPath[0] == '\0') {
        return true;
    }

    errno = 0;
    if (nftw(logPath, RemoveLogPathEntry, 64, FTW_DEPTH | FTW_PHYS) != 0) {
        if (errno == ENOENT) {
            return true;
        }
        printf("[ERROR] failed to remove ASCEND_PROCESS_LOG_PATH=%s, errno=%d\n", logPath, errno);
        return false;
    }
    printf("removed ASCEND_PROCESS_LOG_PATH=%s\n", logPath);
    return true;
}

}  // namespace Mc2LogUtils

#endif
