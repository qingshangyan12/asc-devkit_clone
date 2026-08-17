/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hcomm_device_profiling_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

namespace {
void* ResolveSymbolFromHandle(void* handle, const char* name, void* self)
{
    if (handle == nullptr) {
        return nullptr;
    }
    dlerror();
    void* ptr = dlsym(handle, name);
    const char* err = dlerror();
    if (ptr == nullptr) {
        HCCL_DEBUG("[HcclWrapper] %s not found from handle[%p], %s", name, handle, err ? err : "(no error)");
        return nullptr;
    }
    if (ptr == self) {
        HCCL_DEBUG("[HcclWrapper] %s resolved to compat wrapper, skip.", name);
        return nullptr;
    }
    return ptr;
}

void* ResolveDeviceProfilingSymbol(const char* name, void* self, void* libHcommHandle)
{
    void* cclKernelHandle = dlopen("libccl_kernel.so", RTLD_NOW | RTLD_NOLOAD);
    void* ptr = ResolveSymbolFromHandle(cclKernelHandle, name, self);
    if (ptr != nullptr) {
        return ptr;
    }
    ptr = ResolveSymbolFromHandle(libHcommHandle, name, self);
    if (ptr != nullptr) {
        return ptr;
    }
    return ResolveSymbolFromHandle(RTLD_DEFAULT, name, self);
}
} // namespace

#define DEFINE_DEVICE_PROF_FUNC(type, func_name, params, args, fail_value)                  \
    using func_name##Handle = type (*) params;                                              \
    static func_name##Handle g_##func_name = nullptr;                                       \
    static bool g_##func_name##Supported = false;                                           \
    extern "C" bool HcommIsSupport##func_name(void) { return g_##func_name##Supported; }    \
    type func_name params __attribute__((weak));                                            \
    type func_name params                                                                   \
    {                                                                                       \
        if (g_##func_name != nullptr) {                                                     \
            return g_##func_name args;                                                      \
        }                                                                                   \
        HCCL_ERROR("[HcclWrapper] %s not supported", __func__);                             \
        return fail_value;                                                                  \
    }                                                                                       \
    static void Resolve##func_name(void* libHcommHandle)                                    \
    {                                                                                       \
        void* ptr = ResolveDeviceProfilingSymbol(#func_name, reinterpret_cast<void*>(func_name), libHcommHandle); \
        g_##func_name = reinterpret_cast<func_name##Handle>(ptr);                           \
        g_##func_name##Supported = (g_##func_name != nullptr);                              \
    }

DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportMainStreamAndFirstTask, (ThreadHandle thread), (thread),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportMainStreamAndLastTask, (ThreadHandle thread), (thread),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportDeviceHcclOpInfo, (HcomProInfoTmp profInfo), (profInfo),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingInit, (ThreadHandle* threads, uint32_t threadNum), (threads, threadNum),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingEnd, (ThreadHandle* threads, uint32_t threadNum), (threads, threadNum),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportDeviceOp, (const char* groupname), (groupname),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportKernelStartTask, (uint64_t thread, const char* groupname), (thread, groupname),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(
    HcclResult, HcommProfilingReportKernelEndTask, (uint64_t thread, const char* groupname), (thread, groupname),
    static_cast<HcclResult>(-1));
DEFINE_DEVICE_PROF_FUNC(uint32_t, RunAicpuDfxOpInfoInitV2, (void* args), (args), static_cast<uint32_t>(-1));

// 初始化
void HcommDeviceProfilingDlInit(void* libHcommHandle)
{
    ResolveHcommProfilingReportMainStreamAndFirstTask(libHcommHandle);
    ResolveHcommProfilingReportMainStreamAndLastTask(libHcommHandle);
    ResolveHcommProfilingReportDeviceHcclOpInfo(libHcommHandle);
    ResolveHcommProfilingInit(libHcommHandle);
    ResolveHcommProfilingEnd(libHcommHandle);
    ResolveHcommProfilingReportDeviceOp(libHcommHandle);
    ResolveHcommProfilingReportKernelStartTask(libHcommHandle);
    ResolveHcommProfilingReportKernelEndTask(libHcommHandle);
    ResolveRunAicpuDfxOpInfoInitV2(libHcommHandle);
}
