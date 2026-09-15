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
#include "mc2_thread_config_compat.h"
#include <limits>
#include <vector>
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

namespace {
using Mc2ThreadAcquireFunc = HcclResult (*)(
    HcclComm, CommEngine, uint32_t, mc2_thread_config::ThreadType, const mc2_thread_config::ThreadConfig*,
    ThreadHandle*);
Mc2ThreadAcquireFunc g_mc2ThreadAcquire = nullptr;
} // namespace

HcclResult Mc2ThreadAcquireWithConfig(
    HcclComm comm, CommEngine engine, uint32_t threadNum, uint32_t notifyNum, ThreadHandle* threads)
{
    if (notifyNum > std::numeric_limits<uint16_t>::max()) {
        HCCL_ERROR("[Mc2ThreadAcquireWithConfig] notifyNum[%u] exceeds ThreadConfig v1 range", notifyNum);
        return HCCL_E_PARA;
    }
    if (g_mc2ThreadAcquire == nullptr) {
        const CommEngine legacyEngine = engine == COMM_ENGINE_AICPU ? COMM_ENGINE_AICPU_TS : engine;
        return HcclThreadAcquire(comm, legacyEngine, threadNum, notifyNum, threads);
    }
    const std::vector<mc2_thread_config::ThreadConfig> configs(
        threadNum, mc2_thread_config::MakeConfig(static_cast<uint16_t>(notifyNum)));
    return g_mc2ThreadAcquire(comm, engine, threadNum, mc2_thread_config::THREAD_TYPE_TS, configs.data(), threads);
}

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
DEFINE_WEAK_FUNC(
    HcclResult, HcclChannelQuery, HcclComm comm, CommEngine engine, const HcclChannelDesc* channelDescs,
    uint32_t channelNum, ChannelHandle* channels);
DEFINE_WEAK_FUNC(HcclResult, HcclChannelDestroy, HcclComm comm, const ChannelHandle* channels, uint32_t channelNum);

// 初始化
void HcclResDlInit(void* libHcommHandle)
{
    g_mc2ThreadAcquire = reinterpret_cast<Mc2ThreadAcquireFunc>(dlsym(libHcommHandle, "HcclThreadAcquireWithConfig"));
    if (g_mc2ThreadAcquire == nullptr) {
        HCCL_WARNING("[MC2 profiling] ThreadConfig unavailable; use legacy resources without deferred association");
    }
    INIT_SUPPORT_FLAG(libHcommHandle, HcclGetRemoteIpcHcclBuf);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskUnRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclDevMemAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadExportToCommEngine);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelGetRemoteMems);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCommMemReg);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxDestroy);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelQuery);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelDestroy);
}
