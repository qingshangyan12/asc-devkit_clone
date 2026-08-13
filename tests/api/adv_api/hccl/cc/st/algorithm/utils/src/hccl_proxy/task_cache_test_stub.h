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

#include "hccl_types.h"
#include <string>
#include <vector>

struct Mc2TaskCacheTestStubState {
    HcclResult batchStartRet = HCCL_SUCCESS;
    HcclResult batchEndRet = HCCL_SUCCESS;
    HcclResult lookupRet = HCCL_SUCCESS;
    HcclResult cacheStartRet = HCCL_SUCCESS;
    HcclResult cacheEndRet = HCCL_SUCCESS;
    HcclResult executeRet = HCCL_SUCCESS;
    HcclResult clearRet = HCCL_SUCCESS;
    bool lookupHit = false;
    uint32_t batchStartCalls = 0U;
    uint32_t batchEndCalls = 0U;
    uint32_t lookupCalls = 0U;
    uint32_t cacheStartCalls = 0U;
    uint32_t cacheEndCalls = 0U;
    uint32_t executeCalls = 0U;
    uint32_t clearCalls = 0U;
};

Mc2TaskCacheTestStubState& GetMc2TaskCacheTestStubState();
void ResetMc2TaskCacheTestStubState();
void RecordAicpuLaunchAlgName(const char* algName);
std::vector<std::string> GetAicpuLaunchAlgNames();
void ResetAicpuLaunchAlgNames();
uint32_t GetOpenExecutorLaunchCount();
void ResetOpenExecutorLaunchCount();
void RecordOpenExecutorLaunch();

#endif // TASK_CACHE_TEST_STUB_H
