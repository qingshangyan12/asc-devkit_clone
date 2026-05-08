/**
 * Copyright (c) 2025 Huawei Technologies Co., Ltd.
 * This program is free software, you can redistribute it and/or modify it under the terms and conditions of
 * CANN Open Software License Agreement Version 2.0 (the "License").
 * Please refer to the License for details. You may not use this file except in compliance with the License.
 * THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 * INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT, MERCHANTABILITY, OR FITNESS FOR A PARTICULAR PURPOSE.
 * See LICENSE in the root of the software repository for the full text of the License.
 */

/*!
 * \file log1p_common_impl.h
 * \brief Common implementation layer for log1p function
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/math/log1p/log1p_common_impl.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/log1p.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_COMMON_IMPL_H__
#endif
#ifndef IMPL_MATH_LOG1P_LOG1P_COMMON_IMPL_H
#define IMPL_MATH_LOG1P_LOG1P_COMMON_IMPL_H

#include "kernel_tensor.h"
#include "kernel_basic_intf.h"
#include "kernel_pop_stack_buffer.h"
#include "kernel_tiling/kernel_tiling.h"
#include "../../common/check.h"
#include "../../common/common.h"

#ifdef ASCENDC_CPU_DEBUG
#include "../../api_check/kernel_check/math/log1p/log1p_check.h"
#endif // ASCENDC_CPU_DEBUG

#include "../../api_check/kernel_api_check.h"

#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "log1p_3510_impl.h"
#endif

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                               __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)

namespace AscendC {

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImplAPI(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, uint32_t calCount)
{
    CHECK_FUNC_HIGHLEVEL_API(Log1p, (T, isReuseSource), (dstTensor, srcTensor, calCount));

#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
#else
    const UnaryRepeatParams unaryParams;
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, calCount);
    
    Adds<T, false>(dstTensor, srcTensor, static_cast<T>(1.0), MASK_PLACEHOLDER, 1, unaryParams);
    PipeBarrier<PIPE_V>();
    Ln<T, false>(dstTensor, dstTensor, MASK_PLACEHOLDER, 1, unaryParams);
    
    SetMaskNorm();
    SetVectorMask<half, MaskMode::NORMAL>(FULL_MASK, FULL_MASK);
#endif
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1pImplAPI(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, 
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    CHECK_FUNC_HIGHLEVEL_API(Log1p, (T, isReuseSource), (dstTensor, srcTensor, sharedTmpBuffer, calCount));
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
#else
    SetMaskCount();
    SetVectorMask<T, MaskMode::COUNTER>(0, calCount);
    
    if constexpr (std::is_same_v<T, half>) {
        uint32_t tmpBufferSize = sharedTmpBuffer.GetSize();
        uint32_t splitSize = tmpBufferSize / sizeof(float) / ONE_BLK_SIZE * ONE_BLK_SIZE;
        CheckTmpBufferSize(splitSize, 0, tmpBufferSize);
        uint32_t loopCount = calCount / splitSize;
        uint32_t calcTail = calCount % splitSize;
        
        LocalTensor<float> tmpTensor = sharedTmpBuffer.ReinterpretCast<float>();
        const UnaryRepeatParams unaryParams;
        
        for (uint32_t i = 0; i < loopCount; ++i) {
            SetVectorMask<T, MaskMode::COUNTER>(0, splitSize);
            Cast<float, half, false>(tmpTensor, srcTensor[i * splitSize], 
                                     RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, 
                                     {1, 1, DEFAULT_REPEAT_STRIDE, HALF_DEFAULT_REPEAT_STRIDE});
            PipeBarrier<PIPE_V>();
            
            Adds<float, false>(tmpTensor, tmpTensor, 1.0, MASK_PLACEHOLDER, 1, unaryParams);
            PipeBarrier<PIPE_V>();
            
            Ln<float, false>(tmpTensor, tmpTensor, MASK_PLACEHOLDER, 1, unaryParams);
            PipeBarrier<PIPE_V>();
            
            Cast<half, float, false>(dstTensor[i * splitSize], tmpTensor, 
                                     RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, 
                                     {1, 1, HALF_DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            PipeBarrier<PIPE_V>();
        }
        
        if (calcTail > 0) {
            SetVectorMask<T, MaskMode::COUNTER>(0, calcTail);
            Cast<float, half, false>(tmpTensor, srcTensor[loopCount * splitSize], 
                                     RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, 
                                     {1, 1, DEFAULT_REPEAT_STRIDE, HALF_DEFAULT_REPEAT_STRIDE});
            PipeBarrier<PIPE_V>();
            
            Adds<float, false>(tmpTensor, tmpTensor, 1.0, MASK_PLACEHOLDER, 1, unaryParams);
            PipeBarrier<PIPE_V>();
            
            Ln<float, false>(tmpTensor, tmpTensor, MASK_PLACEHOLDER, 1, unaryParams);
            PipeBarrier<PIPE_V>();
            
            Cast<half, float, false>(dstTensor[loopCount * splitSize], tmpTensor, 
                                     RoundMode::CAST_NONE, MASK_PLACEHOLDER, 1, 
                                     {1, 1, HALF_DEFAULT_REPEAT_STRIDE, DEFAULT_REPEAT_STRIDE});
            PipeBarrier<PIPE_V>();
        }
    } else {
        const UnaryRepeatParams unaryParams;
        Adds<T, false>(dstTensor, srcTensor, static_cast<T>(1.0), MASK_PLACEHOLDER, 1, unaryParams);
        PipeBarrier<PIPE_V>();
        Ln<T, false>(dstTensor, dstTensor, MASK_PLACEHOLDER, 1, unaryParams);
        PipeBarrier<PIPE_V>();
    }
    
    SetMaskNorm();
    SetVectorMask<half, MaskMode::NORMAL>(FULL_MASK, FULL_MASK);
#endif
}

} // namespace AscendC

#endif

#endif // IMPL_MATH_LOG1P_LOG1P_COMMON_IMPL_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_COMMON_IMPL_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_MATH_LOG1P_LOG1P_COMMON_IMPL_H__
#endif