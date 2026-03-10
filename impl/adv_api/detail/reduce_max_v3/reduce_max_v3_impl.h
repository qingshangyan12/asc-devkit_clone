/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You may not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * @file reduce_max_v3_impl.h
 * @brief ReduceMax V3 implementation supporting half, float, and int32_t data types.
 * 
 * This implementation:
 * - Reuses the existing vcgmax-based implementation for half and float types
 *   (from reduce_max_v220_impl.h)
 * - Provides new vmax-based implementation for int32_t type
 * 
 * Key differences:
 * - vcgmax: Single-input reduction instruction (block-level max)
 *   Used for: half, float
 * - vmax:   Dual-input comparison instruction (element-wise max)
 *   Used for: int32_t
 */

#ifndef IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_
#define IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_

// Include existing implementation for half/float (vcgmax-based)
#include "../reduce/reduce_max/reduce_max_v220_impl.h"
// Include int32_t implementation (vmax-based)
#include "reduce_max_int32_impl.h"

namespace AscendC {
namespace Internal {

// ============================================================================
// ReduceMaxV3Impl - Unified implementation for all supported types
// ============================================================================
// Supported types: half, float, int32_t
// Supported patterns: Pattern::Reduce::AR, Pattern::Reduce::RA
// 
// Type dispatch:
// - half/float: Reuse ReduceMaxImpl from reduce_max_v220_impl.h (vcgmax)
// - int32_t:    Use new vmax-based implementation
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxV3Impl(const LocalTensor<T>& dstTensor,
                                       const LocalTensor<T>& srcTensor,
                                       const LocalTensor<uint8_t>& sharedTmpBuffer,
                                       const uint32_t srcShape[],
                                       bool srcInnerPad)
{
    // Type check: only support half, float, int32_t
    static_assert(SupportType<T, half, float, int32_t>(),
        "ReduceMaxV3: unsupported data type! Only half, float, int32_t are supported.");
    
    // Pattern check: only support AR and RA
    static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(),
        "ReduceMaxV3: unsupported pattern! Only AR and RA patterns are supported.");

    if constexpr (IsSameType<T, int32_t>::value) {
        // int32_t: Use vmax-based implementation
        // Note: vcgmax does not support int32_t, so we use Max instruction instead
        ReduceMaxInt32Impl<pattern, isReuseSource>(
            dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
    } else {
        // half/float: reuse existing vcgmax implementation
        // Note: ReduceMaxImpl is defined in reduce_max_v220_impl.h
        ReduceMaxImpl<T, pattern, isReuseSource>(
            dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
    }
}

// ============================================================================
// GetReduceMaxV3TmpSize - Get temporary buffer size for ReduceMax V3
// ============================================================================
// Unified tmp size calculation for all supported types (half, float, int32_t)
// For int32_t: Uses vmax-based binary reduction (special calculation)
// For half/float: Uses vcgmax-based implementation (reuse existing)
template <class T>
inline void GetReduceMaxV3TmpSize(const uint32_t srcShape[2],
                                   bool srcInnerPad,
                                   bool isReuseSource,
                                   ReducePattern pattern,
                                   uint32_t& maxValue,
                                   uint32_t& minValue)
{
    if constexpr (IsSameType<T, int32_t>::value) {
        // int32_t: Use vmax-specific calculation
        GetReduceMaxInt32TmpSize(srcShape, srcInnerPad, isReuseSource, 
                                  pattern, maxValue, minValue);
    } else {
        // half/float: Reuse existing calculation logic
        constexpr uint32_t typeSize = sizeof(T);
        const uint32_t first = srcShape[0];
        const uint32_t last = srcShape[1];
        constexpr uint32_t elePerBlk = ONE_BLK_SIZE / typeSize;
        constexpr uint32_t elePerRep = ONE_REPEAT_BYTE_SIZE / typeSize;
        
        if (isReuseSource) {
            maxValue = minValue = 0U;
            return;
        }
        
        if (pattern == ReducePattern::AR) {
            if (last <= elePerBlk) {
                maxValue = minValue = 0U;
            } else if (last < elePerRep) {
                maxValue = minValue = first * elePerBlk * typeSize;
            } else {
                maxValue = minValue = first * elePerRep * typeSize;
            }
        } else {
            // RA mode
            uint32_t k = first;
            if (k > 0) {
                k |= (k >> 1);
                k |= (k >> 2);
                k |= (k >> 4);
                k |= (k >> 8);
                k |= (k >> 16);
                k = (k + 1) >> 1;
            }
            if (k > 1) {
                k >>= 1;
            }
            uint32_t padLast = (last + elePerBlk - 1) / elePerBlk * elePerBlk;
            maxValue = minValue = k * padLast * typeSize;
        }
    }
}

// ============================================================================
// ReduceMaxV3 - Public API (with sharedTmpBuffer)
// ============================================================================
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxV3(const LocalTensor<T>& dstTensor,
                                    const LocalTensor<T>& srcTensor,
                                    const LocalTensor<uint8_t>& sharedTmpBuffer,
                                    const uint32_t srcShape[],
                                    bool srcInnerPad = false)
{
    // Only for AI Vector Core
    if ASCEND_IS_AIC {
        return;
    }
    
    // Type check
    static_assert(SupportType<T, half, float, int32_t>(),
        "ReduceMaxV3: unsupported data type! Only half, float, int32_t are supported.");
    
    // Pattern check
    static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(),
        "ReduceMaxV3: unsupported pattern! Only AR and RA patterns are supported.");
    
    ReduceMaxV3Impl<T, pattern, isReuseSource>(
        dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

} // namespace Internal
} // namespace AscendC

#endif // IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_
