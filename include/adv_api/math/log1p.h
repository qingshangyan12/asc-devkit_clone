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
 * \file log1p.h
 * \brief Public API for log1p function - calculates log(1+x) with high precision
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__
#endif

#ifndef LIB_MATH_LOG1P_H
#define LIB_MATH_LOG1P_H

#include "kernel_tensor.h"
#include "../../../impl/adv_api/detail/math/log1p/log1p_common_impl.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                               __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)

namespace AscendC {

#pragma begin_pipe(V)

/**
 * @brief Compute log(1+x) element-wise with high precision
 * @ingroup Log1p
 * 
 * For small values of x (|x| < threshold), uses Taylor expansion for better precision:
 * log(1+x) ≈ x - x²/2 + x³/3 - x⁴/4 + ...
 * 
 * For larger values, uses standard logarithm computation.
 * 
 * @param [out] dstTensor Output LocalTensor containing log(1+x) values
 * @param [in] srcTensor Input LocalTensor containing x values
 * @param [in] calCount Number of elements to compute
 * @note Supported data types: half, float
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
                              uint32_t calCount)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    Log1pImplAPI<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

/**
 * @brief Compute log(1+x) element-wise for entire tensor
 * @ingroup Log1p
 * 
 * @param [out] dstTensor Output LocalTensor
 * @param [in] srcTensor Input LocalTensor
 * @note Computes log(1+x) for all elements in srcTensor
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Log1p<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

/**
 * @brief Compute log(1+x) with temporary buffer
 * @ingroup Log1p
 * 
 * @param [out] dstTensor Output LocalTensor
 * @param [in] srcTensor Input LocalTensor
 * @param [in] sharedTmpBuffer Temporary buffer for computation (required for half type)
 * @param [in] calCount Number of elements to compute
 * @note The sharedTmpBuffer is only used for half type on certain architectures
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
                              const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102)
    if ASCEND_IS_AIC {
        return;
    }
#endif
    Log1pImplAPI<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

/**
 * @brief Compute log(1+x) with temporary buffer for entire tensor
 * @ingroup Log1p
 * 
 * @param [out] dstTensor Output LocalTensor
 * @param [in] srcTensor Input LocalTensor
 * @param [in] sharedTmpBuffer Temporary buffer for computation
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
                              const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    Log1p<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

#pragma end_pipe

} // namespace AscendC

#endif

#endif // LIB_MATH_LOG1P_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__
#endif