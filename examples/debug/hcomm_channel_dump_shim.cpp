/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>

#ifndef CHANNEL_HANDLE_DEFINED
#define CHANNEL_HANDLE_DEFINED
using ChannelHandle = uint64_t;
#endif

namespace {
using GetDumpInfoSizeFunc = int32_t (*)(uint32_t, ChannelHandle *, uint32_t *);
using DumpInfoFunc = int32_t (*)(uint32_t, ChannelHandle *, void *, uint32_t);

void *ResolveRealSymbol(const char *symbolName)
{
    static void *handle = []() -> void * {
        const char *soPath = std::getenv("HCOMM_REAL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhccl.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[hcomm_channel_dump_shim] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }
        std::printf("[hcomm_channel_dump_shim] loaded real dump library from %s\n", soPath);
        std::fflush(stdout);
        return handle;
    }();
    if (handle == nullptr) {
        return nullptr;
    }

    dlerror();
    void *symbol = dlsym(handle, symbolName);
    const char *error = dlerror();
    if (error != nullptr) {
        std::printf("[hcomm_channel_dump_shim] dlsym %s failed: %s\n", symbolName, error);
        std::fflush(stdout);
        return nullptr;
    }
    return symbol;
}

GetDumpInfoSizeFunc ResolveRealGetDumpInfoSizeFunc()
{
    static GetDumpInfoSizeFunc getSizeFunc =
        reinterpret_cast<GetDumpInfoSizeFunc>(ResolveRealSymbol("HcommChannelGetDumpInfoSize"));
    return getSizeFunc;
}

DumpInfoFunc ResolveRealDumpInfoFunc()
{
    static DumpInfoFunc dumpInfoFunc = reinterpret_cast<DumpInfoFunc>(ResolveRealSymbol("HcommChannelDumpInfo"));
    return dumpInfoFunc;
}
} // namespace

extern "C" int32_t HcommChannelGetDumpInfoSize(uint32_t channelNum, ChannelHandle *channels, uint32_t *len)
{
    GetDumpInfoSizeFunc getSizeFunc = ResolveRealGetDumpInfoSizeFunc();
    if (getSizeFunc == nullptr) {
        return -1;
    }
    return getSizeFunc(channelNum, channels, len);
}

extern "C" int32_t HcommChannelDumpInfo(uint32_t channelNum, ChannelHandle *channels, void *buffer, uint32_t len)
{
    DumpInfoFunc dumpInfoFunc = ResolveRealDumpInfoFunc();
    if (dumpInfoFunc == nullptr) {
        return -1;
    }
    return dumpInfoFunc(channelNum, channels, buffer, len);
}
