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

DEFINE_DLSYM_FUNC(HcclResult, HcclGetHcclBuffer, (HcclComm comm, void** buffer, uint64_t* size), (comm, buffer, size));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclThreadAcquire,
    (HcclComm comm, CommEngine engine, uint32_t threadNum, uint32_t notifyNumPerThread, ThreadHandle* threads),
    (comm, engine, threadNum, notifyNumPerThread, threads));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclThreadAcquireWithStream,
    (HcclComm comm, CommEngine engine, aclrtStream stream, uint32_t notifyNum, ThreadHandle* thread),
    (comm, engine, stream, notifyNum, thread));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclThreadResGetInfo,
    (HcclComm comm, ThreadHandle thread, ThreadResType resType, uint32_t infoLen, void** info),
    (comm, thread, resType, infoLen, info));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclChannelAcquire,
    (HcclComm comm, CommEngine engine, const HcclChannelDesc* channelDescs, uint32_t channelNum,
        ChannelHandle* channels),
    (comm, engine, channelDescs, channelNum, channels));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclChannelGetHcclBuffer,
    (HcclComm comm, ChannelHandle channel, void** buffer, uint64_t* size), (comm, channel, buffer, size));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclEngineCtxCreate, (HcclComm comm, const char* ctxTag, CommEngine engine, uint64_t size, void** ctx),
    (comm, ctxTag, engine, size, ctx));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclEngineCtxGet,
    (HcclComm comm, const char* ctxTag, CommEngine engine, void** ctx, uint64_t* size),
    (comm, ctxTag, engine, ctx, size));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclEngineCtxCopy,
    (HcclComm comm, CommEngine engine, const char* ctxTag, const void* srcCtx, uint64_t size,
        uint64_t dstCtxOffset),
    (comm, engine, ctxTag, srcCtx, size, dstCtxOffset));
DEFINE_DLSYM_FUNC(HcclResult, HcclEngineCtxDestroy, (HcclComm comm, const char* ctxTag, CommEngine engine),
    (comm, ctxTag, engine));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclCommMemReg, (HcclComm comm, const char* memTag, const CommMem* mem, HcclMemHandle* memHandle),
    (comm, memTag, mem, memHandle));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclChannelGetRemoteMems,
    (HcclComm comm, ChannelHandle channel, uint32_t* memNum, CommMem** remoteMems, char*** memTags),
    (comm, channel, memNum, remoteMems, memTags));
DEFINE_DLSYM_FUNC(HcclResult, HcclGetRemoteIpcHcclBuf,
    (HcclComm comm, uint64_t remoteRank, void** addr, uint64_t* size), (comm, remoteRank, addr, size));
DEFINE_DLSYM_FUNC(int32_t, HcclTaskRegister, (HcclComm comm, const char* msgTag, Callback cb), (comm, msgTag, cb));
DEFINE_DLSYM_FUNC(int32_t, HcclTaskUnRegister, (HcclComm comm, const char* msgTag), (comm, msgTag));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclDevMemAcquire, (HcclComm comm, const char* memTag, uint64_t* size, void** addr, bool* newCreated),
    (comm, memTag, size, addr, newCreated));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclThreadExportToCommEngine,
    (HcclComm comm, uint32_t threadNum, const ThreadHandle* threads, CommEngine dstCommEngine,
        ThreadHandle* exportedThreads),
    (comm, threadNum, threads, dstCommEngine, exportedThreads));

// 初始化
void HcclResDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcclGetHcclBuffer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadAcquireWithStream);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadResGetInfo);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelGetHcclBuffer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxCreate);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxGet);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxCopy);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclEngineCtxDestroy);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCommMemReg);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclChannelGetRemoteMems);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclGetRemoteIpcHcclBuf);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclTaskUnRegister);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclDevMemAcquire);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclThreadExportToCommEngine);
}
