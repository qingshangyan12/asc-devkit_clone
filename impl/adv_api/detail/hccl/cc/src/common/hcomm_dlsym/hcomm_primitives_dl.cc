/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#include "hcomm_primitives_dl.h"
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>

DEFINE_DLSYM_FUNC(
    int32_t, HcommLocalCopyOnThread, (ThreadHandle thread, void* dst, const void* src, uint64_t len),
    (thread, dst, src, len));
DEFINE_DLSYM_FUNC(
    int32_t, HcommLocalReduceOnThread,
    (ThreadHandle thread, void* dst, const void* src, uint64_t count, HcommDataType dataType, HcommReduceOp reduceOp),
    (thread, dst, src, count, dataType, reduceOp));
DEFINE_DLSYM_FUNC(
    int32_t, HcommThreadNotifyRecordOnThread, (ThreadHandle thread, ThreadHandle dstThread, uint32_t dstNotifyIdx),
    (thread, dstThread, dstNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommThreadNotifyWaitOnThread, (ThreadHandle thread, uint32_t notifyIdx, uint32_t timeOut),
    (thread, notifyIdx, timeOut));
DEFINE_DLSYM_FUNC(
    int32_t, HcommAclrtNotifyRecordOnThread, (ThreadHandle thread, uint64_t dstNotifyId), (thread, dstNotifyId));
DEFINE_DLSYM_FUNC(
    int32_t, HcommAclrtNotifyWaitOnThread, (ThreadHandle thread, uint64_t notifyId, uint32_t timeOut),
    (thread, notifyId, timeOut));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteOnThread, (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (thread, channel, dst, src, len));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteReduceOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
        HcommReduceOp reduceOp),
    (thread, channel, dst, src, count, dataType, reduceOp));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteWithNotifyOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len, uint32_t remoteNotifyIdx),
    (thread, channel, dst, src, len, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteReduceWithNotifyOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
        HcommReduceOp reduceOp, uint32_t remoteNotifyIdx),
    (thread, channel, dst, src, count, dataType, reduceOp, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommReadOnThread, (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (thread, channel, dst, src, len));
DEFINE_DLSYM_FUNC(
    int32_t, HcommReadReduceOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
        HcommReduceOp reduceOp),
    (thread, channel, dst, src, count, dataType, reduceOp));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteNbiOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (thread, channel, dst, src, len));
DEFINE_DLSYM_FUNC(int32_t, HcommWriteNbi, (ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (channel, dst, src, len));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteWithNotifyNbiOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len, uint32_t remoteNotifyIdx),
    (thread, channel, dst, src, len, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommWriteWithNotifyNbi,
    (ChannelHandle channel, void* dst, const void* src, uint64_t len, uint32_t remoteNotifyIdx),
    (channel, dst, src, len, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommReadNbiOnThread,
    (ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (thread, channel, dst, src, len));
DEFINE_DLSYM_FUNC(int32_t, HcommReadNbi, (ChannelHandle channel, void* dst, const void* src, uint64_t len),
    (channel, dst, src, len));
DEFINE_DLSYM_FUNC(
    int32_t, HcommChannelNotifyRecordOnThread,
    (ThreadHandle thread, ChannelHandle channel, uint32_t remoteNotifyIdx), (thread, channel, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(int32_t, HcommChannelNotifyRecord, (ChannelHandle channel, uint32_t remoteNotifyIdx),
    (channel, remoteNotifyIdx));
DEFINE_DLSYM_FUNC(
    int32_t, HcommChannelNotifyWaitOnThread,
    (ThreadHandle thread, ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeout),
    (thread, channel, localNotifyIdx, timeout));
DEFINE_DLSYM_FUNC(int32_t, HcommChannelNotifyWait,
    (ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeout), (channel, localNotifyIdx, timeout));
DEFINE_DLSYM_FUNC(int32_t, HcommBatchModeStart, (const char* batchTag), (batchTag));
DEFINE_DLSYM_FUNC(int32_t, HcommBatchModeEnd, (const char* batchTag), (batchTag));
DEFINE_DLSYM_FUNC(int32_t, HcommAcquireComm, (const char* commId), (commId));
DEFINE_DLSYM_FUNC(int32_t, HcommReleaseComm, (const char* commId), (commId));
DEFINE_DLSYM_FUNC(
    HcclResult, HcommSymWinGetPeerPointer,
    (HcclCommSymWindow winHandle, size_t offset, uint32_t peerRank, void** ptr), (winHandle, offset, peerRank, ptr));
DEFINE_DLSYM_FUNC(int32_t, HcommThreadSynchronize, (ThreadHandle thread), (thread));
DEFINE_DLSYM_FUNC(
    int32_t, HcommSendRequest, (uint64_t handle, const char* msgTag, const void* src, size_t sizeByte, uint32_t* msgId),
    (handle, msgTag, src, sizeByte, msgId));
DEFINE_DLSYM_FUNC(int32_t, HcommWaitResponse, (uint64_t handle, void* dst, size_t sizeByte, uint32_t* msgId),
    (handle, dst, sizeByte, msgId));
DEFINE_DLSYM_FUNC(int32_t, HcommFlush, (void), ());
DEFINE_DLSYM_FUNC(int32_t, HcommChannelFence, (ChannelHandle channel), (channel));
DEFINE_DLSYM_FUNC(int32_t, HcommFenceOnThread, (ThreadHandle thread), (thread));
DEFINE_DLSYM_FUNC(int32_t, HcommChannelFenceOnThread, (ThreadHandle thread, ChannelHandle channel),
    (thread, channel));
DEFINE_DLSYM_FUNC(HcclResult, HcommThreadJoin, (ThreadHandle thread, uint32_t timeout), (thread, timeout));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclSymWinGetPeerPointer,
    (HcclCommSymWindow winHandle, size_t offset, uint32_t peerRank, void** ptr), (winHandle, offset, peerRank, ptr));
DEFINE_DLSYM_FUNC(
    HcclResult, HcclCommSymWinGet,
    (HcclComm comm, void* ptr, size_t size, HcclCommSymWindow* winHandle, size_t* offset),
    (comm, ptr, size, winHandle, offset));

// ---------- 初始化函数 ----------
void HcommPrimitivesDlInit(void* libHcommHandle)
{
    INIT_SUPPORT_FLAG(libHcommHandle, HcommLocalCopyOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommLocalReduceOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommThreadNotifyRecordOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommThreadNotifyWaitOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommAclrtNotifyRecordOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommAclrtNotifyWaitOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteReduceOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteWithNotifyOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteReduceWithNotifyOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommReadOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommReadReduceOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteNbiOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteNbi);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteWithNotifyNbiOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWriteWithNotifyNbi);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommReadNbiOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommReadNbi);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelNotifyRecordOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelNotifyRecord);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelNotifyWaitOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelNotifyWait);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommBatchModeStart);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommBatchModeEnd);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommAcquireComm);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommReleaseComm);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommSymWinGetPeerPointer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommThreadSynchronize);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommSendRequest);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommWaitResponse);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommFlush);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelFence);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommFenceOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommChannelFenceOnThread);
    INIT_SUPPORT_FLAG(libHcommHandle, HcommThreadJoin);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclSymWinGetPeerPointer);
    INIT_SUPPORT_FLAG(libHcommHandle, HcclCommSymWinGet);
}
