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
 * \file swiglu.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "swiglu.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SWIGLU_H__
#endif

#ifndef LIB_MATH_SWIGLU_H
#define LIB_MATH_SWIGLU_H

#include "kernel_tensor.h"
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/activation/swiglu/swiglu_3510_impl.h"
#else
#include "../../../impl/adv_api/detail/activation/swiglu/swiglu_common_impl.h"
#endif
namespace AscendC {
#pragma begin_pipe(V)
/* !
 * \note support data type: half and float
 *  Function�?
    swish(x) = x / (1 + e^(-βx))
    x1 = 1 + e^(-βx)
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] scalarValue, input scalar
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.swiglu_interface
 * @override_id SwiGLU__Rltg1x2_pT_Rltg1x2_pT_Rltg1x2_pT_RCf32
 * @archs 3510 2201 2002 x90 9030
 * @brief SwiGLU是采用Swish作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] scalarValue 激活函数中的β参数。
 */
__aicore__ inline void SwiGLU(
    LocalTensor<T>& dstTensor, LocalTensor<T>& srcTensor0, LocalTensor<T>& srcTensor1, const float& scalarValue)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    SwiGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, scalarValue);
}

/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] scalarValue, input scalar
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.swiglu_interface
 * @override_id SwiGLU__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCf32_RCltg1x2_u8
 * @archs 3510 2201 2002 x90 9030
 * @brief SwiGLU是采用Swish作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] scalarValue 激活函数中的β参数。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于SwiGLU内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetSwiGLUMaxMinTmpSize](GetSwiGLUMaxMinTmpSize.md)。
 */
__aicore__ inline void SwiGLU(
    LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    const float& scalarValue, const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    SwiGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, scalarValue, sharedTmpBuffer);
}

/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] scalarValue, input scalar
 * \param [in] calCount, amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.swiglu_interface
 * @override_id SwiGLU__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCf32_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief SwiGLU是采用Swish作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] scalarValue 激活函数中的β参数。
 * @param [in] calCount 实际计算数据元素个数。
 */
__aicore__ inline void SwiGLU(
    LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    const float& scalarValue, const uint32_t calCount)
{
    // Only for AI Vector Core.
    if ASCEND_IS_AIC {
        return;
    }

    SwiGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, scalarValue, calCount);
}

/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] scalarValue, input scalar
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] calCount, amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.swiglu_interface
 * @override_id SwiGLU__Rltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCf32_RCltg1x2_u8_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief SwiGLU是采用Swish作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] scalarValue 激活函数中的β参数。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于SwiGLU内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetSwiGLUMaxMinTmpSize](GetSwiGLUMaxMinTmpSize.md)。
 * @param [in] calCount 实际计算数据元素个数。
 */
__aicore__ inline void SwiGLU(
    LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    const float& scalarValue, const LocalTensor<uint8_t>& sharedTmpBuffer, const uint32_t calCount)
{
    SwiGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, scalarValue, sharedTmpBuffer, calCount);
}
#pragma end_pipe
} // namespace AscendC
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SWIGLU_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_SWIGLU_H__
#endif
