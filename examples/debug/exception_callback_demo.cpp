/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <dlfcn.h>
#include <atomic>
#include <mutex>
#include <vector>

#include "acl/acl_rt.h"
#include "hcomm_channel_dump_info.h"
#include "hcomm_exception_fault_cases.h"

namespace {
using ChannelHandle = uint64_t;
using HcommChannelGetDumpInfoSizeFunc = int32_t (*)(uint32_t, ChannelHandle *, uint32_t *);
using HcommChannelDumpInfoFunc = int32_t (*)(uint32_t, ChannelHandle *, void *, uint32_t);

void *ResolveChannelDumpSymbol(const char *symbolName)
{
    static void *handle = []() -> void * {
        const char *soPath = std::getenv("HCOMM_CHANNEL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhcomm_channel_dump.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[ExceptionCallbackDemo] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }
        return handle;
    }();
    if (handle == nullptr) {
        return nullptr;
    }

    dlerror();
    void *symbol = dlsym(handle, symbolName);
    const char *error = dlerror();
    if (error != nullptr) {
        std::printf("[ExceptionCallbackDemo] dlsym %s failed: %s\n", symbolName, error);
        return nullptr;
    }
    return symbol;
}

HcommChannelGetDumpInfoSizeFunc ResolveChannelGetDumpInfoSizeFunc()
{
    static HcommChannelGetDumpInfoSizeFunc getSizeFunc =
        reinterpret_cast<HcommChannelGetDumpInfoSizeFunc>(ResolveChannelDumpSymbol("HcommChannelGetDumpInfoSize"));
    return getSizeFunc;
}

HcommChannelDumpInfoFunc ResolveChannelDumpInfoFunc()
{
    static HcommChannelDumpInfoFunc dumpInfoFunc =
        reinterpret_cast<HcommChannelDumpInfoFunc>(ResolveChannelDumpSymbol("HcommChannelDumpInfo"));
    return dumpInfoFunc;
}

struct ExceptionSummary {
    uint32_t taskId = 0xFFFFFFFFU;
    uint32_t streamId = 0xFFFFFFFFU;
    uint32_t threadId = 0xFFFFFFFFU;
    uint32_t deviceId = 0xFFFFFFFFU;
    uint32_t errorCode = 0xFFFFFFFFU;
    void *devArgs = nullptr;
    uint32_t devArgsLen = 0;
};

std::mutex g_exceptionMutex;
std::atomic<bool> g_exceptionPending{false};
ExceptionSummary g_lastException;

void CustomCallback(aclrtExceptionInfo *exceptionInfo)
{
    if (exceptionInfo == nullptr) {
        return;
    }

    ExceptionSummary summary;
    summary.taskId = aclrtGetTaskIdFromExceptionInfo(exceptionInfo);
    summary.streamId = aclrtGetStreamIdFromExceptionInfo(exceptionInfo);
    summary.threadId = aclrtGetThreadIdFromExceptionInfo(exceptionInfo);
    summary.deviceId = aclrtGetDeviceIdFromExceptionInfo(exceptionInfo);
    summary.errorCode = aclrtGetErrorCodeFromExceptionInfo(exceptionInfo);
    (void)aclrtGetArgsFromExceptionInfo(exceptionInfo, &summary.devArgs, &summary.devArgsLen);

    {
        std::lock_guard<std::mutex> lock(g_exceptionMutex);
        g_lastException = summary;
    }
    g_exceptionPending.store(true, std::memory_order_release);
}

bool TryGetPendingException(ExceptionSummary &summary)
{
    if (!g_exceptionPending.exchange(false, std::memory_order_acq_rel)) {
        return false;
    }
    std::lock_guard<std::mutex> lock(g_exceptionMutex);
    summary = g_lastException;
    return true;
}

void ReportPendingException()
{
    ExceptionSummary summary;
    if (!TryGetPendingException(summary)) {
        return;
    }

    std::printf("[ExceptionCallbackDemo] runtime exception task=%u stream=%u thread=%u device=%u error=%u\n",
        summary.taskId, summary.streamId, summary.threadId, summary.deviceId, summary.errorCode);

    if (summary.devArgs == nullptr) {
        std::printf("[ExceptionCallbackDemo] exception dev args are unavailable, len=%u, skip channel dump.\n",
            summary.devArgsLen);
        return;
    }

    void *ctxAddr = summary.devArgs;
    if (summary.devArgsLen == sizeof(uint64_t)) {
        uint64_t rawCtxAddr = 0;
        aclError ret = aclrtMemcpy(&rawCtxAddr, sizeof(rawCtxAddr), summary.devArgs, sizeof(rawCtxAddr),
            ACL_MEMCPY_DEVICE_TO_HOST);
        if (ret != ACL_SUCCESS) {
            std::printf("[ExceptionCallbackDemo] copy exception dev args pointer failed, ret=%d.\n", ret);
            return;
        }
        ctxAddr = reinterpret_cast<void *>(rawCtxAddr);
    } else if (summary.devArgsLen < sizeof(CommContext)) {
        std::printf("[ExceptionCallbackDemo] exception dev args len is too small, len=%u, skip channel dump.\n",
            summary.devArgsLen);
        return;
    }

    CommContext ctx = {};
    aclError ret = aclrtMemcpy(&ctx, sizeof(ctx), ctxAddr, sizeof(ctx), ACL_MEMCPY_DEVICE_TO_HOST);
    if (ret != ACL_SUCCESS) {
        std::printf("[ExceptionCallbackDemo] copy CommContext failed at report point, ret=%d.\n", ret);
        return;
    }

    std::printf("[ExceptionCallbackDemo] rankId=%u channel=0x%llx localBuffer=0x%llx remoteBuffer=0x%llx\n",
        ctx.rankId,
        static_cast<unsigned long long>(ctx.channelHandle),
        static_cast<unsigned long long>(ctx.localBufferAddr),
        static_cast<unsigned long long>(ctx.remoteBufferAddr));

    ChannelHandle channel = static_cast<ChannelHandle>(ctx.channelHandle);
    auto getSizeFunc = ResolveChannelGetDumpInfoSizeFunc();
    auto dumpInfoFunc = ResolveChannelDumpInfoFunc();
    if (getSizeFunc == nullptr || dumpInfoFunc == nullptr || channel == 0) {
        return;
    }

    uint32_t len = 0;
    int32_t dumpRet = getSizeFunc(1, &channel, &len);
    if (dumpRet != 0 || len < sizeof(HcommChannelDumpData)) {
        std::printf("[ExceptionCallbackDemo] HcommChannelGetDumpInfoSize failed, ret=%d len=%u.\n", dumpRet, len);
        return;
    }
    std::vector<uint8_t> buffer(len, 0);
    dumpRet = dumpInfoFunc(1, &channel, buffer.data(), len);
    std::printf("[ExceptionCallbackDemo] HcommChannelDumpInfo ret=%d len=%u.\n", dumpRet, len);
}
} // namespace

int main()
{
    aclError ret = aclrtSetExceptionInfoCallback(CustomCallback);
    if (ret != ACL_SUCCESS) {
        std::printf("[ExceptionCallbackDemo] aclrtSetExceptionInfoCallback failed, ret=%d.\n", ret);
        return static_cast<int>(ret);
    }

    std::printf("[ExceptionCallbackDemo] exception callback registered. Launch communication kernel here.\n");
    ReportPendingException();
    return 0;
}
