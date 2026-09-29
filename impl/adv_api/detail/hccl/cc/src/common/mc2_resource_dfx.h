/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MC2_RESOURCE_DFX_H
#define ASCENDC_MC2_RESOURCE_DFX_H

#include <cerrno>
#include <cstdarg>
#include <cstring>
#include <sys/syscall.h>
#include <unistd.h>
#include "log.h"

namespace Mc2ResourceDfx {
// Independent failure sink: no Dlog level check, stdio buffering, allocation or
// business lock. Keep errno unchanged and never replace the original result.
static inline void WriteStderr(const char* format, ...)
{
    const int savedErrno = errno;
    char buffer[2048] = {};
    const int prefix = snprintf_s(buffer, sizeof(buffer), sizeof(buffer) - 1,
        "[MC2_ALLOC_DFX_v2][stderr] pid[%ld] tid[%ld] ",
        static_cast<long>(getpid()), static_cast<long>(syscall(SYS_gettid)));
    if (prefix > 0 && static_cast<size_t>(prefix) < sizeof(buffer) - 1) {
        va_list args;
        va_start(args, format);
        // On truncation securec terminates the available buffer and returns -1.
        const size_t available = sizeof(buffer) - static_cast<size_t>(prefix);
        (void)vsnprintf_s(buffer + prefix, available, available - 1, format, args);
        va_end(args);
        buffer[sizeof(buffer) - 1] = '\0';
        size_t length = std::strlen(buffer);
        if (length >= sizeof(buffer) - 1) {
            length = sizeof(buffer) - 2;
        }
        buffer[length++] = '\n';
        size_t offset = 0;
        while (offset < length) {
            const ssize_t written = write(STDERR_FILENO, buffer + offset, length - offset);
            if (written > 0) {
                offset += static_cast<size_t>(written);
            } else if (written < 0 && errno == EINTR) {
                continue;
            } else {
                break; // Closed/full output must not change the allocation result.
            }
        }
    }
    errno = savedErrno;
}

// Write the independent sink first, even if Dlog filters or cannot persist logs.
#define MC2_DFX_LOG(...) \
    do { \
        Mc2ResourceDfx::WriteStderr(__VA_ARGS__); \
        HCCL_ERROR(__VA_ARGS__); \
        HCCL_RUN_INFO(__VA_ARGS__); \
    } while (0)

// Success never emits a message; expressions are evaluated exactly once.
static inline HcclResult Result(HcclResult result, const char* file, int line, const char* function, const char* call)
{
    if (result != HCCL_SUCCESS) {
        MC2_DFX_LOG("[MC2_ALLOC_DFX_v2] file[%s:%d] function[%s] call[%s] ret[%d]",
            file, line, function, call, static_cast<int>(result));
    }
    return result;
}
} // namespace Mc2ResourceDfx

#define MC2_DFX_RESULT(call) Mc2ResourceDfx::Result((call), __FILE__, __LINE__, __func__, #call)
#define MC2_DFX_CHK_RET(call) \
    do { \
        const HcclResult mc2DfxRet = MC2_DFX_RESULT(call); \
        if (mc2DfxRet != HCCL_SUCCESS) { \
            return mc2DfxRet; \
        } \
    } while (0)
#define MC2_DFX_CHK_PTR_NULL(ptr) \
    do { \
        if ((ptr) == nullptr) { \
            return Mc2ResourceDfx::Result(HCCL_E_PTR, __FILE__, __LINE__, __func__, #ptr " == nullptr"); \
        } \
    } while (0)
#define MC2_DFX_CHK_PRT_RET(condition, log, result) \
    do { \
        if (condition) { \
            log; \
            return Mc2ResourceDfx::Result((result), __FILE__, __LINE__, __func__, #condition); \
        } \
    } while (0)

#endif // ASCENDC_MC2_RESOURCE_DFX_H
