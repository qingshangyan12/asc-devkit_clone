/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

#pragma once

#include <cstdint>

#include "c_api/asc_simd.h"
#include "kernel_operator.h"

// 四个Case共用的工具函数，使各实现文件只保留SIMD、Scalar、SIMT的核心思路差异。
namespace pool3d_common {

// 返回两个值中的较小值，避免在设备代码中依赖主机侧标准库工具。
template <typename T>
__aicore__ inline T min(T a, T b)
{
    return a < b ? a : b;
}

// 等待MTE2的GM到UB搬运结果对标量代码可见。
__aicore__ inline void wait_mte2_to_scalar()
{
    asc_sync_notify(PIPE_MTE2, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_S, EVENT_ID0);
}

// 等待MTE2的GM到UB搬运结果对向量代码可见。
__aicore__ inline void wait_mte2_to_v()
{
    asc_sync_notify(PIPE_MTE2, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE2, PIPE_V, EVENT_ID0);
}

// 等待标量写入UB的结果对向量代码可见。
__aicore__ inline void wait_scalar_to_v()
{
    asc_sync_notify(PIPE_S, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_S, PIPE_V, EVENT_ID0);
}

// 等待向量写入UB的结果对MTE3存储代码可见。
__aicore__ inline void wait_v_to_mte3()
{
    asc_sync_notify(PIPE_V, PIPE_MTE3, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_MTE3, EVENT_ID0);
}

// 等待向量写入UB的结果对标量代码可见。
__aicore__ inline void wait_v_to_scalar()
{
    asc_sync_notify(PIPE_V, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_V, PIPE_S, EVENT_ID0);
}

// 等待MTE3的UB到GM存储完成，确保UB源缓冲区可继续被向量代码复用。
__aicore__ inline void wait_mte3_to_v()
{
    asc_sync_notify(PIPE_MTE3, PIPE_V, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_V, EVENT_ID0);
}

// 等待MTE3的UB到GM存储完成，确保标量代码可继续复用UB源缓冲区。
__aicore__ inline void wait_mte3_to_scalar()
{
    asc_sync_notify(PIPE_MTE3, PIPE_S, EVENT_ID0);
    asc_sync_wait(PIPE_MTE3, PIPE_S, EVENT_ID0);
}

} // namespace pool3d_common
