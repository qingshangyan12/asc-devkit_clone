/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */
#ifndef HCCL_LOOP_SYNC_POLICY_H
#define HCCL_LOOP_SYNC_POLICY_H
#include <cstddef>
#include <cstdint>

namespace HcclApi {
// Byte encoding shared by tiling reserved space and the private OpParam trailer.
// Never reinterpret the potentially unaligned storage as a C++ structure.
constexpr size_t LOOP_SYNC_EXTENSION_SIZE = 8;
inline void EncodeLoopSyncPolicy(uint8_t* bytes, bool enabled)
{
    bytes[0] = 'L';
    bytes[1] = 'S';
    bytes[2] = 'Y';
    bytes[3] = 'N';
    bytes[4] = 1;
    bytes[5] = LOOP_SYNC_EXTENSION_SIZE;
    bytes[6] = enabled ? 1 : 0;
    bytes[7] = 0;
}

inline bool DecodeLoopSyncPolicy(const uint8_t* bytes, size_t size, bool& enabled)
{
    enabled = false;
    if (size < LOOP_SYNC_EXTENSION_SIZE) {
        return false;
    }
    if (bytes[0] != 'L' || bytes[1] != 'S' || bytes[2] != 'Y' || bytes[3] != 'N') {
        // Legacy tiling has no extension. Unknown reserved bytes do not opt in.
        return true;
    }
    if (bytes[4] != 1 || bytes[5] != LOOP_SYNC_EXTENSION_SIZE || bytes[6] > 1 || bytes[7] != 0) {
        return false;
    }
    enabled = bytes[6] != 0;
    return true;
}
} // namespace HcclApi
#endif
