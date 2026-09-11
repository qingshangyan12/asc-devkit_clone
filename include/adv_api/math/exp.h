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
 * \file exp.h
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "exp.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_EXP_H__
#endif

#ifndef LIB_MATH_EXP_H
#define LIB_MATH_EXP_H

#include "kernel_tensor.h"

#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/math/exp/exp_3510_impl.h"
#else
#include "../../../impl/adv_api/detail/math/exp/exp_common_impl.h"
#endif
#endif

namespace AscendC {

#pragma begin_pipe(V)

/*!
 * \ingroup Exp
 * \brief Use Taylor Formula for Exp calculation
 *        exp(a) = (e^b) * (e^c) b is integer part of a, c is decimal part of a.  a = b + c
 *        e^c = 1 + c + c^2 / (2!) + .... + c^n / n!
 * \tparam T: Data type to be calculated, half or float
 * \tparam taylorExpandLevel: The number of Taylor formula terms (n above)
 * \tparam isReuseSource: Whether to reuse the buffer of srcTensor.
 *         If the value is true, srcTensor can be used as tmpBuffer and the data in it will be overwritten.
 *         If the value is false, srcTensor will not be used as tmpBuffer for calculation.
 * \param [out] dstLocal: Output localTensor.
 * \param [in] srcLocal: Input localTensor
 * \param [in] calCount: The number of elements to be processed.
 */
template <typename T, uint8_t taylorExpandLevel, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.exp_interface
 * @override_id Exp__RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201
 * @brief 按元素取自然指数。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Exp(const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(
        (std::is_same<T, float>::value || std::is_same<T, half>::value),
        "Failed to check the data types, current api support data types are half/float.");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ExpAPI::ExpImpl<T, taylorExpandLevel, isReuseSource>(dstLocal, srcLocal, calCount);
#endif
}

/*!
 * \ingroup Exp
 * \brief Use Taylor Formula for Exp calculation
 *        exp(a) = (e^b) * (e^c) b is integer part of a, c is decimal part of a.  a = b + c
 *        e^c = 1 + c + c^2 / (2!) + .... + c^n / n!
 * \tparam T: Data type to be calculated, half or float
 * \tparam taylorExpandLevel: The number of Taylor formula terms (n above)
 * \tparam isReuseSource: Whether to reuse the buffer of srcTensor.
 *         If the value is true, srcTensor can be used as tmpBuffer and the data in it will be overwritten.
 *         If the value is false, srcTensor will not be used as tmpBuffer for calculation.
 * \param [out] dstLocal: Output localTensor.
 * \param [in] srcLocal: Input localTensor
 * \param [in] sharedTmpBuffer：extra temporary shared space used for intermediate values among calculation process,
 *             whose required space size should refer to corresponding tiling API, which is defined at exp_tiling.h.
 *             Generally, the more space you allocate, the better performance you will achieve, and the performance
 *             reaches peak when buffer size is maximum(calculated by tiling function). Moreover, it is not guaranteed
 *             that the shared space will be cleared after usage, the data could be anything.
 * \param [in] calCount: The number of elements to be processed.
 */
template <typename T, uint8_t taylorExpandLevel, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.math_compute.exp_interface
 * @override_id Exp__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201
 * @brief 按元素取自然指数。
 * @param [out] dstLocal 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] srcLocal 源操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。源操作数的数据类型需要与目的操作数保持一致。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于Exp内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetExpMaxMinTmpSize](GetExpMaxMinTmpSize.md)。
 * @param [in] calCount 参与计算的元素个数。
 */
__aicore__ inline void Exp(
    const LocalTensor<T>& dstLocal, const LocalTensor<T>& srcLocal, const LocalTensor<uint8_t>& sharedTmpBuffer,
    const uint32_t calCount)
{
    if ASCEND_IS_AIC {
        return;
    }
    static_assert(
        (std::is_same<T, float>::value || std::is_same<T, half>::value),
        "Failed to check the data types, current api support data types are half/float.");
#if defined(__NPU_ARCH__) && (__NPU_ARCH__ == 2201 || __NPU_ARCH__ == 2002 || __NPU_ARCH__ == 3510 || \
                              __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
    ExpAPI::ExpImpl<T, taylorExpandLevel, isReuseSource>(dstLocal, srcLocal, sharedTmpBuffer, calCount);
#endif
}

#pragma end_pipe
} // namespace AscendC
#endif // LIB_MATH_EXP_H

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_EXP_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_EXP_H__
#endif
