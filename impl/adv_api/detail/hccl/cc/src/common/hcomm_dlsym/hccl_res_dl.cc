/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hccl_res_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

namespace {
using HcclDedicatedThreadAcquireFunc =
    HcclResult (*)(HcclComm comm, int32_t useType, uint32_t notifyNumPerThread, ThreadHandle* thread);

constexpr int32_t HCCL_DED_THREAD_TYPE_AICPU_ORDER_LAUNCH_DEVICE_VALUE = 5;

struct Mc2DedicatedThreadApi {
    void* libHcommHandle = nullptr;
    HcclDedicatedThreadAcquireFunc acquire = nullptr;
    HcclResult loadRet = HCCL_SUCCESS;

    Mc2DedicatedThreadApi()
    {
        dlerror();
        libHcommHandle = dlopen("libhcomm.so", RTLD_NOW | RTLD_LOCAL);
        if (libHcommHandle == nullptr) {
            const char* err = dlerror();
            HCCL_ERROR(
                "[Mc2HcommWrapper] failed to open libhcomm.so for HcclDedicatedThreadAcquire, %s",
                err == nullptr ? "(no error)" : err);
            loadRet = HCCL_E_INTERNAL;
            return;
        }

        dlerror();
        acquire = reinterpret_cast<HcclDedicatedThreadAcquireFunc>(
            dlsym(libHcommHandle, "HcclDedicatedThreadAcquire"));
        const char* err = dlerror();
        if (err != nullptr || acquire == nullptr) {
            acquire = nullptr;
            HCCL_DEBUG(
                "[Mc2HcommWrapper] HcclDedicatedThreadAcquire not supported, %s",
                err == nullptr ? "(no error)" : err);
        }
    }
};

const Mc2DedicatedThreadApi& GetMc2DedicatedThreadApi()
{
    // C++11 guarantees thread-safe initialization. Keep libhcomm loaded for the
    // process lifetime so that the cached function pointer always remains valid.
    static const Mc2DedicatedThreadApi api;
    return api;
}
} // namespace

// ---------- 桩函数定义（签名与真实API完全一致）----------
DEFINE_WEAK_FUNC(HcclResult, HcclGetRemoteIpcHcclBuf, HcclComm comm, uint64_t remoteRank, void** addr, uint64_t* size);
DEFINE_WEAK_FUNC(int32_t, HcclTaskRegister, HcclComm comm, const char* msgTag, Callback cb);
DEFINE_WEAK_FUNC(int32_t, HcclTaskUnRegister, HcclComm comm, const char* msgTag);
DEFINE_WEAK_FUNC(
    HcclResult, HcclDevMemAcquire, HcclComm comm, const char* memTag, uint64_t* size, void** addr, bool* newCreated);
DEFINE_WEAK_FUNC(
    HcclResult, HcclThreadExportToCommEngine, HcclComm comm, uint32_t threadNum, const ThreadHandle* threads,
    CommEngine dstCommEngine, ThreadHandle* exportedThreads);
DEFINE_WEAK_FUNC(
    HcclResult, HcclChannelGetRemoteMems, HcclComm comm, ChannelHandle channel, uint32_t* memNum, CommMem** remoteMems,
    char*** memTags);
DEFINE_WEAK_FUNC(
    HcclResult, HcclCommMemReg, HcclComm comm, const char* memTag, const CommMem* mem, HcclMemHandle* memHandle);
DEFINE_WEAK_FUNC(HcclResult, HcclEngineCtxDestroy, HcclComm comm, const char* ctxTag, CommEngine engine);

HcclResult Mc2HcommAcquireDeviceOrderLaunchThread(
    HcclComm comm, uint32_t notifyNumPerThread, ThreadHandle* thread, bool* supported)
{
    CHK_PTR_NULL(thread);
    CHK_PTR_NULL(supported);
    *thread = 0;
    *supported = false;

    const Mc2DedicatedThreadApi& api = GetMc2DedicatedThreadApi();
    if (api.loadRet != HCCL_SUCCESS) {
        return api.loadRet;
    }
    if (api.acquire == nullptr) {
        return HCCL_SUCCESS;
    }

    *supported = true;
    return api.acquire(
        comm, HCCL_DED_THREAD_TYPE_AICPU_ORDER_LAUNCH_DEVICE_VALUE, notifyNumPerThread, thread);
}

// 初始化
void HcclResDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcclGetRemoteIpcHcclBuf);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskUnRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclDevMemAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadExportToCommEngine);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelGetRemoteMems);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCommMemReg);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxDestroy);
}
