/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_AICPU_TIMING_RECORDER_TEST_LOG_STUB_H
#define HCCL_AICPU_TIMING_RECORDER_TEST_LOG_STUB_H

#include <cstdio>
#include <string>

extern uint32_t g_aicpuTimingRunInfoLogCount;
extern uint32_t g_aicpuTimingWarningLogCount;
extern std::string g_aicpuTimingLastRunInfo;

template <typename... Args>
inline void AicpuTimingRecorderTestRunInfoLog(const char* format, const Args&... args)
{
    char buffer[4096] = {};
    (void)std::snprintf(buffer, sizeof(buffer), format, args...);
    g_aicpuTimingLastRunInfo = buffer;
    ++g_aicpuTimingRunInfoLogCount;
}

template <typename... Args>
inline void AicpuTimingRecorderTestWarningLog(const char*, const Args&...)
{
    ++g_aicpuTimingWarningLogCount;
}

#define HCCL_RUN_INFO(...) AicpuTimingRecorderTestRunInfoLog(__VA_ARGS__)
#define HCCL_WARNING(...) AicpuTimingRecorderTestWarningLog(__VA_ARGS__)

#endif // HCCL_AICPU_TIMING_RECORDER_TEST_LOG_STUB_H
