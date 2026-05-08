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
 * \file log1p_check.h
 * \brief API check entry point for log1p function
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#pragma message( \
    "impl/adv_api/detail/api_check/kernel_check/math/log1p/log1p_check.h is an internal header file and must not be used directly. Functions or variables defined in this file may be removed in the future. Please use \"#include \"adv_api/math/log1p.h\"\" and use public functions or variables defined in interface headers files.")
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_CHECK_H__
#endif

#ifndef IMPL_API_CHECK_KERNEL_CHECK_MATH_LOG1P_LOG1P_CHECK_H_
#define IMPL_API_CHECK_KERNEL_CHECK_MATH_LOG1P_LOG1P_CHECK_H_

#include "log1p_check_aicore.h"

namespace AscendC {
namespace HighLevelApiCheck {

template <typename T, bool isReuseSource = false>
__aicore__ inline void CheckFuncLog1p(
    __gm__ const char* apiName, const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, uint32_t calCount)
{
    CheckFuncClassLog1p<T, isReuseSource> checkFun(apiName);
    checkFun.VerifyingParameters(dstTensor, srcTensor, calCount);
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void CheckFuncLog1p(
    __gm__ const char* apiName, const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    CheckFuncClassLog1p<T, isReuseSource> checkFun(apiName);
    checkFun.VerifyingParameters(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

} // namespace HighLevelApiCheck
} // namespace AscendC

#endif // IMPL_API_CHECK_KERNEL_CHECK_MATH_LOG1P_LOG1P_CHECK_H_

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_CHECK_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_LOG1P_CHECK_H__
#endif