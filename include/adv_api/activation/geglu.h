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
 * \file geglu.h
 * \brief
 */

#if defined(__NPU_COMPILER_INTERNAL_PURE_SIMT__)
#error "geglu.h cannot be used with compile flag --enable-simt enabled."
#endif

#if !defined(__ASCENDC_INCLUDE_INTERNAL_HEADERS__)
#define __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#define __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GEGLU_H__
#endif

#ifndef LIB_ACTIVATION_GEGLU_H
#define LIB_ACTIVATION_GEGLU_H

#include "kernel_tensor.h"
#if defined(__NPU_ARCH__) && \
    (__NPU_ARCH__ == 3510 || __NPU_ARCH__ == 5102 || __NPU_ARCH__ == 3003 || __NPU_ARCH__ == 3113)
#include "../../../impl/adv_api/detail/activation/geglu/geglu_3510_impl.h"
#else
#include "../../../impl/adv_api/detail/activation/geglu/geglu_common_impl.h"
#endif
namespace AscendC {
#pragma begin_pipe(V)
/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 * \param [in] calCount, amount of data to be calculated
 */
// GeGLU(x1,x2) = x1*GeLU(x2), x1 is src0, x2 is src1
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.geglu_interface
 * @override_id GeGLU__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief GeGLU是采用GELU作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于GeGLU内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetGeGLUMaxMinTmpSize](GetGeGLUMaxMinTmpSize.md)。
 * @param [in] calCount 实际计算数据元素个数。
 */
__aicore__ inline void GeGLU(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    const LocalTensor<uint8_t>& sharedTmpBuffer, uint32_t calCount)
{
    GeGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, sharedTmpBuffer, calCount);
}

/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] calCount, amount of data to be calculated
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.geglu_interface
 * @override_id GeGLU__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_u32
 * @archs 3510 2201 2002 x90 9030
 * @brief GeGLU是采用GELU作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] calCount 实际计算数据元素个数。
 */
__aicore__ inline void GeGLU(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    uint32_t calCount)
{
    // Only for AI Vector Core.
    if (g_coreType == AIC) {
        return;
    }

    GeGLUImpl<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, calCount);
}

/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 * \param [in] sharedTmpBuffer, input local temporary Tensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.geglu_interface
 * @override_id GeGLU__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_u8
 * @archs 3510 2201 2002 x90 9030
 * @brief GeGLU是采用GELU作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 * @param [in] sharedTmpBuffer 临时缓存。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。用于GeGLU内部复杂计算时存储中间变量，由开发者提供。临时空间大小BufferSize的获取方式请参考[GetGeGLUMaxMinTmpSize](GetGeGLUMaxMinTmpSize.md)。
 */
__aicore__ inline void GeGLU(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1,
    const LocalTensor<uint8_t>& sharedTmpBuffer)
{
    GeGLU<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, sharedTmpBuffer, srcTensor0.GetSize());
}
/* !
 * \note support data type: half and float
 *
 * \param [out] dstTensor, output LocalTensor
 * \param [in] srcTensor0, input0 LocalTensor
 * \param [in] srcTensor1, input1 LocalTensor
 */
template <typename T, bool isReuseSource = false>
/**
 * @ingroup simd_api.adv_api.activation_functions.geglu_interface
 * @override_id GeGLU__RCltg1x2_pT_RCltg1x2_pT_RCltg1x2_pT
 * @archs 3510 2201 2002 x90 9030
 * @brief GeGLU是采用GELU作为激活函数的GLU变体。
 * @param [out] dstTensor 目的操作数。类型为[LocalTensor](../../../basic_api/data_structures/LocalTensor/LocalTensor.md)，支持的TPosition为VECIN/VECCALC/VECOUT。
 */
__aicore__ inline void GeGLU(
    const LocalTensor<T>& dstTensor, const LocalTensor<T>& srcTensor0, const LocalTensor<T>& srcTensor1)
{
    GeGLU<T, isReuseSource>(dstTensor, srcTensor0, srcTensor1, srcTensor0.GetSize());
}
#pragma end_pipe
} // namespace AscendC
#endif

#if defined(__UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GEGLU_H__)
#undef __ASCENDC_INCLUDE_INTERNAL_HEADERS__
#undef __UNDEF_ASCENDC_INCLUDE_INTERNAL_HEADERS_GEGLU_H__
#endif
