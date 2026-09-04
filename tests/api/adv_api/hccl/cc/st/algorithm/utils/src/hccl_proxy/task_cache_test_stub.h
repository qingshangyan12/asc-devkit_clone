/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef TASK_CACHE_TEST_STUB_H
#define TASK_CACHE_TEST_STUB_H

#include <string>
#include <vector>

#include "hccl_types.h"

struct Mc2TaskCacheTestStubState {
    HcclResult batchStartRet = HCCL_SUCCESS;
    HcclResult batchEndRet = HCCL_SUCCESS;
    HcclResult lookupRet = HCCL_SUCCESS;
    HcclResult cacheStartRet = HCCL_SUCCESS;
    HcclResult cacheEndRet = HCCL_SUCCESS;
    HcclResult executeRet = HCCL_SUCCESS;
    HcclResult clearRet = HCCL_SUCCESS;
    HcclResult notifyWaitRet = HCCL_SUCCESS;
    HcclResult notifyRecordRet = HCCL_SUCCESS;
    HcclResult threadAcquireRet = HCCL_SUCCESS;
    bool lookupHit = false;
    bool interceptNotifyCalls = false;
    bool emulatePooledThreads = false;
    bool returnNullThread = false;
    bool returnDuplicateThread = false;
    bool aliasUnfoldThread = false;
    uint32_t batchStartCalls = 0U;
    uint32_t batchEndCalls = 0U;
    uint32_t lookupCalls = 0U;
    uint32_t cacheStartCalls = 0U;
    uint32_t cacheEndCalls = 0U;
    uint32_t executeCalls = 0U;
    uint32_t clearCalls = 0U;
    uint32_t notifyWaitCalls = 0U;
    uint32_t notifyRecordCalls = 0U;
    uint32_t aicpuThreadAcquireCalls = 0U;
    uint32_t lastThreadNum = 0U;
    uint32_t lastThreadNotifyNum = 0U;
    ThreadHandle waitThread = 0;
    ThreadHandle recordThread = 0;
    ThreadHandle recordDstThread = 0;
    uint32_t waitNotifyIdx = 0U;
    uint32_t recordNotifyIdx = 0U;
    std::vector<ThreadHandle> pooledThreads;
    std::vector<std::string> events;
};

Mc2TaskCacheTestStubState& GetMc2TaskCacheTestStubState();
void ResetMc2TaskCacheTestStubState();

#endif // TASK_CACHE_TEST_STUB_H
