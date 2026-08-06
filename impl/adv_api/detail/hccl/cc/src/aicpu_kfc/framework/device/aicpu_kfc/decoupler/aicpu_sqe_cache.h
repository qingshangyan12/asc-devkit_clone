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
/**
 * @brief 按当前算子参数查找并回放兼容的 AICPU SQE 缓存
 * @param[in] param 当前算子的缓存匹配参数
 * @param[out] replayed 是否命中并完成回放，非 0 表示已回放
 * @return int32_t 执行结果状态码
 */
int32_t HcommAicpuSqeCacheReplay(const HcommAicpuSqeCacheParam *param, uint32_t *replayed);

/**
 * @brief 创建本次 AICPU SQE 捕获上下文
 * @param[in] param 当前算子的缓存绑定参数
 * @return int32_t 执行结果状态码
 */
int32_t HcommAicpuSqeCacheBegin(const HcommAicpuSqeCacheParam *param);

/**
 * @brief 将 MC2 使用的 Thread 绑定到当前 SQE 捕获上下文
 * @param[in] thread AICPU TS Thread 句柄
 * @return int32_t 执行结果状态码
 */
int32_t HcommAicpuSqeCacheBindThread(uint64_t thread);

/**
 * @brief 冻结当前捕获结果并解除全部 Thread/RTSQ 绑定
 * @return int32_t 执行结果状态码
 */
int32_t HcommAicpuSqeCacheFreezeAndUnbind(void);

/**
 * @brief 提交当前捕获结果并发布为可回放缓存
 * @return int32_t 执行结果状态码
 */
int32_t HcommAicpuSqeCacheCommit(void);

/**
 * @brief 终止当前捕获并清理全部 Thread/RTSQ 绑定
 */
void HcommAicpuSqeCacheAbort(void);
}

#endif // ASC_COMM_AICPU_SQE_CACHE_H
