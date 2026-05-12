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
 * \brief
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__
#endif

#ifndef LIB_MATH_LOG1P_H
#define LIB_MATH_LOG1P_H
#include "kernel_tensor.h"
#include "../../../impl/adv_api/detail/math/log1p/log1p_3510_impl.h"

namespace AscendC {

#pragma begin_pipe(V)
/*
 * @brief dst = log1p(src)
 * @ingroup Log1p
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor, input LocalTensor
 * @param [in] calCount, amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    Log1pImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

/*
 * @brief dst = log1p(src)
 * @ingroup Log1p
 * @param [out] dstTensor, output LocalTensor
 * @param [in] srcTensor, input LocalTensor
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Log1p(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Log1p<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}

#pragma end_pipe
}  // namespace AscendC

#endif  // LIB_MATH_LOG1P_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_H__
#endif
