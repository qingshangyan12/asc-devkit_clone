/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/**
 * @file utils.h
 * @brief Utility types and enums for TensorAPI.
 * 
 * @details
 * Defines cache modes, mmad traits, and common types.
 */
#ifndef INCLUDE_TENSOR_API_UTILS_UTILS_H
#define INCLUDE_TENSOR_API_UTILS_UTILS_H

#include "include/utils/base/sys_macros.h"
#include "include/utils/base/sys_constants.h"
#include "include/utils/common_types.h"

namespace AscendC {
namespace Te {

/** Cache mode for memory access. */
enum class CacheMode : uint8_t {
    CACHE_MODE_NORMAL = 0,
    CACHE_MODE_DISABLE = 4,
    CACHE_MODE_LAST,
    CACHE_MODE_PERSISTENT
};

/** Mmad operation type. */
enum class MmadType : uint8_t { NORMAL = 0, MX = 1};

/** Traits for Mmad operation. */
struct MmadTrait {
    int32_t fmOffset = 0;
    bool kDirectionAlign = false;
    bool cmatrixSource = false;
    bool disableGemv = true;
    MmadType mmadType = MmadType::NORMAL;
};

/** Parameters for Mmad operation. */
struct MmadParams {
    uint16_t m = 0;
    uint16_t n = 0;
    uint16_t k = 0;
    uint8_t unitFlag = 0;
    bool cmatrixInitVal = false;
};

/** Traits for data copy operation. */
struct DataCopyTrait {};

/** Round mode for fixpipe. */
enum class RoundMode : uint8_t {DEFAULT = 0, HYBRID};

/** Dual destination mode. */
enum DualDstMode : uint8_t {
    DUAL_DST_DISABLE = 0,
    DUAL_DST_SPLIT_M,
    DUAL_DST_SPLIT_N
};

/** Traits for fixpipe operation. */
struct FixpipeTrait {
    RoundMode roundMode = RoundMode::DEFAULT;
    bool enableRelu = false;
    bool enableChannelSplit = false;
    DualDstMode dualDstCtl = DUAL_DST_DISABLE;
};

/** Parameters for fixpipe operation. */
struct FixpipeParams {
    uint8_t unitFlag = 0;
};

/** Traits for load data operation. */
struct LoadDataTrait {
    bool transposed = false;
};

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_UTILS_UTILS_H