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
 * \file bessel_i0.h
 * \brief Defines BesselI0 elementwise high-level API for experimental prims.bessel_i0.
 */

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__
#endif

#ifndef LIB_MATH_BESSEL_I0_H
#define LIB_MATH_BESSEL_I0_H

#include "kernel_operator.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "../../../impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
/*!
 * \ingroup BesselI0
 * \brief Computes modified Bessel function of the first kind, order 0, elementwisely.
 * \tparam T: Input and output data type. Only float is supported in this experimental version.
 * \tparam isReuseSource: Kept for API compatibility. The implementation does not modify srcTensor.
 * \param [out] dstTensor: output LocalTensor.
 * \param [in] srcTensor: input LocalTensor.
 * \param [in] calCount: number of valid elements to process.
 * \note Only AI Vector Core on dav-3510 is supported. src/dst tensors must be 32B aligned.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(SupportType<T, float>(), "BesselI0 only supports float in this experimental version.");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
    BesselI0Impl<T, isReuseSource>(dstTensor, srcTensor, calCount);
#endif
}

/*!
 * \ingroup BesselI0
 * \brief Computes BesselI0 elementwisely for the whole source tensor.
 * \tparam T: Input and output data type. Only float is supported in this experimental version.
 * \tparam isReuseSource: Kept for API compatibility. The implementation does not modify srcTensor.
 * \param [out] dstTensor: output LocalTensor.
 * \param [in] srcTensor: input LocalTensor.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    BesselI0<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}
#pragma end_pipe
} // namespace AscendC

#endif // LIB_MATH_BESSEL_I0_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__
#endif

