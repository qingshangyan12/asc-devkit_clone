/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/
#ifndef HCCL_API_DATA_AICPU_TS_H
#define HCCL_API_DATA_AICPU_TS_H

bool IsBatchLaunchMode();
HcclResult HandleDispatchAllStreams();


#ifdef __cplusplus
extern "C" {
#endif  // __cplusplus
/**
 * @brief NPU上查询 rtsq任务执行完成的接口（阻塞）
 * @param[in] thread NPU上执行的线程句柄
 * @param[in] timeout 超时时间(秒)
 * @return int32_t 执行结果状态码
 * 
 * WARNING: experimental API, No compatibility is currently guaranteed for this API
 */
extern int32_t HcommThreadJoin(ThreadHandle thread, uint32_t timeout);

/**
 * @brief 查询线程对应 RTSQ 的当前 SQ tail 位置
 * @param[in] thread NPU上执行的线程句柄
 * @param[out] tail 返回的 SQ tail 值
 * @return int32_t 执行结果状态码
 *
 * WARNING: experimental API, No compatibility is currently guaranteed for this API
 */
extern int32_t HcommQueryThreadSqTail(ThreadHandle thread, uint32_t *tail);

/**
 * @brief 等待线程对应 RTSQ 的 SQ head 追上指定的 targetTail 位置（阻塞）
 * @param[in] thread NPU上执行的线程句柄
 * @param[in] targetTail 需要等待 head 追上的目标 tail 值
 * @param[in] timeout 超时时间(秒)
 * @return int32_t 执行结果状态码
 *
 * WARNING: experimental API, No compatibility is currently guaranteed for this API
 */
extern int32_t HcommWaitThreadSqHeadReach(ThreadHandle thread, uint32_t targetTail, uint32_t timeout);
#ifdef __cplusplus
}
#endif  // __cplusplus

#endif // HCCL_API_DATA_AICPU_TS_H
