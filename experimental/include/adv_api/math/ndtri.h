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
 * \file ndtri.h
 * \brief Defines a series of interface used to do elementwise math Ndtri calculation.
 * Formula: inverse of the standard normal cumulative distribution function (the probit / quantile
 * function). Given a probability p in (0,1), Ndtri returns the z such that P(Z <= z) = p, i.e.
 *   Ndtri(p) = Phi^-1(p) = sqrt(2) * erfinv(2p - 1)
 * There is no elementary closed form, so it is computed by a piecewise rational approximation:
 *   central (0.02425 <= p <= 0.97575): z = sqrt(2*pi) * q * (1 + r*P0(r)/Q0(r)),  q = p-0.5, r = q*q
 *   tail    (otherwise)              : z = +-C(u)/D(u),  u = sqrt(-2*ln(min(p, 1-p)))
 * Aligns with PyTorch torch.special.ndtri / prims.ndtri.
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "ndtri.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_NDTRI_H__
#endif

#ifndef LIB_MATH_NDTRI_H
#define LIB_MATH_NDTRI_H

#include "kernel_tensor.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
#include "../../../impl/adv_api/detail/math/ndtri/ndtri_3510_impl.h"
#endif

namespace AscendC {
#pragma begin_pipe(V)
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 3510)
/*!
 * \ingroup Ndtri
 * \brief compute Ndtri elementwisely
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor, probabilities in (0, 1)
 * \param [in] sharedTmpBuffer: extra temporary shared space used for intermediate values among calculation process,
 *             whose required space size should refer to corresponding tiling API, which is defined at ndtri_tiling.h.
 *             On this device the computation is register resident, so no temporary space is required and this
 *             buffer is not accessed; it is accepted for interface parity.
 * \param [in] calCount: the number of elements to be processed.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst/sharedTmpBuffer tensor address overlap.
 * \note boundary handling: p<0 or p>1 -> NaN; p==0 -> -Inf; p==1 -> +Inf; p==0.5 -> 0; NaN -> NaN.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    NdtriImpl<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, calCount);
}

/*!
 * \ingroup Ndtri
 * \brief compute Ndtri elementwisely for whole source tensor
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor, probabilities in (0, 1)
 * \param [in] sharedTmpBuffer: extra temporary shared space, see the overload above.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst/sharedTmpBuffer tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    Ndtri<T, isReuseSource>(dstTensor, srcTensor, sharedTmpBuffer, srcTensor.GetSize());
}

/*!
 * \ingroup Ndtri
 * \brief compute Ndtri elementwisely
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor, probabilities in (0, 1)
 * \param [in] calCount: the number of elements to be processed.
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    NdtriImpl<T, isReuseSource>(dstTensor, srcTensor, calCount);
}

/*!
 * \ingroup Ndtri
 * \brief compute Ndtri elementwisely for whole source tensor
 * \tparam T: float
 * \tparam isReuseSource: whether allows API to modify source data, usually for performance reason
 * \param [out] dstTensor: output LocalTensor
 * \param [in] srcTensor: input LocalTensor, probabilities in (0, 1)
 * \note src/dst Tensor must be 32B aligned, and it doesn't allow src/dst tensor address overlap.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    Ndtri<T, isReuseSource>(dstTensor, srcTensor, srcTensor.GetSize());
}
#else
/*!
 * \ingroup Ndtri
 * \brief Ndtri is only supported on Ascend 950PR (__NPU_ARCH__ == 3510).
 * These empty definitions keep call sites compilable on other architectures and in the host
 * compilation pass, where the register-level intrinsics do not exist.
 */
template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)sharedTmpBuffer;
    (void)calCount;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)sharedTmpBuffer;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor, const uint32_t calCount)
{
    (void)dstTensor;
    (void)srcTensor;
    (void)calCount;
}

template <typename T, bool isReuseSource = false>
__aicore__ inline void Ndtri(const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor)
{
    (void)dstTensor;
    (void)srcTensor;
}
#endif
#pragma end_pipe
} // namespace AscendC
#endif // LIB_MATH_NDTRI_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_NDTRI_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_NDTRI_H__
#endif
