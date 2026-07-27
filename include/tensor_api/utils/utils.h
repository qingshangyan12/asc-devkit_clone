/**
 * Copyright (c) 2026 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file utils.h
 * \brief
 */
#ifndef INCLUDE_TENSOR_API_UTILS_UTILS_H
#define INCLUDE_TENSOR_API_UTILS_UTILS_H

#include "utils/base/sys_macros.h"
#include "utils/base/sys_constants.h"
#include "utils/common_types.h"

namespace AscendC {
namespace Te {

/**
 * @brief L2 cache hint applied to Tensor API memory accesses.
 */
enum class CacheMode : uint8_t {
    /**
     * @brief Use the normal L2 cache policy.
     */
    CACHE_MODE_NORMAL = 0,
    /**
     * @brief Disable L2 caching for the memory access.
     */
    CACHE_MODE_DISABLE = 4,
    /**
     * @brief Mark the memory access as the last cache access.
     */
    CACHE_MODE_LAST,
    /**
     * @brief Request the persistent L2 cache policy.
     */
    CACHE_MODE_PERSISTENT
};

/**
 * @brief Matrix multiply-accumulate calculation mode. NORMAL selects the standard mode and MX selects the MX mode.
 */
enum class MmadType : uint8_t { NORMAL = 0, MX = 1 };

/**
 * @brief Static configuration for a matrix multiply-accumulate operation.
 */
struct MmadTrait {
    /**
     * @brief Construct a matrix multiply-accumulate trait with default values.
     */
    __aicore__ constexpr MmadTrait(){};

    /**
     * @brief Construct a matrix multiply-accumulate trait with the specified configuration.
     * @param fmOffsetIn : Feature-map offset retained for compatibility.
     * @param kDirectionAlignIn : K-direction alignment setting retained for compatibility.
     * @param cmatrixSourceIn : Whether to initialize the result matrix from Bias Table Buffer.
     * @param disableGemvIn : Whether to disable GEMV mode when M is 1.
     * @param mmadTypeIn : Matrix multiply-accumulate calculation mode.
     */
    __aicore__ constexpr MmadTrait(
        int32_t fmOffsetIn, bool kDirectionAlignIn, bool cmatrixSourceIn, bool disableGemvIn, MmadType mmadTypeIn)
    {
        fmOffset = fmOffsetIn;
        kDirectionAlign = kDirectionAlignIn;
        cmatrixSource = cmatrixSourceIn;
        disableGemv = disableGemvIn;
        mmadType = mmadTypeIn;
    };

    /**
     * @brief Feature-map offset retained for compatibility.
     */
    int32_t fmOffset = 0;
    /**
     * @brief K-direction alignment setting retained for compatibility.
     */
    bool kDirectionAlign = false;
    /**
     * @brief Whether to initialize the result matrix from Bias Table Buffer.
     */
    bool cmatrixSource = false;
    /**
     * @brief Whether to disable GEMV mode when M is 1.
     */
    bool disableGemv = true;
    /**
     * @brief Matrix multiply-accumulate calculation mode.
     */
    MmadType mmadType = MmadType::NORMAL;
};

/**
 * @brief Runtime parameters for a matrix multiply-accumulate operation.
 */
struct MmadParams {
    /**
     * @brief Construct matrix multiply-accumulate parameters with default values.
     */
    __aicore__ constexpr MmadParams(){};

    /**
     * @brief Construct matrix multiply-accumulate parameters with the specified matrix dimensions and controls.
     * @param mIn : Height of the left and result matrices.
     * @param nIn : Width of the right and result matrices.
     * @param kIn : Shared dimension of the left and right matrices.
     * @param unitFlagIn : Unit flag used for fine-grained synchronization with result copies.
     * @param cmatrixInitValIn : Whether to initialize the result matrix before accumulation.
     */
    __aicore__ constexpr MmadParams(uint16_t mIn, uint16_t nIn, uint16_t kIn, uint8_t unitFlagIn, bool cmatrixInitValIn)
        : m(mIn), n(nIn), k(kIn), unitFlag(unitFlagIn), cmatrixInitVal(cmatrixInitValIn){};

    /**
     * @brief Height of the left and result matrices.
     */
    uint16_t m = 0;
    /**
     * @brief Width of the right and result matrices.
     */
    uint16_t n = 0;
    /**
     * @brief Shared dimension of the left and right matrices.
     */
    uint16_t k = 0;
    /**
     * @brief Unit flag used for fine-grained synchronization with result copies.
     */
    uint8_t unitFlag = 0;
    /**
     * @brief Whether to initialize the result matrix before accumulation.
     */
    bool cmatrixInitVal = false;
};

/**
 * @brief Rounding mode used during matrix result conversion. HYBRID enables hybrid rounding where supported.
 */
enum class RoundMode : uint8_t { DEFAULT = 0, HYBRID };

/**
 * @brief Dual-destination mode for disabling output splitting or splitting it along the M or N dimension.
 */
enum DualDstMode : uint8_t { DUAL_DST_DISABLE = 0, DUAL_DST_SPLIT_M, DUAL_DST_SPLIT_N };

/**
 * @brief Runtime parameters for copying matrix results from L0C Buffer.
 */
struct FixpipeParams {
    /**
     * @brief Construct result-copy parameters with default values.
     */
    __aicore__ constexpr FixpipeParams(){};

    /**
     * @brief Construct result-copy parameters with synchronization and destination controls.
     * @param unitFlagIn : Unit flag used for fine-grained synchronization with matrix calculation.
     * @param subBlockIdIn : Destination Unified Buffer identifier in single-destination mode.
     */
    __aicore__ constexpr FixpipeParams(uint8_t unitFlagIn, bool subBlockIdIn = false)
        : unitFlag(unitFlagIn), subBlockId(subBlockIdIn){};

    /**
     * @brief Unit flag used for fine-grained synchronization with matrix calculation.
     */
    uint8_t unitFlag = 0;

    /**
     * @brief Destination Unified Buffer identifier in single-destination mode.
     */
    bool subBlockId = false;
};

} // namespace Te
} // namespace AscendC

#endif // INCLUDE_TENSOR_API_UTILS_UTILS_H
