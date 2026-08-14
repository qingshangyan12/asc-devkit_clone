/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include "kernel_specialization_log.h"

#include <cstdarg>
#include <cstdio>
#include <mutex>

namespace ascendc {
namespace aclrtc {
namespace {
std::mutex gCapturedLogsMutex;
std::string gCapturedLogs;
} // namespace

void RecordKernelSpecializationTestLog(const char* severity, const char* format, ...) noexcept
{
    try {
        char formattedMessage[2048] {};
        va_list arguments;
        va_start(arguments, format);
        const int formatResult = vsnprintf(formattedMessage, sizeof(formattedMessage), format, arguments);
        va_end(arguments);
        if (formatResult < 0) {
            return;
        }
        std::lock_guard<std::mutex> lock(gCapturedLogsMutex);
        gCapturedLogs.append(severity).append(": ").append(formattedMessage).append("\n");
    } catch (...) {
    }
}

void ClearCapturedKernelSpecializationLogs()
{
    std::lock_guard<std::mutex> lock(gCapturedLogsMutex);
    gCapturedLogs.clear();
}

bool CapturedKernelSpecializationLogsContain(const std::string& expectedText)
{
    std::lock_guard<std::mutex> lock(gCapturedLogsMutex);
    return gCapturedLogs.find(expectedText) != std::string::npos;
}

} // namespace aclrtc
} // namespace ascendc
