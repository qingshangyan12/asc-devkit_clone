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
using DumpFunc = void (*)(uint32_t, ChannelHandle *);

DumpFunc ResolveRealDumpFunc()
{
    static DumpFunc dumpFunc = []() -> DumpFunc {
        const char *soPath = std::getenv("HCOMM_REAL_DUMP_SO");
        if (soPath == nullptr || soPath[0] == '\0') {
            soPath = "libhccl.so";
        }

        void *handle = dlopen(soPath, RTLD_NOW | RTLD_LOCAL);
        if (handle == nullptr) {
            std::printf("[hcomm_channel_dump_shim] dlopen %s failed: %s\n", soPath, dlerror());
            return nullptr;
        }

        dlerror();
        void *symbol = dlsym(handle, "HcommChannelInfoDump");
        const char *error = dlerror();
        if (error != nullptr) {
            std::printf("[hcomm_channel_dump_shim] dlsym HcommChannelInfoDump failed: %s\n", error);
            return nullptr;
        }
        std::printf("[hcomm_channel_dump_shim] resolved real HcommChannelInfoDump from %s\n", soPath);
        std::fflush(stdout);
        return reinterpret_cast<DumpFunc>(symbol);
    }();
    return dumpFunc;
}
} // namespace

extern "C" void HcommChannelInfoDump(uint32_t channelNum, ChannelHandle *channels)
{
    DumpFunc dumpFunc = ResolveRealDumpFunc();
    if (dumpFunc == nullptr) {
        return;
    }
    std::printf("[hcomm_channel_dump_shim] call real HcommChannelInfoDump channelNum=%u\n", channelNum);
    std::fflush(stdout);
    dumpFunc(channelNum, channels);
    std::printf("[hcomm_channel_dump_shim] real HcommChannelInfoDump returned\n");
    std::fflush(stdout);
}
