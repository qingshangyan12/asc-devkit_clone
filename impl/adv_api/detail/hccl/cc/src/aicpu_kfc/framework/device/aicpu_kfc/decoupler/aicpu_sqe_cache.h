/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASC_COMM_AICPU_SQE_CACHE_H
#define ASC_COMM_AICPU_SQE_CACHE_H

#include <cstdint>

typedef enum {
    HCOMM_AICPU_CACHE_RANGE_INPUT = 0,
    HCOMM_AICPU_CACHE_RANGE_OUTPUT = 1,
    HCOMM_AICPU_CACHE_RANGE_CCL = 2,
    HCOMM_AICPU_CACHE_RANGE_REMOTE_CCL = 3,
    HCOMM_AICPU_CACHE_RANGE_REMOTE_INPUT = 4,
    HCOMM_AICPU_CACHE_RANGE_REMOTE_OUTPUT = 5
} HcommAicpuSqeCacheRangeKind;

typedef struct {
    uint64_t addr;
    uint64_t size;
    uint32_t kind;
    uint32_t index;
} HcommAicpuSqeCacheRange;

typedef struct {
    const char *algName;
    uint32_t opType;
    uint32_t modeFlags;
    uint64_t resourceId;
    uint64_t dataBytes;
    uint64_t inputUnitBytes;
    uint64_t outputUnitBytes;
    uint32_t notifyWaitTimeout;
    uint32_t rankSize;
    const HcommAicpuSqeCacheRange *ranges;
    uint32_t rangeCount;
    const uint64_t *sendCountsBytes;
    const uint64_t *recvCountsBytes;
    const uint64_t *sendDisplsBytes;
    const uint64_t *recvDisplsBytes;
} HcommAicpuSqeCacheParam;

extern "C" {
int32_t HcommAicpuSqeCacheReplay(const HcommAicpuSqeCacheParam *param, uint32_t *replayed);
int32_t HcommAicpuSqeCacheBegin(const HcommAicpuSqeCacheParam *param);
int32_t HcommAicpuSqeCacheBindThread(uint64_t thread);
int32_t HcommAicpuSqeCacheFreezeAndUnbind(void);
int32_t HcommAicpuSqeCacheCommit(void);
void HcommAicpuSqeCacheAbort(void);
}

#endif // ASC_COMM_AICPU_SQE_CACHE_H
