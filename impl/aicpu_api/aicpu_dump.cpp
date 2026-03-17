/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/*!
 * \file aicpu_dump.cpp
 * \brief
 */

#include <stdarg.h>
#include <mutex>
#include <stdio.h>
#include "securec.h"
#include "aicpu_api/aicpu_api.h"

struct DumpConfig {
    void *dumpBufAddr;
    size_t dumpBufSize;
    size_t dumpOffset;
};

constexpr size_t DUMP_BUFFER_HEADER_SIZE = sizeof(uint64_t);

std::mutex dump_mutex;
extern "C" {
DumpConfig g_aicpuDumpConfig = {nullptr, 0x100000, 0};
}

namespace AscendC {
void printf(const char *fmt, ...)
{
    std::lock_guard<std::mutex> lock(dump_mutex);
    if (g_aicpuDumpConfig.dumpBufAddr == nullptr) {
        return;
    }
    size_t dataOffset = g_aicpuDumpConfig.dumpOffset;
    size_t dataSize = g_aicpuDumpConfig.dumpBufSize - DUMP_BUFFER_HEADER_SIZE;
    if (dataOffset >= dataSize) {
        return;
    }
    va_list otherArgs;
    va_start(otherArgs, fmt);
    dataOffset += vsnprintf_s(
        reinterpret_cast<char*>(g_aicpuDumpConfig.dumpBufAddr) + DUMP_BUFFER_HEADER_SIZE + dataOffset,
        dataSize - dataOffset,
        dataSize - dataOffset, fmt, otherArgs);
    g_aicpuDumpConfig.dumpOffset = dataOffset;
    *reinterpret_cast<size_t*>(g_aicpuDumpConfig.dumpBufAddr) = dataOffset;
    va_end(otherArgs);
}
}
