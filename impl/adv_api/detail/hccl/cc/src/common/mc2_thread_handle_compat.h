/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef ASCENDC_MC2_THREAD_HANDLE_COMPAT_H
#define ASCENDC_MC2_THREAD_HANDLE_COMPAT_H

#include "hccl_types.h"
#include "hcomm_res_defs.h"

#include <cstddef>
#include <cstdint>

namespace mc2_thread_compat {
constexpr uint32_t THREAD_ENTITY_VERSION = 0U;
constexpr uint32_t THREAD_ENTITY_MAGIC_WORD = 0x0fcf0f6fU;

// MC2 过渡兼容视图，对应 HCOMM pkg_inc/hcomm/hcomm_res_entity_defs.h 中的 HcommThreadEntity v0。
// 资源字段仅占位，继续复用 HCOMM 数据面及其 SQ 状态。
struct ThreadEntityV0 {
    CommAbiHeader abiHeader;
    uint8_t resourceFields[64]; // HcommDevInfo、notify 数量及填充、notify 指针、SQ 上下文指针。
    ThreadHandle compatThreadHandle;
    uint8_t reserved[168];
};

static_assert(sizeof(ThreadHandle) == 8, "MC2 requires 64-bit thread handles");
static_assert(sizeof(CommAbiHeader) == 16, "MC2 thread entity ABI header mismatch");
static_assert(offsetof(ThreadEntityV0, compatThreadHandle) == 80, "MC2 thread entity compatibility offset mismatch");
static_assert(sizeof(ThreadEntityV0) == 256, "MC2 thread entity size mismatch");

// 仅在 device 侧解包；原始句柄保留给 HCOMM 接口、缓存和序列化。
// handle 必须指向存活的设备资源；本函数无法校验任意地址的可访问性。
inline HcclResult UnwrapThreadHandle(ThreadHandle handle, ThreadHandle& rawHandle)
{
    rawHandle = 0;
    if (handle == 0) {
        return HCCL_E_PTR;
    }
    const auto* bytes = reinterpret_cast<const uint8_t*>(handle);
    uint32_t magicWord = 0;
    // 先读取 magic 所在的 4 字节，避免把旧 Thread 当作完整 Entity 头读取。
    if (memcpy_s(&magicWord, sizeof(magicWord), bytes + offsetof(CommAbiHeader, magicWord), sizeof(magicWord)) != EOK) {
        return HCCL_E_INTERNAL;
    }
    if (magicWord != THREAD_ENTITY_MAGIC_WORD) {
        rawHandle = handle;
        return HCCL_SUCCESS;
    }

    CommAbiHeader header{};
    if (memcpy_s(&header, sizeof(header), bytes, sizeof(header)) != EOK) {
        return HCCL_E_INTERNAL;
    }
    if (header.version != THREAD_ENTITY_VERSION || header.size < sizeof(ThreadEntityV0)) {
        return HCCL_E_PARA;
    }
    ThreadHandle compatHandle = 0;
    if (memcpy_s(
            &compatHandle, sizeof(compatHandle), bytes + offsetof(ThreadEntityV0, compatThreadHandle),
            sizeof(compatHandle)) != EOK) {
        return HCCL_E_INTERNAL;
    }
    if (compatHandle == 0 || compatHandle == handle) {
        return HCCL_E_INTERNAL;
    }
    rawHandle = compatHandle;
    return HCCL_SUCCESS;
}
} // namespace mc2_thread_compat

#endif // ASCENDC_MC2_THREAD_HANDLE_COMPAT_H
