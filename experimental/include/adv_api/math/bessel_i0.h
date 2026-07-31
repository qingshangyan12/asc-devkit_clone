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
 * \brief Defines a series of interface used to do elementwise math BesselI0 calculation.
 * Formula: the modified Bessel function of the first kind, order 0:
 *   BesselI0(x) = I0(x) = sum_{k>=0} (1/(k!)^2) * (x/2)^(2k)
 * It is even. There is no elementary closed form, so it is computed by the Abramowitz & Stegun
 * two-region rational approximation on a = |x| (branch point a = 3.75):
 *   region1 (a <= 3.75): I0 = P1((a/3.75)^2)
 *   region2 (a >  3.75): I0 = exp(a-3.75) * (P2S(3.75/a) / sqrt(a)),  P2S = P2 * e^3.75
 * Aligns with PyTorch torch.special.i0 / prims.bessel_i0.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "bessel_i0.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__
#endif

#ifndef LIB_MATH_BESSEL_I0_H
#define LIB_MATH_BESSEL_I0_H

#include "kernel_tensor.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "../../../impl/adv_api/detail/math/bessel_i0/bessel_i0_3510_impl.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/*!
 * \ingroup BesselI0
 * \brief compute BesselI0 elementwisely
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] sharedTmpBuffer: extra temporary shared space used for intermediate values among calculation process,
 *             whose required space size should refer to corresponding tiling API, which is defined at
 *             bessel_i0_tiling.h. On this device the computation is register resident, so no temporary space is
 *             required and this buffer is not accessed; it is accepted for interface parity.
 * \param [in] calCount: the number of elements to be processed.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst/sharedTmpBuffer tensor address overlap.
 * \note boundary handling: x==0 -> 1; +-Inf -> NaN; NaN -> NaN (aligns with scipy.special.i0 and torch.i0).
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    BesselI0Impl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

/*!
 * \ingroup BesselI0
 * \brief compute BesselI0 elementwisely for whole source tensor
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] sharedTmpBuffer: extra temporary shared space, see the overload above.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst/sharedTmpBuffer tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    BesselI0<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

/*!
 * \ingroup BesselI0
 * \brief compute BesselI0 elementwisely
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \param [in] calCount: the number of elements to be processed.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    BesselI0Impl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

/*!
 * \ingroup BesselI0
 * \brief compute BesselI0 elementwisely for whole source tensor
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    BesselI0<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}
#else
/*!
 * \ingroup BesselI0
 * \brief BesselI0 is only supported on Ascend 950PR (__NPU_ARCH__ == 3510).
 * These empty definitions keep call sites compilable on other architectures and in the host
 * compilation pass, where the register-level intrinsics do not exist.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)sharedTmpBuffer;
    (void)calCount;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)sharedTmpBuffer;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)calCount;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void BesselI0(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    (void)dstTensor;
    (void)srcTensor;
}
#endif
#pragma end_pipe
} // namespace AscendC
#endif // LIB_MATH_BESSEL_I0_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_BESSEL_I0_H__
#endif
