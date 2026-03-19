/**
* Copyright (c) 2025 Huawei Technologies Co., Ltd.
* This program is free software, you can redistribute it and/or modify it under the terms and conditions of
* CANN Open Software License Agreement Version 2.0 (the "License").
* Please refer to the License for details. You can not use this file except in compliance with the License.
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED,
* INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
* See LICENSE in the root of the software repository for the full text of the License.
*/

/**
 * @file reduce_max_v3_impl.h
 * @brief ReduceMaxExtend implementation for int32_t data type.
 * 
 * This implementation uses vmax-based binary reduction for int32_t type,
 * since vcgmax does not support int32_t.
 * 
 * Key points:
 * - vmax: Dual-input comparison instruction (element-wise max)
 * - Uses Brcb for small data expansion and binary reduction strategy
 */

#ifndef IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_
#define IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_

// Include int32_t implementation (vmax-based)
#include "reduce_max_int32_impl.h"

namespace AscendC {
namespace Internal {

// ============================================================================
// ReduceMaxExtendImpl - Implementation for int32_t type
// ============================================================================
// Supported types: int32_t
// Supported patterns: Pattern::Reduce::AR, Pattern::Reduce::RA
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxExtendImpl(const LocalTensor<T>& dstTensor,
                                           const LocalTensor<T>& srcTensor,
                                           const LocalTensor<uint8_t>& sharedTmpBuffer,
                                           const uint32_t srcShape[],
                                           bool srcInnerPad)
{
    // Type check: only support int32_t
    static_assert(IsSameType<T, int32_t>::value,
        "ReduceMaxExtend: unsupported data type! Only int32_t is supported.");
    
    // Pattern check: only support AR and RA
    static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(),
        "ReduceMaxExtend: unsupported pattern! Only AR and RA patterns are supported.");

    // Use vmax-based implementation for int32_t
    ReduceMaxInt32Impl<pattern, isReuseSource>(
        dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

// ============================================================================
// GetReduceMaxExtendTmpSize - Get temporary buffer size for ReduceMaxExtend
// ============================================================================
// Tmp size calculation for int32_t type
template <class T>
inline void GetReduceMaxExtendTmpSize(const uint32_t srcShape[2],
                                      bool srcInnerPad,
                                      bool isReuseSource,
                                      ReducePattern pattern,
                                      uint32_t& maxValue,
                                      uint32_t& minValue)
{
    static_assert(IsSameType<T, int32_t>::value,
        "GetReduceMaxExtendTmpSize: unsupported data type! Only int32_t is supported.");
    
    GetReduceMaxInt32TmpSize(srcShape, srcInnerPad, isReuseSource, 
                             pattern, maxValue, minValue);
}

// ============================================================================
// ReduceMaxExtend - Public API (with sharedTmpBuffer)
// ============================================================================
template <class T, class pattern, bool isReuseSource = false>
__aicore__ inline void ReduceMaxExtend(const LocalTensor<T>& dstTensor,
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
    static_assert(IsSameType<T, int32_t>::value,
        "ReduceMaxExtend: unsupported data type! Only int32_t is supported.");
    
    // Pattern check
    static_assert(SupportType<pattern, Pattern::Reduce::AR, Pattern::Reduce::RA>(),
        "ReduceMaxExtend: unsupported pattern! Only AR and RA patterns are supported.");
    
    ReduceMaxExtendImpl<T, pattern, isReuseSource>(
        dstTensor, srcTensor, sharedTmpBuffer, srcShape, srcInnerPad);
}

} // namespace Internal
} // namespace AscendC

#endif // IMPL_REDUCE_MAX_V3_REDUCE_MAX_V3_IMPL_H_
