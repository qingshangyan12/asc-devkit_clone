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
HcclDedicatedThreadAcquireFunc g_hcclDedicatedThreadAcquire = nullptr;
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

bool HcommIsSupportHcclDedicatedThreadAcquire(void)
{
    return g_hcclDedicatedThreadAcquire != nullptr;
}

HcclResult HcommAcquireDeviceOrderLaunchThread(
    HcclComm comm, uint32_t notifyNumPerThread, ThreadHandle* thread)
{
    if (g_hcclDedicatedThreadAcquire == nullptr) {
        HCCL_ERROR("[HcclWrapper] HcclDedicatedThreadAcquire not supported");
        return HCCL_E_NOT_SUPPORT;
    }
    return g_hcclDedicatedThreadAcquire(
        comm, HCCL_DED_THREAD_TYPE_AICPU_ORDER_LAUNCH_DEVICE_VALUE, notifyNumPerThread, thread);
}

// 初始化
void HcclResDlInit(void* libHcommHandle)
{
    dlerror();
    g_hcclDedicatedThreadAcquire = reinterpret_cast<HcclDedicatedThreadAcquireFunc>(
        dlsym(libHcommHandle, "HcclDedicatedThreadAcquire"));
    const char* dedicatedThreadErr = dlerror();
    if (g_hcclDedicatedThreadAcquire == nullptr) {
        HCCL_DEBUG(
            "[HcclWrapper] HcclDedicatedThreadAcquire not supported, %s",
            dedicatedThreadErr == nullptr ? "(no error)" : dedicatedThreadErr);
    }
    INIT_SUPPORT_FLAG(libHcommHandle, HcclGetRemoteIpcHcclBuf);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskUnRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclDevMemAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadExportToCommEngine);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelGetRemoteMems);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCommMemReg);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxDestroy);
}
