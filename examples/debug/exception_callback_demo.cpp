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

#include "acl/acl_rt.h"
#include "hcomm_exception_fault_cases.h"

namespace {
using ChannelHandle = uint64_t;
using HcommChannelInfoDumpFunc = void (*)(uint32_t, ChannelHandle *);

HcommChannelInfoDumpFunc ResolveChannelDumpFunc()
{
    static HcommChannelInfoDumpFunc dumpFunc = []() -> HcommChannelInfoDumpFunc {
        const char *soPath = std::getenv("HCOMM_CHANNEL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhcomm_channel_dump.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[ExceptionCallbackDemo] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }

        dlerror();
        void *symbol = dlsym(handle, "HcommChannelInfoDump");
        const char *error = dlerror();
        if (error != nullptr) {
            std::printf("[ExceptionCallbackDemo] dlsym HcommChannelInfoDump failed: %s\n", error);
            return nullptr;
        }
        return reinterpret_cast<HcommChannelInfoDumpFunc>(symbol);
    }();
    return dumpFunc;
}

bool GetCommContext(aclrtExceptionInfo *exceptionInfo, CommContext &hostCtx)
{
    if (exceptionInfo == nullptr) {
        std::printf("[ExceptionCallbackDemo] exceptionInfo is nullptr.\n");
        return false;
    }

    auto argAddr = exceptionInfo->expandInfo.aicoreInfo.exceptionArgs.argAddr;
    if (argAddr == nullptr) {
        std::printf("[ExceptionCallbackDemo] exception argAddr is 0.\n");
        return false;
    }

    aclError ret = aclrtMemcpy(&hostCtx, sizeof(hostCtx), argAddr, sizeof(CommContext), ACL_MEMCPY_DEVICE_TO_HOST);
    if (ret != ACL_SUCCESS) {
        std::printf("[ExceptionCallbackDemo] copy CommContext failed, ret=%d.\n", ret);
        return false;
    }
    return true;
}

void CustomCallback(aclrtExceptionInfo *exceptionInfo)
{
    CommContext ctx = {};
    if (!GetCommContext(exceptionInfo, ctx)) {
        return;
    }

    std::printf("[ExceptionCallbackDemo] rankId=%u channel=0x%llx localBuffer=0x%llx remoteBuffer=0x%llx\n",
        ctx.rankId,
        static_cast<unsigned long long>(ctx.channelHandle),
        static_cast<unsigned long long>(ctx.localBufferAddr),
        static_cast<unsigned long long>(ctx.remoteBufferAddr));

    if (ctx.channelHandle == 0) {
        std::printf("[ExceptionCallbackDemo] channel handle is 0, skip channel dump.\n");
        return;
    }

    ChannelHandle channel = static_cast<ChannelHandle>(ctx.channelHandle);
    auto dumpFunc = ResolveChannelDumpFunc();
    if (dumpFunc == nullptr) {
        return;
    }
    dumpFunc(1, &channel);
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
    return 0;
}
