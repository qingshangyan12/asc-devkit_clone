/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef MC2_AICPU_PRIMITIVE_CACHE_H
#define MC2_AICPU_PRIMITIVE_CACHE_H

#include "hcomm_primitives_dl.h"
#include "mc2_aicpu_task_cache.h"

namespace mc2_ops_hccl {

HcclResult Mc2PrimitiveTaskCacheLookup(const Mc2AicpuTaskCachePlan& plan, bool& isHit);
HcclResult Mc2PrimitiveTaskCacheBeginCapture(const Mc2AicpuTaskCachePlan& plan);
void Mc2PrimitiveTaskCacheFinishCapture();
void Mc2PrimitiveTaskCacheAbortCapture();
HcclResult Mc2PrimitiveTaskCachePublish(const Mc2AicpuTaskCachePlan& plan);
HcclResult Mc2PrimitiveTaskCacheReplay(const Mc2AicpuTaskCachePlan& plan);
HcclResult Mc2PrimitiveTaskCacheClear(const Mc2AicpuTaskCachePlan& plan);
HcclResult Mc2PrimitiveTaskCacheEvict(void* comm);

// These facades preserve the Hcomm signatures used by copied algorithms. They record successful calls only while
// an asc-devkit primitive capture is active; otherwise they are direct pass-through calls.
int32_t Mc2CachedHcommLocalCopyOnThread(ThreadHandle thread, void* dst, const void* src, uint64_t len);
int32_t Mc2CachedHcommLocalReduceOnThread(
    ThreadHandle thread, void* dst, const void* src, uint64_t count, HcommDataType dataType, HcommReduceOp reduceOp);
int32_t Mc2CachedHcommThreadNotifyRecordOnThread(ThreadHandle thread, ThreadHandle dstThread, uint32_t dstNotifyIdx);
int32_t Mc2CachedHcommThreadNotifyWaitOnThread(ThreadHandle thread, uint32_t notifyIdx, uint32_t timeOut);
int32_t Mc2CachedHcommWriteOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len);
int32_t Mc2CachedHcommWriteReduceOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
    HcommReduceOp reduceOp);
int32_t Mc2CachedHcommReadOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t len);
int32_t Mc2CachedHcommReadReduceOnThread(
    ThreadHandle thread, ChannelHandle channel, void* dst, const void* src, uint64_t count, HcommDataType dataType,
    HcommReduceOp reduceOp);
int32_t Mc2CachedHcommChannelNotifyRecordOnThread(
    ThreadHandle thread, ChannelHandle channel, uint32_t remoteNotifyIdx);
int32_t Mc2CachedHcommChannelNotifyWaitOnThread(
    ThreadHandle thread, ChannelHandle channel, uint32_t localNotifyIdx, uint32_t timeOut);
int32_t Mc2CachedHcclHcommBatchTransferOnThread(
    ThreadHandle thread, ChannelHandle channel, const HcclHcommBatchTransferDesc* transferDescs,
    uint32_t transferDescNum);
int32_t Mc2CachedHcommBatchModeStart(const char* batchTag);
int32_t Mc2CachedHcommBatchModeEnd(const char* batchTag);

} // namespace mc2_ops_hccl

#endif // MC2_AICPU_PRIMITIVE_CACHE_H
